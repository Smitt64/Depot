#include "shelpcontentviewwidget.h"
#include "sapplication.h"
#include "cmainwindow.h"
#include <QToolBar>
#include <QComboBox>
#include <QDebug>
#include <QWebPage>
#include <QtWebKit>
#include <QtNetwork>
#include <QLineEdit>

struct ExtensionMap {
    const char *extension;
    const char *mimeType;
} extensionMap[] = {
    { ".bmp", "image/bmp" },
    { ".css", "text/css" },
    { ".gif", "image/gif" },
    { ".html", "text/html" },
    { ".htm", "text/html" },
    { ".ico", "image/x-icon" },
    { ".jpeg", "image/jpeg" },
    { ".jpg", "image/jpeg" },
    { ".js", "application/x-javascript" },
    { ".mng", "video/x-mng" },
    { ".pbm", "image/x-portable-bitmap" },
    { ".pgm", "image/x-portable-graymap" },
    { ".pdf", "application/pdf" },
    { ".png", "image/png" },
    { ".ppm", "image/x-portable-pixmap" },
    { ".rss", "application/rss+xml" },
    { ".svg", "image/svg+xml" },
    { ".svgz", "image/svg+xml" },
    { ".text", "text/plain" },
    { ".tif", "image/tiff" },
    { ".tiff", "image/tiff" },
    { ".txt", "text/plain" },
    { ".xbm", "image/x-xbitmap" },
    { ".xml", "text/xml" },
    { ".xpm", "image/x-xpm" },
    { ".xsl", "text/xsl" },
    { ".xhtml", "application/xhtml+xml" },
    { ".wml", "text/vnd.wap.wml" },
    { ".wmlc", "application/vnd.wap.wmlc" },
    { "about:blank", 0 },
    { 0, 0 }
};

QString mimeFromUrl(const QUrl &url)
{
    const QString &path = url.path();
    const int index = path.lastIndexOf(QLatin1Char('.'));
    const QByteArray &ext = path.mid(index).toUtf8().toLower();

    const ExtensionMap *e = extensionMap;
    while (e->extension) {
        if (ext == e->extension)
            return QLatin1String(e->mimeType);
        ++e;
    }
    return QLatin1String("");
}


class HelpNetworkReply : public QNetworkReply
{
public:
    HelpNetworkReply(const QNetworkRequest &request, const QByteArray &fileData,
        const QString &mimeType);

    virtual void abort();

    virtual qint64 bytesAvailable() const
        { return data.length() + QNetworkReply::bytesAvailable(); }

protected:
    virtual qint64 readData(char *data, qint64 maxlen);

private:
    QByteArray data;
    qint64 origLen;
};

HelpNetworkReply::HelpNetworkReply(const QNetworkRequest &request,
        const QByteArray &fileData, const QString& mimeType)
    : data(fileData), origLen(fileData.length())
{
    setRequest(request);
    setOpenMode(QIODevice::ReadOnly);

    setHeader(QNetworkRequest::ContentTypeHeader, mimeType);
    setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(origLen));
    QTimer::singleShot(0, this, SIGNAL(metaDataChanged()));
    QTimer::singleShot(0, this, SIGNAL(readyRead()));
}

void HelpNetworkReply::abort() {
}

qint64 HelpNetworkReply::readData(char *buffer, qint64 maxlen) {
    qint64 len = qMin(qint64(data.length()), maxlen);
    if (len) {
        memcpy(buffer, data.constData(), len);
        data.remove(0, len);
    }
    if (!data.length())
        QTimer::singleShot(0, this, SIGNAL(finished()));
    return len;
}

/////////////////////////////////////////////////////////////////////////////
class HelpNetworkAccessManager : public QNetworkAccessManager
{
public:
    HelpNetworkAccessManager(QObject *parent);

protected:
    virtual QNetworkReply *createRequest(Operation op,
        const QNetworkRequest &request, QIODevice *outgoingData = 0);
};

