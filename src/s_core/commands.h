#ifndef ADDTHEMECOMMAND_H
#define ADDTHEMECOMMAND_H

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

#endif // ADDTHEMECOMMAND_H
