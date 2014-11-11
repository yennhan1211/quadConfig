#include "volumelabel.h"
#include <math.h>
volumeLabel::volumeLabel(QObject *parent) :
    QLabel(parent)
{

    m_step_width = this->width()*3/80;// 20 step

}

volumeLabel::~volumeLabel()
{

}

void volumeLabel::setVolValue(int val)
{
    if(val > 100)m_percent = 100;
    else if(val < 0)m_percent = 0;
    else m_percent = val;
}

void volumeLabel::paintEvent(QPaintEvent *)
{
    int tmp_int = m_percent/5;
    QPolygonF m_poly;
}
