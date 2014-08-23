#ifndef DRAWIMAGES_H
#define DRAWIMAGES_H

#include "drawimages_global.h"
#include <QLabel>
#include <QPixmap>
#include <QWidget>
class DRAWIMAGESSHARED_EXPORT DrawImages: public QLabel
{


    Q_OBJECT
public:
    explicit DrawImages(QWidget *parent = 0);
    ~DrawImages();
    void setPixmapUrl(const QString&);
    void setAngle(double);
    void setRollPixmapUrl(const QString&);
    void setPitchPixmapUrl(const QString&);
    void setBackGroundImage(const QString&);
    void setAngleRoll(int);
    void setAnglePitch(int);
    void setDrawBackGround(bool);
signals:
    void SIGNAL_rollvalueChange(int);
    void SIGNAL_pitchvalueChange(int);
public slots:

private:
    QPixmap *m_pixmap;
    QPixmap *m_pixmap1;
    int m_currentAngle;
    int m_currentAngleLast;
    int m_pos;
    int m_posLast;
    bool m_background_flag;
protected:
     void paintEvent(QPaintEvent*);

};

#endif // DRAWIMAGES_H
