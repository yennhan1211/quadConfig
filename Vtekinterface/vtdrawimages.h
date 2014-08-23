#ifndef VTDRAWIMAGES_H
#define VTDRAWIMAGES_H
#include <QVariant>
#include "vtekinterface.h"

class vtdrawimages : public VtekInterface
{
public:
    explicit vtdrawimages(QObject* view);

public slots:
    QVariant getValue() const;
    void setValue(const QVariant &value);

};
#endif // VTDRAWIMAGES_H
