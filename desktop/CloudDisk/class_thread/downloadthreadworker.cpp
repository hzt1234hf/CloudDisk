#include "downloadthreadworker.h"

DownloadThreadWorker::DownloadThreadWorker(QObject* parent) : BaseThreadWorker(parent)
{

}

void DownloadThreadWorker::continuousReadData()
{
    BaseThreadWorker::continuousReadData();
    connect(timer, &QTimer::timeout, [ = ] {emit readData(time);});
}


void DownloadThreadWorker::createDownloadItem(Obj_File* obj)
{
    Obj_Transfer_Download* tmp =  new Obj_Transfer_Download(obj);
    if(tmp->openFile())
    {
        m_datum.push_back(tmp);
        connect(this, &DownloadThreadWorker::readData, tmp, &Obj_Transfer_Download::readData);
        connect(tmp, &Obj_Transfer_Download::movetoFinishedView, [ = ]
        {
            m_datum.removeOne(tmp);
            emit updateView();
            emit movetoFinishedView(tmp);
        });
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
            emit createDownloadTask(m_datum[index.row()]->getObj(), reinterpret_cast<Obj_Transfer_Download*>(m_datum[index.row()]));
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
//    m_datum[index.row()] // 打开文件地址
    QDesktopServices::openUrl(QUrl::fromLocalFile(setting::GetInstance()->getDownloadDir()));
}
