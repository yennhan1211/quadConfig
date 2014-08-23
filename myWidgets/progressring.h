#ifndef PROGRESSRING_H
#define PROGRESSRING_H

#include <QLabel>
#include <QPixmap>
#include <QTimer>
#include <QWidget>
#include "mywidgets_global.h"

class MYWIDGETSSHARED_EXPORT ProgressRing : public QLabel
{
    Q_OBJECT
public:
    explicit ProgressRing(QWidget *parent = 0);
    ~ProgressRing();

    void setPixmapUrl(const QString&);
    void setSpeed(double);

public slots:
    void start();
    void stop();

signals:

protected:
    virtual void paintEvent(QPaintEvent*);

    QPixmap *m_pixmap;
    double m_currentAngle;
    double m_speed;
    QTimer m_rotateTimer;

protected slots:
    virtual void rotateTimer_timeout();
};

#endif // PROGRESSRING_H
