#include "sidebarbutton.h"
#include <QPainter>
#include <QHBoxLayout>
#include "customwidgets/Material/components/lib/qtmaterialtheme.h"
SideBarButton::SideBarButton(QString text,QToolButton *parent)
    :QToolButton(parent)
{
    this->setFixedWidth(500);
//    this->setMinimumHeight(40);
//    this->setFixedHeight(100);
    QString bullet = QChar(0x98, 0x03);
    QString b2 = "\u20aa";

    font.setPointSize(10);
    if(text.length()>19){
        font.setPointSize(9);
    }

    label = new QLabel(this);
    blabel = new QLabel(b2);
    lblText = text;

    label->setText(lblText);
    label->setFont(font);
    label->setWordWrap(false);

//    this->setFont(font);
//    this->setToolTip(lblText);
//    this->setAutoRaise(true);

    QHBoxLayout *lay = new QHBoxLayout(this);
    lay->setContentsMargins(1,2,2,1);
    lay->addWidget(blabel);
    lay->addWidget(label);
    lay->setStretch(1,1);

    lay->setAlignment(label,Qt::AlignLeft|Qt::AlignVCenter);

//    QSizePolicy sizePolicy;
//    sizePolicy.setHorizontalPolicy(QSizePolicy::Minimum);
//    sizePolicy.setVerticalPolicy(QSizePolicy::Minimum);
//    this->setSizePolicy(sizePolicy);

//    label->setSizePolicy(sizePolicy);
//    this->setIcon(QIcon(":/images/documentExport.ico"));

}

SideBarButton::~SideBarButton()
{

}

void SideBarButton::setText(QString text)
{
    lblText = text;
    label->setText(lblText);
}

//void SideBarButton::paintEvent(QPaintEvent *event)
//{
//    QPainter painter(this);
//    const QFontMetrics fontMetrics = painter.fontMetrics();
//    const int usableWidth = qRound(0.9 * this->width());
//    const QString elidedText = fontMetrics.elidedText(lblText, Qt::ElideRight, usableWidth);
//    m_elided = (elidedText != lblText);
//    QToolButton::setText(elidedText);
//    QToolButton::paintEvent(event);
//}
