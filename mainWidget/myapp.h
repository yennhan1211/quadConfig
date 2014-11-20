#ifndef MYAPP_H
#define MYAPP_H

#include <QApplication>

class myApp : public QApplication
{
public:
    myApp(int argc, char *argv[]) : QApplication(argc, argv){}
protected:
    bool virtual notify(QObject *, QEvent *);
};

#endif // MYAPP_H
