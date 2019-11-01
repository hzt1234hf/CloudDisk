from DAO import *


def create_all_tables():
    db.connect()
    db.create_tables([Folder, File])
    Folder.create(id=0, path='', name='./', parentid=0)
    db.close()


if __name__ == '__main__':
    create_all_tables()
