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

    /*QStandardItem *it = new QStandardItem;
    it->setColumnCount(2);
    QList<QStandardItem*> list;

    QStandardItem *item1 = new QStandardItem;
    item1->setText("text 1");
    it->appendRow(item1);
    QStandardItem *item2 = new QStandardItem;
    item2->setText("text 2");
    it->appendRow(item2);

    appendRow(it);
    setData(index(0, 1), "dfdsg");*/
}

QuestionsModel::~QuestionsModel() {

}
