#ifndef QUESTIONSSPREADSHEET_H
#define QUESTIONSSPREADSHEET_H

#include <QDialog>
#include <QTableWidget>
#include <QItemDelegate>

class SpreadSheetQuetTypeDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    SpreadSheetQuetTypeDelegate(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData (QWidget *editor, QAbstractItemModel *model,
                       const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
};

////////////////////////////////////////////////////////////////////////////////

class QuestionsSpreadSheet : public QDialog
{
    Q_OBJECT
public:
    explicit QuestionsSpreadSheet(QWidget *parent = 0);

signals:

public slots:

private:
    QTableWidget *spreadsheet;

};

#endif // QUESTIONSSPREADSHEET_H
