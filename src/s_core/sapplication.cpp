﻿#include "sapplication.h"
#include "sproject.h"
#ifndef S_OS_MEEGO
#include "shelpcontentviewwidget.h"
#endif
#include <QMessageBox>

SApplication *SApplication::s_App = NULL;

SApplication::SApplication(int argc, char *args[], QObject *parent) :
    QObject(parent),
    s_project(NULL),
    mainWnd(NULL),
    last_db_errorMsg(QString::null)
#ifndef S_OS_MEEGO
    ,s_helpView(NULL),
    helpEngine(NULL)
#endif
{
    QCoreApplication::setOrganizationName("Serpkov_Nikita");
    QCoreApplication::setApplicationName("TestBuilder");
    QCoreApplication::setApplicationVersion(VERSION);

    s_app = new QApplication(argc, args);
    log = new QFile("log.txt");
    bool opened = log->open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream stream(log);

#ifndef S_OS_MEEGO
    helpEngine = new QHelpEngine("../doc/depot_doc_collection.qhc");
    stream << QDir::current().path() << helpEngine->error();
#endif
    QSettings set;

    resource = new FSHANDLE;

    s_App = this;
}

SApplication::~SApplication() {
    s_app = NULL;
    SAFE_DELETE(resource);
}

SApplication *SApplication::inst() {
    if(!s_App) {
        s_App = new SApplication(0, NULL);
    }

   return s_App;
}

QApplication *SApplication::app() {
    return s_app;
}

bool SApplication::setResourcePackage(const QString &filename) {
    /*bool hr = FileSystem::getInst()->fsOpen(filename, resource);
    FileSystem::getInst()->fsClose(resource);
    return hr;*/
    QResource resource(filename);
    return resource.registerResource(filename);
}

QIcon SApplication::iconResource(QString name) {
    QPixmap img;
    FileSystem::getInst()->fsOpen(resource);
    img.loadFromData(FileSystem::getInst()->fsGetFile(name, resource));
    FileSystem::getInst()->fsClose(resource);
    return QIcon(img);
}

QVariant SApplication::settings(const QString &key, const QVariant &defaultValue) {
    QSettings set;
    return set.value(key, defaultValue);
}

void SApplication::writeSettings(const QString &key, const QVariant &value) {
    QSettings set;
    set.setValue(key, value);
}

void SApplication::installTranslator(QTranslator *translator) {
    QCoreApplication::installTranslator(translator);
}

QObject *SApplication::project() {
    if(!s_project)
        s_project = new SProject;

    return (QObject*)s_project;
}

void SApplication::setMainWindow(CMainWindow *value) {
    mainWnd = value;
}

CMainWindow *SApplication::mainWindow() {
    return mainWnd;
}

#ifndef S_OS_MEEGO
QByteArray SApplication::helpData(QString name) {
    return helpEngine->fileData(QUrl(name));
}

QWidget *SApplication::helpViewWidget(bool makeControls) {
    HelpView *hlp = new HelpView(makeControls);
    return (QWidget*)hlp;
}

QHelpEngine *SApplication::assistantEngine() {
    return helpEngine;
}
#endif

bool SApplication::openDataBase() {
    QString data_b = settings("database/driver", "QSQLITE").toString();
    db = QSqlDatabase::addDatabase(data_b);

    bool hr = false;
    if(data_b == "QSQLITE") {
        db.setDatabaseName("../share/depot.sqlite");
    } else {
        db.setDatabaseName(settings("database/databasename").toString());
        db.setHostName(settings("database/hostname").toString());
        db.setPort(settings("database/port").toInt());
        db.setUserName(settings("database/username").toString());
        db.setPassword(settings("database/password").toString());
    }

    hr = db.open();
    if(!hr) {
        last_db_errorMsg = db.lastError().text();
        emit db_error(db.lastError().type(), tr("Connection error!"), last_db_errorMsg);
    } else {
        last_db_errorMsg = QString::null;
    }
    return hr;
}

QString SApplication::lastDatabaseError() const{
    return last_db_errorMsg;
}
