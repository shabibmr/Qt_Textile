#include "gmhovertoolbutton.h"
#include <QDebug>


GMHoverToolButton::GMHoverToolButton(QString imageUrl ,QToolButton *parent) : QToolButton (parent)
{
    url = imageUrl;
    setIcon(QIcon(imageUrl));
    QString tooltip = "<img src="+url+">";
    setToolTip(tooltip);
}

GMHoverToolButton::~GMHoverToolButton()
{


}


