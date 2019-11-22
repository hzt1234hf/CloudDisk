#include "clouddiskwindow.h"
#include "ui_clouddiskwindow.h"

CloudDiskWindow::CloudDiskWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CloudDiskWindow)
{
    ui->setupUi(this);
    connect(ui->action_A,SIGNAL(triggered()),ui->show_panel,SLOT(add()));
}

CloudDiskWindow::~CloudDiskWindow()
{
    delete ui;
}
