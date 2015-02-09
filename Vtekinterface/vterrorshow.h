#ifndef VTERRORSHOW_H
#define VTERRORSHOW_H
#include <QVariant>
#include "vtekinterface.h"


class vtErrorShow : public VtekInterface
{
public:
   explicit vtErrorShow(QObject* view);
public slots:
    QVariant getValue() const;
    void setValue(const QVariant &value);
};

#endif // VTERRORSHOW_H
