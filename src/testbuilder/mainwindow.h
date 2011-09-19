#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QUndoView>
#include <QtGui>
#include "cmainwindow.h"

class MainWindow : public CMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);

signals:

public slots:
    void openDocument(const QString &fileName);

private slots:
    void createProject();
    void closeProject();
    void openProject();
    void saveProject();
    void projectModifyed(bool value);
    void projectClosed();

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
    void showTestConfig();
    void openRecentFile();

private:
    void updateTestStruct();
    void updateRecentFileActions();
    void setCurrentFile(const QString &fileName);
    QString curFile;
    QList<QAction*> recentFileActs;
    int MaxRecentFiles;
    QAction *separatorAct;
    QUndoView *commandsHistory;
    QTreeWidget *tst_struct;
    QComboBox *helpSearch;
    QMenu *theme_mehu, *quest_menu, *test_menu;
    QTreeWidgetItem *theme_item;
    QMdiArea *mdiArea;
};

#endif // MAINWINDOW_H
