QT += core gui xml xmlpatterns webkit

TARGET = dAssistant
TEMPLATE = app

CONFIG += help

OBJECTS_DIR = .build
MOC_DIR = .build
UI_DIR = .build

SOURCES += main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

target.path = ../../bin/
INSTALLS += target

contains(MEEGO_EDITION,harmattan) {
    target.path = /opt/dAssistant/bin
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../s_core/release/ -ls_core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../s_core/debug/ -ls_core
else:symbian: LIBS += -ls_core
else:unix: LIBS += -L$$OUT_PWD/../s_core/ -ls_core

INCLUDEPATH += $$PWD/../s_core
DEPENDPATH += $$PWD/../s_core
