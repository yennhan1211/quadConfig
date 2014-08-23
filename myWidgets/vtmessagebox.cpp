#include "vtmessagebox.h"
#include "ui_vtmessagebox.h"
#include "QPropertyAnimation"
vtmessagebox::vtmessagebox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::vtmessagebox)
{
    ui->setupUi(this);
    m_show_flag = false;
    connect(ui->btnNo,SIGNAL(clicked()),this,SLOT(SLOT_stateChange()));
    connect(ui->btnYes,SIGNAL(clicked()),this,SLOT(SLOT_stateChange()));
}

vtmessagebox::~vtmessagebox()
{
    delete ui;
}

void vtmessagebox::setMessage(QString mes)
{
    ui->lb_dis->setText(mes);
}

void vtmessagebox::showMe()
{
    if(!m_show_flag){
    QPropertyAnimation *anim = new QPropertyAnimation(this, "pos");
    anim->setKeyValueAt(0, QPointF(this->x(), this->y() ));
    anim->setKeyValueAt(0.7, QPointF(this->x() , this->y()-80));
    anim->setKeyValueAt(1, QPointF(this->x(), this->y()-60));
    anim->setDuration(200);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
    m_show_flag = true;
    }
}

void vtmessagebox::hideMe()
{
    if(m_show_flag){
    QPropertyAnimation *anim = new QPropertyAnimation(this, "pos");
    anim->setKeyValueAt(0, QPointF(this->x() , this->y()));
    anim->setKeyValueAt(1, QPointF(this->x(), this->y()+60));
    anim->setDuration(150);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
    m_show_flag = false;
    }
}

void vtmessagebox::SLOT_stateChange()
{
    if(sender() == ui->btnNo)emit SIGNAL_stateChange(0);
    else if(sender() == ui->btnYes)emit SIGNAL_stateChange(1);
}
