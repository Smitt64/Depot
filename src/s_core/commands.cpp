#include "commands.h"
#include "sproject.h"

addThemeCommand::addThemeCommand(const QString &title, QUndoCommand *parent) :
    QUndoCommand(parent),
    theme_alias("alias"),
    theme_title(title)
{
}

void addThemeCommand::undo() {

}

void addThemeCommand::redo() {
    if(!S_PROJECT->addTheme(theme_title))
        return;

    theme_alias = S_PROJECT->themeAlias(theme_title);
    setText(QString(QObject::tr("New theme: %1.\nAlias: %2"))
            .arg(theme_title, theme_alias));
}
