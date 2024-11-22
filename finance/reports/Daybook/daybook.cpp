#include "daybook.h"
#include "ui_daybook.h"
#include "customwidgets/Delegates/reportcommondelegate.h"
Daybook::Daybook(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Daybook)
{
    ui->setupUi(this);

    ui->fromDateDateEdit->setDate(QDate::currentDate());
    ui->toDateDateEdit->setDate(QDate::currentDate());

    ledHelper = new LedgerHelper();
    ui->tableView->setItemDelegate(new ReportCommonDelegate());
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


//    ui->tableWidget->setColumnCount(7);

//    int i=0;

//    QTableWidgetItem* SLNO = new QTableWidgetItem;
//    SLNO->setText("#");
//    ui->tableWidget->setHorizontalHeaderItem(i++,SLNO);

//    QTableWidgetItem* dateItem = new QTableWidgetItem;
//    dateItem->setText("Date");
//    ui->tableWidget->setHorizontalHeaderItem(i++,dateItem);

//    QTableWidgetItem* LedgerName = new QTableWidgetItem;
//    LedgerName->setText("Ledger Name");
//    ui->tableWidget->setHorizontalHeaderItem(i++,LedgerName);

//    QTableWidgetItem* VchrType = new QTableWidgetItem;
//    VchrType->setText("Voucher Type");
//    ui->tableWidget->setHorizontalHeaderItem(i++,VchrType);

//    QTableWidgetItem* VchrNo = new QTableWidgetItem;
//    VchrNo->setText("Voucher No.");
//    ui->tableWidget->setHorizontalHeaderItem(i++,VchrNo);

//    QTableWidgetItem* Debit = new QTableWidgetItem;
//    Debit->setText("Debit");
//    ui->tableWidget->setHorizontalHeaderItem(i++,Debit);

//    QTableWidgetItem* Credit = new QTableWidgetItem;
//    Credit->setText("Credit");
//    ui->tableWidget->setHorizontalHeaderItem(i++,Credit);

//    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

//    ui->tableWidget->hideColumn(0);
//    ui->tableWidget->setShowGrid(false);

    shareButton = new QtMaterialIconButton(QtMaterialTheme::icon("social", "share"));
    shareButton->setParent(this);
//    shareButton->setColor(QColor(255,250,0));
    shareButton->setIconSize(QSize(30,30));
    QObject::connect(shareButton,&QPushButton::clicked,
                     this,&Daybook::showShareOptions);

    ui->horizontalLayout->addWidget(shareButton);
    ui->excelButton->hide();
    ui->printButton->hide();

    setTable();
    this->setFocus();

}

Daybook::~Daybook()
{
    delete ui;
}

