#ifndef SCOMBOBOX_H
#define SCOMBOBOX_H

#include "s_core_global.h"
#include <QComboBox>

class S_CORESHARED_EXPORT SComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit SComboBox(QAbstractItemModel *item_model, QWidget *parent = 0);

    bool eventFilter(QObject* object, QEvent* event);
    void hidePopup();
    int selectedItemsCount();
    QStringList selectedItems();

private:
    QString str;
    bool skipNextHide;

protected:
    virtual void paintEvent(QPaintEvent *e);
};

#endif // SCOMBOBOX_H
