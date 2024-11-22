#include "restaurantsalesordersreport.h"
#include "ui_restaurantsalesordersreport.h"
#include <QSqlRecord>
#include "customwidgets/Delegates/reportcommondelegate.h"
#include "counter/MainScreen/modeofpay.h"
#include "inventory/reports/ItemwiseReport/itemwisesalesorderbymode.h"
#include <QHBoxLayout>
#include "counter/MainScreen/tableselect.h"

#include <customwidgets/Delegates/Orders/pendingdeliverydelegate.h>
RestaurantSalesOrdersReport::RestaurantSalesOrdersReport(salesVoucherDeletedDatabaseHelper *deletedSalesHelper1,
                                                         SalesOrderDatabaseHelper *soHelper,
                                                         AccountGroupMasterDatabaseHelper *accHelper,
                                                         LedgerMasterDatabaseHelper *ledHelper,
                                                         QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RestaurantSalesOrdersReport)
{
    ui->setupUi(this);
    deletedSalesHelper = deletedSalesHelper1;
    this->salesOrderHelper = soHelper;
    this->accHelper = accHelper;
    this->ledHelper = ledHelper;
    ui->fromDateDateTimeEdit->setDate(QDate(2024,1,1));
    ui->toDateDateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->dateWidget->setHidden(true);

    employees = userHelper.getAllUsers();
    prices = pHelper.getAllPriceLists(false);

    sort= new GMItemSortFilterProxyModel();
    ui->filterComboBox->addItem("All",0);
    ui->serviceCombo->addItem("All",0);
    QMapIterator<int, QString> i(employees);
    while (i.hasNext()) {
        i.next();
        ui->filterComboBox->addItem(i.value(),i.key());
    }
    int rowHeight = ConfigurationSettingsDatabaseHelper::getValue(tableRowHeight, 30).toInt();

    QHeaderView *verticalHeader = ui->tableView->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(rowHeight);

    int PreviewWidthValue = ConfigurationSettingsDatabaseHelper::getValue(billPreviewWidth, 35).toInt();

    if(PreviewWidthValue>100)
        PreviewWidthValue=35;

    ui->mainLayout->setStretch(0,PreviewWidthValue);
    ui->mainLayout->setStretch(1,100-PreviewWidthValue);

    multiVoucher = ConfigurationSettingsDatabaseHelper::getValue(multiVoucherFlag, true).toBool();
    QString rcss = "QRadioButton {";
    rcss += "border: 2px solid gray; ";/* Define border style and color */
    rcss += "border-radius: 5px; ";
    rcss += "background-color: white;";  /* Set default background color */
    rcss += "padding: 5px;";  /* Add some padding for better visibility */
    rcss += "}";

    rcss += "QRadioButton::indicator {";
    rcss += "width: 0px;";
    rcss += "height: 0px;";  /* Hide the indicator */
    rcss += "}";

    rcss += "QRadioButton:checked {";
    rcss += "    background-color: lightblue;"; /* Set background color for selected state */
    rcss += "}";

    ui->routeComboBox->blockSignals(true);
    ui->filterComboBox->blockSignals(true);
    ui->serviceCombo->blockSignals(true);

    QFont fnt;fnt.setBold(true);
    QRadioButton *b =new QRadioButton(this);
    b->setFont(fnt);
    b->setText("ALL");
    b->setProperty("id",0);
    b->setStyleSheet(rcss);
    ui->horizontalLayout_7->addWidget(b);
    radioList.append(b);

    connect(b,QRadioButton::clicked,this,[=](bool checked){
        if(checked ){
            plistID =0;
        }
        setTable();
    });
    b->setChecked(true);
    for(int i=0;i<prices.length();i++){
        // ui->serviceCombo->addItem(prices[i].priceListName,prices[i].priceListID);
        QRadioButton *r = new QRadioButton(this);
        r->setText(prices[i].priceListName);
        r->setStyleSheet(rcss);
        r->setFont(fnt);
        r->setProperty("id",prices[i].priceListID);
        ui->horizontalLayout_7->addWidget(r);
        radioList.append(r);
        connect(r,QRadioButton::clicked,this,[=](bool checked){
            if(checked){
                plistID=prices[i].priceListID;
            }
            setTable();
        });

    }

    if(ConfigurationSettingsDatabaseHelper::getValue(showRouteFilterInDrafts, false).toBool()){
        ui->routeComboBox->addItem("All",0);

        AddressBookDatabaseHelper addHelper;
        ui->routeComboBox->addItems(addHelper.getAllRoutesForSearch());

    }
    else {
        ui->label_3->hide();
        ui->routeComboBox->hide();
    }

    QFile inputFile2("kot.txt");
    if (inputFile2.open(QIODevice::ReadOnly)){
        ui->printBill->hide();
        ui->modePay->hide();
    }

    ui->cancelButton->setHidden(true);
    ui->editButton->setHidden(true);
    ui->copyButton->setHidden(true);


    ui->printBill->setHidden(true);

    ui->switchButton->setHidden(true);
    ui->switchTableButton->setHidden(true);

    ui->serviceCombo->hide();


    ui->routeComboBox->blockSignals(false);
    ui->filterComboBox->blockSignals(false);
    ui->serviceCombo->blockSignals(false);

    // setTable();
}

