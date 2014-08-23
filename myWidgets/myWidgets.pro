#-------------------------------------------------
#
# Project created by QtCreator 2014-06-24T14:53:49
#
#-------------------------------------------------

QT       += widgets

TARGET = myWidgets
TEMPLATE = lib

DEFINES += MYWIDGETS_LIBRARY

SOURCES += \
    lineedittooltip.cpp \
    rxmonitorview.cpp \
    vtmessagebox.cpp \
    vtinfoform.cpp \
    progressdialog.cpp \
    progressring.cpp \
    counterlabel.cpp \
    volumeslider.cpp

HEADERS +=\
        mywidgets_global.h \
    lineedittooltip.h \
    rxmonitorview.h \
    vtmessagebox.h \
    vtinfoform.h \
    progressdialog.h \
    progressring.h \
    counterlabel.h \
    volumeslider.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    vtmessagebox.ui \
    vtinfoform.ui \
    progressdialog.ui \
    volumeslider.ui

