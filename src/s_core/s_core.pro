#-------------------------------------------------
#
# Project created by QtCreator 2011-09-20T21:44:43
#
#-------------------------------------------------
include(../../build.pri)
QT += network sql webkit xml xmlpatterns declarative

TARGET = s_core
TEMPLATE = lib
CONFIG += dll

#DESTDIR = ../../bin/

DEFINES += S_CORE_LIBRARY

OBJECTS_DIR = .build
MOC_DIR = .build
UI_DIR = .build

#install
target.path = $$PWD/../../bin/
INSTALLS += target

SOURCES += s_core.cpp \
    stoolbar.cpp \
    stexteditor.cpp \
    sproject.cpp \
    scustomizedlg.cpp \
    scombobox.cpp \
    sapplication.cpp \
    s_texteditwidget.cpp \
    questionsmodel.cpp \
    filesystemobject.cpp \
    filesystem.cpp \
    commands.cpp \
    cmainwindow.cpp \
    defaulttesttypes/default_tst_types.cpp \
    msstyle/windowsmodernstyle.cpp \
    showpanel/defaultshowpanel.cpp \
    showpanel/showpanelcomponents.cpp


HEADERS += s_core.h\
        s_core_global.h \
    stoolbar.h \
    stexteditor.h \
    sproject.h \
    scustomizedlg.h \
    scombobox.h \
    sapplication.h \
    s_texteditwidget.h \
    questionsmodel.h \
    filesystemobject.h \
    filesystem.h \
    commands.h \
    cmainwindow.h \
    defaulttesttypes/default_tst_types.h \
    interfaces/testtype_interface.h \
    interfaces/questeditor_interface.hpp \
    msstyle/windowsmodernstyle.h \
    interfaces/showpanel_interface.h \
    showpanel/defaultshowpanel.h \
    showpanel/showpanelcomponents.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE2682E2C
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = s_core.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

!contains(MEEGO_EDITION,harmattan) {
    SOURCES += shelpcontentviewwidget.cpp
    HEADERS += shelpcontentviewwidget.h
    CONFIG += help
}









