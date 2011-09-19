#include "mainwindow.h"
#include "sapplication.h"
//#include "assistantadaptor.h"

MainWindow::MainWindow(QWidget *parent) :
    CMainWindow(parent)
{
    QCoreApplication::setApplicationName("DepotAssistant");
    indexWidget = SApplication::inst()->assistantEngine()->indexWidget();
    contentWidget = SApplication::inst()->assistantEngine()->contentWidget();

    QDockWidget *indexDoc = addDockPanel(tr("Index"), "indexDoc");
    QDockWidget *contentDoc = addDockPanel(tr("Content"), "contentDoc");
    indexDoc->setWidget(indexWidget);
    contentDoc->setWidget(contentWidget);

    HelpView *w = new HelpView(false);
    setCentralWidget((QWidget*)w);

    restore();
}

void MainWindow::openHelpPage(const QString &page) {
}
