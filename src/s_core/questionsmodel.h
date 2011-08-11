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

    QByteArray questionData(QString name);
    QDomElement questionSettings(QString name);
    QModelIndex questionIndex(QString name);
    QString questionType(QString name);

    bool hasQuestion(QString name);

    void addQuestion(QString type, QString name, QByteArray settings, QString label);

signals:
    void error(QString msg);
    void warning(QString msg);

public slots:

};

#endif // QUESTIONSMODEL_H
