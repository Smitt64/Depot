#ifndef SPROJECT_H
#define SPROJECT_H

#include <QObject>
#include <QDomElement>
#include <QUndoStack>
#include <QStandardItemModel>
#include <QXmlQuery>
#include "sapplication.h"
#include "commands.h"
#include "questionsmodel.h"
#include "interfaces/testtype_interface.h"
#include "interfaces/questeditor_interface.h"

#define S_PROJECT ((SProject*)(SApplication::inst()->project()))

typedef struct {
    QString name;
    QString source;
}question;

typedef struct {
    QString name;
    QString alias;
    int row;
}theme;

class SProject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool in_redactor_mode READ isRedactorMode WRITE setRedactorMode)
public:
    explicit SProject(QObject *parent = 0);

    void loadPlugins();

    bool isRedactorMode();
    void setRedactorMode(bool value);

    bool create(const QString &filename);
    void close();
    QByteArray readData(const QString &filename);
    bool addData(QByteArray s_data, const QString &filename);

    QUndoStack *undoStack();

    bool containsTheme(const QString &title);
    bool addTheme(const QString &title, const QString &alias = "", const int &index = -1);
    QString themeAlias(const QString &title);
    QString themeTitle(const QString &alias);
    int themeRow(const QString &alias);
    void removeTheme(const QString &alias);
    void decrimentTheme();

    int *resourceCounter();

    QuestionsModel *questions();
    QStandardItemModel *questTypes();
    QStandardItemModel *themesModel();

    QuestEditorInterface *questEditing(QString name);

signals:
    void themeAdded(QString title, QString alias);
    void themeRemoved(QString alias);

public slots:
    bool openProject(const QString &filename);
    bool saveProject();

private:
    QByteArray writeXMLConfig(QDomElement question = QDomElement());
    bool redactor_mode;
    FSHANDLE *file_handle, *temp_handle;
    QUndoStack *undo_stack;
    QMap<QString, theme*> themes;
    QList<TestType_Interface*> testTypesPlugins;
    QuestionsModel *quest_model;
    QStandardItemModel *quest_types;
    QStandardItemModel *themes_model;
    int thmes_counter, res_counter;
    QXmlQuery xmlQuery;
};

#endif // SPROJECT_H
