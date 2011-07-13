#include "scustomizedlg.h"
#include "cmainwindow.h"

#define MAIN(x) (((CMainWindow*)(x)))

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
}
