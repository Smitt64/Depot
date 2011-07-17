#ifndef QUESTEDOTOR_INTERFACE_H
#define QUESTEDOTOR_INTERFACE_H

#include <QWidget>
#include <QAbstractItemModel>

class QuestEditorInterface : public QWidget{
    Q_OBJECT
public:
    virtual ~QuestEditorInterface() {}
    virtual void getResource(int id, QString *name, QByteArray *data) = 0;
    virtual int getResCount() = 0;
    virtual void makeQuestion(QAbstractItemModel *questions) = 0;

signals:
    void needSaveResource(QString name, QByteArray data);
    void removeResource(QString name);
};

#endif //QUESTEDOTOR_INTERFACE_H
