#ifndef SAPPLICATION_H
#define SAPPLICATION_H

#include "filesystem.h"
#include "sproject.h"
#include "cmainwindow.h"

#include <QApplication>
#include <QSettings>
#include <QIcon>
#include <QDebug>

#define VERSION "1.0.0"
#define SAFE_DELETE(p) ({delete p; p = NULL;})

class SApplication {
public:
    SApplication(int argc, char **args);
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

private:
    static SApplication *s_App;
    QApplication *s_app;
    FSHANDLE *resource;
    QObject *s_project;
    CMainWindow *mainWnd;
};
#endif
