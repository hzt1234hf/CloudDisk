import hashlib
import os
import random
import shutil
import datetime
import base64
import threading

from Crypto.Cipher import AES  # 安装依赖pycryptodome，不是Crypto
from Crypto.Util import Counter
import peewee
from flask import Flask, jsonify, request, send_file
from peewee import *
from itsdangerous import (TimedJSONWebSignatureSerializer as URLSafeSerializer, BadSignature, SignatureExpired)
from functools import wraps
from flask_cors import CORS, cross_origin
from playhouse.shortcuts import model_to_dict, dict_to_model
import asyncio

from watchdog.observers import Observer
from watchdog.events import *
from watchdog.utils.dirsnapshot import DirectorySnapshot, DirectorySnapshotDiff

from DAO import *
from tcp_app import *

app = Flask(__name__)
app.config.from_object('config')
# 允许跨域
CORS(app, supports_credentials=True)
SecreyKey = app.config['SECRET_KEY']
DesKey = app.config['DES_KEY']
ClouDiskPath = os.path.abspath(app.config['UPLOAD_FOLDER'])
UpdateInterval = app.config['UPDATE_INTERVAL']
UpdateMaxInterval = app.config['UPDATE_MAX_INTERVAL']
Encrypt_File_Mode = app.config['ENCRYPT_FILE_MODE']


class FileEventHandler(FileSystemEventHandler):
    def __init__(self, cloud_disk_path):
        FileSystemEventHandler.__init__(self)
        self.cloud_disk_path = cloud_disk_path
        self.timer_task = None
        self.last_snapshot = DirectorySnapshot(self.cloud_disk_path)
        self.cancel_count = 0

    def on_any_event(self, event):
        if self.timer_task:
            self.timer_task.cancel()
        self.timer_task = threading.Timer(UpdateInterval, self.check_snapshot)
        self.timer_task.start()

    def check_snapshot(self):
        snapshot = DirectorySnapshot(self.cloud_disk_path)
        diff = DirectorySnapshotDiff(self.last_snapshot, snapshot)
        self.last_snapshot = snapshot
        self.timer_task = None

        print("files_created:", diff.files_created)
        print("files_deleted:", diff.files_deleted)
        print("files_modified:", diff.files_modified)
        print("files_moved:", diff.files_moved)
        print("dirs_modified:", diff.dirs_modified)
        print("dirs_moved:", diff.dirs_moved)
        print("dirs_deleted:", diff.dirs_deleted)
        print("dirs_created:", diff.dirs_created)


class DirMonitor(object):

    def __init__(self, cloud_disk_path):
        self.cloud_disk_path = cloud_disk_path
        self.observer = Observer()

    def start(self):
        event_handler = FileEventHandler(self.cloud_disk_path)
        print(event_handler)
        self.observer.schedule(event_handler, self.cloud_disk_path, True)
        self.observer.start()

    def stop(self):
        self.observer.stop()


def verify_auth_token(token):
    s = URLSafeSerializer(SecreyKey)
    try:
        data = s.loads(token)
    except SignatureExpired:
        return None
    except BadSignature:
        return None
    return 'key' in data and data['key'] == SecreyKey


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


def generate_path(fid, pfolder, pchild):
    actual_path = pchild
    if type(fid) == str:
        try:
            fid = int(fid)
        except Exception  as e:
            return actual_path
    if fid != 0:
        actual_path = pfolder + '/' + pchild
    return actual_path


def generate_foldername(folderid, foldername):
    return hashlib.md5(("foldid" + str(folderid) + "foldername" + foldername).encode('utf-8')).hexdigest()


def generate_filename(folderid, filename):
    return hashlib.md5(("foldid" + str(folderid) + "filename" + filename).encode('utf-8')).hexdigest()


def AES_Encrypt(data):
    vi = '0918273645abcdef'
    pad = lambda s: s + (16 - len(s) % 16) * chr(16 - len(s) % 16)
    data = pad(data)
    cipher = AES.new(DesKey.encode('utf8'), AES.MODE_CBC, vi.encode('utf8'))
    encryptedbytes = cipher.encrypt(data.encode('utf8'))
    encodestrs = base64.urlsafe_b64encode(encryptedbytes)
    enctext = encodestrs.decode('utf8')
    return enctext


