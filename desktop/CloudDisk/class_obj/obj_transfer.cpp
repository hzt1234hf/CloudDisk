#include "obj_transfer.h"


Obj_Transfer::Obj_Transfer()
{

}

Obj_Transfer::Obj_Transfer(bool isDownload, Obj_File* obj, QNetworkReply* reply, double totalSize): Obj_Transfer()
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
    else if(curStatus == STOPPED)
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
    if(reply)
        reply->abort();
    curStatus = IS_DELETTING;
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

QString Obj_Transfer::objReceivedSizeStr() const
{
    if(receivedSize > 0)
    {
//        switch(fileReceivedSizeUint)
        switch(fileTotalSizeUnit)
        {
            case B:
                return QString::number(receivedSize, 'f', 2) + "B";
            case KB:
                return QString::number(receivedSize / 1024.0, 'f', 2) + "KB";
            case MB:
                return QString::number(receivedSize / 1024.0 / 1024.0, 'f', 2) + "MB";
            case GB:
                return QString::number(receivedSize / 1024.0 / 1024.0 / 1024.0, 'f', 2) + "GB";
        }
    }
    return "-";
}

QString Obj_Transfer::objSizeScale() const
{
    return objReceivedSizeStr() + "/" + objTotalSizeStr();
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
    return (double)receivedSize * 10 / totalSize * 10;
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
    return curStatus == STOPPED;
}

qint64 Obj_Transfer::objReceivedSize() const
{
    return receivedSize;
}

void Obj_Transfer::setTransferSpeed(int interval)
{
    qint64 transferBytes = totalSize - curTotalSize + lastReceivedSize - receivedSize;
    transferSpeed = transferBytes * 1000.0 / interval;
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

void Obj_Transfer::setReceivedSize(qint64 size)
{
    this->receivedSize = size + totalSize - curTotalSize;
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

void Obj_Transfer::appendReceivedSize(qint64 size)
{
    this->receivedSize += size;
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





void Obj_Transfer::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    lastReceivedSize = bytesReceived;
    if(isBreadpointResume == false && (totalSize <= 0 || bytesTotal != totalSize))
    {
        this->setTotalSize(bytesTotal);
    }
    if(curTotalSize <= 0 || bytesTotal != curTotalSize)
        curTotalSize = bytesTotal;
}

void Obj_Transfer::error(QNetworkReply::NetworkError code)
{
    qDebug() << "error:   " << code;
    curStatus = HAS_ERROR;
}

void Obj_Transfer::finished()
{
    if(curStatus != IS_PAUSING && curStatus != STOPPED)
    {
        curStatus = IS_FINISHING;
    }
}

void Obj_Transfer::readyRead()
{
    dataIsReady = true;
}

void Obj_Transfer::readData(QTime* time)
{
    switch(curStatus)
    {

        case IS_FINISHING:
        case IS_TRANSMITTING:
            {
                if(dataIsReady)
                {
                    dataIsReady = false;
//                    reply->setReadBufferSize(setting::GetInstance()->getSingleLimitDownloadSpeed());
                    file->write(reply->readAll());
                    file->flush();

                    this->setTransferSpeed(time->elapsed() - lastTime);
                    this->setReceivedSize(lastReceivedSize);
                    lastTime = time->elapsed();

                }
                if(curStatus == IS_FINISHING)
                {
                    curStatus = FINISHED;
                }
            }
            break;

        case IS_DELETTING:
        case IS_PAUSING:
        case HAS_ERROR:
        case FINISHED:
            {
                if(curStatus == FINISHED)
                {
                    curStatus = STOPPED;
                    file->rename(setting::GetInstance()->getDownloadDir() + "/" + obj->name);
                }
                if(file)
                {
                    file->close();
//                    file->deleteLater();
                    file = nullptr;
                }
                if(reply)
                {
                    reply->deleteLater();
                    reply = nullptr;
                }

                if(curStatus == HAS_ERROR)
                {
                }
                else if(curStatus == IS_PAUSING)
                {
                    curStatus = STOPPED;
                }
                else if(curStatus == IS_DELETTING)
                {
                    this->deleteLater();
                    curStatus = DELETED;
                }
            }
            break;
        case DELETED:
        case STOPPED:
            {

            } break;
        case NONE:
        default:
            {
                qDebug() << "Unknow Status!";
            }
            break;
    }

    emit updateView();
}
