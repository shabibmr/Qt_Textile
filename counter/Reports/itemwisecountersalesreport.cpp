#include "itemwisecountersalesreport.h"
#include "ui_itemwisesalesreport.h"

ItemWiseCounterSalesReport::ItemWiseCounterSalesReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ItemWiseCounterSalesReport)
{
    ui->setupUi(this);
}

ItemWiseCounterSalesReport::~ItemWiseCounterSalesReport()
{
    delete ui;
}
