#include "vtlineedit.h"
#include <QLineEdit>
#include <QIntValidator>
vtlineEdit::vtlineEdit(QObject *view)
{
    this->view = view;
}

QVariant vtlineEdit::getValue() const
{
    return -1;
}

void vtlineEdit::setValue(const QVariant &value)
{
    QLineEdit *btt = qobject_cast<QLineEdit*> (view);
    if(btt == NULL)return;
     const QIntValidator *bvt = qobject_cast<const QIntValidator* > (btt->validator());
     if(bvt == NULL)return;
     int tmp = value.toInt();
     if(bvt->bottom() == -50)tmp = tmp - 50;
     if(QString::number(tmp) != btt->text())
        btt->setText(QString::number(tmp));
}

