#ifndef COMMAND_H
#define COMMAND_H

#include <QUndoCommand>

class addThemeCommand : public QUndoCommand
{
public:
    addThemeCommand(const QString &title, QUndoCommand *parent = 0);
    void undo();
    void redo();

private:
    QString theme_title, theme_alias;
    int row;
};

class removeThemeCommand : public QUndoCommand
{
public:
    removeThemeCommand(const QString &alias, QUndoCommand *parent = 0);
    void undo();
    void redo();

private:
    QString theme_title, theme_alias;
    int row;
};


class addQuestionCommand : public QUndoCommand
{
public:
    addQuestionCommand(const QString &type, const QString &alias,
                       const QByteArray &settings, const QString &label,
                       QUndoCommand *parent = 0);
    void undo();
    void redo();

private:
    QString quest_type, quest_alias, quest_label;
    QByteArray quest_settings;
};

#endif // COMMAND_H
