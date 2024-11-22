#include "itemwisepurchaseorderreport.h"
#include "ui_itemwisepurchaseorderreport.h"




ItemwisePurchaseOrderReport::ItemwisePurchaseOrderReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ItemwisePurchaseOrderReport)
{
    ui->setupUi(this);
    this->setWindowTitle("Purchase Itemwise");
    ui->tableView->setItemDelegate(new ReportCommonDelegate());
    ui->fromDate->setDate(QDate::currentDate());
    ui->toDate->setDate(QDate::currentDate());
    inHelper = new InventoryHelper;
    ui->tableView->setSortingEnabled(true);
    setTableView();

}

ItemwisePurchaseOrderReport::~ItemwisePurchaseOrderReport()
{
    delete ui;
}

void ItemwisePurchaseOrderReport::setTableView()
{
    QDate fromDate = ui->fromDate->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDate->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

    qDebug()<<"PURCHASE ORDER";
//    model = inHelper->getItemWisePurchaseOrderByDateModel(fromDateTime,toDateTime);
    model = inHelper->getItemwiseTransactionSummaryByVoucherType(fromDateTime, toDateTime, GMVoucherTypes::PurchaseOrder);

    //    sort = new GMItemSortFilterProxyModel(this);
    //    sort->setDynamicSortFilter(true);
    //    sort->setSourceModel(model);

    ui->tableView->setModel(model);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(1);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

void ItemwisePurchaseOrderReport::on_okButton_clicked()
{
    setTableView();
}
