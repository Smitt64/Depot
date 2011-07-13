#ifndef SMAINWINDOW_H
#define SMAINWINDOW_H

#include <QWidget>
#include <QMainWindow>
#include <QMenuBar>
#include <QMap>
#include <QToolBar>
#include <QAction>
#include <QMenu>
#include <QStatusBar>
#include <QRect>
#include <QDockWidget>
#include "sproject.h"

class QToolBar;

class CMainWindow : public QMainWindow {
    Q_OBJECT

    friend class SCustomizeDlg;
public:
    CMainWindow(QWidget *parent = 0);
    virtual ~CMainWindow();
    void restore();

    QToolBar *addToolBar(const QString &title, const QString &name);
    QMenu *addMenu(const QString &title, const QString &name, const QIcon &icon = QIcon(), const QString &menu = "");
    QAction *addAction(const QString &title, const QString &name, const QString &menu = "", const QIcon &icon = QIcon());
    QAction *addAction(QAction *action, const QString &name, const QString &menu = "");
    QDockWidget *addDockPanel(const QString &title, const QString &name, const QIcon &icon = QIcon());
    void makeFileMenu(const bool &withToolBar = true);
    void makeEditMenu();
    void makeViewMenu();
    void makeServiceMenu();
    void setWindowFilePath(const QString &filePath);

    QAction *action(const QString &title);

private slots:
    void checkToolBars(bool value);
    void checkStatusBar(bool value);
    void fullscreen(bool value);
    void dockVisiblseChanged(bool value);
    void canUndo(bool value);
    void canRedo(bool value);
    void tool_customize();

private:
    QMap<QString, QToolBar*> toolBars;
    QMap<QString, QAction*> actions;
    QMap<QString, QMenu*> menus;
    QMap<QString, QDockWidget*> docks;
    QMenu *fileMenu, *editMenu,*viewMenu;

protected:
    virtual void closeEvent(QCloseEvent *event);
};
#endif
