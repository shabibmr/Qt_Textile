#include "gstreport.h"
#include "ui_gstreport.h"
#include <sharetools/excel/excelreporthelper.h>

GSTReport::GSTReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GSTReport)
{
    ui->setupUi(this);

    dbHelper = new VatReportDatabaseHelper;
    ledHelper = new LedgerHelper;

    ui->fromDateEdit->setDate(QDate(QDate::currentDate().year(),QDate::currentDate().month(),1));
    ui->toDateEdit->setDate(QDate::currentDate());
    ui->tabWidget->setCurrentIndex(0);
//    ui->tabWidget->removeTab(4);
//    ui->tabWidget->removeTab(4);
//    ui->tabWidget->removeTab(2);
    ui->allDebitNoteTableWidget->hide();
    ui->allCreditNoteTableWidget->hide();
    ui->label_5->hide();
    ui->label_4->hide();

    salesTableView = new ReportsTableView();
    salesTableView->setItemDelegate(new ReportCommonDelegate());
    ui->salesLayout->addWidget(salesTableView);

    b2csalesTableView = new ReportsTableView();
    b2csalesTableView->setItemDelegate(new ReportCommonDelegate());
    ui->b2CSalesLayout->addWidget(b2csalesTableView);

    purchaseTableView = new ReportsTableView();
    purchaseTableView->setItemDelegate(new ReportCommonDelegate());
    ui->purchaseLayout->addWidget(purchaseTableView);

    debitNoteTableView = new ReportsTableView();
    debitNoteTableView->setItemDelegate(new ReportCommonDelegate());
    ui->debitNoteLayout->addWidget(debitNoteTableView);

    creditNoteTableView = new ReportsTableView();
    creditNoteTableView->setItemDelegate(new ReportCommonDelegate());
    ui->creditNoteLayout->addWidget(creditNoteTableView);
    //    ui->excelButton->hide();
    hsnTableView = new ReportsTableView("");
    hsnTableView->setItemDelegate(new HsnReportDelegate());
    ui->hsnLayout->addWidget(hsnTableView);

    cessTableView = new ReportsTableView();
    cessTableView->setItemDelegate(new ReportCommonDelegate());
    ui->cessLayout->addWidget(cessTableView);


    ui->allDebitNoteTableWidget->setStyleSheet(

                   "QTableView::item{"
                   "gridline-color : #ffffff;"
                   "border : 0px solid #fffff8;"
                   "}"
                   "QTableView::item:selected{"
                   "color:#00acc1;"
                   "background-color:#e0f7fa;"
                   "}"
                   "QTableView#EditInvTable{"
                   "background-color: #ffffff;"
                   "color: #006064;"
                   "border : none;"
                   "/*selection-background-color: #999*/}"
                   "QHeaderView::section:horizontal"
                   "{"
                   "background-color: #00bcd4;"
                   "color: white;"
                   "border: 1px solid #ffffff;"
                   "}"
                   );



    ui->allCreditNoteTableWidget->setStyleSheet(

                   "QTableView::item{"
                   "gridline-color : #ffffff;"
                   "border : 0px solid #fffff8;"
                   "}"
                   "QTableView::item:selected{"
                   "color:#00acc1;"
                   "background-color:#e0f7fa;"
                   "}"
                   "QTableView#EditInvTable{"
                   "background-color: #ffffff;"
                   "color: #006064;"
                   "border : none;"
                   "/*selection-background-color: #999*/}"
                   "QHeaderView::section:horizontal"
                   "{"
                   "background-color: #00bcd4;"
                   "color: white;"
                   "border: 1px solid #ffffff;"
                   "}"
                   );


    ui->allPurchaseTableWidget->setStyleSheet(

                   "QTableView::item{"
                   "gridline-color : #ffffff;"
                   "border : 0px solid #fffff8;"
                   "}"
                   "QTableView::item:selected{"
                   "color:#00acc1;"
                   "background-color:#e0f7fa;"
                   "}"
                   "QTableView#EditInvTable{"
                   "background-color: #ffffff;"
                   "color: #006064;"
                   "border : none;"
                   "/*selection-background-color: #999*/}"
                   "QHeaderView::section:horizontal"
                   "{"
                   "background-color: #00bcd4;"
                   "color: white;"
                   "border: 1px solid #ffffff;"
                   "}"
                   );



    ui->allSalesTableWidget->setStyleSheet(

                   "QTableView::item{"
                   "gridline-color : #ffffff;"
                   "border : 0px solid #fffff8;"
                   "}"
                   "QTableView::item:selected{"
                   "color:#00acc1;"
                   "background-color:#e0f7fa;"
                   "}"
                   "QTableView#EditInvTable{"
                   "background-color: #ffffff;"
                   "color: #006064;"
                   "border : none;"
                   "/*selection-background-color: #999*/}"
                   "QHeaderView::section:horizontal"
                   "{"
                   "background-color: #00bcd4;"
                   "color: white;"
                   "border: 1px solid #ffffff;"
                   "}"
                   );

    setTable();
}

GSTReport::~GSTReport()
{
    delete ui;
}

