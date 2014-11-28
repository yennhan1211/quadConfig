#include "progressdialog.h"
#include "ui_progressdialog.h"
#include "strings.h"
#include <QGraphicsOpacityEffect>
#include <QKeyEvent>

ProgressDialog::ProgressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProgressDialog)
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

    m_progressRing = new ProgressRing(ui->stackedWidget->widget(0));
    m_progressRing->setGeometry(15, 12, 32, 32);
    m_progressRing->start();

    m_IsHide = true;
}

ProgressDialog::~ProgressDialog()
{
    delete ui;
}

void ProgressDialog::changeState(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
    if (index == 0) {
        ui->lbTitle->setText("Please wait...");
    }
    else if (index == 0) {
        ui->lbTitle->setText(ui->lbContent->text());
        ui->progressBar->setValue(0);
    }
}

void ProgressDialog::showDialog()
{
    if(m_IsHide){
    m_blurBackground->show();
    QWidget * parent = parentWidget();
    move(parent->x() + parent->width()/2 - width()/2,
         parent->y() + parent->height()/2 - height()/2);
    show();
    m_IsHide = false;
    }
}

void ProgressDialog::hideDialog()
{
    if(!m_IsHide){
    m_blurBackground->hide();
    hide();
    m_IsHide = true;
    }
}
QString ProgressDialog::title() const
{
    return ui->lbTitle->text();
}

void ProgressDialog::setTitle(const QString &title)
{
    ui->lbTitle->setText(title);
}
QString ProgressDialog::content() const
{
    return ui->lbContent->text();
}

void ProgressDialog::setContent(const QString &content)
{
    ui->lbContent->setText(content);
}

int ProgressDialog::progressBarValue()
{
    return ui->progressBar->value();
}

void ProgressDialog::setProgressBarValue(int val)
{
    ui->progressBar->setValue(val);
}

void ProgressDialog::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Escape)return;
}
