﻿#include "filesystem.h"
#include <QDataStream>
#include <QObject>
#include <QFileInfo>
#include <qstringlist.h>
#include <QBuffer>
#include <QDebug>

FileSystem *FileSystem::m_pInstance = NULL;

FileSystem::FileSystem()
{
}

FileSystem *FileSystem::getInst()
{
    if(!FileSystem::m_pInstance)
        FileSystem::m_pInstance = new FileSystem();

    return FileSystem::m_pInstance;
}

bool FileSystem::fsOpen(const QString &fname, FSHANDLE *handle)
{
    //Открыть архив для чтения
    bool hr = true;//Результат операций

    handle->archFName = fname;

    if(handle->file.isOpen())
        handle->file.close();

    handle->file.setFileName(fname);
    //handle->file.setPermissions(QFile::WriteOwner);
    if(handle->file.open(QFile::ReadWrite))//Если удалось открыть файл
    {
        handle->dataHeader = new Header();

        handle->file.read(((char*)(handle->dataHeader)), sizeof(Header));
        if(handle->dataHeader->m_pVersion != 121)//Проверить версию
            return false;

        if(handle->dataHeader->m_pFileCount)
            hr = this->fsReadContents(handle);
    }

    return hr;
}

bool FileSystem::fsOpen(FSHANDLE *handle)
{
    return this->fsOpen(handle->archFName, handle);
}

bool FileSystem::fsCreate(QString fname, FSHANDLE *handle)
{
    QFile f(fname);
    if(f.open(QIODevice::WriteOnly))
    {
        //Записать пустой заголовок
        Header *head = new Header();
        head->m_pFileCount = 0;
        head->m_pVersion = 121;
        head->m_pFileBlockSize = 0;

        if(f.write((const char*)head, sizeof(Header)) != sizeof(Header))
        {
            f.close();
            return false;
        }
    }
    f.close();

    this->fsOpen(fname, handle);

    return true;
}

bool FileSystem::fsReadContents(FSHANDLE *handle)
{
    handle->file.seek(handle->dataHeader->m_pFileBlockSize + sizeof(Header));//Переместить указатель в конец блока с файлами
    int size = handle->file.size() - (handle->dataHeader->m_pFileBlockSize + sizeof(Header));
    QByteArray fileTable;
    fileTable = handle->file.read(size);
    QByteArray newTable = qUncompress(fileTable);

    File *tmpFile = (File*)newTable.constData();
    for(int i = 0; i < handle->dataHeader->m_pFileCount; i++)
        handle->fileList.push_back(tmpFile[i]);//Добавить в буфер

    return true;
}

bool FileSystem::fsWriteFile(QByteArray data, QString name, bool temp, FSHANDLE *handle, int compressLevel)
{
    QByteArray compressed = qCompress(data, compressLevel);

    int dataSize = compressed.size();
    handle->file.seek(handle->dataHeader->m_pFileBlockSize + sizeof(Header));
    if(handle->file.write((const char*)compressed, dataSize) == -1)
        return false;

    //Переместиться в начало архива
    handle->file.seek(0);
    //Изменить количество файлов
    handle->dataHeader->m_pFileCount++;
    handle->dataHeader->m_pFileBlockSize += dataSize;
    //Перезаписать заголовок
    handle->file.write((const char*)handle->dataHeader, sizeof(Header));

    //Заполнить информацию о файле
    File info;
    info.m_pCompressedSize = compressed.size();
    info.m_pCompressLevel = compressLevel;

    qstrcpy(info.m_pName, name.toLocal8Bit().data());
    strcpy(info.m_pExtension, this->fsGetExstension(name).toLocal8Bit().data());

    info.m_pSize = data.size();
    info.m_pPosition = sizeof(Header) + handle->dataHeader->m_pFileBlockSize - dataSize;
    info.m_flags = (temp ? FileSystem::FileTemporary : 0);

    handle->fileList.push_back(info);

    //Изменить размер файла архива
    handle->file.resize(sizeof(Header) + handle->dataHeader->m_pFileBlockSize +
                        sizeof(File) * handle->dataHeader->m_pFileCount);

    return fsWriteFileList(handle);
}

