#include "purchaseorders.h"
#include "ui_purchaseorders.h"
#include "datamodels/Counter/countersettingsdatamodel.h"
#include "customwidgets/Delegates/reportcommondelegate.h"

purchaseOrders::purchaseOrders(bool hideCreate,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::purchaseOrders)
{
    ui->setupUi(this);

//    ui->createNewPushButton->setHidden(hideCreate);
    ui->fromDateDateEdit->setDate(QDate::currentDate());
    ui->toDateDateEdit->setDate(QDate::currentDate());
    dbhelper = new PurchaseOrderDatabaseHelper();
    ui->tableView->setItemDelegate(new ReportCommonDelegate());
    setTableView();
}

purchaseOrders::~purchaseOrders()
{
    delete ui;
}

void purchaseOrders::setHeading(QString label)
{
    ui->titleLabel->setText(label);
}

void purchaseOrders::keyPressEvent(QKeyEvent *e)
{
    if ((e->key() == Qt::Key_N)  && (e->modifiers().testFlag(Qt::ControlModifier))){
        on_createNewPushButton_clicked();
    }

}

void purchaseOrders::setTableView()
{

    QDate fromDate = ui->fromDateDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

    model = dbhelper->getVoucherListByDateModel(fromDate,toDate);
    sort = new GMItemSortFilterProxyModel(this);
    sort->setDynamicSortFilter(true);
    sort->setSourceModel(model);

    ui->tableView->setModel(sort);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(1);
    ui->tableView->hideColumn(2);
    ui->tableView->hideColumn(3);

    float total =0;
    for(int i=0;i<ui->tableView->model()->rowCount();i++){
        total+= model->record(i).value(6).toFloat();
    }
    ui->totalAmountLineEdit->setText(QString::number(total,'f',2));
    //    ui->tableView->hideColumn(3);
    ui->tableView->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);


}

void purchaseOrders::on_createNewPushButton_clicked()
{
    AddPurchaseOrderWidget = new PurchaseOrderVoucherEditor(this);
    AddPurchaseOrderWidget->setWindowFlags(Qt::Window);
    AddPurchaseOrderWidget->showMaximized();
    QObject::connect(AddPurchaseOrderWidget,SIGNAL(closing()),this,SLOT(setTableView()));
}


void purchaseOrders::on_okPushButton_clicked()
{
    setTableView();
}

void purchaseOrders::delButtonClicked(bool)
{
    int x= sender()->property("row").toInt();
    qDebug()<<x;
}

void purchaseOrders::on_tableView_doubleClicked(const QModelIndex &index)
{
    int row = sort->mapToSource(index).row();
    QString voucherNo = model->record(row).value(1).toString();
    QString voucherPrefix = model->record(row).value(3).toString();
    qDebug()<<"at 0 "<<voucherNo;
    GeneralVoucherDataObject *voucher = dbhelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);

    AddPurchaseOrderWidget = new PurchaseOrderVoucherEditor(this);
    AddPurchaseOrderWidget->setWindowFlags(Qt::Window);
    AddPurchaseOrderWidget->showVoucher(voucher);
    AddPurchaseOrderWidget->showMaximized();
    QObject::connect(AddPurchaseOrderWidget,SIGNAL(closing()),this,SLOT(setTableView()));
}
