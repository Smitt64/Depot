#ifndef STEXTEDITOR_H
#define STEXTEDITOR_H

#include "s_core_global.h"
#include "s_texteditwidget.h"
#include <QDialog>
#include <QMainWindow>
#include <QToolBar>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFontComboBox>

class S_CORESHARED_EXPORT STextEditorView : public QMainWindow
{
    Q_OBJECT
public:
    explicit STextEditorView(QWidget *parent = 0);

    bool isModified();
    bool isEmpty();

    QByteArray save();
    QString html();
    QString plainText();

signals:
    void contentChanged();

public slots:
    void setHtml(QString source);
    void setText(QString source);

private slots:
    void textAlign(QAction *a);
    void textBold();
    void textUnderline();
    void textItalic();
    void currentCharFormatChanged(const QTextCharFormat &format);
    void cursorPositionChanged();
    void fontChanged(const QFont &f);
    void colorChanged(const QColor &c);
    void alignmentChanged(Qt::Alignment a);
    void textFamily(const QString &f);
    void textSize(const QString &p);
    void textColor();

private:
    STextEditWidget *textEdit;
    QComboBox *size_box;
    QFontComboBox *font_box;
    QToolBar *tool_bar;

    QAction *actionTextBold, *actionTextItalic, *actionTextUnderline;
    QAction *actionAlignLeft, *actionAlignCenter, *actionAlignRight, *actionAlignJustify;
    QAction *actionTextColor;
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
};

class S_CORESHARED_EXPORT STextEditor : public QDialog
{
    Q_OBJECT
public:
    explicit STextEditor(QWidget *parent = 0);
    QByteArray save();
    QString html();
    QString plainText();

    void setHtml(QString source);
    void setText(QString source);

signals:

public slots:
    void reject();

private slots:
    void contentChanged();

private:
    STextEditorView *editor;
    QDialogButtonBox *buttons;
    QPushButton *okButton, *cancelButton;
};

#endif // STEXTEDITOR_H
