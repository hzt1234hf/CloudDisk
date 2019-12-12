#include "obj_transfer.h"


Obj_Transfer::Obj_Transfer()
{
    start();
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
    file = new QSaveFile(obj->name, this);
    return file->open(QSaveFile::WriteOnly);

    QFileInfo f(obj->name);
    if(f.exists() == true)
    {
        if(file)
        {
            file->deleteLater();
            file = nullptr;
        }
        return false;
    }
    else
    {
        if(file == nullptr)
        {
            file = new QSaveFile(obj->name);
        }
        return file->open(QSaveFile::WriteOnly);
    }
}

void Obj_Transfer::start()
{
    timer.start();
    isTransmitting = true;
}

void Obj_Transfer::stop()
{
    isTransmitting = false;
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
    return (double)receivedSize * 10 / totalSize * 10;
}

bool Obj_Transfer::objIsDownload() const
{
    return isDownload;
}

bool Obj_Transfer::objIsFinished() const
{
    return isFinished;
}

bool Obj_Transfer::objIsTransmitting() const
{
    return isTransmitting;
}

void Obj_Transfer::setTransferSpeed(qint64 transferBytes, int interval)
{
    qDebug() << transferBytes << "   " << interval;
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
    this->receivedSize = size;
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
    this->isFinished = is_finish;
}

void Obj_Transfer::setTransmitting(bool is_trans)
{
    this->isTransmitting = is_trans;
}

void Obj_Transfer::setReply(QNetworkReply* reply)
{
    this->reply = reply;
}

void Obj_Transfer::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    qDebug() << QThread::currentThreadId();
    lastReceivedSize = bytesReceived;
    if(totalSize <= 0 || bytesTotal != totalSize)
    {
        this->setTotalSize(bytesTotal);
        emit updateView();
    }
    int curTime = timer.elapsed();
    if(curTime - lastTime >= TimeInterval)
    {

        this->setTransferSpeed(lastReceivedSize - receivedSize, curTime - lastTime);
        this->setReceivedSize(lastReceivedSize);
        lastTime = curTime; // 重设时间

        emit updateView();
    }
}

void Obj_Transfer::error(QNetworkReply::NetworkError code)
{
    qDebug() << code;
    file->commit();
//    reply->deleteLater();
//    file->deleteLater();
}

void Obj_Transfer::finished()
{
    qDebug() << "finished.";

    this->setTransferSpeed(totalSize - receivedSize, timer.elapsed() - lastTime);
    this->setReceivedSize(totalSize);

    emit updateView();
    file->commit();
    reply->deleteLater();
    file->deleteLater();

    reply = nullptr;
    file = nullptr;
}

void Obj_Transfer::readyRead()
{
    while(timer.elapsed() - lastTime <= TimeInterval);
    qDebug() << file;
    file->write(reply->readAll());
//    dataIsReady = true;
}


void Obj_Transfer::readData()
{
    if(dataIsReady)
    {
        file->write(reply->readAll());
    }
    dataIsReady = false;
}
