#ifndef COUNTERLABEL_H
#define COUNTERLABEL_H

#include <QLabel>
#include <QWidget>
#include "mywidgets_global.h"

class MYWIDGETSSHARED_EXPORT counterLabel : public QLabel
{
    Q_OBJECT
public:
    explicit counterLabel(int x,int y,QWidget *parent = 0);
    ~counterLabel();
    void setCurrentCounter(int);
    void setTotalCounter(int);
   // void setCircleRad(int);
private:
    int m_counter;
    int m_totalcounter;
    int m_rad;
    int m_counterDraw;
    bool m_user_draw;
    bool m_init;
    void m_draw();
signals:

public slots:

protected:
    virtual void paintEvent(QPaintEvent*);

};

#endif // COUNTERLABEL_H
