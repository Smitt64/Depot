#ifndef S_TEXTEDITWIDGET_H
#define S_TEXTEDITWIDGET_H

#include "s_core_global.h"
#include <QTextEdit>

class S_CORESHARED_EXPORT STextEditWidget : public QTextEdit
{
    Q_OBJECT
public:
    explicit STextEditWidget(QWidget *parent = 0);

protected:
    virtual void insertFromMimeData(const QMimeData * source);
    virtual QVariant loadResource(int type, const QUrl &name);

};

#endif // S_TEXTEDITWIDGET_H
