#ifndef CUSTOMTITLEBAR_H
#define CUSTOMTITLEBAR_H

#include <QWidget>
#include <QLabel>
#include "custombutton.h"


class CustomTitlebar : public QWidget
{
    Q_OBJECT
public:
    explicit CustomTitlebar(QWidget *parent = 0);
    ~CustomTitlebar();

signals:

protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent (QPaintEvent  *event);

    void mousePressEvent (QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent (QMouseEvent *event);

public slots:
  void UpdateWindowTitle(const QString&);
  void Minimized();
  void Maximized();
  void Quit     ();

private:
    QPixmap *m_Cache;
    QLabel m_Title;
    CustomButton m_Minimize;
#ifdef FRAME_CAN_RESIZE
    CustomButton m_Maximize;
#endif
    CustomButton m_Close;
    QPoint m_Diff;

};

#endif // CUSTOMTITLEBAR_H
