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
    isShared = BooleanField(default=False)
    isShareEncryped = BooleanField(default=False)
    sharePassword = CharField(max_length=32, default="")
    shareUrl = CharField(max_length=128, default="")
    sharePeriod = DateTimeField()


class File(BaseModel):
    id = BigIntegerField(primary_key=True)
    name = CharField(max_length=255)
    parentid = BigIntegerField()
    isShared = BooleanField(default=False)
    isShareEncryped = BooleanField(default=False)
    sharePassword = CharField(max_length=32, default="")
    shareUrl = CharField(max_length=128, default="")
    sharePeriod = DateTimeField()
