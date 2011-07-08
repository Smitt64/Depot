#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QUndoView>
#include <QTreeWidget>
#include "cmainwindow.h"

class MainWindow : public CMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);

signals:

private slots:
    void createProject();
    void closeProject();

    void addTheme();

private:
    void updateTestStruct();
    QUndoView *commandsHistory;
    QTreeWidget *tst_struct;
    QMenu *theme_mehu;
};

#endif // MAINWINDOW_H
