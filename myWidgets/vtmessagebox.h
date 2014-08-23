#ifndef VTMESSAGEBOX_H
#define VTMESSAGEBOX_H

#include <QWidget>
#include "mywidgets_global.h"

namespace Ui {
class vtmessagebox;
}

class MYWIDGETSSHARED_EXPORT vtmessagebox : public QWidget
{
    Q_OBJECT

public:
    explicit vtmessagebox(QWidget *parent = 0);
    ~vtmessagebox();
    void setMessage(QString);
    void setMove(int x,int y);
    void showMe();
    void hideMe();
private:
    Ui::vtmessagebox *ui;
    bool m_show_flag;
signals:
    void SIGNAL_stateChange(int);
private slots:
    void SLOT_stateChange();
};

#endif // VTMESSAGEBOX_H
