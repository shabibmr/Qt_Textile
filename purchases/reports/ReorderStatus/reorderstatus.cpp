#include "reorderstatus.h"
#include "ui_reorderstatus.h"

ReorderStatus::ReorderStatus(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReorderStatus)
{
    ui->setupUi(this);
    ui->DateWidget->hide();
    itemReportHelper = new InventoryHelper;
    ui->toDateDateEdit->setDate(QDate::currentDate());
    setTable();

}

ReorderStatus::~ReorderStatus()
{
    delete ui;
}

void ReorderStatus::on_OKButton_clicked()
{
    setTable();
}

void ReorderStatus::on_excelButton_clicked()
{

}

void ReorderStatus::on_printButton_clicked()
{

}

void ReorderStatus::on_backButton_clicked()
{

}

void ReorderStatus::setTable()
{
    QDate dateTo = ui->toDateDateEdit->date();
    model = itemReportHelper->getReOrderLevelReport(dateTo);
    ui->tableView->setModel(model);
    ui->tableView->hideColumn(0);
}
