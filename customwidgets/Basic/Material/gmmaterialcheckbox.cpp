#include "gmmaterialcheckbox.h"

GMMaterialCheckBox::GMMaterialCheckBox()
{

}

GMMaterialCheckBox::GMMaterialCheckBox(bool *m, QtMaterialCheckBox *parent)
{
    QObject::connect(this,&QtMaterialCheckBox::toggled,
                     this,&GMMaterialCheckBox::checked);
    value =m;
    this->setMaximumWidth(80);
    this->setChecked(*value);


}

void GMMaterialCheckBox::setVariable(bool *temp)
{
    value = temp;
    this->setChecked(*temp);
}

void GMMaterialCheckBox::checked()
{
    *value = this->isChecked();
    emit itemChecked();
}
