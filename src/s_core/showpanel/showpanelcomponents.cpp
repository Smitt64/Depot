#include "showpanelcomponents.h"
#include "../sproject.h"

QMap<QString, QString> panel_resources;
void makeResource(QString image_name, QString replaced_name, QColor colorMask) {
    QString fullName = QString("%1/%2")
            .arg(QDir::tempPath(), image_name);

    QPixmap image(QString(":/DefShowPanel/images/%1")
                  .arg(image_name));

    QPainter painter(&image);
#ifndef S_WETAB_OS
    painter.setCompositionMode(QPainter::CompositionMode_Multiply);
    painter.fillRect(image.rect(), colorMask);
#else
    QColor temp_color = colorMask;
    temp_color.setAlpha(120);
#endif

    image.save(fullName, QFileInfo(fullName).completeSuffix().toLocal8Bit().data());

    if(!panel_resources.keys().contains(replaced_name)) {
        panel_resources.insert(replaced_name, fullName);
    }

    if(!panel_resources.keys().contains("${color}")) {
        panel_resources.insert("${color}", colorMask.name());
    }
}

void registerColor(QColor color, QString alias) {
    if(!panel_resources.keys().contains(alias)) {
        panel_resources.insert(alias, color.name());
    } else {
        panel_resources[alias] = color.name();
    }
}

void freeResources() {
    foreach (QString path, panel_resources) {
        QFile::remove(path);
    }
    panel_resources.clear();
}

QString makeStyleSheet(QString base_name) {
    QStringList keys = panel_resources.keys();

    QFile file(QString(":/DefShowPanel/styles/%1")
               .arg(base_name));
    QString makedStyle;
    if(file.open(QIODevice::ReadOnly)) {
        makedStyle = file.readAll();
        file.close();
        foreach (QString replace_mask, keys) {
            makedStyle.replace(replace_mask, panel_resources[replace_mask]);
        }
    }
    return makedStyle;
}

///////////////////////////////////////////////////////

ShowPanelPushButton::ShowPanelPushButton(QColor color, QWidget *parent) :
    QPushButton(parent),
    buttonColor(color)
{
    setAutoFillBackground(false);
    setColor(color);
}

ShowPanelPushButton::~ShowPanelPushButton() {
}

void ShowPanelPushButton::setColor(QColor color) {
    buttonColor = color;

    makeResource("pushbutton.png", "${pushbutton}", buttonColor);
    makeResource("pushbutton_hover.png", "${pushbutton_hover}", buttonColor);
    makeResource("pushbutton_pressed.png", "${pushbutton_pressed}", buttonColor);
    setStyleSheet(makeStyleSheet("pushbutton.txt"));
}

///////////////////////////////////////////////////////
ShowPanelComboBox::ShowPanelComboBox(QColor color, QWidget *parent) :
    QComboBox(parent),
    comboColor(color)
{
    setColor(comboColor);
}

ShowPanelComboBox::~ShowPanelComboBox()
{
}

void ShowPanelComboBox::setColor(QColor color) {
    comboColor = color;
    makeResource("pushbutton.png", "${pushbutton}", comboColor);
    makeResource("pushbutton_hover.png", "${pushbutton_hover}", comboColor);
    makeResource("pushbutton_pressed.png", "${pushbutton_pressed}", comboColor);
    makeResource("frame.png", "${frame}", comboColor);

    registerColor(color, "${color1}");
    registerColor(color.darker(), "${color2}");

    setStyleSheet(makeStyleSheet("combobox.txt"));
}

///////////////////////////////////////////////////////
ShowPanelGroupBox::ShowPanelGroupBox(QColor color, QWidget *parent) :
    QGroupBox(parent)
{

}

ShowPanelGroupBox::~ShowPanelGroupBox()
{

}
