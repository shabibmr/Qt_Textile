#include "todaysbills.h"
#include "ui_todaysbills.h"
#include "sharetools/email/emailthread.h"

#include "customwidgets/appbarwidget.h"
#include <QHeaderView>
TodaysBills::TodaysBills(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TodaysBills)
{
    ui->setupUi(this);

    ui->label->hide();

    AppBarWidget *appBar = new  AppBarWidget("Today's Bills");
    connect(appBar, &AppBarWidget::backButtonClicked, this, &TodaysBills::on_backButton_clicked);
    connect(appBar, &AppBarWidget::shareButtonClicked, this, &TodaysBills::showShareOptions);

    ui->appBarLayout->addWidget(appBar);

    QPalette pal = palette();
    pal.setColor(QPalette::Background,QColor("#ffffff"));
    setAutoFillBackground(true);
    setPalette(pal);

    tableView = new QTableView(this);
    ui->tableLayout->addWidget(tableView);

    ui->fromDateDateTimeEdit->setDate(QDate::currentDate());
    ui->toDateDateTimeEdit->setDateTime(QDateTime::currentDateTime());
    salesHelper = new salesVoucherDatabaseHelper();
    userHelper = new UserProfileDatabaseHelper();
    ledHelper = new LedgerMasterDatabaseHelper();
    deletedHelper = new salesVoucherDeletedDatabaseHelper();

    sort = new GMItemSortFilterProxyModel(this);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(tableView, &QTableView::clicked, this,&TodaysBills::on_tableView_clicked);

    ui->cancelButton->hide();
    ui->printButton->hide();
    setTable();
    this->setFocus();
    ui->cancelButton->setHidden(true);
    ui->editButton->setHidden(true);
    ui->copyButton->setHidden(true);
    ui->switchButton->setHidden(true);


    ui->upButton->hide();
    ui->downButton->hide();
    if(LoginValues::Privilege<3){
         ui->totalLabel->hide();
         ui->totalLineEdit->hide();
    }
}

TodaysBills::~TodaysBills()
{
    delete ui;
}

void TodaysBills::showDateWidget(bool v)
{
    ui->dateWidget->setHidden(!v); //change
}

void TodaysBills::setTable()
{
    qDebug()<<"setTable started";

    QDate dateFrom = ui->fromDateDateTimeEdit->date();    

    QDate dateTo = ui->toDateDateTimeEdit->date();

    ShiftDatabaseHelper shift;

    QDateTime dfrom = shift.getlastShiftDetailByVoucherPrefix(LoginValues::voucherPrefix).ShiftEndDateTime;

    if(shift.getlastShiftDetailByVoucherPrefix(LoginValues::voucherPrefix).ShiftEndDateTime.isNull())
        dfrom = QDateTime(dateFrom);

    QDate toDate = ui->toDateDateTimeEdit->date().addDays(CounterSettingsDataModel::EndDay);

    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));


    model = salesHelper->getVoucherListByDateTimeasModel(dfrom,toDateTime,LoginValues::voucherPrefix);
    float Total =0;

    for(int i=0;i<model->rowCount();i++){
        Total += model->record(i).value(7).toFloat();
    }

    ui->totalLineEdit->setAlignment(Qt::AlignRight);
    ui->totalLineEdit->setText(QString::number(Total,'f',2));

    sort->setDynamicSortFilter(true);
    sort->setSourceModel(model);
    sort->sort(0);

    tableView->setModel(sort);
    tableView->setSortingEnabled(true);
    tableView->setItemDelegate(new todaysBillsDelegate);
    tableView->hideColumn(1);

//    tableView->setAvlcols(QList<int> {7});
//    tableView->setColumns();

    bool show = ConfigurationSettingsDatabaseHelper::getValue(POSshowLedger,true).toBool();
    if(!show){
        tableView->hideColumn(3);
    }

    // tableView->lastRow->hide();



    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    for(int i=0;i<tableView->model()->rowCount();i++){
        voucherNos<<tableView->model()->index(i,0).data().toString();
    }
    tableView->verticalHeader()->show();
    // tableView->showVerticalHeader(true);
    qDebug()<<"VISIBILITY :     "<<tableView->verticalHeader()->isVisible();

}


void TodaysBills::setTitle(QString title)
{
    ui->label->setText(title);
}



void TodaysBills::on_backButton_clicked()
{
    this->close();
}

