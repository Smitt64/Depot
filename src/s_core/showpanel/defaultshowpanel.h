#ifndef DEFAULTSHOWPANEL_H
#define DEFAULTSHOWPANEL_H

#include "../s_core_global.h"
#include "../interfaces/showpanel_interface.h"
#include "../showpanelstyle.h"
#include <QtGui>

class S_CORESHARED_EXPORT ViewWidget : public QWidget
{
    Q_OBJECT
public:
    ViewWidget(QColor themeColor, QWidget *parent = 0);

public slots:
    void setColor(QColor color);
    void onPushButton();

    void enterStartScreen();

signals:
    void colorChanged(QColor color);

private:
    QColor panelColor;
    bool enableAnimation;
    QPushButton *startButton;
    QComboBox *groupCombo;

protected:
    void paintEvent(QPaintEvent *);
};

class S_CORESHARED_EXPORT DefaultShowPanel : public ShowPanelInterface
{
    Q_OBJECT
public:
    explicit DefaultShowPanel(QWidget *parent = 0);

signals:
    void colorChanged(QColor color);

public slots:
    void setColor(QColor color);

private:
    QColor colortheme;

protected:
    void resizeEvent(QResizeEvent *event);

};

#endif // DEFAULTSHOWPANEL_H
