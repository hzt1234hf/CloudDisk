#include "clouddiskwindow.h"
#include "ui_clouddiskwindow.h"

CloudDiskWindow::CloudDiskWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::CloudDiskWindow)
{
    ui->setupUi(this);
    connect(ui->action_add, SIGNAL(triggered()), ui->show_panel, SLOT(add()));
    connect(ui->show_panel, SIGNAL(enableBackbtn(bool)), ui->action_back, SLOT(setEnabled(bool)));
    connect(ui->show_panel, SIGNAL(enableUpperbtn(bool)), ui->action_upper, SLOT(setEnabled(bool)));
    connect(ui->action_back, SIGNAL(triggered()), ui->show_panel, SLOT(getLastFolderInfo()));
    connect(ui->action_upper, SIGNAL(triggered()), ui->show_panel, SLOT(getUpperFolderInfo()));
}

CloudDiskWindow::~CloudDiskWindow()
{
    delete ui;
}
