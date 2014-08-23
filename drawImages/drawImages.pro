#-------------------------------------------------
#
# Project created by QtCreator 2014-06-18T15:06:58
#
#-------------------------------------------------

QT       += gui widgets

TARGET = drawImages
TEMPLATE = lib

DEFINES += DRAWIMAGES_LIBRARY

SOURCES += drawimages.cpp

HEADERS += drawimages.h\
        drawimages_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
