#include "uploadthreadworker.h"

UploadThreadWorker::UploadThreadWorker(QObject* parent) : BaseThreadWorker(parent)
{

}

void UploadThreadWorker::continuousReadData()
{
    BaseThreadWorker::continuousReadData();
}
