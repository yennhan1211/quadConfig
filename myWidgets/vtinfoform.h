#ifndef VTINFOFORM_H
#define VTINFOFORM_H


#include "mywidgets_global.h"
#include <QFrame>
#include <QDialog>
#include <QProcess>
namespace Ui {
class vtinfoform;
}

 class MYWIDGETSSHARED_EXPORT vtinfoform : public QDialog
{
    Q_OBJECT

public:
    explicit vtinfoform(QWidget *parent = 0);
    ~vtinfoform();
    void setCurrentSkin(int);
private:
    Ui::vtinfoform *ui;
    QFrame *m_blurBackground;
    QProcess    *mpro ;
    QString mPath;
    int mState;
 protected:
    void keyPressEvent(QKeyEvent * e);
 signals:
    void SIGNAL_SkinChange(int);
    void SIGNAL_checkUpdateSoft(QString);
    void SIGNAL_getUpdateSoft();
    void SIGNAL_changeBackGroundDownload(int);
 public slots:
    void SLOT_cbSkinIndexChange(int);
    void SLOT_hideForm();
    void SLOT_showForm();
    void SLOT_checkUpdate();
    void SLOT_changeUpdateStatus(int);
    void SLOT_getRunPath(QString);
    void SLOT_RunProcess();
    void SLOT_setText();
};

#endif // VTINFOFORM_H
