#include "obj_transfer.h"

Obj_Transfer::Obj_Transfer()
{

}

Obj_Transfer::Obj_Transfer(bool isDownload, Obj_File* obj, QNetworkReply* reply, int totalSize): Obj_Transfer()
{
    this->isDownload = isDownload;
    this->obj = obj;
    this->totalSize = totalSize;
    this->reply = reply;

}


bool Obj_Transfer::openFile()
{
    qDebug() << setting::GetInstance()->getDownloadDir() + "/" + obj->name;
    QFileInfo f(setting::GetInstance()->getDownloadDir() + "/" + obj->name);
    if(f.exists() == true)
    {
        if(curStatus != FINISHED)
        {
            file = new QFile(setting::GetInstance()->getDownloadDir() + "/" + obj->name + ".tmp");
            return file->open(QIODevice::WriteOnly | QIODevice::Append);
        }
        else if(file)
        {
            file->close();
//            file->deleteLater();
            file = nullptr;
        }
        return false;
    }
    else
    {
        if(file == nullptr)
        {
            file = new QFile(setting::GetInstance()->getDownloadDir() + "/" + obj->name + ".tmp");
        }
        return file->open(QIODevice::WriteOnly | QIODevice::Append);
    }
}



void Obj_Transfer::start()
{
    if(curStatus == NONE)
    {
        curStatus = IS_TRANSMITTING;
    }
    else if(curStatus == PAUSED || curStatus == STOPPED)
    {
        isBreadpointResume = true;
        curStatus = IS_TRANSMITTING;
    }
}

void Obj_Transfer::stop()
{
    if(curStatus == IS_TRANSMITTING )
    {
        reply->abort();
        curStatus = IS_PAUSING;
    }
}

void Obj_Transfer::deleteTask()
{
    curStatus = IS_DELETTING;
    if(reply)
        reply->abort();
}

Obj_File* Obj_Transfer::getObj()
{
    return obj;
}

QString Obj_Transfer::objName() const
{
    return obj->name;
}

QString Obj_Transfer::objTotalSizeStr() const
{
    if(totalSize > 0)
    {
        switch(fileTotalSizeUnit)
        {
            case B:
                return QString::number(totalSize, 'f', 2) + "B";
            case KB:
                return QString::number(totalSize / 1024.0, 'f', 2) + "KB";
            case MB:
                return QString::number(totalSize / 1024.0 / 1024.0, 'f', 2) + "MB";
            case GB:
                return QString::number(totalSize / 1024.0 / 1024.0 / 1024.0, 'f', 2) + "GB";
        }
    }
    return "-";
}

QString Obj_Transfer::objTransferedSizeStr() const
{
    if(transferedSize > 0)
    {
//        switch(fileReceivedSizeUint)
        switch(fileTotalSizeUnit)
        {
            case B:
                return QString::number(transferedSize, 'f', 2) + "B";
            case KB:
                return QString::number(transferedSize / 1024.0, 'f', 2) + "KB";
            case MB:
                return QString::number(transferedSize / 1024.0 / 1024.0, 'f', 2) + "MB";
            case GB:
                return QString::number(transferedSize / 1024.0 / 1024.0 / 1024.0, 'f', 2) + "GB";
        }
    }
    return "-";
}

QString Obj_Transfer::objSizeScale() const
{
    return objTransferedSizeStr() + "/" + objTotalSizeStr();
}

QString Obj_Transfer::objTransferSpeed() const
{
    if(transferSpeed > 0)
    {
        switch(fileTransferSpeedUnit)
        {
            case B:
                return QString::number(transferSpeed, 'f', 2) + "B/s";
            case KB:
                return QString::number(transferSpeed / 1024.0, 'f', 2) + "KB/s";
            case MB:
                return QString::number(transferSpeed / 1024.0 / 1024.0, 'f', 2) + "MB/s";
            case GB:
                return QString::number(transferSpeed / 1024.0 / 1024.0 / 1024.0, 'f', 2) + "GB/s";
        }
    }
    return "-";
}

int Obj_Transfer::objTransferRate() const
{
    if(totalSize < 0.01)
        return 0;
    return (double)transferedSize * 10 / totalSize * 10;
}

bool Obj_Transfer::objIsDownload() const
{
    return isDownload;
}

bool Obj_Transfer::objIsFinished() const
{
    return curStatus == FINISHED;
}

bool Obj_Transfer::objIsTransmitting() const
{
    return curStatus == IS_TRANSMITTING;
}

bool Obj_Transfer::objIsStopped() const
{
    return curStatus == PAUSED;
}

qint64 Obj_Transfer::objTransferedSize() const
{
    return transferedSize;
}

void Obj_Transfer::setTransferSpeed(int interval)
{
    qint64 transferBytes = totalSize - curTotalSize + lastTransferedSize - transferedSize;
    qDebug() << lastTransferedSize << "   " << transferedSize << "   " << (lastTransferedSize - transferedSize) / 1024.0 << "  " << curTotalSize << "  " << totalSize;
    transferSpeed = transferBytes * 1000.0 / interval + 1;
    if(transferSpeed > 1073741824.0)
        fileTransferSpeedUnit = GB;
    else if(transferSpeed > 1048576.0)
        fileTransferSpeedUnit = MB;
    else if(transferSpeed > 1024.0)
        fileTransferSpeedUnit = KB;
    else
        fileTransferSpeedUnit = B;
}

void Obj_Transfer::setTotalSize(qint64 size)
{
    totalSize = size;
    if(totalSize >= 1073741824)
        fileTotalSizeUnit = GB;
    else if(totalSize >= 1048576)
        fileTotalSizeUnit = MB;
    else if(totalSize >= 1024)
        fileTotalSizeUnit = KB;
    else
        fileTotalSizeUnit = B;
}

void Obj_Transfer::setTransferedSize(qint64 size)
{
    this->transferedSize = size + totalSize - curTotalSize;
//    fileReceivedSizeUint = B;
//    if(receivedSize > 1024)
//    {
//        receivedSize = receivedSize  /1024.0;
//        fileReceivedSizeUint = KB;
//    }
//    if(receivedSize > 1024)
//    {
//        receivedSize = receivedSize  /1024.0;
//        fileReceivedSizeUint = MB;
//    }
//    if(receivedSize > 1024)
//    {
//        receivedSize = receivedSize  /1024.0;
//        fileReceivedSizeUint = GB;
//    }
}

void Obj_Transfer::appendTransferedSize(qint64 size)
{
    this->transferedSize += size;
}

void Obj_Transfer::setFinished(bool is_finish)
{
    this->curStatus = IS_FINISHING;
}

void Obj_Transfer::setTransmitting(bool is_trans)
{
    this->curStatus = IS_TRANSMITTING;
}

void Obj_Transfer::setReply(QNetworkReply* reply)
{
    this->reply = reply;
}

void Obj_Transfer::error(QNetworkReply::NetworkError code)
{
    qDebug() << "error:   " << code;
    if(curStatus == IS_TRANSMITTING)
        curStatus = HAS_ERROR;
}

void Obj_Transfer::finished()
{
    if(curStatus == IS_TRANSMITTING)
    {
        curStatus = IS_FINISHING;
    }
}
