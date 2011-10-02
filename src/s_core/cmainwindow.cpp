#include "cmainwindow.h"
#include "sapplication.h"
#include "sproject.h"
#include "scustomizedlg.h"
#include <QtGui>
#include <QDomDocument>
#include <QTreeWidget>

CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent),
    fileMenu(NULL),
    editMenu(NULL),
    viewMenu(NULL),
    viewSeparator(NULL)
{
#ifndef S_OS_MEEGO
    setProperty("size", QSize(640, 480));
#endif
    setCentralWidget(new QWidget);
    setDocumentMode(false);
    setDockNestingEnabled(true);
    setWindowModality(Qt::WindowModal);
}

CMainWindow::~CMainWindow() {
    SAFE_DELETE(fileMenu);
    toolBars.clear();
    menus.clear();
    sactions.clear();
}

void CMainWindow::restore() {
    QDomDocument doc;
    if(doc.setContent(SApplication::inst()->settings("window/user_toolbars").toByteArray())) {
        QDomElement root = doc.documentElement();

        for(QDomNode n = root.firstChild(); !n.isNull(); n = n.nextSibling()) {
            QDomElement bar_element = n.toElement();
            if(bar_element.tagName() != "bar")
                continue;
            QString title = bar_element.attribute("title", "");
            QString name = bar_element.attribute("name", "");

            if(title.isEmpty() || name.isEmpty())
                continue;

            SToolBar *bar = addToolBar(title, name);
            bar->setUser(true);

            for(QDomNode n2 = bar_element.firstChild(); !n2.isNull(); n2 = n2.nextSibling()) {
                 QDomElement action_element = n2.toElement();
                 if(action_element.tagName() == "action") {
                     QString name = action_element.attribute("name", "");
                     if(name.isEmpty())
                         continue;
                     if(sactions.contains(name))
                         bar->addAction(sactions[name]);
                 }
                 else if(action_element.tagName() == "separator") {
                     bar->addSeparator();
                 }
            }
        }
    }
    restoreState(SApplication::inst()->settings("window/state").toByteArray());
    restoreGeometry(SApplication::inst()->settings("window/geometry").toByteArray());

    if(sactions.contains("fullscreen"))
        sactions["fullscreen"]->setChecked(isFullScreen());
}

SToolBar *CMainWindow::addToolBar(const QString &title,
                                  const QString &name) {
    SToolBar *bar = new SToolBar;
    bar->setWindowTitle(title);
    bar->setObjectName(name);
    QMainWindow::addToolBar((QToolBar*)bar);
    toolBars.insert(name, bar);
    toolBars[name]->setIconSize(SApplication::inst()->settings("interface/tb_iconsize",
                                                                QSize(16, 16)).toSize());
    if(viewSeparator) {
        menus["View"]->insertAction(viewSeparator, bar->toggleViewAction());
    }

    return toolBars.value(name);
}

SToolBar *CMainWindow::addToolBar(SToolBar *bar) {
    QMainWindow::addToolBar((QToolBar*)bar);
    toolBars.insert(bar->objectName(), bar);

    if(viewSeparator) {
        menus["View"]->insertAction(viewSeparator, bar->toggleViewAction());
    }

    return toolBars.value(bar->objectName());
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
    if(sactions.contains(name))
        return NULL;

    if(menus.contains(menu)) {
        sactions.insert(name, (icon.isNull() ?  menus[menu]->addAction(title) :
                                                menus[menu]->addAction(icon, title)));
        sactions.value(name)->setData(name);
        sactions.value(name)->setObjectName(name);
        return sactions.value(name);
    }
    else {
       sactions.insert(name, menuBar()->addAction(title));
       sactions.value(name)->setData(name);
       sactions.value(name)->setObjectName(name);
       return sactions[name];
    }
}

