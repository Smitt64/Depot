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
};

class removeThemeCommand : public QUndoCommand
{
public:
    removeThemeCommand(const QString &alias, QUndoCommand *parent = 0);
    void undo();
    void redo();

private:
    QString theme_title, theme_alias;
};

#endif // COMMAND_H
