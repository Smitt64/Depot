#ifndef SCUSTOMIZEDLG_H
#define SCUSTOMIZEDLG_H

#include <QDialog>
#include <QTabWidget>
#include <QListWidget>
#include <QFormLayout>
#include <QBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QStylePainter>
#include "stoolbar.h"

class SActionListWidget : public QListWidget
{
    Q_OBJECT
public:
    SActionListWidget(QMainWindow *wnd, QWidget *parent = 0);
    ~SActionListWidget();

protected:
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);

private:
    QMainWindow *window;
};


class SCustomizeDlg : public QDialog
{
    Q_OBJECT
public:
    explicit SCustomizeDlg(QMainWindow *wnd, QWidget *parent = 0);
    ~SCustomizeDlg();

public slots:
    void show();

private slots:
    void addUserToolBar();
    void onClickActionItem(QListWidgetItem *item);

private:
    QDialogButtonBox *buttons, *actions;
    QTabWidget *tab;
    QBoxLayout *layout, *tab1_layout, *tab2_layout;
    QListWidget *tool_list;
    SActionListWidget *action_list;
    QPushButton *create_toolbar, *remove_toolbar;
    QMainWindow *window;
};

#endif // SCUSTOMIZEDLG_H
