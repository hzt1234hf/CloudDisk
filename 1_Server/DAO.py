from peewee import *

# db = SqliteDatabase('mydb.db', pragmas=(('foreign_keys', 'on'),))
# db = SqliteDatabase("mydb.db")
settings = {'host': 'localhost', 'password': 'ziteng', 'port': 3306, 'user': 'root'}
db = MySQLDatabase("cloud_disk", **settings)


class BaseModel(Model):
    class Meta:
        database = db
        table_name = 'cloud_disk'


class User(BaseModel):
    # ID号（主键）
    # id=BigIntegerField(primary_key=True)
    # 用户邮箱（唯一）
    email = CharField(max_length=32, primary_key=True)
    # 用户密码
    password = CharField(max_length=32)


class Folder(BaseModel):
    # ID号（主键）
    id = BigIntegerField(primary_key=True)
    # 文件夹名
    name = CharField(max_length=255)
    # 文件夹创建时间
    # createDate = DateTimeField()
    # 文件夹地址
    path = TextField()
    # 文件夹所属父目录ID
    parentid = BigIntegerField()
    # 文件夹是否共享
    isShared = BooleanField(default=False)
    # 文件夹是否加密
    isShareEncryped = BooleanField(default=False)
    # 文件夹分享模式
    # sharedMode = SmallIntegerField()
    # 文件夹分享密码
    sharePassword = CharField(max_length=32, default="")
    # 文件夹分享URL
    shareUrl = CharField(max_length=128, default="")
    # 文件夹分享过期日期
    sharePeriod = DateTimeField()


class File(BaseModel):
    # ID号（主键）
    id = BigIntegerField(primary_key=True)
    # 文件名
    name = CharField(max_length=255)
    # 文件大小
    # size = BigIntegerField()
    # 文件创建时间
    # createDate = DateTimeField()
    # 文件所属父目录ID
    parentid = BigIntegerField()
    # 文件是否共享
    isShared = BooleanField(default=False)
    # 文件是否加密
    isShareEncryped = BooleanField(default=False)
    # 文件分享模式
    # sharedMode = SmallIntegerField()
    # 文件分享密码
    sharePassword = CharField(max_length=32, default="")
    # 文件分享URL
    shareUrl = CharField(max_length=128, default="")
    # 文件分享过期日期
    sharePeriod = DateTimeField()
