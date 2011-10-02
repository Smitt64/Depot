#include "sproject.h"
#include <QDomDocument>
#include <QDir>
#include <QFileInfo>
#include <QPluginLoader>
#include <QStandardItem>
#include <QXmlQuery>
#include <QBuffer>
#include <QXmlFormatter>
#include <QMessageBox>
#include <QDebug>
#include "defaulttesttypes/default_tst_types.h"

SProject::SProject(QObject *parent) :
    QObject(parent),
    //file_handle(NULL),
    fsManager(NULL),
    thmes_counter(0),
    res_counter(0),
    quest_counter(0)
{
    //temp_handle = new FSHANDLE;
    fsManager = new FileSystemObject(this);
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

    connect(undo_stack, SIGNAL(cleanChanged(bool)), this, SIGNAL(modifyChanged(bool)));
}

SProject::~SProject() {
    close();
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

bool SProject::isModifyed() {
    return bool(!undo_stack->isClean());
}

void SProject::setRedactorMode(bool value) {
    redactor_mode = value;
}

bool SProject::isRedactorMode() {
    return redactor_mode;
}

bool SProject::create(const QString &filename) {
    bool hr = false;
    int saveRet = canClose();
    if(saveRet != 0) {
        if(saveRet == 1) {
            saveProject();
            close();
        } else if(saveRet == 2) {
            close();
        } else {
            return false;
        }
    }
    //Test file
    fsManager->addArchive(filename, true);
    //Temp test file
    fsManager->addArchive(QString("%1/~%2")
                          .arg(QDir::tempPath(), QFileInfo(filename).fileName()));

    QDomDocument document("DepotTest");
    QDomElement test = document.createElement("test");
    test.setAttribute("version", "depot1");

    QDomElement themes = document.createElement("themes");
    themes.appendChild(document.createTextNode("theme1"));
    test.appendChild(themes);
    document.appendChild(test);

    QByteArray qdata;
    QTextStream stream(&qdata);
    document.save(stream, 3);

    return fsManager->addFile(qdata, "questions.xml");
}

bool SProject::openProject(const QString &filename) {
    int saveRet = canClose();
    if(saveRet != 0) {
        if(saveRet == 1) {
            saveProject();
            close();
        } else if(saveRet == 2) {
            close();
        } else {
            return false;
        }
    }

    if(!fsManager->addArchive(filename)) {
        return false;
    }

    fsManager->addArchive(QString("%1/~%2")
                          .arg(QDir::tempPath(), QFileInfo(filename).fileName()));

    QByteArray data = fsManager->fileData("questions.xml");
    QBuffer file(&data);
    file.open(QIODevice::ReadOnly);

    xmlQuery.bindVariable("document", &file);

    QDomDocument document;
    QString err;
    int line, column;
    if(!document.setContent(data, &err, &line, &column)) {
        error(tr("%1\nLine: %2\nColumn: %3")
              .arg(err)
              .arg(line)
              .arg(column));
        qDebug() << data;
        return false;
    }
    QDomElement documentElement = document.documentElement();
    QDomElement themesElement = documentElement.firstChildElement("themes");
    thmes_counter = themesElement.attribute("thmes_counter", "-1").toInt();
    bool auto_theme_counting = false;

    if(thmes_counter == -1)
        auto_theme_counting = true;

    for(QDomNode n = themesElement.firstChild(); !n.isNull(); n = n.nextSibling()) {
        QDomElement element = n.toElement();
        QString node;
        xmlQuery.setQuery(QString("doc($document)/test/themes/theme[@alias=\"%1\"]")
                          .arg(element.attribute("alias")));
        xmlQuery.evaluateTo(&node);

        QString tAlias = element.attribute("alias", "");
        addTheme(element.attribute("title"),
                 tAlias,
                 element.attribute("row", "-1").toInt());

        for(QDomNode qn = element.firstChild(); !qn.isNull(); qn = qn.nextSibling()) {
            QDomElement qElement = qn.toElement();
            themes[themes.keys().last()]->questions.append(qElement.text());
        }

        if(auto_theme_counting)
            thmes_counter ++;
    }
    QDomElement questsElement = documentElement.firstChildElement("questions");
    for (QDomNode n = questsElement.firstChild(); !n.isNull(); n = n.nextSibling()) {
        QDomElement element = n.toElement();
        QString qType = element.attribute("type");
        QString qAlias = element.attribute("alias");
        QString qLabel = (element.attribute("label").isNull() ? qAlias : element.attribute("label"));
        QStringList list;
        foreach (theme *th, themes) {
            if(th->questions.contains(qAlias))
                list.append(th->alias);
        }
        QString settings;
        xmlQuery.setQuery(QString("doc($document)/test/questions/question[@alias=\"%1\"]")
                          .arg(qAlias));
        xmlQuery.evaluateTo(&settings);
        addQuestion(qType, qAlias, settings.toLocal8Bit(), list, qLabel);
    }
    return true;
}

bool SProject::saveProject() {

    fsManager->addFile(writeXMLConfig(), "questions.xml");
    foreach (reg_resource *rc, registered_resources) {
        fsManager->moveFile(rc->resourceAlias, 1, 0);
    }
    undo_stack->clear();
    return true;
}

int SProject::canClose() {
    if(isModifyed()) {
        QMessageBox::StandardButton btn = QMessageBox::question(SApplication::inst()->mainWindow(),
                                                                tr("Saving test!"),
                                                                tr("<B>Test has been modifyed.</B>"
                                                                   "<P>Do you want to save changes?</P>"),
                                                                QMessageBox::Yes | QMessageBox::No
                                                                | QMessageBox::Cancel);
        if(btn == QMessageBox::Yes) {
            return 1;//Save and close project
        } else if(btn == QMessageBox::No) {
            return 2;//Close project without save
        } else if(btn == QMessageBox::Cancel) {
            return -1;//Don't close project
        }

    }
    return 0;
}

QByteArray SProject::readData(const QString &filename) {
    return fsManager->fileData(filename);
}

bool SProject::addData(QByteArray s_data, const QString &filename) {
    return fsManager->addFile(s_data, filename);
}

bool SProject::hasData(const QString &filename) {
    return fsManager->hasFile(filename);
}

QUndoStack *SProject::undoStack() {
    return undo_stack;
}

void SProject::close() {
    int saveRet = canClose();
    if(saveRet != 0) {
        if(saveRet == 1) {
            saveProject();
        } else if(saveRet == 2) {
        } else {
            return;
        }
    }
    emit projectClosed();
    undo_stack->clear();
    fsManager->deleteArchive(1, true);
    fsManager->deleteArchive(0);
    thmes_counter = 0;
    res_counter = 0;
    quest_counter = 0;
    registered_resources.clear();
    quest_model->clear();
    themes_model->clear();
}

bool SProject::containsTheme(const QString &alias) {
    foreach (theme *tm, themes) {
        if(tm->alias == alias)
            return true;
    }
    return false;
}

bool SProject::addTheme(const QString &title, const QString &alias, const int &index) {
    if(containsTheme(alias))
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

    themes_model->insertRow(row);
    themes_model->setData(themes_model->index(row, 0), title, Qt::DisplayRole);
    themes_model->setData(themes_model->index(row, 0), Qt::Unchecked, Qt::CheckStateRole);
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
        emit themeRemoved(alias);
    }
}