def AES_Decrypt(data):
    vi = '0918273645abcdef'
    data = data.encode('utf8')
    encodebytes = base64.urlsafe_b64decode(data)
    cipher = AES.new(DesKey.encode('utf8'), AES.MODE_CBC, vi.encode('utf8'))
    text_decrypted = cipher.decrypt(encodebytes)
    unpad = lambda s: s[0:-s[-1]]
    text_decrypted = unpad(text_decrypted)
    text_decrypted = text_decrypted.decode('utf8')
    return text_decrypted


def get_encrypt_url(obj):
    data = ''
    if isinstance(obj, File):
        data = 'fi '
    elif isinstance(obj, Folder):
        data = 'fo '
    else:
        return None
    data += str(obj.id)
    url = AES_Encrypt(data)
    return url


def get_decrypt_url(url):
    data = AES_Decrypt(url)
    srcdata = data.split(' ')
    if len(srcdata) != 2:
        return None
    return srcdata


def get_encrypt_file_path(file):
    actual_path = generate_filename(file.parentid, file.name)
    target_file = os.path.join(os.path.expanduser(ClouDiskPath), actual_path)
    return target_file


def get_noencrypt_file_path(file):
    folderpath = Folder.get(Folder.id == file.parentid)
    actual_path = generate_path(file.parentid, folderpath.path, file.name)
    target_file = os.path.join(os.path.expanduser(ClouDiskPath), actual_path)
    return target_file


def get_download_file(file):
    actual_path = generate_filename(file.parentid, file.name)
    target_file = os.path.join(os.path.expanduser(ClouDiskPath), actual_path)
    if not Encrypt_File_Mode:
        folderpath = Folder.get(Folder.id == file.parentid)
        actual_path = generate_path(file.parentid, folderpath.path, file.name)
        target_file = os.path.join(os.path.expanduser(ClouDiskPath), actual_path)
    if os.path.exists(target_file):
        rv = send_file(target_file, as_attachment=True, attachment_filename=file.name)
        return rv
    return None


def base36_encode(number):
    assert number >= 0, 'positive integer required'
    if number == 0:
        return '0'
    base36 = []
    while number != 0:
        number, i = divmod(number, 36)
        base36.append('0123456789abcdefghijklmnopqrstuvwxyz'[i])
    return ''.join(reversed(base36))


def get_random_long_int():
    return random.SystemRandom().randint(1000000000, 9999999999)


def get_random_short_int():
    return random.SystemRandom().randint(100000, 999999)


def generate_password():
    return base36_encode(get_random_short_int())


@app.route('/login', methods=['POST'])
def login():
    req = request.get_json()
    user = User.select().where((User.email == req['email']) & (User.password == req['password'])).count()
    if user:
        # 生成 Token，有效期为一周
        s = URLSafeSerializer(SecreyKey, expires_in=7 * 24 * 3600)

        return jsonify(message='OK',
                       token=s.dumps({'key': SecreyKey}).decode('utf-8'))
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
        if Folder.select().where((Folder.parentid == parentid) & (Folder.name == foldername)).count():
            return jsonify(message='error'), 409
        folder = Folder.get_by_id(parentid)
        folderpath = generate_path(parentid, folder.path, foldername)
        if Encrypt_File_Mode:
            try:
                f = Folder.create(name=foldername, path=folderpath, parentid=parentid, isShared=False,
                                  isShareEncryped=False, sharePassword="", shareUrl="",
                                  sharePeriod=datetime.datetime.now())
                f.save()
                return jsonify(message='OK'), 201
            except peewee.IntegrityError as e:
                return jsonify(message='error'), 409
        else:
            target_folder = os.path.join(os.path.expanduser(ClouDiskPath), folderpath)
            if os.path.exists(target_folder):
                return jsonify(message='error'), 409
            try:
                os.mkdir(target_folder)
                f = Folder.create(name=foldername, path=folderpath, parentid=parentid, isShared=False,
                                  isShareEncryped=False, sharePassword="", shareUrl="",
                                  sharePeriod=datetime.datetime.now())
                f.save()
                return jsonify(message='OK'), 201
            except peewee.IntegrityError as e:
                return jsonify(message='error'), 409
            except Exception as e:
                app.logger.exception(e)
                return jsonify(message='error'), 409
    if request.method == 'GET':
        query = Folder.select()
        if query.exists():
            return jsonify(message='ok', data=[model_to_dict(folder) for folder in query])
        else:
            return jsonify(message='ok', data=[])


