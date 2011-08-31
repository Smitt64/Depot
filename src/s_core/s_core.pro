﻿CONFIG += qt
QT += xml sql xmlpatterns webkit network
TEMPLATE = lib
CONFIG += dll help

DESTDIR = ../../bin
#release: DESTDIR = ../../bin/release

#inctall
#target.path = ../../bin/
#INSTALLS += target

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
    stoolbar.h \
    interfaces/testtype_interface.h \
    defaulttesttypes/default_tst_types.h \
    scombobox.h \
    interfaces/questeditor_interface.hpp \
    shelpcontentviewwidget.h \
    msstyle/windowsmodernstyle.h \
    filesystemobject.h

SOURCES = sapplication.cpp \
          cmainwindow.cpp \
          filesystem.cpp \
    sproject.cpp \
    commands.cpp \
    questionsmodel.cpp \
    s_texteditwidget.cpp \
    stexteditor.cpp \
    scustomizedlg.cpp \
    stoolbar.cpp \
    defaulttesttypes/default_tst_types.cpp \
    scombobox.cpp \
    shelpcontentviewwidget.cpp \
    msstyle/windowsmodernstyle.cpp \
    filesystemobject.cpp

RESOURCES += \
    ../../res/s_core_res.qrc

DEFINES += FS_DEBUG
