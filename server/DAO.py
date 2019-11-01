from peewee import *

# db = SqliteDatabase('mydb.db', pragmas=(('foreign_keys', 'on'),))
db = SqliteDatabase('mydb.db')


class BaseModel(Model):
    class Meta:
        database = db


class Folder(BaseModel):
    id = BigIntegerField(primary_key=True)
    name = CharField(max_length=255)
    path = TextField()
    parentid = BigIntegerField()


class File(BaseModel):
    id = BigIntegerField(primary_key=True)
    name = CharField(max_length=255)
    folderid = BigIntegerField()
