#ifndef SHOWPANEL_INTERFACE_H
#define SHOWPANEL_INTERFACE_H

#include <QWidget>
#include <QtDeclarative>

class ShowPanelInterface : public QGraphicsView {
    Q_OBJECT
public:
    ShowPanelInterface(QWidget *parent = 0) :
        QGraphicsView(new QGraphicsScene, parent)
    {

    }
};

#endif // SHOWPANEL_INTERFACE_H
