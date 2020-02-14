#include "obj_folder.h"

Obj_Folder::Obj_Folder(QWidget* parent) : Obj_Base(parent)
{

}
Obj_Folder::Obj_Folder(QWidget* parent,
                       long id, QString name, long parentid, QDate sharePeriod, QString path,
                       bool isShared, bool isShareEncryped, QString sharePassword, QString shareUrl)
    : Obj_Folder (parent)
{
    setFolderProperty(id, name, parentid, sharePeriod, path, isShared, isShareEncryped, sharePassword, shareUrl);
}

void Obj_Folder::setFolderProperty(long id, QString name, long parentid, QDate sharePeriod, QString path, bool isShared, bool isShareEncryped, QString sharePassword, QString shareUrl)
{
    setProperty(id, name, parentid, sharePeriod, false, isShared, isShareEncryped, sharePassword, shareUrl);
    this->path = path;
}