RestaurantSalesOrdersReport::~RestaurantSalesOrdersReport()
{
    delete ui;
}

void RestaurantSalesOrdersReport::setPlistID(int newPlistID)
{
    plistID = newPlistID;
    for(int i=0;i<radioList.length();i++){
        if(radioList[i]->property("id").toInt()==newPlistID){
            radioList[i]->click();
        }
    }
}

void RestaurantSalesOrdersReport::calcTotals()
{
    TransactionCalculator::VoucherCalculateSales(voucher);

}


void RestaurantSalesOrdersReport::showDateWidget(bool v)
{
    ui->dateWidget->setHidden(!v);
}

void RestaurantSalesOrdersReport::setTable()
{
    QDate dateFrom = ui->fromDateDateTimeEdit->date();
    QDate dateTo = QDate::currentDate();//ui->toDateDateTimeEdit->date();
    int filterID = ui->filterComboBox->currentData().toInt();
    int plistID =this->plistID;// ui->serviceCombo->currentData().toInt();
    QString route = ui->routeComboBox->currentIndex()>0? ui->routeComboBox->currentText():"%";

    //    voucherList = salesOrderHelper.getVoucherListByDate(dateFrom,dateTo);

    bool showAllDraftsFlag = ConfigurationSettingsDatabaseHelper::getValue(showAllDrafts, true).toBool();
    QString vpref = showAllDraftsFlag == true ? "%": LoginValues::voucherPrefix;

    model = salesOrderHelper->getVoucherListByDateQModel(dateFrom,dateTo,filterID,plistID,route,
                                                         "",
                                                         QString::number(QuotationStatus::SalesInvoiceRaised)+","+QString::number(QuotationStatus::SalesOrderDeliveryCreated),
                                                         vpref);
    // qDebug()<<Q_FUNC_INFO;
    sort->setSourceModel(model);
    sort->setDynamicSortFilter(true);
    ui->tableView->setModel(sort);
    ui->tableView->setSortingEnabled(true);

    ui->tableView->verticalHeader()->show();

    ui->tableView->setItemDelegate(new PendingDeliveryDelegate());

    if(!multiVoucher)
        ui->tableView->hideColumn(1);

    ui->tableView->hideColumn(11);
    // ui->tableView->hideColumn(12);
    ui->tableView->hideColumn(13);

    // ledger
    ui->tableView->hideColumn(6);


    bool routeFlag = ConfigurationSettingsDatabaseHelper::getValue(showRouteFilterInDrafts, false).toBool();
    if(routeFlag)
        ui->tableView->hideColumn(5);

    bool val = ConfigurationSettingsDatabaseHelper::getValue(POSDrafttableHideCol,true).toBool();
    if(val)
    {
        ui->tableView->hideColumn(7);
        ui->tableView->hideColumn(8);
        ui->tableView->hideColumn(9);
    }

    float Total=0;

    voucherNos.clear();

    int colcnt =  model->columnCount()-1;

    for(int i=0;i<model->rowCount();i++){
        voucherNos.append(model->record(i).value(2).toString());
        Total += model->record(i).value(10).toFloat();
    }

    ui->tableView->setColumnWidth(2,60);
    ui->tableView->setColumnWidth(1,25);
    ui->tableView->setColumnWidth(0,80);
    ui->tableView->setColumnWidth(4,80);
    ui->tableView->setColumnWidth(5,100);
    ui->tableView->setColumnWidth(7,80);

    //    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->tableView->horizontalHeader()->setSectionResizeMode(5,QHeaderView::Stretch);
    ui->totalLineEdit->setAlignment(Qt::AlignRight);
    ui->totalLineEdit->setText(QString::number(Total,'f',2));

    qDebug()<<"View Set";

}



