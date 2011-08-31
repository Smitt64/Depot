#include "stoolbar.h"
#include "cmainwindow.h"
#include "sapplication.h"
#include <QDebug>
#include <QPainter>
#include <QStyle>
#include <QStyleOption>

SToolBar::SToolBar(QWidget *parent) :
    QToolBar(parent),
    isUserToolBar(false),
    movedAction(false),
    firstGrab(true)
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
        if(name == "Separator") {
            insertSeparator(at);
            event->accept();
            return;
        }
        insertAction(at, SApplication::inst()->mainWindow()->sactions[name]);
        movedAction = false;
        firstGrab = true;
        update();
        event->accept();
    }
    else {
        event->ignore();
    }
}

void SToolBar::dragMoveEvent(QDragMoveEvent *event) {
    if(event->mimeData()->hasFormat("actions/x-actiondata") && isUserToolBar) {
        event->setDropAction(Qt::MoveAction);
        update();
        event->accept();
    }
    else {
        event->ignore();
    }
}

void SToolBar::dragLeaveEvent(QDragLeaveEvent *event) {
    movedAction = false;
    firstGrab = true;
    update();
    event->accept();
}

void SToolBar::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasFormat("actions/x-actiondata") && isUserToolBar) {
        if(firstGrab) {
            grabed = QPixmap::grabWidget(this);
            firstGrab = false;
        }
        movedAction = true;
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
    if(!movedAction)
        QToolBar::paintEvent(event);
    else {
        QPainter painter(this);
        QPixmap positioner;//(":/insert_at");
        if(orientation() == Qt::Horizontal) {
            positioner.load(":/insert_at");
        } else {
            QPixmap tmp(":/insert_at");
            positioner = QPixmap(tmp.height(), tmp.width());
            QPainter posPaint(&positioner);
            posPaint.rotate(90);
            posPaint.drawPixmap(0, 0, tmp);
        }

        painter.drawPixmap(0, 0, grabed);

        QAction *action = actionAt(mapFromGlobal(QCursor::pos()));
        if(action) {
            QWidget *widget = widgetForAction(action);
            if(orientation() == Qt::Horizontal) {
                painter.drawPixmap(widget->pos().x(), 0, positioner);
            } else {
                painter.drawPixmap(0, widget->pos().y(), positioner);
            }
        } else {
            if(orientation() == Qt::Horizontal) {
                painter.drawPixmap(actions().count() * iconSize().width() +
                                   (iconSize().width() / 2) * actions().count()
                                   + positioner.width(),
                                   0, positioner);
            } else {
                painter.drawPixmap(0, actions().count() * iconSize().height() +
                                   (iconSize().height() / 2) * actions().count()
                                   + positioner.height(), positioner);
            }
        }
    }
}

bool SToolBar::event(QEvent *event) {
    if(event->type() == QEvent::MouseMove) {
    }
    return QToolBar::event(event);
}