void GSTReport::setTable()
{
    switch (ui->tabWidget->currentIndex()) {
    case 0:
    {
        if(!loadedAllGst){
            setTableAll();
            loadedAllGst = true;
        }
        break;
    }
    case 1:
    {
        if(!loadedSales){
            setTableSales();
            loadedSales = true;
        }
        break;
    }
    case 2:
    {
        if(!loadedb2cSales){
            setTableb2cSales();
            loadedb2cSales = true;
        }
        break;
    }
    case 3:
    {
        if(!loadedPurchase){
            setTablePurchase();
            loadedPurchase = true;
        }
        break;
    }
    case 4:
    {
        if(!loadedDebitNotes){
            setTableDebitNote();
            loadedDebitNotes = true;
        }
        break;
    }
    case 5:
    {
        if(!loadedCreditNotes){
            setTableCreditNote();
            loadedCreditNotes = true;
        }
        break;
    }
    case 6:
    {
        if(!loadedHsn){
            setHsnTable();
            loadedHsn = true;
        }
        break;
    }
    case 7:
    {
        if(!loadedCess){
            setCessReport();
            loadedCess = true;
        }
        break;
    }

    }
}

void GSTReport::on_backButton_clicked()
{
    this->close();
}

void GSTReport::setTableAll()
{
    qDebug()<<"tab clicked"<<ui->tabWidget->currentIndex();
    ui->allSalesTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->allSalesTableWidget->setColumnWidth(1,200);
    ui->allSalesTableWidget->setColumnWidth(2,200);
    ui->allSalesTableWidget->setColumnWidth(3,200);
    ui->allSalesTableWidget->setColumnWidth(4,200);

    ui->allPurchaseTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->allPurchaseTableWidget->setColumnWidth(1,200);
    ui->allPurchaseTableWidget->setColumnWidth(2,200);
    ui->allPurchaseTableWidget->setColumnWidth(3,200);
    ui->allPurchaseTableWidget->setColumnWidth(4,200);

    ui->allCreditNoteTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->allCreditNoteTableWidget->setColumnWidth(1,200);
    ui->allCreditNoteTableWidget->setColumnWidth(2,200);

    ui->allDebitNoteTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->allDebitNoteTableWidget->setColumnWidth(1,200);
    ui->allDebitNoteTableWidget->setColumnWidth(2,200);

    ShiftDataModel *shift = new ShiftDataModel;
    ShiftDatabaseHelper shiftHelper;

    QDate fromDate = ui->fromDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

    shift->ShiftStartDateTime = fromDateTime;
    shift->ShiftEndDateTime = toDateTime;

//    QString billsFrom = shiftHelper.getFirstBillByDate(shift);
//    QString billsTo = shiftHelper.getLastBillByDate(shift);

//    qDebug()<<"sales bill range "<<fromDate<<toDate;


    float Sales5 = 0;
    float Sales5SGST = 0;
    float Sales5CGST = 0;
    float Sales5IGST = 0;
    float Sales5Total = 0;

    float Sales12 = 0;
    float Sales12SGST = 0;
    float Sales12CGST = 0;
    float Sales12IGST = 0;
    float Sales12Total = 0;

    float Sales18 = 0;
    float Sales18SGST = 0;
    float Sales18CGST = 0;
    float Sales18IGST = 0;
    float Sales18Total= 0;

    float Sales28 = 0;
    float Sales28SGST = 0;
    float Sales28CGST = 0;
    float Sales28IGST = 0;
    float Sales28Total= 0;


    float TotalSales = 0;
    float TotalTaxable = 0;
    float TotalSGST = 0;
    float TotalCGST = 0;
    float TotalIGST = 0;

    float Sales0 = dbHelper->getAmountByLedger("0x7x3", fromDate, toDate, GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesVoucher));

    Sales5 = dbHelper->getAmountByLedger("0x7xSV5", fromDate, toDate, GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesVoucher));
    Sales5CGST = dbHelper->getAmountByLedger("0x2x14xOCG25", fromDate, toDate, GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesVoucher));
    Sales5SGST = dbHelper->getAmountByLedger("0x2x14xOSG25", fromDate, toDate, GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesVoucher));
    Sales5Total = Sales5 + Sales5CGST + Sales5SGST + Sales5IGST;

    Sales12 = dbHelper->getAmountByLedger("0x7xSV12", fromDate, toDate, GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesVoucher));
    Sales12CGST = dbHelper->getAmountByLedger("0x2x14xOCG6", fromDate, toDate, GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesVoucher));
    Sales12SGST = dbHelper->getAmountByLedger("0x2x14xOSG6", fromDate, toDate, GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesVoucher));
    Sales12Total = Sales12 + Sales12CGST + Sales12SGST + Sales12IGST;

    Sales18 = dbHelper->getAmountByLedger("0x7xSV18", fromDate, toDate, GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesVoucher));
    Sales18CGST = dbHelper->getAmountByLedger("0x2x14xOCG9", fromDate, toDate, GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesVoucher));
    Sales18SGST = dbHelper->getAmountByLedger("0x2x14xOSG9", fromDate, toDate, GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesVoucher));
    Sales18Total = Sales18 + Sales18CGST + Sales18SGST + Sales18IGST;

    Sales28 = dbHelper->getAmountByLedger("0x7xSV28", fromDate, toDate, GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesVoucher));
    Sales28CGST = dbHelper->getAmountByLedger("0x2x14xOCG14", fromDate, toDate, GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesVoucher));
    Sales28SGST = dbHelper->getAmountByLedger("0x2x14xOSG14", fromDate, toDate, GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesVoucher));
    Sales28Total = Sales28 + Sales28CGST + Sales28SGST + Sales28IGST;

    TotalSales = Sales5Total + Sales12Total + Sales18Total + Sales28Total + Sales0;
    TotalTaxable = Sales5 + Sales12 + Sales18 + Sales28 + Sales0;
    TotalSGST = Sales5SGST + Sales12SGST + Sales18SGST + Sales28SGST;
    TotalCGST = Sales5CGST + Sales12CGST + Sales18CGST + Sales28CGST;
    TotalIGST = Sales5IGST + Sales12IGST + Sales18IGST + Sales28IGST;


    float Purchase5 = 0;
    float Purchase5SGST = 0;
    float Purchase5CGST = 0;
    float Purchase5IGST = 0;
    float Purchase5Total = 0;

    float Purchase12 = 0;
    float Purchase12SGST = 0;
    float Purchase12CGST = 0;
    float Purchase12IGST = 0;
    float Purchase12Total = 0;

    float Purchase18 = 0;
    float Purchase18SGST = 0;
    float Purchase18CGST = 0;
    float Purchase18IGST = 0;
    float Purchase18Total = 0;

    float Purchase28 = 0;
    float Purchase28SGST = 0;
    float Purchase28CGST = 0;
    float Purchase28IGST = 0;
    float Purchase28Total = 0;

    float TotalPurchase = 0;
    float TotalPurchaseTaxable = 0;
    float TotalPurchaseSGST = 0;
    float TotalPurchaseCGST = 0;
    float TotalPurchaseIGST = 0;

