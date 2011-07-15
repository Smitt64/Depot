#include "stoolbar.h"
#include "cmainwindow.h"
#include "sapplication.h"
#include <QDebug>
#include <QPainter>
#include <QStyle>
#include <QStyleOption>

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
    if (event->mimeData()->hasFormat("actions/x-actiondata") && isUserToolBar) {
        QByteArray itemData = event->mimeData()->data("actions/x-actiondata");
        QString name = itemData;

        QAction *at = actionAt(event->pos());
        insertAction(at, SApplication::inst()->mainWindow()->sactions[name]);
        event->accept();
    }
    else {
        event->ignore();
    }
}

void SToolBar::dragMoveEvent(QDragMoveEvent *event) {
    if(event->mimeData()->hasFormat("actions/x-actiondata") && isUserToolBar) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    else {
        event->ignore();
    }
}

void SToolBar::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasFormat("actions/x-actiondata") && isUserToolBar) {
        event->accept();
    }
    else
        event->ignore();
}

bool SToolBar::isUser() {
    return isUserToolBar;
}

void SToolBar::setUser(bool value) {
    isUserToolBar = value;
}

void SToolBar::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QToolBar::paintEvent(event);
}

bool SToolBar::event(QEvent *event) {
    if(event->type() == QEvent::MouseMove) {
    }
    return QToolBar::event(event);
}
