#include "vtinfoform.h"
#include "ui_vtinfoform.h"
#include <QGraphicsOpacityEffect>
#include <QWidget>
#include <QPainter>
#include <QKeyEvent>

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


    QPixmap tmppix1("images/background.png");
    //tmppix.fill(Qt::red);
    ui->cbSkin->addItem(QIcon(tmppix1),"Blue");
    QPixmap tmppix("images/background_dark.png");
    //tmppix.fill(Qt::red);
    ui->cbSkin->addItem(QIcon(tmppix),"Dark");
    connect(ui->btnCloseInfoForm,SIGNAL(clicked()),this,SLOT(SLOT_hideForm()));
    connect(ui->cbSkin,SIGNAL(currentIndexChanged(int)),this,SLOT(SLOT_cbSkinIndexChange(int)));
    this->hide();
}

vtinfoform::~vtinfoform()
{
    delete ui;
}

void vtinfoform::setCurrentSkin(int index)
{
    ui->cbSkin->setCurrentIndex(index);
}

void vtinfoform::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Escape)return;
}

void vtinfoform::SLOT_cbSkinIndexChange(int index)
{
    emit SIGNAL_SkinChange(index);
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
