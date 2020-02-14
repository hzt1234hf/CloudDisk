#include "obj_transfer_download.h"

Obj_Transfer_Download::Obj_Transfer_Download(Obj_File* obj, QNetworkReply* reply, int totalSize): Obj_Transfer(true, obj, reply, totalSize)
{

}



void Obj_Transfer_Download::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    lastTransferedSize = bytesReceived;
    if(isBreadpointResume == false && (totalSize <= 0 || bytesTotal != totalSize))
    {
        this->setTotalSize(bytesTotal);
    }
    if(curTotalSize <= 0 || bytesTotal != curTotalSize)
        curTotalSize = bytesTotal;
}


void Obj_Transfer_Download::readyRead()
{
    dataIsReady = true;
}

void Obj_Transfer_Download::readData(const QTime* time)
{
    switch(curStatus)
    {
        case IS_TRANSMITTING:
            {
                if(dataIsReady)
                {
                    dataIsReady = false;
                    reply->setReadBufferSize(setting::GetInstance()->getSingleLimitDownloadSpeed());
                    file->write(reply->readAll());

                    this->setTransferSpeed(time->elapsed() - lastTime);
                    this->setTransferedSize(lastTransferedSize);
                    lastTime = time->elapsed();
                }
            }
            break;
        case IS_FINISHING:
        case IS_DELETTING:
        case IS_PAUSING:
        case HAS_ERROR:
            {
                qDebug() << curStatus;
                if(curStatus == IS_FINISHING)
                {
                    if(dataIsReady)
                    {
                        dataIsReady = false;
                        file->write(reply->readAll());

                        this->setTransferSpeed(time->elapsed() - lastTime);
                        this->setTransferedSize(lastTransferedSize);
                    }
                    file->rename(setting::GetInstance()->getDownloadDir() + "/" + obj->name);
                    emit movetoFinishedView();
                }
                if(file)
                {
                    file->close();
                    file->deleteLater();
                    file = nullptr;
                }
                if(reply)
                {
                    reply->deleteLater();
                    reply = nullptr;
                }

                if(curStatus == HAS_ERROR)
                {
                    curStatus = STOPPED;
                }
                else if(curStatus == IS_PAUSING)
                {
                    curStatus = PAUSED;
                }
                else if(curStatus == IS_DELETTING)
                {
                    QFile::remove(setting::GetInstance()->getDownloadDir() + "/" + obj->name + ".tmp");
                    this->deleteLater();
                    curStatus = DELETED;
                }
                else if(curStatus == IS_FINISHING)
                {
                    curStatus = FINISHED;
                }
            }
            break;
        case STOPPED:
        case FINISHED:
        case DELETED:
        case PAUSED:
            break;
        case NONE:
        default:
            {
                qDebug() << "Unknow Status!";
            }
            break;
    }

    emit updateView();
}
