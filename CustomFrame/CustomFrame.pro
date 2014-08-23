#-------------------------------------------------
#
# Project created by QtCreator 2014-05-14T12:10:24
#
#-------------------------------------------------

QT       += widgets

TARGET = customframe
TEMPLATE = lib

DEFINES += CUSTOMFRAME_LIBRARY

SOURCES += customframe.cpp \
    custombutton.cpp \
    customtitlebar.cpp

HEADERS += customframe.h\
        customframe_global.h \
    custombutton.h \
    customtitlebar.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
