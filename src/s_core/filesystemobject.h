#ifndef FILESYSTEMOBJECT_H
#define FILESYSTEMOBJECT_H

#include "s_core_global.h"
#include <QObject>
#include "filesystem.h"

class S_CORESHARED_EXPORT FileSystemObject : public QObject
{
    Q_OBJECT
public:
    explicit FileSystemObject(QObject *parent = 0);
    bool addArchive(const QString &filename, bool replace = false);
    bool isOpened(int id);
    int hasFile(const QString &filename, int id = -1);
    bool addFile(QByteArray f_data, const QString &filename, int id = -1);
    QByteArray fileData(const QString &filename, int id = -1);
    bool deleteArchive(int id, bool del_file = false);
    bool moveFile(const QString &filename, int from, int to);

signals:

public slots:

private:
    QList<FSHANDLE*> archives;

};

#endif // FILESYSTEMOBJECT_H
