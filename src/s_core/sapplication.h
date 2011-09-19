#ifndef SAPPLICATION_H
#define SAPPLICATION_H

#include "filesystem.h"
#include "sproject.h"
#include "cmainwindow.h"

#include <QApplication>
#include <QSettings>
#include <QIcon>
#include <QDebug>
#include <QFile>
#include <QtHelp/QHelpEngine>

#define VERSION "1.0.0"
#define SAFE_DELETE(p) ({delete p; p = NULL;})

class SApplication {
public:
    SApplication(int argc, char *args[]);
    ~SApplication();

    static SApplication *inst();

    QApplication *app();
    bool setResourcePackage(const QString &filename);
    QIcon iconResource(QString name);

    QVariant settings(const QString &key, const QVariant &defaultValue = QVariant());
    void writeSettings(const QString &key, const QVariant &value);
    void installTranslator(QTranslator *translator);
    void setMainWindow(CMainWindow *value);
    CMainWindow *mainWindow();

    QObject *project();
    QByteArray helpData(QString name);
    QHelpEngine *assistantEngine();
    QWidget *helpViewWidget(bool makeControls = false);

private:
    static SApplication *s_App;
    QHelpEngine *helpEngine;
    QApplication *s_app;
    FSHANDLE *resource;
    QObject *s_project;
    QWidget *s_helpView;
    CMainWindow *mainWnd;
    QFile *log;
};
#endif
