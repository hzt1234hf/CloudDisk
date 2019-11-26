#include "clouddiskwindow.h"
#include "ui_clouddiskwindow.h"

CloudDiskWindow::CloudDiskWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::CloudDiskWindow)
{
    ui->setupUi(this);
    connect(ui->action_upload, SIGNAL(triggered()), ui->show_panel, SLOT(add()));
    connect(ui->show_panel, SIGNAL(enableBackbtn(bool)), ui->action_back, SLOT(setEnabled(bool)));
    connect(ui->show_panel, SIGNAL(enableUpperbtn(bool)), ui->action_upper, SLOT(setEnabled(bool)));
    connect(ui->show_panel, SIGNAL(enableObjbtn(bool)), this, SLOT(enableObjBtn(bool)));
    connect(ui->action_back, SIGNAL(triggered()), ui->show_panel, SLOT(getLastFolderInfo()));
    connect(ui->action_upper, SIGNAL(triggered()), ui->show_panel, SLOT(getUpperFolderInfo()));
    connect(ui->action_refresh, SIGNAL(triggered()), ui->show_panel, SLOT(refresh()));

    ui->show_panel->objToolPalette->addAction(ui->action_delete);
    ui->show_panel->objToolPalette->addAction(ui->action_download);
    ui->show_panel->objToolPalette->addAction(ui->action_share);
    ui->show_panel->objToolPalette->addAction(ui->action_sync);

    ui->show_panel->panelToolPalette->addAction(ui->action_back);
    ui->show_panel->panelToolPalette->addAction(ui->action_upper);
    ui->show_panel->panelToolPalette->addAction(ui->action_addfolder);
    ui->show_panel->panelToolPalette->addAction(ui->action_refresh);
    ui->show_panel->panelToolPalette->addAction(ui->action_upload);
    ui->show_panel->panelToolPalette->addAction(ui->action_config);

}

CloudDiskWindow::~CloudDiskWindow()
{
    delete ui;
}

void CloudDiskWindow::enableObjBtn(bool flag)
{
    ui->action_delete->setEnabled(flag);
    ui->action_download->setEnabled(flag);
    ui->action_share->setEnabled(flag);
    ui->action_sync->setEnabled(flag);
}
