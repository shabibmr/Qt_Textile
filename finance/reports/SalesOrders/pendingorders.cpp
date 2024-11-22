#include "pendingorders.h"
#include "ui_pendingorders.h"

#include "datamodels/Counter/countersettingsdatamodel.h"
#include <QLabel>
#include "pendingordersdelegate.h"


PendingOrders::PendingOrders(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PendingOrders)
{
    ui->setupUi(this);
    ui->fromDateEdit->setDate(QDate::currentDate());
    ui->toDateEdit->setDate(QDate::currentDate());


    setDBHelpers();
    setFilters();
    shareButton = new QtMaterialIconButton(QtMaterialTheme::icon("social", "share"));
    shareButton->setParent(this);
    //    shareButton->setColor(QColor(255,250,0));
    shareButton->setIconSize(QSize(30,30));
    QObject::connect(shareButton,&QPushButton::clicked,
                     this,&PendingOrders::showShareOptions2);

    ui->horizontalLayout->addWidget(shareButton);
    ui->printPushButton->hide();
    ui->previewTextEdit->hide();
    ui->printSOButton->hide();
    ui->editButton->hide();
    setTableView();
    setTableParams();
    tableView->setFocus();
}

PendingOrders::~PendingOrders()
{
    delete ui;
}

void PendingOrders::setTableParams()
{

    //    ui->tableView->setSortingEnabled(true);
    tableView->hideColumn(2);

    tableView->hideColumn(4);
//    tableView->hideColumn(5);
    tableView->hideColumn(6);
    tableView->hideColumn(11);
    tableView->hideColumn(12);

    tableView->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Stretch);
    tableView->horizontalHeader()->setSectionResizeMode(9, QHeaderView::Stretch);


    tableView->setColumnWidth(1,60);
    tableView->setColumnWidth(12,25);
    tableView->setColumnWidth(11,25);


    tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    tableView->horizontalHeader()->setSectionResizeMode(4,QHeaderView::ResizeToContents);
    tableView->horizontalHeader()->setSectionResizeMode(5,QHeaderView::ResizeToContents);
    tableView->horizontalHeader()->setSectionResizeMode(10,QHeaderView::ResizeToContents);


    ui->itemWiseTableView_2->setSortingEnabled(true);
    ui->itemWiseTableView_2->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
}

void PendingOrders::setDBHelpers()
{
    userHelper = new UserProfileDatabaseHelper();
    dbhelper = new SalesOrderDatabaseHelper();
    pHelper = new PricelistDatabaseHelper();
    addHelper = new AddressBookDatabaseHelper();
    salesHelper = new salesVoucherDatabaseHelper();
    godownHelper = new GodownDatabaseHelper();
    ledHelper = new LedgerMasterDatabaseHelper();
    accHelper = new AccountGroupMasterDatabaseHelper();
    empHelper = new UserProfileDatabaseHelper();

    printer = new CounterPrinterHelper();

    QStringList salesGroups;
    salesGroups.append(accHelper->getGroupIDByName("Cash-in-Hand"));
    salesGroups.append(accHelper->getGroupIDByName("Sundry Debtors"));
    salesGroups.append(accHelper->getGroupIDByName("Bank Accounts"));


    tableView = new QTableView(this);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    QVBoxLayout *tlay = new QVBoxLayout(this);

    ui->horizontalLayout_4->addLayout(tlay);
    tlay->addWidget(tableView);

    totalLine = new QLineEdit(this);
    totalLine->setAlignment(Qt::AlignRight);

    tlay->addWidget(totalLine,0,Qt::AlignRight);


    PendingOrdersDelegate *delegate = new PendingOrdersDelegate(ledHelper,ledHelper->getLedgerQueryString(salesGroups),empHelper);
    tableView->setItemDelegate(delegate);

    ui->horizontalLayout_4->setStretch(0,2);
    ui->horizontalLayout_4->setStretch(1,8);

    connect(delegate,&PendingOrdersDelegate::printKot,this,&PendingOrders::printKOT);
    connect(delegate,&PendingOrdersDelegate::saveSalesVoucher,this,&PendingOrders::saveSalesVoucher);
    connect(tableView,&QTableView::clicked,this,&PendingOrders::on_tableView_clicked);

}

void PendingOrders::setHeading(QString label)
{
    ui->titleLabel->setText(label);
}

