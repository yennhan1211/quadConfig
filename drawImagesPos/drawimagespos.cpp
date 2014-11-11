#include "drawimagespos.h"
#include <QPainter>
DrawImagesPos::DrawImagesPos(QWidget *parent) :
    QLabel(parent)
{
    m_pos = 0;
    m_posLast = 0;
    m_pixmap = NULL;
//    m_currentAngle = 0;
//    m_currentAngleLast =0;
}

DrawImagesPos::~DrawImagesPos()
{
    if (m_pixmap != NULL) {
        delete m_pixmap;
    }
}

void DrawImagesPos::setPixmapUrl(const QString &url)
{
    m_pixmap = new QPixmap(url);
}

void DrawImagesPos::setPitchAndAngle(int pitch)
{
    if(pitch != m_posLast)
    {
        m_pos = pitch;
        if (m_pos > 360) {
            m_pos = m_pos - 65535;
        }
        SIGNAL_valueChange(m_pos);
        m_posLast = pitch;
           update();
    }
//    if(angle != m_currentAngleLast)
//    {
//        m_currentAngle = angle;
//        if (m_currentAngle > 360) {
//            m_currentAngle = m_currentAngle - 65535 ;
//        }
//        m_currentAngleLast = angle;
//    }

}

void DrawImagesPos::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
//    painter.translate(width() / 2,height() / 2);
//    painter.rotate(m_currentAngle);
    QPoint point(width()/2 - m_pixmap->width()/2 + 2,50);
    QRect rect(0,(530 - height())/2 + m_pos, m_pixmap->width(),height() - 100);
    painter.drawPixmap(point, *m_pixmap, rect);
    painter.restore();

}
