QT += sql xml

TARGET = TestBuilder
TEMPLATE = app

OBJECTS_DIR = ../../.build
MOC_DIR = ../../.build
UI_DIR = ../../.build

INCLUDEPATH = ../s_core

release: DESTDIR = ../../bin/release
debug: DESTDIR = ../../bin/debug

#HEADERS = ../s_core/sapplication.h \
#          ../s_core/cmainwindow.h
#SOURCES = ../s_core/sapplication.cpp \
#          ../s_core/cmainwindow.cpp \
#          main.cpp
SOURCES = main.cpp \
    mainwindow.cpp

debug:LIBS += ../../bin/debug/libs_core.a

HEADERS += \
    mainwindow.h
