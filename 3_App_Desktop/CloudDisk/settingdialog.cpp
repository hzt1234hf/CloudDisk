#include "settingdialog.h"
#include "ui_settingdialog.h"

SettingDialog::SettingDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);
    setting* tmp = setting::GetInstance();


    ui->comboBox_download_count->setCurrentIndex(tmp->getDownloadTaskCountIndex());
    ui->comboBox_download_limit->setCurrentIndex(tmp->getLimitDownloadSpeedIndex());
    ui->comboBox_download_singlelimit->setCurrentIndex(tmp->getSingleLimitDownloadSpeedIndex());
    ui->comboBox_upload_count->setCurrentIndex(tmp->getUploadTaskCountIndex());
    ui->comboBox_upload_limit->setCurrentIndex(tmp->getLimitUploadSpeedIndex());
    ui->comboBox_upload_singlelimit->setCurrentIndex(tmp->getSingleLimitUploadSpeedIndex());

    ui->lineEdit_downalod->setText(tmp->getDownloadDir());
    ui->lineEdit_sync->setText(tmp->getSyncDir());


    connect(ui->comboBox_upload_count, SIGNAL(currentIndexChanged(int)), setting::GetInstance(), SLOT(setUploadTaskCount(int)));
    connect(ui->comboBox_download_count, SIGNAL(currentIndexChanged(int)), setting::GetInstance(), SLOT(setDownloadTaskCount(int)));
    connect(ui->comboBox_upload_singlelimit, SIGNAL(currentIndexChanged(int)), setting::GetInstance(), SLOT(setSingleLimitUploadSpeed(int)));
    connect(ui->comboBox_download_singlelimit, SIGNAL(currentIndexChanged(int)), setting::GetInstance(), SLOT(setSingleLimitDownloadSpeed(int)));
    connect(ui->comboBox_upload_limit, SIGNAL(currentIndexChanged(int)), setting::GetInstance(), SLOT(setLimitUploadSpeed(int)));
    connect(ui->comboBox_download_limit, SIGNAL(currentIndexChanged(int)), setting::GetInstance(), SLOT(setLimitDownloadSpeed(int)));
    connect(ui->lineEdit_address, SIGNAL(textChanged(const QString)), setting::GetInstance(), SLOT(setServerAddress(const QString)));
    connect(ui->pushButton_download, &QPushButton::clicked, [ = ]
    {
        QString path = QFileDialog::getExistingDirectory(nullptr, tr("选择目录"));
        qDebug() << path;
        tmp->setDownloadDir(path);
        ui->lineEdit_downalod->setText(path);
    });
    connect(ui->pushButton_sync, &QPushButton::clicked, [ = ]
    {
        QString path = QFileDialog::getExistingDirectory(nullptr, tr("选择目录"));
        tmp->setSyncDir(path);
        ui->lineEdit_sync->setText(path);
    });
}

SettingDialog::~SettingDialog()
{
    delete ui;
}
