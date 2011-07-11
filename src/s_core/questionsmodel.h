#ifndef QUESTIONSMODEL_H
#define QUESTIONSMODEL_H

#include <QStandardItemModel>

class QuestionsModel : public QStandardItemModel
{
    Q_OBJECT
public:
    QuestionsModel(QObject *parent = 0);
    ~QuestionsModel();

signals:

public slots:

};

#endif // QUESTIONSMODEL_H
