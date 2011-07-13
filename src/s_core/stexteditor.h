#ifndef STEXTEDITOR_H
#define STEXTEDITOR_H

#include "s_texteditwidget.h"
#include <QDialog>
#include <QMainWindow>
#include <QToolBar>
#include <QComboBox>
#include <QFontComboBox>

class STextEditorView : public QMainWindow
{
    Q_OBJECT
public:
    explicit STextEditorView(QWidget *parent = 0);

private:
    STextEditWidget *textEdit;
    QComboBox *size_box;
    QFontComboBox *font_box;
    QToolBar *tool_bar;
};

class STextEditor : public QDialog
{
    Q_OBJECT
public:
    explicit STextEditor(QWidget *parent = 0);

signals:

public slots:

};

#endif // STEXTEDITOR_H
