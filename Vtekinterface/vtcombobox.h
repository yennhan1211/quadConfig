#ifndef VTCOMBOBOX_H
#define VTCOMBOBOX_H

#include <QVariant>
#include "vtekinterface.h"

class VtComboBox : public VtekInterface
{
public:
    explicit VtComboBox(QObject* view);

public slots:
    QVariant getValue() const;
    void setValue(const QVariant &value);

};

#endif // VTCOMBOBOX_H