bool FileSystem::fsAddFile(QString source, QString folder, FSHANDLE *handle, int compressLevel)
{
    bool hr = false;//Результат операций

    QFile sourceFile(source);//Файд для добавления
    if(sourceFile.open(QIODevice::ReadOnly))
    {
        QByteArray data = sourceFile.read(sourceFile.size());
        hr = this->fsWriteFile(data, folder, false, handle, compressLevel);
    }

    return hr;
}

bool FileSystem::fsAddFile(QByteArray data, const QString &name,  FSHANDLE *handle, int compressLevel)
{
    return this->fsWriteFile(data, name, false, handle, compressLevel);
}

bool FileSystem::fsAddTempFile(QString source, QString name, FSHANDLE *handle, int compressLevel)
{
    bool hr = false;//Результат операций

    QFile sourceFile(source);//Файд для добавления
    if(sourceFile.open(QIODevice::ReadOnly))
    {
        QByteArray data = sourceFile.read(sourceFile.size());
        hr = this->fsWriteFile(data, name, true, handle, compressLevel);
    }

    return hr;
}

bool FileSystem::fsAddTempFile(QByteArray data, QString name, FSHANDLE *handle, int compressLevel)
{
    return this->fsWriteFile(data, name, true, handle, compressLevel);
}

bool FileSystem::fsDelete(QString fileName, FSHANDLE *handle)
{
    //Найти файл в таблице
    File file = File();//Информация об удаляемом файле
    bool finded = false;//Флаг, найден ли файл
    int index = 0;//Его индекс в таблице
    for(int i = 0; i < handle->dataHeader->m_pFileCount; i++)
        if(fileName == QString::fromLocal8Bit(handle->fileList[i].m_pName))
        {
            file = handle->fileList[i];
            finded = true;
            index = i;
        }

    //Если файл не найден
    if(!finded)
        return false;

    int size = file.m_pCompressedSize;//Размер файла после сжатия

    /*
        Сместить последующие файлы на место данного.
        Перезаписать позийии.
    */
    for(int i = index + 1; i < handle->dataHeader->m_pFileCount; i++)
    {
        //Переместиться в начало следующего файла
        handle->file.seek(handle->fileList[i].m_pPosition);
        //Прочитать файл
        QByteArray data = handle->file.read(handle->fileList[i].m_pCompressedSize);

        //Изменить в таблице положение файла
        handle->fileList[i].m_pPosition -= size;
        //Переместить указатель в новое положение
        handle->file.seek(handle->fileList[i].m_pPosition);
        //Записать файл
        handle->file.write(data);
    }
    //Удалить информацию о файле из таблицы
    handle->fileList.removeAt(index);
    //Изменить в заголовке размер блока с файлами
    handle->dataHeader->m_pFileBlockSize -= size;
    handle->dataHeader->m_pFileCount--;

    //Перезаписать заголовок
    handle->file.seek(0);
    handle->file.write((const char*)handle->dataHeader, sizeof(Header));

    //Изменить размер файла
    handle->file.resize(sizeof(Header) + handle->dataHeader->m_pFileBlockSize +
                        sizeof(File) * handle->dataHeader->m_pFileCount);
    //Перезаписать список файлов
    fsWriteFileList(handle);

    return true;
}

bool FileSystem::fsExtractFile(QString fileName, QString saveTo, FSHANDLE *handle)
{
    //Найти файл в таблице
    File file;//Информация об удаляемом файле
    bool finded = false;//Флаг, найден ли файл
    for(int i = 0; i < handle->dataHeader->m_pFileCount; i++)
        if(fileName == QString::fromLocal8Bit(handle->fileList[i].m_pName))
        {
            file = handle->fileList[i];
            finded = true;
        }
    if(!finded)
        return false;

    //Содержание файла
    QByteArray data = this->fsGetFile(fileName, handle);
    QFile f(saveTo);
    if(!f.open(QFile::WriteOnly))
        return false;

    f.write(data);
    f.close();
    return true;
}

