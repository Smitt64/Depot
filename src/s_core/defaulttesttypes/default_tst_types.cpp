#include "default_tst_types.h"
#include "../sapplication.h"
#include <QBoxLayout>
#include <QTabWidget>
#include <QDebug>
#include <QTime>

//////////////////////////////////////////////////////////////////////////////////////////
ClosedTestEditor::ClosedTestEditor(QWidget *parent) :
    QuestEditorInterface(parent),
    oldAnswersCount(0)
{
    text_editor = new STextEditorView;
    QTabWidget *tab = new QTabWidget;
    tab->addTab(text_editor, QObject::tr("Editing question"));
    QBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(tab);

    setLayout(layout);

    QWidget *tab2 = new QWidget(this);
    tab->addTab(tab2, tr("Answers"));
    QLayout *buttonLayout = new QVBoxLayout;
    QLayout *answersLayout = new QHBoxLayout;

    addBtn = new QPushButton(tr("Add"), this);
    editBtn = new QPushButton(tr("Edit"), this);
    editBtn->setEnabled(false);
    remBtn = new QPushButton(tr("Remove"), this);
    remBtn->setEnabled(false);

    QSpacerItem *spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    buttonLayout->addWidget(addBtn);
    buttonLayout->addWidget(editBtn);
    buttonLayout->addWidget(remBtn);
    buttonLayout->addItem(spacer);

    answers = new QTableWidget(0, 2, this);
    answers->setSelectionBehavior(QAbstractItemView::SelectRows);
    answers->setColumnWidth(0, 50);
    answers->setColumnWidth(1, 400);
    answers->setHorizontalHeaderLabels(QStringList() << tr("Right")
                                       << tr("Text"));

    answersLayout->addWidget(answers);
    answersLayout->addItem(buttonLayout);
    tab2->setLayout(answersLayout);

    connect(addBtn, SIGNAL(clicked()), this, SLOT(addAnswer()));
    connect(editBtn, SIGNAL(clicked()), this, SLOT(editAnswer()));
    connect(remBtn, SIGNAL(clicked()), this, SLOT(removeAnswer()));
    connect(answers, SIGNAL(cellClicked(int,int)), this, SLOT(updateCheckCell(int,int)));
    connect(answers, SIGNAL(itemSelectionChanged()), this, SLOT(answerSelectionChanged()));
    connect(text_editor, SIGNAL(contentChanged()), this, SLOT(validQuestion()));
}

void ClosedTestEditor::updateCheckCell(int row, int column) {
    if(column == 0) {
        QTableWidgetItem *item = answers->item(row, 0);
        item->setText((item->checkState() == Qt::Checked ? tr("Yes") : tr("No")));
        validQuestion();
    }
}

void ClosedTestEditor::addAnswer() {
    STextEditor *dlg = new STextEditor((QWidget*)this);
    if(dlg->exec() == QDialog::Accepted) {
        QTableWidgetItem *checkItem = new QTableWidgetItem;
        checkItem->setCheckState(Qt::Unchecked);
        checkItem->setText(tr("No"));
        checkItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);

        QTableWidgetItem *textItem = new QTableWidgetItem;
        textItem->setTextAlignment(Qt::AlignAbsolute | Qt::AlignVCenter);
        textItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        textItem->setText(dlg->plainText());
        textItem->setData(Qt::UserRole, dlg->html());

        int rows = answers->rowCount();
        answers->insertRow(rows);
        answers->setItem(rows, 0, checkItem);
        answers->setItem(rows, 1, textItem);
    }
    SAFE_DELETE(dlg);
    validQuestion();
}

void ClosedTestEditor::editAnswer() {
    STextEditor *dlg = new STextEditor((QWidget*)this);
    int row = answers->currentRow();
    dlg->setHtml(answers->item(row, 1)->data(Qt::UserRole).toString());

    if(dlg->exec() == QDialog::Accepted) {
        answers->item(row, 1)->setText(dlg->plainText());
        answers->item(row, 1)->setData(Qt::UserRole, dlg->html());
    }
    SAFE_DELETE(dlg);
    validQuestion();
}

void ClosedTestEditor::removeAnswer() {
    answers->removeRow(answers->currentRow());
    validQuestion();
}

