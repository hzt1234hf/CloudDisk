#ifndef OBJ_FILE_H
#define OBJ_FILE_H

#include "obj_base.h"

class Obj_File : public Obj_Base
{
    Q_OBJECT


public:
    explicit Obj_File(QWidget* parent = nullptr);
    explicit Obj_File(QWidget* parent,
                      long id, QString name, long parentid, QDate sharePeriod,
                      bool isShared = false, bool isShareEncryped = false, QString sharePassword = "", QString shareUrl = "");
    void setFileProperty(long id, QString name, long parentid, QDate sharePeriod,
                         bool isShared = false, bool isShareEncryped = false, QString sharePassword = "", QString shareUrl = "");
signals:

public slots:
};

#endif // OBJ_FILE_H
