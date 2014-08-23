#ifndef VTRXMONITORVIEW_H
#define VTRXMONITORVIEW_H

#include <QVariant>
#include "vtekinterface.h"

class VtRxMonitorView : public VtekInterface
{
public:
    explicit VtRxMonitorView(QObject *view);

public slots:
    QVariant getValue() const;
    QVariant getValue(int key) const;
    void setValue(const QVariant &value);

};

#endif // VTRXMONITORVIEW_H
