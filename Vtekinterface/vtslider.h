#ifndef VTSLIDER_H
#define VTSLIDER_H

#include <QVariant>
#include "vtekinterface.h"

class VtSlider : public VtekInterface
{
public:
    explicit VtSlider(QObject* view);

public slots:
    QVariant getValue() const;
    void setValue(const QVariant &value);

};

#endif // VTSLIDER_H
