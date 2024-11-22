#include "modeofservicebutton.h"

ModeOfServiceButton::ModeOfServiceButton(QWidget *parent)
    :QToolButton(parent)
{
    this->setAutoRaise(true);
    this->setStyleSheet("border-style: outset;border-bottom-color: rgb(255, 127, 80);"
                        "border-top-color: rgb(255, 160, 122);border-left-color:  rgb(255, 210, 0);"
                        "background-color: rgb(255, 255, 255);border-right-color: rgb(255, 140, 0);"
                        "border-radius: 10px;border-width: 1px;");

//    this->setContentsMargins(3,3,3,3);
    effect = new QGraphicsDropShadowEffect();
    effect->setColor(QColor(180, 2, 20, 240));
    effect->setBlurRadius(3); //Adjust accordingly
    effect->setOffset(3,3);
}

ModeOfServiceButton::~ModeOfServiceButton()
{

}

void ModeOfServiceButton::setEffectEnable(bool v)
{
    effect->setEnabled(v);
}
void ModeOfServiceButton::mousePressEvent(QMouseEvent *e)
{
    this->setGraphicsEffect(effect);
    if(e->button()==Qt::RightButton)
    {
        emit rightClicked();
    }
    else
    {
        emit clicked();
    }
}

void ModeOfServiceButton::mouseReleaseEvent(QMouseEvent *e)
{
    effect->setEnabled(false);
}


