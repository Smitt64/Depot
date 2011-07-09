#include "edittheme.h"
#include <QFormLayout>

EditThemeDlg::EditThemeDlg(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Edeting theme"));
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    theme_caption = new QLineEdit;
    theme_caption->setWhatsThis(tr("Theme caption."));
    theme_alias = new QLineEdit;
    theme_alias->setWhatsThis(tr("Theme alias."));

    line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    quests = new QListView;
    quests->setWhatsThis(tr("The list of all questions in the test."
                            "The allocated questions will be added in a theme."));

    buttons = new QDialogButtonBox;
    save_changes = buttons->addButton(tr("Save changes"), QDialogButtonBox::AcceptRole);
    save_changes->setEnabled(false);
    save_changes->setWhatsThis(tr("Keep changes in a theme."));

    cancel = buttons->addButton(tr("Cancel"), QDialogButtonBox::RejectRole);
    cancel->setWhatsThis(tr("Discard all changes."));

    QFormLayout *form = new QFormLayout;
    form->addRow(tr("Theme caption: "), theme_caption);
    form->addRow(tr("Theme alias: "), theme_alias);
    form->setWidget(2, QFormLayout::SpanningRole, line);
    form->addRow(tr("Questions in theme: "), quests);
    form->addWidget(buttons);

    setLayout(form);

    connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));
    connect(theme_caption, SIGNAL(textChanged(QString)), this, SLOT(captionChanged(QString)));
    connect(theme_alias, SIGNAL(textChanged(QString)), this, SLOT(captionChanged(QString)));
}

void EditThemeDlg::setThemeTitleAlias(const QString &title, const QString &alias) {
    old_title = title;
    old_alias = alias;

    theme_caption->setText(title);
    theme_alias->setText(alias);
}

void EditThemeDlg::captionChanged(const QString &text) {
    if(theme_caption->text() != old_title || theme_alias->text() != old_alias)
        save_changes->setEnabled(true);
    else
        save_changes->setEnabled(false);
}

bool EditThemeDlg::isTitleChanged() {
    if(old_title != theme_caption->text())
        return true;
    return false;
}

bool EditThemeDlg::isAliasChanged() {
    if(old_alias != theme_alias->text())
        return true;
    return false;
}
