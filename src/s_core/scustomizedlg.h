#ifndef SCUSTOMIZEDLG_H
#define SCUSTOMIZEDLG_H

#include "s_core_global.h"
#include <QDialog>
#include <QTabWidget>
#include <QListWidget>
#include <QFormLayout>
#include <QBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QStylePainter>
#include <QItemDelegate>
#include <QStyledItemDelegate>
#include <QAbstractItemModel>
#include "stoolbar.h"

class S_CORESHARED_EXPORT SActionListItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    SActionListItemDelegate(QObject *parent = 0);
    virtual void paint(QPainter *painter, const QStyleOptionViewItem & option,
                       const QModelIndex &index) const;
};

class SActionListWidget : public QListView
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
    void removeUserToolBar();
    void clearToolBar();
    void onClickActionItem(QListWidgetItem *item);

private:
    QDialogButtonBox *buttons, *actions;
    QTabWidget *tab;
    QBoxLayout *layout, *tab1_layout, *tab2_layout;
    QListWidget *tool_list;
    SActionListWidget *action_list;
    QPushButton *create_toolbar, *remove_toolbar, *clear_toolbar;
    QMainWindow *window;
};

#endif // SCUSTOMIZEDLG_H
