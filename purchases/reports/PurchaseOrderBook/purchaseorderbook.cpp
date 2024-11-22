#include "purchaseorderbook.h"
#include "ui_purchaseorderbook.h"

PurchaseOrderBook::PurchaseOrderBook(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PurchaseOrderBook)
{
    ui->setupUi(this);
}

PurchaseOrderBook::~PurchaseOrderBook()
{
    delete ui;
}

void PurchaseOrderBook::on_OKButton_clicked()
{

}

void PurchaseOrderBook::on_excelButton_clicked()
{

}

void PurchaseOrderBook::on_printButton_clicked()
{

}

void PurchaseOrderBook::on_backButton_clicked()
{

}