void RestaurantSalesOrdersReport::on_backButton_clicked()
{
    emit closeWindow();
    // this->close();
}

void RestaurantSalesOrdersReport::on_copyButton_clicked()
{
    printVoucher(voucher);
}

void RestaurantSalesOrdersReport::on_editButton_clicked()
{
    emit selectedVoucher(voucher);
    emit closeWindow();
    // this->close();
}

void RestaurantSalesOrdersReport::on_cancelButton_clicked()
{
    QMessageBox::StandardButton button =
        QMessageBox::question(this,"Delete Order","Are you Sure You want to Delete voucher",QMessageBox::Yes|
                                                                                                   QMessageBox::No);
    if(button == QMessageBox::Yes){
        DeleteSalesOrder();
    }
}

void RestaurantSalesOrdersReport::on_OKButton_clicked()
{
    if(ui->voucherNoLineEdit->text().length()>0)
        on_voucherNoLineEdit_returnPressed();
    else
        setTable();
}

void RestaurantSalesOrdersReport::setBillPreview(GeneralVoucherDataObject *voucher)
{
    CounterPrinterHelper pHelper;
    QString html = pHelper.getHtml(voucher);
    ui->textEdit->setHtml(html);
}


void RestaurantSalesOrdersReport::on_switchButton_clicked()
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

void RestaurantSalesOrdersReport::SwitchSalesMan(int empid)
{
    //    qDebug()<<empid<< "switch";
    salesOrderHelper->switchSalesMan(empid,voucher->voucherNumber,voucher->VoucherPrefix);
}

void RestaurantSalesOrdersReport::DeleteSalesOrder()
{
    passWordShiftDialog = new QDialog;
    passWordShiftDialog->setFixedSize(300,150);
    passWordShiftDialog->setWindowTitle("Password");
    passWordShiftDialog->setWindowFlags(Qt::WindowStaysOnTopHint);
    QHBoxLayout *hLayout = new QHBoxLayout(passWordShiftDialog);
    textLabel = new QLabel(passWordShiftDialog);
    textLabel->setGeometry(QRect(50, 20, 221, 31));
    textLabel->clear();
    //  textLabel->setText("SWIPE YOUR CARD!!!");
    passwordShiftLine = new QLineEdit(passWordShiftDialog);
    passwordShiftLine->setEchoMode(QLineEdit::Password);
    passwordShiftLine->setFixedSize(150,25);
    passwordShiftLine->setGeometry(QRect(60, 60, 150, 25));
    QFont font;
    font.setPointSize(11);
    passwordShiftLine->setFont(font);
    passwordShiftLine->setPlaceholderText("Enter Password Here");
    passwordShiftLine->setFocus();
    // passwordShift->setHidden(true);
    QPushButton *okayButton = new QPushButton(passWordShiftDialog);
    okayButton->setFixedSize(70,25);
    okayButton->setGeometry(QRect(200, 60, 70, 25));
    okayButton->setText("OKAY");
    connect(okayButton,SIGNAL(clicked()),this,SLOT(passwordEntered()));
    passWordShiftDialog->setLayout(hLayout);
    passWordShiftDialog->exec();
}

void RestaurantSalesOrdersReport::passwordEntered()
{
    QString password = passwordShiftLine->text();
    QString userpass;
    QString qStr = "SELECT password,privilege from employee_details";
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    bool flag = false;
    if(query.exec(qStr)){
        while(query.next()){
            if(query.value(0).toString() == password && password.length() > 0
                && query.value(1).toInt() >= 5 ){
                flag = true;
            }
        }
    }
    if(flag == true ){
        passWordShiftDialog->close();
        salesOrderHelper->deleteVoucher(voucher);
        TransactionCalculator::VoucherCalculateSales(voucher);
        deletedSalesHelper->insertVoucher(voucher);
        MailDeletedList("");
    }

}

void RestaurantSalesOrdersReport::MailDeletedList(QString name){
    QString text="";
    text+=LoginValues::company.CompanyName+"\n";
    text+="Branch:\t"+LoginValues::company.branch+"\n";
    text+="Time:\t"+QDateTime::currentDateTime().toString()+"\n\n";
    if(voucher->narration.length()>0){
        text += " Message : "+voucher->narration+"\n\n";
    }
    text+= "Item Name - Quantity\n";

    int i;
    for(i=0;i<voucher->InventoryItems.size();i++){
        text+= voucher->InventoryItems[i].BaseItem.ItemName+" - "+QString::number(voucher->InventoryItems[i].BaseItem.quantity)+"\n";
    }

    text+= "Cashier:\t"+LoginValues::empName;

    EmailThread* mail = new EmailThread();
    mail->subject = LoginValues::company.branch+ " - ORDER CANCELLED - "+voucher->voucherNumber;
    mail->Message=text;
    mail->setToAddress(LoginValues::getCompany().toMailAddresses.split("|")[0]);
    mail->run();
}


