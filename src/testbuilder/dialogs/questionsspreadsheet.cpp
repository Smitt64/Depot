#include "questionsspreadsheet.h"
#include "sapplication.h"
#include "sproject.h"
#include "cmainwindow.h"

SpreadSheetQuetTypeDelegate::SpreadSheetQuetTypeDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

QWidget *SpreadSheetQuetTypeDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                      const QModelIndex &index) const
{
    int row = index.row();
    if(row != 0) {
        QComboBox *combo = new QComboBox(parent);
        combo->setModel(S_PROJECT->questTypes());
        return combo;
    }
}

void SpreadSheetQuetTypeDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    int row = index.row();
    if(row != 0) {
        QComboBox *combo = qobject_cast<QComboBox*>(editor);
        QStandardItemModel *model = S_PROJECT->questTypes();
        int row_count = model->rowCount();

        for(int i = 0; i < row_count; i++) {
            QModelIndex index2 = model->index(i, 1);
            if(model->data(index2).toString() == index.data().toString()) {
                combo->setCurrentIndex(i);
                break;
            }
        }
    } else
        QItemDelegate::setEditorData(editor, index);
}

void SpreadSheetQuetTypeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                   const QModelIndex &index) const {
    int row = index.row();
    if(row != 0) {
        QComboBox *combo = qobject_cast<QComboBox*>(editor);
        QStandardItemModel *model2 = S_PROJECT->questTypes();
        QModelIndex index2 = model2->index(combo->currentIndex(), 1);
        QModelIndex index3 = model2->index(combo->currentIndex(), 0);

        model->setData(index, model2->data(index2));
        model->setData(index, model2->data(index3), Qt::UserRole);
    } else
        QItemDelegate::setModelData(editor, model, index);
}

void SpreadSheetQuetTypeDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
           const QModelIndex &index) const {
    if(index.row() != 0) {
        QTextOption topt;
        topt.setAlignment(option.decorationAlignment);
        topt.setUseDesignMetrics(true);
        painter->drawText(option.rect, index.data(Qt::UserRole).toString(), topt);
    } else
        QItemDelegate::paint(painter, option, index);
}

//////////////////////////////////////////////////////////////

QuestionsSpreadSheet::QuestionsSpreadSheet(QWidget *parent) :
    QDialog(parent)
{
    setMinimumSize(QSize(640, 480));
    spreadsheet = new QTableWidget(this);
    spreadsheet->setColumnCount(11);
    spreadsheet->setRowCount(11);

    spreadsheet->setItemDelegateForColumn(0, new SpreadSheetQuetTypeDelegate);

    QStringList headers;
    headers << tr("Question type")
            << tr("Question");

    for(int i = 0; i < headers.count(); i++) {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(headers.at(i));
        item->setTextAlignment(Qt::AlignCenter);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        spreadsheet->setItem(0, i, item);
    }

    CMainWindow *wnd = new CMainWindow;
    wnd->setCentralWidget(spreadsheet);
    QLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(wnd);
    setLayout(mainLayout);
}
