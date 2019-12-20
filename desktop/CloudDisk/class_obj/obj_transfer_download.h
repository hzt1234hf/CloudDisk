#ifndef OBJ_TRANSFER_DOWNLOAD_H
#define OBJ_TRANSFER_DOWNLOAD_H


#include <QObject>
#include <QString>
#include <QNetworkReply>
#include <QFile>
#include <QFileInfo>
#include <QThread>

#include "obj_file.h"
#include "../class_global/setting.h"
#include "obj_transfer.h"



class Obj_Transfer_Download: public Obj_Transfer
{
    Q_OBJECT
private:
    bool dataIsReady = false;           // 传输的数据是否准备完毕

public:

private:

public:
    Obj_Transfer_Download(Obj_File* obj, QNetworkReply* reply = nullptr, int allSize = 0);

signals:

public slots:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void readyRead();
    void readData(const QTime* time);

};

#endif // OBJ_TRANSFER_DOWNLOAD_H