void Daybook::setTable()
{

        QDate dateFrom = ui->fromDateDateEdit->date();
        QDate dateTo = ui->toDateDateEdit->date();

    model = ledHelper->getDayBookReportModel(dateFrom,dateTo);
    ui->tableView->setModel(model);
//    QDate dateFrom = ui->fromDateDateEdit->date();
//    QDate dateTo = ui->toDateDateEdit->date();
//    voucherList = dbhelper->getVoucherListByDate(dateFrom,dateTo);
//    int cnt =0;
//    int i=0;
//    ui->tableWidget->setRowCount(0);
//    for(GeneralVoucherDataObject obj:voucherList){
//        ui->tableWidget->insertRow(cnt);
//        i=0;

//        QTableWidgetItem* SLNO = new QTableWidgetItem;
//        SLNO->setText(QString::number(cnt+1));
//        SLNO->setFlags(SLNO->flags() ^ Qt::ItemIsEditable);
//        ui->tableWidget->setItem(cnt,i++,SLNO);

//        QTableWidgetItem* dateItem = new QTableWidgetItem;
//        dateItem->setText(obj.VoucherDate.toString("dd-MM-yyyy"));
//        dateItem->setFlags(dateItem->flags() ^ Qt::ItemIsEditable);
//        ui->tableWidget->setItem(cnt,i++,dateItem);

//        QTableWidgetItem* LedgerName = new QTableWidgetItem;
//        LedgerName->setText(obj.Contact.CompanyName);
//        LedgerName->setFlags(LedgerName->flags() ^ Qt::ItemIsEditable);
//        ui->tableWidget->setItem(cnt,i++,LedgerName);

//        QTableWidgetItem* VchrType = new QTableWidgetItem;
//        VchrType->setText(obj.voucherNumber);
//        VchrType->setFlags(VchrType->flags()^ Qt::ItemIsEditable);
//        ui->tableWidget->setItem(cnt,i++,VchrType);

//        QTableWidgetItem* VchrNo = new QTableWidgetItem;
//        VchrNo->setTextAlignment(Qt::AlignRight |Qt::AlignVCenter);
//        VchrNo->setText(obj.status);
//        VchrNo->setFlags(VchrNo->flags() ^ Qt::ItemIsEditable);
//        ui->tableWidget->setItem(cnt,i++,VchrNo);

//        QTableWidgetItem* Debit = new QTableWidgetItem;
//        Debit->setText(QString::number(obj.grandTotal,'f',2));
//        Debit->setTextAlignment(Qt::AlignRight |Qt::AlignVCenter);
//        Debit->setFlags(Debit->flags()^ Qt::ItemIsEditable);
//        ui->tableWidget->setItem(cnt,i++,Debit);

//        QTableWidgetItem* Credit = new QTableWidgetItem;
//        Credit->setText(QString::number(obj.grandTotal,'f',2));
//        Credit->setTextAlignment(Qt::AlignRight |Qt::AlignVCenter);
//        Credit->setFlags(Credit->flags()^ Qt::ItemIsEditable);
//        ui->tableWidget->setItem(cnt,i++,Credit);

//        cnt++;
//    }
}

void Daybook::on_OKButton_clicked()
{
    setTable();
}

void Daybook::on_excelButton_clicked()
{
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();
    QString reportName = "Day Book";

    QString dateNow = QDate::currentDate().toString("dd_MMM_yyyy");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\"+reportName+"_"+dateNow+".xlsx",tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
    {
        return;
    }


    ExcelReportHelper *ex = new ExcelReportHelper;

    QList<int> avlCols; avlCols<<0<<1<<2<<3<<4<<5;
    ex->GenerateReportFromTableView(fileName,reportName,
                                    widthList,ui->tableView,
                                    fromDate.toString("yyyy-MM-dd"),
                                    toDate.toString("yyyy-MM-dd"),
                                    avlCols);
}

