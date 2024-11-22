#include "iteemwisesalesorderlist.h"
#include "ui_iteemwisesalesorderlist.h"

IteemwiseSalesOrderList::IteemwiseSalesOrderList(QString voucherItemwiseType,
                                                 QString voucherType,
                                                 VoucherDBAbstract *vHelper,
                                                 QString tableName, QString mainTable, QString detailTable,
                                                 QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IteemwiseSalesOrderList)
{
    ui->setupUi(this);
    this->voucherType = voucherType;
    this->voucherItemwise = voucherItemwiseType;
    this->voucherDbHelper = vHelper;
    dbHelper = new ItemwiseSalesOrderDatabaseHelper(mainTable,detailTable);
    dbHelper->setOrder_Itemwise_TableName(tableName);
    ui->fromDateEdit->setDate(QDate::currentDate());
    ui->toDateEdit->setDate(QDate::currentDate());
    setData();

}

IteemwiseSalesOrderList::~IteemwiseSalesOrderList()
{
    delete ui;
}

void IteemwiseSalesOrderList::setTitle(const QString &value)
{
    title = value;
    ui->titleLabel->setText(title);
}

void IteemwiseSalesOrderList::setTableName(QString value)
{
    dbHelper->setOrder_Itemwise_TableName(value);
}

void IteemwiseSalesOrderList::setData()
{
    QDate fromDate = ui->fromDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

    model = dbHelper->getiSalesOrdersbetweenDates(fromDate,toDate);
    M =  dbHelper->getiiSalesOrdersbetweenDates(fromDate,toDate);

    ui->tableView->setModel(M);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
}

void IteemwiseSalesOrderList::on_createNewPushButton_clicked()
{
    qDebug()<<"creste New";
    AddItemwiseSalesOrder *AddVoucher = new AddItemwiseSalesOrder(voucherItemwise,voucherType,voucherDbHelper,dbHelper,this);
    AddVoucher->setWindowTitle(title);
    AddVoucher->setWindowFlags(Qt::Window);
    AddVoucher->showMaximized();
    QObject::connect(AddVoucher,SIGNAL(closing()),this,SLOT(setData()));

//    QObject::connect();
}

void IteemwiseSalesOrderList::on_okPushButton_clicked()
{
    setData();
}

void IteemwiseSalesOrderList::on_tableView_doubleClicked(const QModelIndex &index)
{

    int row = index.row();
    QString voucherNo = model->record(row).value(1).toString();
    QString voucherPrefix = model->record(row).value(2).toString();
    qDebug()<<"at 0 "<<voucherNo;

    GeneralVoucherDataObject *voucher = dbHelper->getVoucherByVoucherNoAsPtr(voucherNo,voucherPrefix);
    AddItemwiseSalesOrder *AddVoucher = new AddItemwiseSalesOrder(voucherItemwise,voucherType,voucherDbHelper,dbHelper,this);
    AddVoucher->setWindowTitle(title);
    AddVoucher->setVoucher(voucher);
    AddVoucher->setWindowFlags(Qt::Window);
    AddVoucher->showMaximized();
    QObject::connect(AddVoucher,SIGNAL(closing()),this,SLOT(setData()));
    //    QObject::connect();
}
