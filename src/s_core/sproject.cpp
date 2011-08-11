#include "sproject.h"
#include <QDomDocument>
#include <QDir>
#include <QFileInfo>
#include <QPluginLoader>
#include <QStandardItem>
#include <QXmlQuery>
#include <QBuffer>
#include <QXmlFormatter>
#include <QDebug>
#include "defaulttesttypes/default_tst_types.h"

SProject::SProject(QObject *parent) :
    QObject(parent),
    file_handle(NULL),
    thmes_counter(0),
    res_counter(0),
    quest_counter(0)
{
    temp_handle = new FSHANDLE;
    undo_stack = new QUndoStack(this);
    quest_model = new QuestionsModel;
    quest_types = new QStandardItemModel;

    quest_types->setColumnCount(3);
    quest_types->setHeaderData(0, Qt::Horizontal, tr("Type caption"));
    quest_types->setHeaderData(1, Qt::Horizontal, tr("Name"));
    quest_types->setHeaderData(2, Qt::Horizontal, tr("Plugin"));

    //Adding new question type
    TestType_Interface *closed_interface = new Closed_Question;
    testTypesPlugins.push_back(closed_interface);
    QStandardItem *item = testTypesPlugins.last()->questinfo();
    quest_types->insertRow(0);
    quest_types->setData(quest_types->index(0, 0), item->child(0)->text());
    quest_types->setData(quest_types->index(0, 1), item->child(1)->text());
    quest_types->setData(quest_types->index(0, 2), QVariant(0));
    //
    testTypesPlugins.push_back(new FreeChouse_Question);
    QStandardItem *item2 = testTypesPlugins.last()->questinfo();
    quest_types->insertRow(1);
    quest_types->setData(quest_types->index(1, 0), item2->child(0)->text());
    quest_types->setData(quest_types->index(1, 1), item2->child(1)->text());
    quest_types->setData(quest_types->index(1, 2), QVariant(1));

    themes_model = new QStandardItemModel;
    themes_model->setColumnCount(2);
    themes_model->setHeaderData(0, Qt::Horizontal, "Theme");
    themes_model->setHeaderData(1, Qt::Horizontal, "Alias");

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

    QDomDocument document("DepotTest");
    QDomElement test = document.createElement("test");
    test.setAttribute("version", "depot1");

    QDomElement themes = document.createElement("themes");
    themes.appendChild(document.createTextNode("theme1"));
    test.appendChild(themes);
    document.appendChild(test);

    QByteArray data;
    QTextStream stream(&data);
    document.save(stream, 3);

    if(!FileSystem::getInst()->fsOpen(file_handle))
        return false;
    if(!FileSystem::getInst()->fsAddFile(data,"questions.xml", file_handle)) {
        FileSystem::getInst()->fsClose(file_handle);
        return false;
    }

    FileSystem::getInst()->fsClose(file_handle);

    return true;
}

bool SProject::openProject(const QString &filename) {
    if(file_handle)
        SAFE_DELETE(file_handle);
    file_handle = new FSHANDLE;
    file_handle->archFName = filename;

    bool hr = FileSystem::getInst()->fsOpen(file_handle);

    if(!hr) {
        FileSystem::getInst()->fsClose(file_handle);
        return false;
    }
    FileSystem::getInst()->fsClose(file_handle);

    temp_handle->archFName = QString("%1/%2").arg(QDir::tempPath(), QFileInfo(filename).fileName());
    FileSystem::getInst()->fsCreate(temp_handle->archFName, temp_handle);

    QByteArray data = readData("questions.xml");
    qDebug() << data;
    QBuffer file(&data);
    file.open(QIODevice::ReadOnly);

    xmlQuery.bindVariable("document", &file);

    QDomDocument document;
    if(!document.setContent(data))
        return false;
    QDomElement documentElement = document.documentElement();
    QDomElement themesElement = documentElement.firstChildElement("themes");
    for(QDomNode n = themesElement.firstChild(); !n.isNull(); n = n.nextSibling()) {
        QDomElement element = n.toElement();
        QString node;
        xmlQuery.setQuery(QString("doc($document)/test/themes/theme[@alias=\"%1\"]")
                          .arg(element.attribute("alias")));
        xmlQuery.evaluateTo(&node);

        addTheme(element.attribute("title"),
                 element.attribute("alias", ""),
                 element.attribute("row", "-1").toInt());
    }
    return hr;
}

bool SProject::saveProject() {
    if(!FileSystem::getInst()->fsOpen(temp_handle)) {
        FileSystem::getInst()->fsClose(temp_handle);
        return false;
    }

    foreach (File file, temp_handle->fileList) {
        if(!FileSystem::getInst()->fsOpen(file_handle)) {
            FileSystem::getInst()->fsClose(file_handle);
            continue;
        }
        QString str = QString(file.m_pName);
        QByteArray data = FileSystem::getInst()->fsGetFile(str, temp_handle);
        if(data.isNull()) {
            FileSystem::getInst()->fsClose(file_handle);
            continue;
        }
        if(!FileSystem::getInst()->fsHasFile(str, file_handle))
            FileSystem::getInst()->fsAddFile(data, str, file_handle);
        else
            FileSystem::getInst()->fsRewriteFile(data, str, file_handle, 2);
        FileSystem::getInst()->fsDelete(str, temp_handle);
        FileSystem::getInst()->fsClose(file_handle);
    }
    FileSystem::getInst()->fsClose(temp_handle);
    return true;
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

    bool hr;
    if(!FileSystem::getInst()->fsHasFile(filename, temp_handle))
        hr = FileSystem::getInst()->fsAddFile(s_data, filename, temp_handle);
    else
        hr = FileSystem::getInst()->fsRewriteFile(s_data, filename, temp_handle, 2);
    FileSystem::getInst()->fsClose(temp_handle);
    return hr;
}

