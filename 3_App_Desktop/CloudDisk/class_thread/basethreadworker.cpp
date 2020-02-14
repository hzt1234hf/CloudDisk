#include "basethreadworker.h"



BaseThreadWorker::BaseThreadWorker(QObject* parent): QObject(parent)
{

}

QList<Obj_Transfer*>& BaseThreadWorker::getData()
{
    return m_datum;
}

void BaseThreadWorker::continuousReadData()
{
    time = new QTime();
    timer = new QTimer(this);
    time->start();
    startWork();
}

void BaseThreadWorker::stopWork()
{
    isRunningWork = false;
    timer->stop();
}

void BaseThreadWorker::startWork()
{
    isRunningWork = true;
    timer->start(1000);
}
