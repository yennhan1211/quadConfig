#include "counterlabel.h"
#include <QPainter>
#include <QDebug>
counterLabel::counterLabel(int x,int y,QWidget *parent) :
    QLabel(parent),m_init(false)
{
    this->setGeometry(x,y,60,12);
    m_counter = 0;
    m_totalcounter = 2;
    m_rad = this->height()/2 - 1 ;
    m_user_draw = false;
    m_counterDraw = 0;
}

counterLabel::~counterLabel()
{

}

void counterLabel::setCurrentCounter(int val)
{
    m_counter = val;
    m_counterDraw = m_counter;
    update();
}

void counterLabel::setTotalCounter(int valt)
{
    m_totalcounter = valt;
}

void counterLabel::m_draw()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.save();
    m_counterDraw = m_counter;
    for(int i = 0;i < m_totalcounter;i++){
        QPointF center(m_rad*(2*i+1), height()/2);
        if(i == m_counterDraw-1  ){

            painter.setBrush(Qt::white);
            painter.drawEllipse(center,m_rad-1,m_rad-1);
        }
        else {
            painter.setBrush(Qt::gray);
            painter.drawEllipse(center,m_rad-1,m_rad-1);
        }
    }

    painter.restore();
}

void counterLabel::paintEvent(QPaintEvent *)
{
        m_draw();
}

