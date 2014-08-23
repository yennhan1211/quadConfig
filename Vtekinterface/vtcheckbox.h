#ifndef VTCHECKBOX_H
#define VTCHECKBOX_H
#include <QVariant>
#include "vtekinterface.h"

class vtcheckbox : public VtekInterface
{
public:
    explicit vtcheckbox(QObject* view);

public slots:
    QVariant getValue() const;
    void setValue(const QVariant &value);

};

#endif // VTCHECKBOX_H
