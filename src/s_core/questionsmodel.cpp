#include "questionsmodel.h"
#include <QDebug>
#include <QFile>
#include <QDataStream>

QuestionsModel::QuestionsModel(QObject *parent) :
    QStandardItemModel(parent)
{
    setColumnCount(6);
    setHeaderData(0, Qt::Horizontal, tr("Question"));
    setHeaderData(1, Qt::Horizontal, tr("Name"));
    setHeaderData(2, Qt::Horizontal, tr("Question type"));
    setHeaderData(3, Qt::Horizontal, tr("Source file"));
    setHeaderData(4, Qt::Horizontal, tr("Answer"));
    setHeaderData(5, Qt::Horizontal, tr("Data"));
}

QuestionsModel::~QuestionsModel() {

}
