#include "lineedittooltip.h"

LineEditTooltip::LineEditTooltip(QWidget *parent) :
    QLabel(parent)
{
    setStyleSheet("QLabel { background-image:url(images/tooltip.png);"
                    "padding-left:20px; color:#fefefe;padding-top:7px;"
                  "font-family:UTM Avo; font-size:13px;}");
    QLabel::hide();
     m_counter = 0;
     m_autoHide_flag = false;
     m_showOrhide_flag = false;
     m_time_to_hide = 0;
}

LineEditTooltip::~LineEditTooltip()
{
    if (m_tooltipTimer.isActive()) {
        m_tooltipTimer.stop();
    }
}

void LineEditTooltip::showMe()
{
    connect(&m_tooltipTimer, SIGNAL(timeout()),
            this, SLOT(tooltipTimer_timeout()));

    if (!m_tooltipTimer.isActive()) {
        m_tooltipTimer.start(TOOLTIP_INTERVAL);
    }
    show();
    m_showOrhide_flag = true;
}

void LineEditTooltip::hideMe()
{
    disconnect(&m_tooltipTimer, SIGNAL(timeout()), 0, 0);

    if (m_tooltipTimer.isActive()) {
        m_tooltipTimer.stop();
    }
    hide();
    m_showOrhide_flag = false;
}

void LineEditTooltip::setTimeAutoHide(int t)
{
    m_autoHide_flag = true;
    m_time_to_hide = t*1000;
}

void LineEditTooltip::tooltipTimer_timeout()
{
    if (m_tooltipCounter % 2 == 0) {
        setStyleSheet("QLabel { background-image:url(images/tooltip.png);"
                        "padding-left:20px; color:#fefefe;padding-top:7px;"
                        "font-family:UTM Avo; font-size:13px;}");
        update();
    }
    else {
        setStyleSheet("QLabel { background-image:url(images/tooltip.png);"
                        "padding-left:20px; color:#ff2b2b;padding-top:7px;"
                        "font-family:UTM Avo; font-size:13px;}");
        update();
    }

    if (m_tooltipCounter >= 10) {
        m_tooltipCounter = 1;
    }
    else {
        m_tooltipCounter ++;
    }

    if(m_autoHide_flag && m_showOrhide_flag){
        m_counter++;
        if(m_counter >= (m_time_to_hide/250)){
            m_counter = 0;
            hideMe();
        }
    }
}
