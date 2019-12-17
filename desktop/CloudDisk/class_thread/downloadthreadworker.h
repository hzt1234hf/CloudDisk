#ifndef DOWNLOADTHREADWORKER_H
#define DOWNLOADTHREADWORKER_H

#include <QObject>
#include <QTime>
#include <QTimer>
#include <QElapsedTimer>
#include <QDesktopServices>

#include "../class_obj/obj_frame.h"
#include "../class_obj/obj_transfer.h"

class DownloadThreadWorker: public QObject
{
    Q_OBJECT

public:
    QTime* time;

private:
    QList<Obj_Transfer*> m_datum;
    bool isRunningWork = false; // 线程是否运行中
    QTimer* timer;

public:
    explicit DownloadThreadWorker(QObject* parent = nullptr);
    QList<Obj_Transfer*>& getData();

public slots:
    /* 线程相关 */
    void continuousReadData();
    void work();        // 线程主函数
    void stopWork();    // 停止线程任务
    void startWork();   // 开始线程任务

    /* 任务创建 */
    void createDownloadItem(Obj_File*);    // 创建Download文件对象

    void switchDownloadTaskStatus(const QModelIndex& index);
    void deleteDownloadTask(const QModelIndex& index);
    void openDownloadFileDir(const QModelIndex& index);

signals:
    void readData(QTime* time);
    void addDownloadItem(Obj_Transfer*);    // 添加Download文件对象
    void createDownloadTask(Obj_File*, Obj_Transfer*); // 创建Download任务
    void updateView();
};

#endif // DOWNLOADTHREADWORKER_H