char *FileSystem::fsGetFName(QString fileName)
{
    if(fileName.isEmpty())
        return ((char*)(""));

    QStringList list = fileName.split('\\');

    return list.last().toLatin1().data();
}

QByteArray FileSystem::fsGetFile(QString name, FSHANDLE *handle)
{
    QByteArray data;

    foreach(File file, handle->fileList) {
        if(qstrcmp(file.m_pName, name.toLocal8Bit().data()) != 0) {
            continue;
        }

        int pos = file.m_pPosition;
        int csize = file.m_pCompressedSize;
        handle->file.seek(pos);
        data = handle->file.read(csize);
        break;
    }

    return qUncompress(data);
}

QString FileSystem::fsGetExstension(QString file)
{
    QFileInfo inf(file);

    return inf.completeSuffix();
            //.extension();
}

void FileSystem::fsClose(FSHANDLE *handle)
{
    //fsWriteFileList(handle);
    handle->fileList.clear();
    handle->file.close();
}

bool FileSystem::fsHasFile(QString name, FSHANDLE *handle)
{
    if(fsGetFileIndex(name, handle) != -1)
        return true;
    return false;
}

int FileSystem::fsGetFileIndex(QString name, FSHANDLE *handle)
{
    for(int i = 0; i < handle->dataHeader->m_pFileCount; i++)
        if(QString::fromLocal8Bit(handle->fileList[i].m_pName) == name)
            return i;
    return -1;
}

bool FileSystem::fsRewriteFile(QByteArray data, QString name, FSHANDLE *handle, int compressLevel)
{
    //Найти файл в таблице

    int index = this->fsGetFileIndex(name, handle);//Его индекс в таблице

    //Если файл не найден
    if(index == -1)
    {
        return false;
    }

    int size = handle->fileList[index].m_pCompressedSize;//Размер файла после сжатия

    /*
        Сместить последующие файлы на место данного.
        Перезаписать позийии.
    */
    for(int i = index + 1; i < handle->dataHeader->m_pFileCount; i++)
    {
        //Переместиться в начало следующего файла
        handle->file.seek(handle->fileList[i].m_pPosition);
        //Прочитать файл
        QByteArray data = handle->file.read(handle->fileList[i].m_pCompressedSize);

        //Изменить в таблице положение файла
        handle->fileList[i].m_pPosition -= size;
        //Переместить указатель в новое положение
        handle->file.seek(handle->fileList[i].m_pPosition);
        //Записать файл
        handle->file.write(data);
    }
    //Изменить в заголовке размер блока с файлами
    handle->dataHeader->m_pFileBlockSize -= size;

    QByteArray compressed = qCompress(data, compressLevel);

    //Записать новый файл в конец архива
    handle->file.seek(sizeof(Header) + handle->dataHeader->m_pFileBlockSize);
    handle->file.write(compressed);

    //Изменить информацию о новом файле
    handle->fileList[index].m_pCompressLevel = compressLevel;
    handle->fileList[index].m_pCompressedSize = compressed.size();
    handle->fileList[index].m_pPosition = sizeof(Header) + handle->dataHeader->m_pFileBlockSize;
    handle->fileList[index].m_pSize = data.size();

    //Изменить в заголовке размер блока с файлами
    handle->dataHeader->m_pFileBlockSize += compressed.size();

    handle->file.seek(0);
    handle->file.write((const char*)handle->dataHeader, sizeof(Header));

    //Изменить размер файла
    handle->file.resize(sizeof(Header) + handle->dataHeader->m_pFileBlockSize +
                        sizeof(File) * handle->dataHeader->m_pFileCount);
    //Перезаписать список файлов
    fsWriteFileList(handle);

    return true;
}

bool FileSystem::fsRenameFile(QString fname, QString newFName, FSHANDLE *handle)
{
    //Проверить наличие файла
    if(!this->fsHasFile(fname, handle))
        return false;

    //Положение файла в таблице
    return true;
}