HelpNetworkAccessManager::HelpNetworkAccessManager(QObject *parent)
    : QNetworkAccessManager(parent)
{
}

QNetworkReply *HelpNetworkAccessManager::createRequest(Operation /*op*/,
    const QNetworkRequest &request, QIODevice* /*outgoingData*/) {
    QString url = request.url().toString();
    QByteArray data = SApplication::inst()->helpData(url);
    return new HelpNetworkReply(request, data, mimeFromUrl(url));
}
/////////////////////////////////////////////////////////////////////////////
class HelpPage : public QWebPage
{
public:
    HelpPage(QObject *parent);

protected:
    virtual QWebPage *createWindow(QWebPage::WebWindowType);
    virtual void triggerAction(WebAction action, bool checked = false);

    virtual bool acceptNavigationRequest(QWebFrame *frame,
        const QNetworkRequest &request, NavigationType type);

private:
    bool closeNewTabIfNeeded;

    friend class HelpViewer;
    QUrl m_loadingUrl;
    Qt::MouseButtons m_pressedButtons;
    Qt::KeyboardModifiers m_keyboardModifiers;
};

HelpPage::HelpPage(QObject *parent)
    : QWebPage(parent)
    , closeNewTabIfNeeded(false)
    , m_pressedButtons(Qt::NoButton)
    , m_keyboardModifiers(Qt::NoModifier)
{
}

QWebPage *HelpPage::createWindow(QWebPage::WebWindowType) {
    HelpPage* newPage = new HelpPage(this);
    if (newPage)
        newPage->closeNewTabIfNeeded = closeNewTabIfNeeded;
    closeNewTabIfNeeded = false;
    return newPage;
}

void HelpPage::triggerAction(WebAction action, bool checked) {
    switch (action) {
        case OpenLinkInNewWindow:
            closeNewTabIfNeeded = true;
        default:
            QWebPage::triggerAction(action, checked);
            break;
    }
}

bool HelpPage::acceptNavigationRequest(QWebFrame *,
    const QNetworkRequest &request, QWebPage::NavigationType type) {

    const QUrl &url = request.url();

    if (type == QWebPage::NavigationTypeLinkClicked
        && (m_keyboardModifiers & Qt::ControlModifier
        || m_pressedButtons == Qt::MidButton)) {
            ((SHelpContentViewWidget*)parent())->setSource(url);
            return false;
    }

    m_loadingUrl = url;
    return true;
}

/////////////////////////////////////////////////////////////////////////////

SHelpContentViewWidget::SHelpContentViewWidget(QWidget *parent) :
    QWebView(parent),
    links(NULL)
{
    setPage(new HelpPage(this));
    page()->setNetworkAccessManager(new HelpNetworkAccessManager(this));
}

void SHelpContentViewWidget::setSource(const QUrl &url) {
    load(url);
}

/////////////////////////////////////////////////////////////////////////////
HelpView::HelpView(bool makeControls, QWidget *parent) :
    CMainWindow(parent)
{
    address_box = new QComboBox;
    address_box->setEditable(true);
    helpContent = new SHelpContentViewWidget;
    helpContent->setSource(QUrl("qthelp://Serpkov_Nikita.com.Depot.2.0/doc/index.html"));

    QLayout *main = new QVBoxLayout;
    QFormLayout *layout = new QFormLayout;
    layout->insertRow(0, "URL: ", address_box);

    main->addItem(layout);
    main->addWidget(helpContent);

    QWidget *central = new QWidget(this);
    central->setLayout(main);

    setCentralWidget(central);
    setWindowTitle(tr("Assistant"));

    connect(helpContent, SIGNAL(urlChanged(QUrl)), this, SLOT(urlChanged(QUrl)));
}

void HelpView::urlChanged(const QUrl &url) {
    address_box->lineEdit()->setText(url.toString());
}
