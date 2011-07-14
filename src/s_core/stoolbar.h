#ifndef STOOLBAR_H
#define STOOLBAR_H

#include <QToolBar>
#include <QDragEnterEvent>

class SToolBar : public QToolBar
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

protected:
    virtual void dropEvent(QDropEvent *event);
    virtual void dragMoveEvent(QDragMoveEvent *event);
    virtual void dragEnterEvent(QDragEnterEvent *event);
};

#endif // STOOLBAR_H