void TodaysBills::on_copyButton_clicked()
{
//    int row = ui->tableWidget->currentRow();
    CounterPrinterHelper *pHelper = new CounterPrinterHelper;
    pHelper->printSalesVoucher(voucher);

}

void TodaysBills::on_editButton_clicked()
{

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Edit","Edit Voucher : "+ voucher->voucherNumber,
                                  QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::Yes){
//        if(LoginValues::Privilege >= 6){
            emit selectedVoucher(voucher);
            this->close();
//        }
//        else{
//            QMessageBox box; box.setText("Sorry! You Do not have Privileges for the operation!!!");
//            box.exec();
//        }
    }
    else{
        qDebug()<<"Not Printing";
    }
}

void TodaysBills::on_cancelButton_clicked()
{


    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Delete","Delete Voucher : "+ voucher->voucherNumber,
                                  QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::Yes){
//        if(LoginValues::Privilege >= 6){
            salesHelper->deleteVoucher(voucher);
            deletedHelper->insertVoucher(voucher);
            MailDeletedList("");
            setTable();
//        }
//        else{
//            QMessageBox box; box.setText("Sorry! You Do not have Privileges for the operation!!!");
//            box.exec();
//        }
    }
    else{
        qDebug()<<"Not Printing";
    }


}

void TodaysBills::MailDeletedList(QString name){
    QString text="";
    text+=LoginValues::company.CompanyName+"\n";
    text+="Branch:\t"+LoginValues::company.branch+"\n";
    text+="Time:\t"+QDateTime::currentDateTime().toString()+"\n\n";
    if(voucher->narration.length()>0){
        text += " Message : "+voucher->narration+"\n\n";
    }
//    text+= "Item Name - Quantity\n";

//    int i;
//    for(i=0;i<voucher->InventoryItems.size();i++){
//        text+= voucher->InventoryItems[i].BaseItem.ItemName+" - "+QString::number(voucher->InventoryItems[i].BaseItem.quantity)+"\n";
//    }

    CounterPrinterHelper printer;
    text += printer.getHtml(voucher);
    text+= "Cashier :\t"+LoginValues::empName;

    EmailThread* mail = new EmailThread();
    mail->subject = LoginValues::company.branch+ " - BILL CANCELLED - "+voucher->voucherNumber;
    mail->Message=text;
    mail->setToAddress(LoginValues::getCompany().toMailAddresses.split("|")[0]);
    mail->run();
}

void TodaysBills::disableEdit()
{

    ui->editButton->setDisabled(true);
    ui->editButton->setStyleSheet("background-color:#ffffff");
}

void TodaysBills::disableDelete()
{
    ui->cancelButton->setDisabled(true);
    ui->cancelButton->setStyleSheet("background-color:#ffffff");
}

void TodaysBills::disablePrint()
{
    ui->printButton->hide();
}

void TodaysBills::setBillPreview(GeneralVoucherDataObject *voucher)
{
    CounterPrinterHelper pHelper;
    QString html = pHelper.getHtml(voucher);
    ui->textEdit->setHtml(html);
    qDebug()<<"Document Created";
}

void TodaysBills::printVoucher(GeneralVoucherDataObject *voucher)
{
    CounterPrinterHelper pHelper;
    for(int i=0;i<voucher->NoOfCopies;i++){
        pHelper.printSalesVoucher(voucher);
    }
}

void TodaysBills::on_upButton_clicked()
{

}

void TodaysBills::on_downButton_clicked()
{

}

void TodaysBills::on_printButton_clicked()
{


}

void TodaysBills::on_switchButton_clicked()
{
    AssignSalesman *AssignSalesmanWidget = new AssignSalesman(voucher,this);
    AssignSalesmanWidget->setWindowFlags(Qt::Window);
    AssignSalesmanWidget->setWindowTitle("Assign Employee");
    AssignSalesmanWidget->setGroupName("");

    AssignSalesmanWidget->setTable();
    AssignSalesmanWidget->show();

    QObject::connect(AssignSalesmanWidget,SIGNAL(selectEmployeeID(int)),
                     this,SLOT(SwitchSalesMan(int)));
}

void TodaysBills::on_voucherNoLineEdit_returnPressed()
{
    QString vNo = ui->voucherNoLineEdit->text();
    if(voucherNos.contains(vNo)){
        ui->cancelButton->setHidden(false);
        ui->editButton->setHidden(false);
        ui->copyButton->setHidden(false);
        ui->switchButton->setHidden(false);
        voucher = salesHelper->getVoucherByVoucherNoPtr(vNo, LoginValues::voucherPrefix);
        setBillPreview(voucher);
    }
    ui->voucherNoLineEdit->clear();
}