//    billsFrom = shiftHelper.getFirstPurchaseBillByDate(shift);
//    billsTo = shiftHelper.getLastPurchaseBillByDate(shift);
    float Purchase0 = dbHelper->getAmountByLedger("0x8x4", fromDate, toDate, GMVoucherTypes::getVoucherString(GMVoucherTypes::PurchaseVoucher));

    Purchase5 = dbHelper->getAmountByLedger("0x8xPV5", fromDate, toDate, GMVoucherTypes::getVoucherString(GMVoucherTypes::PurchaseVoucher));
    Purchase5CGST = dbHelper->getAmountByLedger("0x2x14xICG25", fromDate, toDate, GMVoucherTypes::getVoucherString(GMVoucherTypes::PurchaseVoucher));
    Purchase5SGST = dbHelper->getAmountByLedger("0x2x14xISG25", fromDate, toDate, GMVoucherTypes::getVoucherString(GMVoucherTypes::PurchaseVoucher));
    Purchase5Total = Purchase5 + Purchase5CGST + Purchase5SGST + Purchase5IGST;

    Purchase12 = dbHelper->getAmountByLedger("0x8xPV12", fromDate, toDate, GMVoucherTypes::getVoucherString(GMVoucherTypes::PurchaseVoucher));
    Purchase12CGST = dbHelper->getAmountByLedger("0x2x14xICG6", fromDate, toDate, GMVoucherTypes::getVoucherString(GMVoucherTypes::PurchaseVoucher));
    Purchase12SGST = dbHelper->getAmountByLedger("0x2x14xISG6", fromDate, toDate, GMVoucherTypes::getVoucherString(GMVoucherTypes::PurchaseVoucher));
    Purchase12Total = Purchase12 + Purchase12CGST + Purchase12SGST + Purchase12IGST;

    Purchase18 = dbHelper->getAmountByLedger("0x8xPV18", fromDate, toDate, GMVoucherTypes::getVoucherString(GMVoucherTypes::PurchaseVoucher));
    Purchase18CGST = dbHelper->getAmountByLedger("0x2x14xICG9", fromDate, toDate, GMVoucherTypes::getVoucherString(GMVoucherTypes::PurchaseVoucher));
    Purchase18SGST = dbHelper->getAmountByLedger("0x2x14xISG9", fromDate, toDate, GMVoucherTypes::getVoucherString(GMVoucherTypes::PurchaseVoucher));
    Purchase18Total = Purchase18 + Purchase18CGST + Purchase18SGST + Purchase18IGST;

    Purchase28 = dbHelper->getAmountByLedger("0x8xPV28", fromDate, toDate, GMVoucherTypes::getVoucherString(GMVoucherTypes::PurchaseVoucher));
    Purchase28CGST = dbHelper->getAmountByLedger("0x2x14xICG14", fromDate, toDate, GMVoucherTypes::getVoucherString(GMVoucherTypes::PurchaseVoucher));
    Purchase28SGST = dbHelper->getAmountByLedger("0x2x14xISG14", fromDate, toDate, GMVoucherTypes::getVoucherString(GMVoucherTypes::PurchaseVoucher));
    Purchase28Total = Purchase28 + Purchase28CGST + Purchase28SGST + Purchase28IGST;

    TotalPurchase = Purchase5Total + Purchase12Total + Purchase18Total + Purchase28Total + Purchase0;
    TotalPurchaseTaxable = Purchase5 + Purchase12 + Purchase18 + Purchase28 + Purchase0;
    TotalPurchaseSGST = Purchase5SGST + Purchase12SGST + Purchase18SGST + Purchase28SGST;
    TotalPurchaseCGST = Purchase5CGST + Purchase12CGST + Purchase18CGST + Purchase28CGST;
    TotalPurchaseIGST = Purchase5IGST + Purchase12IGST + Purchase18IGST + Purchase28IGST;

    QTableWidgetItem *ItemSales5 = new QTableWidgetItem;
    QTableWidgetItem *ItemSales5SGST= new QTableWidgetItem;
    QTableWidgetItem *ItemSales5CGST= new QTableWidgetItem;
    QTableWidgetItem *ItemSales5IGST= new QTableWidgetItem;
    QTableWidgetItem *ItemSales5Total= new QTableWidgetItem;

    QTableWidgetItem *ItemSales12= new QTableWidgetItem;
    QTableWidgetItem *ItemSales12SGST= new QTableWidgetItem;
    QTableWidgetItem *ItemSales12CGST= new QTableWidgetItem;
    QTableWidgetItem *ItemSales12IGST= new QTableWidgetItem;
    QTableWidgetItem *ItemSales12Total= new QTableWidgetItem;

    QTableWidgetItem *ItemSales18= new QTableWidgetItem;
    QTableWidgetItem *ItemSales18SGST= new QTableWidgetItem;
    QTableWidgetItem *ItemSales18CGST= new QTableWidgetItem;
    QTableWidgetItem *ItemSales18IGST= new QTableWidgetItem;
    QTableWidgetItem *ItemSales18Total= new QTableWidgetItem;

    QTableWidgetItem *ItemSales28= new QTableWidgetItem;
    QTableWidgetItem *ItemSales28SGST= new QTableWidgetItem;
    QTableWidgetItem *ItemSales28CGST= new QTableWidgetItem;
    QTableWidgetItem *ItemSales28IGST= new QTableWidgetItem;
    QTableWidgetItem *ItemSales28Total= new QTableWidgetItem;

    QTableWidgetItem *ItemSales0= new QTableWidgetItem;
    QTableWidgetItem *ItemSales0SGST= new QTableWidgetItem;
    QTableWidgetItem *ItemSales0CGST= new QTableWidgetItem;
    QTableWidgetItem *ItemSales0IGST= new QTableWidgetItem;
    QTableWidgetItem *ItemSales0Total= new QTableWidgetItem;

    QTableWidgetItem *ItemPurchase0= new QTableWidgetItem;
    QTableWidgetItem *ItemPurchase0SGST= new QTableWidgetItem;
    QTableWidgetItem *ItemPurchase0CGST= new QTableWidgetItem;
    QTableWidgetItem *ItemPurchase0IGST= new QTableWidgetItem;
    QTableWidgetItem *ItemPurchase0Total= new QTableWidgetItem;

    QTableWidgetItem *ItemPurchase5= new QTableWidgetItem;
    QTableWidgetItem *ItemPurchase5SGST= new QTableWidgetItem;
    QTableWidgetItem *ItemPurchase5CGST= new QTableWidgetItem;
    QTableWidgetItem *ItemPurchase5IGST= new QTableWidgetItem;
    QTableWidgetItem *ItemPurchase5Total= new QTableWidgetItem;

    QTableWidgetItem *ItemPurchase12= new QTableWidgetItem;
    QTableWidgetItem *ItemPurchase12SGST= new QTableWidgetItem;
    QTableWidgetItem *ItemPurchase12CGST= new QTableWidgetItem;
    QTableWidgetItem *ItemPurchase12IGST= new QTableWidgetItem;
    QTableWidgetItem *ItemPurchase12Total= new QTableWidgetItem;

    QTableWidgetItem *ItemPurchase18= new QTableWidgetItem;
    QTableWidgetItem *ItemPurchase18SGST= new QTableWidgetItem;
    QTableWidgetItem *ItemPurchase18CGST= new QTableWidgetItem;
    QTableWidgetItem *ItemPurchase18IGST= new QTableWidgetItem;
    QTableWidgetItem *ItemPurchase18Total= new QTableWidgetItem;

    QTableWidgetItem *ItemPurchase28= new QTableWidgetItem;
    QTableWidgetItem *ItemPurchase28SGST= new QTableWidgetItem;
    QTableWidgetItem *ItemPurchase28CGST= new QTableWidgetItem;
    QTableWidgetItem *ItemPurchase28IGST= new QTableWidgetItem;
    QTableWidgetItem *ItemPurchase28Total= new QTableWidgetItem;

    ItemSales5->setText(QString::number(Sales5,'f',2));
    ItemSales5SGST->setText(QString::number(Sales5SGST,'f',2));
    ItemSales5CGST->setText(QString::number(Sales5CGST,'f',2));
    ItemSales5IGST->setText(QString::number(Sales5IGST,'f',2));
    ItemSales5Total->setText(QString::number(Sales5Total,'f',2));

    ItemSales12->setText(QString::number(Sales12,'f',2));
    ItemSales12SGST->setText(QString::number(Sales12SGST,'f',2));
    ItemSales12CGST->setText(QString::number(Sales12CGST,'f',2));
    ItemSales12IGST->setText(QString::number(Sales12IGST,'f',2));
    ItemSales12Total->setText(QString::number(Sales12Total,'f',2));

    ItemSales18->setText(QString::number(Sales18,'f',2));
    ItemSales18SGST->setText(QString::number(Sales18SGST,'f',2));
    ItemSales18CGST->setText(QString::number(Sales18CGST,'f',2));
    ItemSales18IGST->setText(QString::number(Sales18IGST,'f',2));
    ItemSales18Total->setText(QString::number(Sales18Total,'f',2));

    ItemSales28->setText(QString::number(Sales28,'f',2));
    ItemSales28SGST->setText(QString::number(Sales28SGST,'f',2));
    ItemSales28CGST->setText(QString::number(Sales28CGST,'f',2));
    ItemSales28IGST->setText(QString::number(Sales28IGST,'f',2));
    ItemSales28Total->setText(QString::number(Sales28Total,'f',2));

    ItemSales0->setText(QString::number(Sales0,'f',2));
    ItemSales0SGST->setText(QString::number(0,'f',2));
    ItemSales0CGST->setText(QString::number(0,'f',2));
    ItemSales0IGST->setText(QString::number(0,'f',2));
    ItemSales0Total->setText(QString::number(Sales0,'f',2));

    ItemPurchase0->setText(QString::number(Purchase0,'f',2));
    ItemPurchase0SGST->setText(QString::number(0,'f',2));
    ItemPurchase0CGST->setText(QString::number(0,'f',2));
    ItemPurchase0IGST->setText(QString::number(0,'f',2));
    ItemPurchase0Total->setText(QString::number(Purchase0,'f',2));

    ItemPurchase5->setText(QString::number(Purchase5,'f',2));
    ItemPurchase5SGST->setText(QString::number(Purchase5SGST,'f',2));
    ItemPurchase5CGST->setText(QString::number(Purchase5CGST,'f',2));
    ItemPurchase5IGST->setText(QString::number(Purchase5IGST,'f',2));
    ItemPurchase5Total->setText(QString::number(Purchase5Total,'f',2));

    ItemPurchase12->setText(QString::number(Purchase12,'f',2));
    ItemPurchase12SGST->setText(QString::number(Purchase12SGST,'f',2));
    ItemPurchase12CGST->setText(QString::number(Purchase12CGST,'f',2));
    ItemPurchase12IGST->setText(QString::number(Purchase12IGST,'f',2));
    ItemPurchase12Total->setText(QString::number(Purchase12Total,'f',2));

    ItemPurchase18->setText(QString::number(Purchase18,'f',2));
    ItemPurchase18SGST->setText(QString::number(Purchase18SGST,'f',2));
    ItemPurchase18CGST->setText(QString::number(Purchase18CGST,'f',2));
    ItemPurchase18IGST->setText(QString::number(Purchase18IGST,'f',2));
    ItemPurchase18Total->setText(QString::number(Purchase18Total,'f',2));

    ItemPurchase28->setText(QString::number(Purchase28,'f',2));
    ItemPurchase28SGST->setText(QString::number(Purchase28SGST,'f',2));
    ItemPurchase28CGST->setText(QString::number(Purchase28CGST,'f',2));
    ItemPurchase28IGST->setText(QString::number(Purchase28IGST,'f',2));
    ItemPurchase28Total->setText(QString::number(Purchase28Total,'f',2));

    ItemSales5->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemSales5SGST->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemSales5CGST->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemSales5IGST->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemSales5Total->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);

    ItemSales12->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemSales12SGST->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemSales12CGST->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemSales12IGST->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemSales12Total->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);

    ItemSales18->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemSales18SGST->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemSales18CGST->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemSales18IGST->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemSales18Total->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);

    ItemSales28->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemSales28SGST->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemSales28CGST->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemSales28IGST->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemSales28Total->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);

    ItemSales0->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemSales0SGST->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemSales0CGST->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemSales0IGST->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemSales0Total->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);

    ItemPurchase0->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemPurchase0SGST->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemPurchase0CGST->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemPurchase0IGST->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemPurchase0Total->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);

    ItemPurchase5->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemPurchase5SGST->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemPurchase5CGST->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemPurchase5IGST->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemPurchase5Total->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);

    ItemPurchase12->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemPurchase12SGST->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemPurchase12CGST->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemPurchase12IGST->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemPurchase12Total->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);

    ItemPurchase18->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemPurchase18SGST->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemPurchase18CGST->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemPurchase18IGST->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemPurchase18Total->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);

    ItemPurchase28->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemPurchase28SGST->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemPurchase28CGST->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemPurchase28IGST->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemPurchase28Total->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);



    ui->allSalesTableWidget->setItem(0,0,ItemSales5);
    ui->allSalesTableWidget->setItem(0,1,ItemSales5SGST);
    ui->allSalesTableWidget->setItem(0,2,ItemSales5CGST);
    ui->allSalesTableWidget->setItem(0,3,ItemSales5IGST);
    ui->allSalesTableWidget->setItem(0,4,ItemSales5Total);

    ui->allSalesTableWidget->setItem(1,0,ItemSales12);
    ui->allSalesTableWidget->setItem(1,1,ItemSales12SGST);
    ui->allSalesTableWidget->setItem(1,2,ItemSales12CGST);
    ui->allSalesTableWidget->setItem(1,3,ItemSales12IGST);
    ui->allSalesTableWidget->setItem(1,4,ItemSales12Total);

    ui->allSalesTableWidget->setItem(2,0,ItemSales18);
    ui->allSalesTableWidget->setItem(2,1,ItemSales18SGST);
    ui->allSalesTableWidget->setItem(2,2,ItemSales18CGST);
    ui->allSalesTableWidget->setItem(2,3,ItemSales18IGST);
    ui->allSalesTableWidget->setItem(2,4,ItemSales18Total);

    ui->allSalesTableWidget->setItem(3,0,ItemSales28);
    ui->allSalesTableWidget->setItem(3,1,ItemSales28SGST);
    ui->allSalesTableWidget->setItem(3,2,ItemSales28CGST);
    ui->allSalesTableWidget->setItem(3,3,ItemSales28IGST);
    ui->allSalesTableWidget->setItem(3,4,ItemSales28Total);


    ui->allSalesTableWidget->setItem(4,0,ItemSales0);
    ui->allSalesTableWidget->setItem(4,1,ItemSales0SGST);
    ui->allSalesTableWidget->setItem(4,2,ItemSales0CGST);
    ui->allSalesTableWidget->setItem(4,3,ItemSales0IGST);
    ui->allSalesTableWidget->setItem(4,4,ItemSales0Total);

    ui->allPurchaseTableWidget->setItem(0,0,ItemPurchase5);
    ui->allPurchaseTableWidget->setItem(0,1,ItemPurchase5SGST);
    ui->allPurchaseTableWidget->setItem(0,2,ItemPurchase5CGST);
    ui->allPurchaseTableWidget->setItem(0,3,ItemPurchase5IGST);
    ui->allPurchaseTableWidget->setItem(0,4,ItemPurchase5Total);

    ui->allPurchaseTableWidget->setItem(1,0,ItemPurchase12);
    ui->allPurchaseTableWidget->setItem(1,1,ItemPurchase12SGST);
    ui->allPurchaseTableWidget->setItem(1,2,ItemPurchase12CGST);
    ui->allPurchaseTableWidget->setItem(1,3,ItemPurchase12IGST);
    ui->allPurchaseTableWidget->setItem(1,4,ItemPurchase12Total);

    ui->allPurchaseTableWidget->setItem(2,0,ItemPurchase18);
    ui->allPurchaseTableWidget->setItem(2,1,ItemPurchase18SGST);
    ui->allPurchaseTableWidget->setItem(2,2,ItemPurchase18CGST);
    ui->allPurchaseTableWidget->setItem(2,3,ItemPurchase18IGST);
    ui->allPurchaseTableWidget->setItem(2,4,ItemPurchase18Total);

    ui->allPurchaseTableWidget->setItem(3,0,ItemPurchase28);
    ui->allPurchaseTableWidget->setItem(3,1,ItemPurchase28SGST);
    ui->allPurchaseTableWidget->setItem(3,2,ItemPurchase28CGST);
    ui->allPurchaseTableWidget->setItem(3,3,ItemPurchase28IGST);
    ui->allPurchaseTableWidget->setItem(3,4,ItemPurchase28Total);

    ui->allPurchaseTableWidget->setItem(4,0,ItemPurchase0);
    ui->allPurchaseTableWidget->setItem(4,1,ItemPurchase0SGST);
    ui->allPurchaseTableWidget->setItem(4,2,ItemPurchase0CGST);
    ui->allPurchaseTableWidget->setItem(4,3,ItemPurchase0IGST);
    ui->allPurchaseTableWidget->setItem(4,4,ItemPurchase0Total);


    QTableWidgetItem *TItemSalesTotal = new QTableWidgetItem;
    QTableWidgetItem *TItemSalesSGST = new QTableWidgetItem;
    QTableWidgetItem *TItemSalesCGST = new QTableWidgetItem;
    QTableWidgetItem *TItemSalesIGST = new QTableWidgetItem;
    QTableWidgetItem *TItemSalesTaxable = new QTableWidgetItem;

    QTableWidgetItem *TItemPurchaseTotal = new QTableWidgetItem;
    QTableWidgetItem *TItemPurchaseSGST = new QTableWidgetItem;
    QTableWidgetItem *TItemPurchaseCGST = new QTableWidgetItem;
    QTableWidgetItem *TItemPurchaseIGST = new QTableWidgetItem;
    QTableWidgetItem *TItemPurchaseTaxable = new QTableWidgetItem;


    TItemSalesTotal->setText(QString::number(TotalSales,'f',2));
    TItemSalesSGST->setText(QString::number(TotalSGST,'f',2));
    TItemSalesCGST->setText(QString::number(TotalCGST,'f',2));
    TItemSalesIGST->setText(QString::number(TotalIGST,'f',2));
    TItemSalesTaxable->setText(QString::number(TotalTaxable,'f',2));

    TItemPurchaseTotal->setText(QString::number(TotalPurchase,'f',2));
    TItemPurchaseSGST->setText(QString::number(TotalPurchaseSGST,'f',2));
    TItemPurchaseCGST->setText(QString::number(TotalPurchaseCGST,'f',2));
    TItemPurchaseIGST->setText(QString::number(TotalPurchaseIGST,'f',2));
    TItemPurchaseTaxable ->setText(QString::number(TotalPurchaseTaxable,'f',2));


    TItemSalesTotal->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    TItemSalesSGST->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    TItemSalesCGST->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    TItemSalesIGST->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    TItemSalesTaxable->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);

    TItemPurchaseTotal->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    TItemPurchaseSGST->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    TItemPurchaseCGST->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    TItemPurchaseIGST->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    TItemPurchaseTaxable->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);


    ui->allSalesTableWidget->setItem(5,0,TItemSalesTaxable);
    ui->allSalesTableWidget->setItem(5,1,TItemSalesSGST);
    ui->allSalesTableWidget->setItem(5,2,TItemSalesCGST);
    ui->allSalesTableWidget->setItem(5,3,TItemSalesIGST);
    ui->allSalesTableWidget->setItem(5,4,TItemSalesTotal);
    ui->allPurchaseTableWidget->setItem(5,0,TItemPurchaseTaxable);
    ui->allPurchaseTableWidget->setItem(5,1,TItemPurchaseSGST);
    ui->allPurchaseTableWidget->setItem(5,2,TItemPurchaseCGST);
    ui->allPurchaseTableWidget->setItem(5,3,TItemPurchaseIGST);
    ui->allPurchaseTableWidget->setItem(5,4,TItemPurchaseTotal);

    float totalSalesGst = TotalCGST + TotalSGST + TotalIGST;
    float totalPurchaseGst = TotalPurchaseSGST + TotalPurchaseCGST + TotalPurchaseIGST;


