#include "salesbillspending.h"
#include "ui_salesbillspending.h"

SalesBillsPending::SalesBillsPending(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SalesBillsPending)
{
    ui->setupUi(this);
    ui->fromDateDateEdit->setDate(QDate::currentDate());
    ui->toDateDateEdit->setDate(QDate::currentDate());

}

SalesBillsPending::~SalesBillsPending()
{
    delete ui;
}

void SalesBillsPending::on_OKButton_clicked()
{

}

void SalesBillsPending::on_excelButton_clicked()
{

}

void SalesBillsPending::on_printButton_clicked()
{

}

void SalesBillsPending::on_backButton_clicked()
{

}