void PendingOrders::setTableView()
{
    QDate fromDate = ui->fromDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

    //    model = dbhelper->getPendingSalesOrdersByDeliveryDate(fromDateTime,toDateTime);

    int empID = empCombo->currentData().toInt();
    QString time = timeCombo->currentData().toString();
    int priceID = serviceCombo->currentData().toInt();
    QString route = routeCombo->currentData().toString();
    int statusFilter =  statusCombo->currentData().toInt();

    model = dbhelper->getVouchersForBooking(fromDate,toDate,empID,priceID,route,"110,115","",time,QuotationStatus::SalesOrderDeliveryCreated);

    float total = 0;
    for(int i=0;i<model->rowCount();i++){
        total += model->record(i).value(10).toFloat();
    }

    totalLine->setText(QString::number(total,'f',2));
    model2 = new  DeliveryOrdersModel(dbhelper, model);

    tableView->setModel(model2);

    itemwiseModel = dbhelper->getItemwisePendingOrdersByDeliveryDate(fromDate,toDate, time);

    ui->itemWiseTableView_2->setModel(itemwiseModel);

    setTableParams();

}

void PendingOrders::showBill()
{
    CounterPrinterHelper pHelper;
    QString html = pHelper.getHtml(voucher);
}

void PendingOrders::on_okPushButton_clicked()
{
    setTableView();
}

void PendingOrders::delButtonClicked(bool)
{
    int x= sender()->property("row").toInt();
    qDebug()<<x;
}

void PendingOrders::on_tableView_doubleClicked(const QModelIndex &index)
{
    //    int row = sort->mapToSource(index).row();
    int row = index.row();
    QString voucherNo = model->record(row).value(1).toString();
    QString voucherPrefix = model->record(row).value(2).toString();
    //    qDebug()<<"at 0 "<<voucherNo;

    if(prevPrefix != voucherPrefix || prevVoucher != voucherNo){
        prevVoucher = voucherNo;
        prevPrefix = voucherPrefix;
        voucher = dbhelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);
    }
    AddSalesOrderWidget = new SalesOrderVoucherEditor();
    AddSalesOrderWidget->setWindowFlags(Qt::Window);
    AddSalesOrderWidget->setVoucher(voucher);
    AddSalesOrderWidget->showVoucher();
    AddSalesOrderWidget->showMaximized();
    QObject::connect(AddSalesOrderWidget,SIGNAL(closing()),this,SLOT(setTableView()));
}

void PendingOrders::setFilters()
{
    empCombo = new QComboBox(this);
    timeCombo = new QComboBox(this);
    serviceCombo = new QComboBox(this);
    routeCombo = new QComboBox(this);
    statusCombo = new QComboBox(this);




    empCombo->setMinimumHeight(25);
    timeCombo->setMinimumHeight(25);
    serviceCombo->setMinimumHeight(25);
    routeCombo->setMinimumHeight(25);
    statusCombo->setMinimumHeight(25);

    empCombo->setMinimumWidth(100);
    timeCombo->setMinimumWidth(100);
    serviceCombo->setMinimumWidth(100);
    routeCombo->setMinimumWidth(100);
    statusCombo->setMinimumWidth(100);

    QList<PriceListDataObject> prices = pHelper->getAllPriceLists();

    QMap<int,QString> employees = userHelper->getAllUsers();

    QMapIterator<int, QString> i(employees);
    empCombo->addItem("All",0);
    while (i.hasNext()) {
        i.next();
        empCombo->addItem(i.value(),i.key());
    }

    serviceCombo->addItem("ALL",0);
    for(int i=0;i<prices.length();i++){
        serviceCombo->addItem(prices[i].priceListName,prices[i].priceListID);
    }

    timeCombo->addItem("All","%");
    timeCombo->addItem("Lunch","13:00:00");
    timeCombo->addItem("Dinner","19:00:00");

    routeCombo->addItem("All","%");

    QStringList list = addHelper->getAllRoutesForSearch();
    for(int i =0; i < list.size();i++){
        routeCombo->addItem(list[i],list[i]);
    }
    //    routeCombo->addItems(addHelper->getAllRoutesForSearch());

    statusCombo->addItem("All",0);
    statusCombo->addItem("Current",QuotationStatus::SalesOrderCreated);
    statusCombo->addItem("Pre Order",QuotationStatus::SalesOrderDeliveryCreated);
    statusCombo->addItem("Billed",QuotationStatus::SalesInvoiceRaised);

    ui->filterLay->addWidget(new QLabel("Employee"));
    ui->filterLay->addWidget(empCombo);

    if(true){

//    ui->filterLay->addWidget(new QLabel("Time"));
//    ui->filterLay->addWidget(timeCombo);
//    ui->filterLay->addWidget(new QLabel("Route"));
//    ui->filterLay->addWidget(routeCombo);
//    ui->filterLay->addWidget(new QLabel("Service"));
//    ui->filterLay->addWidget(serviceCombo);


//    ui->filterLay->addWidget(new QLabel("Order Type"));
//    ui->filterLay->addWidget(statusCombo);

    timeCombo->hide();
    routeCombo->hide();
    statusCombo->hide();
    serviceCombo->hide();
    }
    ui->filterLay->addStretch();

}

