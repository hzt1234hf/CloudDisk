#include "setting.h"

setting* setting::instance = nullptr;

void setting::reinit()
{
    limitDownloadSpeed = SPEED::NONE;
    limitUploadSpeed = SPEED::NONE;
    singleLimitDownloadSpeed = SPEED::NONE;
    singleLimitUploadSpeed = SPEED::NONE;
    downloadTaskCount = TASK_COUNT::NONE;
    uploadTaskCount = TASK_COUNT::NONE;
    downloadDir = "";
    syncDir = "";
    serverAddress = "http://127.0.0.1:5000";
}

void setting::print()
{
    qDebug() << (int)singleLimitDownloadSpeed << "   " << (int)singleLimitUploadSpeed << "   "
             << (int)limitDownloadSpeed << "   " << (int)limitUploadSpeed << "   "
             << (int)downloadTaskCount << "   " << (int)uploadTaskCount << "   "
             << downloadDir << "   " << syncDir;
}

qint64 setting::getSingleLimitDownloadSpeed()
{
    return toRealSpeed(singleLimitDownloadSpeed);
}

qint64 setting::getSingleLimitUploadSpeed()
{
    return toRealSpeed(singleLimitUploadSpeed);
}

qint64 setting::getLimitDownloadSpeed()
{
    return toRealSpeed(limitDownloadSpeed);
}

qint64 setting::getLimitUploadSpeed()
{
    return toRealSpeed(limitUploadSpeed);
}

int setting::getDownloadTaskCount()
{
    return toRealCount(downloadTaskCount);
}

int setting::getUploadTaskCount()
{
    return toRealCount(uploadTaskCount);
}

int setting::getSingleLimitDownloadSpeedIndex()
{
    return (int)singleLimitDownloadSpeed;
}

int setting::getSingleLimitUploadSpeedIndex()
{
    return (int)singleLimitUploadSpeed;
}

int setting::getLimitDownloadSpeedIndex()
{
    return (int)limitDownloadSpeed;
}

int setting::getLimitUploadSpeedIndex()
{
    return (int)limitUploadSpeed;
}

int setting::getDownloadTaskCountIndex()
{
    return (int)downloadTaskCount;
}

int setting::getUploadTaskCountIndex()
{
    return (int)uploadTaskCount;
}
QString setting::getDownloadDir()
{
    return downloadDir;
}

QString setting::getSyncDir()
{
    return syncDir;
}

QString setting::getServerAddress()
{
    return serverAddress;
}

setting* setting::GetInstance()
{
    if(setting::instance == nullptr)
    {

        setting::instance = new setting();
    }
    return setting::instance;
}

void setting::applySetting()
{
    if(tmpServerAddress != serverAddress)
    {
        serverAddress = tmpServerAddress;
    }
}

void setting::loadSetting()
{
    qDebug() << "Load Setting";
    QFile file("setting.dat");
    if(file.open(QIODevice::ReadOnly | QIODevice::ExistingOnly))
    {
        QDataStream in(&file);
        in >> *this;
        file.close();
    }
    else
    {
        hasChanged = true;
        QMessageBox::warning(nullptr, tr("出错！"), tr("加载配置文件出错！使用默认配置."));
    }
}

void setting::saveSetting()
{
    qDebug() << "Save Setting";
    QFile file("setting.dat");
    if(hasChanged)
    {
        if(file.open(QIODevice::WriteOnly | QIODevice::ExistingOnly) || file.open(QIODevice::WriteOnly))
        {
            QDataStream out(&file);
            out << *this;
            hasChanged = false;
            file.close();
        }
        else
        {
            QMessageBox::warning(nullptr, tr("出错！"), tr("保存配置文件出错！"));
        }
    }
}


setting::setting(QObject* parent) : QObject(parent)
{
    loadSetting();
}

qint64 setting::toRealSpeed(SPEED s)
{
    switch(s)
    {
        case SPEED::NONE:
            return 0;
        case SPEED::KB50:
            return 50000;
        case SPEED::KB100:
            return 100000;
        case SPEED::KB200:
            return 200000;
        case SPEED::KB500:
            return 500000;
        case SPEED::MB1:
            return 1000000;
        case SPEED::MB2:
            return 2000000;
        case SPEED::MB5:
            return 5000000;
        case SPEED::MB10:
            return 1000000;
    }
}

