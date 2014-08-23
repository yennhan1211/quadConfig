#include "vtslider.h"
#include <QSlider>
VtSlider::VtSlider(QObject *view)
{
    this->view = view;
}

QVariant VtSlider::getValue() const
{
    QSlider *slider = qobject_cast<QSlider*> (view);
    int val = slider->value();
    if (slider->minimum() < 0) {
        val = val + 100;
    }
    return val;
}

void VtSlider::setValue(const QVariant &value)
{
    QSlider *slider = qobject_cast<QSlider*> (view);   
    int val = value.toInt() ;
    if (slider->minimum() < 0) {
        val = val - 100;
    }
    if (val >= slider->maximum() )val = slider->maximum();if (val <= slider->minimum() )val = slider->minimum();
    slider->setValue(val);

}