//    float vatdiff = TotalPurchase - TotalSales;
    float vatdiff = totalPurchaseGst - totalSalesGst;
    if(vatdiff > 0){
        ui->payableCreditLineEdit->setText(QString::number(vatdiff,'f',2));
    }
    else{
        ui->receivableCreditLineEdit->setText(QString::number(-vatdiff,'f',2));
    }


}

void GSTReport::setTableSales()
{
    qDebug()<<"tab clicked"<<ui->tabWidget->currentIndex();

    QDate fromDate = ui->fromDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));
    salesModel = ledHelper->getB2BSalesReportWithTaxModelByDate(fromDateTime,toDateTime);

    sortSales = new GMItemSortFilterProxyModel(this);
    sortSales->setDynamicSortFilter(true);
    sortSales->setSourceModel(salesModel);

    salesTableView->setAvlcols(QList<int>({4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20}));
    salesTableView->setModel(salesModel);
    salesTableView->setModelQry(salesModel);
    salesTableView->setSortingEnabled(true);

    salesTableView->setColumns();
    salesTableView->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);

    float total = 0;
    for(int i=0;i<salesModel->rowCount();i++){
        total += salesModel->record(i).value(6).toFloat();
    }

    qDebug()<<"sales @ 5 total "<<total;
}


void GSTReport::setTableb2bSales()
{
    qDebug()<<"tab clicked"<<ui->tabWidget->currentIndex();

    QDate fromDate = ui->fromDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));
