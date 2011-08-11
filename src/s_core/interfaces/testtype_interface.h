#ifndef TESTTYPE_INTERFACE_H
#define TESTTYPE_INTERFACE_H

#include <QStandardItem>
#include "questeditor_interface.hpp"

class TestType_Interface : public QObject {
    Q_OBJECT
public:
    //TestType_Interface(QObject *parent = 0);
    virtual ~TestType_Interface() {}
    /*
      row 1: Type caption;
      row 2: Name;
      */
    virtual QStandardItem *questinfo() = 0;
    virtual QuestEditorInterface *editor() = 0;
};

Q_DECLARE_INTERFACE(TestType_Interface, "Depot.TestBuilder.Plugin/1.0")

#endif // TESTTYPE_INTERFACE_H
