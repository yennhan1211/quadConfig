#include "volumeslider.h"
#include "ui_volumeslider.h"

volumeSlider::volumeSlider(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::volumeSlider)
{
    ui->setupUi(this);
}

volumeSlider::~volumeSlider()
{
    delete ui;
}

void volumeSlider::paintEvent(QPaintEvent *)
{

}
