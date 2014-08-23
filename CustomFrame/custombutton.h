#ifndef CUSTOMBUTON_H
#define CUSTOMBUTON_H

#include <QAbstractButton>

#define PADDING_X 10
#define PADDING_Y 5

class CustomButton : public QAbstractButton
{
    Q_OBJECT
public:
    enum ButtonType
    {
        BUTTON_MINIMIZE, BUTTON_MAXIMIZE, BUTTON_CLOSE
    };

    explicit CustomButton(CustomButton::ButtonType, QWidget *parent = 0);
    ~CustomButton();

protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent (QPaintEvent  *event);

    void enterEvent       (QEvent      *event);
    void leaveEvent       (QEvent      *event);
    void mousePressEvent  (QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:

public slots:

private:
    enum ButtonState
    {
        STATE_NORMAL, STATE_HOVERED, STATE_CLICKED
    };

    ButtonType  m_Type   ;
    ButtonState m_State  ;
    QPixmap    *m_Normal ;
    QPixmap    *m_Hovered;
    QPixmap    *m_Clicked;

    void InitPixmaps();
    void InitPixmap(QPixmap **pixmap);
    void InitMinimize();
    void InitMaximize();
    void InitClose();

};

#endif // CUSTOMBUTON_H
