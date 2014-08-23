#ifndef RXMONITORVIEW_H
#define RXMONITORVIEW_H

#include <QGraphicsView>
#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QPainter>
#include "mywidgets_global.h"


class SensorItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    Q_INTERFACES(QGraphicsItem)

public:
    explicit SensorItem(qint32 width, qint32 height)
    {
        m_width = width;
        m_height = height;
    }

protected:
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        Q_UNUSED(option);
        Q_UNUSED(widget);

        QRectF rect = boundingRect();
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(Qt::red));
        painter->drawEllipse(rect.x() + 2, rect.y() + 2,
                             rect.width() - 4, rect.height() - 4);
    }

    QRectF boundingRect() const
    {
        return QRectF(-m_width/2, -m_height/2, m_width, m_height);
    }

private:
    qint32 m_width, m_height;
};


class RxItem : public QGraphicsItem
{
public:
    explicit RxItem(qint32 parentWidth, qint32 parentHeight)
    {
        m_rxWidth = parentWidth;
        m_rxHeight = parentHeight;
    }

protected:
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        Q_UNUSED(option);
        Q_UNUSED(widget);

        QPen pen;
        pen.setWidth(1);
        pen.setColor(Qt::black);
        painter->setPen(pen);

        QVector<QLineF> lines;
        lines.append(QLineF(m_rxWidth/2, 0, m_rxWidth/2, m_rxHeight));
        lines.append(QLineF(0, m_rxHeight/2, m_rxWidth, m_rxHeight/2));

        qreal startX = m_rxWidth/2 - 5;
        qreal endX = m_rxWidth/2 + 5;
        qreal startY = m_rxHeight/2 - 5;
        qreal endY = m_rxHeight/2 + 5;

        for (qint8 i = 1; i <= 9; i++) {
            if (i == 5) continue;
            qreal dx = m_rxWidth/10 * i;
            qreal dy = m_rxHeight/10 * i;
            lines.append(QLineF(startX, dy, endX, dy));
            lines.append(QLineF(dx, startY, dx, endY));
        }

        painter->drawLines(lines);
    }

    QRectF boundingRect() const
    {
        return QRectF(0, 0, m_rxWidth, m_rxHeight);
    }

private:
    qint32 m_rxWidth, m_rxHeight;
};


class MYWIDGETSSHARED_EXPORT RxMonitorView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit RxMonitorView(qint32, qint32, qint32, qint32, QWidget *parent = 0);
    void setRangeValue(qint32, qint32);

    qreal x, y;
    bool need_move;

    bool animFinished() const;

    void setLocation(qint32 location);
    qint32 getLocation() const;

    void moveSensor();

signals:
    void _animationFinished();
    void _sensorValueChanged(int, int);

private slots:
    void handleAnimFinished();

private:
    SensorItem *sensor;
    RxItem *rx;
    QGraphicsScene *scene;

    bool m_animFinish;
    qint32 m_location;
    qint32 m_rxWidth, m_rxHeight;
    qint32 min, max;
};


#endif // RXMONITORVIEW_H
