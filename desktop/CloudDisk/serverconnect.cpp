#include "serverconnect.h"

const QString ServerConnect::address = "http://127.0.0.1:5000";
ServerConnect* ServerConnect::serverConnect = nullptr;
QNetworkAccessManager* ServerConnect::accessManager = nullptr;

ServerConnect::ServerConnect()
{
    this->CreateConnect();
}

ServerConnect::~ServerConnect()
{
    if(serverConnect)
        delete serverConnect;
}

ServerConnect& ServerConnect::getInstance()
{
    if(ServerConnect::serverConnect == nullptr)
    {
        serverConnect = new ServerConnect();
    }
    return *serverConnect;
}

QNetworkAccessManager* ServerConnect::getNetwordAccessManager()
{
    return accessManager;
}



void ServerConnect::CreateConnect()
{
    if(accessManager)
    {
        delete accessManager;
        accessManager = nullptr;
    }
    accessManager = new QNetworkAccessManager();
    QMetaObject::Connection connection = QObject::connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestCallback(QNetworkReply*)));
    Q_ASSERT(connection);
}

void ServerConnect::requestCallback(QNetworkReply* reply)
{
    qDebug() << "--------------------------------------------------------------";
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(statusCode.isValid())
        qDebug() << "status code=" << statusCode.toInt();

    QVariant reason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
    if(reason.isValid())
        qDebug() << "reason=" << reason.toString();

    QNetworkReply::NetworkError err = reply->error();
    if(err != QNetworkReply::NoError)
    {
        qDebug() << "Failed: " << reply->errorString();
    }
    qDebug() << "--------------------------------------------------------------";
}
QNetworkReply* ServerConnect::http_get(QString url, QMap<QString, QString> param)
{
    QNetworkRequest request;
    QString data = "";
    QMapIterator<QString, QString> iter(param);
    while (iter.hasNext())
    {
        iter.next();
        if(data == "")
            data += iter.key() + '=' + iter.value();
        else
            data += '&' + iter.key() + '=' + iter.value();
    }
    if(data == "")
        request.setUrl(QUrl(ServerConnect::address + url));
    else
        request.setUrl(QUrl(ServerConnect::address + url + '?' + data));
    QNetworkReply* reply = accessManager->get(request);
    return reply;
}
QNetworkReply* ServerConnect::http_post(QString url, QJsonDocument jsonData)
{
    QNetworkRequest request;
    request.setUrl(QUrl(ServerConnect::address + url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply* reply = accessManager->post(request, jsonData.toJson());
    return reply;
}

QNetworkReply* ServerConnect::http_delete(QString url)
{
    QNetworkRequest request;
    request.setUrl(QUrl(ServerConnect::address + url));
    QNetworkReply* reply = accessManager->deleteResource(request);
    return reply;
}
QNetworkReply* ServerConnect::http_patch(QString url, QJsonDocument jsonData)
{
    QNetworkRequest request;
    request.setUrl(QUrl(ServerConnect::address + url));
    QNetworkReply* reply = accessManager->sendCustomRequest(request, QByteArray("PATCH"), jsonData.toJson());
    return reply;
}
