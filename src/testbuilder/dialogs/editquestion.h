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
#include <QStandardItemModel>
#include "interfaces/questeditor_interface.hpp"

class EditQuestionDlg : public QDialog
{
    Q_OBJECT
public:
    EditQuestionDlg(QWidget *parent = 0);
    ~EditQuestionDlg();

    QString questName();
    QString questLabel();
    QString questType();
    QByteArray questConfig(QString questionName);

private slots:
    void questTypeChanged(int index);
    void addResourceFromRedactor(QString name, QByteArray data);
    void validQestion(bool isValid);

private:
    QDialogButtonBox  *buttons;
    QComboBox *quest_type, *groups_box;
    QTabWidget *tabWidget;
    QPushButton *save_changes;
    QPushButton *cancel;
    QFormLayout *form;
    QBoxLayout *layout;
    QuestEditorInterface *editor;
};

#endif // EDITQUESTION_H
