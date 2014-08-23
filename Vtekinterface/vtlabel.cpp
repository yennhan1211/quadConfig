#include "vtlabel.h"
#include <QLabel>
#include <QDebug>
vtlabel::vtlabel(QObject *view)
{
    this->view = view;
}

QVariant vtlabel::getValue() const
{
    return -1;
}

void vtlabel::setValue(const QVariant &value)
{
    QLabel *btt = qobject_cast<QLabel*> (view);
    if(value.toString() != btt->text())
        btt->setText(value.toString());
}
