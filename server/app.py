import hashlib
import os
import shutil

import peewee
from flask import Flask, jsonify, request, send_file
from peewee import *
from itsdangerous import (TimedJSONWebSignatureSerializer as URLSafeSerializer, BadSignature, SignatureExpired)
from functools import wraps
from flask_cors import CORS, cross_origin
from playhouse.shortcuts import model_to_dict, dict_to_model

from DAO import *

app = Flask(__name__)
app.config.from_object('config')
# 允许跨域
CORS(app)


def verify_auth_token(token):
    s = URLSafeSerializer(app.config['SECRET_KEY'])
    try:
        data = s.loads(token)
    except SignatureExpired:
        return None
    except BadSignature:
        return None
    return 'key' in data and data['key'] == app.config['SECRET_KEY']


def test_authorization():
    cookies = request.cookies
    headers = request.headers
    args = request.args
    token = None
    if 'token' in cookies:
        token = cookies['token']
    elif 'Authorization' in headers:
        token = headers['Authorization']
    elif 'token' in args:
        token = args['token']
    else:
        return False
    return verify_auth_token(token)


def authorization_required(f):
    @wraps(f)
    def wrapper(*args, **kwargs):
        if not test_authorization():
            return jsonify(message='unauthorized'), 401
        return f(*args, **kwargs)

    return wrapper


def generate_filename(folder, filename):
    return hashlib.md5((folder + '_' + filename).encode('utf-8')).hexdigest()


def base36_encode(number):
    assert number >= 0, 'positive integer required'
    if number == 0:
        return '0'
    base36 = []
    while number != 0:
        number, i = divmod(number, 36)
        base36.append('0123456789abcdefghijklmnopqrstuvwxyz'[i])
    return ''.join(reversed(base36))


# def generate_url():
#     return base36_encode(get_random_long_int())


@app.route('/login', methods=['POST'])
def login():
    req = request.get_json()
    # 验证用户名密码
    if req['email'] == app.config['EMAIL'] and req['password'] == app.config['PASSWORD']:
        # 生成 Token，有效期为一周
        s = URLSafeSerializer(app.config['SECRET_KEY'], expires_in=7 * 24 * 3600)

        return jsonify(message='OK',
                       token=s.dumps({'key': app.config['SECRET_KEY']}).decode('utf-8'))
    else:
        return jsonify(message='unauthorized'), 401


@app.route('/auth', methods=['GET'])
@authorization_required
def auth():
    return jsonify(message='OK')


@app.route('/folders', methods=['GET', 'POST'])
# TODO: @authorization_required
def folders():
    if request.method == 'POST':
        req = request.get_json()
        foldername = req['name']
        parentid = req['parentid']
        folder = Folder.get_by_id(parentid)
        folderpath = foldername
        if parentid != 0:
            folder = Folder.get_by_id(parentid)
            folderpath = folder.path + '/' + foldername
        target_folder = os.path.join(os.path.expanduser(app.config['UPLOAD_FOLDER']), folderpath)
        if os.path.exists(target_folder):
            return jsonify(message='error'), 409
        try:
            os.mkdir(target_folder)
            f = Folder.create(name=foldername, path=folderpath, parentid=parentid)
            f.save()
            return jsonify(message='OK'), 201
        except peewee.IntegrityError as e:
            return jsonify(message="error"), 409
        except Exception as e:
            app.logger.exception(e)
            return jsonify(message="error"), 409
    if request.method == 'GET':
        query = Folder.select()
        if query.exists():
            return jsonify(message='ok', data=[model_to_dict(folder) for folder in query])
        else:
            return jsonify(message='ok', data=[])


@app.route('/folders/<folder_id>', methods=['GET', 'POST', 'DELETE'])
# TODO: @authorization_required
def folder(folder_id):
    try:
        folder = Folder.get_by_id(folder_id)
    except peewee.DoesNotExist:
        return jsonify(message='error'), 404
    if request.method == 'GET':
        subfolders = Folder.select().where(Folder.parentid == folder.id)
        files = File.select().where(File.folderid == folder_id)
        subfolderList = []
        for subfolder in subfolders:
            if subfolder.id != 0:
                subfolderList.append(model_to_dict(subfolder))
        return jsonify(message='OK', data=model_to_dict(folder, backrefs=True),
                       subfolders=subfolderList,
                       files=[model_to_dict(file) for file in files])
    if request.method == 'DELETE':
        folderpath = folder.path
        target_file = os.path.join(os.path.expanduser(app.config['UPLOAD_FOLDER']), folderpath)
        try:
            # 递归删除数据库函数
            def recuesiondel(folder_id):
                files = File.select().where(File.folderid == folder_id)
                for file in files:
                    file.delete_instance()
                subfolders = Folder.select().where(Folder.parentid == folder_id)
                for subfolder in subfolders:
                    recuesiondel(subfolder.id)
                    subfolder.delete_instance()

            # 递归删除数据库
            recuesiondel(folder_id)
            # 删除文件夹
            shutil.rmtree(target_file)
            # 数据库删除自身
            folder.delete_instance()

        except peewee.IntegrityError:
            return jsonify(message='error'), 409
        except Exception as e:
            app.logger.exception(e)
            return jsonify(message='error'), 409
    if request.method == 'POST':
        file = request.files['file']
        if file:
            # actual_filename = generate_filename(folder_name, file.name)
            filepath = file.filename
            if folder_id != '0':
                filepath = folder.path + '/' + file.filename
            target_file = os.path.join(os.path.expanduser(app.config['UPLOAD_FOLDER']), filepath)
            print(folder_id)
            print(filepath)
            print(target_file)
            if os.path.exists(target_file):
                return jsonify(message='error'), 409
            try:
                file.save(target_file)
                f = File.create(folder=folder, name=file.filename, folderid=folder_id)
                f.save()
            except Exception as e:
                app.logger.exception(e)
                return jsonify(message='error'), 500
    return jsonify(message='OK')


@app.route('/files/<file_id>', methods=['GET', 'DELETE'])
# TODO: @authorization_required
def files(file_id):
    # actual_filename = generate_filename(folder_id, filename)

    try:
        file = File.get_by_id(file_id)
    except peewee.DoesNotExist:
        return jsonify(message='error'), 404
    actual_path = file.name
    if file.folderid != 0:
        folderpath = Folder.select(Folder.path).where(Folder.id == file.folderid)
        actual_path = folderpath + '/' + file.name
    target_file = os.path.join(os.path.expanduser(app.config['UPLOAD_FOLDER']), actual_path)
    if request.method == 'GET':
        args = request.args
        if 'query' in args and args['query'] == 'info':
            return jsonify(message='OK', data=model_to_dict(file), actual_path=actual_path)
        if os.path.exists(target_file):
            return send_file(target_file)
        else:
            return jsonify(message='error'), 404

    if request.method == 'DELETE':
        if os.path.exists(target_file):
            try:
                file.delete_instance()
                os.remove(target_file)
                return jsonify(message='OK')
            except Exception as e:
                app.logger.exception(e)
                return jsonify(message='error'), 500
        else:
            return jsonify(message='error'), 404


if __name__ == '__main__':
    app.run(debug=True)
