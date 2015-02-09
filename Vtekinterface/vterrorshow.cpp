#include "vterrorshow.h"
#include <errorform.h>
#include <QDebug>
vtErrorShow::vtErrorShow(QObject *view)
{
    this->view = view;
}

QVariant vtErrorShow::getValue() const
{

}

void vtErrorShow::setValue(const QVariant &value)
{
    errorForm *err = qobject_cast<errorForm*> (view);
    qDebug() << "error code :" << value.toInt();
    err->SLOT_showForm(value.toInt());
}
