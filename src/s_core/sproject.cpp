#include "sproject.h"
#include <QDomDocument>
#include <QDir>
#include <QFileInfo>
#include <QPluginLoader>
#include <QStandardItem>
#include "defaulttesttypes/default_tst_types.h"

SProject::SProject(QObject *parent) :
    QObject(parent),
    file_handle(NULL),
    thmes_counter(0),
    res_counter(0)
{
    temp_handle = new FSHANDLE;
    undo_stack = new QUndoStack(this);
    quest_model = new QuestionsModel;
    quest_types = new QStandardItemModel;

    quest_types->setColumnCount(3);
    quest_types->setHeaderData(0, Qt::Horizontal, tr("Type caption"));
    quest_types->setHeaderData(1, Qt::Horizontal, tr("Name"));
    quest_types->setHeaderData(2, Qt::Horizontal, tr("Plugin"));

    //
    TestType_Interface *closed_interface = new Closed_Question;
    testTypesPlugins.push_back(closed_interface);
    QStandardItem *item = testTypesPlugins.last()->questinfo();
    quest_types->insertRow(0);
    quest_types->setData(quest_types->index(0, 0), item->child(0)->text());
    quest_types->setData(quest_types->index(0, 1), item->child(1)->text());
    quest_types->setData(quest_types->index(0, 2), QVariant(0));

    loadPlugins();
}

void SProject::loadPlugins() {
    QDir pluginsDir = QDir(qApp->applicationDirPath());
    pluginsDir.cd("TestTypes");

    int rows = quest_types->rowCount();
    foreach(QString fileName, pluginsDir.entryList(QDir::Files)){
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();

        if(plugin){
            testTypesPlugins.push_back(qobject_cast<TestType_Interface*>(plugin));
            QStandardItem *item = testTypesPlugins.last()->questinfo();

            quest_types->insertRow(rows);
            quest_types->setData(quest_types->index(rows, 0, QModelIndex()), item->child(0)->text());
            quest_types->setData(quest_types->index(rows, 1, QModelIndex()), item->child(1)->text());
            quest_types->setData(quest_types->index(rows, 2, QModelIndex()), rows);
            rows ++;
        }
    }
}

void SProject::setRedactorMode(bool value) {
    redactor_mode = value;
}

bool SProject::isRedactorMode() {
    return redactor_mode;
}

bool SProject::create(const QString &filename) {
    bool hr = false;
    file_handle = new FSHANDLE;
    if(!(hr = FileSystem::getInst()->fsCreate(filename, file_handle)))
    {
        FileSystem::getInst()->fsClose(file_handle);
        return false;
    }
    FileSystem::getInst()->fsClose(file_handle);

    temp_handle->archFName = QString("%1/%2").arg(QDir::tempPath(), QFileInfo(filename).fileName());
    FileSystem::getInst()->fsCreate(temp_handle->archFName, temp_handle);

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
    if(!FileSystem::getInst()->fsHasFile(filename, file_handle)) {
        FileSystem::getInst()->fsClose(file_handle);
        if(!redactor_mode && !FileSystem::getInst()->fsOpen(temp_handle))
            return QByteArray();

        if(!FileSystem::getInst()->fsHasFile(filename, temp_handle)) {
            FileSystem::getInst()->fsClose(temp_handle);
            return QByteArray();
        }

        QByteArray data = FileSystem::getInst()->fsGetFile(filename, temp_handle);
        FileSystem::getInst()->fsClose(temp_handle);
        return data;
    }

    QByteArray data = FileSystem::getInst()->fsGetFile(filename, file_handle);
    FileSystem::getInst()->fsClose(file_handle);

    return data;
}

bool SProject::addData(QByteArray s_data, const QString &filename) {
    if(!FileSystem::getInst()->fsOpen(temp_handle))
        return false;

    bool hr = FileSystem::getInst()->fsAddFile(s_data, filename, temp_handle);
    FileSystem::getInst()->fsClose(temp_handle);
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

QuestionsModel *SProject::questions() {
    return quest_model;
}

QStandardItemModel *SProject::questTypes() {
    return quest_types;
}

int *SProject::resourceCounter() {
    return (int*)&res_counter;
}

QuestEditorInterface *SProject::questEditing(QString name) {
    for(int i = 0; i < quest_types->rowCount(); i++) {
        QString str = quest_types->data(quest_types->index(i, 1)).toString();
                //->item(i, 1)->text();
        if(str == name)
            return testTypesPlugins[quest_types->data(quest_types->index(i, 2)).toInt()]->editor();
                    //quest_types->item(i, 2)->text().toInt()]->editor();
    }
    return NULL;
}