void RestaurantSalesOrdersReport::printVoucher(GeneralVoucherDataObject *voucher)
{
    bool flag = ConfigurationSettingsDatabaseHelper::getValue(POSallowSOCopy,true).toBool();
    if(flag){
        CounterPrinterHelper pHelper;
        pHelper.printSalesOrder(voucher);
    }

}



void RestaurantSalesOrdersReport::on_printButton_clicked()
{
    showShareOptions();
}

void RestaurantSalesOrdersReport::showShareOptions()
{
    shareOptionsWidget = new ShareTools("Itemwise Sales Report", ui->printButton);
    shareOptionsWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup|Qt::WindowStaysOnTopHint);;
    //    shareOptionsWidget->setAttribute(Qt::WA_DeleteOnClose);
    shareOptionsWidget->show();

    QObject::connect(shareOptionsWidget,&ShareTools::printClicked,
                     this,&RestaurantSalesOrdersReport::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::pdfClicked,
                     this,&RestaurantSalesOrdersReport::printReport);

    QObject::connect(shareOptionsWidget,&ShareTools::excelClicked,
                     this,&RestaurantSalesOrdersReport::on_excelButton_clicked);

}

void RestaurantSalesOrdersReport::on_excelButton_clicked()
{

    QDate fDate = ui->fromDateDateTimeEdit->date();
    QDate tDate = ui->toDateDateTimeEdit->date();
    QString fromDate = fDate.toString("yyyy-MM-dd");
    QString toDate = tDate.toString("yyyy-MM-dd");


    QString reportName = "Sales  Order List";

    QDate curDate = QDate::currentDate();
    QString dateNow = curDate.toString("dd_MMM_yyyy");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\"+reportName+"_"+dateNow+".xlsx",tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
    {
        return;
    }


    ExcelReportHelper *ex = new ExcelReportHelper;

    QStringList cols; cols<<"20"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20";
    //    reportName = "Itemwise Sales Report";
    QList<int> avlCols; avlCols <<1<<3<<4<<8<<11<<7<<9<<10;
    ex->GenericReportFromTableView(fileName,reportName,cols,ui->tableView,fromDate,toDate,avlCols);
}

void RestaurantSalesOrdersReport::printReport(QPrinter *printer)
{
    CounterPrinterHelper printerHelper;
    printerHelper.setPrinter(printer);
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QString total = ui->totalLineEdit->text();
    QDate fromDate = ui->toDateDateTimeEdit->date();
    QDate toDate = ui->toDateDateTimeEdit->date();
    extraBottom<< "Total: " <<total;
    QList<int> avlCols;
    avlCols <<1<<3<<4<<5<<6<<7<<9<<10;
    widthList <<20<<20<<20<<20<<20<<20<<20<<20;
    AlignmentList<<0 << 0<<0<< 0 <<1;

    printerHelper.printReportGeneralwithModel(model, widthList,"Sales Order List",fromDate,toDate,AlignmentList
                                              ,extraTop,extraBottom,avlCols);
}


void RestaurantSalesOrdersReport::on_voucherNoLineEdit_returnPressed()
{
    QString vNo = ui->voucherNoLineEdit->text();
    if(voucherNos.contains(vNo))
    {
        ui->cancelButton->setHidden(false);
        ui->editButton->setHidden(false);
        ui->copyButton->setHidden(false);
        ui->switchButton->setHidden(false);
        ui->switchTableButton->setHidden(false);
        voucher = salesOrderHelper->getVoucherByVoucherNoPtr(vNo);
        setBillPreview(voucher);
    }
    ui->voucherNoLineEdit->clear();
}

void RestaurantSalesOrdersReport::on_filterComboBox_currentIndexChanged(int index)
{
    setTable();
}

void RestaurantSalesOrdersReport::on_printBill_clicked()
{

    emit selectedVoucher(voucher);
    emit directCheckOut();
    emit closeWindow();
}

