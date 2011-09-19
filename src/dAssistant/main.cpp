#include <QtGui/QApplication>
#include "mainwindow.h"
#include "sapplication.h"

int main(int argc, char *argv[])
{
    SApplication a(argc, argv);

    QTranslator tr;
    tr.load("depot_ru_RU");
    a.installTranslator(&tr);

    if(QApplication::startingUp())
        return 0x01;

    a.setResourcePackage("./data.sarc");

    MainWindow w;
    w.show();

    a.setMainWindow(&w);

    return a.app()->exec();
}
