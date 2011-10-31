#include "defaultshowpanel.h"
#include "../showpanelstyle.h"
#include "showpanelcomponents.h"
#include "../sapplication.h"
#include <QPainter>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QPainterPath>
#include <QStyleOptionButton>
#include <QLinearGradient>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QColorDialog>

///////////////////////////////////////////////////////

ViewWidget::ViewWidget(QColor themeColor, QWidget *parent) :
    QWidget(parent),
    enableAnimation(true),
    panelColor("gainsboro")//gainsboro
{
    setObjectName("DefaultShowPanel");
    SApplication::inst()->setResourcePackage("./DefShowPanel.brc");

    setColor(panelColor);
    startButton = new ShowPanelPushButton(panelColor, this);
    startButton->setText("Test color");
    startButton->setGeometry(QRect(10, 10, 75, 25));

    groupCombo = new ShowPanelComboBox(panelColor, this);
    groupCombo->setGeometry(QRect(10, 50, 150, 25));
    groupCombo->addItems(S_PROJECT->db_groups());

    connect(this, SIGNAL(colorChanged(QColor)), startButton, SLOT(setColor(QColor)));
    connect(this, SIGNAL(colorChanged(QColor)), groupCombo, SLOT(setColor(QColor)));

    connect(startButton, SIGNAL(clicked()), this, SLOT(onPushButton()));
}

void ViewWidget::setColor(QColor color) {
    panelColor = color;
    makeResource("back.png", "${back}", panelColor);
    setStyleSheet(makeStyleSheet("defaultshowpanel.txt"));

    emit colorChanged(color);
}

void ViewWidget::onPushButton() {
    setColor(QColorDialog::getColor());
}

void ViewWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    QStyleOption opt;
    opt.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void ViewWidget::enterStartScreen() {
    if(!enableAnimation) {

    }
}

///////////////////////////////////////////////////////

DefaultShowPanel::DefaultShowPanel(QWidget *parent) :
    ShowPanelInterface(parent),
    colortheme(QColor("gainsboro"))
{
    scene()->setBackgroundBrush(colortheme);
    setSceneRect(0, 0, width(), height());

    ViewWidget *viewWidget= new ViewWidget(colortheme);
    viewWidget->setGeometry(20, 20, 320, 240);
    viewWidget->setStyle(new QPlastiqueStyle);
    QGraphicsProxyWidget *proxyWidget = scene()->addWidget(viewWidget, Qt::Window);

    connect(this, SIGNAL(colorChanged(QColor)), viewWidget, SLOT(setColor(QColor)));
}

void DefaultShowPanel::setColor(QColor color) {
    colortheme = color;
}

void DefaultShowPanel::resizeEvent(QResizeEvent *event) {
    setSceneRect(0, 0, event->size().width(), event->size().height());
}
