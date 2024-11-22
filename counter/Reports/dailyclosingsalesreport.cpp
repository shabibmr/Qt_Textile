#include "dailyclosingsalesreport.h"
#include "ui_dailyclosingsalesreport.h"

DailyClosingSalesReport::DailyClosingSalesReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DailyClosingSalesReport)
{
    ui->setupUi(this);
    ui->fromDateDateEdit->setDate(QDate::currentDate().addDays(-30));
    ui->toDateDateEdit->setDate(QDate::currentDate());
}

DailyClosingSalesReport::~DailyClosingSalesReport()
{
    delete ui;
}

void DailyClosingSalesReport::on_OKButton_clicked()
{

}

void DailyClosingSalesReport::on_excelButton_clicked()
{

}

void DailyClosingSalesReport::on_printButton_clicked()
{

}

void DailyClosingSalesReport::on_backButton_clicked()
{
    this->close();
}