QStringList SProject::themesAliases() {
    QStringList list;

    foreach (theme *th, themes) {
        list.append(th->alias);
    }

    return list;
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
    themesElement.setAttribute("thmes_counter", thmes_counter);

    foreach (theme *th, themes) {
        QDomElement themeElement = document.createElement("theme");
        themeElement.setAttribute("title", th->name);
        themeElement.setAttribute("alias", th->alias);
        themeElement.setAttribute("row", th->row);
        if(!th->questions.isEmpty()) {
            foreach (QString quest, th->questions) {
                QDomElement quest_thElement = document.createElement("question");
                quest_thElement.appendChild(document.createTextNode(quest));
                themeElement.appendChild(quest_thElement);
            }
        }
        themesElement.appendChild(themeElement);
    }
    QDomElement questionsElement = document.createElement("questions");
    register int questCount = questionsCount();
    questionsElement.setAttribute("questCount", questCount);
    for (int i = 0; i < questCount; i++) {
        QDomElement questElement = makeElement(quest_model->data(quest_model->index(i, 3)).toString());
        questionsElement.appendChild(questElement);
    }

    QDomElement resourcesElement = document.createElement("reg_resources");
    foreach (reg_resource *rc, registered_resources) {
        QDomElement resElement = document.createElement("resource");
        resElement.setAttribute("question", rc->questionAlias);
        resElement.appendChild(document.createTextNode(rc->resourceAlias));
    }
    test.appendChild(themesElement);
    test.appendChild(questionsElement);
    test.appendChild(resourcesElement);
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
                           const QByteArray &settings, QStringList toThemes,
                           QString label) {
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
    if(!questTypeIndex(type).isValid()) {
        error(tr("Question type [%1] is not exists!").arg(type));
        return false;
    }

    QString complete_name = name;

    if(quest_model->questionIndex(complete_name).isValid()) {
        error(tr("Question [%1] allready exists!")
              .arg(complete_name));
        return false;
    }

    quest_model->addQuestion(type, complete_name, settings, label);
    foreach (QString value, toThemes) {
        addQuestionToTheme(value, complete_name);
    }
    emit questionAdded(complete_name, label, toThemes);

    int arch = -1;
    //has question in temp archive
    QString quest_data_name = QString("quest_data\\%1.dat")
            .arg(complete_name);
    if((arch = fsManager->hasFile(quest_data_name)) != -1) {
        QByteArray qdata = fsManager->fileData(quest_data_name);
        QDataStream stream(qdata);
        stream.setVersion(QDataStream::Qt_4_7);
        quint32 magic = -1;
        stream >> magic;
        if(magic == 0xA0B0C0D0) {
            QString name;
            int rcCount = 0;
            stream >> name >> rcCount;
            qDebug() << name;

            for(int i = 0; i < rcCount; i++) {
                int arch = 0;
                QString rc_alias;

                stream >> arch >> rc_alias;
                regResource(name, rc_alias);

                //Move file to main test file
                if(arch == 0)
                    fsManager->moveFile(rc_alias, 1, 0);
            }
        }
    }

    return true;
}

