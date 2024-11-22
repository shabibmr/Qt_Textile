#include "additemprice.h"
#include "ui_additemprice.h"

addItemPrice::addItemPrice(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addItemPrice)
{
    ui->setupUi(this);
    ui->priceLineEdit->setValidator(new QDoubleValidator(0.0002,999999,2,this));
}

addItemPrice::~addItemPrice()
{
    delete ui;
}

void addItemPrice::on_buttonBox_accepted()
{

}

void addItemPrice::on_buttonBox_rejected()
{

}
