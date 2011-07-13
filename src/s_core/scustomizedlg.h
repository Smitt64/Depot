#ifndef SCUSTOMIZEDLG_H
#define SCUSTOMIZEDLG_H

#include <QDialog>
#include <QTabWidget>
#include <QListWidget>
#include <QFormLayout>
#include <QBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>

class SCustomizeDlg : public QDialog
{
    Q_OBJECT
public:
    explicit SCustomizeDlg(QWidget *parent = 0);

private:
    QDialogButtonBox *buttons, *actions;
    QTabWidget *tab;
    QBoxLayout *layout, *tab1_layout;
    QListWidget *tool_list;
    QPushButton *create_toolbar, *remove_toolbar;
};

#endif // SCUSTOMIZEDLG_H