void PendingOrders::askForPrint()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Print Voucher","Would You Like to Print?",
                                  QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::Yes){
        printReport(new QPrinter);
    }
}

void PendingOrders::printReport(QPrinter *printer)
{
    printer->setPrinterName("printerName");
    printerHelper = new CounterPrinterHelper;
    printerHelper->setPrinter(printer);

    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateEdit->date();
    QDate toDate = ui->toDateEdit->date();


    widthList <<60<<20;
    AlignmentList<< 0<< 1 ;
    QList<int> avlCols; avlCols<<0<<1;
    //    CounterPrinterHelper *printer = new CounterPrinterHelper;

    printerHelper-> printReportGeneral(ui->itemWiseTableView_2
                                       ,widthList,"Pending Orders Itemwise",fromDate,toDate,AlignmentList
                                       ,extraTop,extraBottom,avlCols);
}

void PendingOrders::on_excelButton_clicked()
{

    QDate fDate = ui->fromDateEdit->date();
    QDate tDate = ui->toDateEdit->date();
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

    QList<int> cols; cols<<20<<20<<20<<20<<20<<20<<20<<20;
    //    reportName = "Itemwise Sales Report";
    QList<int> avlCols;// avlCols <<1<<3<<4<<8<<11<<7<<9<<10;
    avlCols<<1<<11<<3<<8<<12<<7<<4<<10;
    ex->GenericReportFromModel(fileName,reportName,cols,model,fromDate,toDate,avlCols);
}

void PendingOrders::printReportSlot(QPrinter *printer)
{
    int val = sender()->property("PrintType").toInt();

    CounterPrinterHelper printerHelper;
    printerHelper.setPrinter(printer);
    QDate fDate = ui->fromDateEdit->date();
    QDate tDate = ui->toDateEdit->date();

    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    //    QString total = ui->totalLineEdit->text();
    //    QDate fromDate = ui->toDateDateTimeEdit->date();
    //    QDate toDate = ui->toDateDateTimeEdit->date();
    //    extraBottom<< "Total: " <<total;
    QList<int> avlCols;
    if(val == PrintTypes::Thermal){
        avlCols <<1<<7<<8<<10;
        widthList <<20<<20<<20<<20;
        AlignmentList<< 0<<0<< 0 <<1;

    }
    else{
        avlCols <<1<<3<<4<<5<<6<<7<<9<<10;
        widthList <<20<<20<<20<<20<<20<<20<<20<<20;
        AlignmentList<<0 << 0<<0<< 0 <<1;
    }

    printerHelper.printReportGeneralwithModel(model, widthList,"Sales Order List",fDate,tDate,AlignmentList
                                              ,extraTop,extraBottom,avlCols);
}


void PendingOrders::showShareOptions()
{
    shareOptionsWidget = new ShareTools("Pending Orders Report", shareButton);
    shareOptionsWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup|Qt::WindowStaysOnTopHint);;
    shareOptionsWidget->excelButton->hide();
    shareOptionsWidget->show();

    QObject::connect(shareOptionsWidget,&ShareTools::printClicked,
                     this,&PendingOrders::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::pdfClicked,
                     this,&PendingOrders::printReport);
    //    QObject::connect(shareOptionsWidget,&ShareTools::excelClicked,
    //                     this,&PendingOrders::on_excelButton_clicked);
}

void PendingOrders::on_printPushButton_clicked()
{
    askForPrint();
}

