#include "editquestion.h"
#include "sapplication.h"
#include "sproject.h"
#include "scombobox.h"

EditQuestionDlg::EditQuestionDlg(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Creating question"));
    setMinimumSize(640, 480);
    buttons = new QDialogButtonBox;
    buttons->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

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
    editor = S_PROJECT->questEditing(((QStandardItemModel*)(quest_type->model()))->item(quest_type->currentIndex(), 1)->text());
    layout->addWidget((QWidget*)editor);
    layout->addWidget(buttons);

    setLayout(layout);

    restoreGeometry(SApplication::inst()->settings("window/questedit_geometry").toByteArray());

    connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));
    connect(quest_type, SIGNAL(currentIndexChanged(int)), this, SLOT(questTypeChanged(int)));
}

EditQuestionDlg::~EditQuestionDlg() {
    SAFE_DELETE(buttons);
}

void EditQuestionDlg::addResourceFromRedactor(QString name, QByteArray data) {
    S_PROJECT->addData(data, name);
}

void EditQuestionDlg::questTypeChanged(int index) {
    layout->removeWidget(editor);
    SAFE_DELETE(editor);
    editor = S_PROJECT->questEditing(((QStandardItemModel*)(quest_type->model()))->item(index, 1)->text());
    layout->insertWidget(1, editor);
}