//    b2bModel = ledHelper->getB2BSalesReportWithTaxModelByDate(fromDateTime,toDateTime);

//    sortB2BSales = new GMItemSortFilterProxyModel(this);
//    sortB2BSales->setDynamicSortFilter(true);
//    sortB2BSales->setSourceModel(b2bModel);


//    salesTableView->setAvlcols(QList<int>({5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21}));
//    salesTableView->setModel(sortB2BSales);
//    qDebug()<<"set model";
//    salesTableView->setModelQry(b2bModel);
    qDebug()<<"set qry model";
    salesTableView->setSortingEnabled(true);
    qDebug()<<"to set cols"<<sortSales->columnCount();
    salesTableView->setColumns();
    qDebug()<<"631 line";
    salesTableView->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);


//    float total = 0;
//    for(int i=0;i<salesModel->rowCount();i++){
//        total += salesModel->record(i).value(4).toFloat();
    //    }
}

void GSTReport::setTableb2cSales()
{
    qDebug()<<"tab clicked"<<ui->tabWidget->currentIndex();

    QDate fromDate = ui->fromDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));
    b2cModel = ledHelper->getB2CSalesReportWithTaxModelByDate(fromDateTime,toDateTime);

    sortB2CSales = new GMItemSortFilterProxyModel(this);
    sortB2CSales->setDynamicSortFilter(true);
    sortB2CSales->setSourceModel(b2cModel);


    b2csalesTableView->setAvlcols(QList<int>({5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21}));
    b2csalesTableView->setModel(b2cModel);
    b2csalesTableView->setModelQry(b2cModel);
    b2csalesTableView->setSortingEnabled(true);

    b2csalesTableView->setColumns();
    b2csalesTableView->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);


}

