#include "commands.h"
#include "sproject.h"

addThemeCommand::addThemeCommand(const QString &title, QUndoCommand *parent) :
    QUndoCommand(parent),
    theme_alias("alias"),
    theme_title(title)
{
}

void addThemeCommand::undo() {
    S_PROJECT->removeTheme(theme_alias);
    S_PROJECT->decrimentTheme();
}

void addThemeCommand::redo() {
    if(!S_PROJECT->addTheme(theme_title))
        return;

    theme_alias = S_PROJECT->themeAlias(theme_title);
    setText(QString(QObject::tr("New theme: %1.\nAlias: %2"))
            .arg(theme_title, theme_alias));
}

//==================================================================================
removeThemeCommand::removeThemeCommand(const QString &alias, QUndoCommand *parent) :
    QUndoCommand(parent),
    theme_alias(alias),
    theme_title("")
{
}

void removeThemeCommand::undo() {
    S_PROJECT->addTheme(theme_title, theme_alias);
}

void removeThemeCommand::redo() {
    theme_title = S_PROJECT->themeTitle(theme_alias);
    S_PROJECT->removeTheme(theme_alias);
    setText(QString(QObject::tr("Theme removed: %1"))
            .arg(theme_title));
}
