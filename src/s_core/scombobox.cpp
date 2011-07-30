#include "scombobox.h"
#include <QtGui>
#include <QDebug>

SComboBox::SComboBox(QAbstractItemModel *item_model, QWidget *parent) :
    QComboBox(parent),
    skipNextHide(false),
    str("")
{
    setModel(item_model);
    setView(new QListView(this));
    view()->viewport()->installEventFilter(this);
}

void SComboBox::hidePopup() {
    if (skipNextHide)
        skipNextHide = false;
    else
        QComboBox::hidePopup();
}

bool SComboBox::eventFilter(QObject* object, QEvent* event) {
    if (event->type() == QEvent::MouseButtonPress && object == view()->viewport()) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        QModelIndex index = view()->indexAt(mouseEvent->pos());
        int checkState = model()->data(index, Qt::CheckStateRole).toInt();
        model()->setData(index, (checkState == Qt::Checked ? Qt::Unchecked : Qt::Checked), Qt::CheckStateRole);
        str = "";
        for(int i = 0; i < model()->rowCount(); i++) {
            if(model()->data(model()->index(i, 0), Qt::CheckStateRole).toInt() == Qt::Checked)
                str += model()->data(model()->index(i, 0)).toString() + ";";
        }
            skipNextHide = true;
    }
    return false;
}

void SComboBox::paintEvent(QPaintEvent *e) {
    QPainter painter(this);
    QStyleOptionComboBox option;
    option.initFrom(this);
    option.editable = false;
    option.currentText = str;
    option.frame = true;

    style()->drawComplexControl(QStyle::CC_ComboBox, &option, &painter, this);
    style()->drawControl(QStyle::CE_ComboBoxLabel, &option, &painter, this);
}
