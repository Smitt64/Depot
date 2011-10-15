#ifndef SAPPLICATION_H
#define SAPPLICATION_H

#include "s_core_global.h"
#include "filesystem.h"
#include "sproject.h"
#include "cmainwindow.h"

#include <QtSql>
#include <QApplication>
#include <QSettings>
#include <QIcon>
#include <QDebug>
#include <QFile>

#ifndef S_OS_MEEGO
#include <QtHelp/QHelpEngine>
#endif

#define VERSION "1.0.0"
#define SAFE_DELETE(p) if((p) != NULL) delete(p); (p) = NULL;

class S_CORESHARED_EXPORT SApplication : public QObject {
    Q_OBJECT
public:
    explicit SApplication(int argc, char *args[], QObject *parent = 0);
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

    bool openDataBase();
    QString lastDatabaseError() const;

    QObject *project();
#ifndef S_OS_MEEGO
    QByteArray helpData(QString name);
    QHelpEngine *assistantEngine();
    QWidget *helpViewWidget(bool makeControls = false);
#endif

signals:
    void db_error(int err_type, QString msg1, QString msg2);

private:
    static SApplication *s_App;
#ifndef S_OS_MEEGO
    QHelpEngine *helpEngine;
    QWidget *s_helpView;
#endif
    QApplication *s_app;
    FSHANDLE *resource;
    QObject *s_project;
    CMainWindow *mainWnd;
    QFile *log;
    QSqlDatabase db;
    QString last_db_errorMsg;
};
#endif
