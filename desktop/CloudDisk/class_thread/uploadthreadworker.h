#ifndef UPLOADTHREADWORKER_H
#define UPLOADTHREADWORKER_H

#include <QObject>

#include "../class_obj/obj_transfer_download.h"
#include "basethreadworker.h"

class UploadThreadWorker : public BaseThreadWorker
{
    Q_OBJECT

public:

private:

public:
    explicit UploadThreadWorker(QObject* parent = nullptr);

private:

signals:

public slots:
    /* 线程相关 */
    void continuousReadData() override;

};

#endif // UPLOADTHREADWORKER_H
