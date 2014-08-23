
#include <QPropertyAnimation>

#include "rxmonitorview.h"
#include "math.h"

RxMonitorView::RxMonitorView(qint32 width, qint32 height,
                             qint32 sensorWidth, qint32 sensorHeight,
                             QWidget *parent)
    : QGraphicsView(parent),
      m_animFinish(true)
{
    m_rxWidth = width;
    m_rxHeight = height;

    setFixedSize(m_rxWidth + 10, m_rxHeight + 10);

    setRenderHint(QPainter::Antialiasing, true);
    setDragMode(RubberBandDrag);
    setOptimizationFlags(DontSavePainterState);
    setViewportUpdateMode(SmartViewportUpdate);
    setTransformationAnchor(AnchorUnderMouse);
    setInteractive(false);
    rx = new RxItem(m_rxWidth, m_rxHeight);
    rx->setPos(5, 5);
    sensor = new SensorItem(sensorWidth, sensorHeight);
    sensor->setPos(5 + m_rxWidth/2, 5 + m_rxHeight/2);

    scene = new QGraphicsScene;
    scene->addItem(rx);
    scene->addItem(sensor);

    setScene(scene);
}

void RxMonitorView::moveSensor()
{
    need_move = false;
    emit _sensorValueChanged((qint32) x, (qint32) y);

    if ((x + y < 2*max) && (x + y > 2*min)) {
        qreal dx = (m_location == RX_LOCATION_RIGHT)
                ? (x - min) / (max - min) * m_rxWidth
                : (max - x) / (max - min) * m_rxWidth;
        qreal dy = (max - y) / (max - min) * m_rxHeight;

        dx += 5;
        dy += 5;

        qint32 duration = 0;
        qreal distance = sqrt(pow(sensor->pos().x() - dx, 2)
                + pow(sensor->pos().y() - dy, 2));
        duration = (distance / m_rxWidth) * 100;

        m_animFinish = false;

        QPropertyAnimation *animation = new QPropertyAnimation(sensor, "pos");
        connect(animation, SIGNAL(finished()), this, SLOT(handleAnimFinished()));

        animation->setDuration(duration);
        animation->setStartValue(sensor->pos());
        animation->setEndValue(QPointF(dx, dy));
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void RxMonitorView::handleAnimFinished()
{
    m_animFinish = true;
}

void RxMonitorView::setLocation(int location)
{
    m_location = location;
}

qint32 RxMonitorView::getLocation() const
{
    return m_location;
}

bool RxMonitorView::animFinished() const
{
    return m_animFinish;
}

void RxMonitorView::setRangeValue(qint32 min, qint32 max)
{
    this->min = min;
    this->max = max;
    x = y = (max - min) / 2 + min;
    need_move = false;
}





