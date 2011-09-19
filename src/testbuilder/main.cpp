﻿#include "sapplication.h"
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
    //a.app()->setStyle("WindowsModernStyle");
    a.setResourcePackage("./data.sarc");

    QIcon::setThemeName("Depot_XP");
    MainWindow w;
    w.show();

    a.setMainWindow(&w);

    return a.app()->exec();
}
