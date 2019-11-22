#include "login.h"
#include "ui_login.h"

LogIn::LogIn(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::LogIn)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    connect(ui->login, SIGNAL(clicked()), this, SLOT(login()));
    connect(ServerConnect::getInstance().getNetwordAccessManager(), SIGNAL(finished(QNetworkReply*)), this, SLOT(response(QNetworkReply*)));
}


LogIn::~LogIn()
{
    delete ui;
}
void LogIn::login()
{
    QMap<QString, QString> datum;
    datum["email"] = ui->lineEdit_email->text();
    datum["password"] = ui->lineEdit_password->text();

    QJsonObject data;
    QMapIterator<QString, QString> iter(datum);
    while (iter.hasNext())
    {
        iter.next();
        data.insert(iter.key(), iter.value());
    }
    QNetworkReply* reply = ServerConnect::getInstance().http_post("/login", QJsonDocument(data));
    replyMap.insert(reply, requestType::LOGIN);
}


void LogIn::response(QNetworkReply* reply)
{
    if(replyMap.contains(reply))
    {
        switch(replyMap[reply])
        {
            case requestType::AUTH:
                {
                    auto data = reply->readAll();
                    qDebug() << data;
                    replyMap.remove(reply);
                    reply->deleteLater();
                }
                break;
            case requestType::LOGIN:
                {
                    auto data = reply->readAll();
                    auto jsonData = QJsonDocument::fromJson(data).object();
                    if (jsonData["message"].toString() == "OK")
                        accept();
                    else
                    {
                        qDebug() << "unauthorized!";
                    }
                    replyMap.remove(reply);
                    reply->deleteLater();
                }
                break;
        }
    }
}
