#include "salesorders.h"
#include "ui_salesorders.h"
#include "datamodels/Counter/countersettingsdatamodel.h"
#include "customwidgets/Delegates/salesordersdelgate.h"
#include "customwidgets/Delegates/reportcommondelegate.h"

salesOrders::salesOrders(bool hide,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::salesOrders)
{
    ui->setupUi(this);
    //    ui->createNewPushButton->setHidden(hide);
    ui->totalAmountLineEdit->setValidator(new QDoubleValidator(0.0002,999999,2,this));
    ui->dateEdit->setDate(QDate(QDate::currentDate().year(),QDate::currentDate().month(),1));
    ui->toDateEdit->setDate(QDate::currentDate());
    dbhelper = new SalesOrderDatabaseHelper();
    setTableView();

    //    ui->generateWorkOrder->hide();
    ui->tableView->setItemDelegate(new SalesOrdersDelgate());

}

salesOrders::~salesOrders()
{
    delete ui;
}

void salesOrders::setHeading(QString label)
{
    ui->titleLabel->setText(label);
}

void salesOrders::keyPressEvent(QKeyEvent *e)
{
    if ((e->key() == Qt::Key_N)  && (e->modifiers().testFlag(Qt::ControlModifier))){
        on_createNewPushButton_clicked();
    }
}

void salesOrders::on_createNewPushButton_clicked()
{
    AddSalesOrderWidget = new SalesOrderVoucherEditor(this);
    AddSalesOrderWidget->setWindowFlags(Qt::Window);
    AddSalesOrderWidget->showMaximized();
    QObject::connect(AddSalesOrderWidget,SIGNAL(closing()),this,SLOT(setTableView()));
}

void salesOrders::setTableView()
{
    QDate fromDate = ui->dateEdit->date();
    fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

    model = dbhelper->getVoucherListByDateTimeModel(fromDateTime,toDateTime);
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

void salesOrders::on_okPushButton_clicked()
{
    setTableView();
}

void salesOrders::delButtonClicked(bool)
{
    int x= sender()->property("row").toInt();
    qDebug()<<x;
}

void salesOrders::on_tableView_doubleClicked(const QModelIndex &index)
{
    int row = sort->mapToSource(index).row();
    QString voucherNo = model->record(row).value(1).toString();
    QString voucherPrefix = model->record(row).value(3).toString();
    //    qDebug()<<"at 0 "<<voucherNo;
    GeneralVoucherDataObject *voucher = dbhelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);

    AddSalesOrderWidget = new SalesOrderVoucherEditor();
    AddSalesOrderWidget->setWindowFlags(Qt::Window);
    AddSalesOrderWidget->showVoucher(voucher);
    AddSalesOrderWidget->showMaximized();
    QObject::connect(AddSalesOrderWidget,SIGNAL(closing()),this,SLOT(setTableView()));
}

void salesOrders::on_generateWorkOrder_clicked()
{
    //    QItemSelectionModel *select = ui->tableView->selectionModel();

    //    if(select->hasSelection()){
    //       QModelIndexList list =  select->selectedRows(); // return selected row(s)

    //       for(QModelIndex index:list){
    //           qDebug()<<"Selected : "<<index.row();
    //       }
    //    } //check if has selection
    //    else{
    //        qDebug()<<"no Rows Selected";
    //    }

    GodownSelectorWidget *godownSelector = new GodownSelectorWidget();
    godownSelector->show();

    connect(godownSelector, &GodownSelectorWidget::sectionSelected, this, &salesOrders::exportToWorkOrder);
}

void salesOrders::exportToWorkOrder(QString section )
{
    qDebug()<<Q_FUNC_INFO<<section;
    exportSection = section;
    ShowVouchersWidget *selectVoucherWidget = new ShowVouchersWidget(this);
    selectVoucherWidget->setShowWidget(false);
    selectVoucherWidget->setVoucherTypes(GMVoucherTypes::SalesOrder, GMVoucherTypes::WorkOrder);
    selectVoucherWidget->setDate(fromDateTime,toDateTime);
    selectVoucherWidget->setSection(exportSection);
    //  QObject::connect(selectVoucherWidget,SIGNAL(importVoucher(GeneralVoucherDataObject *)),this,SLOT(openWorkorder(GeneralVoucherDataObject *voucher)));
    connect(selectVoucherWidget, &ShowVouchersWidget::importVoucher, this, &salesOrders::openWorkorder);

    connect(selectVoucherWidget, &ShowVouchersWidget::noVoucherToExport, this, [=](){
        qDebug()<<"no vouchers"<<Q_FUNC_INFO<<__LINE__;
        QMessageBox box; box.setText("No vouchers to export for selected section : " + exportSection );box.exec();
    });
    selectVoucherWidget->selectAllVouchers();
    selectVoucherWidget->import();
    qDebug()<<Q_FUNC_INFO<<__LINE__;


}

void salesOrders::openWorkorder(GeneralVoucherDataObject *voucher)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__<<voucher->InventoryItems.size()<<voucher->toGodownID;
    // GeneralVoucherDataObject *voucher;
    WorkOrderDatabaseHelper woHelper;
    voucher->VoucherDate = QDate::currentDate();
    voucher->VoucherPrefix = LoginValues::voucherPrefix;
    voucher->voucherType = GMVoucherTypes::getVoucherString(GMVoucherTypes::WorkOrder);
    voucher->voucherNumber = woHelper.getNextVoucherNo(voucher->VoucherPrefix);
    voucher->status = QuotationStatus::Started;

    WorkOrderEditor  *editor = new WorkOrderEditor(this);
    editor->setWindowFlags(Qt::Window);
    editor->showVoucher(voucher);
    editor->showMaximized();

}
