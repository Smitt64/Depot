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

#endif // COMMAND_H
