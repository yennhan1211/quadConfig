#ifndef VOLUMESLIDER_H
#define VOLUMESLIDER_H

#include <QWidget>
#include "mywidgets_global.h"
namespace Ui {
class volumeSlider;
}

class MYWIDGETSSHARED_EXPORT volumeSlider : public QWidget
{
    Q_OBJECT

public:
    explicit volumeSlider(QWidget *parent = 0);
    ~volumeSlider();

private:
    Ui::volumeSlider *ui;
protected:
    virtual void paintEvent(QPaintEvent*);
};

#endif // VOLUMESLIDER_H
