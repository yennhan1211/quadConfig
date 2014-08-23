#include "progressring.h"
#include <QPainter>

ProgressRing::ProgressRing(QWidget *parent) :
    QLabel(parent)
{
    m_speed = 1;
    m_currentAngle = 0;
    m_pixmap = NULL;

    m_rotateTimer.setSingleShot(false);
    connect(&m_rotateTimer, SIGNAL(timeout()),
            this, SLOT(rotateTimer_timeout()));
}

ProgressRing::~ProgressRing()
{
    if (m_pixmap != NULL) {
        delete m_pixmap;
    }
}

void ProgressRing::setPixmapUrl(const QString &url)
{
    m_pixmap = new QPixmap(url);
}

void ProgressRing::setSpeed(double speed)
{
    this->m_speed = speed;
}

void ProgressRing::start()
{
    if (!m_rotateTimer.isActive()) {
        m_rotateTimer.start(4);
    }
    QLabel::show();
}

void ProgressRing::stop()
{
    if (m_rotateTimer.isActive()) {
        m_rotateTimer.stop();
    }
    QLabel::hide();
}

void ProgressRing::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    painter.save();

    if (m_pixmap != NULL) {
        painter.translate(m_pixmap->width() / 2, m_pixmap->height() / 2);
        painter.rotate(m_currentAngle);
        painter.drawPixmap(m_pixmap->width()/-2, m_pixmap->height()/-2, *m_pixmap);
    }
    else {
        QPointF center(width()/2, height()/2);
        QConicalGradient gr(center, m_currentAngle);
        gr.setColorAt(0, QColor("#232323"));
        gr.setColorAt(1, QColor("#dedede"));
        painter.setBrush(gr);
        painter.drawEllipse(center, width()/2, height()/2);

        painter.setBrush(QColor("#f5f5f5"));
        painter.drawEllipse(center, width()/2 - 4, height()/2 - 4);
    }

    painter.restore();
}

void ProgressRing::rotateTimer_timeout()
{
    m_currentAngle += m_speed;
    if (m_currentAngle >= 360) {
        m_currentAngle = 0;
    }
    update();
}
