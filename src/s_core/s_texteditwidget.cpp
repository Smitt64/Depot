#include "s_texteditwidget.h"
#include "sproject.h"
#include <QMimeData>
#include <QBuffer>
#include <QUrl>

STextEditWidget::STextEditWidget(QWidget *parent) :
    QTextEdit(parent)
{
}

void STextEditWidget::insertFromMimeData(const QMimeData * source) {
    if(source->hasImage()) {
        QImage image = qvariant_cast<QImage>(source->imageData());
        QByteArray ba;
        QBuffer buffer(&ba);
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer, "PNG");
        buffer.close();

        int counter = *S_PROJECT->resourceCounter();
        QString name = QString("resources\\mimeres%1.png")
                .arg(counter);

        counter ++;
        S_PROJECT->addData(ba, name);

        QTextImageFormat imageFormat;
        imageFormat.setName(name);
        textCursor().insertImage(image, name);
    }
    else
        QTextEdit::insertFromMimeData(source);
}

QVariant STextEditWidget::loadResource(int type, const QUrl &name) {
    if(type == QTextDocument::ImageResource) {
        QPixmap data;
        data.loadFromData(S_PROJECT->readData(name.path()));
    }

    return QTextEdit::loadResource(type, name);
}
