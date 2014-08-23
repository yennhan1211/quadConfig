#include "vtprogessbar.h"
#include    <QProgressBar>


vtprogessbar::vtprogessbar(QObject *view)
{
    this->view = view;
}

QVariant vtprogessbar::getValue() const
{
    return -1;
}

void vtprogessbar::setValue(const QVariant &value)
{
    QProgressBar *slider = qobject_cast<QProgressBar*> (view);
    int val = value.toInt();
    if(val != slider->value() && val >=0)
    {
        slider->setValue(val);
    }
}
