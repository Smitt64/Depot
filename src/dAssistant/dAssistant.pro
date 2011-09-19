QT += core gui xml xmlpatterns webkit

TARGET = dAssistant
TEMPLATE = app

CONFIG += help

OBJECTS_DIR = .build
MOC_DIR = .build
UI_DIR = .build

INCLUDEPATH += ../s_core/
DESTDIR = ../../bin
LIBS += ../../bin/libs_core.a

#DBUS_ADAPTORS += com.depot.2_0.assistant.xml

SOURCES += main.cpp \
    mainwindow.cpp

HEADERS  += \
    mainwindow.h





