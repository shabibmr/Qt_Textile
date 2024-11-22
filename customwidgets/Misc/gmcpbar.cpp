#include "gmcpbar.h"

GMCPBar::GMCPBar(QWidget *parent) : QWidget(parent), p(0) {
    setMinimumSize(208, 208);
}


void GMCPBar::paintEvent(QPaintEvent *event)
{
    qreal pd = p * 360;
    qreal rd = 360 - pd;
    QPainter p(this);
    p.fillRect(rect(), Qt::white);
    p.translate(4, 4);
    p.setRenderHint(QPainter::Antialiasing);
    QPainterPath path, path2;
    path.moveTo(100, 0);
    path.arcTo(QRectF(0, 0, 200, 200), 90, -pd);
    QPen pen, pen2;
    pen.setCapStyle(Qt::FlatCap);
    pen.setColor(QColor("#30b7e0"));
    pen.setWidth(8);
    p.strokePath(path, pen);
    path2.moveTo(100, 0);
    pen2.setWidth(8);
    pen2.setColor(QColor("#d7d7d7"));
    pen2.setCapStyle(Qt::FlatCap);
    pen2.setDashPattern(QVector<qreal>{0.5, 1.105});
    path2.arcTo(QRectF(0, 0, 200, 200), 90, rd);
    pen2.setDashOffset(2.2);
    p.strokePath(path2, pen2);

}
