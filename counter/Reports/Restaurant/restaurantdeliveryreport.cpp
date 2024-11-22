#include "restaurantdeliveryreport.h"
#include "ui_restaurantdeliveryreport.h"

#include "counter/PrinterHelper/counterprinterhelper.h"

#include <customwidgets/Delegates/Orders/pendingdeliverydelegate.h>

RestaurantDeliveryReport::RestaurantDeliveryReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RestaurantDeliveryReport)
{
    ui->setupUi(this);
    ui->fromDateDateTimeEdit->setDate(QDate(QDate::currentDate().year(),QDate::currentDate().month(),1).addDays(-1));
    ui->toDateDateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->dateWidget->setHidden(true);
    ui->upButton->hide();
    ui->downButton->hide();
//    ui->printButton->hide();

//    ui->printBill->setHidden(true);
    salesOrderHelper = new SalesOrderDatabaseHelper();

    employees = userHelper.getAllUsers();

    ui->filterComboBox->addItem("All",0);
    QMapIterator<int, QString> i(employees);
    while (i.hasNext()) {
        i.next();
        ui->filterComboBox->addItem(i.value(),i.key());
    }


    ui->cancelButton->setHidden(true);
    ui->editButton->setHidden(true);
    ui->copyButton->setHidden(true);
    ui->switchButton->setHidden(true);

    //    ui->settleButton->hide();
}

RestaurantDeliveryReport::~RestaurantDeliveryReport()
{
    delete ui;
}

void RestaurantDeliveryReport::showDateWidget(bool v)
{
    ui->dateWidget->setHidden(!v);
}

void RestaurantDeliveryReport::setTable()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    selectedRows.clear();
    QDate dateFrom = ui->fromDateDateTimeEdit->date();
    QDate dateTo = ui->toDateDateTimeEdit->date();
    int empID = ui->filterComboBox->currentData().toInt();
    qModel = salesOrderHelper->getVoucherListByDateQModel(dateFrom,dateTo,empID,0,
                                                          "%",QString::number(QuotationStatus::DeliveryCompleted),
                                                          "");

    QMap<int, Qt::CheckState> check_state_map1;
    model = new GMSQLQUeryModel(check_state_map1, 0);
    connect(model, &GMSQLQUeryModel::checked, this, &RestaurantDeliveryReport::checked);
    model->setQuery(qModel->query());

    ui->tableView->setModel(model);

    ui->tableView->setItemDelegate(new PendingDeliveryDelegate());
    ui->tableView->setSelectionBehavior(QHeaderView::SelectRows);
    float Total =0;
    for(int i=0;i<model->rowCount();i++){
        Total += model->record(i).value(10).toFloat();
    }

    //    ui->tableView->hideColumn(0);
    ui->tableView->setColumnWidth(1,25);

    //    ui->tableView->hideColumn(2);

    ui->tableView->hideColumn(5);
    ui->tableView->hideColumn(6);

    ui->tableView->hideColumn(9);
    ui->tableView->hideColumn(11);

    ui->tableView->horizontalHeader()->setSectionResizeMode(7,QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(8,QHeaderView::Stretch);

    ui->totalLineEdit->setAlignment(Qt::AlignRight);
    ui->totalLineEdit->setText(QString::number(Total,'f',2));

}





void RestaurantDeliveryReport::on_backButton_clicked()
{
    emit closeWindow();
}

void RestaurantDeliveryReport::on_copyButton_clicked()
{
    printVoucher(voucher);
}

void RestaurantDeliveryReport::on_editButton_clicked()
{
    emit selectedVoucher(voucher);
     emit closeWindow();
}

void RestaurantDeliveryReport::on_cancelButton_clicked()
{

}

void RestaurantDeliveryReport::on_OKButton_clicked()
{
    if(ui->voucherNoLineEdit->text().length()>0)
        on_voucherNoLineEdit_returnPressed();
    else
        setTable();
}

void RestaurantDeliveryReport::setBillPreview(GeneralVoucherDataObject *voucher)
{
    CounterPrinterHelper pHelper;
    qDebug()<<"setting printer";
    QString html = pHelper.getHtml(voucher);
    ui->textEdit->setHtml(html);
    qDebug()<<"Document Created";
}

void RestaurantDeliveryReport::on_switchButton_clicked()
{
    AssignSalesman *AssignSalesmanWidget = new AssignSalesman(voucher,this);
    AssignSalesmanWidget->setWindowFlags(Qt::Window);
    AssignSalesmanWidget->setWindowTitle("Assign Employee");
    AssignSalesmanWidget->setGroupName("Delivery");
    AssignSalesmanWidget->setTable();
    QObject::connect(AssignSalesmanWidget,SIGNAL(selectEmployeeID(int)),
                     this,SLOT(SwitchSalesMan(int)));
    AssignSalesmanWidget->show();

}

void RestaurantDeliveryReport::SwitchSalesMan(int empid)
{
    qDebug()<<empid<< "switch";
    salesOrderHelper->switchSalesMan(empid,voucher->voucherNumber,voucher->VoucherPrefix);
}

void RestaurantDeliveryReport::printVoucher(GeneralVoucherDataObject *voucher)
{
    if(ConfigurationSettingsDatabaseHelper::getValue(POSallowSOCopy,true).toBool()){
        CounterPrinterHelper pHelper;
        pHelper.printSalesOrder(voucher);
    }
}

void RestaurantDeliveryReport::on_upButton_clicked()
{

}

void RestaurantDeliveryReport::on_downButton_clicked()
{

}


