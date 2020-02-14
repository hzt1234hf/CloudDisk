#ifndef DOWNLOADTHREADWORKER_H
#define DOWNLOADTHREADWORKER_H

#include <QObject>
#include <QTime>
#include <QTimer>
#include <QElapsedTimer>
#include <QDesktopServices>

#include "../class_obj/obj_transfer_download.h"
#include "basethreadworker.h"

class DownloadThreadWorker: public BaseThreadWorker
{
    Q_OBJECT

public:

private:

public:
    explicit DownloadThreadWorker(QObject* parent = nullptr);

signals:
    void readData(const QTime* time);
    void addDownloadItem(Obj_Transfer_Download*);    // 添加Download文件对象
    void createDownloadTask(Obj_File*, Obj_Transfer_Download*); // 创建Download任务

public slots:
    /* 线程相关 */
    void continuousReadData() override;

    /* 任务创建 */
    void createDownloadItem(Obj_File*);    // 创建Download文件对象

    void switchDownloadTaskStatus(const QModelIndex& index);
    void deleteDownloadTask(const QModelIndex& index);
    void openDownloadFileDir(const QModelIndex& index);


};

#endif // DOWNLOADTHREADWORKER_H
