#include <QApplication>

#include "clouddiskwindow.h"
#include "login.h"


int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    CloudDiskWindow w;
    w.show();
    return a.exec();

    LogIn login;
    if(login.exec() == QDialog::Accepted)
    {
        CloudDiskWindow w;
        w.show();
        return a.exec();
    }
    return 0;
}