int setting::toRealCount(setting::TASK_COUNT t)
{
    switch(t)
    {
        case TASK_COUNT::NONE:
            return 0;
        case TASK_COUNT::COUNT1:
            return 1;
        case TASK_COUNT::COUNT2:
            return 2;
        case TASK_COUNT::COUNT3:
            return 3;
        case TASK_COUNT::COUNT4:
            return 4;
        case TASK_COUNT::COUNT5:
            return 5;
        case TASK_COUNT::COUNT10:
            return 10;
    };
}


QDataStream& operator<<(QDataStream& fout, const setting& s)
{
    fout.writeRawData(reinterpret_cast<const char*>(&s.limitDownloadSpeed), sizeof(setting::SPEED));
    fout.writeRawData(reinterpret_cast<const char*>(&s.limitUploadSpeed), sizeof(setting::SPEED));
    fout.writeRawData(reinterpret_cast<const char*>(&s.singleLimitDownloadSpeed), sizeof(setting::SPEED));
    fout.writeRawData(reinterpret_cast<const char*>(&s.singleLimitUploadSpeed), sizeof(setting::SPEED));
    fout.writeRawData(reinterpret_cast<const char*>(&s.downloadTaskCount), sizeof(setting::TASK_COUNT));
    fout.writeRawData(reinterpret_cast<const char*>(&s.uploadTaskCount), sizeof(setting::TASK_COUNT));


    fout << s.serverAddress << s.downloadDir << s.syncDir << QString("DataIsIntegrity");
    return fout;
}
QDataStream& operator>>(QDataStream& fin, setting& s)
{

    fin.readRawData(reinterpret_cast<char*>(&s.limitDownloadSpeed), sizeof(setting::SPEED));
    fin.readRawData(reinterpret_cast<char*>(&s.limitUploadSpeed), sizeof(setting::SPEED));
    fin.readRawData(reinterpret_cast<char*>(&s.singleLimitDownloadSpeed), sizeof(setting::SPEED));
    fin.readRawData(reinterpret_cast<char*>(&s.singleLimitUploadSpeed), sizeof(setting::SPEED));
    fin.readRawData(reinterpret_cast<char*>(&s.downloadTaskCount), sizeof(setting::TASK_COUNT));
    fin.readRawData(reinterpret_cast<char*>(&s.uploadTaskCount), sizeof(setting::TASK_COUNT));

    QString  tmp;
    fin >> s.serverAddress >> s.downloadDir >> s.syncDir >> tmp;
    if(tmp != "DataIsIntegrity")
    {
        s.reinit();
    }
    return fin;
}

void setting::setSingleLimitDownloadSpeed(int value)
{
    if(singleLimitDownloadSpeed != SPEED(value))
    {
        singleLimitDownloadSpeed = SPEED(value);
        hasChanged = true;
    }
}

void setting::setSingleLimitUploadSpeed(int value)
{
    if(singleLimitUploadSpeed != SPEED(value))
    {
        singleLimitUploadSpeed = SPEED(value);
        hasChanged = true;
    }
}

void setting::setLimitDownloadSpeed(int value)
{
    if(limitDownloadSpeed != SPEED(value))
    {
        limitDownloadSpeed = SPEED(value);
        hasChanged = true;
    }
}

void setting::setLimitUploadSpeed(int value)
{
    if(limitUploadSpeed != SPEED(value))
    {
        limitUploadSpeed = SPEED(value);
        hasChanged = true;
    }
}

void setting::setDownloadTaskCount(int value)
{
    if(downloadTaskCount != TASK_COUNT(value))
    {
        downloadTaskCount = TASK_COUNT(value);
        hasChanged = true;
    }
}

void setting::setUploadTaskCount(int value)
{
    if(uploadTaskCount != TASK_COUNT(value))
    {
        uploadTaskCount = TASK_COUNT(value);
        hasChanged = true;
    }
}
void setting::setDownloadDir(QString value)
{
    if(downloadDir != value)
    {
        downloadDir = value;
        hasChanged = true;
    }
}

void setting::setSyncDir(QString value)
{
    if(syncDir != value)
    {
        syncDir = value;
        hasChanged = true;
    }
}

void setting::setServerAddress(const QString value)
{
    if(serverAddress != value)
    {
        tmpServerAddress = value;
        hasChanged = true;
    }
    else
    {
        tmpServerAddress = serverAddress;
    }
}
