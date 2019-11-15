import datetime

from DAO import *


def create_all_tables():
    db.connect()
    db.create_tables([Folder, File, User])
    Folder.create(id=0, path='', name='NULL', parentid=0,
                  isShareEncryped=False, sharePassword="", shareUrl="",
                  sharePeriod=datetime.datetime.now())
    Folder.create(id=1, path='', name='全部文件', parentid=0,
                  isShareEncryped=False, sharePassword="", shareUrl="",
                  sharePeriod=datetime.datetime.now())
    Folder.create(id=2, path='', name='根目录', parentid=0,
                  isShareEncryped=False, sharePassword="", shareUrl="",
                  sharePeriod=datetime.datetime.now())
    User.create(email="admin@CloudDisk.com", password="admin")
    db.close()


if __name__ == '__main__':
    create_all_tables()
