#ifndef DEFAULT_TST_TYPES_H
#define DEFAULT_TST_TYPES_H

#include "../interfaces/testtype_interface.h"
#include "../stexteditor.h"
#include <QPushButton>
#include <QTableWidget>

//////////////////////////////////////////////////////////////////////////////////////////

/*!
  \english
  ClosedTestEditor class provides
  \endenglish
  */
class ClosedTestEditor : QuestEditorInterface
{
    Q_OBJECT
public:
    ClosedTestEditor(QWidget *parent = 0);

    virtual void getResource(int id, QString *name, QByteArray *data);
    virtual int getResCount();
    virtual void makeQuestionConfig(QDomElement *questElement, QDomDocument document);
    virtual QString makeQuestionAlias(int counter);
    virtual QString makeQuestionLabel();

private slots:
    void addAnswer();
    void editAnswer();
    void removeAnswer();
    void updateCheckCell(int row, int column);
    void answerSelectionChanged();
    void validQuestion();

private:
    STextEditorView *text_editor;
    QPushButton *addBtn, *editBtn, *remBtn;
    QTableWidget *answers;
    int oldAnswersCount;
    QMap<QString, QByteArray> resources;
};

class Closed_Question : public TestType_Interface
{
public:
    virtual QStandardItem *questinfo();
    virtual QuestEditorInterface *editor();
};
//////////////////////////////////////////////////////////////////////////////////////////
class FreeChouseEditor : QuestEditorInterface
{
public:
    FreeChouseEditor(QWidget *parent = 0);

    virtual void getResource(int id, QString *name, QByteArray *data);
    virtual int getResCount();
    virtual void makeQuestionConfig(QDomElement *questElement, QDomDocument document);
    virtual QString makeQuestionAlias(int counter);
    virtual QString makeQuestionLabel();

private:
    STextEditorView *text_editor;
};

class FreeChouse_Question : public TestType_Interface
{
public:
    virtual QStandardItem *questinfo();
    virtual QuestEditorInterface *editor();
};

#endif // DEFAULT_TST_TYPES_H
