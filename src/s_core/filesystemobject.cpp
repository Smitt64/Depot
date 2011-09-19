#include "filesystemobject.h"
#include <QFile>
#include <QDebug>

FileSystemObject::FileSystemObject(QObject *parent) :
    QObject(parent)
{
}

bool FileSystemObject::addArchive(const QString &filename, bool replace) {
    foreach (FSHANDLE *handle, archives) {
        if(handle->archFName == filename)
            return false;
    }

    FSHANDLE *handle = new FSHANDLE;
    if(QFile::exists(filename)) {
        if(!replace) {
            if(!FileSystem::getInst()->fsOpen(filename, handle)) {
                FileSystem::getInst()->fsClose(handle);
                return false;
            } else {
                archives.push_back(handle);
                FileSystem::getInst()->fsClose(handle);
            }
        } else {
            QFile::remove(filename);
            bool hr =  FileSystem::getInst()->fsCreate(filename, handle);
            FileSystem::getInst()->fsClose(handle);
            if(hr)
                archives.push_back(handle);
            return hr;
        }
    } else {
        bool hr =  FileSystem::getInst()->fsCreate(filename, handle);
        FileSystem::getInst()->fsClose(handle);
        if(hr)
            archives.push_back(handle);
        return hr;
    }
    return true;
}

bool FileSystemObject::isOpened(int id) {
    return FileSystem::getInst()->fsIsOpen(archives[id]);
}

int FileSystemObject::hasFile(const QString &filename, int id) {
    if(id == -1) {
        int index = 0;
        foreach (FSHANDLE *handle, archives) {
            if(FileSystem::getInst()->fsOpen(handle)) {
                if(FileSystem::getInst()->fsHasFile(filename, handle)) {
                    FileSystem::getInst()->fsClose(handle);
                    return index;
                }
                FileSystem::getInst()->fsClose(handle);
            }
            index ++;
        }
    }

    if(id > 0 && id < archives.count()) {
        FSHANDLE *handle = archives[id];
        if(FileSystem::getInst()->fsOpen(handle)) {
            if(FileSystem::getInst()->fsHasFile(filename, handle)) {
                FileSystem::getInst()->fsClose(handle);
                return id;
            }
            FileSystem::getInst()->fsClose(handle);
        }
    }

    return -1;
}

bool FileSystemObject::addFile(QByteArray f_data, const QString &filename, int id) {
    FSHANDLE *handle = NULL;
    if(id == -1) {
        handle = archives.last();
    } else {
        if(id >= 0 && id < archives.count())
            handle = archives[id];
    }

    if(!handle)
        return false;

    if(FileSystem::getInst()->fsIsOpen(handle))
        FileSystem::getInst()->fsClose(handle);

    if(!FileSystem::getInst()->fsOpen(handle)) {
        FileSystem::getInst()->fsClose(handle);
        return false;
    }

    if(FileSystem::getInst()->fsHasFile(filename, handle)) {
        if(!FileSystem::getInst()->fsDelete(filename, handle)) {
            FileSystem::getInst()->fsClose(handle);
            return false;
        }
    }
    bool hr = FileSystem::getInst()->fsAddFile(f_data, filename, handle);
    FileSystem::getInst()->fsClose(handle);
    return hr;
}

QByteArray FileSystemObject::fileData(const QString &filename, int id) {
    QByteArray f_data;
    int archID = hasFile(filename);
    if(archID == -1)
        return QByteArray();

    FSHANDLE *handle = NULL;
    if(id == -1) {
        handle = archives[archID];
    } else {
        handle = archives[id];
    }
    if(FileSystem::getInst()->fsIsOpen(handle))
        FileSystem::getInst()->fsClose(handle);

    if(!FileSystem::getInst()->fsOpen(handle)) {
        FileSystem::getInst()->fsClose(handle);
        return QByteArray();
    }

    f_data = FileSystem::getInst()->fsGetFile(filename, handle);
    FileSystem::getInst()->fsClose(handle);

    return f_data;
}

bool FileSystemObject::deleteArchive(int id, bool del_file) {
    if(id > 0 && id < archives.count()) {
        if(del_file)
            QFile::remove(archives[id]->archFName);
        archives.removeAt(id);
        return true;
    }
    return false;
}

bool FileSystemObject::moveFile(const QString &filename, int from, int to) {
    int hFile = hasFile(filename, from);

    if(hFile == -1)
        return false;

    QByteArray f_data = fileData(filename, from);
    qDebug() << "moveFile" << f_data;
    return addFile(f_data, filename, to);
}
