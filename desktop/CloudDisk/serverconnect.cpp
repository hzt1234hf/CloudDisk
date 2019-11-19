#include "serverconnect.h"

const QString ServerConnect::address = "http://127.0.0.1:5000";
ServerConnect * ServerConnect::serverConnect = nullptr;
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

ServerConnect & ServerConnect::getInstance()
{
    if(ServerConnect::serverConnect == nullptr){
        serverConnect = new ServerConnect();
    }
    return *serverConnect;
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
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(statusCode.isValid())
        qDebug() << "status code=" << statusCode.toInt();

    QVariant reason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
    if(reason.isValid())
        qDebug() << "reason=" << reason.toString();

    QNetworkReply::NetworkError err = reply->error();
    if(err != QNetworkReply::NoError) {
        qDebug() << "Failed: " << reply->errorString();
    }
    else {
        qDebug() << reply->readAll();
    }
    if(CallBackFunction != nullptr)
    {
        CallBackFunction(reply);
        CallBackFunction = nullptr;
    }
}

void ServerConnect::test()
{
    QNetworkRequest request;
    request.setUrl(QUrl(ServerConnect::address + "/test"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject obj;
    obj.insert("testdata", "abc");
    QNetworkReply* reply = accessManager->post(request, QJsonDocument(obj).toJson());
}

QNetworkReply *ServerConnect::post(QString url, QMap<QString, QString> datum,void (*func)(QNetworkReply*))
{
    QNetworkRequest request;
    request.setUrl(QUrl(ServerConnect::address + url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject data;
    QMapIterator<QString, QString> iter(datum);
    while (iter.hasNext()) {
        iter.next();
        data.insert(iter.key(),iter.value());
    }
    CallBackFunction = func;
    QNetworkReply* reply = accessManager->post(request, QJsonDocument(data).toJson());

}

QNetworkReply *ServerConnect::get(QString url, QMap<QString, QString> datum,void (*func)(QNetworkReply*))
{
    QNetworkRequest request;

    QString data = "";
    QMapIterator<QString, QString> iter(datum);
    while (iter.hasNext()) {
        iter.next();
        qDebug() << iter.key()<<"  "<<iter.value();
        if(data == "")
            data += iter.key()+'='+iter.value();
        else
            data += '&'+iter.key()+'='+iter.value();
    }
    if(data == "")
        request.setUrl(QUrl(ServerConnect::address + url));
    else
        request.setUrl(QUrl(ServerConnect::address + url + '?' + data));
    CallBackFunction = func;
    QNetworkReply* reply = accessManager->get(request);
}



