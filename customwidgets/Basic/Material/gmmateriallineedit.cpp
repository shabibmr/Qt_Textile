#include "gmmateriallineedit.h"
#include <QDebug>

GMMaterialLineEdit::GMMaterialLineEdit(QString *m,int dataType,int dec, QtMaterialTextField *parent) :
    QtMaterialTextField (parent)
{
    qDebug()<<"material lineedit"<<*m;
    QObject::connect(this,&QLineEdit::editingFinished,
                     this,&GMMaterialLineEdit::FinishedEdit);
    value =m;
//    this->setMaximumWidth(80);

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

void GMMaterialLineEdit::FinishedEdit()
{
    qDebug()<<"edited line edit &&&&&&&&&&";
    *value = this->text();
    emit editFinished();
}

void GMMaterialLineEdit::setVariable(QString *temp)
{
    value = temp;
    this->setText(*temp);
}
