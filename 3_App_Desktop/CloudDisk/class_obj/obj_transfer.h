#ifndef OBJ_TRANSFER_H
#define OBJ_TRANSFER_H

#include <QObject>
#include <QString>
#include <QNetworkReply>
#include <QFile>
#include <QFileInfo>
#include <QThread>

#include "obj_file.h"
#include "../class_global/setting.h"



class Obj_Transfer: public QObject
{

    Q_OBJECT
protected:

    //  初始状态      传输中         正在暂停    暂停状态     正在完成      已完成       出错    停止状态     正在删除      已删除
    enum {NONE, IS_TRANSMITTING, IS_PAUSING, PAUSED, IS_FINISHING, FINISHED, HAS_ERROR, STOPPED, IS_DELETTING, DELETED};
    bool isDownload;                // 下载还是上传
    qint64 totalSize = 0;           // 文件总大小
    qint64 curTotalSize = 0;        // 此次传输文件总大小
    qint64 lastTransferedSize = 0;  // 未设置的接收到的文件大小
    qint64 transferedSize = 0;      // 接收到的文件大小
    double transferSpeed = 0;       // 传输速度

    QNetworkReply* reply = nullptr;     // Http回应对象，负责释放
    Obj_File* obj = nullptr;            // 文件信息对象
    QFile* file = nullptr;              // 输出文件信息

    enum {B, KB, MB, GB};           // 文件大小单位定义
    int fileTotalSizeUnit = B;      // 文件总大小单位
    int fileTransferedSizeUnit = B; // 文件已传输大小单位
    int fileTransferSpeedUnit = B;  // 文件传输速度单位

    int lastTime = 0;                   // 上一次时间
    int curStatus = NONE;               // 当前状态
    bool isBreadpointResume = false;    // 断电重传模式

public:


private:

public:
    Obj_Transfer();
    Obj_Transfer(bool isDownload, Obj_File* obj, QNetworkReply* reply = nullptr, int allSize = 0);

    bool openFile();

    void start();
    void stop();
    void deleteTask();

    Obj_File* getObj();
    QString objName() const;            // 文件名字
    QString objTotalSizeStr() const;    // 总大小字符串
    QString objTransferedSizeStr() const; // 已传输大小字符串
    QString objSizeScale() const;       // 文件传输比例字符串
    QString objTransferSpeed() const;   // 传输速度字符串
    int objTransferRate() const;        // 传输比例
    bool objIsDownload() const;         // 是否是下载
    bool objIsFinished() const;         // 是否完成
    bool objIsTransmitting() const;     // 是否正在传输
    bool objIsStopped() const;          // 是否暂停
    qint64 objTransferedSize() const;

    void setTransferSpeed(int interval);
    void setTotalSize(qint64 size);         // 设置文件大小
    void setTransferedSize(qint64 size);      // 设置接收到的文件大小
    void appendTransferedSize(qint64 size);   // 增加接收到的文件大小
    void setFinished(bool is_finish);       // 设置是否完成
    void setTransmitting(bool is_trans);    // 设置是否传输
    void setReply(QNetworkReply*);          // 设置QNetworkReply对象

signals:
    void updateView();
    void movetoFinishedView();

public slots:
    void error(QNetworkReply::NetworkError code);
    void finished();

};

#endif // OBJ_TRANSFER_H
