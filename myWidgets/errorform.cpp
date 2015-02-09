#include "errorform.h"
#include "ui_errorform.h"
#include <QDesktopWidget>
#include <QDebug>
#include <QPropertyAnimation>
#include <QFile>
errorForm::errorForm(QWidget *parent) :
    CustomFrame(parent,1),
    ui(new Ui::errorForm)
{
    ui->setupUi(this);

    CustomFrame::setWindowSize(250,150);
    CustomFrame::setWindowTitleSize(250,0);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint|Qt::WindowStaysOnTopHint |Qt::Tool);
    QFile m_File("doc/Aircraft_ErrorList.txt");
    if(m_File.exists()){
        m_File.open(QIODevice::ReadOnly);
        QString string(m_File.readAll());
        QStringList strList = string.split("\r\n");
        m_File.close();
        foreach (QString s, strList) {
            int _pos1 = s.indexOf("><");
            QString param1 = s.mid(1,_pos1-1);
            QString param2 = s.mid(_pos1+2,s.length() - _pos1-3);
            mErrorList.insert(param1.toInt(),param2);
            qDebug() << param1 << param2;
        }

    }
    isShow = false;
    connect(ui->btnHide,SIGNAL(clicked()),this,SLOT(SLOT_hideForm()));
}

errorForm::~errorForm()
{
    delete ui;
}

void errorForm::SLOT_hideForm()
{
         QDesktopWidget screen;
         QRect screenGeom = screen.geometry();
         QRect screenGeomA = screen.availableGeometry();
         int screenCenterX = screenGeom.width();
         int screenCenterY = screenGeom.height();

         qDebug() << "error form hide event" << screenCenterX << screenCenterY << this->x() << this->y();
         QPropertyAnimation *anim = new QPropertyAnimation(this, "pos");
         if(anim == NULL)return;
         anim->setKeyValueAt(0, QPointF(this->x(),this->y()));
         anim->setKeyValueAt(1, QPointF(this->x(),this->y() + this->height()*2 + screenGeom.height()+ 10 - screenGeomA.height() ));
         anim->setDuration(350);
         anim->start(QAbstractAnimation::DeleteWhenStopped);
         isShow = false;
}

void errorForm::SLOT_showForm(int _error)
{
    if(!_error){
        if(isShow)SLOT_hideForm();
        return;
    }
    QDesktopWidget screen;
    QRect screenGeom = screen.geometry();
    QRect screenGeomA = screen.availableGeometry();
    int screenCenterX = screenGeom.width();
    int screenCenterY = screenGeom.height();
    ui->teErrorDisplay->setText(mErrorList[_error]);
    move(screenCenterX - this->width ()-10,
         screenCenterY + this->height());
    qDebug() << "error form show event" << screenCenterX << screenCenterY;
    QPropertyAnimation *anim = new QPropertyAnimation(this, "pos");
    if(anim == NULL)return;
    anim->setKeyValueAt(0, QPointF(this->x(),this->y()));
    anim->setKeyValueAt(0.7, QPointF(this->x(),this->y() - this->height()*2 - screenGeom.height()-60+ screenGeomA.height() ));
    anim->setKeyValueAt(1, QPointF(this->x(),this->y() - this->height()*2 - screenGeom.height()-10+ screenGeomA.height() ));
    anim->setDuration(400);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
    isShow = true;
}

void errorForm::showEvent(QShowEvent *event)
{
    //Q_UNUSED(event);
    QWidget::showEvent(event);
    QDesktopWidget screen;
    QRect screenGeom = screen.geometry();
    int screenCenterX = screenGeom.width();
    int screenCenterY = screenGeom.height();
    move(screenCenterX - this->width ()-10,
         screenCenterY + this->height());
}