void GSTReport::setTablePurchase()
{
    qDebug()<<"tab clicked"<<ui->tabWidget->currentIndex();

    QDate fromDate = ui->fromDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));
    purchaseModel = ledHelper->getPurchaseReportWithTaxModelByDate(fromDateTime,toDateTime);
    sortPurchase = new GMItemSortFilterProxyModel(this);
    sortPurchase->setDynamicSortFilter(true);
    sortPurchase->setSourceModel(purchaseModel);

    purchaseTableView->setAvlcols(QList<int>({4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19, 20}));
    purchaseTableView->setModel(purchaseModel);
    purchaseTableView->setModelQry(purchaseModel);
    purchaseTableView->setSortingEnabled(true);

    purchaseTableView->setColumns();
    purchaseTableView->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);

//    float total = 0;
//    for(int i=0;i<purchaseModel->rowCount();i++){
//        total += purchaseModel->record(i).value(3).toFloat();
//    }
}

void GSTReport::setTableCreditNote()
{
    QDate fromDate = ui->fromDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));
    creditNoteModel = ledHelper->getCreditNoteReportModelByDate(fromDateTime,toDateTime);
    sortCredit = new GMItemSortFilterProxyModel(this);
    sortCredit->setDynamicSortFilter(true);
    sortCredit->setSourceModel(creditNoteModel);


    creditNoteTableView->setAvlcols(QList<int>({4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19, 20}));
    creditNoteTableView->setModel(sortCredit);
    creditNoteTableView->setModelQry(creditNoteModel);
    creditNoteTableView->setSortingEnabled(true);
    creditNoteTableView->setColumns();
    creditNoteTableView->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);

}

