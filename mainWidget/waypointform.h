#ifndef WAYPOINTFORM_H
#define WAYPOINTFORM_H

#include <QWidget>
#include <customframe.h>
namespace Ui {
class wayPointForm;
}

class wayPointForm : public CustomFrame
{
    Q_OBJECT

public:
    explicit wayPointForm(QWidget *parent = 0);
    ~wayPointForm();

private:
    Ui::wayPointForm *ui;
};

#endif // WAYPOINTFORM_H
