#include "mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QHeaderView>
#include <QTableView>
#include <QMdiSubWindow>
#include "sproject.h"
#include "commands.h"
#include "questionsmodel.h"
#include "dialogs/editquestion.h"
#include "dialogs/edittheme.h"
#include "shelpcontentviewwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    CMainWindow(parent),
    commandsHistory(NULL),
    tst_struct(NULL),
    theme_item(NULL),
    mdiArea(NULL)
{
    mdiArea = new QMdiArea(this);
    mdiArea->setDocumentMode(false);
    mdiArea->setViewMode(QMdiArea::TabbedView);
    QMdiSubWindow *subWindow = mdiArea->addSubWindow(SApplication::inst()->helpViewWidget());
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    subWindow->showMaximized();
    setCentralWidget(mdiArea);

    setWindowTitle(tr("TestBuiler"));
    setDockOptions(QMainWindow::VerticalTabs);

    makeFileMenu();
    makeEditMenu();
    makeViewMenu();

    theme_mehu = addMenu(tr("Theme"), "theme");
    QAction *add_theme = addAction(tr("Add theme"), "add_theme", "theme",
                                   QIcon(":/addtheme"));
    add_theme->setStatusTip(tr("Adding new theme to test..."));
    add_theme->setEnabled(false);

    QAction *rem_theme = addAction(tr("Remove theme"), "remove_theme", "theme",
                                   QIcon(":/removetheme"));
    rem_theme->setStatusTip(tr("Removing current theme..."));
    rem_theme->setEnabled(false);

    QAction *edit_theme = addAction(tr("Edit theme"), "edit_theme", "theme");
    edit_theme->setStatusTip(tr("Edeting current theme..."));
    edit_theme->setEnabled(false);

    quest_menu = addMenu(tr("Question"), "quest_menu");
    QAction *add_quest = addAction(tr("Add question"), "add_quest", "quest_menu",
                                   QIcon(":/add_quest"));
    add_quest->setStatusTip(tr("Adding new question..."));
    add_quest->setEnabled(false);

    QAction *rem_quest = addAction(tr("Remove question"), "rem_quest", "quest_menu",
                                   QIcon(":/removeQuestion"));
    rem_quest->setStatusTip(tr("Removing current question..."));
    rem_quest->setEnabled(false);

    commandsHistory = new QUndoView(S_PROJECT->undoStack());
    commandsHistory->setEmptyLabel(tr("<empty>"));

    QDockWidget *undoDock = addDockPanel(tr("Changes list"), "ch_list",
                                         QIcon(":/legend"));
    undoDock->setWidget(commandsHistory);
    commandsHistory->setEnabled(false);

    tst_struct = new QTreeWidget;
    tst_struct->setContextMenuPolicy(Qt::CustomContextMenu);
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
    QAction *open = action("open");
    QAction *save = action("save");
    QAction *close = action("close");

    close->setEnabled(false);

    makeServiceMenu();

    restore();

    QTableView *table = new QTableView;
    table->setModel(S_PROJECT->questions());
    QMdiSubWindow *subWindow2 = mdiArea->addSubWindow(table);
    subWindow2->setAttribute(Qt::WA_DeleteOnClose);
    //subWindow2->showMaximized();

    connect(create, SIGNAL(triggered()), this, SLOT(createProject()));
    connect(open, SIGNAL(triggered()), this, SLOT(openProject()));
    connect(save, SIGNAL(triggered()), this, SLOT(saveProject()));
    connect(close, SIGNAL(triggered()), this, SLOT(closeProject()));

    connect(add_theme, SIGNAL(triggered()), this, SLOT(addTheme()));
    connect(rem_theme, SIGNAL(triggered()), this, SLOT(removeTheme()));
    connect(edit_theme, SIGNAL(triggered()), this, SLOT(editTheme()));

    connect(add_quest, SIGNAL(triggered()), this, SLOT(addQuestion()));

    connect(S_PROJECT, SIGNAL(themeAdded(QString,QString)), this, SLOT(themeAdded(QString,QString)));
    connect(S_PROJECT, SIGNAL(themeRemoved(QString)), this, SLOT(themeRemoved(QString)));
    connect(tst_struct, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
            this, SLOT(tstSctructCurItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
    connect(tst_struct, SIGNAL(customContextMenuRequested(QPoint)), this,
            SLOT(tstStructContextMenu(QPoint)));

    connect(S_PROJECT, SIGNAL(error(QString)), this, SLOT(debugOutput(QString)));
    connect(S_PROJECT, SIGNAL(modifyChanged(bool)), this, SLOT(projectModifyed(bool)));
    connect(this, SIGNAL(destroyed()), S_PROJECT, SLOT(close()));

    connect(S_PROJECT, SIGNAL(questionAdded(QString,QString,QStringList)), this,
            SLOT(questionAdded(QString,QString,QStringList)));
    connect(S_PROJECT, SIGNAL(questionRemoved(QString,QStringList)), this,
            SLOT(questRemoved(QString,QStringList)));
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
    action("add_quest")->setEnabled(true);

    updateTestStruct();
}

void MainWindow::saveProject() {
    S_PROJECT->saveProject();
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
    S_PROJECT->close();
    SApplication::inst()->writeSettings("window/tst_struct_header", tst_struct->header()->saveState());
    tst_struct->clear();
    tst_struct->setEnabled(false);
    commandsHistory->setEnabled(false);
    action("close")->setEnabled(false);
    action("undo")->setEnabled(false);
    action("redo")->setEnabled(false);
    action("add_theme")->setEnabled(false);
    action("remove_theme")->setEnabled(false);
    action("add_quest")->setEnabled(false);
}

void MainWindow::tstSctructCurItemChanged(QTreeWidgetItem *cur, QTreeWidgetItem *prev) {
    if(cur->data(0, Qt::UserRole + 1).toString() == "theme") {
        action("remove_theme")->setEnabled(true);
        action("edit_theme")->setEnabled(true);
    } else {
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
    item->setData(0, Qt::UserRole, alias);
    item->setData(0, Qt::UserRole + 1, "theme");
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

void MainWindow::addQuestion() {
    EditQuestionDlg *dlg = new EditQuestionDlg(this);

    if(dlg->exec() == QDialog::Accepted) {
        QString baseName = dlg->questName(S_PROJECT->questionsCount());
        QString label = dlg->questLabel();
        QString type = dlg->questType();
        QStringList q_themes = dlg->selectedThemes();
        S_PROJECT->undoStack()->push(new addQuestionCommand(type, baseName,
                                                            dlg->questConfig(baseName),
                                                            label, q_themes));
        int rc_count = dlg->getResCount();
        for(int i = 0; i < rc_count; i++) {
            QString name;
            QByteArray rc_data;
            dlg->getResource(i, &name, &rc_data);
            S_PROJECT->addData(rc_data, name);
            qDebug() << name << S_PROJECT->regResource(baseName, name);
        }
    }
    SAFE_DELETE(dlg);
}

void MainWindow::questionAdded(QString alias, QString label, QStringList toThemes) {
    int thCount = theme_item->childCount();
    for(int i = 0; i < thCount; i++) {
        QTreeWidgetItem *child = theme_item->child(i);
        QString themeAlias = child->text(1);
        if(toThemes.contains(themeAlias)) {
            QTreeWidgetItem *item = new QTreeWidgetItem;
            item->setText(0, label);
            item->setText(1, alias);
            item->setIcon(0, QIcon(":/quest"));
            item->setToolTip(0, tr("<P><B>Question: </B>%1</P>"
                                   "<B>Alias: </B>%2"
                                   "<P><B>Type: </B>%3</P>")
                             .arg(label, alias)
                             .arg(S_PROJECT->questionTypeLabel(alias)));
            item->setData(0, Qt::UserRole, alias);
            item->setData(0, Qt::UserRole + 1, "question");
            item->setData(0, Qt::UserRole + 2, child->text(1));

            child->addChild(item);
        }
    }
}

void MainWindow::questRemoved(QString qAlias, QStringList fromThemes) {
    int thCount = theme_item->childCount();
    for(int i = 0; i < thCount; i++) {
        QTreeWidgetItem *child = theme_item->child(i);

        if(!fromThemes.contains(child->data(0, Qt::UserRole).toString()))
            continue;

        for(int j = 0; j < child->childCount(); j++) {
            QTreeWidgetItem *questItem = child->child(j);
            if(questItem->data(0, Qt::UserRole).toString() != qAlias)
                continue;
            delete questItem;
        }
    }
}

void MainWindow::openProject() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Opening test..."),
                                                    "", tr("Test files (*.tst)"));

    if(fileName.isEmpty() )
        return;

    setWindowFilePath(fileName);
    commandsHistory->setStack(S_PROJECT->undoStack());
    commandsHistory->setEnabled(true);
    tst_struct->setEnabled(true);
    action("close")->setEnabled(true);
    action("add_theme")->setEnabled(true);
    action("add_quest")->setEnabled(true);

    updateTestStruct();

    S_PROJECT->openProject(fileName);
}

void MainWindow::tstStructContextMenu(QPoint pos) {
    QTreeWidgetItem *itemAt = tst_struct->itemAt(pos);

    if(!itemAt)
        return;

    QMenu *tmpMenu = NULL;
    QString type = itemAt->data(0, Qt::UserRole + 1).toString();
    if(type == "theme")
        tmpMenu = theme_mehu;

    if(tmpMenu) {
        QPoint mapedPos = tst_struct->mapToGlobal(pos);
        tmpMenu->exec(QCursor::pos());
    }
}

void MainWindow::projectModifyed(bool value) {
    action("close")->setEnabled(value);
    action("save")->setEnabled(value);
}

void MainWindow::debugOutput(QString msg) {
    qDebug() << msg;
}
