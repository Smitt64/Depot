#include "assistantadaptor.h"

AssistantAdaptor::AssistantAdaptor(QObject *parent) :
    QDBusAbstractAdaptor(parent)
{
    setAutoRelaySignals(true);
}
