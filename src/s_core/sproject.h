#ifndef SPROJECT_H
#define SPROJECT_H

#include <QObject>
#include <QUndoStack>
#include <QStandardItemModel>
#include "sapplication.h"
#include "commands.h"
#include "questionsmodel.h"

#define S_PROJECT ((SProject*)(SApplication::inst()->project()))

typedef struct {
    QString name;
    QString source;
}question;

typedef struct {
    QString name;
    QString alias;
}theme;

class SProject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool in_redactor_mode READ isRedactorMode WRITE setRedactorMode)
public:
    explicit SProject(QObject *parent = 0);

    bool isRedactorMode();
    void setRedactorMode(bool value);

    bool create(const QString &filename);
    bool openProject(const QString &filename);
    void close();
    QByteArray readData(const QString &filename);
    bool addData(QByteArray s_data, const QString &filename);

    QUndoStack *undoStack();

    bool containsTheme(const QString &title);
    bool addTheme(const QString &title, const QString &alias = "");
    QString themeAlias(const QString &title);
    QString themeTitle(const QString &alias);
    void removeTheme(const QString &alias);
    void decrimentTheme();

    int *resourceCounter();

    QuestionsModel *questions();
    QStandardItemModel *questTypes();

signals:
    void themeAdded(QString title, QString alias);
    void themeRemoved(QString alias);

public slots:

private:
    bool redactor_mode;
    FSHANDLE *file_handle, *temp_handle;
    QUndoStack *undo_stack;
    QMap<QString, theme*> themes;
    //QMap<QString, question*> questions;
    QuestionsModel *quest_model;
    QStandardItemModel *quest_types;
    int thmes_counter, res_counter;
};

#endif // SPROJECT_H
