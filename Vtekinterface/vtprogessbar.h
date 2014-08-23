#ifndef VTPROGESSBAR_H
#define VTPROGESSBAR_H

#include <QVariant>
#include "vtekinterface.h"

class vtprogessbar : public VtekInterface
{
public:
    explicit vtprogessbar(QObject* view);

public slots:
    QVariant getValue() const;
    void setValue(const QVariant &value);

};

#endif // VTPROGESSBAR_H
