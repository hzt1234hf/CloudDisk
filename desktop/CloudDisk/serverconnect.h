#ifndef SERVERCONNECT_H
#define SERVERCONNECT_H

#include <QMap>
#include <QObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>

#include <QAbstractNetworkCache>

class ServerConnect : QObject
{
    Q_OBJECT
private:
    static const QString address;
    static ServerConnect * serverConnect;
    static QNetworkAccessManager* accessManager;
    void (*CallBackFunction)(QNetworkReply*);

public:
    ServerConnect();
    ~ServerConnect();

    static ServerConnect & getInstance();

    QNetworkReply* post(QString url,QMap<QString,QString> datum,void (*func)(QNetworkReply*));
    QNetworkReply* get(QString url,QMap<QString,QString> datum,void (*func)(QNetworkReply*));
    void test();

private:
    void CreateConnect();

public slots:
    void requestCallback(QNetworkReply* reply);
};

#endif // SERVERCONNECT_H
