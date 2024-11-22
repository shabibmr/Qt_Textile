#include "customerlist.h"
#include "ui_customerlist.h"

CustomerList::CustomerList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomerList)
{
    ui->setupUi(this);

    inventoryReportsHelper = new InventoryHelper;
    model = new QSqlQueryModel;
    sort = new GMItemSortFilterProxyModel();
    ui->fromDateDateEdit->setDate(QDate::currentDate());
    ui->toDateDateEdit->setDate(QDate::currentDate());

    setTableView();
}

CustomerList::~CustomerList()
{
    delete ui;
}

void CustomerList::setTableView()
{

    model = inventoryReportsHelper->getDistinctCustomersPOC(ui->fromDateDateEdit->date(),ui->toDateDateEdit->date());
    sort->setDynamicSortFilter(true);
    sort->setSourceModel(model);
    ui->tableView->setModel(sort);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->sortByColumn(1,Qt::AscendingOrder);

    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableView->reset();

}

void CustomerList::on_OKButton_2_clicked()
{
    setTableView();
}

void CustomerList::on_tableView_doubleClicked(const QModelIndex &index)
{
    int row = sort->mapToSource(index).row();
    QString phoneNumber = model->record(row).value(0).toString();
    summaryWidget = new POCwiseSalesSummary('Phone : '+phoneNumber);
    summaryWidget->setPhoneNumber(phoneNumber);
    summaryWidget->showMaximized();
}

void CustomerList::on_searchLine_textChanged(const QString &arg1)
{
    QRegExp regExp(ui->searchLine->text(),Qt::CaseInsensitive);

    sort->setFilterRegExp(regExp);
}

void CustomerList::on_OKButton_clicked()
{
    setTableView();
}
