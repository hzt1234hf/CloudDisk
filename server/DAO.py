from peewee import *

db = SqliteDatabase('mydb.db', pragmas=(('foreign_keys', 'on'),))


class BaseModel(Model):
    class Meta:
        database = db


class Folder(BaseModel):
    name = CharField(max_length=64, unique=True)


class File(BaseModel):
    folder = ForeignKeyField(Folder, backref='files')
    filename = CharField()


