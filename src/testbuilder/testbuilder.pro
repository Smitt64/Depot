include(../../build.pri)
QT += core gui sql xml xmlpatterns webkit declarative

TARGET = TestBuilder
TEMPLATE = app

OBJECTS_DIR = .build
MOC_DIR = .build
UI_DIR = .build

#install
target.path = $$PWD/../../bin/
INSTALLS += target

SOURCES = main.cpp \
    mainwindow.cpp \
    dialogs/edittheme.cpp \
    dialogs/editquestion.cpp \
    xmlsyntaxhighlighter.cpp \
    dialogs/xmlconfigdialog.cpp \
    dialogs/questionsspreadsheet.cpp

HEADERS += \
    mainwindow.h \
    dialogs/edittheme.h \
    dialogs/editquestion.h \
    xmlsyntaxhighlighter.h \
    dialogs/xmlconfigdialog.h \
    dialogs/questionsspreadsheet.h

contains(MEEGO_EDITION,harmattan) {
    target.path = /opt/testbuilder/bin
    INSTALLS += target
    CONFIG -= help
} else {
    CONFIG += help
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../s_core/release/ -ls_core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../s_core/debug/ -ls_core
else:symbian: LIBS += -ls_core
else:unix: LIBS += -L$$OUT_PWD/../s_core/ -ls_core
#LIBS += -L/../../bin -ls_core

warning($$OUT_PWD)

INCLUDEPATH += $$PWD/../s_core
DEPENDPATH += $$PWD/../s_core


