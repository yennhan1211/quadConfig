#-------------------------------------------------
#
# Project created by QtCreator 2014-07-17T10:47:40
#
#-------------------------------------------------

QT       += gui widgets network

TARGET = IOhelp
TEMPLATE = lib

DEFINES += IOHELP_LIBRARY

SOURCES += \
    iohelper.cpp \
    downloadhelper.cpp

HEADERS +=\
        iohelp_global.h \
    iohelper.h \
    downloadhelper.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
