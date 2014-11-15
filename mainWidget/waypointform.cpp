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
//    m_webView = new QWebView(this);
//    m_webView->setGeometry(0,0,this->width(),this->height());
//    m_webView->setUrl(QUrl("file:///E:/Dropbox/VtekCo/Project/Quad2/build-Quad2-Desktop_Qt_5_3_0_MinGW_32bit/mainWidget/firstExample.html"));
}

wayPointForm::~wayPointForm()
{
    delete ui;
}
