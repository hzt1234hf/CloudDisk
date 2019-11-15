#include "login.h"
#include "ui_login.h"

LogIn::LogIn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogIn)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

LogIn::~LogIn()
{
    delete ui;
}
