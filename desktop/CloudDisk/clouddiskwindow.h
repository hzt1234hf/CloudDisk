#ifndef CLOUDDISKWINDOW_H
#define CLOUDDISKWINDOW_H

#include <QMainWindow>

namespace Ui {
class CloudDiskWindow;
}

class CloudDiskWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CloudDiskWindow(QWidget *parent = nullptr);
    ~CloudDiskWindow();

private:
    Ui::CloudDiskWindow *ui;
};

#endif // CLOUDDISKWINDOW_H
