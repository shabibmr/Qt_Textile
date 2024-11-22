#include "gmlineedit.h"

GMLineEdit::GMLineEdit(QString *m,int dataType,int dec, QLineEdit *parent) :
    QLineEdit (parent)
{
    QObject::connect(this,&QLineEdit::editingFinished,
                     this,&GMLineEdit::FinishedEdit);
    value =m;
    this->setMaximumWidth(80);

    this->setText(*value);

    switch(dataType){
    case 0 :{
        this->setAlignment(Qt::AlignLeft);
        break;
    }
    case 1 : {
        this->setAlignment(Qt::AlignRight);
        this->setValidator(new MyValidator(0,99999999,dec,this));
        break;
    }

    }

}

void GMLineEdit::FinishedEdit()
{
    *value = this->text();
    emit editFinished();
}

void GMLineEdit::setVariable(QString *temp)
{
    value = temp;
    this->setText(*temp);
}