QUndoStack *SProject::undoStack() {
    return undo_stack;
}

void SProject::close() {
    undo_stack->clear();
    file_handle->file.close();
    QFile::remove(file_handle->archFName);
}

bool SProject::containsTheme(const QString &title) {
    foreach (theme *tm, themes) {
        if(tm->name == title)
            return true;
    }
    return false;
}

bool SProject::addTheme(const QString &title, const QString &alias, const int &index) {
    if(containsTheme(title))
        return false;

    if(alias.isEmpty())
        thmes_counter ++;

    theme *th = new theme;
    th->name = title;
    th->alias = (alias.isEmpty() ? QString("theme_%1")
            .arg(thmes_counter) : alias);

    int row = (index == -1 ? themes_model->rowCount() : index);
    th->row = row;

    if(themes.contains(th->alias))
        return false;

    themes.insert(th->alias, th);
    addData(writeXMLConfig(), "questions.xml");

    themes_model->insertRow(row);
    themes_model->setData(themes_model->index(row, 0), title, Qt::DisplayRole);
    themes_model->setData(themes_model->index(row, 0), title, Qt::CheckStateRole);
    themes_model->setData(themes_model->index(row, 1), th->alias, Qt::DisplayRole);

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

int SProject::themeRow(const QString &alias) {
    if(themes.contains(alias))
        return themes[alias]->row;
    return -1;
}

void SProject::removeTheme(const QString &alias) {
    if(themes.contains(alias))
    {
        themes_model->removeRow(themes[alias]->row);
        themes.remove(alias);
        addData(writeXMLConfig(), "questions.xml");
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

QuestEditorInterface *SProject::questEditing(const QString &name) {
    for(int i = 0; i < quest_types->rowCount(); i++) {
        QString str = quest_types->data(quest_types->index(i, 1)).toString();
        if(str == name)
            return testTypesPlugins[quest_types->data(quest_types->index(i, 2)).toInt()]->editor();
    }
    return NULL;
}

QStandardItemModel *SProject::themesModel() {
    return themes_model;
}

QByteArray SProject::writeXMLConfig(const QDomElement &question) {
    QByteArray res;
    QDomDocument document("DepotTest");
    QDomElement test = document.createElement("test");
    test.setAttribute("version", "depot1");

    QDomElement themesElement = document.createElement("themes");

    foreach (theme *th, themes) {
        QDomElement themeElement = document.createElement("theme");
        themeElement.setAttribute("title", th->name);
        themeElement.setAttribute("alias", th->alias);
        themeElement.setAttribute("row", th->row);
        themesElement.appendChild(themeElement);
    }
    test.appendChild(themesElement);
    document.appendChild(test);

    QTextStream stream(&res);
    document.save(stream, 3);

    return res;
}

QModelIndex SProject::questTypeIndex(const QString &typeName) {
    QList<QStandardItem*> items = quest_types->findItems(typeName, Qt::MatchCaseSensitive, 1);

    if(items.count() == 0)
        return QModelIndex();

    return items[0]->index();
}

int SProject::questionsInType(QString typeName) {
    QList<QStandardItem*> items = quest_model->findItems(typeName, Qt::MatchCaseSensitive, 2);

    return items.count();
}

int SProject::questionsCount() {
    return quest_model->rowCount();
}

int SProject::numberForQuestion() {
    return quest_counter;
}

bool SProject::addQuestion(const QString &type, const QString &name,
                           const QByteArray &settings, QString label) {
    if(quest_model->hasQuestion(name))
        return false;

    QDomDocument doc;
    QString errMsg;
    if(!doc.setContent(settings, &errMsg)) {
        error(tr("Quest configuration invalid!"
                 "%1").arg(errMsg));
        return false;
    }

    label = (label.isNull() ? name : label);
    if(questTypeIndex(type).isValid()) {
        error(tr("Question type [%1] is not exists!").arg(name));
        return false;
    }

    QString complete_name = QString("%1_%2")
            .arg(name)
            .arg(quest_counter);

    if(quest_model->questionIndex(complete_name).isValid()) {
        error(tr("Question [%1] allready exists!").arg(complete_name));
        return false;
    }

    quest_model->addQuestion(type, complete_name, settings, label);

    return true;
}

void SProject::removeQuestion(const QString &name) {
    QModelIndex index = quest_model->questionIndex(name);
    if(!index.isValid()) {
        error(tr("Can't remove question [%1]").arg(name));
        return;
    }

    int row = index.row();
    quest_model->removeRow(row);
}
