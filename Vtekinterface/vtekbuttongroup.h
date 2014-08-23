#ifndef VTEKBUTTONGROUP_H
#define VTEKBUTTONGROUP_H

#include <QVariant>
#include "vtekinterface.h"

class VtButtonGroup : public VtekInterface
{
public:
    explicit VtButtonGroup(QObject* view);

public slots:
    QVariant getValue() const;
    void setValue(const QVariant &value);

};

#endif // VTEKBUTTONGROUP_H
