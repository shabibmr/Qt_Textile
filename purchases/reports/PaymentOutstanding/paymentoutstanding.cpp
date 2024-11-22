#include "paymentoutstanding.h"
#include "ui_paymentoutstanding.h"

PaymentOutstanding::PaymentOutstanding(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaymentOutstanding)
{
    ui->setupUi(this);

    ui->fromDateDateEdit->setDate(QDate(QDate::currentDate().year(),QDate::currentDate().month(),1));
    ui->toDateDateEdit->setDate(QDate::currentDate());
}

PaymentOutstanding::~PaymentOutstanding()
{
    delete ui;
}

void PaymentOutstanding::on_OKButton_clicked()
{

}

void PaymentOutstanding::on_excelButton_clicked()
{

}

void PaymentOutstanding::on_printButton_clicked()
{

}

void PaymentOutstanding::on_backButton_clicked()
{

}
