#-------------------------------------------------
#
# Project created by QtCreator 2014-06-07T14:26:14
#
#-------------------------------------------------

QT       += core gui\
            serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Quad
TEMPLATE = app


SOURCES += main.cpp\
        mainwidget.cpp \
    myserialport.cpp \
    serialread.cpp \
    serialWrite.cpp \
    flashhelper.cpp

HEADERS  += mainwidget.h \
    myserialport.h \
    addrdefine.h \
    serialread.h \
    serialWrite.h \
    mystring.h \
    flashhelper.h

FORMS    += mainwidget.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../CustomFrame/release/ -lcustomframe
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../CustomFrame/debug/ -lcustomframe
else:unix: LIBS += -L$$OUT_PWD/../CustomFrame/ -lcustomframe

INCLUDEPATH += $$PWD/../CustomFrame
DEPENDPATH += $$PWD/../CustomFrame

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Vtekinterface/release/ -livtek
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Vtekinterface/debug/ -livtek
else:unix: LIBS += -L$$OUT_PWD/../Vtekinterface/ -livtek

INCLUDEPATH += $$PWD/../Vtekinterface
DEPENDPATH += $$PWD/../Vtekinterface

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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../IOhelp/release/ -lIOhelp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../IOhelp/debug/ -lIOhelp
else:unix: LIBS += -L$$OUT_PWD/../IOhelp/ -lIOhelp

INCLUDEPATH += $$PWD/../IOhelp
DEPENDPATH += $$PWD/../IOhelp

RESOURCES += \
    appicon.qrc


RC_ICONS = Apollo.ico
