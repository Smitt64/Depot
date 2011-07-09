#include "mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QHeaderView>
#include "sproject.h"
#include "commands.h"
#include "dialogs/edittheme.h"

MainWindow::MainWindow(QWidget *parent) :
    CMainWindow(parent),
    commandsHistory(NULL),
    tst_struct(NULL),
    theme_item(NULL)
{
    setWindowTitle(tr("TestBuiler"));
    setDockOptions(QMainWindow::VerticalTabs);
    makeFileMenu();
    makeEditMenu();
    makeViewMenu();

    theme_mehu = addMenu(tr("Theme"), "theme");
    QAction *add_theme = addAction(tr("Add theme"), "add_theme", "theme", QIcon(":/addtheme"));
    add_theme->setStatusTip(tr("Adding new theme to test..."));
    add_theme->setEnabled(false);

    QAction *rem_theme = addAction(tr("Remove theme"), "remove_theme", "theme", QIcon(":/removetheme"));
    rem_theme->setStatusTip(tr("Removing current theme..."));
    rem_theme->setEnabled(false);

    QAction *edit_theme = addAction(tr("Edit theme"), "edit_theme", "theme");
    edit_theme->setStatusTip(tr("Edeting current theme..."));
    edit_theme->setEnabled(false);

    commandsHistory = new QUndoView(S_PROJECT->undoStack());
    commandsHistory->setEmptyLabel(tr("<empty>"));

    QDockWidget *undoDock = addDockPanel(tr("Changes list"), "ch_list", QIcon(":/legend"));
    undoDock->setWidget(commandsHistory);
    commandsHistory->setEnabled(false);

    tst_struct = new QTreeWidget;
    QDockWidget *test_struct = addDockPanel(tr("Test struct"), "tst_struct", QIcon(":/book"));
    test_struct->setWidget(tst_struct);
    tst_struct->setEnabled(false);
    tst_struct->setHeaderLabels(QStringList()
                                << tr("Elements") << tr("Alias"));
    tst_struct->header()->restoreState(SApplication::inst()->settings("window/tst_struct_header")
                                       .toByteArray());
    QHeaderView *hview = tst_struct->header();
    hview->resizeSection(0, 140);

    QAction *create = action("new");
    QAction *close = action("close");

    close->setEnabled(false);

    restore();

    connect(create, SIGNAL(triggered()), this, SLOT(createProject()));
    connect(close, SIGNAL(triggered()), this, SLOT(closeProject()));

    connect(add_theme, SIGNAL(triggered()), this, SLOT(addTheme()));
    connect(rem_theme, SIGNAL(triggered()), this, SLOT(removeTheme()));
    connect(edit_theme, SIGNAL(triggered()), this, SLOT(editTheme()));

    connect(S_PROJECT, SIGNAL(themeAdded(QString,QString)), this, SLOT(themeAdded(QString,QString)));
    connect(S_PROJECT, SIGNAL(themeRemoved(QString)), this, SLOT(themeRemoved(QString)));
    connect(tst_struct, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
            this, SLOT(tstSctructCurItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
}

void MainWindow::createProject() {
    QString filename = QFileDialog::getSaveFileName(this, tr("Creating new test..."),
                                                    "./", tr("Test file (*.tst)"));

    if(filename.isEmpty())
        return;

    if(!S_PROJECT->create(filename)) {
        QMessageBox::critical(this, tr("Creating error!"),
                              tr("Can't create new test!"));
        return;
    }

    setWindowFilePath(filename);

    commandsHistory->setStack(S_PROJECT->undoStack());
    commandsHistory->setEnabled(true);
    tst_struct->setEnabled(true);
    action("close")->setEnabled(true);
    action("add_theme")->setEnabled(true);

    updateTestStruct();
}

void MainWindow::updateTestStruct() {
    QTreeWidgetItem *top = new QTreeWidgetItem;
    top->setIcon(0, QIcon(":/book"));
    top->setText(0, tr("Test"));
    tst_struct->addTopLevelItem(top);

    theme_item = new QTreeWidgetItem;
    theme_item->setText(0, tr("Themes"));
    theme_item->setIcon(0, QIcon(":/themegroup"));
    top->addChild(theme_item);
}

void MainWindow::closeProject() {
    SApplication::inst()->writeSettings("window/tst_struct_header", tst_struct->header()->saveState());
    tst_struct->clear();
    tst_struct->setEnabled(false);
    commandsHistory->setEnabled(false);
    action("close")->setEnabled(false);
    action("undo")->setEnabled(false);
    action("redo")->setEnabled(false);
    action("add_theme")->setEnabled(false);
    action("remove_theme")->setEnabled(false);
}

void MainWindow::tstSctructCurItemChanged(QTreeWidgetItem *cur, QTreeWidgetItem *prev) {
    if(cur->data(0, Qt::UserRole).toString() == "theme") {
        action("remove_theme")->setEnabled(true);
        action("edit_theme")->setEnabled(true);
    }
    else {
        action("remove_theme")->setEnabled(false);
        action("edit_theme")->setEnabled(false);
    }
}

void MainWindow::addTheme() {
    bool ok;
    QString title = QInputDialog::getText(this, tr("Addeting new theme..."),
                                         tr("Enter caption for new theme:"),
                                          QLineEdit::Normal, "", &ok,
                                          Qt::Dialog | Qt::WindowTitleHint);
    if(!ok || title.isEmpty() || S_PROJECT->containsTheme(title))
        return;

    S_PROJECT->undoStack()->push(new addThemeCommand(title));
}

void MainWindow::removeTheme() {
    if(QMessageBox::question(this, tr("Removing theme."),
                          QString(tr("Are You really want remove theme ' %1 '?"))
                          .arg(tst_struct->currentItem()->text(0)),
                          QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No)
        return;

    S_PROJECT->undoStack()->push(new removeThemeCommand(tst_struct->currentItem()->text(1)));
}

void MainWindow::editTheme() {
    EditThemeDlg *dlg = new EditThemeDlg(this);
    dlg->setThemeTitleAlias(tst_struct->currentItem()->text(0), tst_struct->currentItem()->text(1));
    if(dlg->exec() == QDialog::Rejected) {
        SAFE_DELETE(dlg);
        return;
    }
}

void MainWindow::themeAdded(QString title, QString alias) {
    QTreeWidgetItem *item = new QTreeWidgetItem;
    item->setText(0, title);
    item->setText(1, alias);
    item->setIcon(0, QIcon(":/theme"));
    item->setToolTip(0, QString(tr("<P><B>Theme: </B>%1</P><B>Alias: </B>%2"))
                     .arg(title, alias));
    item->setData(0, Qt::UserRole, "theme");
    theme_item->addChild(item);
}

void MainWindow::themeRemoved(QString alias) {
    for(int i = 0; i < theme_item->childCount(); i++){
        QTreeWidgetItem *item = theme_item->child(i);
        if(item->text(1) == alias) {
            delete item;
            break;
        }
    }
}

