#include "cmainwindow.h"
#include "sapplication.h"
#include <QtGui>
#include <QTreeWidget>
#include "sproject.h"

CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent),
    fileMenu(NULL),
    editMenu(NULL),
    viewMenu(NULL)
{
    setProperty("size", QSize(640, 480));
    setCentralWidget(new QWidget);
    setDocumentMode(false);
    setDockNestingEnabled(true);
    setWindowModality(Qt::WindowModal);
}

CMainWindow::~CMainWindow() {
    SAFE_DELETE(fileMenu);
    toolBars.clear();
    menus.clear();
    actions.clear();
}

void CMainWindow::restore() {
    restoreState(SApplication::inst()->settings("window/state").toByteArray());
    restoreGeometry(SApplication::inst()->settings("window/geometry").toByteArray());

    if(actions.contains("fullscreen"))
        actions["fullscreen"]->setChecked(isFullScreen());
}

QToolBar *CMainWindow::addToolBar(const QString &title,
                                  const QString &name) {
    toolBars.insert(name, QMainWindow::addToolBar(title));
    toolBars[name]->setIconSize(SApplication::inst()->settings("interface/tb_iconsize",
                                                                QSize(16, 16)).toSize());
    return toolBars.value(name);
}

QMenu *CMainWindow::addMenu(const QString &title,
                            const QString &name,
                            const QIcon &icon,
                            const QString &menu) {
    if(!menuBar())
        setMenuBar(new QMenuBar);
    if(menu.isEmpty())
        menus.insert(name, (icon.isNull() ? menuBar()->addMenu(title) : menuBar()->addMenu(icon, title)));
    else {
        menus.insert(name, menus[menu]->addMenu(icon, menu));
    }
    return menus.value(name);
}

QAction *CMainWindow::addAction(const QString &title,
                                const QString &name,
                                const QString &menu,
                                const QIcon &icon) {
    if(menus.contains(menu)) {
        actions.insert(name, (icon.isNull() ?  menus[menu]->addAction(title) :
                                                menus[menu]->addAction(icon, title)));
        return actions.value(name);
    }
    else {
       return menuBar()->addAction(name);
    }
}

QAction *CMainWindow::addAction(QAction *action,
                                const QString &name,
                                const QString &menu) {
    if(menus.contains(menu))
        menus[menu]->addAction(action);
    else
        menuBar()->addAction(action);
    actions.insert(name, action);
    return action;
}

QDockWidget *CMainWindow::addDockPanel(const QString &title, const QString &name, const QIcon &icon)
{
    QDockWidget *dock = new QDockWidget(title, this);
    docks.insert(name, dock);
    dock->setObjectName(name);
    addDockWidget(Qt::LeftDockWidgetArea, dock);

    if(!icon.isNull())
        dock->setWindowIcon(icon);

    if(menus.contains("View"))
    {
        QAction *action = dock->toggleViewAction();
        if(!icon.isNull())
            action->setIcon(icon);
        QString act_name = QString("%1_action")
                .arg(name);
        addAction(action, act_name, "View");

        connect(action, SIGNAL(triggered(bool)), this, SLOT(dockVisiblseChanged(bool)));

        dock->setVisible(SApplication::inst()->settings(act_name, false).toBool());
    }
    return dock;
}

void CMainWindow::makeFileMenu(const bool &withToolBar) {
    if(!fileMenu) {
        fileMenu = addMenu(tr("File"), "File");
        QAction *fileNew = addAction(tr("New"), "new", "File", QIcon(":/new"));
        fileNew->setStatusTip(tr("Create new file..."));
        fileNew->setShortcut(QKeySequence::New);

        QAction *fileOpen = addAction(tr("Open..."), "open", "File", QIcon(":/open"));
        fileOpen->setStatusTip(tr("Open exist file..."));
        fileOpen->setShortcut(QKeySequence::Open);

        QAction *fileSave = addAction(tr("Save..."), "save", "File", QIcon(":/save"));
        fileSave->setStatusTip(tr("Save changes..."));
        fileSave->setShortcut(QKeySequence::Save);

        QAction *fileSaveAs = addAction(tr("Save as..."), "saveas", "File", QIcon(":/saveall"));
        fileSaveAs->setStatusTip(tr("Save as new file..."));
        fileSaveAs->setShortcut(QKeySequence::SaveAs);

        QAction *fileClose = addAction(tr("Close"), "close", "File");
        fileClose->setStatusTip(tr("Close current file..."));
        fileClose->setShortcut(QKeySequence::Close);

        if(withToolBar) {
            QToolBar *standart = addToolBar(tr("Standart"), "Standart");
            standart->setObjectName("Standart");
            standart->addAction(fileNew);
            standart->addAction(fileOpen);
            standart->addAction(fileSave);
            standart->addAction(fileSaveAs);
            standart->addAction(fileClose);
        }
    }
}

