#include "vtdrawimages.h"
#include <drawimages.h>

vtdrawimages::vtdrawimages(QObject *view)
{
    this->view = view;
}

QVariant vtdrawimages::getValue() const
{

}

void vtdrawimages::setValue(const QVariant &value)
{
    DrawImages *dig = qobject_cast<DrawImages*> (view);
    int tmpint = value.toInt();
    int key = tmpint >> 16;
    int val = tmpint & 0xFFFF;

    if (key == 501 || key == 503) {
        dig->setAngleRoll(val);
    }
    else if (key == 502) {
        dig->setAnglePitch(val);
    }
    else return;

}
