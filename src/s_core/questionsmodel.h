#ifndef QUESTIONSMODEL_H
#define QUESTIONSMODEL_H

#include <QStandardItemModel>
#include <QDomElement>

class QuestionsModel : public QStandardItemModel
{
    Q_OBJECT
public:
    QuestionsModel(QObject *parent = 0);
    ~QuestionsModel();

    QDomElement questionData(QString name);
    QModelIndex questionIndex(QString name);
    QString questionType(QString name);

signals:
    void error(QString msg);
    void warning(QString msg);

public slots:

};

#endif // QUESTIONSMODEL_H