void SProject::removeQuestion(const QString &name) {
    QModelIndex index = quest_model->questionIndex(name);
    if(!index.isValid()) {
        error(tr("Can't remove question [%1]").arg(name));
        return;
    }

    QByteArray quest_data;
    QBuffer buf(&quest_data);
    buf.open(QIODevice::WriteOnly);
    QDataStream stream(&buf);
    stream.setVersion(QDataStream::Qt_4_7);
    stream << (quint32)0xA0B0C0D0;
    stream << name;
    QStringList list = resourcesForQuestion(name);
    stream << list.count();

    foreach (QString str, list) {
        int arch = fsManager->hasFile(str);
        if(arch == 0)
            fsManager->moveFile(str, 0, 1);

        stream << arch << str;

    }
    QByteArray questConfig = quest_model->data(
                quest_model->index(quest_model->questionIndex(name).row(), 3))
            .toByteArray();

    questConfig = qCompress(questConfig, 3);
    stream << questConfig.size() << questConfig;
    buf.close();

    if(!addData(quest_data, QString("quest_data\\%1.dat").arg(name))) {
        error(tr("Can't write question data to temporary file!"));
    }

    unregResource(name);

    int row = index.row();
    quest_model->removeRow(row);

    removeQuestionFromTheme(name);
}

bool SProject::containsQuestion(const QString &alias) {
    return quest_model->hasQuestion(alias);
}

bool SProject::addQuestionToTheme(const QString &themeAlias, const QString &questAlias) {
    if(!containsTheme(themeAlias) || !containsQuestion(questAlias))
        return false;

    if(themes[themeAlias]->questions.contains(questAlias))
        return false;

    themes[themeAlias]->questions.append(questAlias);
}

QString SProject::questionType(const QString &alias) {
    return quest_model->questionType(alias);
}

QString SProject::questionTypeLabel(const QString &alias) {
    QString typeAlias = questionType(alias);
    QList<QStandardItem*> items = quest_types->findItems(typeAlias, Qt::MatchCaseSensitive, 1);

    if(!items.count())
        return typeAlias;

    return items[0]->text();
}

bool SProject::removeQuestionFromTheme(const QString &questAlias, const QString &themeAlias) {
    if(themeAlias.isNull()) {
        foreach (theme *th, themes) {
            if(!th->questions.contains(questAlias))
                continue;
            th->questions.removeOne(questAlias);
            emit questionRemoved(questAlias, QStringList() << th->alias);
        }
    } else {
        if(!containsTheme(themeTitle(themeAlias)))
            return false;
        theme *th = themes[themeAlias];
        if(th->questions.contains(questAlias)) {
            if(th->questions.removeOne(questAlias))
                emit questionRemoved(questAlias, QStringList() << themeAlias);
        }
    }
    return true;
}

bool SProject::regResource(const QString &questAlias, const QString &qResource) {
    if(!quest_model->hasQuestion(questAlias) || qResource.isEmpty())
        return false;

    reg_resource *tmp = new reg_resource;
    tmp->questionAlias = questAlias;
    tmp->resourceAlias = qResource;
    registered_resources.push_back(tmp);

    return true;
}

void SProject::unregResource(const QString &questAlias, const QString &qResource) {
    if (!quest_model->hasQuestion(questAlias) || qResource.isNull()) {
        foreach (reg_resource *res, registered_resources) {
            if (res->questionAlias == questAlias) {
                registered_resources.removeOne(res);
            }
        }
        return;
    }
    foreach (reg_resource *res, registered_resources) {
        if (res->questionAlias == questAlias && res->resourceAlias == qResource) {
            registered_resources.removeOne(res);
        }
    }
}

QStringList SProject::resourcesForQuestion(const QString &questAlias) {
    QStringList list;
    foreach (reg_resource *rc, registered_resources) {
        if(rc->questionAlias == questAlias)
            list.append(rc->resourceAlias);
    }
    return list;
}

QDomElement SProject::makeElement(const QString &xml_string) {
    QDomDocument doc;
    QString err;
    int errLine, errColumn;
    if(!doc.setContent(xml_string, &err, &errLine, &errColumn)) {
        error(tr("%1\nLine: %1\nColumn: %3")
              .arg(err)
              .arg(errLine)
              .arg(errColumn));
        return QDomElement();
    }

    return doc.documentElement();
}
