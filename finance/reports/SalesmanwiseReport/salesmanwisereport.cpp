#include "salesmanwisereport.h"
#include "ui_salesmanwisereport.h"

SalesmanwiseReport::SalesmanwiseReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SalesmanwiseReport)
{
    ui->setupUi(this);

    ui->fromDateDateEdit->setDate(QDate::currentDate());
    ui->toDateDateEdit  ->setDate(QDate::currentDate());

    dbhelper = new LedgerHelper();

    setTable();
    this->setFocus();
}

SalesmanwiseReport::~SalesmanwiseReport()
{
    delete ui;
}

void SalesmanwiseReport::setTable()
{
    QDateTime dateFrom = ui->fromDateDateEdit->dateTime();
    QDateTime dateTo = ui->toDateDateEdit->dateTime();

    dateFrom.setTime(CounterSettingsDataModel::StartTime);
    dateTo.addDays(CounterSettingsDataModel::EndDay);
    dateTo.setTime(CounterSettingsDataModel::EndTime);


    model = dbhelper->getSalesReportModelByDate(dateFrom,dateTo);

    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);



}

void SalesmanwiseReport::on_OKButton_clicked()
{
    setTable();
}

void SalesmanwiseReport::on_excelButton_clicked()
{

}

void SalesmanwiseReport::on_printButton_clicked()
{

}

void SalesmanwiseReport::on_backButton_clicked()
{

}
