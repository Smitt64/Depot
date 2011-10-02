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

    QString questName(int counter);
    QString questLabel();
    QString questType();
    QStringList selectedThemes();
    QByteArray questConfig(QString questionName);

    int getResCount();
    void getResource(int id, QString *name, QByteArray *res_data);

private slots:
    void questTypeChanged(int index);
    void addResourceFromRedactor(QString name, QByteArray data);
    void validQestion(bool isValid);
#ifndef S_OS_MEEGO
    void showHelp();
#endif

private:
    QDialogButtonBox  *buttons;
    QComboBox *quest_type, *groups_box;
    QTabWidget *tabWidget;
    QPushButton *save_changes;
    QPushButton *cancel, *help;
    QFormLayout *form;
    QBoxLayout *layout;
    QuestEditorInterface *editor;
};

#endif // EDITQUESTION_H
