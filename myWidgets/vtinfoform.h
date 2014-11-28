#ifndef VTINFOFORM_H
#define VTINFOFORM_H


#include "mywidgets_global.h"
#include <QFrame>
#include <QDialog>

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
 protected:
    void keyPressEvent(QKeyEvent * e);
 signals:
    void SIGNAL_SkinChange(int);
 public slots:
    void SLOT_cbSkinIndexChange(int);
    void SLOT_hideForm();
    void SLOT_showForm();
};

#endif // VTINFOFORM_H
