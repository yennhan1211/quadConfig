#ifndef ERRORFORM_H
#define ERRORFORM_H

#include <QWidget>
#include "customframe.h"
#include <QHash>
#include "mywidgets_global.h"
namespace Ui {
class errorForm;
}


class MYWIDGETSSHARED_EXPORT errorForm : public CustomFrame
{
    Q_OBJECT

public:
    explicit errorForm(QWidget *parent = 0);
    ~errorForm();
    QHash<int,QString> mErrorList;
public slots:
    void SLOT_hideForm();
    void SLOT_showForm(int _error);
private:
    Ui::errorForm *ui;
    bool isShow;
protected:
      void showEvent(QShowEvent *event);

};

#endif // ERRORFORM_H
