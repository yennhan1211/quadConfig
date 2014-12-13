#ifndef BUTTONPROGRESS_H
#define BUTTONPROGRESS_H

#include <QWidget>
#include "mywidgets_global.h"
#include <QPixmap>
#include <QTimer>

namespace Ui {
class buttonProgress;
}

class MYWIDGETSSHARED_EXPORT buttonProgress : public QWidget
{
    Q_OBJECT


public:
    enum State{
        NewSoft = 1,
        Downloading = 2,
        DownloadFinish =3,
        Normal =4
    };
    explicit buttonProgress(QWidget *parent = 0);
    ~buttonProgress();
public slots:
    void SLOT_percentChange(int);
    void SLOT_changeBackground(int);
    void SLOT_timeOut();
signals:
    void btn_Click();
private:
    Ui::buttonProgress *ui;
    QPixmap *mPixmap;
    QTimer  mBlinkTimer;
    int mState,mx,my ;
    int mPercent;
    bool mBlinkState ;
protected:
    virtual void paintEvent(QPaintEvent*);
    virtual bool eventFilter(QObject *, QEvent *);
};

#endif // BUTTONPROGRESS_H
