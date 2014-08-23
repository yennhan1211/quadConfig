#ifndef VTLINEEDIT_H
#define VTLINEEDIT_H

#include <QVariant>
#include "vtekinterface.h"

class vtlineEdit : public VtekInterface
{
public:
    explicit vtlineEdit(QObject* view);

public slots:
    QVariant getValue() const;
    void setValue(const QVariant &value);

};

#endif // VTLINEEDIT_H
