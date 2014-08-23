#include "drawimages.h"
#include <QPainter>
#include <QDebug>

DrawImages::DrawImages(QWidget *parent) :
    QLabel(parent)
{
    m_currentAngle = 0;
    m_currentAngleLast = 0;
    m_pos = 0;
    m_posLast = 0;
    m_pixmap = NULL;
    m_pixmap1 = NULL;
    m_background_flag = false;
}

DrawImages::~DrawImages()
{
    if (m_pixmap != NULL) {
        delete m_pixmap;
    }
    if(m_pixmap1 != NULL)delete m_pixmap1;
}

void DrawImages::setPixmapUrl(const QString &url)
{
    m_pixmap = new QPixmap(url);
}

void DrawImages::setAngle(double angle)
{
    if(angle != m_currentAngleLast)
    {
    m_currentAngle = angle;
    if (m_currentAngle > 360) {
        m_currentAngle = m_currentAngle - 65535 ;
    }
   // emit SIGNAL_rollvalueChange(int(m_currentAngle));
    update();
    m_currentAngleLast = angle;
    }
}

void DrawImages::setRollPixmapUrl(const QString &url)
{
         m_pixmap = new QPixmap(url);
}

void DrawImages::setPitchPixmapUrl(const QString &url1)
{
    m_pixmap1 = new QPixmap(url1);
}

void DrawImages::setBackGroundImage(const QString &url2)
{
    this->setStyleSheet(
                "QLabel {background-image: url("+url2+");border:1px black;}"
                );
}

void DrawImages::setAngleRoll(int angle)
{
    if(angle != m_currentAngleLast)
    {
    m_currentAngle = angle;
    if (m_currentAngle > 180) {
        m_currentAngle = m_currentAngle%360;
        m_currentAngle = m_currentAngle - 360 ;
    }


    update();
    m_currentAngleLast = angle;
    }
     emit SIGNAL_rollvalueChange(int(m_currentAngle));
}

void DrawImages::setAnglePitch(int pitch)
{
    if(pitch != m_posLast)
    {
        m_pos = pitch;
        if (m_pos > 180) {
            m_pos = m_pos - 360;

        }


        m_posLast = pitch;
        update();
    }
     emit  SIGNAL_pitchvalueChange(m_pos);
}

void DrawImages::setDrawBackGround(bool b)
{
    m_background_flag = b;
}

void DrawImages::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::Antialiasing );
    painter.translate(m_pixmap->width() / 2, m_pixmap->height() / 2);
    painter.rotate(m_currentAngle);
    if(m_background_flag){
        QPoint start(0,-100);
        QPoint end(0,100);
        QLinearGradient gradient(start,end);
        double tmpd = (100 + (double)m_pos)/200;
        if(tmpd > 1)tmpd =0.9999;else if(tmpd < 0) tmpd =0.0001;
        tmpd = 1- tmpd;
        gradient.setColorAt(0,QColor("#1AD6FD"));
        gradient.setColorAt(tmpd,QColor("#1D62F0"));
        gradient.setColorAt(tmpd + 0.00001,QColor("#FF5E3A"));
        gradient.setColorAt(1, QColor("#d35400"));
        QBrush brush(gradient);
        painter.setBrush(brush);
        painter.setPen(Qt::NoPen);
        QPoint center(0,0);
        painter.drawEllipse(center,100,100);
    }
    painter.drawPixmap(m_pixmap->width() / -2, m_pixmap->height() / -2 , *m_pixmap);
    QPoint point(-m_pixmap1->width()/2,-height()/2 + 50);
    QRect rect(0,(530 - height())/2 - m_pos, m_pixmap1->width(),height() - 100);
    painter.drawPixmap(point, *m_pixmap1, rect);
    painter.restore();
}
