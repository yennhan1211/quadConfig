#include "vtcombobox.h"
#include <QDebug>
#include <QComboBox>

VtComboBox::VtComboBox(QObject *view)
{
    this->view = view;
}

QVariant VtComboBox::getValue() const
{
    QComboBox *cbBox = qobject_cast<QComboBox*> (view);
    return cbBox->currentData();
}

void VtComboBox::setValue(const QVariant &value)
{
    QComboBox *cbBox = qobject_cast<QComboBox*> (view);
    if(cbBox == NULL)return;
    int val = value.toInt();
    for (int i = 0; i < cbBox->count(); i++) {
        if (cbBox->itemData(i) == val) {

            cbBox->setCurrentIndex(i);

            return;
        }
    }
}
