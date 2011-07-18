#include "stexteditor.h"

STextEditorView::STextEditorView(QWidget *parent) :
    QMainWindow(parent)
{
    textEdit = new STextEditWidget;
    setCentralWidget(textEdit);

    size_box = new QComboBox;
    size_box->addItems(QStringList()
                       << "9"
                       << "10"
                       << "11"
                       << "12"
                       << "14"
                       << "16"
                       << "18"
                       << "20"
                       << "22"
                       << "24"
                       << "26"
                       << "28"
                       << "36"
                       << "48"
                       << "72");

    font_box = new QFontComboBox;
    tool_bar = addToolBar(tr("Formating"));
    tool_bar->addWidget(size_box);
    tool_bar->addWidget(font_box);
    tool_bar->addSeparator();
}

/////////////////////////////////////////////////////////////////////////

STextEditor::STextEditor(QWidget *parent) :
    QDialog(parent)
{
}
