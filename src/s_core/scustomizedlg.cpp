#include "scustomizedlg.h"
#include "cmainwindow.h"
#include "sapplication.h"
#include <QDebug>
#include <QMdiArea>
#include <QInputDialog>
#include <QPainter>

#define MAIN(x)(((CMainWindow*)(x)))

SActionListWidget::SActionListWidget(QMainWindow *wnd, QWidget *parent) :
    QListWidget(parent),
    window(wnd)
{
    QStringList keys = MAIN(window)->sactions.keys();
    for(int i = 0; i < keys.count(); i++) {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(MAIN(window)->sactions[keys[i]]->text());
        item->setData(Qt::UserRole, MAIN(window)->sactions[keys[i]]->data());
        item->setIcon(MAIN(window)->sactions[keys[i]]->icon());
        item->setToolTip(MAIN(window)->sactions[keys[i]]->statusTip());

        addItem(item);
    }

    QListWidgetItem *item = new QListWidgetItem;
    item->setText(tr("Separator"));
    item->setBackground(QBrush(Qt::lightGray, Qt::Dense6Pattern));
    item->setData(Qt::UserRole, "Separator");
    addItem(item);
}

SActionListWidget::~SActionListWidget()
{

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

    QPixmap icon(32, 32);
    icon.fill(Qt::transparent);
    QPainter painter;
    painter.begin(&icon);
    if(data != "Separator")
        painter.drawPixmap(0, 0, 16, 16, map);
    else
        painter.drawPixmap(0, 0, 16, 16, QPixmap(":/separator"));
    painter.end();

    drag->setPixmap(icon);
    drag->setHotSpot(QPoint(icon.width() / 2, icon.height() / 2));

    if(drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction) {

    }
}

void SActionListWidget::dragEnterEvent(QDragEnterEvent *event) {
    if(event->mimeData()->hasFormat("application/x-dnditemdata")) {

    }
}

////////////////////////////////////////////////////////////////////////

SCustomizeDlg::SCustomizeDlg(QMainWindow *wnd, QWidget *parent) :
    QDialog(parent),
    window(wnd)
{
    setWindowTitle(tr("Customizing"));
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
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
    clear_toolbar = actions->addButton(tr("Clear"), QDialogButtonBox::ActionRole);
    clear_toolbar->setEnabled(false);
    clear_toolbar->setWhatsThis(tr("Removing all actions from toolbar."));

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
    action_list = new SActionListWidget(window);

    tab2_layout->addWidget(action_list);
    page2->setLayout(tab2_layout);

    tab->addTab(page1, tr("Toolbars"));
    tab->addTab(page2, tr("Commands"));

    connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));
    connect(create_toolbar, SIGNAL(clicked()), this, SLOT(addUserToolBar()));
    connect(remove_toolbar, SIGNAL(clicked()), this, SLOT(removeUserToolBar()));
    connect(clear_toolbar, SIGNAL(clicked()), this, SLOT(clearToolBar()));
    connect(tool_list, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onClickActionItem(QListWidgetItem*)));
}

SCustomizeDlg::~SCustomizeDlg() {
    SAFE_DELETE(layout);
}

void SCustomizeDlg::show() {
    tool_list->clear();
    QStringList keys = MAIN(window)->toolBars.keys();
    for(int i = 0; i < keys.count(); i++) {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(MAIN(window)->toolBars[keys[i]]->windowTitle());
        item->setData(Qt::UserRole, keys[i]);
        item->setCheckState((MAIN(window)->toolBars[keys[i]]->isVisible() ? Qt::Checked : Qt::Unchecked));

        tool_list->addItem(item);
    }

    QDialog::show();
}

void SCustomizeDlg::addUserToolBar() {
    bool ok = false;
    QString text = QInputDialog::getText(this, tr("New toolbar"),
                                         tr("Enter neme for new toolbar:"),
                                         QLineEdit::Normal, "", &ok);

    if(!ok || text.isEmpty())
        return;

    QString name = QString("user_bar_%1").arg(MAIN(window)->toolBars.count());
    QListWidgetItem *item = new QListWidgetItem;
    item->setText(text);
    item->setCheckState(Qt::Checked);
    item->setData(Qt::UserRole, name);

    tool_list->addItem(item);

    SToolBar *bar = MAIN(window)->addToolBar(text, name);
    bar->setUser(true);
}

void SCustomizeDlg::onClickActionItem(QListWidgetItem *item) {
    if(!item)
        return;

    SToolBar *bar = MAIN(window)->toolBars[item->data(Qt::UserRole).toString()];
    bar->setVisible((item->checkState() == Qt::Checked ? true : false));

    if(bar->isUser()) {
        remove_toolbar->setEnabled(true);
        clear_toolbar->setEnabled(true);
    }else {
        remove_toolbar->setEnabled(false);
        clear_toolbar->setEnabled(false);
    }
}

void SCustomizeDlg::removeUserToolBar() {
    QString name = tool_list->currentItem()->data(Qt::UserRole).toString();
    MAIN(window)->removeToolBar((QToolBar*)MAIN(window)->toolBars[name]);
    MAIN(window)->toolBars.remove(name);
    delete tool_list->currentItem();

    onClickActionItem(tool_list->currentItem());
}

void SCustomizeDlg::clearToolBar() {
    QString name = tool_list->currentItem()->data(Qt::UserRole).toString();
    MAIN(window)->toolBars[name]->clear();
}
