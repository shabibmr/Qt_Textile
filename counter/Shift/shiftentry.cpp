#include "shiftentry.h"
#include "ui_shiftentry.h"

ShiftEntry::ShiftEntry(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShiftEntry)
{
    ui->setupUi(this);
    shift = new ShiftDataModel;
    dbHelper = new ShiftDatabaseHelper();
    salesHelper = new salesVoucherDatabaseHelper();
    ledHelper  = new LedgerHelper();

    //    ui->outputVatAmountLabel->hide();
    //    ui->outputVatAmountLineEdit->hide();
    //    ui->taxableTotalLabel->hide();
    //    ui->taxableTotalLineEdit->hide();


    ui->CalculateButton->hide();
    ui->CalculateButton->setText("Edit");

    ui->printButton->setDisabled(true);
    ui->CloseButton->setDisabled(true);

    if(CounterSettingsDataModel::TypeOfTrade!="Restaurant")
        ui->ZomatoWidget->hide();

    shift->cashID = CounterSettingsDataModel::DefaultCash;
    shift->voucherPrefix = LoginValues::voucherPrefix;

    setAllNonEditable();

}

ShiftEntry::~ShiftEntry()
{
    delete ui;
}

void ShiftEntry::setAllNonEditable()
{

    ui->cashWithdrawnLineEdit->setReadOnly(false);
    ui->narration->setReadOnly(false);
    ui->openingCashLineEdit->setReadOnly(false);

}

void ShiftEntry::on_printButton_clicked()
{
    CounterPrinterHelper pHelper;
    pHelper.printShiftReport(shift);
    pHelper.printExpenses(PaymentDetails);
    ui->CloseButton->setEnabled(true);

}

void ShiftEntry::on_saveButton_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Close Shift","Are you sure you want to close Shift",
                                  QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::Yes){
        dbHelper->updateShiftData(shift,shift->shiftID);
        ui->saveButton->setDisabled(true);
        ui->narration->setDisabled(true);
        ui->printButton->setEnabled(true);

    }
}

void ShiftEntry::on_CloseButton_clicked()
{

    this->close();
    //    emit shiftClosed();

}

void ShiftEntry::on_CalculateButton_clicked()
{
    if(ui->CalculateButton->text()=="Edit"){
        ui->CalculateButton->setText("Calculate");
    }
    else{
        CalculateValues();
    }

}

void ShiftEntry::setDenominationValues(float amount){
    shift->CashInCounter = amount;
    CalculateValues();
    setValues();
    shift->shiftID=dbHelper->insertShiftData(shift);
}

void ShiftEntry::CalculateValues()
{
    DailyClosingDatabaseHelper closingHelper;
    //shift->BillsFrom = QString::number(closingHelper.getlastClosingDetailByVoucherPrefix().BillsTo.toInt()+1);

    shift->BillsFrom =  dbHelper->getFirstBillinShift(shift);

    shift->BillsTo = dbHelper->getLastBill(shift);
    shift->billsCount = dbHelper->getBillsCount(shift);
    shift->cashierName = LoginValues::userName;
    shift->cashierID = LoginValues::userID;

    // shift from daily closing
    //shift->ShiftStartDateTime = closingHelper.getlastClosingDetailByVoucherPrefix().ShiftEndDateTime;

    // shift from last shift
        shift->ShiftStartDateTime = dbHelper->getlastShiftDetailByVoucherPrefix(
                    shift->voucherPrefix).ShiftEndDateTime;

    if(shift->ShiftStartDateTime.toString("yyyy").length()<1 ){
        QString nulldate = "2017-01-01 00:00:00";
        shift->ShiftStartDateTime = QDateTime::fromString(nulldate,"yyyy-MM-dd hh:mm:ss");
    }

    shift->ShiftEndDateTime = QDateTime::currentDateTime();
    shift->Narration = ui->narration->toPlainText();
    shift->POSNumber = CounterSettingsDataModel::POSNumber;

    //shift->OpeningCash=dbHelper->getOpeningCash(shift);
    shift->OpeningCash= dbHelper->getOpeningCash(shift);
    shift->SalesTotal = dbHelper->getSalesTotal(shift);
    shift->CashSales = dbHelper->getSalesByCash(shift);
    shift->CardSales = dbHelper->getSalesByCard(shift);
    shift->CreditSales = dbHelper->getSalesByCredit(shift);
    shift->vatTotal = dbHelper->getVatTotal(shift);
    shift->taxableTotal = shift->SalesTotal - shift->vatTotal;

    shift->CashPurchase = dbHelper->getPurchaseByCash(shift);
    shift->CreditPurchase = dbHelper->getPurchaseByCredit(shift);
    shift->PurchaseTotal = dbHelper->getPurchaseTotal(shift);

    shift->CashReceipts = dbHelper->getReceiptByCash(shift);
    shift->CashPayments = dbHelper->getPaymentByCash(shift);

    PaymentDetails = dbHelper->getPaymentDetails(shift);

    shift->ZomatoSales = dbHelper->getZomatoSalesTotal(shift);
    shift->ZomatoCash = dbHelper->getZomatoSalesByCash(shift);
    shift->ZomatoCredit = dbHelper->getZomatoSalesByCredit(shift);

    shift->totalCash = shift->OpeningCash + shift->CashSales - shift->CashPurchase
            -shift->CashPayments + shift->CashReceipts;

    shift->TillDifference   = shift->CashInCounter-shift->totalCash;
    shift->CashBalance      = shift->CashInCounter - shift->CashWithDrawn;

    ui->cashBalanceInCounterLineEdit->setText(QString::number(shift->CashBalance,'f',2));

    //shift->ShiftStartDateTime = dbHelper->getlastShiftDetailByVoucherPrefix(
    //            shift->voucherPrefix).ShiftEndDateTime;

    //    if(shift->ShiftStartDateTime.toString("yyyy").length()<1 ){
    //        QString nulldate = "2017-01-01 00:00:00";
    //        shift->ShiftStartDateTime = QDateTime::fromString(nulldate,"yyyy-MM-dd hh:mm:ss");
    //    }
    qDebug()<<"Shift 133";

}

