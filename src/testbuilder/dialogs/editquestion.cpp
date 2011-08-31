#include "editquestion.h"
#include "sapplication.h"
#include "sproject.h"
#include "scombobox.h"
#include "shelpcontentviewwidget.h"

EditQuestionDlg::EditQuestionDlg(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Creating question"));
    setMinimumSize(640, 480);
    buttons = new QDialogButtonBox;
    save_changes = buttons->addButton(tr("Save"), QDialogButtonBox::AcceptRole);
    save_changes->setEnabled(false);
    cancel = buttons->addButton(tr("Cancel"), QDialogButtonBox::RejectRole);
    help = buttons->addButton(tr("Help"), QDialogButtonBox::HelpRole);

    quest_type = new QComboBox;
    quest_type->setWhatsThis(tr("List of accessible questions types."));
    quest_type->setModel(S_PROJECT->questTypes());

    groups_box = new SComboBox(S_PROJECT->themesModel());
    groups_box->setWhatsThis(tr("List of groups in which it is necessary to add a question."));

    form = new QFormLayout;
    form->addRow(tr("Question type: "), quest_type);
    form->addRow(tr("Add to themes:"), groups_box);

    layout = new QVBoxLayout;
    layout->addLayout(form);
    editor = S_PROJECT->questEditing(quest_type->model()->data(quest_type->model()->index(quest_type->currentIndex(), 1)).toString());
    layout->addWidget((QWidget*)editor);
    layout->addWidget(buttons);

    setLayout(layout);

    restoreGeometry(SApplication::inst()->settings("window/questedit_geometry").toByteArray());

    connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));
    connect(quest_type, SIGNAL(currentIndexChanged(int)), this, SLOT(questTypeChanged(int)));
    connect(editor, SIGNAL(validationChanged(bool)), this, SLOT(validQestion(bool)));
    connect(help, SIGNAL(clicked()), this, SLOT(showHelp()));
}

EditQuestionDlg::~EditQuestionDlg() {
    SAFE_DELETE(buttons);
}

void EditQuestionDlg::addResourceFromRedactor(QString name, QByteArray data) {
    S_PROJECT->addData(data, name);
}

void EditQuestionDlg::questTypeChanged(int index) {
    layout->removeWidget(editor);
    disconnect(editor, SIGNAL(validationChanged(bool)), this, SLOT(validQestion(bool)));
    SAFE_DELETE(editor);

    editor = S_PROJECT->questEditing(((QStandardItemModel*)(quest_type->model()))->item(index, 1)->text());
    layout->insertWidget(1, editor);

    connect(editor, SIGNAL(validationChanged(bool)), this, SLOT(validQestion(bool)));
}

void EditQuestionDlg::validQestion(bool isValid) {
    if(isValid && ((SComboBox*)groups_box)->selectedItemsCount()) {
        save_changes->setEnabled(true);
    }else {
        save_changes->setEnabled(false);
    }
}

QString EditQuestionDlg::questName(int counter) {
    return editor->makeQuestionAlias(counter);
}

QString EditQuestionDlg::questLabel() {
    return editor->makeQuestionLabel();
}

QString EditQuestionDlg::questType() {
    return ((QStandardItemModel*)(quest_type->model()))
            ->item(quest_type->currentIndex(), 1)->text();
}

QByteArray EditQuestionDlg::questConfig(QString questionName) {
    QByteArray content;

    QDomDocument doc;
    QDomElement element = doc.createElement("question");
    element.setAttribute("alias", questionName);
    element.setAttribute("type", questType());
    editor->makeQuestionConfig(&element, doc);
    doc.appendChild(element);

    QTextStream stream(&content);
    doc.save(stream, 3);

    return content;
}

QStringList EditQuestionDlg::selectedThemes() {
    return ((SComboBox*)groups_box)->selectedItems();
}

void EditQuestionDlg::showHelp() {
    QDialog *dlg = new QDialog(this);
    dlg->setWindowModality(Qt::NonModal);
    QLayout *form = new QVBoxLayout(dlg);
    form->addWidget(SApplication::inst()->helpViewWidget());
    dlg->setLayout(form);
    dlg->show();
}

int EditQuestionDlg::getResCount() {
    return editor->getResCount();
}

void EditQuestionDlg::getResource(int id, QString *name, QByteArray *res_data) {
    editor->getResource(id, name, res_data);
}
