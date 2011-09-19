#ifndef ASSISTANTADAPTOR_H
#define ASSISTANTADAPTOR_H

#include <QDBusAbstractAdaptor>

class AssistantAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "depot.DBus.assistant")
public:
    explicit AssistantAdaptor(QObject *parent = 0);

signals:

public slots:

};

#endif // ASSISTANTADAPTOR_H
