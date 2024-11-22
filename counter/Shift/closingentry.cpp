#include "closingentry.h"
#include "ui_closingentry.h"

ClosingEntry::ClosingEntry(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClosingEntry)
{
    ui->setupUi(this);
    printer = new CounterPrinterHelper;
    dbHelper = new DailyClosingDatabaseHelper;
    ledhelper = new LedgerMasterDatabaseHelper;
    shiftDatabaseHelper = new ShiftDatabaseHelper;

    CurrClosing = new ShiftDataModel;

    lastClosing = dbHelper->getlastClosingDetailByVoucherPrefix();

    CurrClosing->ShiftStartDateTime = lastClosing.ShiftEndDateTime;
    CurrClosing->ShiftEndDateTime = QDateTime::currentDateTime();


    CurrClosing->BillsFrom = QString::number(lastClosing.BillsTo.toInt()+1);

    CurrClosing->voucherPrefix = "%";
    CurrClosing->cashierID = LoginValues::userID;
    CurrClosing->cashierName = LoginValues::userName;
    CurrClosing->cashID = ledhelper->getLedgerIDByName("Cash");

    shiftsPending = shiftDatabaseHelper->getAllShiftDetailsByDate(
                CurrClosing->ShiftStartDateTime,CurrClosing->ShiftEndDateTime);

    ui->ShiftsGroup->hide();

}

ClosingEntry::~ClosingEntry()
{
    delete ui;
}


void ClosingEntry::on_CancelButton_clicked()
{
    this->close();
}


void ClosingEntry::on_OkButton_clicked()
{

}

void ClosingEntry::calcValues()
{
    CurrClosing->BillsFrom = dbHelper->getFirstBillinShift(CurrClosing);
    CurrClosing->BillsTo = dbHelper->getLastBill(CurrClosing);
    CurrClosing->billsCount = dbHelper->getBillsCount(CurrClosing);
    CurrClosing->cashierName = LoginValues::userName;

    CurrClosing->cashierID = LoginValues::userID;
    CurrClosing->ShiftStartDateTime =lastClosing.ShiftEndDateTime;

    if(CurrClosing->ShiftStartDateTime.toString("yyyy").length()<1 ){
        QString nulldate = "2017-01-01 00:00:00";
        CurrClosing->ShiftStartDateTime = QDateTime::fromString(nulldate,"yyyy-MM-dd hh:mm:ss");
    }

    CurrClosing->ShiftEndDateTime = QDateTime::currentDateTime();
    CurrClosing->Narration = ui->narration->toPlainText();
    CurrClosing->POSNumber = CounterSettingsDataModel::POSNumber;

    CurrClosing->OpeningCash = dbHelper->getOpeningCash(CurrClosing);
    CurrClosing->SalesTotal = dbHelper->getSalesTotal(CurrClosing);
    CurrClosing->CashSales = dbHelper->getSalesByCash(CurrClosing);
    CurrClosing->CardSales = dbHelper->getSalesByCard(CurrClosing);
    CurrClosing->CreditSales = dbHelper->getSalesByCredit(CurrClosing);

    CurrClosing->CashPurchase = dbHelper->getPurchaseByCash(CurrClosing);
    CurrClosing->CreditPurchase = dbHelper->getPurchaseByCredit(CurrClosing);
    CurrClosing->PurchaseTotal =dbHelper->getPurchaseTotal(CurrClosing);

    CurrClosing->CashReceipts = dbHelper->getReceiptByCash(CurrClosing);
    CurrClosing->CashPayments = dbHelper->getPaymentByCash(CurrClosing);

    CurrClosing->ZomatoSales = dbHelper->getZomatoSalesByCash(CurrClosing);
    CurrClosing->ZomatoCash = dbHelper->getZomatoSalesByCash(CurrClosing);
    CurrClosing->ZomatoCredit = dbHelper->getZomatoSalesByCredit(CurrClosing);

    CurrClosing->totalCash = dbHelper->getClosingCash(CurrClosing);

    CurrClosing->TillDifference =  CurrClosing->totalCash -  CurrClosing->CashInCounter;
    CurrClosing->CashBalance      = CurrClosing->CashInCounter - CurrClosing->CashWithDrawn;

}

void ClosingEntry::setValues()
{

    ui->noOfShiftsLineEdit->setText(QString::number(shiftsPending.size()));
    ui->billsFromLineEdit->setText(CurrClosing->BillsFrom);
    ui->billsToLineEdit->setText(CurrClosing->BillsTo);

    ui->salesTotalLineEdit->setText(QString::number(CurrClosing->SalesTotal,'f',2));
    ui->cardSalesLineEdit->setText(QString::number(CurrClosing->CardSales,'f',2));
    ui->creditSalesLineEdit->setText(QString::number(CurrClosing->CreditSales,'f',2));
    ui->purchaseTotalLineEdit->setText(QString::number(CurrClosing->PurchaseTotal,'f',2));

    ui->openingCashLineEdit->setText(QString::number(CurrClosing->OpeningCash,'f',2));
    ui->cashSalesLineEdit->setText(QString::number(CurrClosing->CashSales,'f',2));
    ui->cashPurchaseLineEdit->setText(QString::number(CurrClosing->CashPurchase,'f',2));
    ui->cashPaymentsLineEdit->setText(QString::number(CurrClosing->CashPayments,'f',2));
    ui->cashReceiptsLineEdit->setText(QString::number(CurrClosing->CashReceipts,'f',2));
    ui->closingCashLineEdit->setText(QString::number(CurrClosing->totalCash,'f',2));
}
