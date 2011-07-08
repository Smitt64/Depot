#include "mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include "sproject.h"
#include "commands.h"

MainWindow::MainWindow(QWidget *parent) :
    CMainWindow(parent),
    commandsHistory(NULL),
    tst_struct(NULL)
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

    commandsHistory = new QUndoView;
    commandsHistory->setEmptyLabel(tr("<empty>"));

    QDockWidget *undoDock = addDockPanel(tr("Changes list"), "ch_list", QIcon(":/legend"));
    undoDock->setWidget(commandsHistory);
    commandsHistory->setEnabled(false);

    tst_struct = new QTreeWidget;
    QDockWidget *test_struct = addDockPanel(tr("Test struct"), "tst_struct", QIcon(":/book"));
    test_struct->setWidget(tst_struct);
    tst_struct->setEnabled(false);
    tst_struct->setHeaderLabel(tr("Elements"));

    QAction *create = action("new");
    QAction *close = action("close");

    close->setEnabled(false);

    restore();

    connect(create, SIGNAL(triggered()), this, SLOT(createProject()));
    connect(close, SIGNAL(triggered()), this, SLOT(closeProject()));

    connect(add_theme, SIGNAL(triggered()), this, SLOT(addTheme()));
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
}

void MainWindow::closeProject() {
    tst_struct->clear();
    tst_struct->setEnabled(false);
    commandsHistory->setEnabled(false);
    action("close")->setEnabled(false);
    action("undo")->setEnabled(false);
    action("redo")->setEnabled(false);
    action("add_theme")->setEnabled(false);
}

void MainWindow::addTheme() {
    bool ok;
    QString title = QInputDialog::getText(this, tr("Addeting new theme..."),
                                         tr("Enter caption for new theme:"),
                                          QLineEdit::Normal, "", &ok,
                                          Qt::Dialog | Qt::WindowTitleHint);
    if(title.isEmpty() || S_PROJECT->containsTheme(title))
        return;

    S_PROJECT->undoStack()->push(new addThemeCommand(title));
}
