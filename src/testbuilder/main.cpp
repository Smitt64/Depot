#include "sapplication.h"
#include "mainwindow.h"
#include <QTranslator>

int main(int argc, char **args)
{
    SApplication a(argc, args);

    QTranslator tr;
    tr.load("depot_ru_RU");
    a.installTranslator(&tr);

    if(QApplication::startingUp())
        return 0x01;
    a.setResourcePackage("./data.sarc");

    MainWindow w;
    w.show();

    return a.app()->exec();
}
