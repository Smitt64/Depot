#include "sapplication.h"
#include "sproject.h"

SApplication *SApplication::s_App = NULL;

SApplication::SApplication(int argc, char **args) :
    s_project(NULL),
    mainWnd(NULL)
{
    QCoreApplication::setOrganizationName("Serpkov_Nikita");
    QCoreApplication::setApplicationName("TestBuilder");
    QCoreApplication::setApplicationVersion(VERSION);

    s_app = new QApplication(argc, args);
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
    bool hr = FileSystem::getInst()->fsOpen(filename, resource);
    FileSystem::getInst()->fsClose(resource);
    return hr;
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
