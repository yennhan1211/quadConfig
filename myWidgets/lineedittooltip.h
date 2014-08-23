#ifndef LINEEDITTOOLTIP_H
#define LINEEDITTOOLTIP_H

#include <QLabel>
#include <QTimer>
#include "mywidgets_global.h"

#define TOOLTIP_INTERVAL 250

class MYWIDGETSSHARED_EXPORT LineEditTooltip : public QLabel
{
    Q_OBJECT
public:
    explicit LineEditTooltip(QWidget *parent = 0);
    ~LineEditTooltip();
    void showMe();
    void hideMe();
    void setTimeAutoHide(int);
private:
    int m_tooltipCounter;
    QTimer m_tooltipTimer;
    int m_counter;
    int m_time_to_hide;
    bool m_autoHide_flag,m_showOrhide_flag;
private slots:
    void tooltipTimer_timeout();
};

#endif // LINEEDITTOOLTIP_H

