#ifndef VTDRAWSENSOR_H
#define VTDRAWSENSOR_H

#include <QVariant>
#include "vtekinterface.h"

class vtdrawsensor : public VtekInterface
{
public:
    explicit vtdrawsensor(QObject* view);

public slots:
    QVariant getValue() const;
    void setValue(const QVariant &value);

};
#endif // VTDRAWSENSOR_H
