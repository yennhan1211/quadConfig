#-------------------------------------------------
#
# Project created by QtCreator 2014-05-17T11:22:18
#
#-------------------------------------------------

QT       += widgets

TARGET = ivtek
TEMPLATE = lib

DEFINES += VTEKINTERFACE_LIBRARY

SOURCES += vtekinterface.cpp \
    vttogglebutton.cpp \
    vtekbuttongroup.cpp \
    vtslider.cpp \
    vtcombobox.cpp \
    vtdrawimages.cpp \
    vtcheckbox.cpp \
    vtprogessbar.cpp \
    vtlabel.cpp \
    vtrxmonitorview.cpp \
    vtlineedit.cpp \
    vterrorshow.cpp

HEADERS += vtekinterface.h\
        vtekinterface_global.h \
    vttogglebutton.h \
    vtekbuttongroup.h \
    vtslider.h \
    vtcombobox.h \
    vtdrawimages.h \
    vtcheckbox.h \
    vtprogessbar.h \
    vtlabel.h \
    vtrxmonitorview.h \
    vtlineedit.h \
    vterrorshow.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}



win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../drawImages/release/ -ldrawImages
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../drawImages/debug/ -ldrawImages
else:unix: LIBS += -L$$OUT_PWD/../drawImages/ -ldrawImages

INCLUDEPATH += $$PWD/../drawImages
DEPENDPATH += $$PWD/../drawImages



win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../myWidgets/release/ -lmyWidgets
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../myWidgets/debug/ -lmyWidgets
else:unix: LIBS += -L$$OUT_PWD/../myWidgets/ -lmyWidgets

INCLUDEPATH += $$PWD/../myWidgets
DEPENDPATH += $$PWD/../myWidgets

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../CustomFrame/release/ -lcustomframe
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../CustomFrame/debug/ -lcustomframe

INCLUDEPATH += $$PWD/../CustomFrame
DEPENDPATH += $$PWD/../CustomFrame