void ShiftEntry::setValues()
{
    ui->billFromLineEdit->setText(shift->BillsFrom);
    ui->billToLineEdit->setText(shift->BillsTo);
    ui->billsCountLineEdit->setText(QString::number(shift->billsCount));
    ui->cashierNameLineEdit->setText(shift->cashierName);


    ui->timeFromLineEdit->setText(shift->ShiftStartDateTime.toString("dd-MM-yyyy hh:mm"));
    ui->timeToLineEdit->setText(shift->ShiftEndDateTime.toString("dd-MM-yyyy hh:mm"));
    ui->narration->setPlainText(shift->Narration);
    ui->pOSNoLineEdit->setText(shift->POSNumber);
    ui->shiftIDLineEdit->setText(QString::number(shift->shiftID));

    ui->cashInCounterLineEdit->setText(QString::number(shift->CashInCounter,'f',2));

    ui->openingCashLineEdit->setText( QString::number(shift->OpeningCash,'f',2));
    ui->salesTotalLineEdit->setText(QString::number(shift->SalesTotal,'f',2));
    ui->salesByCashLineEdit->setText(QString::number(shift->CashSales,'f',2));
    ui->salesByCardLineEdit->setText(QString::number(shift->CardSales,'f',2));
    ui->salesCreditLineEdit->setText(QString::number(shift->CreditSales,'f',2));

    ui->outputVatAmountLineEdit->setText(QString::number(shift->vatTotal,'f',2));
    ui->taxableTotalLineEdit->setText(QString::number(shift->taxableTotal,'f',2));

    ui->cashPurchaseLineEdit->setText(QString::number(shift->CashPurchase,'f',2));
    ui->creditPurchaseLine->setText(QString::number(shift->CreditPurchase,'f',2));
    ui->purchaseTotal->setText(QString::number(shift->PurchaseTotal,'f',2));

    ui->receiptsLineEdit->setText(QString::number(shift->CashReceipts,'f',2));
    ui->cashPaymentsLineEdit->setText(QString::number(shift->CashPayments,'f',2));

    ui->totalCashLineEdit->setText(QString::number(shift->totalCash,'f',2));
    ui->cashWithdrawnLineEdit->setText(QString::number(shift->CashWithDrawn,'f',2));
    ui->cashBalanceInCounterLineEdit->setText(QString::number(shift->CashBalance,'f',2));

    ui->tillDifferenceLineEdit->setText(QString::number(shift->TillDifference,'f',2));

    ui->zomatosalesLineEdit->setText(QString::number(shift->ZomatoSales,'f',2));
    ui->zomatoCashLineEdit->setText(QString::number(shift->ZomatoCash,'f',2));
    ui->zomatoCODLineEdit->setText(QString::number(shift->ZomatoCredit,'f',2));
    qDebug()<<"Shift values set";
}

void ShiftEntry::on_cashWithdrawnLineEdit_textChanged(const QString &arg1)
{
    shift->CashWithDrawn    = ui->cashWithdrawnLineEdit->text().toFloat();
    shift->CashBalance      = shift->CashInCounter - shift->CashWithDrawn;
    ui->cashBalanceInCounterLineEdit->setText(QString::number(shift->CashBalance,'f',2));
}

void ShiftEntry::on_openingCashLineEdit_returnPressed()
{
    shift->OpeningCash = ui->openingCashLineEdit->text().toFloat();

    shift->totalCash = shift->OpeningCash + shift->CashSales - shift->CashPurchase
            -shift->CashPayments + shift->CashReceipts;

    shift->TillDifference =  shift->totalCash -  shift->CashInCounter;
    shift->CashBalance      = shift->CashInCounter - shift->CashWithDrawn;

    ui->cashBalanceInCounterLineEdit->setText(QString::number(shift->CashBalance,'f',2));

    setValues();
}


