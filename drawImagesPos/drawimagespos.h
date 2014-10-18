#ifndef DRAWIMAGESPOS_H
#define DRAWIMAGESPOS_H

#include "drawimagespos_global.h"
#include <QLabel>
#include <QPixmap>
#include <QWidget>
class DRAWIMAGESPOSSHARED_EXPORT DrawImagesPos :  public QLabel
{
    Q_OBJECT
public:
    explicit DrawImagesPos(QWidget *parent = 0);
    ~DrawImagesPos();
    void setPixmapUrl(const QString&);
    void setPitchAndAngle(int);
signals:
    void SIGNAL_valueChange(int);
public slots:

private:
    QPixmap *m_pixmap;
    int m_pos,m_posLast;
protected:
     void paintEvent(QPaintEvent*);
};


#endif // DRAWIMAGESPOS_H