void Daybook::on_printButton_clicked()
{
    ShiftDatabaseHelper sHelper;
    ShiftDataModel *shift = new ShiftDataModel;
    ShiftDataModel *prevShift = new ShiftDataModel;


    QDate fromDate = ui->fromDateDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

    shift->voucherPrefix = LoginValues::voucherPrefix;
    shift->cashID = CounterSettingsDataModel::DefaultCash;
    shift->ShiftStartDateTime = fromDateTime;
    shift->ShiftEndDateTime = toDateTime;
    shift->BillsFrom = sHelper.getFirstBillByDate(shift);
    shift->BillsTo = sHelper.getLastBillByDate(shift);
    shift->billsCount = sHelper.getBillsCount(shift);

    prevShift->voucherPrefix = CounterSettingsDataModel::VoucherPrefix;
    prevShift->cashID = CounterSettingsDataModel::DefaultCash;
    QString nulldate = "2017-01-01 00:00:00";
    prevShift->ShiftStartDateTime = QDateTime::fromString(nulldate,"yyyy-MM-dd hh:mm:ss");
    prevShift->ShiftEndDateTime = fromDateTime;
    prevShift->BillsFrom = "0";
    prevShift->BillsTo = QString::number(shift->BillsFrom.toInt()-1);

    LedgerMasterDatabaseHelper led;
    LedgerMasterDataModel Cash = led.getLedgerObjectByID(prevShift->cashID);

    prevShift->OpeningCash           = -Cash.openingBalance;
    prevShift->CashSales             = sHelper.getSalesByCash(prevShift);
//    prevShift->ShiftEndDateTime      = prevShift->ShiftEndDateTime.addDays(-1);

    prevShift->CashPurchase          = sHelper.DayBookgetPurchaseByCash(prevShift);
    prevShift->CashReceipts          = sHelper.DayBookgetReceiptByCash(prevShift);
    prevShift->CashPayments          = sHelper.DayBookgetPaymentByCash(prevShift);

    qDebug()<<"till Time "<<prevShift->ShiftEndDateTime.toString("yyyy-MM-dd hh:mm:ss");
    qDebug()<<prevShift->OpeningCash<<prevShift->CashSales<<prevShift->CashPurchase;
    qDebug()<<prevShift->CashReceipts<<prevShift->CashPayments;


    shift->OpeningCash           =  prevShift->OpeningCash+prevShift->CashSales+prevShift->CashReceipts
            -prevShift->CashPurchase-prevShift->CashPayments;
    shift->SalesTotal            = sHelper.getSalesTotal(shift);
    shift->CashSales             = sHelper.getSalesByCash(shift);
    shift->CreditSales           = sHelper.getSalesByCredit(shift);
    shift->CardSales             = sHelper.getSalesByCard(shift);

    shift->vatTotal              = sHelper.getVatTotal(shift);
    shift->taxableTotal          = shift->SalesTotal - shift->vatTotal;


    shift->PurchaseTotal         = sHelper.DayBookgetPurchaseTotal(shift);
    shift->CreditPurchase        = sHelper.DayBookgetPurchaseByCredit(shift);
    shift->CashPurchase          = sHelper.DayBookgetPurchaseByCash(shift);

    shift->PaymentsTotal         = sHelper.DayBookgetPaymentTotal(shift);
    shift->ReceiptTotal          = sHelper.DayBookgetReceiptTotal(shift);

    shift->CashReceipts          = sHelper.DayBookgetReceiptByCash(shift);
    shift->CashPayments          = sHelper.DayBookgetPaymentByCash(shift);



    shift->ZomatoSales           = sHelper.getZomatoSalesTotal(shift);
    shift->ZomatoCash            = sHelper.getZomatoSalesByCash(shift);
    shift->ZomatoCredit          = sHelper.getZomatoSalesByCredit(shift);


    shift->CashBalance           = shift->OpeningCash + shift->CashSales + shift->CashReceipts
            - shift->CashPayments - shift->CashPurchase;


    GeneralVoucherDataObject PaymentDetails     = sHelper.getCreditPaymentDetails(shift);
    GeneralVoucherDataObject ReceiptDetails     = sHelper.getCreditReceiptDetails(shift);
    GeneralVoucherDataObject SalesDetails       = sHelper.getCreditSalesDetails(shift);
    GeneralVoucherDataObject PurchaseDetails    = sHelper.getCreditPurchaseDetails(shift);

    CounterPrinterHelper printer;

    printer.printDaybookReport(shift,PaymentDetails,ReceiptDetails,SalesDetails,PurchaseDetails);

}

void Daybook::on_backButton_clicked()
{
    this->close();
}

void Daybook::showShareOptions()
{
    shareOptionsWidget = new ShareTools("Daybook", shareButton);
    shareOptionsWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup|Qt::WindowStaysOnTopHint);;
//    shareOptionsWidget->setAttribute(Qt::WA_DeleteOnClose);
    shareOptionsWidget->show();

    QObject::connect(shareOptionsWidget,&ShareTools::printClicked,
                     this,&Daybook::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::pdfClicked,
                     this,&Daybook::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::excelClicked,
                     this,&Daybook::on_excelButton_clicked);
}

