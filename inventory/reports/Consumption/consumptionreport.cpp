#include "consumptionreport.h"
#include "ui_consumptionreport.h"

ConsumptionReport::ConsumptionReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConsumptionReport)
{
    ui->setupUi(this);
    ui->fromDateDateEdit->setDate(QDate(QDate::currentDate().year(),QDate::currentDate().month(),1));
    ui->toDateDateEdit->setDate(QDate::currentDate());
    inventoryReportsHelper = new InventoryHelper;

    model = new QSqlQueryModel;
    sort = new GMItemSortFilterProxyModel();

    ui->excelButton->hide();
    ui->printButton->hide();


    setTable();

}

ConsumptionReport::~ConsumptionReport()
{
    delete ui;
}

void ConsumptionReport::setTable()
{
    QDate dateTo = ui->toDateDateEdit->date();
    QDate dateFrom = ui->fromDateDateEdit->date();
    model = inventoryReportsHelper->getConsumptionModel(dateFrom, dateTo);

    sort->setDynamicSortFilter(true);
    sort->setSourceModel(model);
    ui->tableView->setModel(sort);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->sortByColumn(1,Qt::AscendingOrder);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(1);

    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableView->reset();

}

void ConsumptionReport::on_OKButton_clicked()
{
    setTable();
}

void ConsumptionReport::on_excelButton_clicked()
{

}

void ConsumptionReport::on_printButton_clicked()
{

}

void ConsumptionReport::on_backButton_clicked()
{

}
