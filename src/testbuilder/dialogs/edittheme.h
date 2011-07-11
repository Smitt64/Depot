#ifndef EDITTHEME_H
#define EDITTHEME_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QListView>
#include <QFormLayout>
#include <QPushButton>

class EditThemeDlg : public QDialog
{
    Q_OBJECT
public:
    EditThemeDlg(QWidget *parent = 0);
    ~EditThemeDlg();

    void setThemeTitleAlias(const QString &title, const QString &alias);
    bool isTitleChanged();
    bool isAliasChanged();

private slots:
    void captionChanged(const QString &text);

private:
    QLineEdit *theme_caption, *theme_alias;
    QFrame *line;
    QListView *quests;
    QDialogButtonBox  *buttons;
    QPushButton *save_changes;
    QPushButton *cancel;
    QFormLayout *form;

    QString old_title, old_alias;
};

#endif // EDITTHEME_H