QAction *CMainWindow::addAction(QAction *action,
                                const QString &name,
                                const QString &menu) {
    if(sactions.contains(name))
        return NULL;

    if(menus.contains(menu))
        menus[menu]->addAction(action);
    else
        menuBar()->addAction(action);
    sactions.insert(name, action);
    sactions[name]->setData(name);
    sactions[name]->setObjectName(name);
    return sactions[name];
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


        QAction *fileNew = addAction(tr("New"), "new", "File", QIcon::fromTheme("document-new"));
        fileNew->setStatusTip(tr("Create new file..."));
        fileNew->setShortcut(QKeySequence::New);

        QAction *fileOpen = addAction(tr("Open..."), "open", "File", QIcon::fromTheme("document-open"));
        fileOpen->setStatusTip(tr("Open exist file..."));
        fileOpen->setShortcut(QKeySequence::Open);

        QAction *fileSave = addAction(tr("Save..."), "save", "File", QIcon::fromTheme("document-save"));
        fileSave->setEnabled(false);
        fileSave->setStatusTip(tr("Save changes..."));
        fileSave->setShortcut(QKeySequence::Save);

        QAction *fileSaveAs = addAction(tr("Save as..."), "saveas", "File", QIcon::fromTheme("document-save-as"));
        fileSaveAs->setEnabled(false);
        fileSaveAs->setStatusTip(tr("Save as new file..."));
        fileSaveAs->setShortcut(QKeySequence::SaveAs);

        QAction *fileClose = addAction(tr("Close"), "close", "File");
        fileClose->setEnabled(false);
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
    QAction *undo_action = addAction(tr("Undo"), "undo", "Edit", QIcon::fromTheme("edit-undo"));
    undo_action->setShortcut(QKeySequence::Undo);
    undo_action->setStatusTip(tr("Cancellation of last action..."));
    undo_action->setEnabled(false);

    QAction *redo_action = addAction(tr("Redo"), "redo", "Edit", QIcon::fromTheme("edit-undo"));
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
    foreach (SToolBar *bar, toolBars) {
        viewMenu->addAction(bar->toggleViewAction());
    }

    if(!statusBar()) {
        setStatusBar(new QStatusBar);
    }

    addAction(tr("Status bar"), "sb", "View");
    sactions["sb"]->setCheckable(true);
    sactions["sb"]->setStatusTip(tr("Show/hide status bar..."));
    sactions["sb"]->setChecked(SApplication::inst()->settings("view/status_bar", true).toBool());

    statusBar()->setVisible(sactions["sb"]->isChecked());
    connect(sactions["sb"], SIGNAL(triggered(bool)), this, SLOT(checkStatusBar(bool)));

    viewSeparator = menus["View"]->addSeparator();
    QAction *fullscrAction = addAction(tr("Full screen"), "fullscreen", "View", QIcon::fromTheme("view-fullscreen"));
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
    QString key = sactions.key(dock);
    SApplication::inst()->writeSettings(key, value);
}

void CMainWindow::closeEvent(QCloseEvent *event) {
    SApplication::inst()->writeSettings("window/state", saveState());
    SApplication::inst()->writeSettings("window/geometry", saveGeometry());
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
    if(sactions.contains(title)) {
        return sactions[title];
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
    sactions["undo"]->setEnabled(value);
}

void CMainWindow::canRedo(bool value) {
    sactions["redo"]->setEnabled(value);
}

void CMainWindow::makeServiceMenu() {
    QMenu *service_menu = addMenu(tr("Service"), "service_menu");

    QAction *customize = addAction(tr("Customize"), "custom_action", "service_menu");
    customize->setStatusTip(tr("Customizing toolbars..."));

    dlg = new SCustomizeDlg(this, this);
    dlg->setWindowModality(Qt::NonModal);

    connect(dlg, SIGNAL(finished(int)), this, SLOT(customizeDlgClosed(int)));
    connect(customize, SIGNAL(triggered()), dlg, SLOT(show()));
}

QList<QAction*> CMainWindow::actionList() {
    QList<QAction*> list;
    QStringList ac = sactions.keys();

    for(int i = 0; i < ac.count(); i++) {
        list.append(sactions[ac[i]]);
    }
    return list;
}

void CMainWindow::removeAction(const QString &name) {

    if(sactions.contains(name)) {
        QAction *action = sactions[name];
        sactions.remove(name);
    }
}

void CMainWindow::customizeDlgClosed(int result) {   
    if(result == QDialog::Rejected)
        return;

    QByteArray data;
    QTextStream stream(&data, QIODevice::WriteOnly);
    stream.setCodec("UTF-8");
    QDomDocument doc("user_toolbars");
    QDomElement root = doc.createElement("toolbars");

    foreach (SToolBar *bar, toolBars) {
        if(!bar->isUser())
            continue;

        QDomElement bar_element = doc.createElement("bar");
        bar_element.setAttribute("name", bar->objectName());
        bar_element.setAttribute("title", bar->windowTitle());
        QList<QAction*> bar_actions = bar->actions();

        foreach (QAction *action, bar_actions) {
            if(action->isSeparator()) {
                QDomElement sep_element = doc.createElement("separator");
                bar_element.appendChild(sep_element);
            } else {
                QDomElement action_element = doc.createElement("action");
                action_element.setAttribute("name", action->objectName());
                action_element.appendChild(doc.createTextNode(action->text()));
                bar_element.appendChild(action_element);
            }
        }
        root.appendChild(bar_element);
    }

    doc.appendChild(root);
    doc.save(stream, 3);
    SApplication::inst()->writeSettings("window/user_toolbars", data);
}

void CMainWindow::showExpanded() {
#if defined(Q_OS_SYMBIAN) || defined(MEEGO_EDITION_HARMATTAN) || defined(Q_WS_SIMULATOR)
    showFullScreen();
#elif defined(Q_WS_MAEMO_5)
    showMaximized();
#else
    show();
#endif
}
