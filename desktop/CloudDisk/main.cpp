#include <QApplication>

#include "clouddiskwindow.h"
#include "login.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LogIn login;
    CloudDiskWindow w;
    login.show();
    if(login.exec() == QDialog::Accepted)
    {
        w.show();
    }


    return a.exec();
}
