#include "questionsmodel.h"
#include <QDebug>
#include <QFile>
#include <QDataStream>
#include <QDomDocument>

QuestionsModel::QuestionsModel(QObject *parent) :
    QStandardItemModel(parent)
{
    setColumnCount(6);
    setHeaderData(0, Qt::Horizontal, tr("Question"));
    setHeaderData(1, Qt::Horizontal, tr("Name"));
    setHeaderData(2, Qt::Horizontal, tr("Question type"));
    setHeaderData(3, Qt::Horizontal, tr("Data"));
}

QuestionsModel::~QuestionsModel() {

}

QDomElement QuestionsModel::questionData(QString name) {
    QModelIndex index = questionIndex(name);
    if(!index.isValid()) {
        error(tr("Can't obtain the data about a question [%1]").arg(name));
        return QDomElement();
    }

    QDomDocument doc;
    doc.setContent(item(index.row(), 3)->text());
    return doc.documentElement();
}

QString QuestionsModel::questionType(QString name) {
    QModelIndex index = questionIndex(name);
    if(!index.isValid()) {
        error(tr("Can't obtain the question type [%1]").arg(name));
        return QString::null;
    }
    return item(index.row(), 2)->text();
}

QModelIndex QuestionsModel::questionIndex(QString name) {
    QList<QStandardItem*> items = findItems(name, Qt::MatchCaseSensitive, 1);

    if(items.count() == 0) {
        return QModelIndex();
    }

    return items[0]->index();
}
