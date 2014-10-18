
#include "customtitlebar.h"

#include <QApplication>
#include <QtGui>

CustomTitlebar::CustomTitlebar(QWidget *parent) :
    QWidget(parent),
    m_Cache(new QPixmap),
    m_Title(this),
    m_Minimize(CustomButton::BUTTON_MINIMIZE, this),
#ifdef FRAME_CAN_RESIZE
    m_Maximize(CustomButton::BUTTON_MAXIMIZE, this),
#endif
    m_Close(CustomButton::BUTTON_CLOSE, this)
{

    m_Title.setStyleSheet
            ("font-size:14px;font-weight:bold;color:#ecf0f1;alignment:center");
    mWidgetType = 0;
    connect ( & m_Minimize, SIGNAL (clicked ()),
                this, SLOT(Minimized ()));
#ifdef FRAME_CAN_RESIZE
    connect ( & m_Maximize, SIGNAL (clicked ()),
            this, SLOT(Maximized ()));
#endif

    connect ( & m_Close, SIGNAL (clicked ()),
            this, SLOT(Quit ()));
}

CustomTitlebar::~CustomTitlebar()
{
    delete m_Cache;
}

void CustomTitlebar::setWidgetType(int mType)
{
    mWidgetType = mType;
}

void CustomTitlebar::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    delete m_Cache; // Remove old cache
    m_Cache = new QPixmap(size()); // Create a cache with same size as the widget
    m_Cache->fill(Qt::transparent);  // Create a the transparent background
    QPainter painter(m_Cache); // Start painting the cache

#ifdef DEFAULT_FRAME
    QColor lightBlue    (177, 177, 203, 255);
    QColor gradientStart(  0,   0,   0,   0);
    QColor gradientEnd  (  0,   0,   0, 220);

    QLinearGradient linearGrad(QPointF(0, 0), QPointF(0, height()));
    linearGrad.setColorAt(0, gradientStart);
    linearGrad.setColorAt(1, gradientEnd);

    /********** Title bar's frame **********/
    QPolygon frame;

    frame << QPoint(         20,  4)
       << QPoint(width() - 4,  4)
       << QPoint(width() - 4, 32)
       << QPoint(          4, 32)
       << QPoint(          4, 20);

    painter.setPen  (QPen  (lightBlue ));
    painter.setBrush(QBrush(linearGrad));

    painter.drawPolygon(frame);
    /***************************************/

    /********** Title bar's buttons area **********/
    QPolygon buttons;

    buttons << QPoint(width() - 80,  4)
         << QPoint(width() -  4,  4)
         << QPoint(width() -  4, 32)
         << QPoint(width() - 88, 32)
         << QPoint(width() - 88, 12);

    painter.setPen  (QPen  (lightBlue));
    painter.setBrush(QBrush(lightBlue));

    painter.drawPolygon(buttons);
    /**********************************************/
#else

#endif

    m_Title.move(450, 5);
    m_Title.resize(width() - (height()*2 + 20), height());

    m_Minimize.resize(height(), height());
    m_Minimize.move(width() - (height()*2 + 20), 0);

#ifdef FRAME_CAN_RESIZE
    m_Maximize.move(width() - 57, 6);
    m_Maximize.resize(22, 22);
#endif

    m_Close.resize(height(), height());
    m_Close.move(width() - (height() + 20), 0);
}

void CustomTitlebar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    if(m_Cache != NULL)
    {
        QPainter painter(this);
        painter.drawPixmap(0, 0, *m_Cache);
    }
}

void CustomTitlebar::mousePressEvent(QMouseEvent *event)
{
    m_Diff = event->pos();
    setCursor(QCursor(Qt::SizeAllCursor));
}

void CustomTitlebar::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    setCursor(QCursor(Qt::ArrowCursor));
}

void CustomTitlebar::mouseMoveEvent(QMouseEvent *event)
{
    QPoint p = event->globalPos();
    window()->move(p - m_Diff);
}

void CustomTitlebar::UpdateWindowTitle(const QString& title)
{
    if (! window()->windowTitle().isEmpty()) {
        m_Title.setText(window()->windowTitle());
    }
    else {
        m_Title.setText(title);
    }
}


void CustomTitlebar::Minimized()
{
    window()->showMinimized();
}

void CustomTitlebar::Maximized()
{
    if(window()->windowState() == Qt::WindowMaximized)
    {
        window()->showNormal();
    }
    else
    {
        window()->showMaximized();
    }
}

void CustomTitlebar::Quit()
{
    QWidget *mainWidget = qobject_cast<QWidget*> (this->parent());
    mainWidget->hide();
    if(mWidgetType == 0)qApp->quit();
}
