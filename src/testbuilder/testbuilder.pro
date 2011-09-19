QT += sql xml xmlpatterns webkit
CONFIG += help

TARGET = TestBuilder
TEMPLATE = app

OBJECTS_DIR = .build
MOC_DIR = .build
UI_DIR = .build

INCLUDEPATH = ../s_core/

LIBS = ../../bin/libs_core.a
DESTDIR = ../../bin

#install
#target.path = ../../bin/

#INSTALLS += target

SOURCES = main.cpp \
    mainwindow.cpp \
    dialogs/edittheme.cpp \
    dialogs/editquestion.cpp \
    xmlsyntaxhighlighter.cpp \
    dialogs/xmlconfigdialog.cpp

HEADERS += \
    mainwindow.h \
    dialogs/edittheme.h \
    dialogs/editquestion.h \
    xmlsyntaxhighlighter.h \
    dialogs/xmlconfigdialog.h