void RestaurantDeliveryReport::on_settleButton_clicked()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    float Total = 0;
    for(int i=0; i<selectedRows.size(); i++){
        qDebug()<<"voucher: "<<qModel->record(selectedRows[i]).value(2);
        Total += qModel->record(selectedRows[i]).value(10).toFloat();
    }
    //        QStringList voucherNumbers;
    //    for(int i=0;i<ui->tableWidget->rowCount();i++){
    //        QCheckBox* check = qobject_cast <QCheckBox*>(ui->tableWidget->cellWidget(i,6));
    //        if(check->isChecked()){
    //            voucherNumbers << ui->tableWidget->item(i,2)->text();
    //        }
    //    }
    if(selectedRows.size()==0){
        QMessageBox box; box.setText("Select the orders to settle"); box.exec();
    }
    else{
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this,"Settle Delivery Bills",
                                      "Are you sure you want to settle Checked Bills of Value "+QString::number(Total,'f',2),
                                      QMessageBox::Yes|QMessageBox::No);
        if(reply==QMessageBox::Yes){
            for(int i=0;i<selectedRows.size();i++){
                QString voucherNo =qModel->record(selectedRows[i]).value(2).toString();
                QString voucherPrefix = qModel->record(selectedRows[i]).value(1).toString();
                qDebug()<<Q_FUNC_INFO<<__LINE__<<voucherNo<<voucherPrefix;

                GeneralVoucherDataObject *obj = salesOrderHelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);
                qDebug()<<"Got item : "<<obj->SalesmanID<<obj->voucherNumber<<obj->VoucherPrefix;
                obj->AddedBy = QString::number(LoginValues::getUserID());
                obj->ledgerObject = ledHelper.getLedgerObjectByID(CounterSettingsDataModel::DefaultCash);
                obj->status = QuotationStatus::SalesInvoiceRaised;
                obj->ConvertedToSalesOrder = voucherNo;

                qDebug()<<obj->ConvertedToSalesOrder<<Q_FUNC_INFO<<__LINE__;
                obj->VoucherDate = QDate::currentDate();
                obj->timestamp = QDateTime::currentDateTime();
                TransactionCalculator::VoucherCalculateSales(obj);
                bool saveStatus = salesVoucherHelper.insertVoucher(obj);
                if(saveStatus){
                    salesOrderHelper->setSalesOrderStatus(QuotationStatus::SalesInvoiceRaised,
                                                          voucherNo,obj->VoucherPrefix);
                }
                //                    qDebug()<<"Save completed for "<<obj->voucherNumber;
                //                    salesOrderHelper->deleteVoucher(obj);
            }
        }
    }
    // qDebug()<<Q_FUNC_INFO<<__LINE__;
    // qDebug()<<"Selected"<<selectedRows;
    CounterPrinterHelper pHelper;
    pHelper.printSettlementReportWithCheck(ui->tableView,ui->filterComboBox->currentText(),Total,selectedRows);
    setTable();
}


void RestaurantDeliveryReport::calcTotals(GeneralVoucherDataObject *obj)
{

    TransactionCalculator::VoucherCalculateSales(voucher);
}


void RestaurantDeliveryReport::on_printButton_clicked()
{
    CounterPrinterHelper pHelper;
    pHelper.printSettlementReport(ui->tableView,ui->filterComboBox->currentText(),
                                               ui->totalLineEdit->text().toFloat());
}

void RestaurantDeliveryReport::on_voucherNoLineEdit_returnPressed()
{
    QString vNo = ui->voucherNoLineEdit->text();
    if(voucherNos.contains(vNo)){

        ui->cancelButton->setHidden(false);
        ui->editButton->setHidden(false);
        ui->copyButton->setHidden(false);
        ui->switchButton->setHidden(false);

        voucher = salesOrderHelper->getVoucherByVoucherNoPtr(vNo);
        setBillPreview(voucher);
    }
    ui->voucherNoLineEdit->clear();
}

void RestaurantDeliveryReport::on_filterComboBox_currentIndexChanged(int index)
{
    setTable();
}



void RestaurantDeliveryReport::on_tableView_clicked(const QModelIndex &index)
{
    int row = index.row();

    ui->cancelButton->setHidden(false);
    ui->editButton->setHidden(false);
    ui->copyButton->setHidden(false);
    ui->switchButton->setHidden(false);
    ui->printBill->setHidden(false);

    QString vNo = model->record(row).value(2).toString();
    QString vPref = model->record(row).value(1).toString();
    voucher = salesOrderHelper->getVoucherByVoucherNoPtr(vNo,vPref);
    TransactionCalculator::VoucherCalculateSales(voucher);
    setBillPreview(voucher);
}

void RestaurantDeliveryReport::on_printBill_clicked()
{
    emit selectedVoucher(voucher);
    emit directCheckOut();
}

void RestaurantDeliveryReport::checked(int row, bool checkState)
{
    if(checkState){
        selectedRows.append(row);
    }
    else{
        selectedRows.removeOne(row);
    }

}

void RestaurantDeliveryReport::on_selectALL_clicked()
{
    if(ui->selectALL->isChecked()){
        for(int i=0;i<model->rowCount();i++){
            model->markChecked(i,true);
        }
        ui->selectALL->setText("Deselect All");
    }
    else{
        for(int i=0;i<model->rowCount();i++){
            model->markChecked(i,false);
        }
        ui->selectALL->setText("Select All");
    }

}



void RestaurantDeliveryReport::showEvent(QShowEvent *event)
{
    setTable();
    QWidget::showEvent(event);
}
