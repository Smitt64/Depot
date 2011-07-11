#include "editquestion.h"
#include "sapplication.h"
#include "sproject.h"

EditQuestionDlg::EditQuestionDlg(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Creating question"));
    buttons = new QDialogButtonBox;

    quest_type = new QComboBox;
    quest_type->setModel(S_PROJECT->questTypes());

    tabWidget = new QTabWidget;
    tabWidget->addTab(new QWidget, tr("Editing question"));
    tabWidget->setMinimumSize(320, 240);

    form = new QFormLayout;
    form->addRow(tr("Question type: "), quest_type);

    layout = new QVBoxLayout;
    layout->addLayout(form);
    layout->addWidget(tabWidget);

    setLayout(layout);

    restoreGeometry(SApplication::inst()->settings("window/questedit_geometry").toByteArray());

    connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));
}

EditQuestionDlg::~EditQuestionDlg() {
    SAFE_DELETE(buttons);
}