void GSTReport::setTableDebitNote()
{
    QDate fromDate = ui->fromDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));
    debitNoteModel = ledHelper->getDebitNoteReportModelByDate(fromDateTime,toDateTime);
    sortDebit = new GMItemSortFilterProxyModel(this);
    sortDebit->setDynamicSortFilter(true);
    sortDebit->setSourceModel(debitNoteModel);


    debitNoteTableView->setAvlcols(QList<int>({4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19, 20}));
    debitNoteTableView->setModel(sortDebit);
    debitNoteTableView->setModelQry(debitNoteModel);
    debitNoteTableView->setSortingEnabled(true);
    debitNoteTableView->setColumns();
    debitNoteTableView->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);

}

void GSTReport::setHsnTable()
{
    qDebug()<<"tab clicked"<<ui->tabWidget->currentIndex();

    QDate fromDate = ui->fromDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));
    hsnModel = dbHelper->getTaxReportModelByHSN(fromDate,toDate);
    sortHSN = new GMItemSortFilterProxyModel(this);
    sortHSN->setDynamicSortFilter(true);
    sortHSN->setSourceModel(hsnModel);

    hsnTableView->setAvlcols(QList<int>({3,4,5,6,7}));
    hsnTableView->setModel(sortHSN);
    hsnTableView->setModelQry(hsnModel);
    hsnTableView->setSortingEnabled(true);
    hsnTableView->setColumns();
    hsnTableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);

}

