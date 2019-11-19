#include "login.h"
#include "ui_login.h"

LogIn::LogIn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogIn)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    Response = [](QNetworkReply* reply){
        auto data = reply->readAll();
        auto jsonData = QJsonDocument::fromJson(data).object();
        qDebug()<<QJsonDocument::fromJson(data);
        for(auto iter=jsonData.constBegin();iter!=jsonData.constEnd();++iter)
        {
            qDebug()<<iter.key()<<"----------------------"<<iter.value();
        }
    };
    connect(ui->login,SIGNAL(clicked()),this,SLOT(login()));
}


LogIn::~LogIn()
{
    delete ui;
}

void LogIn::login()
{
    QMap<QString,QString> datum;
    datum["email"] = ui->lineEdit_email->text();
    datum["password"] = ui->lineEdit_password->text();
//    ServerConnect::getInstance().post("/login",datum,Response);
//    ServerConnect::getInstance().test();

    QNetworkAccessManager* accessManager = new QNetworkAccessManager();
    QMetaObject::Connection connection = QObject::connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(response(QNetworkReply*)));
    Q_ASSERT(connection);


    QNetworkRequest request;
    request.setUrl(QUrl("http://localhost:5000/login"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject data;
    QMapIterator<QString, QString> iter(datum);
    while (iter.hasNext()) {
        iter.next();
        data.insert(iter.key(),iter.value());
    }
    QNetworkReply* reply = accessManager->post(request, QJsonDocument(data).toJson());
}

void LogIn::response(QNetworkReply *reply)
{
    auto data = reply->readAll();
    auto jsonData = QJsonDocument::fromJson(data).object();
    qDebug()<<QJsonDocument::fromJson(data);
    for(auto iter=jsonData.constBegin();iter!=jsonData.constEnd();++iter)
    {
        qDebug()<<iter.key()<<"----------------------"<<iter.value();
    }
    accept();
}
