#include "obj_transfer.h"

Obj_Transfer::Obj_Transfer()
{

}

Obj_Transfer::Obj_Transfer(bool isDownload, long id, QString name, double allSize): Obj_Transfer()
{
    this->isDownload = isDownload;
    this->id = id;
    this->name = name;
    this->allSize = allSize;
}

QString Obj_Transfer::objName() const
{
    return this->name;
}

QString Obj_Transfer::objSize() const
{
    return QString::number(allSize) + "MB";
}

QString Obj_Transfer::objProcSize() const
{
    return QString::number(curSize) + "MB/" + QString::number(allSize) + "MB";
}

QString Obj_Transfer::objTransSpeed() const
{
    return QString::number(transferSpeed) + "KB/s";
}

double Obj_Transfer::objTransScale() const
{
    return (double)curSize / allSize;
}

bool Obj_Transfer::objIsDownload() const
{
    return isDownload;
}

void Obj_Transfer::setCurSize(double size)
{
    this->curSize = size;
    this->processScale = curSize / allSize;
}

void Obj_Transfer::appendCurSize(double size)
{
    this->curSize += size;
    this->processScale = curSize / allSize;
}

void Obj_Transfer::setFinished(bool is_finish)
{
    this->isFinished = is_finish;
}

void Obj_Transfer::setTransmitting(bool is_trans)
{
    this->isTransmitting = is_trans;
}
