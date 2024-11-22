#include "loading.h"

Loading::Loading(QWidget *parent) : QWidget(parent), current(0) {
    Q_ASSERT(parent);
    parent->installEventFilter(this);
    raise();
    setAttribute(Qt::WA_TranslucentBackground);
    startTimer(20);
}

bool Loading::eventFilter(QObject *obj, QEvent *ev) {
    if (obj == parent()) {
        if (ev->type() == QEvent::Resize) {
            QResizeEvent * rev = static_cast<QResizeEvent*>(ev);
            resize(rev->size());
        }
        else if (ev->type() == QEvent::ChildAdded)
            raise();
    }
    return QWidget::eventFilter(obj, ev);
}

bool Loading::event(QEvent *ev) {
    if (ev->type() == QEvent::ParentAboutToChange) {
        if (parent()) parent()->removeEventFilter(this);
    }
    else if (ev->type() == QEvent::ParentChange) {
        if (parent()) {
            parent()->installEventFilter(this);
            raise();
        }
    }
    return QWidget::event(ev);
}

void Loading::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.fillRect(rect(), QColor(100, 100, 100, 64));

    QPen pen;
    pen.setWidth(12);
    pen.setColor(QColor(0, 191, 255));  // DeepSkyBlue
    p.setPen(pen);

    p.setRenderHint(QPainter::Antialiasing);

    QRectF rectangle(width()/2 - 40.0, height()/2 - 40.0, 80.0, 80.0);

    int spanAngle = (int)(qMin(0.2, current) * 360 * -16);
    int startAngle = (int)(qMin(0.0, 0.2 - current) * 360 * 16);

    p.drawArc(rectangle, startAngle, spanAngle);
}

void Loading::timerEvent(QTimerEvent *) {
    if (isVisible()) {
        current += 0.03;
        if (current > 1.2) current = 0.2; // restart cycle
        update();
    }
}
