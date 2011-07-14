#include "stoolbar.h"
#include "cmainwindow.h"
#include <QDebug>

SToolBar::SToolBar(QWidget *parent) :
    QToolBar(parent),
    isUserToolBar(false)
{
    setAcceptDrops(true);
    setMinimumSize(100, 16);
}

SToolBar::SToolBar(bool user_bar, QWidget *parent) :
    QToolBar(parent),
    isUserToolBar(user_bar)
{
    setMinimumSize(100, 16);
}

SToolBar::SToolBar(QList<QAction*> actions, QWidget *parent) :
    QToolBar(parent),
    s_actions(actions),
    isUserToolBar(false)
{
    setMinimumSize(100, 16);
}

void SToolBar::setActions(QList<QAction*> actions) {
    s_actions = actions;
}

void SToolBar::dropEvent(QDropEvent *event) {
    if (event->mimeData()->hasFormat("actions/x-actiondata")) {
        QByteArray itemData = event->mimeData()->data("actions/x-actiondata");
        QString name = itemData;

        for(int i = 0; i < s_actions.count(); i++) {
            if(s_actions[i]->data().toString() == name) {
                addAction(s_actions[i]);
                event->accept();
            }
        }

        event->accept();
    }
    else {
        event->ignore();
    }
}

void SToolBar::dragMoveEvent(QDragMoveEvent *event) {
    if(event->mimeData()->hasFormat("actions/x-actiondata")) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    else {
        event->ignore();
    }
}

void SToolBar::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasFormat("actions/x-actiondata"))
        event->accept();
    else
        event->ignore();
}

bool SToolBar::isUser() {
    return isUserToolBar;
}

void SToolBar::setUser(bool value) {
    isUserToolBar = value;
}