void CMainWindow::makeEditMenu() {
    if(editMenu)
        return;

    editMenu = addMenu(tr("Edit"), "Edit");
    QAction *undo_action = addAction(tr("Undo"), "undo", "Edit", QIcon(":/undo"));
    undo_action->setShortcut(QKeySequence::Undo);
    undo_action->setStatusTip(tr("Cancellation of last action..."));
    undo_action->setEnabled(false);

    QAction *redo_action = addAction(tr("Redo"), "redo", "Edit", QIcon(":/redo"));
    redo_action->setShortcut(QKeySequence::Redo);
    redo_action->setStatusTip(tr("Repeated performance of last action..."));
    redo_action->setEnabled(false);

    editMenu->addSeparator();

    connect(S_PROJECT->undoStack(), SIGNAL(canUndoChanged(bool)), this, SLOT(canUndo(bool)));
    connect(S_PROJECT->undoStack(), SIGNAL(canRedoChanged(bool)), this, SLOT(canRedo(bool)));
    connect(undo_action, SIGNAL(triggered()), S_PROJECT->undoStack(), SLOT(undo()));
    connect(redo_action, SIGNAL(triggered()), S_PROJECT->undoStack(), SLOT(redo()));
}

void CMainWindow::makeViewMenu() {
    viewMenu = addMenu(tr("View"), "View");

    QAction *action;
    foreach (QToolBar *bar, toolBars) {
        action = addAction(bar->windowTitle(), bar->windowTitle(), "View");
        action->setData(toolBars.key(bar));
        action->setCheckable(true);
        action->setChecked(SApplication::inst()->settings(QString("view/%1")
                                                          .arg(action->data().toString()),
                                                          true).toBool());
        bar->setVisible(action->isChecked());

        connect(action, SIGNAL(triggered(bool)), this, SLOT(checkToolBars(bool)));
    }
    addAction(tr("Status bar"), "sb", "View");
    actions["sb"]->setCheckable(true);
    actions["sb"]->setStatusTip(tr("Show/hide status bar..."));
    actions["sb"]->setChecked(SApplication::inst()->settings("view/status_bar", true).toBool());
    if(!statusBar()) {
        setStatusBar(new QStatusBar);
    }
    statusBar()->setVisible(actions["sb"]->isChecked());
    connect(actions["sb"], SIGNAL(triggered(bool)), this, SLOT(checkStatusBar(bool)));

    menus["View"]->addSeparator();
    QAction *fullscrAction = addAction(tr("Full screen"), "fullscreen", "View", QIcon(":/fullscreen"));
    fullscrAction->setCheckable(true);
    fullscrAction->setStatusTip(tr("Show window full screen..."));
    fullscrAction->setShortcut(QKeySequence(Qt::SHIFT + Qt::ALT + Qt::Key_Enter));
    menus["View"]->addSeparator();

    connect(fullscrAction, SIGNAL(triggered(bool)), this, SLOT(fullscreen(bool)));
}

void CMainWindow::checkToolBars(bool value) {
    QAction *action = (QAction*)sender();
    toolBars[action->data().toString()]->setVisible(value);
    SApplication::inst()->writeSettings(QString("view/%1")
                                        .arg(action->data().toString()), value);
}

void CMainWindow::checkStatusBar(bool value) {
    if(!statusBar()) {
        setStatusBar(new QStatusBar);
    }
    statusBar()->setVisible(value);
    SApplication::inst()->writeSettings("view/status_bar", value);
}

void CMainWindow::dockVisiblseChanged(bool value) {
    QAction *dock = (QAction*)sender();
    QString key = actions.key(dock);
    SApplication::inst()->writeSettings(key, value);
}

void CMainWindow::closeEvent(QCloseEvent *event) {
    SApplication::inst()->writeSettings("window/geometry", saveGeometry());
    SApplication::inst()->writeSettings("window/state", saveState());
}

void CMainWindow::setWindowFilePath(const QString &filePath) {
    QString wTitle = windowTitle();
    int index = -1;
    if((index = wTitle.indexOf('-')) != -1)
        wTitle = wTitle.remove(index, wTitle.length());

    if(!filePath.isEmpty()) {
        setWindowTitle(QString("%1 - %2")
                       .arg(wTitle)
                       .arg(QFileInfo(filePath).fileName()));
    }
    else {
        setWindowTitle(wTitle);
    }
    QMainWindow::setWindowFilePath(filePath);
}

QAction *CMainWindow::action(const QString &title) {
    if(actions.contains(title)) {
        return actions[title];
    }
    return new QAction(NULL);
}

void CMainWindow::fullscreen(bool value) {
    if(value)
        showFullScreen();
    else
        showNormal();
}

void CMainWindow::canUndo(bool value) {
    actions["undo"]->setEnabled(value);
}

void CMainWindow::canRedo(bool value) {
    actions["redo"]->setEnabled(value);
}
