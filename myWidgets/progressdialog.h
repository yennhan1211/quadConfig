#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>
#include "progressring.h"
#include "mywidgets_global.h"

namespace Ui {
class ProgressDialog;
}

class MYWIDGETSSHARED_EXPORT ProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProgressDialog(QWidget *parent = 0);
    ~ProgressDialog();
    void changeState(int);

    void showDialog();
    void hideDialog();

    QString title() const;
    void setTitle(const QString &title);

    QString content() const;
    void setContent(const QString &content);

    int progressBarValue();
public slots:
    void setProgressBarValue(int);

private:
    Ui::ProgressDialog *ui;
    ProgressRing* m_progressRing;
    QFrame *m_blurBackground;
    bool m_IsHide;
};

#endif // PROGRESSDIALOG_H
