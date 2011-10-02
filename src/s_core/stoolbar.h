#ifndef STOOLBAR_H
#define STOOLBAR_H

#include "s_core_global.h"
#include <QToolBar>
#include <QDragEnterEvent>

class S_CORESHARED_EXPORT SToolBar : public QToolBar
{
    Q_OBJECT
    Q_PROPERTY(bool isUserBar READ isUser WRITE setUser)
public:
    SToolBar(QWidget *parent = 0);
    SToolBar(bool user_bar, QWidget *parent = 0);
    SToolBar(QList<QAction*> actions, QWidget *parent = 0);

    void setActions(QList<QAction*> actions);
    bool isUser();
    void setUser(bool value);

private:
    QList<QAction*> s_actions;
    bool isUserToolBar;
    bool movedAction;
    bool firstGrab;
    QPixmap grabed;

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void dropEvent(QDropEvent *event);
    virtual void dragMoveEvent(QDragMoveEvent *event);
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dragLeaveEvent(QDragLeaveEvent *event);
    virtual bool event(QEvent *event);
};

#endif // STOOLBAR_H
