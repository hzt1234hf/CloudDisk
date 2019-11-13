#include "clouddiskwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CloudDiskWindow w;
    w.show();

    return a.exec();
}
