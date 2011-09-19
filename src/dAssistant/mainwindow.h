#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QHelpContentWidget>
#include <QHelpIndexWidget>
#include "cmainwindow.h"
#include "shelpcontentviewwidget.h"

class MainWindow : public CMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

signals:

public slots:
    void openHelpPage(const QString &page);

private:
    //QDBusConnection conn;
    QHelpIndexWidget *indexWidget;
    QHelpContentWidget *contentWidget;
};

#endif // MAINWINDOW_H
