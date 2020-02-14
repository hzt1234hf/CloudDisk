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
    this->isFile = isFile;
    if(this->obj)
    {
        if(isFile)
            this->objfile->setFileProperty(id, name, parentid, sharePeriod, isShared, isShareEncryped, sharePassword, shareUrl);
        else
            this->objfolder->setFolderProperty(id, name, parentid, sharePeriod, path, isShared, isShareEncryped, sharePassword, shareUrl);
    }
    else
    {
        if(isFile)
            this->objfile = new Obj_File(this, id, name, parentid, sharePeriod, isShared, isShareEncryped, sharePassword, shareUrl);
        else
            this->objfolder = new Obj_Folder(this, id, name, parentid, sharePeriod, path, isShared, isShareEncryped, sharePassword, shareUrl);

        this->obj->setObjectName("obj");

        this->obj->setMinimumSize(owidth, oheight);
        this->obj->setMaximumSize(owidth, oheight);

        this->obj->setGeometry(objx, objy, owidth, oheight);
    }
    if(this->label)
    {
        this->label->setText(name);
    }
    else
    {
        this->label = new QLabel(name, this);
        this->label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        this->label->setFont(QFont("微软雅黑", 9));
        this->label->setMinimumSize(lwidth, lheight);
        this->label->setMaximumSize(lwidth, lheight);
        this->label->setGeometry(labelx, labely, lwidth, lheight);
    }

    QString objName = "";
    if(isFile)
    {
        objName = "file_" + QString::number(id);
        objType = name.section('.', 1, 1);
        QString icon = "....";
        if(objType != "")
        {
            icon = "image";
        }
        this->setObjectName(objName);
    }
    else
    {
        objName = "folder_" + QString::number(id);
        this->setObjectName(objName);
    }
    setUnselected();

    this->show();
}

void obj_frame::resetInfo(bool isFile, long id, QString name, long parentid, QDate sharePeriod,
                          QString path, bool isShared, bool isShareEncryped, QString sharePassword, QString shareUrl)
{
    setInfo(isFile, id, name, parentid, sharePeriod, path, isShared, isShareEncryped, sharePassword, shareUrl);
}

void obj_frame::setUnselected()
{
    if(isFile)
    {
        QString icon = "....";
        if(objType != "")
        {
            icon = "image";
        }
        this->setStyleSheet("\
                           QWidget#obj{\
                               image: url(:/file/icon/file/" + icon + ".png);\
                               padding:" + QString::number(opadding) + "px;\
                           }\
                           QWidget#" + this->objectName() + "{\
                               background-color:white;\
                               padding:" + QString::number(fpadding) + "px;\
                           }\
                           QWidget#" + this->objectName() + ":hover{\
                               background-color:#BBDAFF;\
                           }\
                           ");
    }
    else
    {
        this->setStyleSheet("\
                            QWidget#obj{\
                                image: url(:/panel/icon/panel/folder (1).png);\
                                padding:" + QString::number(opadding) + "px;\
                            }\
                            QWidget#" + this->objectName() + "{\
                                background-color:white;\
                                padding:" + QString::number(fpadding) + "px;\
                            }\
                            QWidget#" + this->objectName() + ":hover{\
                                background-color:#BBDAFF;\
                            }\
                            ");
    }
}

void obj_frame::setSelected()
{
    qDebug() << "???";
    if(isFile)
    {
        QString icon = "....";
        if(objType != "")
        {
            icon = "image";
        }
        this->setStyleSheet("\
                           QWidget#obj{\
                               image: url(:/file/icon/file/" + icon + ".png);\
                               padding:" + QString::number(opadding) + "px;\
                           }\
                           QWidget#" + this->objectName() + "{\
                               background-color:#8AD;\
                               padding:" + QString::number(fpadding) + "px;\
                           }\
                           ");
    }
    else
    {
        this->setStyleSheet("\
                            QWidget#obj{\
                                image: url(:/panel/icon/panel/folder (1).png);\
                                padding:" + QString::number(opadding) + "px;\
                            }\
                            QWidget#" + this->objectName() + "{\
                                background-color:#8AD;\
                                padding:" + QString::number(fpadding) + "px;\
                            }\
                            ");
    }
}

void obj_frame::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        emit selected(Qt::LeftButton, this);
        event->accept();
    }
    else if(event->button() == Qt::RightButton)
    {
        emit selected(Qt::RightButton, this);
        event->accept();
    }
}

void obj_frame::mouseDoubleClickEvent(QMouseEvent* event)
{
    qDebug() << obj->id;
    emit open_obj(obj->id);
}

void obj_frame::paintEvent(QPaintEvent* e)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    this->style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(e);
}
