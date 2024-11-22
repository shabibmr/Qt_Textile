#include "purchasebillspending.h"
#include "ui_purchasebillspending.h"

PurchaseBillsPending::PurchaseBillsPending(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PurchaseBillsPending)
{
    ui->setupUi(this);
    ui->fromDateDateEdit->setDate(QDate::currentDate());
    ui->toDateDateEdit->setDate(QDate::currentDate());

}

PurchaseBillsPending::~PurchaseBillsPending()
{
    delete ui;
}

void PurchaseBillsPending::on_OKButton_clicked()
{

}

void PurchaseBillsPending::on_excelButton_clicked()
{

}

void PurchaseBillsPending::on_printButton_clicked()
{

}

void PurchaseBillsPending::on_backButton_clicked()
{

}
