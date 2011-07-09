QT += sql xml

TARGET = TestBuilder
TEMPLATE = app

OBJECTS_DIR = .build
MOC_DIR = .build
UI_DIR = .build

INCLUDEPATH = ../s_core

release: DESTDIR = ../../bin/release
debug: DESTDIR = ../../bin/debug

SOURCES = main.cpp \
    mainwindow.cpp \
    dialogs/edittheme.cpp

HEADERS += \
    mainwindow.h \
    dialogs/edittheme.h

debug:LIBS += ../../bin/debug/libs_core.a
