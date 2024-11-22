#include "sharebutton.h"

ShareButton::ShareButton(QPushButton *parent) : QPushButton(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

}

ShareButton::~ShareButton()
{
    delete this;
}
