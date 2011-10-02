#include "xmlconfigdialog.h"
#include "sproject.h"
#include <QtXmlPatterns>
#include "../xmlsyntaxhighlighter.h"

XmlConfigDialog::XmlConfigDialog(QWidget *parent) :
    QDialog(parent)
{
    QLayout *mainLayout = new QVBoxLayout;
    QTabWidget *tabWidget = new QTabWidget;
    configView = new QTextEdit;
    configView->setPlainText(S_PROJECT->writeXMLConfig());
    new XmlSyntaxHighlighter(configView->document());

    //Query tab
    QWidget *queryTab = new QWidget;
    QFormLayout *queryLayout = new QFormLayout;
    queryEdit = new QTextEdit;
    queryEdit->setPlainText("declare variable $inputDocument external;\n"
                            "doc($inputDocument)/");
    execButton = new QPushButton(tr("Execute"));
    queryResult = new QTextEdit;
    queryResult->setReadOnly(true);
    new XmlSyntaxHighlighter(queryResult->document());

    queryLayout->addRow(tr("XQuery"), queryEdit);
    queryLayout->addWidget(execButton);
    queryLayout->addRow(tr("Query result"), queryResult);

    queryTab->setLayout(queryLayout);
    mainLayout->addWidget(tabWidget);
    setLayout(mainLayout);

    tabWidget->addTab(configView, tr("Configuration"));
    tabWidget->addTab(queryTab, tr("Query"));

    setProperty("size", QSize(320, height()));

    connect(execButton, SIGNAL(clicked()), this, SLOT(evaluateQuery()));
}

void XmlConfigDialog::evaluateQuery() {
    QXmlQuery query;
    query.setQuery(queryEdit->toPlainText());
    QBuffer document;
    document.setData(QByteArray().append(configView->toPlainText()));
    document.open(QIODevice::ReadOnly);
    query.bindVariable("inputDocument", &document);
    QString result;
    if(query.evaluateTo(&result)) {
        queryResult->setPlainText(result);
    } else {
        qDebug() << "query error!";
    }
}
