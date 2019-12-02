#ifndef OBJ_TRANSFER_H
#define OBJ_TRANSFER_H

#include <QString>



class Obj_Transfer
{
private:
    bool isDownload;
    QString name;
    long id;
    double allSize;

public:
    double processScale = 0.0;
    double transferSpeed = 0;
    double curSize = 0;
    bool isFinished = false;
    bool isTransmitting = false;

public:
    Obj_Transfer();
    Obj_Transfer(bool isDownload, long id = 0, QString name = "", double allSize = 0);

    QString objName() const;
    QString objSize() const;
    QString objProcSize() const;
    QString objTransSpeed() const;
    double objTransScale() const;
    bool objIsDownload() const;

    void setCurSize(double size);
    void appendCurSize(double size);
    void setFinished(bool is_finish);
    void setTransmitting(bool is_trans);
};

#endif // OBJ_TRANSFER_H
