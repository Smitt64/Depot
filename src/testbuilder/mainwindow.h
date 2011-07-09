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
    void removeTheme();
    void editTheme();
    void themeAdded(QString title, QString alias);
    void themeRemoved(QString alias);
    void tstSctructCurItemChanged(QTreeWidgetItem *cur, QTreeWidgetItem *prev);

private:
    void updateTestStruct();
    QUndoView *commandsHistory;
    QTreeWidget *tst_struct;
    QMenu *theme_mehu;
    QTreeWidgetItem *theme_item;
};

#endif // MAINWINDOW_H
