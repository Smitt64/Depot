#ifndef STOOLBAR_H
#define STOOLBAR_H

#include <QToolBar>
#include <QDragEnterEvent>

class SToolBar : public QToolBar
{
    Q_OBJECT
public:
    SToolBar(QWidget *parent = 0);
    SToolBar(QList<QAction*> actions, QWidget *parent = 0);
    void setActions(QList<QAction*> actions);

private:
    QList<QAction*> s_actions;

protected:
    virtual void dropEvent(QDropEvent *event);
    virtual void dragMoveEvent(QDragMoveEvent *event);
    virtual void dragEnterEvent(QDragEnterEvent *event);
};

#endif // STOOLBAR_H
