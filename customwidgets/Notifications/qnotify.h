#ifndef QNOTIFY_H
#define QNOTIFY_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QMouseEvent>

class QNotify : public QWidget
{
    Q_OBJECT
public:
    enum NotificationType {
        SUCCESS,
        ERROR,
        WARNING
    };

    explicit QNotify( QWidget *parent );
    ~QNotify();

    void setPosition( QPoint p );
    void setDialogWidth( int width );
    void adjustInViewport();
    void notify( QString text, NotificationType type, int duration = 1000 );

private slots:
    void onFinished();

protected:
    virtual void paintEvent( QPaintEvent * );
    virtual void mousePressEvent( QMouseEvent *mouseEvent );

private:
    QPropertyAnimation *animation;
    int x;
    int y;
    int w;
    bool showingNow;
    QString msgText;
    NotificationType notifType;
    int keepDuration;
};

#endif
