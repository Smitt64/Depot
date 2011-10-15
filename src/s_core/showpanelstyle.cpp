#include "showpanelstyle.h"
#include <QPainter>
#include <QStyleOption>
#include <QDebug>
#include <QStylePlugin>

ShowPanelStyle::ShowPanelStyle() :
    QPlastiqueStyle()
{
}

void ShowPanelStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                   QPainter *painter, const QWidget *widget) const {
    qDebug() << "drawPrimitive";
    //if(element == QStyle::PE_Widget) {
        painter->fillRect(option->rect, Qt::red);
    //} else {
        QPlastiqueStyle::drawPrimitive(element, option, painter, widget);
    //}
}

/*class ShowPanelStylePlugin : public QStylePlugin
{
public:
    QStringList keys() const;
    QStyle* create(const QString& key);
};

QStringList ShowPanelStylePlugin::keys() const
{
    return QStringList() << "ShowPanelStyle";
}

QStyle* ShowPanelStylePlugin::create(const QString& key)
{
    if (key.toLower() == QLatin1String("showpanelstyle"))
        return new ShowPanelStyle();
    return NULL;
}

Q_EXPORT_PLUGIN2(showpanelstyle, ShowPanelStylePlugin)*/