bool FileSystem::fsWriteFileList(FSHANDLE *handle)
{
    QByteArray fileTable;
    QBuffer buf(&fileTable);
    buf.open(QIODevice::WriteOnly);

    QDataStream stream(&buf);
    stream.setVersion(QDataStream::Qt_4_7);

    /*for(int i = 0; i < handle->fileList.size(); i++)
    {
        File f = handle->fileList[i];
        stream.device()->write((const char*)&f, sizeof(File));
    }*/
    foreach (File f, handle->fileList) {
        if(stream.device()->write((const char*)&f, sizeof(File)) != sizeof(File))
            return false;
    }
    buf.close();

    fileTable = qCompress(fileTable, 1);

    if(fileTable.isNull()) {
        qDebug() << "fileTable is null!";
        return false;
    }

    handle->file.seek(sizeof(Header) + handle->dataHeader->m_pFileBlockSize);

    if(handle->file.write(fileTable) != fileTable.size()) {
        qDebug() << "write file table error!";
        return false;
    }

    if(!handle->file.flush()) {
        qDebug() << "flush error!";
        return false;
    }

    return true;
}

QString FileSystem::fsGetFDir(QString fileName)
{
    if(fileName.isEmpty())
        return "";

    int size = 0;
    for(int i = fileName.length() - 1; i > 0; i--)
    {
        if(fileName[i].toLatin1() == '/' || fileName[i].toLatin1() == '\\')
            break;
        size++;
    }
    QString name;
    for(int i = 0; i < fileName.length() - size; i++)
        name += fileName[i];

    return name;
}

void FileSystem::fsFlushTempFiles(FSHANDLE *handle)
{
    for(int i = 0; i < handle->fileList.count(); i++) {
        QFlags<FileFlag> fl = QFlags<FileFlag>(handle->fileList[i].m_flags);
        if(fl.testFlag(FileSystem::FileTemporary))
            handle->fileList[i].m_flags ^= QFlags<FileFlag>(FileSystem::FileTemporary);
    }

    this->fsWriteFileList(handle);
}

void FileSystem::fsDeleteTempFiles(FSHANDLE *handle)
{
    QStringList toDelete;
    for(int i = 0; i < handle->fileList.count(); i++)
        if(QFlags<FileFlag>(handle->fileList[i].m_flags).testFlag(FileSystem::FileTemporary))
            toDelete.push_back(handle->fileList[i].m_pName);

    for(int i = 0; i < toDelete.count(); i++)
        this->fsDelete(toDelete[i], handle);
}

QStringList FileSystem::fsGetFolders(QString parent, FSHANDLE *handle)
{
    QStringList list;

    for(int i = 0; i < handle->fileList.count(); i++)
    {
        QString str = QString(handle->fileList.at(i).m_pName);

        if(str.contains(parent))
        {
            str.remove(0, parent.length());

            QStringList buf = str.split('\\');
            if(buf.count() > 1 && !list.contains(buf[0]))
                list.push_back(buf[0]);
        }
    }
    list.sort();

    return list;
}

QStringList FileSystem::fsGetListFiles(QString parent, FSHANDLE *handle)
{
    QStringList list;

    for(int i = 0; i < handle->fileList.count(); i++)
    {
        QString str = QString(handle->fileList.at(i).m_pName);

        if(str.contains(parent))
        {
            str.remove(0, parent.length());

            QStringList buf = str.split('\\');
            if(buf.count() == 1)
            {
                list.push_back(buf[0]);
            }
        }
    }

    list.sort();

    return list;
}

QStringList FileSystem::fsGetFileNamesList(FSHANDLE *handle)
{
    QStringList list;

    for(int i = 0; i < handle->fileList.count(); i++)
    {
        list.push_back(QString(handle->fileList[i].m_pName));
    }

    return list;
}

bool FileSystem::fsIsOpen(FSHANDLE *handle)
{
    return handle->file.isOpen();
}

void FileSystem::fsSetFileFlag(QString filename, int flag, FSHANDLE *handle) {
    if(!fsHasFile(filename, handle))
        return;
    int index = fsGetFileIndex(filename, handle);
    handle->fileList[index].m_flags = flag;

    this->fsWriteFileList(handle);
}
