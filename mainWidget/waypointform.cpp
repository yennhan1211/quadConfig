#include "waypointform.h"
#include "ui_waypointform.h"
#include "mystring.h"
#include <QDesktopWidget>
wayPointForm::wayPointForm(QWidget *parent) :
    CustomFrame(parent,1),
    ui(new Ui::wayPointForm)
{
    ui->setupUi(this);
    QDesktopWidget qDesk;
    CustomFrame::setWindowSize(qDesk.width()-100,qDesk.height()-100);
    CustomFrame::setWindowTitleSize(qDesk.width()-100,30);
}

wayPointForm::~wayPointForm()
{
    delete ui;
}
