#include "uploadthreadworker.h"

UploadThreadWorker::UploadThreadWorker(QObject* parent) : QObject(parent)
{

}

QList<Obj_Transfer*>& UploadThreadWorker::getData()
{
    return m_datum;
}
