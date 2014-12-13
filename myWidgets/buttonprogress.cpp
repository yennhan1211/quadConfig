#include "buttonprogress.h"
#include "ui_buttonprogress.h"
#include <QPainter>
#include <QConicalGradient>
#include <QDebug>
#include <sslclient.h>


#define  PIX_PATH "images/info.png"

buttonProgress::buttonProgress(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::buttonProgress)
{
    ui->setupUi(this);
    connect(ui->btnMybutton,SIGNAL(clicked()),this,SIGNAL(btn_Click()));
    connect(&mBlinkTimer,SIGNAL(timeout()),this,SLOT(SLOT_timeOut()));
    mPixmap = new QPixmap(PIX_PATH);
    mState = buttonProgress::Normal;
    mBlinkTimer.setInterval(800);
    mBlinkTimer.start();
    ui->btnMybutton->installEventFilter(this);
    mx = 0;my = 0;
    mBlinkState = true;
    mPercent = 100;
}

buttonProgress::~buttonProgress()
{
    if(mPixmap != NULL)delete mPixmap;
    delete ui;
}

void buttonProgress::SLOT_percentChange(int val)
{
    mPercent = val;
    if(mPercent > 100)mPercent = 100;else if(mPercent < 0)mPercent = 0;
    if(mPercent >= 100)mState = buttonProgress::DownloadFinish;
    update();
}

void buttonProgress::SLOT_changeBackground(int val)
{
    mState = val;
    update();
}

void buttonProgress::SLOT_timeOut()
{
    if(mState == buttonProgress::DownloadFinish || mState == buttonProgress::NewSoft){
        mBlinkState = !mBlinkState;
        update();
    }
}

void buttonProgress::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::Antialiasing);

    if(mState == buttonProgress::Downloading)
    {
            painter.save();
            painter.setPen(Qt::NoPen);        QPointF center(width()/2, height()/2);
            QConicalGradient gr(center, 360);
            double x = 0,y = 0;
            if(mPercent < 10){x = this->width()/2.3529; y = this->height()/1.6667;}
            else if(mPercent >= 10 && mPercent < 100){x = this->width()/2.8571; y = this->height()/1.6667;}
            else if(mPercent == 100){x = this->width()/4; y = this->height()/1.6667;}
            float fper = (float)mPercent / 100;
            if(fper == 1)fper = 0.9998;
            gr.setColorAt(0, Qt::white);
            gr.setColorAt(fper,Qt::white);
            gr.setColorAt(fper+ 0.0001,Qt::transparent);
            gr.setColorAt(1,Qt::transparent);
            painter.setBrush(gr);
            painter.drawEllipse(center, width()/2, height()/2);
            gr.setColorAt(0, QColor("#34495e"));
            gr.setColorAt(fper,QColor("#34495e"));
            gr.setColorAt(fper+ 0.0001,Qt::transparent);
            gr.setColorAt(1,Qt::transparent);
            painter.setBrush(gr);
            painter.drawEllipse(center, width()/2 - 4, height()/2 - 4);
            painter.restore();
            painter.save();
            QPen pen;
            pen.setWidth(1);
            painter.setFont(QFont("Segoe UI", 8));
            pen.setColor(Qt::white);
            painter.setPen(pen);
            painter.translate(x,y);
            painter.drawText(0,0,  QString::number(mPercent));
            painter.restore();
    }
    if(mState == buttonProgress::DownloadFinish)
    {       painter.translate(mx,my);
            painter.save();
            painter.setPen(Qt::NoPen);        QPointF center(width()/2, height()/2);
            QConicalGradient gr(center, 360);
            double x = 0,y = 0;
            if(mPercent < 10){x = this->width()/2.3529; y = this->height()/1.6667;}
            else if(mPercent >= 10 && mPercent < 100){x = this->width()/2.8571; y = this->height()/1.6667;}
            else if(mPercent == 100){x = this->width()/4; y = this->height()/1.6667;}
            float fper = (float)mPercent / 100;
            if(fper == 1)fper = 0.9998;
            gr.setColorAt(0, Qt::white);
            gr.setColorAt(fper,Qt::white);
            gr.setColorAt(fper+ 0.0001,Qt::transparent);
            gr.setColorAt(1,Qt::transparent);
            painter.setBrush(gr);
            painter.drawEllipse(center, width()/2, height()/2);
            gr.setColorAt(0, QColor("#34495e"));
            gr.setColorAt(fper,QColor("#34495e"));
            gr.setColorAt(fper+ 0.0001,Qt::transparent);
            gr.setColorAt(1,Qt::transparent);
            painter.setBrush(gr);
            painter.drawEllipse(center, width()/2 - 4, height()/2 - 4);
            painter.restore();
            painter.save();
            QPen pen;
            pen.setWidth(1);
            painter.setFont(QFont("Segoe UI", 8));
            if(mBlinkState)
            pen.setColor(Qt::white);
            else pen.setColor(QColor("#34495e"));
            painter.setPen(pen);
            painter.translate(x,y);
            painter.drawText(0,0,  QString::number(mPercent));
            painter.restore();
    }
    else if(mState == buttonProgress::Normal)
    {
            painter.drawPixmap(mx,my,*mPixmap);
    }
}

bool buttonProgress::eventFilter(QObject *obj, QEvent *e)
{
    QString strtmp(obj->metaObject()->className());
    if(strtmp =="QPushButton" || strtmp =="QRadioButton" ){
    if(e->type() == QEvent::HoverEnter){
            mx = 1;my = 1;
            update();
        }
    else if(e->type() == QEvent::HoverLeave){
            mx = 0;my = 0;
            update();
        }
    }
    return false;
}
