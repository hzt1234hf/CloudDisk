#include "obj_base.h"

Obj_Base::Obj_Base(QWidget* parent) : QWidget(parent)
{

}

Obj_Base::Obj_Base(QWidget* parent,
                   long id, QString name, long parentid, QDate sharePeriod, bool isFile,
                   bool isShared, bool isShareEncryped, QString sharePassword, QString shareUrl)
    : Obj_Base(parent)
{
    setProperty(id, name, parentid, sharePeriod, isFile, isShared, isShareEncryped, sharePassword, shareUrl);
}

void Obj_Base::setProperty(long id, QString name, long parentid, QDate sharePeriod, bool isFile,
                           bool isShared, bool isShareEncryped, QString sharePassword, QString shareUrl)
{
    this->id = id;
    this->name = name;
    this->parentid = parentid;
    this->sharePeriod = sharePeriod;
    this->isFile = isFile;
    this->isShared = isShared;
    this->isShareEncryped = isShareEncryped;
    this->sharePassword = sharePassword;
    this->shareUrl = shareUrl;
}

void Obj_Base::paintEvent(QPaintEvent*)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    this->style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
