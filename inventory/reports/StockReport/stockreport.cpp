#include "stockreport.h"
#include "ui_stockreport.h"
#include <QDate>
StockReport::StockReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StockReport)
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

StockReport::~StockReport()
{
    delete ui;
}

void StockReport::setTable()
{
    QDate dateTo = ui->toDateDateEdit->date();
//    model = inventoryReportsHelper->getClosingStockListByEndDateModel(dateTo); -- use closingstockhelper

    // remove

//    QStringList vlist;
//    vlist.append("26604");
//    vlist.append("26605");
//    vlist.append("26606");
//    vlist.append("26607");
//    vlist.append("26608");

//    model = inventoryReportsHelper->getIngredientsByWorkOrders(vlist);

    sort->setDynamicSortFilter(true);
    sort->setSourceModel(model);
    ui->tableView->setModel(sort);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->sortByColumn(1,Qt::AscendingOrder);
    ui->tableView->hideColumn(0);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableView->reset();
}

void StockReport::on_OKButton_clicked()
{
    setTable();
}

void StockReport::on_excelButton_clicked()
{

}

void StockReport::on_printButton_clicked()
{

}

void StockReport::on_backButton_clicked()
{

}