void Daybook::printReport(QPrinter *printer)
{
    ShiftDatabaseHelper sHelper;
    ShiftDataModel *shift = new ShiftDataModel;
    ShiftDataModel *prevShift = new ShiftDataModel;


    QDate fromDate = ui->fromDateDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

    shift->voucherPrefix = LoginValues::voucherPrefix;
    shift->cashID = CounterSettingsDataModel::DefaultCash;
    shift->ShiftStartDateTime = fromDateTime;
    shift->ShiftEndDateTime = toDateTime;
    shift->BillsFrom = sHelper.getFirstBillByDate(shift);
    shift->BillsTo = sHelper.getLastBillByDate(shift);
    shift->billsCount = sHelper.getBillsCount(shift);

    prevShift->voucherPrefix = CounterSettingsDataModel::VoucherPrefix;
    prevShift->cashID = CounterSettingsDataModel::DefaultCash;
    QString nulldate = "2017-01-01 00:00:00";
    prevShift->ShiftStartDateTime = QDateTime::fromString(nulldate,"yyyy-MM-dd hh:mm:ss");
    prevShift->ShiftEndDateTime = fromDateTime;
    prevShift->BillsFrom = "0";
    prevShift->BillsTo = QString::number(shift->BillsFrom.toInt()-1);

    LedgerMasterDatabaseHelper led;
    LedgerMasterDataModel Cash = led.getLedgerObjectByID(prevShift->cashID);

    prevShift->OpeningCash           = -Cash.openingBalance;
    prevShift->CashSales             = sHelper.getSalesByCash(prevShift);
//    prevShift->ShiftEndDateTime      = prevShift->ShiftEndDateTime.addDays(-1);

    prevShift->CashPurchase          = sHelper.DayBookgetPurchaseByCash(prevShift);
    prevShift->CashReceipts          = sHelper.DayBookgetReceiptByCash(prevShift);
    prevShift->CashPayments          = sHelper.DayBookgetPaymentByCash(prevShift);

    qDebug()<<"till Time "<<prevShift->ShiftEndDateTime.toString("yyyy-MM-dd hh:mm:ss");
    qDebug()<<prevShift->OpeningCash<<prevShift->CashSales<<prevShift->CashPurchase;
    qDebug()<<prevShift->CashReceipts<<prevShift->CashPayments;


    shift->OpeningCash           =  prevShift->OpeningCash+prevShift->CashSales+prevShift->CashReceipts
            -prevShift->CashPurchase-prevShift->CashPayments;
    shift->SalesTotal            = sHelper.getSalesTotal(shift);
    shift->CashSales             = sHelper.getSalesByCash(shift);
    shift->CreditSales           = sHelper.getSalesByCredit(shift);
    shift->CardSales             = sHelper.getSalesByCard(shift);

    shift->vatTotal              = sHelper.getVatTotal(shift);
    shift->taxableTotal          = shift->SalesTotal - shift->vatTotal;


    shift->PurchaseTotal         = sHelper.DayBookgetPurchaseTotal(shift);
    shift->CreditPurchase        = sHelper.DayBookgetPurchaseByCredit(shift);
    shift->CashPurchase          = sHelper.DayBookgetPurchaseByCash(shift);

    shift->PaymentsTotal         = sHelper.DayBookgetPaymentTotal(shift);
    shift->ReceiptTotal          = sHelper.DayBookgetReceiptTotal(shift);

    shift->CashReceipts          = sHelper.DayBookgetReceiptByCash(shift);
    shift->CashPayments          = sHelper.DayBookgetPaymentByCash(shift);



    shift->ZomatoSales           = sHelper.getZomatoSalesTotal(shift);
    shift->ZomatoCash            = sHelper.getZomatoSalesByCash(shift);
    shift->ZomatoCredit          = sHelper.getZomatoSalesByCredit(shift);


    shift->CashBalance           = shift->OpeningCash + shift->CashSales + shift->CashReceipts
            - shift->CashPayments - shift->CashPurchase;


    GeneralVoucherDataObject PaymentDetails     = sHelper.getCreditPaymentDetails(shift);
    GeneralVoucherDataObject ReceiptDetails     = sHelper.getCreditReceiptDetails(shift);
    GeneralVoucherDataObject SalesDetails       = sHelper.getCreditSalesDetails(shift);
    GeneralVoucherDataObject PurchaseDetails    = sHelper.getCreditPurchaseDetails(shift);

    printerHelper = new CounterPrinterHelper;
    printerHelper->setPrinter(printer);
    printerHelper->printDaybookReport(shift,PaymentDetails,ReceiptDetails,SalesDetails,PurchaseDetails);

}
