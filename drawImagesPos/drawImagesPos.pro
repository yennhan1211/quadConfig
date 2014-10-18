#-------------------------------------------------
#
# Project created by QtCreator 2014-06-18T16:37:43
#
#-------------------------------------------------

QT       += gui widgets

TARGET = drawImagesPos
TEMPLATE = lib

DEFINES += DRAWIMAGESPOS_LIBRARY

SOURCES += drawimagespos.cpp

HEADERS += drawimagespos.h\
        drawimagespos_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