void GSTReport::setCessReport()
{
    qDebug()<<"tab clicked"<<ui->tabWidget->currentIndex();

    ShiftDataModel *shift = new ShiftDataModel;
    ShiftDatabaseHelper shiftHelper;

    QDate fromDate = ui->fromDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));


    shift->ShiftStartDateTime = fromDateTime;
    shift->ShiftEndDateTime = toDateTime;

    QString billsFrom = shiftHelper.getFirstBillByDate(shift);
    QString billsTo = shiftHelper.getLastBillByDate(shift);

    QSqlQueryModel *cessModel = dbHelper->getCessReportModel(billsFrom, billsTo);

    GMItemSortFilterProxyModel *sortCess = new GMItemSortFilterProxyModel(this);
    sortCess->setDynamicSortFilter(true);
    sortCess->setSourceModel(cessModel);

    cessTableView->setAvlcols(QList<int>({1}));
    cessTableView->setModel(sortCess);
    cessTableView->setModelQry(cessModel);
    cessTableView->setSortingEnabled(true);
    cessTableView->setColumns();
    cessTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

void GSTReport::on_OKButton_clicked()
{
    loadedAllGst = false;
    loadedSales = false;
    loadedPurchase = false;
    loadedCreditNotes = false;
    loadedDebitNotes = false;
    loadedHsn = false;
    loadedCess = false;

    setTable();
}

void GSTReport::on_excelButton_clicked()
{
    ExcelReportHelper excel;
    QDate fDate = ui->fromDateEdit->date();
    QDate tDate = ui->toDateEdit->date();
    QString fromDate = fDate.toString("yyyy-MM-dd");
    QString toDate = tDate.toString("yyyy-MM-dd");
    QString reportName = "Gst Report";
    QString dateNow = QDate::currentDate().toString("dd_MMM_yyyy");
    QStringList cols; cols<<"30"<<"20"<<"20"<<"20"<<"20";
    QList<int> avlCols; avlCols<<0<<1<<2<<3<<4;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\"+reportName+"_"+dateNow+".xlsx",tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
    {
        return;
    }
    QTableView *t = ui->allSalesTableWidget;
    if(!loadedAllGst){
        setTableAll();
        loadedAllGst = true;
    }
    if(!loadedSales){
        setTableSales();
        loadedSales = true;
    }
    if(!loadedb2cSales){
        setTableb2cSales();
        loadedb2cSales = true;
    }
    if(!loadedDebitNotes){
        setTableDebitNote();
        loadedDebitNotes = true;
    }
    if(!loadedCreditNotes){
        setTableCreditNote();
        loadedCreditNotes = true;
    }
    if(!loadedHsn){
        setHsnTable();
        loadedHsn = true;
    }
    if(!loadedPurchase){
        setTablePurchase();
        loadedPurchase = true;
    }
    if(!loadedCess){
        setCessReport();
        loadedCess = true;
    }

    excel.GenericReportFromTableView
            (fileName,"Input Gst",cols,ui->allPurchaseTableWidget,fromDate,toDate,avlCols,true);

    excel.GenericReportFromTableView
            (fileName,"Ouput Gst",cols,t,fromDate,toDate,avlCols,true);

    cols.clear(); cols<<"25"<<"20"<<"20"<<"40"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20";
    avlCols.clear(); avlCols<<0<<1<<2<<3<<4<<5<<6<<7<<8<<9<<10<<11<<12<<13<<14<<15<<16<<17<<18<<19<<20<<21;
    excel.GenericReportFromTableView
            (fileName,"Sales B2B",cols,salesTableView,fromDate,toDate,avlCols,false);
    cols.clear(); cols<<"25"<<"20"<<"20"<<"40"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20";
    avlCols.clear(); avlCols<<0<<1<<2<<3<<4<<5<<6<<7<<8<<9<<10<<11<<12<<13<<14<<15<<16<<17<<18<<19<<20;
    excel.GenericReportFromTableView
            (fileName,"Sales B2C",cols,b2csalesTableView,fromDate,toDate,avlCols,false);

    cols.clear(); cols<<"25"<<"20"<<"20"<<"40"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20";
    avlCols.clear(); avlCols<<0<<1<<2<<3<<4<<5<<6<<7<<8<<9<<10<<11<<12<<13<<14<<15<<16<<17<<18<<19<<20;
    excel.GenericReportFromTableView
            (fileName,"Purchases",cols,purchaseTableView,fromDate,toDate,avlCols,false);
    excel.GenericReportFromTableView
            (fileName,"Debit Note",cols,debitNoteTableView,fromDate,toDate,avlCols,false);
    excel.GenericReportFromTableView
            (fileName,"Credit Note",cols,creditNoteTableView,fromDate,toDate,avlCols,false);

    cols.clear(); cols<<"30"<<"30"<<"10"<<"20"<<"20"<<"20"<<"20"<<"20"<<"20";
    avlCols.clear(); avlCols<<0<<1<<2<<3<<4<<5<<6<<7<<8;
    excel.HSNReportFromTableView
            (fileName,"HSN",cols,hsnTableView,fromDate,toDate,avlCols,false);
    cols.clear(); cols<<"30"<<"20";
    avlCols.clear(); avlCols<<0<<1;
    excel.GenericReportFromTableView
            (fileName,"Cess",cols,cessTableView,fromDate,toDate,avlCols,false);


    qDebug()<<"Excel Completed";
}

void GSTReport::on_tabWidget_tabBarClicked(int index)
{
    setTable();
}

void GSTReport::on_tabWidget_currentChanged(int index)
{
    setTable();
}
