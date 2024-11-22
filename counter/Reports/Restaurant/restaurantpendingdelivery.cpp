#include "restaurantpendingdelivery.h"
#include "ui_restaurantpendingdelivery.h"

#include "customwidgets/Delegates/Orders/pendingdeliverydelegate.h"
RestaurantPendingDelivery::RestaurantPendingDelivery(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RestaurantPendingDelivery)
{
    ui->setupUi(this);
    ui->fromDateDateTimeEdit->setDate(QDate(QDate::currentDate().year(),QDate::currentDate().month(),1).addDays(-1));
    ui->toDateDateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->dateWidget->setHidden(true);

    AddressBookDatabaseHelper addHelper;
    QStringList routes ;
    routes<<"<Select>";
    routes<< addHelper.getAllRoutesForSearch();
    qDebug()<<"&&&&&"<<Q_FUNC_INFO<<routes;
    ui->routeComboBox->addItems(routes);
    ui->label->hide();
    ui->routeComboBox->hide();

//    setTableHeading();
    // setTable();
    this->setFocus();
    salesOrderHelper = new SalesOrderDatabaseHelper();

    ui->assignButton->setHidden(true);
    ui->editButton->setHidden(true);
    ui->copyButton->setHidden(true);
    ui->upButton->hide();
    ui->downButton->hide();
}

RestaurantPendingDelivery::~RestaurantPendingDelivery()
{
    delete ui;
}

void RestaurantPendingDelivery::calcTotals()
{
    TransactionCalculator::VoucherCalculateSales(voucher);

}


void RestaurantPendingDelivery::showDateWidget(bool v)
{
    ui->dateWidget->setHidden(!v);
}

void RestaurantPendingDelivery::setTable()
{
    QDate dateFrom = ui->fromDateDateTimeEdit->date();
    QDate dateTo = ui->toDateDateTimeEdit->date();



    model = salesOrderHelper->getVoucherListByDateQModel(dateFrom,dateTo,0,0,
                                                     "%",QString::number(QuotationStatus::DeliveryReady),
                                                     "");

    ui->tableView->setModel(model);

    float Total =0;
    for(int i=0;i<model->rowCount();i++){

        Total += model->record(i).value(10).toFloat();

    }
    ui->tableView->setItemDelegate(new PendingDeliveryDelegate());
    ui->tableView->setSelectionBehavior(QHeaderView::SelectRows);

//    ui->tableView->hideColumn(0);
    ui->tableView->setColumnWidth(1,25);
//    ui->tableView->hideColumn(2);

    ui->tableView->hideColumn(5);
    ui->tableView->hideColumn(6);

    ui->tableView->hideColumn(9);
    ui->tableView->hideColumn(11);
    ui->tableView->hideColumn(13);

    ui->tableView->horizontalHeader()->setSectionResizeMode(7,QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(8,QHeaderView::Stretch);

    ui->totalLineEdit->setAlignment(Qt::AlignRight);
    ui->totalLineEdit->setText(QString::number(Total,'f',2));
}





void RestaurantPendingDelivery::on_backButton_clicked()
{
    emit closeWindow();
}

void RestaurantPendingDelivery::on_copyButton_clicked()
{
    printVoucher(voucher);
}

void RestaurantPendingDelivery::on_editButton_clicked()
{
    emit selectedVoucher(voucher);
     emit closeWindow();
}

void RestaurantPendingDelivery::on_assignButton_clicked()
{

    AssignSalesman *AssignSalesmanWidget = new AssignSalesman(voucher,this);
    AssignSalesmanWidget->setWindowFlags(Qt::Window);
    AssignSalesmanWidget->setWindowTitle("Assign Employee");
    AssignSalesmanWidget->setGroupName("");
    AssignSalesmanWidget->setTable();
    QObject::connect(AssignSalesmanWidget,SIGNAL(selectEmployeeID(int)),
                     this,SLOT(SwitchSalesMan(int)));
    AssignSalesmanWidget->show();


}

void RestaurantPendingDelivery::on_OKButton_clicked()
{
    if(ui->voucherNoLineEdit->text().length()>0)
        on_voucherNoLineEdit_returnPressed();
    else
        setTable();
}

void RestaurantPendingDelivery::setBillPreview(GeneralVoucherDataObject *voucher)
{
    CounterPrinterHelper pHelper;
    QString html = pHelper.getHtml(voucher);
    ui->textEdit->setHtml(html);

}


void RestaurantPendingDelivery::SwitchSalesMan(int empid)
{
    salesOrderHelper->switchSalesMan(empid,voucher->voucherNumber,voucher->VoucherPrefix,QDateTime::currentDateTime());
    voucher->SalesmanID = empid;
    salesOrderHelper->setSalesOrderStatus(QuotationStatus::DeliveryCompleted,voucher->voucherNumber,voucher->VoucherPrefix);
//    CounterPrinterHelper pHelper;
//    for(int i=0;i<2;i++){
//        pHelper.printSalesOrder(voucher);
//    }
    setTable();
}

void RestaurantPendingDelivery::printVoucher(GeneralVoucherDataObject *voucher)
{
    bool val = ConfigurationSettingsDatabaseHelper::getValue(POSallowSOCopy,true).toBool();
    if(val){
        CounterPrinterHelper pHelper;
        pHelper.printSalesOrder(voucher);
    }
}

void RestaurantPendingDelivery::on_upButton_clicked()
{

}

void RestaurantPendingDelivery::on_downButton_clicked()
{

}

void RestaurantPendingDelivery::on_printButton_clicked()
{

}

void RestaurantPendingDelivery::on_voucherNoLineEdit_returnPressed()
{
    QString vNo = ui->voucherNoLineEdit->text();
    if(voucherNos.contains(vNo)){

        ui->editButton->setHidden(false);
        ui->copyButton->setHidden(false);
        ui->assignButton->setHidden(false);

        voucher = salesOrderHelper->getVoucherByVoucherNoPtr(vNo);
        setBillPreview(voucher);
    }
    ui->voucherNoLineEdit->clear();
}



void RestaurantPendingDelivery::on_routeComboBox_currentIndexChanged(const QString &arg1)
{
    route = arg1;
    // setTable();
}

void RestaurantPendingDelivery::on_tableView_clicked(const QModelIndex &index)
{
    int row = index.row();

    ui->assignButton->setHidden(false);
    ui->editButton->setHidden(false);
    ui->copyButton->setHidden(false);

    QString vNo = model->record(row).value(2).toString();
    QString vPref = model->record(row).value(1).toString();
    voucher = salesOrderHelper->getVoucherByVoucherNoPtr(vNo,vPref);
    calcTotals();
    setBillPreview(voucher);
}


void RestaurantPendingDelivery::showEvent(QShowEvent *event)
{
    setTable();
    QWidget::showEvent(event);
}
