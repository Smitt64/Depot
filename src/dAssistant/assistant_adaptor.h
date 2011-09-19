#ifndef ASSISTANT_ADAPTOR_H
#define ASSISTANT_ADAPTOR_H

#include <QtDBus>

class assistant_adaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
public:
    explicit assistant_adaptor(QObject *parent = 0);

signals:

public slots:

};

#endif // ASSISTANT_ADAPTOR_H
