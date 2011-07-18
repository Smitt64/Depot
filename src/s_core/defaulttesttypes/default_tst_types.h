#ifndef DEFAULT_TST_TYPES_H
#define DEFAULT_TST_TYPES_H

#include "../interfaces/testtype_interface.h"
#include "../stexteditor.h"

//////////////////////////////////////////////////////////////////////////////////////////

class ClosedTestEditor : QuestEditorInterface
{
public:
    ClosedTestEditor();

    virtual void getResource(int id, QString *name, QByteArray *data);
    virtual int getResCount();
    virtual void makeQuestion(QAbstractItemModel *questions);

private:
    STextEditorView *text_editor;
};

class Closed_Question : public TestType_Interface
{
public:
    virtual QStandardItem *questinfo();
    virtual QuestEditorInterface *editor();
};
//////////////////////////////////////////////////////////////////////////////////////////

#endif // DEFAULT_TST_TYPES_H
