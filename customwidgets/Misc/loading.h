

#include <QWidget>

//class Loading
//{
//    Q_OBJECT
//public:
//    Loading();
//};

//#endif // LOADING_H


#pragma once

#include <QtGui>

class Loading : public QWidget {
    Q_OBJECT
public:
    explicit Loading(QWidget *parent);

protected:
    double current;
    bool eventFilter(QObject *obj, QEvent *ev);
    bool event(QEvent *);
    void paintEvent(QPaintEvent *);
    void timerEvent(QTimerEvent *);
};
