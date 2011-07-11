#ifndef S_TEXTEDITWIDGET_H
#define S_TEXTEDITWIDGET_H

#include <QTextEdit>

class STextEditWidget : public QTextEdit
{
    Q_OBJECT
public:
    explicit STextEditWidget(QWidget *parent = 0);

protected:
    virtual void insertFromMimeData(const QMimeData * source);
    virtual QVariant loadResource(int type, const QUrl &name);

};

#endif // S_TEXTEDITWIDGET_H
