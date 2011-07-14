#include "scustomizedlg.h"
#include "cmainwindow.h"
#include "sapplication.h"
#include <QDebug>
#include <QMdiArea>
#include <QInputDialog>
#include <QPainter>

#define MAIN(x) (((CMainWindow*)(x)))

SActionListWidget::SActionListWidget(QWidget *parent) :
    QListWidget(parent)
{
}

SActionListWidget::~SActionListWidget()
{

}

void SActionListWidget::setActionList(QList<QAction*> actions) {
    s_actions = actions;
    for(int i = 0; i < actions.count(); i++) {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(actions[i]->text());
        item->setData(Qt::UserRole, actions[i]->data());
        item->setIcon(actions[i]->icon());
        item->setToolTip(actions[i]->statusTip());

        addItem(item);
    }

    sortItems();
}

void SActionListWidget::mousePressEvent(QMouseEvent *event) {
    QListWidgetItem *item = static_cast<QListWidgetItem*>(itemAt(event->pos()));

    if(!item)
        return;

    QByteArray data = item->data(Qt::UserRole).toByteArray();

    QMimeData *mimeData = new QMimeData;
    mimeData->setData("actions/x-actiondata", data);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    QPixmap map = item->icon().pixmap(64, 64, QIcon::Normal, QIcon::On);
    drag->setPixmap(map);
    drag->setHotSpot(QPoint(map.width() / 2, map.height() / 2));

    if(drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction) {

    }
}

void SActionListWidget::dragEnterEvent(QDragEnterEvent *event) {
    if(event->mimeData()->hasFormat("application/x-dnditemdata")) {

    }
}

////////////////////////////////////////////////////////////////////////

SCustomizeDlg::SCustomizeDlg(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Customizing"));
    tab = new QTabWidget;
    tool_list = new QListWidget;
    buttons = new QDialogButtonBox;
    actions = new QDialogButtonBox;
    actions->setOrientation(Qt::Vertical);

    create_toolbar = actions->addButton(tr("New"), QDialogButtonBox::ActionRole);
    create_toolbar->setWhatsThis(tr("Creating new user toolbar."));
    remove_toolbar = actions->addButton(tr("Remove"), QDialogButtonBox::ActionRole);
    remove_toolbar->setEnabled(false);
    remove_toolbar->setWhatsThis(tr("Removing toolbar."));

    buttons->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    layout = new QVBoxLayout;
    layout->addWidget(tab);
    layout->addWidget(buttons);

    setLayout(layout);

    //Tab1 - Toolbars
    QWidget *page1 = new QWidget;
    tab1_layout = new QHBoxLayout;
    tab1_layout->addWidget(tool_list);
    tab1_layout->addWidget(actions);
    page1->setLayout(tab1_layout);

    //Tab2 - Commands
    QWidget *page2 = new QWidget;
    tab2_layout = new QVBoxLayout;
    wnd = new CMainWindow;
    test_bar = wnd->addToolBar("test", "test");
    test_bar->setActions(MAIN(parent)->actionList());
    action_list = new SActionListWidget;
    action_list->setActionList(MAIN(parent)->actionList());

    tab2_layout->addWidget(action_list);
    tab2_layout->addWidget(wnd);
    page2->setLayout(tab2_layout);

    tab->addTab(page1, tr("Toolbars"));
    tab->addTab(page2, tr("Commands"));

    QStringList keys = MAIN(parent)->toolBars.keys();
    for(int i = 0; i < keys.count(); i++) {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(MAIN(parent)->toolBars[keys[i]]->windowTitle());
        item->setData(Qt::UserRole, keys[i]);
        item->setCheckState((MAIN(parent)->toolBars[keys[i]]->isVisible() ? Qt::Checked : Qt::Unchecked));

        tool_list->addItem(item);
    }

    connect(create_toolbar, SIGNAL(clicked()), this, SLOT(addUserToolBar()));
    connect(tool_list, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onClickActionItem(QListWidgetItem*)));
}

SCustomizeDlg::~SCustomizeDlg() {
    SAFE_DELETE(layout);
}

void SCustomizeDlg::addUserToolBar() {
    bool ok = false;
    QString text = QInputDialog::getText(this, tr("New toolbar"),
                                         tr("Enter neme for new toolbar:"),
                                         QLineEdit::Normal, "", &ok);

    if(!ok || text.isEmpty())
        return;
}

void SCustomizeDlg::onClickActionItem(QListWidgetItem *item) {
    if(MAIN(parent())->toolBars[item->data(Qt::UserRole).toString()]->isUser() == false) {
        remove_toolbar->setEnabled(false);
        tab->widget(1)->setEnabled(false);
    }
    else {
        remove_toolbar->setEnabled(true);
        tab->widget(1)->setEnabled(true);
    }
}
