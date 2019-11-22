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
#include <QJsonDocument>

class ServerConnect : QObject
{
    Q_OBJECT
public:

private:
    static const QString address;
    static ServerConnect* serverConnect;
    static QNetworkAccessManager* accessManager;


public:
    ServerConnect();
    ~ServerConnect();

    static ServerConnect& getInstance();
    QNetworkAccessManager* getNetwordAccessManager();

    QNetworkReply* http_get(QString url, QMap<QString, QString> param = QMap<QString, QString>());
    QNetworkReply* http_post(QString url, QJsonDocument jsonData = QJsonDocument());
    QNetworkReply* http_delete(QString url);
    QNetworkReply* http_patch(QString url, QJsonDocument jsonData);

private:
    void CreateConnect();

public slots:
    void requestCallback(QNetworkReply* reply);
};

#endif // SERVERCONNECT_H
