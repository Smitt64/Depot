#ifndef SCUSTOMIZEDLG_H
#define SCUSTOMIZEDLG_H

#include <QDialog>
#include <QTabWidget>
#include <QListWidget>
#include <QFormLayout>
#include <QBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include "cmainwindow.h"
#include "stoolbar.h"

class SActionListWidget : public QListWidget
{
    Q_OBJECT
public:
    SActionListWidget(QWidget *parent = 0);
    ~SActionListWidget();

    void setActionList(QList<QAction*> actions);

private:
    QList<QAction*> s_actions;

protected:
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
};

class SCustomizeDlg : public QDialog
{
    Q_OBJECT
public:
    explicit SCustomizeDlg(QWidget *parent = 0);
    ~SCustomizeDlg();

private slots:
    void addUserToolBar();

private:
    QDialogButtonBox *buttons, *actions;
    QTabWidget *tab;
    QBoxLayout *layout, *tab1_layout, *tab2_layout;
    QListWidget *tool_list;
    SActionListWidget *action_list;
    QPushButton *create_toolbar, *remove_toolbar;
    CMainWindow *wnd;
    SToolBar *test_bar;
};

#endif // SCUSTOMIZEDLG_H
