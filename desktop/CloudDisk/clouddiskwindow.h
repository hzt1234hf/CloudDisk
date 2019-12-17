#ifndef CLOUDDISKWINDOW_H
#define CLOUDDISKWINDOW_H

#include <QMainWindow>
#include <QHeaderView>
#include "class_obj/transferitem.h"
#include "class_obj/transferitemprocessingdelegate.h"
#include "class_thread/downloadthreadworker.h"
#include "class_thread/uploadthreadworker.h"

#include "login.h"

namespace Ui
{
    class CloudDiskWindow;
}

class CloudDiskWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CloudDiskWindow(QWidget* parent = nullptr);
    ~CloudDiskWindow();

public:
    QThread downloadThread;
    DownloadThreadWorker* downloadThreadWorker;// 线程

    QThread uploadThread;
    UploadThreadWorker* uploadThreadWorker;

private:
    Ui::CloudDiskWindow* ui;
    TransferItem* downloadItemModel;
    TransferItem* uploadItemModel;
    TransferItemProcessingDelegate* downloadDelegate;
    TransferItemProcessingDelegate* uploadDelegate;

signals:
    void runDownloadThread();
    void startDownloadThread();
    void stopDownloadThread();

protected slots:
    void enableObjBtn(bool);
    void updateDownloadView();
    void updateUploadloadView();
    void updateFinishedView();


};

#endif // CLOUDDISKWINDOW_H