void PendingOrders::printKOT(QString voucherNo, QString voucherPrefix)
{
    qDebug()<<"Print KOT";

    GeneralVoucherDataObject* voucher = dbhelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);
    TransactionCalculator::VoucherCalculateSales(voucher);

    for(int i=0;i<voucher->InventoryItems.size();i++){
        voucher->InventoryItems[i].BaseItem.listId = 0;
    }

    printer->printKOT(voucher);
    dbhelper->updateKOTPrintStatus(voucherNo, voucherPrefix, true);
    setTableView();

}

void PendingOrders::saveSalesVoucher(QString voucherNo, QString voucherPrefix)
{
    qDebug()<<"Save Sales Voucher";
    GeneralVoucherDataObject* voucherTemp = dbhelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);
    QString vno = voucherTemp->voucherNumber;
    QString vpref = voucherTemp->VoucherPrefix;

    SalesVoucherEditor *editor = new SalesVoucherEditor(salesHelper);
    editor->setAttribute(Qt::WA_DeleteOnClose);

    if(voucherTemp->status == QuotationStatus::SalesInvoiceRaised){
        QMessageBox box; box.setText("Voucher already created"); box.exec();
        QPair<QString,QString> pair =  salesHelper->getVoucherNoByTransactionId(voucher->TransactionId);
        qDebug()<<"Opening Sales Voucher "<<pair.first<<pair.second;
        voucherTemp = salesHelper->getVoucherByVoucherNoPtr(pair.first,pair.second);
        editor->setVoucher(voucherTemp);
        editor->showVoucher();
        editor->showMaximized();

        return;
    }
    TransactionCalculator::VoucherCalculateSales(voucherTemp);

    connect(editor,&SalesVoucherEditor::saveCompleted,this,[=](){
        dbhelper->setSalesOrderStatus(QuotationStatus::SalesInvoiceRaised,vno,vpref);
        model->query().exec();
        QMessageBox box; box.setText("Voucher Saved as "+voucherTemp->voucherNumber); box.exec();
    });

    voucherTemp->VoucherDate = QDate::currentDate();
    voucherTemp->voucherType = GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesVoucher);
    voucherTemp->voucherNumber = salesHelper->getNextVoucherNo(voucherTemp->VoucherPrefix);
    voucherTemp->status = QuotationStatus::Started;
    voucherTemp->fromGodownID = godownHelper->getDefaultGodown();
    editor->importVoucher(voucherTemp);
    editor->saveVoucher();


}

void PendingOrders::printSalesOrder()
{
    if(voucher != nullptr)
        printer->printSalesOrder(voucher);
}

void PendingOrders::on_tableView_clicked(const QModelIndex &index)
{
    int row = index.row();
    QString voucherNo = model->record(row).value(1).toString();
    QString voucherPrefix = model->record(row).value(2).toString();

    //    qDebug()<<"at 0 "<<voucherNo;
    //    if(voucher!=nullptr)
    //        delete voucher;

    if(prevPrefix != voucherPrefix || prevVoucher != voucherNo){
        prevVoucher = voucherNo;
        prevPrefix = voucherPrefix;
        voucher = dbhelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);
        TransactionCalculator::VoucherCalculateSales(voucher);
    }
    QString html = printer->getHtml(voucher);
    ui->previewTextEdit->setText(html);

    ui->printSOButton->show();
    ui->previewTextEdit->show();
    ui->editButton->show();

}

void PendingOrders::on_printSOButton_clicked()
{
    printSalesOrder();
}

void PendingOrders::on_printDetailed_clicked()
{
    showShareOptions();
}

void PendingOrders::showShareOptions2()
{
    shareOptionsWidget = new ShareTools("Itemwise Sales Report",this);
    shareOptionsWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup|Qt::WindowStaysOnTopHint);;
    //    shareOptionsWidget->setAttribute(Qt::WA_DeleteOnClose);
    shareOptionsWidget->show();

    QObject::connect(shareOptionsWidget,&ShareTools::printClicked,
                     this,&PendingOrders::printReportSlot);
    QObject::connect(shareOptionsWidget,&ShareTools::pdfClicked,
                     this,&PendingOrders::printReport);

    QObject::connect(shareOptionsWidget,&ShareTools::excelClicked,
                     this,&PendingOrders::on_excelButton_clicked);

}

void PendingOrders::on_editButton_clicked()
{
    emit selectedVoucher(voucher);
    this->close();
}
