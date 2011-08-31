QT += sql xml xmlpatterns webkit
CONFIG += help

TARGET = TestBuilder
TEMPLATE = app

OBJECTS_DIR = .build
MOC_DIR = .build
UI_DIR = .build

INCLUDEPATH = ../s_core/

#debug: {
    LIBS = ../../bin/libs_core.a
    DESTDIR = ../../bin
#}

#release: {
    #LIBS = ../../bin/release/libs_core.a
#    DESTDIR = ../../bin/release/
#}

#install
#target.path = ../../bin/

#INSTALLS += target

SOURCES = main.cpp \
    mainwindow.cpp \
    dialogs/edittheme.cpp \
    dialogs/editquestion.cpp

HEADERS += \
    mainwindow.h \
    dialogs/edittheme.h \
    dialogs/editquestion.h
