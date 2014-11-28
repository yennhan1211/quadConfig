
#include <QtGui>
#include <QDesktopWidget>

#include "customframe.h"

CustomFrame::CustomFrame(QWidget *parent,int WidgetType) :
    QWidget(parent),
    m_SizeGrip(this),
    m_Cache(new QPixmap),

    m_TitleBar(this)
{
    m_TitleBar.setWidgetType(WidgetType);
    m_TitleBar.move(0, 0);

    m_SizeGrip.setStyleSheet("image: none");

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);

    connect(this, SIGNAL(WindowTitleChanged(QString)),
            &m_TitleBar, SLOT(UpdateWindowTitle(QString)));

#if QT_VERSION >= 0x040500
#ifdef Q_WS_X11
    if(x11Info().isCompositingManagerRunning())
        setAttribute(Qt::WA_TranslucentBackground);
#else
    setAttribute(Qt::WA_TranslucentBackground);
#endif
#endif
}

CustomFrame::~CustomFrame()
{
    delete m_Cache;
}

void CustomFrame::setWindowTitle(const QString &title)
{
    QWidget::window()->setWindowTitle(title);

    emit WindowTitleChanged(title);
}

void CustomFrame::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    CenterOnScreen();
}

void CustomFrame::paintEvent(QPaintEvent *event)
{
    Q_UNUSED (event);

    if(m_Cache != NULL)
    {
        QPainter painter(this);
        painter.drawPixmap(0, 0, *m_Cache);
#if QT_VERSION >= 0x040500
        if(!testAttribute(Qt::WA_TranslucentBackground))
            setMask(m_Cache->mask());
#else
        setMask(m_Cache->mask());
#endif
    }
}

void CustomFrame::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    delete m_Cache;
    m_Cache = new QPixmap(size());
    m_Cache->fill(Qt::transparent);
    QPainter painter(m_Cache);

#ifdef DEFAULT_FRAME
    QColor darkBlue( 23,  23,  34);
    QColor lightBlue(177, 177, 203);

    /********** Window's background **********/

    QPolygon background;

    background << QPoint(  0,  16)
        << QPoint(  16,  0)
        << QPoint(width() -  1,  0)
        << QPoint(width() -  1, height() - 33)
        << QPoint(width() - 17, height() - 17)
        << QPoint(  272, height() - 17)
        << QPoint(  256, height() -  1)
        << QPoint(  16, height() -  1)
        << QPoint(  16,  272)
        << QPoint(  0,  256);

    painter.setPen (QPen(darkBlue));
    painter.setBrush(QBrush(darkBlue));
    painter.setOpacity(0.8);
    painter.drawPolygon(background);

    /*****************************************/
    /********** Window's frame **********/

    QPolygon frame;

    frame << QPoint(  4,  20)
        << QPoint(  20,  4)
        << QPoint(width() -  4,  4)
        << QPoint(width() -  4, height() - 37)
        << QPoint(width() - 20, height() - 21)
        << QPoint(268, height() - 21)
        << QPoint(252, height() -  5)
        << QPoint(20, height() -  5)
        << QPoint(20,  268)
        << QPoint(4,  252);

    painter.setPen (QPen(lightBlue));
    painter.setBrush(Qt::NoBrush );
    painter.drawPolygon(frame);
#else

    QColor startColor(backgroundColorHexCode);
    QColor endColor(backgroundColorHexCode);

    QLinearGradient gr(0, 0, 0, height());
    gr.setColorAt(0, startColor);
    gr.setColorAt(1, endColor);

    QRectF frame(0, 0, width(), height());
    painter.setPen(Qt::NoPen);
    painter.setBrush(gr);
    painter.drawRect(frame);
    qDebug() << "resize event";
    QPixmap mypixmap(imagePath);
    painter.drawPixmap(frame,mypixmap,mypixmap.rect());

#endif

    m_SizeGrip.move(width() - 32, height() - 32);
    m_SizeGrip.resize(32, 32);
}

QString CustomFrame::getBackgroundColorHexCode() const
{
    return backgroundColorHexCode;
}

void CustomFrame::setBackgroundColorHexCode(const QString &value)
{
    backgroundColorHexCode = value;
}

void CustomFrame::setWindowSize(int width, int height)
{
    setFixedSize(width, height);
}

void CustomFrame::setWindowTitleSize(int width, int height)
{
    m_TitleBar.setFixedSize(width, height);
}

void CustomFrame::setBackGroundImage(const QString &path)
{
    imagePath = path;
    resizeEvent(0);
}

void CustomFrame::CenterOnScreen()
{
    QDesktopWidget screen;

    QRect screenGeom = screen.screenGeometry(this);

    int screenCenterX = screenGeom.center().x();
    int screenCenterY = screenGeom.center().y();

    move(screenCenterX - width () / 2,
        screenCenterY - height() / 2);
}
