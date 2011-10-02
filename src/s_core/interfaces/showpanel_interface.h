#ifndef SHOWPANEL_INTERFACE_H
#define SHOWPANEL_INTERFACE_H

#include <QWidget>

class ShowPanelInterface : public QWidget {
    Q_OBJECT
public:
    ShowPanelInterface(QWidget *parent = 0) :
        QWidget(parent)
    {

    }
};

#endif // SHOWPANEL_INTERFACE_H
