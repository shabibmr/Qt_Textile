#include "lineediticon.h"
#include <QPainter>
#include <QSize>
#include <QDebug>
LineEditIcon::LineEditIcon(const QIcon icon, QWidget *parent)
    : QLineEdit(parent)
{
    icon.actualSize(QSize(16,16));
    qDebug()<<icon.actualSize(QSize(16,16));
    setIcon(icon);
}

LineEditIcon::~LineEditIcon()
{
}

void LineEditIcon::setIcon(QIcon icon)
{
    m_icon = icon;
    if (m_icon.isNull())
        setTextMargins(1, 1, 1, 1);
    else
        setTextMargins(20, 1, 1, 1);
}

void LineEditIcon::paintEvent(QPaintEvent * event)
{
    QLineEdit::paintEvent(event);
    if (!m_icon.isNull()) {
        QPainter painter(this);
        QPixmap pxm = m_icon.pixmap(height() - 6, height() - 6);
        int x = 2, cx = pxm.width();

        painter.drawPixmap(x, 3, pxm);
        painter.setPen(QColor("lightgrey"));
        painter.drawLine(cx + 2, 3, cx + 2, height() - 4);
    }
}
