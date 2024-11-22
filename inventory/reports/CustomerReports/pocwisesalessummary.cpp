#include "pocwisesalessummary.h"
#include "ui_pocwisesalessummary.h"

POCwiseSalesSummary::POCwiseSalesSummary(QString title,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::POCwiseSalesSummary)
{
    ui->setupUi(this);
    this->setWindowTitle(title);
    ui->fromDate->setDate(QDate(QDate::currentDate().year(),QDate::currentDate().month(),1));
    ui->toDate->setDate(QDate::currentDate());
    inventoryReportsHelper = new InventoryHelper;

    model = new QSqlQueryModel;
    sort = new GMItemSortFilterProxyModel();

}



POCwiseSalesSummary::~POCwiseSalesSummary()
{
    delete ui;
}

void POCwiseSalesSummary::setPhoneNumber(const QString &value)
{
    phoneNumber = value;
    setTableView();
}

void POCwiseSalesSummary::on_OKButton_2_clicked()
{
    setTableView();
}

void POCwiseSalesSummary::setTableView()
{
    QDate fromDate = ui->fromDate->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDate->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

    model = inventoryReportsHelper->getItemwiseSalesByDateAndPoc(fromDateTime,toDateTime,phoneNumber);
    sort->setDynamicSortFilter(true);
    sort->setSourceModel(model);
    ui->tableView->setModel(sort);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->sortByColumn(0,Qt::AscendingOrder);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableView->reset();

}