void ClosedTestEditor::answerSelectionChanged() {
    bool enabled = (answers->rowCount() && answers->currentRow() != -1 ? true : false);
    validQuestion();
    editBtn->setEnabled(enabled);
    remBtn->setEnabled(enabled);
}

void ClosedTestEditor::validQuestion() {
    if((text_editor->isModified() || !text_editor->isEmpty())
            && answers->rowCount() != oldAnswersCount
            && answers->rowCount() >= 2) {
        bool fChecked = false;
        for(int i = 0; i < answers->rowCount(); i++) {
            if(answers->item(i, 0)->checkState() == Qt::Checked) {
                emit validationChanged(true);
                fChecked = true;
                break;
            }
        }
        if(!fChecked)
            emit validationChanged(false);
    }else {
        emit validationChanged(false);
    }
}

void ClosedTestEditor::getResource(int id, QString *name, QByteArray *data) {
    QStringList keys = resources.keys();
    *name = keys[id];
    *data = resources[*name];
}

int ClosedTestEditor::getResCount() {
    return answers->rowCount() + 1;
}

void ClosedTestEditor::makeQuestionConfig(QDomElement *questElement,
                                          QDomDocument document) {
    int answersCount = answers->rowCount();

    int checksCount = 0;
    QString questAlias = questElement->attribute("alias", QString::null);

    if(questAlias.isNull())
        return;

    resources.insert(QString("src\\quest_%1_source.html")
                     .arg(questAlias), text_editor->save());

    for(int i = 0; i < answersCount; i++) {
        QDomElement answerElement = document.createElement("answer");
        QString aSource = QString("src\\quest_%1_answer%2.html")
                .arg(questAlias)
                .arg(i + 1);

        answerElement.setAttribute("src", aSource);
        bool checked = (answers->item(i, 0)->checkState() == Qt::Checked ? true : false);
        answerElement.setAttribute("right", checked);
        answerElement.appendChild(document.createTextNode(answers->item(i, 1)->text()));

        resources.insert(aSource, answers->item(i, 1)->data(Qt::UserRole).toByteArray());

        if(checked)
            checksCount ++;

        (*questElement).appendChild(answerElement);
    }
    questElement->setAttribute("checksCount", checksCount);
}

QString ClosedTestEditor::makeQuestionAlias(int counter) {
    return QString("closed_quest_%1").arg(counter);
}

QString ClosedTestEditor::makeQuestionLabel() {
    return text_editor->plainText().left(15);
}

//////////////////////////////////////////////////////////////////////////////////////////
QStandardItem *Closed_Question::questinfo() {
    QStandardItem *item = new QStandardItem;
    item->appendRow(new QStandardItem(QObject::tr("Question with variants of answers")));
    item->appendRow(new QStandardItem("closed_question"));
    return item;
}

QuestEditorInterface *Closed_Question::editor() {
    return (QuestEditorInterface*) new ClosedTestEditor();
}

/*
*
*
*/
FreeChouseEditor::FreeChouseEditor(QWidget *parent) :
    QuestEditorInterface(parent)
{
    /*text_editor = new STextEditorView;
    QTabWidget *tab = new QTabWidget;
    tab->addTab(text_editor, QObject::tr("Editing question"));*/
    QBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(new QPushButton);

    setLayout(layout);
}

void FreeChouseEditor::getResource(int id, QString *name, QByteArray *data) {
}

int FreeChouseEditor::getResCount() {
    return 0;
}

void FreeChouseEditor::makeQuestionConfig(QDomElement *questElement,
                                          QDomDocument document) {
}

QString FreeChouseEditor::makeQuestionAlias(int counter) {
    return QString();
}

QString FreeChouseEditor::makeQuestionLabel() {
    return QString();
}

//////////////////////////////////////////////////////////////////////////////////////////
QStandardItem *FreeChouse_Question::questinfo() {
    QStandardItem *item = new QStandardItem;
    item->appendRow(new QStandardItem(QObject::tr("Free choice")));
    item->appendRow(new QStandardItem("free_question"));
    return item;
}

QuestEditorInterface *FreeChouse_Question::editor() {
    return (QuestEditorInterface*) new FreeChouseEditor();
}
