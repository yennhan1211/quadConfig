#ifndef VTLABEL_H
#define VTLABEL_H

#include <QVariant>
#include "vtekinterface.h"

class vtlabel : public VtekInterface
{
public:
    explicit vtlabel(QObject* view);

public slots:
    QVariant getValue() const;
    void setValue(const QVariant &value);

};


#endif // VTLABEL_H
