#include "vtrxmonitorview.h"
#include <rxmonitorview.h>
#include <QDebug>
VtRxMonitorView::VtRxMonitorView(QObject *view)
{
    this->view = view;
}

QVariant VtRxMonitorView::getValue() const
{
    return 0;
}

QVariant VtRxMonitorView::getValue(int key) const
{
    return key;
}

void VtRxMonitorView::setValue(const QVariant &value)
{
    RxMonitorView *rx = qobject_cast<RxMonitorView*> (view);
    int tmpint = value.toInt();
    int key = tmpint >> 16;
    int val = tmpint & 0xFFFF;

    if (key == 111 || key == 114) {
        rx->x = val;
    }
    else if (key == 112 || key == 113) {
        rx->y = val;
    }
    else return;

    if (rx->need_move) {
        rx->moveSensor();
    }
    else {
        rx->need_move = true;
    }
}