void TodaysBills::on_OKButton_clicked()
{
    if(ui->voucherNoLineEdit->text().length()>0)
        on_voucherNoLineEdit_returnPressed();
    else
        setTable();
}

void TodaysBills::SwitchSalesMan(int id)
{
    qDebug()<<"changing to "<<id;
    salesHelper->switchSalesMan(id,voucher->voucherNumber, voucher->VoucherPrefix);
    setTable();
}

void TodaysBills::on_switchModeofPay_clicked()
{
    ModeOfPay* ModeOfPayWidget = new ModeOfPay(voucher,ledHelper,new SalesOrderDatabaseHelper(),true);
    ModeOfPayWidget->hideDiscounts();
    ModeOfPayWidget->show();
    QObject::connect(ModeOfPayWidget,SIGNAL(modeOfPaySignal()),this,SLOT(switchModeOfPay()));
}

void TodaysBills::calcTotals()
{
   TransactionCalculator::VoucherCalculateSales(voucher);
}

void TodaysBills::switchModeOfPay(){

    calcTotals();
    salesHelper->updateVoucher(voucher,voucher->voucherNumber);
    CounterPrinterHelper pHelper;
    pHelper.printSalesVoucher(voucher);
    setTable();

}

void TodaysBills::setWidgets()
{

}


void TodaysBills::on_tableView_clicked(const QModelIndex &index)
{
    ui->cancelButton->setHidden(false);
    ui->editButton->setHidden(false);
    ui->copyButton->setHidden(false);
    ui->switchButton->setHidden(false);

    int row = sort->mapToSource(index).row();
    QString voucherNo = model->record(row).value(0).toString();
    QString voucherPrefix = model->record(row).value(1).toString();

    voucher = salesHelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);
    calcTotals();
    setBillPreview(voucher);
}


void TodaysBills::showShareOptions()
{
    shareOptionsWidget = new ShareTools("Todays Sales Report", shareButton);
    shareOptionsWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup|Qt::WindowStaysOnTopHint);
    shareOptionsWidget->show();

    QObject::connect(shareOptionsWidget,&ShareTools::printClicked,
                     this,&TodaysBills::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::pdfClicked,
                     this,&TodaysBills::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::excelClicked,
                     this,&TodaysBills::on_excelButton_clicked);
}

void TodaysBills::printReport(QPrinter *printer)
{
    printer->setPrinterName("printerName");
    printerHelper = new CounterPrinterHelper;
    printerHelper->setPrinter(printer);


    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QString total = ui->totalLineEdit->text();
    QDate fromDate = QDate::currentDate();
    QDate toDate = QDate::currentDate();
    extraBottom<< "Total: "<<""<<"" <<""<<""<<""<<""<<total;
    QList<int> avlCols; avlCols<<0<<2<<2<<7;
    widthList <<20<<20<<40<<20;
    AlignmentList<< 0<< 1 << 0 <<1;
    qDebug()<<extraBottom;
    printerHelper-> printReportGeneral(tableView
                                      ,widthList,"Sales Report",fromDate,toDate,AlignmentList
                                      ,extraTop,extraBottom,avlCols);

}

void TodaysBills::on_excelButton_clicked()
{
    qDebug()<<"create Excel";
    QDate fDate = QDate::currentDate();
    QDate tDate = QDate::currentDate();
    QString fromDate = fDate.toString("yyyy-MM-dd");
    QString toDate = tDate.toString("yyyy-MM-dd");
    QString wwaitername ="";

    QString reportName = "Todays Sales Report";

    QDate curDate = QDate::currentDate();
    QString dateNow = curDate.toString("dd_MMM_yyyy");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\"+reportName+"_"+dateNow+".xlsx",tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
    {
        return;
    }


    ExcelReportHelper *ex = new ExcelReportHelper;

    QStringList cols; cols<<"30"<<"20"<<"20"<<"20";
    reportName = "Billwise Sales Report "+wwaitername;
     QList<int> avlCols; avlCols<<0<<2<<2<<7;
    ex->GenericReportFromTableView(fileName,reportName,cols,tableView,fromDate,toDate,avlCols);
}
