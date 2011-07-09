#include "sproject.h"
#include <QDomDocument>

SProject::SProject(QObject *parent) :
    QObject(parent),
    file_handle(NULL),
    thmes_counter(0)
{
    undo_stack = new QUndoStack(this);
}

void SProject::setRedactorMode(bool value) {
    redactor_mode = value;
}

bool SProject::create(const QString &filename) {
    bool hr = false;
    file_handle = new FSHANDLE;
    if(!(hr = FileSystem::getInst()->fsCreate(filename, file_handle)))
    {
        FileSystem::getInst()->fsClose(file_handle);
        return false;
    }

    QDomDocument document("test");
    QDomElement test = document.createElement("test");
    test.setAttribute("version", "depot1");

    QByteArray data;
    QTextStream stream(&data);
    document.save(stream, 3);

    if(!addData(data, "questions.xml"))
        return false;

    return true;
}

bool SProject::openProject(const QString &filename) {
    bool hr = FileSystem::getInst()->fsOpen(filename, file_handle);
    FileSystem::getInst()->fsClose(file_handle);
    return hr;
}

QByteArray SProject::readData(const QString &filename) {
    if(!FileSystem::getInst()->fsOpen(file_handle))
        return QByteArray();
    if(!FileSystem::getInst()->fsHasFile(filename, file_handle))
        return QByteArray();

    QByteArray data = FileSystem::getInst()->fsGetFile(filename, file_handle);
    FileSystem::getInst()->fsClose(file_handle);

    return data;
}

bool SProject::addData(QByteArray s_data, const QString &filename) {
    if(!FileSystem::getInst()->fsOpen(file_handle))
        return false;

    bool hr = FileSystem::getInst()->fsAddFile(s_data, filename, file_handle);
    FileSystem::getInst()->fsClose(file_handle);
    return hr;
}

QUndoStack *SProject::undoStack() {
    return undo_stack;
}

void SProject::close() {
    undo_stack->clear();
    file_handle->file.close();
}

bool SProject::containsTheme(const QString &title) {
    foreach (theme *tm, themes) {
        if(tm->name == title)
            return true;
    }
    return false;
}

bool SProject::addTheme(const QString &title, const QString &alias) {
    if(containsTheme(title))
        return false;

    if(alias.isEmpty())
        thmes_counter ++;

    theme *th = new theme;
    th->name = title;
    th->alias = (alias.isEmpty() ? QString("theme_%1")
            .arg(thmes_counter) : alias);
    if(themes.contains(th->alias))
        return false;

    themes.insert(th->alias, th);

    emit themeAdded(th->name, th->alias);

    return true;
}

QString SProject::themeAlias(const QString &title) {
    foreach (theme *tm, themes) {
        if(tm->name == title)
            return themes.key(tm);
    }

    return QString::null;
}

QString SProject::themeTitle(const QString &alias) {
    if(themes.contains(alias))
        return themes[alias]->name;
    return QString::null;
}

void SProject::removeTheme(const QString &alias) {
    if(themes.contains(alias))
    {
        themes.remove(alias);
        emit themeRemoved(alias);
    }
}

void SProject::decrimentTheme() {
    thmes_counter --;
}
