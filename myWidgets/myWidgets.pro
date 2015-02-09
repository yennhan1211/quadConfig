#-------------------------------------------------
#
# Project created by QtCreator 2014-06-24T14:53:49
#
#-------------------------------------------------

QT       += widgets network

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
    volumeslider.cpp \
    buttonprogress.cpp\
    errorform.cpp

HEADERS +=\
        mywidgets_global.h \
    lineedittooltip.h \
    rxmonitorview.h \
    vtmessagebox.h \
    vtinfoform.h \
    progressdialog.h \
    progressring.h \
    counterlabel.h \
    volumeslider.h \
    buttonprogress.h\
    errorform.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    vtmessagebox.ui \
    vtinfoform.ui \
    progressdialog.ui \
    volumeslider.ui \
    buttonprogress.ui \
    errorform.ui


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../IOhelp/release/ -lIOhelp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../IOhelp/debug/ -lIOhelp

INCLUDEPATH += $$PWD/../IOhelp
DEPENDPATH += $$PWD/../IOhelp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../CustomFrame/release/ -lcustomframe
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../CustomFrame/debug/ -lcustomframe

INCLUDEPATH += $$PWD/../CustomFrame
DEPENDPATH += $$PWD/../CustomFrame
