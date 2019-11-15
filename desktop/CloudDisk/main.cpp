#include <QApplication>

#include "clouddiskwindow.h"
#include "login.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LogIn login;
    login.show();
//    CloudDiskWindow w;
//    w.show();

    return a.exec();
}
