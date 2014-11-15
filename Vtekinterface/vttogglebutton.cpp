#include "vttogglebutton.h"
#include "vtekinterface_global.h"
#include <QAbstractButton>

VtToggleButton::VtToggleButton(QObject *view)
{
    this->view = view;
}

QVariant VtToggleButton::getValue() const
{
    QAbstractButton *btt = qobject_cast<QAbstractButton*> (view);
    return (btt->isChecked() ? TOGGLE_BUTTON_NORMAL : TOGGLE_BUTTON_REVERSED);
}

void VtToggleButton::setValue(const QVariant &value)
{
    QAbstractButton *btt = qobject_cast<QAbstractButton*> (view);   
    if(btt == NULL)return;
    btt->setChecked(value.toBool());
}
