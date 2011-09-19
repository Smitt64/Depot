#include "stexteditor.h"
#include "sapplication.h"
#include <QColorDialog>
#include <QMessageBox>

STextEditorView::STextEditorView(QWidget *parent) :
    QMainWindow(parent)
{
    textEdit = new STextEditWidget;
    setCentralWidget(textEdit);

    size_box = new QComboBox;
    QFontDatabase db;
    foreach(int size, db.standardSizes())
        size_box->addItem(QString::number(size));

    font_box = new QFontComboBox;
    tool_bar = addToolBar(tr("Formating"));
    tool_bar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    tool_bar->setIconSize(SApplication::inst()->settings("interface/tb_iconsize",
                                                         QSize(16, 16)).toSize());
    tool_bar->addWidget(size_box);
    tool_bar->addWidget(font_box);
    tool_bar->addSeparator();

    actionTextBold = tool_bar->addAction(QIcon::fromTheme("format-text-bold"), tr("Bold"));
    actionTextBold->setCheckable(true);
    actionTextBold->setShortcut(Qt::CTRL + Qt::Key_B);
    actionTextBold->setCheckable(true);

    actionTextItalic = tool_bar->addAction(QIcon::fromTheme("format-text-italic"), tr("Italic"));
    actionTextItalic->setShortcut(Qt::CTRL + Qt::Key_I);
    actionTextItalic->setCheckable(true);

    actionTextUnderline = tool_bar->addAction(QIcon::fromTheme("format-text-underline"), tr("Underline"));
    actionTextUnderline->setShortcut(Qt::CTRL + Qt::Key_U);
    actionTextUnderline->setCheckable(true);

    tool_bar->addSeparator();

    QActionGroup *grp = new QActionGroup(this);
    actionAlignLeft = new QAction(QIcon::fromTheme("format-justify-left"), tr("Align left"), grp);
    actionAlignLeft->setShortcut(Qt::CTRL + Qt::Key_L);
    actionAlignLeft->setCheckable(true);

    actionAlignCenter = new QAction(QIcon::fromTheme("format-justify-center"), tr("Align center"), grp);
    actionAlignCenter->setShortcut(Qt::CTRL + Qt::Key_E);
    actionAlignCenter->setCheckable(true);

    actionAlignRight = new QAction(QIcon::fromTheme("format-justify-right"), tr("Align right"), grp);
    actionAlignRight->setShortcut(Qt::CTRL + Qt::Key_R);
    actionAlignRight->setCheckable(true);

    actionAlignJustify = new QAction(QIcon::fromTheme("format-justify-fill"), tr("Align justify"), grp);
    actionAlignJustify->setShortcut(Qt::CTRL + Qt::Key_J);
    actionAlignJustify->setCheckable(true);

    tool_bar->addActions(grp->actions());

    tool_bar->addSeparator();

    actionTextColor = tool_bar->addAction(QIcon::fromTheme("format-font-color"), tr("Font color"));

    connect(grp, SIGNAL(triggered(QAction*)), this, SLOT(textAlign(QAction*)));
    connect(textEdit, SIGNAL(currentCharFormatChanged(QTextCharFormat)),
            this, SLOT(currentCharFormatChanged(QTextCharFormat)));
    connect(textEdit, SIGNAL(cursorPositionChanged()),
            this, SLOT(cursorPositionChanged()));
    connect(font_box, SIGNAL(activated(QString)),
            this, SLOT(textFamily(QString)));
    connect(size_box, SIGNAL(activated(QString)),
            this, SLOT(textSize(QString)));
    connect(actionTextBold, SIGNAL(triggered()), this, SLOT(textBold()));
    connect(actionTextItalic, SIGNAL(triggered()), this, SLOT(textItalic()));
    connect(actionTextUnderline, SIGNAL(triggered()), this, SLOT(textUnderline()));
    connect(actionTextColor, SIGNAL(triggered()), this, SLOT(textColor()));

    connect(textEdit, SIGNAL(textChanged()), this, SIGNAL(contentChanged()));
}

void STextEditorView::textAlign(QAction *a) {
    if (a == actionAlignLeft)
        textEdit->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
    else if (a == actionAlignCenter)
        textEdit->setAlignment(Qt::AlignHCenter);
    else if (a == actionAlignRight)
        textEdit->setAlignment(Qt::AlignRight | Qt::AlignAbsolute);
    else if (a == actionAlignJustify)
        textEdit->setAlignment(Qt::AlignJustify);
}

void STextEditorView::textBold() {
    QTextCharFormat fmt;
    fmt.setFontWeight(actionTextBold->isChecked() ? QFont::Bold : QFont::Normal);
    mergeFormatOnWordOrSelection(fmt);
}

