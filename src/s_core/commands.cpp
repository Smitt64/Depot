#include "commands.h"
#include "sproject.h"

addThemeCommand::addThemeCommand(const QString &title, QUndoCommand *parent) :
    QUndoCommand(parent),
    theme_alias(""),
    theme_title(title),
    row(-1)
{
}

void addThemeCommand::undo() {
    S_PROJECT->removeTheme(theme_alias);
    S_PROJECT->decrimentTheme();
}

void addThemeCommand::redo() {
    if(!S_PROJECT->addTheme(theme_title, theme_alias, row))
        return;

    theme_alias = S_PROJECT->themeAlias(theme_title);
    setText(QString(QObject::tr("New theme: %1.\nAlias: %2"))
            .arg(theme_title, theme_alias));
}
//==================================================================================
removeThemeCommand::removeThemeCommand(const QString &alias, QUndoCommand *parent) :
    QUndoCommand(parent),
    theme_alias(alias),
    theme_title(""),
    row(S_PROJECT->themeRow(alias))
{
}

void removeThemeCommand::undo() {
    S_PROJECT->addTheme(theme_title, theme_alias, row);
}

void removeThemeCommand::redo() {
    theme_title = S_PROJECT->themeTitle(theme_alias);
    row = S_PROJECT->themeRow(theme_alias);
    S_PROJECT->removeTheme(theme_alias);
    setText(QString(QObject::tr("Theme removed: %1"))
            .arg(theme_title));
}
//==================================================================================
addQuestionCommand::addQuestionCommand(const QString &type, const QString &alias,
                                       const QByteArray &settings, const QString &label,
                                       QUndoCommand *parent) :
    QUndoCommand(parent),
    quest_type(type),
    quest_alias(alias),
    quest_settings(settings),
    quest_label(label)
{

}

void addQuestionCommand::undo() {
    S_PROJECT->removeQuestion(quest_alias);
}

void addQuestionCommand::redo() {
    if(!S_PROJECT->addQuestion(quest_type, quest_alias,
                           quest_settings, quest_label))
        return;
    setText(QObject::tr("Question created: %1").arg(quest_alias));
}