@app.route('/folders/<folder_id>', methods=['GET', 'POST', 'DELETE', 'PATCH'])
# TODO: @authorization_required
def folder(folder_id):
    try:
        folder = Folder.get_by_id(folder_id)
    except peewee.DoesNotExist:
        return jsonify(message='error'), 404
    if request.method == 'GET':
        subfolders = Folder.select().where(Folder.parentid == folder.id)
        files = File.select().where(File.parentid == folder_id)
        return jsonify(message='OK', data=model_to_dict(folder, backrefs=True),
                       subfolders=[model_to_dict(folder) for folder in subfolders],
                       files=[model_to_dict(file) for file in files])
    elif request.method == 'DELETE':
        folderpath = folder.path
        try:
            # 递归删除数据库函数
            def recuesiondel(folder_id):
                files = File.select().where(File.parentid == folder_id)
                for file in files:
                    file.delete_instance()
                subfolders = Folder.select().where(Folder.parentid == folder_id)
                for subfolder in subfolders:
                    recuesiondel(subfolder.id)
                    subfolder.delete_instance()

            # 递归删除数据库
            recuesiondel(folder_id)
            if not Encrypt_File_Mode:
                target_file = os.path.join(os.path.expanduser(ClouDiskPath), folderpath)
                # 删除文件夹
                shutil.rmtree(target_file)
            # 数据库删除自身
            folder.delete_instance()

        except peewee.IntegrityError:
            return jsonify(message='error'), 409
        except Exception as e:
            app.logger.exception(e)
            return jsonify(message='error'), 409
    elif request.method == 'POST':
        file = request.files['file']
        if file:
            filepath = generate_path(folder_id, folder.path, file.filename)
            target_file = os.path.join(os.path.expanduser(ClouDiskPath), filepath)
            if Encrypt_File_Mode:
                filepath = generate_filename(folder_id, file.filename)
                target_file = os.path.join(os.path.expanduser(ClouDiskPath), filepath)
            if os.path.exists(target_file):
                return jsonify(message='error'), 409
            try:
                file.save(target_file)
                f = File.create(folder=folder, name=file.filename, parentid=folder_id, isShared=False,
                                isShareEncryped=False, sharePassword="", shareUrl="",
                                sharePeriod=datetime.datetime.now())
                f.save()
            except Exception as e:
                app.logger.exception(e)
                return jsonify(message='error'), 500
    elif request.method == 'PATCH':
        req = request.get_json()
        folder.isShared = req['isShare']
        if folder.isShared:
            folder.isShareEncryped = req['isShareEncryped']
            folder.shareUrl = get_encrypt_url(folder)
            folder.sharePeriod = datetime.datetime.now() + datetime.timedelta(req['sharePeriod'])
            if folder.isShareEncryped:
                # if 'getShareObjInfo' in req:
                #     folder.sharePassword = req['sharePassword']
                # else:
                #     folder.sharePassword = generate_password()
                folder.sharePassword = generate_password()
        folder.save()
        return jsonify(message='OK', data=model_to_dict(folder))
    return jsonify(message='OK')


@app.route('/files/<file_id>', methods=['GET', 'DELETE', 'PATCH'])
# TODO: @authorization_required
def files(file_id):
    try:
        file = File.get_by_id(file_id)
    except peewee.DoesNotExist:
        return jsonify(message='error'), 404
    actual_path = generate_filename(file.parentid, file.name)
    target_file = os.path.join(os.path.expanduser(ClouDiskPath), actual_path)
    if not Encrypt_File_Mode:
        folderpath = Folder.get(Folder.id == file.parentid)
        actual_path = generate_path(file.parentid, folderpath.path, file.name)
        target_file = os.path.join(os.path.expanduser(ClouDiskPath), actual_path)
    if request.method == 'GET':
        args = request.args
        if 'query' in args and args['query'] == 'info':
            return jsonify(message='OK', data=model_to_dict(file), actual_path=actual_path)
        if os.path.exists(target_file):
            return send_file(target_file, as_attachment=True, attachment_filename=file.name)
        else:
            return jsonify(message='error'), 404
    elif request.method == 'DELETE':
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
    elif request.method == 'PATCH':
        req = request.get_json()
        file.isShared = req['isShare']
        if file.isShared:
            file.isShareEncryped = req['isShareEncryped']
            file.shareUrl = get_encrypt_url(file)
            file.sharePeriod = datetime.datetime.now() + datetime.timedelta(req['sharePeriod'])
            if file.isShareEncryped:
                # if 'getShareObjInfo' in req:
                #     file.sharePassword = req['sharePassword']
                # else:
                #     file.sharePassword = generate_password()
                file.sharePassword = generate_password()
        else:
            file.isShareEncryped = False
        file.save()
        return jsonify(message='OK', data=model_to_dict(file))
    return jsonify(message='OK')


