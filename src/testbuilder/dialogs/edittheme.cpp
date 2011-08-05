#include "edittheme.h"
#include "sapplication.h"
#include "questionsmodel.h"
#include "sproject.h"
#include <QFormLayout>
#include <QStandardItemModel>

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
    quests->setSelectionMode(QAbstractItemView::MultiSelection);
    quests->setWhatsThis(tr("The list of all questions in the test."
                            "The allocated questions will be added in a theme."));

    buttons = new QDialogButtonBox;
    save_changes = buttons->addButton(tr("Save changes"), QDialogButtonBox::AcceptRole);
    save_changes->setEnabled(false);
    save_changes->setWhatsThis(tr("Keep changes in a theme."));

    cancel = buttons->addButton(tr("Cancel"), QDialogButtonBox::RejectRole);
    cancel->setWhatsThis(tr("Discard all changes."));

    form = new QFormLayout;
    form->addRow(tr("Theme caption: "), theme_caption);
    form->addRow(tr("Theme alias: "), theme_alias);
    form->setWidget(2, QFormLayout::SpanningRole, line);
    form->addRow(tr("Questions in theme: "), quests);
    form->addWidget(buttons);

    setLayout(form);

    quests->setModel(S_PROJECT->questions());

    connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));
    connect(theme_caption, SIGNAL(textChanged(QString)), this, SLOT(captionChanged(QString)));
    connect(theme_alias, SIGNAL(textChanged(QString)), this, SLOT(captionChanged(QString)));
}

EditThemeDlg::~EditThemeDlg() {
    SAFE_DELETE(form);
    SAFE_DELETE(theme_caption);
    SAFE_DELETE(theme_alias);
    SAFE_DELETE(line);
    SAFE_DELETE(quests);
    SAFE_DELETE(buttons);
}

void EditThemeDlg::setThemeTitleAlias(const QString &title, const QString &alias) {
    old_title = title;
    old_alias = alias;

    theme_caption->setText(title);
    theme_alias->setText(alias);
}

void EditThemeDlg::captionChanged(const QString &text) {
    if((theme_caption->text() != old_title || theme_alias->text() != old_alias) &&
            theme_caption->text().length() > 0 && theme_alias->text().length() > 0)
        save_changes->setEnabled(true);
    else {
        save_changes->setEnabled(false);
    }
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
