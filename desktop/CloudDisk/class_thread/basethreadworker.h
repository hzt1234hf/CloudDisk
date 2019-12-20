#ifndef BASETHREADWORKER_H
#define BASETHREADWORKER_H

#include <QObject>
#include <QTime>
#include <QTimer>

#include "../class_obj/obj_transfer.h"

class BaseThreadWorker: public QObject
{
    Q_OBJECT
public:

protected:
    QList<Obj_Transfer*> m_datum;
    bool isRunningWork = false; // 线程是否运行中
    QTime* time;
    QTimer* timer;

public:
    explicit BaseThreadWorker(QObject* parent = nullptr);
    QList<Obj_Transfer*>& getData();

private:

signals:
    void updateView();
    void movetoFinishedView(Obj_Transfer*);

public slots:
    virtual void continuousReadData();
    void stopWork();
    void startWork();

};

#endif // BASETHREADWORKER_H