@app.route('/share/<path>', methods=['GET'])
def share(path):
    res = get_decrypt_url(path)
    args = request.args
    if res:
        s = URLSafeSerializer(SecreyKey, expires_in=24 * 3600)
        if res[0] == 'fi':
            try:
                file = File.get_by_id(int(res[1]))
            except peewee.DoesNotExist:
                return jsonify(message='error'), 404
            if file.isShared:
                payload = {
                    'path': path,
                    'name': file.name,
                    'isShared': file.isShared,
                    'needPassword': False,
                    'password': None,
                    'isFile': True,
                    'token': None
                }
                cookies = request.cookies
                if file.isShareEncryped:
                    payload['needPassword'] = True
                    if 'password' in args:
                        if args['password'] == file.sharePassword:
                            if 'download' in args and args['download'] == 'true':
                                rv = get_download_file(file)
                                if rv:
                                    return rv
                                else:
                                    return jsonify(message='error'), 409
                            token = s.dumps({'path': path}).decode('utf8')
                            return jsonify(message='ok', token=token)
                        else:
                            return jsonify(message='error', data=payload), 409
                    elif 'shareToken' in cookies:
                        try:
                            token = cookies['shareToken']
                            data = s.loads(token)
                            if data['path'] == path:
                                payload['needPassword'] = False
                                payload['password'] = file.sharePassword
                        except Exception as e:
                            return jsonify(message='unauthorized', data=payload), 401
                else:
                    if 'download' in args and args['download'] == 'true':
                        rv = get_download_file(file)
                        if rv:
                            return rv
                        else:
                            return jsonify(message='error'), 404
                return jsonify(message='ok', data=payload)
            else:
                return jsonify(message='error'), 409
        elif res[0] == 'fo':
            try:
                folder = Folder.get_by_id(int(res[1]))
            except peewee.DoesNotExist:
                return jsonify(message='error'), 404
            if folder.isShared:
                payload = {
                    'name': folder.name,
                    'isShared': folder.isShared,
                    'needPassword': False,
                    'password': None,
                    'isFile': False,
                    'token': None
                }
                cookies = request.cookies
                if folder.isShareEncryped:
                    payload['needPassword'] = True
                    if 'password' in args:
                        print(1)
                        if args['password'] == folder.sharePassword:
                            token = s.dumps({'path': path}).decode('utf8')
                            return jsonify(message='ok', token=token)
                        else:
                            return jsonify(message='error', data=payload), 409
                    elif 'shareToken' in cookies:
                        print(2)
                        try:
                            token = cookies['shareToken']
                            data = s.loads(token)
                            if data['path'] == path:
                                print(3)
                                payload['needPassword'] = False
                                payload['password'] = folder.sharePassword
                        except Exception as e:
                            return jsonify(message='unauthorized', data=payload), 401
                if 'rootpath' in args:
                    pass
                elif 'folderid' in args:
                    print(4)
                    folder_id = get_decrypt_url(args['folderid'])[1]
                    folders_model = Folder.select(Folder.id, Folder.name).where(
                        Folder.parentid == folder_id)
                    files_model = File.select(File.id, File.name).where(File.parentid == folder_id)
                    folders = []
                    files = []
                    for folder in folders_model:
                        folders.append({'id': get_encrypt_url(folder), 'name': folder.name})
                    for file in files_model:
                        files.append({'id': get_encrypt_url(file), 'name': file.name})
                    return jsonify(message='ok', data=payload,
                                   files=files,
                                   folders=folders)

                return jsonify(message='ok', data=payload,
                               files=[],
                               folders=[{'id': get_encrypt_url(folder), 'name': folder.name}])
            else:
                return jsonify(message='error'), 409
        else:
            return jsonify(message='error'), 409
    else:
        return jsonify(message='error'), 409


if __name__ == '__main__':
    # monitor = DirMonitor(ClouDiskPath)
    # monitor.start()
    t = threading.Thread(target=create_tcp_listen, args=('localhost', 16217))
    t.start()
    app.run(debug=True, use_reloader=False)
