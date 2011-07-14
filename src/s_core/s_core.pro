CONFIG += qt
QT += xml sql
TEMPLATE = lib
CONFIG += dll

release: DESTDIR = ../../bin/release
debug: DESTDIR = ../../bin/debug

OBJECTS_DIR = .build
MOC_DIR = .build
UI_DIR = .build

HEADERS = sapplication.h \
          cmainwindow.h \
          filesystem.h \
    sproject.h \
    commands.h \
    questionsmodel.h \
    s_texteditwidget.h \
    stexteditor.h \
    scustomizedlg.h \
    stoolbar.h

SOURCES = sapplication.cpp \
          cmainwindow.cpp \
          filesystem.cpp \
    sproject.cpp \
    commands.cpp \
    questionsmodel.cpp \
    s_texteditwidget.cpp \
    stexteditor.cpp \
    scustomizedlg.cpp \
    stoolbar.cpp

RESOURCES += \
    ../../res/s_core_res.qrc

DEFINES += FS_DEBUG
