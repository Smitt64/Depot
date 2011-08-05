#include "default_tst_types.h"
#include <QBoxLayout>
#include <QTabWidget>
#include <QPushButton>
#include <QDebug>

//////////////////////////////////////////////////////////////////////////////////////////
ClosedTestEditor::ClosedTestEditor() :
    QuestEditorInterface()
{
    text_editor = new STextEditorView;
    QTabWidget *tab = new QTabWidget;
    tab->addTab(text_editor, QObject::tr("Editing question"));
    QBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(tab);

    setLayout(layout);
}

void ClosedTestEditor::getResource(int id, QString *name, QByteArray *data) {
}

int ClosedTestEditor::getResCount() {
    return 0;
}

void ClosedTestEditor::makeQuestion(QAbstractItemModel *questions) {

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
FreeChouseEditor::FreeChouseEditor() :
    QuestEditorInterface()
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

void FreeChouseEditor::makeQuestion(QAbstractItemModel *questions) {

}
//////////////////////////////////////////////////////////////////////////////////////////
QStandardItem *FreeChouse_Question::questinfo() {
    QStandardItem *item = new QStandardItem;
    item->appendRow(new QStandardItem(QObject::tr("Free choice")));
    item->appendRow(new QStandardItem("free_question"));
    return item;
}

QuestEditorInterface *FreeChouse_Question::editor() {
    qDebug() << "FreeChouse_Question editor";
    return (QuestEditorInterface*) new FreeChouseEditor();
}
