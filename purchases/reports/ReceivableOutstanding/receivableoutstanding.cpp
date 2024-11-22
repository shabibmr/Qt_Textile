#include "receivableoutstanding.h"
#include "ui_receivableoutstanding.h"

ReceivableOutstanding::ReceivableOutstanding(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReceivableOutstanding)
{
    ui->setupUi(this);

    ui->fromDateDateEdit->setDate(QDate(QDate::currentDate().year(),QDate::currentDate().month(),1));
    ui->toDateDateEdit->setDate(QDate::currentDate());
}

ReceivableOutstanding::~ReceivableOutstanding()
{
    delete ui;
}

void ReceivableOutstanding::on_OKButton_clicked()
{

}

void ReceivableOutstanding::on_excelButton_clicked()
{

}

void ReceivableOutstanding::on_printButton_clicked()
{

}

void ReceivableOutstanding::on_backButton_clicked()
{

}
