from DAO import *


def create_all_tables():
    db.connect()
    db.create_tables([Folder, File])
    db.close()


if __name__ == '__main__':
    create_all_tables()
