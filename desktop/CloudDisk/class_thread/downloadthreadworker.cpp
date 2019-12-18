#include "downloadthreadworker.h"

DownloadThreadWorker::DownloadThreadWorker(QObject* parent) : QObject(parent)
{

}

QList<Obj_Transfer*>& DownloadThreadWorker::getData()
{
    return m_datum;
}
void DownloadThreadWorker::continuousReadData()
{
    time = new QTime();
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [ = ] {emit readData(time);});
    timer->start(1000);
    time->start();
}

void DownloadThreadWorker::work()
{
    for(;;)
    {
        if(isRunningWork)
        {
            QElapsedTimer t;
            t.start();
            while(t.elapsed() < 1000);
        }
    }
}

void DownloadThreadWorker::stopWork()
{
    isRunningWork = false;
    timer->stop();
}

void DownloadThreadWorker::startWork()
{
    isRunningWork = true;
    timer->start(1000);
}


void DownloadThreadWorker::createDownloadItem(Obj_File* obj)
{
    Obj_Transfer* tmp =  new Obj_Transfer(true, obj);
    if(tmp->openFile())
    {
        m_datum.push_back(tmp);
        connect(this, &DownloadThreadWorker::readData, tmp, &Obj_Transfer::readData);
        emit createDownloadTask(obj, tmp);
    }
    else
    {
        tmp->deleteLater();
    }
}

void DownloadThreadWorker::switchDownloadTaskStatus(const QModelIndex& index)
{
    if(m_datum[index.row()]->objIsTransmitting())
    {
        m_datum[index.row()]->stop();
    }
    else
    {
        qDebug() << "11111111";
        if(m_datum[index.row()]->openFile())
        {
            qDebug() << "22222";
            emit createDownloadTask(m_datum[index.row()]->getObj(), m_datum[index.row()]);
        }
    }
}
void DownloadThreadWorker::deleteDownloadTask(const QModelIndex& index)
{
    qDebug() << "deleteDownloadTask";
    Obj_Transfer* tmp = m_datum[index.row()];
    m_datum.removeOne(tmp);
    emit updateView();
    tmp->deleteTask();

}
void DownloadThreadWorker::openDownloadFileDir(const QModelIndex& index)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(setting::GetInstance()->getDownloadDir()));
}