void RestaurantSalesOrdersReport::on_tableView_clicked(const QModelIndex &index)
{
    ui->cancelButton->setHidden(false);
    ui->editButton->setHidden(false);
    ui->copyButton->setHidden(false);
    ui->switchButton->setHidden(false);

    QFile inputFile2("kot.txt");
    if (inputFile2.open(QIODevice::ReadOnly)){
        ui->printBill->hide();
        ui->modePay->hide();
    }
    else{
        ui->printBill->setHidden(false);
    }
    ui->switchTableButton->setHidden(false);

    int row = sort->mapToSource(index).row();

    QString vNo = model->record(row).value(2).toString();
    QString vPref = model->record(row).value(1).toString();
    voucher = salesOrderHelper->getVoucherByVoucherNoPtr(vNo,vPref);
    calcTotals();
    setBillPreview(voucher);

}

void RestaurantSalesOrdersReport::on_modePay_clicked()
{
    if(voucher->voucherNumber.length()==0)
        return;

    ModeOfPay* ModeOfPayWidget = new ModeOfPay(voucher,new LedgerMasterDatabaseHelper(),new SalesOrderDatabaseHelper(),false, this);
    ModeOfPayWidget->setWindowFlags(Qt::Window);
    ModeOfPayWidget->show();
    ModeOfPayWidget->setAttribute(Qt::WA_DeleteOnClose);

    QObject::connect(ModeOfPayWidget,&ModeOfPay::modeOfPaySignal,this,[=](){
        qDebug()<<"Voucher ledger Name :"<<voucher->ledgerObject.LedgerName;
        emit VoucherModeSelected(voucher);
        setTable();
    });
}

void RestaurantSalesOrdersReport::on_serviceCombo_currentIndexChanged(int index)
{
    setTable();
}

void RestaurantSalesOrdersReport::on_itemwiseButton_clicked()
{
    ItemwiseSalesOrderByMode *orderlist = new ItemwiseSalesOrderByMode(this);
    orderlist->setWindowFlags(Qt::Window);
    orderlist->setAttribute(Qt::WA_DeleteOnClose);
    orderlist->showMaximized();
}

void RestaurantSalesOrdersReport::on_routeComboBox_currentIndexChanged(int index)
{

}

void RestaurantSalesOrdersReport::on_switchTableButton_clicked()
{
    if(voucher->InventoryItems.size()>0 && voucher->reference.length()>0 ){
        previousTable = voucher->reference;
        LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper();
        AccountGroupMasterDatabaseHelper *accHelper = new AccountGroupMasterDatabaseHelper();

        TableSelect* tabSelect = new TableSelect(voucher,salesOrderHelper,ledHelper,accHelper, new PricelistDatabaseHelper(),this);
        tabSelect ->setWindowFlags(Qt::Window);
        tabSelect->setSwitchFlag(true);
        tabSelect->setLineFocus();
        tabSelect->showFullScreen();
        QObject::connect(tabSelect ,SIGNAL(switchTable()),
                         this,SLOT(switchOption()));
    }
}

void RestaurantSalesOrdersReport::switchOption()
{
    qDebug()<<Q_FUNC_INFO;
    if(previousTable!=voucher->reference){
        QPair<QString,QString> vNumber = salesOrderHelper->getVoucherNumberByRef(voucher->reference);
        if(vNumber.first.length()>0){
            GeneralVoucherDataObject *temp = salesOrderHelper->getVoucherByVoucherNoPtr(vNumber.first,vNumber.second);

            for(int i=0;i<voucher->InventoryItems.size();i++){
                temp->InventoryItems.append(voucher->InventoryItems[i]);
            }


            TransactionCalculator::VoucherCalculateSales(temp);
            salesOrderHelper->deleteVoucher(voucher);
            salesOrderHelper->updateVoucher(temp,vNumber.first);
            // qDebug()<<"Temp Total : "<<temp->grandTotal;
        }
        else{
            salesOrderHelper->updateReference(voucher->voucherNumber,voucher->reference);
        }
        setTable();
    }
}

void RestaurantSalesOrdersReport::disableEdit()
{
    ui->editButton->setDisabled(true);
    ui->editButton->setStyleSheet("background-color:#ffffff");
}

void RestaurantSalesOrdersReport::disableDelete()
{
    ui->cancelButton->setDisabled(true);
    ui->cancelButton->setStyleSheet("background-color:#ffffff");
}


void RestaurantSalesOrdersReport::showEvent(QShowEvent *event)
{
    setTable();
    QWidget::showEvent(event);
}
