#include "gmmaterialcombobox.h"

GMMaterialComboBox::GMMaterialComboBox(QString *m, QStringList items, QtMaterialAutoComplete *parent):
    QtMaterialAutoComplete (parent)
{
//    this->addItems(items);
    value = new QString;


    value =m;
    //    this->setMaximumWidth(80);
//    this->setMaximumWidth(80);

    this->setDataSource(items);
    this->setText(*value);
    this->setAutoFillBackground(true);

    QObject::connect(this,&QtMaterialAutoComplete::itemSelected,
                     this,&GMMaterialComboBox::ItemSelected);

}

void GMMaterialComboBox::setVariable(QString *temp)
{
    value = temp;
    this->setText(*temp);
}

void GMMaterialComboBox::FinishedEdit()
{
    *value = this->text();
    emit editFinished();
}

void GMMaterialComboBox::ItemSelected(QString temp)
{

//    qDebug()<<Q_FUNC_INFO<<temp<<*value;
    *value = temp;
//    qDebug()<<"check 1"<<*value;
    emit selectedItem();

}
