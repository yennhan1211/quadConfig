#include "vtcheckbox.h"
#include <QAbstractButton>
vtcheckbox::vtcheckbox(QObject *view)
{
    this->view = view;
}

QVariant vtcheckbox::getValue() const
{
    QAbstractButton *btt = qobject_cast<QAbstractButton*> (view);
    return (btt->isChecked() ? TOGGLE_BUTTON_NORMAL : TOGGLE_BUTTON_REVERSED);
}

void vtcheckbox::setValue(const QVariant &value)
{
    QAbstractButton *btt = qobject_cast<QAbstractButton*> (view);
    if(btt == NULL)return;
    if(value.toBool() != btt->isChecked()){
    if(!value.toBool())btt->setText("Norm");else btt->setText("Rev");
    btt->setChecked(value.toBool());
    }
}
