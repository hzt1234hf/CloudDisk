#ifndef UPLOADTHREADWORKER_H
#define UPLOADTHREADWORKER_H

#include <QObject>

#include "../class_obj/obj_frame.h"
#include "../class_obj/obj_transfer.h"

class UploadThreadWorker : public QObject
{
    Q_OBJECT
public:
    explicit UploadThreadWorker(QObject* parent = nullptr);
    QList<Obj_Transfer*>& getData();
private:
    QList<Obj_Transfer*> m_datum;

signals:

public slots:
};

#endif // UPLOADTHREADWORKER_H
