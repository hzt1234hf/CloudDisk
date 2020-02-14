#ifndef SETTING_H
#define SETTING_H

#include <QObject>
#include <QFile>
#include <QDataStream>
#include <QDebug>

#include <QMessageBox>

#include "../serverconnect.h"

class setting : public QObject
{
    Q_OBJECT
public:

private:
    enum class SPEED {NONE, KB50, KB100, KB200, KB500, MB1, MB2, MB5, MB10 };
    enum class TASK_COUNT {NONE, COUNT1, COUNT2, COUNT3, COUNT4, COUNT5, COUNT10};

    static setting* instance;

    SPEED singleLimitDownloadSpeed;
    SPEED singleLimitUploadSpeed;
    SPEED limitDownloadSpeed;
    SPEED limitUploadSpeed;
    TASK_COUNT downloadTaskCount;
    TASK_COUNT uploadTaskCount;
    QString downloadDir;
    QString syncDir;
    QString tmpServerAddress;
    QString serverAddress;

    bool hasChanged = false;

public:
    void reinit();
    void print();

    qint64 getSingleLimitDownloadSpeed();
    qint64 getSingleLimitUploadSpeed();
    qint64 getLimitDownloadSpeed();
    qint64 getLimitUploadSpeed();
    int getDownloadTaskCount();
    int getUploadTaskCount();
    int getSingleLimitDownloadSpeedIndex();
    int getSingleLimitUploadSpeedIndex();
    int getLimitDownloadSpeedIndex();
    int getLimitUploadSpeedIndex();
    int getDownloadTaskCountIndex();
    int getUploadTaskCountIndex();
    QString getDownloadDir();
    QString getSyncDir();
    QString getServerAddress();


    static setting* GetInstance();
    void applySetting();
    void loadSetting();
    void saveSetting();

private:
    explicit setting(QObject* parent = nullptr);
    qint64 toRealSpeed(SPEED s);
    int toRealCount(TASK_COUNT t);

    friend QDataStream& operator<<(QDataStream& fout, const  setting& s);
    friend QDataStream& operator>>(QDataStream& fin, setting& s);


signals:

public slots:

    void setSingleLimitDownloadSpeed(int);
    void setSingleLimitUploadSpeed(int);
    void setLimitDownloadSpeed(int);
    void setLimitUploadSpeed(int);
    void setDownloadTaskCount(int);
    void setUploadTaskCount(int);
    void setDownloadDir(QString);
    void setSyncDir(QString);
    void setServerAddress(const QString);
};

#endif // SETTING_H

