#include "clouddiskwindow.h"
#include "ui_clouddiskwindow.h"

CloudDiskWindow::CloudDiskWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::CloudDiskWindow)
{
    ui->setupUi(this);


    // 创建下载线程
    downloadThreadWorker = new DownloadThreadWorker();
    downloadThreadWorker->moveToThread(&downloadThread);

    // 线程操作相关信号槽
    connect(this, &CloudDiskWindow::runDownloadThread, downloadThreadWorker, &DownloadThreadWorker::continuousReadData);
    connect(this, &CloudDiskWindow::startDownloadThread, downloadThreadWorker, &DownloadThreadWorker::startWork);
    connect(this, &CloudDiskWindow::stopDownloadThread, downloadThreadWorker, &DownloadThreadWorker::stopWork);
    connect(&downloadThread, &QThread::finished, downloadThreadWorker, &QObject::deleteLater);

    // 线程中运行的函数相关信号槽
    connect(ui->show_panel, &ShowPanel::createDownloadItem, downloadThreadWorker, &DownloadThreadWorker::createDownloadItem);
    connect(downloadThreadWorker, &DownloadThreadWorker::createDownloadTask, ui->show_panel, &ShowPanel::createDownloadTask);
    connect(downloadThreadWorker, &DownloadThreadWorker::readData, [ = ] {updateDownloadView();});
    connect(downloadThreadWorker, &DownloadThreadWorker::createDownloadTask, this, &CloudDiskWindow::updateDownloadView);
    downloadThread.start();
    emit runDownloadThread();

    // 创建下载界面的Model和View
    downloadItemModel = new TransferItem(downloadThreadWorker->getData(), this);
    downloadDelegate = new TransferItemProcessingDelegate(this);

    // 设置下载界面属性
    ui->tableView_download->setModel(downloadItemModel);
    ui->tableView_download->setItemDelegate(downloadDelegate);

    // 设置下载界面比例
    ui->tableView_download->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableView_download->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableView_download->resizeColumnsToContents();
    ui->tableView_download->setMouseTracking(true);

    // 下载界面操作
//    connect(ui->show_panel, SIGNAL(addDownloadFile(Obj_Transfer*)), downloadItemModel, SLOT(addData(Obj_Transfer*)));
//    connect(ui->show_panel, SIGNAL(addDownloadFile(Obj_Transfer*)), this, SLOT(updateDownloadView()));
    connect(downloadThreadWorker, &DownloadThreadWorker::updateView, downloadItemModel, &TransferItem::update);
    connect(downloadThreadWorker, &DownloadThreadWorker::createDownloadTask, downloadItemModel, &TransferItem::update);
    connect(downloadDelegate, &TransferItemProcessingDelegate::signals1, downloadThreadWorker, &DownloadThreadWorker::switchDownloadTaskStatus);
//    connect(downloadDelegate, &TransferItemProcessingDelegate::signals2, downloadThreadWorker, &DownloadThreadWorker::deleteDownloadTask);
    connect(downloadDelegate, &TransferItemProcessingDelegate::signals2, [ = ](const QModelIndex & index)
    {
        downloadThreadWorker->deleteDownloadTask(index);
    });
    connect(downloadDelegate, &TransferItemProcessingDelegate::signals3, downloadThreadWorker, &DownloadThreadWorker::openDownloadFileDir);







    // 创建上传线程
    uploadThreadWorker = new UploadThreadWorker();
    uploadThreadWorker->moveToThread(&uploadThread);

    // 创建上传界面的Model和View
    uploadItemModel = new TransferItem(uploadThreadWorker->getData(), this);
    uploadDelegate = new TransferItemProcessingDelegate(this);

    // 设置上传界面属性
    ui->tableView_upload->setModel(uploadItemModel);
    ui->tableView_upload->setItemDelegate(uploadDelegate);

    // 测试上传界面
    Obj_Transfer* test = new Obj_Transfer(false, new Obj_File(nullptr, 4, "33333", 1, QDate()), nullptr,  100);
    test->setTransferedSize(39);
    uploadItemModel->addData(test);
    uploadItemModel->addData(new Obj_Transfer(false, new Obj_File(nullptr, 3, "8hrht", 1, QDate()), nullptr,  150));

    // 设置上传界面比例
    ui->tableView_upload->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableView_upload->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableView_upload->resizeColumnsToContents();
    ui->tableView_upload->setMouseTracking(true);

    // 上传界面操作



    // 创建完成界面的Model和View
    finishedItemModel = new TransferItem(this);
    finishedDelegate = new TransferItemProcessingDelegate(this);

    // 设置完成界面属性
    ui->tableView_finished->setModel(finishedItemModel);
    ui->tableView_finished->setItemDelegate(finishedDelegate);

    // 测试完成界面
    Obj_Transfer* test3 = new Obj_Transfer(false, new Obj_File(nullptr, 4, "33333", 1, QDate()), nullptr,  100);
    test3->setTransferedSize(100);
    test3->setFinished(true);
    finishedItemModel->addData(test3);

//    // 设置完成界面比例
    ui->tableView_finished->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableView_finished->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableView_finished->resizeColumnsToContents();
    ui->tableView_finished->setMouseTracking(true);

    // 完成界面操作
    connect(downloadThreadWorker, &DownloadThreadWorker::movetoFinishedView, finishedItemModel, &TransferItem::addData);
//    connect(downloadThreadWorker, &DownloadThreadWorker::movetoFinishedView, [ = ] {updateFinishedView();});


    connect(ui->action_config, SIGNAL(triggered()), this, SLOT(showSettingDialog()));
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
    downloadThread.quit();
    downloadThread.wait();
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

void CloudDiskWindow::showSettingDialog()
{
    setting::GetInstance()->loadSetting();
    if(settingDialog.exec() == QDialog::Accepted)
    {
        qDebug() << "save save save";
        setting::GetInstance()->applySetting();
        setting::GetInstance()->saveSetting();
    }
}
