#include "clouddiskwindow.h"
#include "ui_clouddiskwindow.h"

CloudDiskWindow::CloudDiskWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::CloudDiskWindow)
{
    ui->setupUi(this);

    downloadItemModel = new TransferItem(this);
    uploadItemModel = new TransferItem(this);
    downloadDelegate = new TransferItemProcessingDelegate(this);
    uploadDelegate = new TransferItemProcessingDelegate(this);

    ui->tableView_download->setModel(downloadItemModel);
    ui->tableView_upload->setModel(uploadItemModel);
//    ui->tableView_finished->setModel(transferItemModel);



    ui->tableView_download->setItemDelegate(downloadDelegate);
    ui->tableView_upload->setItemDelegate(uploadDelegate);

    Obj_Transfer* test = new Obj_Transfer(false, new Obj_File(nullptr, 4, "33333", 1, QDate()), nullptr,  100);
    test->setReceivedSize(39);
    uploadItemModel->addData(test);
    uploadItemModel->addData(new Obj_Transfer(false, new Obj_File(nullptr, 3, "8hrht", 1, QDate()), nullptr,  150));


    ui->tableView_download->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableView_download->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableView_download->resizeColumnsToContents();
    ui->tableView_download->setMouseTracking(true);

    ui->tableView_upload->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableView_upload->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableView_upload->resizeColumnsToContents();
    ui->tableView_upload->setMouseTracking(true);




    connect(ui->show_panel, SIGNAL(addDownloadFile(Obj_Transfer*)), downloadItemModel, SLOT(addData(Obj_Transfer*)));
//    connect(ui->show_panel->downloadThreadWorker, SIGNAL(addDownloadItem(Obj_Transfer*)), downloadItemModel, SLOT(addData(Obj_Transfer*)));
    connect(ui->show_panel, SIGNAL(updateView()), this, SLOT(updateDownloadView()));
    connect(ui->show_panel, SIGNAL(addDownloadFile(Obj_Transfer*)), this, SLOT(updateDownloadView()));


    connect(ui->action_upload, SIGNAL(triggered()), ui->show_panel, SLOT(add()));
    connect(ui->show_panel, SIGNAL(enableBackbtn(bool)), ui->action_back, SLOT(setEnabled(bool)));
    connect(ui->show_panel, SIGNAL(enableBackbtn(bool)), ui->btn_back, SLOT(setEnabled(bool)));
    connect(ui->show_panel, SIGNAL(enableUpperbtn(bool)), ui->action_upper, SLOT(setEnabled(bool)));
    connect(ui->show_panel, SIGNAL(enableUpperbtn(bool)), ui->btn_upper, SLOT(setEnabled(bool)));
    connect(ui->show_panel, SIGNAL(enableObjbtn(bool)), this, SLOT(enableObjBtn(bool)));
    connect(ui->action_back, SIGNAL(triggered()), ui->show_panel, SLOT(getLastFolderInfo()));
    connect(ui->btn_back, SIGNAL(clicked()), ui->show_panel, SLOT(getLastFolderInfo()));
    connect(ui->action_upper, SIGNAL(triggered()), ui->show_panel, SLOT(getUpperFolderInfo()));
    connect(ui->btn_upper, SIGNAL(clicked()), ui->show_panel, SLOT(getUpperFolderInfo()));
    connect(ui->action_refresh, SIGNAL(triggered()), ui->show_panel, SLOT(refreshCurFolderInfo()));
    connect(ui->btn_refresh, SIGNAL(clicked()), ui->show_panel, SLOT(refreshCurFolderInfo()));
    connect(ui->action_addfolder, SIGNAL(triggered()), ui->show_panel, SLOT(addNewFolder()));
    connect(ui->btn_addfolder, SIGNAL(clicked()), ui->show_panel, SLOT(addNewFolder()));
    connect(ui->action_delete, SIGNAL(triggered()), ui->show_panel, SLOT(deleteObj()));
    connect(ui->btn_delete, SIGNAL(clicked()), ui->show_panel, SLOT(deleteObj()));
    connect(ui->action_upload, SIGNAL(triggered()), ui->show_panel, SLOT(uploadLocalFile()));
    connect(ui->btn_upload, SIGNAL(clicked()), ui->show_panel, SLOT(uploadLocalFile()));

    connect(ui->action_download, SIGNAL(triggered()), ui->show_panel, SLOT(downloadFile()));
    connect(ui->btn_download, SIGNAL(clicked()), ui->show_panel, SLOT(downloadFile()));

    ui->show_panel->objToolPalette->addAction(ui->action_delete);
    ui->show_panel->objToolPalette->addAction(ui->action_download);
    ui->show_panel->objToolPalette->addAction(ui->action_share);
    ui->show_panel->objToolPalette->addAction(ui->action_sync);

    ui->show_panel->panelToolPalette->addAction(ui->action_back);
    ui->show_panel->panelToolPalette->addAction(ui->action_upper);
    ui->show_panel->panelToolPalette->addAction(ui->action_addfolder);
    ui->show_panel->panelToolPalette->addAction(ui->action_refresh);
    ui->show_panel->panelToolPalette->addAction(ui->action_upload);


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
    ui->btn_delete->setEnabled(flag);
    ui->btn_download->setEnabled(flag);
    ui->btn_share->setEnabled(flag);
    ui->btn_sync->setEnabled(flag);
}

void CloudDiskWindow::updateDownloadView()
{
    qDebug() << "3333333333333333";
    ui->tableView_download->resizeColumnsToContents();
    ui->tableView_download->resizeRowsToContents();
}

void CloudDiskWindow::updateUploadloadView()
{
    ui->tableView_upload->resizeColumnsToContents();
    ui->tableView_upload->resizeRowsToContents();
}

void CloudDiskWindow::updateFinishedView()
{
    ui->tableView_finished->resizeColumnsToContents();
    ui->tableView_finished->resizeRowsToContents();
}
