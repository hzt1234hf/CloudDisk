#include "obj_file.h"

Obj_File::Obj_File(QWidget* parent) : Obj_Base(parent)
{

}

Obj_File::Obj_File(QWidget* parent,
                   long id, QString name, long parentid, QDate sharePeriod,
                   bool isShared, bool isShareEncryped, QString sharePassword, QString shareUrl)
    : Obj_File (parent)
{
    setProperty(id, name, parentid, sharePeriod, true, isShared, isShareEncryped, sharePassword, shareUrl);
}

void Obj_File::setFileProperty(long id, QString name, long parentid, QDate sharePeriod,
                               bool isShared, bool isShareEncryped, QString sharePassword, QString shareUrl)
{
    setProperty(id, name, parentid, sharePeriod, true, isShared, isShareEncryped, sharePassword, shareUrl);
}
