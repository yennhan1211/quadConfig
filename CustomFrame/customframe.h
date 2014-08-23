#ifndef CUSTOMFRAME_H
#define CUSTOMFRAME_H

#include "customframe_global.h"
#include "customtitlebar.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QSizeGrip>


class CUSTOMFRAMESHARED_EXPORT CustomFrame : public QWidget
{
    Q_OBJECT
public:
    explicit CustomFrame(QWidget *parent = 0);
    virtual ~CustomFrame();
    QString getBackgroundColorHexCode() const;

    void setWindowTitle(const QString &title);
    void setBackgroundColorHexCode(const QString &value);
    void setWindowSize(int width, int height);
    void setWindowTitleSize(int width, int height);
    void setBackGroundImage(const QString &path);

signals:
    void WindowTitleChanged(const QString&);

public slots:

protected:
    void showEvent(QShowEvent *event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent  *event);

private:
    QVBoxLayout m_VBoxLayout;
    CustomTitlebar m_TitleBar;
    QPixmap *m_Cache;
    QSizeGrip m_SizeGrip;
    QString backgroundColorHexCode;
    QString imagePath;

    void CenterOnScreen();
};

#endif // CUSTOMFRAME_H
