#ifndef SPROJECT_H
#define SPROJECT_H

#include <QObject>
#include <QUndoStack>
#include "sapplication.h"
#include "commands.h"

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
public:
    explicit SProject(QObject *parent = 0);

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

signals:

public slots:

private:
    bool redactor_mode;
    FSHANDLE *file_handle;
    QUndoStack *undo_stack;
    QMap<QString, theme*> themes;
    QMap<QString, question*> questions;

    int thmes_counter;
};

#endif // SPROJECT_H
