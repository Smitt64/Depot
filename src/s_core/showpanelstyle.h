#ifndef SHOWPANELSTYLE_H
#define SHOWPANELSTYLE_H

#include "s_core_global.h"
#include <QPlastiqueStyle>

class S_CORESHARED_EXPORT ShowPanelStyle : public QPlastiqueStyle
{
    Q_OBJECT
public:
    ShowPanelStyle();

    void drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                       QPainter *painter, const QWidget *widget = 0) const;
};

#endif // SHOWPANELSTYLE_H
