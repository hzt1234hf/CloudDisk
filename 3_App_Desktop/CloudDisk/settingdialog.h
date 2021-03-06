#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include <QFileDialog>

#include "class_global/setting.h"

namespace Ui
{
    class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget* parent = nullptr);
    ~SettingDialog();

private:
    Ui::SettingDialog* ui;
};

#endif // SETTINGDIALOG_H
