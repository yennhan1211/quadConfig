#include "vtekbuttongroup.h"

#include <QButtonGroup>
#include <QAbstractButton>
#include <QDebug>
VtButtonGroup::VtButtonGroup(QObject *view)
{
    this->view = view;
}

QVariant VtButtonGroup::getValue() const
{
    QButtonGroup *bg = qobject_cast<QButtonGroup*> (view);
    return bg->checkedId();
}

void VtButtonGroup::setValue(const QVariant &value)
{
    QButtonGroup *bg = qobject_cast<QButtonGroup*> (view);
    int id = value.toInt();
    QAbstractButton *btt = bg->button(id);
    if (btt != NULL) {
        if(!btt->isChecked()){
        btt->setChecked(true);
      //  qDebug() << btt;
        }
    }
}
