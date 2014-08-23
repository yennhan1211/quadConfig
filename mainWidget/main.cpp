#include "mainwidget.h"
#include <QApplication>
#include <QSplashScreen>
#include <mystring.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    QSplashScreen splash(
//                QPixmap(SPLASH_SCREENT_URL), Qt::WindowStaysOnTopHint);
//    splash.show();
//    QString startMes = START_MSG;
//    splash.showMessage(startMes,
//                       Qt::AlignLeft | Qt::AlignBottom, Qt::white);
    mainWidget w;
    w.setWindowIcon(QIcon(ICON_URL));

//    for(int i = 0;i < 40;i++){

//    if(i% 3== 0)startMes.replace(startMes.length()-1,1,47);
//    else if(i%3 == 1)startMes.replace(startMes.length()-1,1,45);
//    else if(i%3 == 2)startMes.replace(startMes.length()-1,1,92);
//    splash.showMessage(startMes,
//                       Qt::AlignLeft | Qt::AlignBottom, Qt::white);
//    QThread::currentThread()->msleep(50);
//    }

    w.show();
//    splash.finish(&w);
    return a.exec();
}
