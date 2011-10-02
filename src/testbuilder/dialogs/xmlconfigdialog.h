#ifndef XMLCONFIGDLG_H
#define XMLCONFIGDLG_H

#include <QtGui>

class XmlConfigDialog : public QDialog
{
    Q_OBJECT
public:
    XmlConfigDialog(QWidget *parent = 0);

private slots:
    void evaluateQuery();

private:
    QTextEdit *configView, *queryEdit, *queryResult;
    QPushButton *execButton;
};

#endif // XMLCONFIGDLG_H
