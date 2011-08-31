#include "questionsmodel.h"
#include <QDebug>
#include <QFile>
#include <QDataStream>
#include <QDomDocument>

QuestionsModel::QuestionsModel(QObject *parent) :
    QStandardItemModel(parent)
{
    setColumnCount(5);
    setHeaderData(0, Qt::Horizontal, tr("Question"));
    setHeaderData(1, Qt::Horizontal, tr("Name"));
    setHeaderData(2, Qt::Horizontal, tr("Question type"));
    setHeaderData(3, Qt::Horizontal, tr("Settings"));
    setHeaderData(4, Qt::Horizontal, tr("Data"));
}

QuestionsModel::~QuestionsModel() {

}

QByteArray QuestionsModel::questionData(QString name) {
    QModelIndex index = questionIndex(name);
    if(!index.isValid()) {
        error(tr("Can't obtain the data about a question [%1]").arg(name));
        return QByteArray();
    }

    return item(index.row(), 4)->data().toByteArray();
}

QDomElement QuestionsModel::questionSettings(QString name) {
    QModelIndex index = questionIndex(name);
    if(!index.isValid()) {
        error(tr("Can't obtain question settings [%1]").arg(name));
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
    int rows = rowCount();
    for(int i = 0; i < rows; i++) {
        if(data(index(i, 1)).toString() == name)
            return index(i, 1);
    }

    return QModelIndex();
}

bool QuestionsModel::hasQuestion(QString name) {
    return questionIndex(name).isValid();
}

void QuestionsModel::addQuestion(QString type, QString name, QByteArray settings, QString label) {
    int rows = rowCount();
    insertRow(rows);
    setData(index(rows, 0), label);
    setData(index(rows, 1), name);
    setData(index(rows, 2), type);
    setData(index(rows, 3), settings);
    setData(index(rows, 4), QByteArray());
}
