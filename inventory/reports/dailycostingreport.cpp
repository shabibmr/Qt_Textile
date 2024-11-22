#include "dailycostingreport.h"
#include "ui_dailycostingreport.h"

DailyCostingReport::DailyCostingReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DailyCostingReport)
{
    ui->setupUi(this);
}

DailyCostingReport::~DailyCostingReport()
{
    delete ui;
}
