#include "sapplication.h"
#include "mainwindow.h"
//#include <QTranslator>

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
    qDebug() << "Create main window";
    MainWindow w;
    qDebug() << "Show main window";
    w.show();
            //.showExpanded();
    qDebug() << "end show main window";

    a.setMainWindow(&w);

    return a.app()->exec();
}
