#-------------------------------------------------
#
# Project created by QtCreator 2014-07-17T10:47:40
#
#-------------------------------------------------

QT       +=  core widgets network serialport

TARGET = IOhelp
TEMPLATE = lib

DEFINES += IOHELP_LIBRARY

SOURCES += \
    iohelper.cpp \
    downloadhelper.cpp \
    ioconfig.cpp \
    sslclient.cpp

HEADERS +=\
        iohelp_global.h \
    iohelper.h \
    downloadhelper.h \
    ioconfig.h \
    sslclient.h \
    cmddef.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    myRes.qrc
