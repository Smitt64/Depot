#ifndef SPROJECT_H
#define SPROJECT_H

#include "s_core_global.h"
#include <QObject>
#include <QDomElement>
#include <QUndoStack>
#include <QStandardItemModel>
#include <QXmlQuery>
#include "sapplication.h"
#include "commands.h"
#include "questionsmodel.h"
#include "filesystemobject.h"
#include "interfaces/testtype_interface.h"
#include "interfaces/questeditor_interface.hpp"

#define S_PROJECT ((SProject*)(SApplication::inst()->project()))

/*typedef struct {
    QString name;
    QString source;
} question;*/

typedef struct {
    QString name;
    QString alias;
    QStringList questions;
    int row;
} theme;

typedef struct {
    QString questionAlias;
    QString resourceAlias;
} reg_resource;

class S_CORESHARED_EXPORT SProject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool in_redactor_mode READ isRedactorMode WRITE setRedactorMode)
public:
    explicit SProject(QObject *parent = 0);
    ~SProject();

    void loadPlugins();
    bool isModifyed();
    bool isRedactorMode();
    void setRedactorMode(bool value);

    bool create(const QString &filename);
    QByteArray readData(const QString &filename);
    bool addData(QByteArray s_data, const QString &filename);
    bool hasData(const QString &filename);

    QUndoStack *undoStack();

    bool containsTheme(const QString &alias);
    bool addTheme(const QString &title, const QString &alias = QString(), const int &index = -1);
    QString themeAlias(const QString &title);
    QString themeTitle(const QString &alias);
    QStringList themesAliases();
    int themeRow(const QString &alias);
    void removeTheme(const QString &alias);
    void decrimentTheme();

    //Working with questions
    bool addQuestion(const QString &type, const QString &name,
                     const QByteArray &settings, QStringList toThemes,
                     QString label = QString());
    QString questionType(const QString &alias);
    QString questionTypeLabel(const QString &alias);
    bool addQuestionToTheme(const QString &themeAlias, const QString &questAlias);
    bool removeQuestionFromTheme(const QString &questAlias, const QString &themeAlias = QString());
    bool containsQuestion(const QString &alias);
    void removeQuestion(const QString &name);

    int questionsInType(QString typeName);
    int questionsCount();
    int numberForQuestion();

    int *resourceCounter();

    QuestionsModel *questions();
    QStandardItemModel *questTypes();
    QStandardItemModel *themesModel();

    QuestEditorInterface *questEditing(const QString &name);

    bool regResource(const QString &questAlias, const QString &qResource);
    void unregResource(const QString &questAlias, const QString &qResource = QString());
    QStringList resourcesForQuestion(const QString &questAlias);

    QByteArray writeXMLConfig(const QDomElement &questionConf = QDomElement());

signals:
    void themeAdded(QString title, QString alias);
    void themeRemoved(QString alias);
    void questionAdded(QString alias, QString label, QStringList toThemes);
    void questionRemoved(QString questAlias, QStringList fromThemes);
    void modifyChanged(bool value);
    void error(QString msg);
    void projectClosed();

public slots:
    bool openProject(const QString &filename);
    void close();
    bool saveProject();

private:
    QModelIndex questTypeIndex(const QString &typeName);
    QDomElement makeElement(const QString &xml_string);
    int canClose();

    bool redactor_mode;
    QUndoStack *undo_stack;
    QMap<QString, theme*> themes;
    QList<reg_resource*> registered_resources;
    QList<TestType_Interface*> testTypesPlugins;
    QuestionsModel *quest_model;
    QStandardItemModel *quest_types;
    QStandardItemModel *themes_model;
    int thmes_counter, res_counter, quest_counter;
    QXmlQuery xmlQuery;
    FileSystemObject *fsManager;
};

#endif // SPROJECT_H
