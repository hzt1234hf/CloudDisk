#ifndef CLOUDDISKWINDOW_H
#define CLOUDDISKWINDOW_H

#include <QMainWindow>
#include <QHeaderView>
#include "class_obj/transferitem.h"
#include "class_obj/transferitemprocessingdelegate.h"

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


private:
    Ui::CloudDiskWindow* ui;
    TransferItem* downloadItemModel;
    TransferItem* uploadItemModel;
    TransferItemProcessingDelegate* downloadDelegate;
    TransferItemProcessingDelegate* uploadDelegate;

protected slots:
    void enableObjBtn(bool);
    void updateDownloadView();
    void updateUploadloadView();
    void updateFinishedView();
};

#endif // CLOUDDISKWINDOW_H
