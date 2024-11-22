#include "QNotify.h"

#include <QPixmapCache>
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <QTimer>

QNotify::QNotify( QWidget *parent ) :
    QWidget( parent )
{
    Q_INIT_RESOURCE( resources );

    QPixmapCache::insert( "success.png", QPixmap( ":/success.png" ).scaledToHeight( 80, Qt::SmoothTransformation ) );
    QPixmapCache::insert( "error.png", QPixmap( ":/error.png" ).scaledToHeight( 80, Qt::SmoothTransformation ) );
    QPixmapCache::insert( "warning.png", QPixmap( ":/warning.png" ).scaledToHeight( 80, Qt::SmoothTransformation ) );

    animation = new QPropertyAnimation( this, "geometry" );
    connect( animation, SIGNAL(finished()), SLOT(onFinished()) );

    // defaults
    w = 300;
    x = qApp->desktop()->availableGeometry().width() - ( w + 50 );
    y = 50;
    setGeometry( x, y, w, 100 );
    showingNow = false;
}

QNotify::~QNotify()
{
    QPixmapCache::clear();
}

void QNotify::setPosition( QPoint p )
{
    x = p.x();
    y = p.y();
    setGeometry( x, y, w, 100 );
    repaint();
}

void QNotify::setDialogWidth( int width )
{
    w = width;
    setGeometry( x, y, w, 100 );
    repaint();
}

void QNotify::adjustInViewport()
{
    x = qApp->desktop()->availableGeometry().width() - ( w + 50 );
    y = 50;
}

void QNotify::notify( QString text, NotificationType type, int duration )
{
    msgText = text;
    notifType = type;
    keepDuration = duration;
    repaint();

    animation->setDuration( 500 );
    animation->setStartValue( QRect( x, -100, w, 100 ) );
    animation->setEndValue( QRect( x, y, w, 100 ) );
    showingNow = true;
    animation->start();
    show();
}

void QNotify::onFinished()
{
    if ( showingNow ) {
        animation->setStartValue( QRect( x, y, w, 100 ) );
        animation->setEndValue( QRect( x, -100, w, 100 ) );
        QTimer::singleShot( keepDuration, animation, SLOT(start()) );
        showingNow = false;
    } else {
        showingNow = false;
        hide();
    }
}

void QNotify::paintEvent( QPaintEvent * )
{
    QPainter painter( this );
    painter.setRenderHints( QPainter::HighQualityAntialiasing | QPainter::Qt4CompatiblePainting );
    painter.setBrush( Qt::white );
    painter.setPen( Qt::NoPen );
    painter.drawRoundedRect( 0, 0, width(), height(), 5.0, 5.0 );
    QString key;
    if ( notifType == QNotify::SUCCESS ) {
        key = "success.png";
    } else if ( notifType == QNotify::ERROR ) {
        key = "error.png";
    } else if ( notifType == QNotify::WARNING ) {
        key = "warning.png";
    }
    int proposedCoord = height()/2 - QPixmapCache::find( key )->height()/2;
    painter.drawPixmap( 10, proposedCoord, *QPixmapCache::find( key ) );
    painter.setPen( Qt::black );
    int remainingWidth = width() - 120;
    painter.drawText( 120, 10, remainingWidth, height() - 20, Qt::AlignCenter | Qt::TextWordWrap, msgText );
}

void QNotify::mousePressEvent( QMouseEvent *mouseEvent )
{
    if ( mouseEvent->buttons() == Qt::LeftButton ) {
        animation->setStartValue( QRect( x, y, w, 100 ) );
        animation->setEndValue( QRect( x, -100, w, 100 ) );
        animation->start();
        showingNow = false;
    }
}
