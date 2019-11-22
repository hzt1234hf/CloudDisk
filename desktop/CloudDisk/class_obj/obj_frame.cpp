#include "obj_frame.h"

obj_frame::obj_frame(QWidget* parent) : QWidget(parent)
{
    this->setMinimumSize(fwidth, fheight);
    this->setMaximumSize(fwidth, fheight);

}

obj_frame::obj_frame(QWidget* parent,
                     bool isFile, long id, QString name, long parentid, QDate sharePeriod,
                     QString path, bool isShared, bool isShareEncryped, QString sharePassword, QString shareUrl) : obj_frame(parent)
{
    setInfo( isFile, id, name, parentid, sharePeriod, path, isShared, isShareEncryped, sharePassword, shareUrl);
}

obj_frame::~obj_frame()
{
    if(this->obj)
        delete this->obj;
    if(this->label)
        delete this->label;
}

void obj_frame::setPosition(int x, int y)
{
    this->setGeometry(x, y, fwidth, fheight);
}

void obj_frame::setInfo(bool isFile, long id, QString name, long parentid, QDate sharePeriod,
                        QString path, bool isShared, bool isShareEncryped, QString sharePassword, QString shareUrl)
{
    if(this->obj)
    {
        if(isFile)
        {
            static_cast<Obj_File*>(this->obj)->setFileProperty(id, name, parentid, sharePeriod, isShared, isShareEncryped, sharePassword, shareUrl);
        }
        else
        {
            static_cast<Obj_Folder*>(this->obj)->setFolderProperty(id, name, parentid, sharePeriod, path, isShared, isShareEncryped, sharePassword, shareUrl);
        }
    }
    else
    {
        if(isFile)
        {
            this->obj = new Obj_File(this, id, name, parentid, sharePeriod, isShared, isShareEncryped, sharePassword, shareUrl);
        }
        else
        {
            this->obj = new Obj_Folder(this, id, name, parentid, sharePeriod, path, isShared, isShareEncryped, sharePassword, shareUrl);
        }
    }
    if(this->label)
    {
        this->label->setObjectName(name);
    }
    else
    {
        this->label = new QLabel(name, this);
    }
    this->label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    this->label->setFont(QFont("微软雅黑", 9));

    this->obj->setObjectName("obj");

    this->obj->setMinimumSize(owidth, oheight);
    this->obj->setMaximumSize(owidth, oheight);
    this->label->setMinimumSize(lwidth, lheight);
    this->label->setMaximumSize(lwidth, lheight);

    this->obj->setGeometry(objx, objy, owidth, oheight);
    this->label->setGeometry(labelx, labely, lwidth, lheight);

    QString objName = "";
    if(isFile)
    {
        objName = "file_" + QString::number(id);
        QString objType = name.section('.', 1, 1);
        QString icon = "....";
        if(objType != "")
        {
            icon = "image";
        }
        qDebug() << objName << "   " << icon;
        this->setStyleSheet("\
                           QWidget#obj{\
                               image: url(:/file/icon/file/" + icon + ".png);\
                               padding:" + QString::number(opadding) + "px;\
                           }\
                           QWidget#" + objName + "{\
                               background-color:white;\
                               padding:" + QString::number(fpadding) + "px;\
                           }\
                           QWidget#" + objName + ":hover{\
                               background-color:#BBDAFF;\
                           }\
                           QWidget#" + objName + ":pressed{\
                               background-color:#57A;\
                           }\
                           QWidget#" + objName + ":checked{\
                               background-color:#571;\
                           }\
                           ");
    }
    else
    {
        objName = "folder_" + QString::number(id);
        this->setStyleSheet("\
                            QWidget#obj{\
                                image: url(:/panel/icon/panel/folder (1).png);\
                                padding:" + QString::number(opadding) + "px;\
                            }\
                            QWidget#" + objName + "{\
                                background-color:white;\
                                padding:" + QString::number(fpadding) + "px;\
                            }\
                            QWidget#" + objName + ":hover{\
                                background-color:#BBDAFF;\
                            }\
                            QWidget#" + objName + ":pressed{\
                                background-color:#57A;\
                            }\
                            QWidget#" + objName + ":checked{\
                                background-color:#571;\
                            }\
                            ");
    }
    this->setObjectName(objName);

    this->show();
    this->obj->show();
    this->label->show();
}

void obj_frame::resetInfo(bool isFile, long id, QString name, long parentid, QDate sharePeriod,
                          QString path, bool isShared, bool isShareEncryped, QString sharePassword, QString shareUrl)
{
    setInfo(isFile, id, name, parentid, sharePeriod, path, isShared, isShareEncryped, sharePassword, shareUrl);
}


void obj_frame::paintEvent(QPaintEvent*)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    this->style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
