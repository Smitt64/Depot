#ifndef EDITQUESTION_H
#define EDITQUESTION_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QListView>
#include <QFormLayout>
#include <QBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QTabWidget>

class EditQuestionDlg : public QDialog
{
    Q_OBJECT
public:
    EditQuestionDlg(QWidget *parent = 0);
    ~EditQuestionDlg();

private:
    QDialogButtonBox  *buttons;
    QComboBox *quest_type;
    QTabWidget *tabWidget;
    QPushButton *save_changes;
    QPushButton *cancel;
    QFormLayout *form;
    QBoxLayout *layout;
};

#endif // EDITQUESTION_H
