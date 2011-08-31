#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QUndoView>
#include <QTreeWidget>
#include <QMdiArea>
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
    void openProject();
    void saveProject();
    void projectModifyed(bool value);

    void addTheme();
    void removeTheme();
    void editTheme();
    void themeAdded(QString title, QString alias);
    void themeRemoved(QString alias);

    void questionAdded(QString alias, QString label, QStringList toThemes);
    void questRemoved(QString qAlias, QStringList fromThemes);
    void tstSctructCurItemChanged(QTreeWidgetItem *cur, QTreeWidgetItem *prev);

    void addQuestion();

    void debugOutput(QString msg);
    void tstStructContextMenu(QPoint pos);

private:
    void updateTestStruct();
    QUndoView *commandsHistory;
    QTreeWidget *tst_struct;
    QMenu *theme_mehu, *quest_menu;
    QTreeWidgetItem *theme_item;
    QMdiArea *mdiArea;
};

#endif // MAINWINDOW_H
