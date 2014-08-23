#ifndef VTTOGGLEBUTTON_H
#define VTTOGGLEBUTTON_H

#include <QVariant>
#include "vtekinterface.h"

class VtToggleButton : public VtekInterface
{
public:
    explicit VtToggleButton(QObject* view);

public slots:
    QVariant getValue() const;
    void setValue(const QVariant &value);

};

#endif // VTTOGGLEBUTTON_H
