#ifndef VTEKINTERFACE_H
#define VTEKINTERFACE_H

#include "vtekinterface_global.h"
#include <QVariant>

class VTEKINTERFACESHARED_EXPORT VtekInterface
{
public:
    virtual ~VtekInterface() {}
    virtual QVariant getValue() const = 0;
    virtual void setValue(const QVariant &value) = 0;
    static VtekInterface *createVtekObject(QObject* view);

protected:
    QObject* view;

};

#endif // VTEKINTERFACE_H
