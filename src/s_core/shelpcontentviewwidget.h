#ifndef SHELPCONTENTVIEWWIDGET_H
#define SHELPCONTENTVIEWWIDGET_H

#include <QtWebKit>
#include "cmainwindow.h"

class CMainWindow;
class QToolBar;
class QComboBox;

class SHelpContentViewWidget : public QWebView
{
    Q_OBJECT
public:
    explicit SHelpContentViewWidget(QWidget *parent = 0);
    void setSource(const QUrl &url);

signals:

public slots:

private:
    QComboBox *links;
};

class HelpView : CMainWindow
{
    Q_OBJECT
public:
    HelpView(bool makeControls, QWidget *parent = 0);

private slots:
    void urlChanged(const QUrl &url);

private:
    QComboBox *address_box;
    SHelpContentViewWidget *helpContent;
};

#endif // SHELPCONTENTVIEWWIDGET_H
