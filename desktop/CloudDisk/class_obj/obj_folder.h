#ifndef OBJ_FOLDER_H
#define OBJ_FOLDER_H

#include "obj_base.h"

class Obj_Folder : public Obj_Base
{
    Q_OBJECT
private:
    QString path;
public:
    explicit Obj_Folder(QWidget* parent = nullptr);
    explicit Obj_Folder(QWidget* parent,
                        long id, QString name, long parentid, QDate sharePeriod, QString path,
                        bool isShared = false, bool isShareEncryped = false, QString sharePassword = "", QString shareUrl = "");
    void setFolderProperty(long id, QString name, long parentid, QDate sharePeriod, QString path,
                           bool isShared = false, bool isShareEncryped = false, QString sharePassword = "", QString shareUrl = "");
signals:

public slots:
};

#endif // OBJ_FOLDER_H
