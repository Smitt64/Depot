#ifndef SHOWPANELCOMPONENTS_H
#define SHOWPANELCOMPONENTS_H

#include <QtCore>
#include <QtGui>

class ShowPanelPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit ShowPanelPushButton(QColor color, QWidget *parent = 0);
    ~ShowPanelPushButton();

public slots:
    void setColor(QColor color);

private:
    QColor buttonColor;
};

class ShowPanelGroupBox : public QGroupBox
{
    Q_OBJECT
public:
    explicit ShowPanelGroupBox(QColor color, QWidget *parent = 0);
    ~ShowPanelGroupBox();
};

class ShowPanelComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit ShowPanelComboBox(QColor color, QWidget *parent = 0);
    ~ShowPanelComboBox();

public slots:
    void setColor(QColor color);

private:
    QColor comboColor;
};

void makeResource(QString image_name, QString replaced_name, QColor colorMask);
void registerColor(QColor color, QString alias);
void freeResources();
QString makeStyleSheet(QString base_name);

#endif // SHOWPANELCOMPONENTS_H