void STextEditorView::textUnderline() {
    QTextCharFormat fmt;
    fmt.setFontUnderline(actionTextUnderline->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}

void STextEditorView::textItalic() {
    QTextCharFormat fmt;
    fmt.setFontItalic(actionTextItalic->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}

void STextEditorView::mergeFormatOnWordOrSelection(const QTextCharFormat &format) {
    QTextCursor cursor = textEdit->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    textEdit->mergeCurrentCharFormat(format);
}

void STextEditorView::currentCharFormatChanged(const QTextCharFormat &format) {
    fontChanged(format.font());
    colorChanged(format.foreground().color());
}

void STextEditorView::cursorPositionChanged() {
    alignmentChanged(textEdit->alignment());
}

void STextEditorView::fontChanged(const QFont &f) {
    font_box->setCurrentIndex(font_box->findText(QFontInfo(f).family()));
    size_box->setCurrentIndex(size_box->findText(QString::number(f.pointSize())));
    actionTextBold->setChecked(f.bold());
    actionTextItalic->setChecked(f.italic());
    actionTextUnderline->setChecked(f.underline());
}

void STextEditorView::colorChanged(const QColor &c) {
    QPixmap pix(":/font_color");
    QPainter painter(&pix);
    painter.fillRect(QRect(0, 12, 16, 4), c);
    actionTextColor->setIcon(pix);
}

void STextEditorView::textColor()
 {
     QColor col = QColorDialog::getColor(textEdit->textColor(), this);
     if (!col.isValid())
         return;
     QTextCharFormat fmt;
     fmt.setForeground(col);
     mergeFormatOnWordOrSelection(fmt);
     colorChanged(col);
 }

void STextEditorView::alignmentChanged(Qt::Alignment a) {
    if (a & Qt::AlignLeft) {
        actionAlignLeft->setChecked(true);
    } else if (a & Qt::AlignHCenter) {
        actionAlignCenter->setChecked(true);
    } else if (a & Qt::AlignRight) {
        actionAlignRight->setChecked(true);
    } else if (a & Qt::AlignJustify) {
        actionAlignJustify->setChecked(true);
    }
}

void STextEditorView::textFamily(const QString &f) {
    QTextCharFormat fmt;
    fmt.setFontFamily(f);
    mergeFormatOnWordOrSelection(fmt);
}

void STextEditorView::textSize(const QString &p) {
    qreal pointSize = p.toFloat();
    if (p.toFloat() > 0) {
        QTextCharFormat fmt;
        fmt.setFontPointSize(pointSize);
        mergeFormatOnWordOrSelection(fmt);
    }
}

bool STextEditorView::isModified() {
    return textEdit->document()->isModified();
}

bool STextEditorView::isEmpty() {
    return textEdit->document()->isEmpty();
}

QString STextEditorView::html() {
    return textEdit->toHtml();
}

QString STextEditorView::plainText() {
    return textEdit->toPlainText();
}

void STextEditorView::setHtml(QString source) {
    textEdit->setHtml(source);
}

void STextEditorView::setText(QString source) {
    textEdit->setPlainText(source);
}

QByteArray STextEditorView::save() {
    QByteArray saved;
    QTextStream stream(&saved);

    stream << html();

    return saved;
}

/////////////////////////////////////////////////////////////////////////

STextEditor::STextEditor(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Text editor"));
    editor = new STextEditorView;
    buttons = new QDialogButtonBox(this);
    okButton = buttons->addButton(tr("Ok"), QDialogButtonBox::AcceptRole);
    okButton->setEnabled(false);
    cancelButton = buttons->addButton(tr("Cancel"), QDialogButtonBox::RejectRole);
    QLayout *mainLayout = new QVBoxLayout(this);

    connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));
    connect(editor, SIGNAL(contentChanged()), this, SLOT(contentChanged()));

    mainLayout->addWidget(editor);
    mainLayout->addWidget(buttons);

    setLayout(mainLayout);
}

void STextEditor::reject() {
    if(editor->isModified()) {
        if(QMessageBox::question(this, tr("Text editor"),
                                 tr("The document has been modified.\n"
                                    "Are you shure to discard your changes?"),
                                 QMessageBox::No | QMessageBox::Discard) == QMessageBox::No)
            return;
        else
            QDialog::reject();
    }
    QDialog::reject();
}

QString STextEditor::html() {
    return editor->html();
}

QString STextEditor::plainText() {
    return editor->plainText();
}

void STextEditor::setHtml(QString source) {
    editor->setHtml(source);
    okButton->setEnabled(editor->isModified());
}

void STextEditor::setText(QString source) {
    editor->setText(source);
    okButton->setEnabled(editor->isModified());
}

void STextEditor::contentChanged() {
    if(editor->isModified() && !editor->isEmpty()) {
        okButton->setEnabled(true);
    }else
        okButton->setEnabled(false);
}

QByteArray STextEditor::save() {
    return editor->save();
}
