#include "vtinfoform.h"
#include "ui_vtinfoform.h"
#include <QGraphicsOpacityEffect>
#include <QWidget>
vtinfoform::vtinfoform(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::vtinfoform)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setWindowModality(Qt::WindowModal);

    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect();
    effect->setOpacity(0.6);

    m_blurBackground = new QFrame(parent);
    m_blurBackground->setGeometry(0, 0, parent->width(), parent->height());
    m_blurBackground->setStyleSheet("QFrame {background:#191919;border:none;}");
    m_blurBackground->setGraphicsEffect(effect);
    m_blurBackground->hide();



    connect(ui->btnCloseInfoForm,SIGNAL(clicked()),this,SLOT(SLOT_hideForm()));
    this->hide();
}

vtinfoform::~vtinfoform()
{
    delete ui;
}

void vtinfoform::SLOT_hideForm()
{
    m_blurBackground->hide();
    this->hide();
}

void vtinfoform::SLOT_showForm()
{
    m_blurBackground->show();
    QWidget * parent = parentWidget();
    move(parent->x() + parent->width()/2 - width()/2,
         parent->y() + parent->height()/2 - height()/2);
    this->show();
}
