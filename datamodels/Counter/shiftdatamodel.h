#ifndef SHIFTDATAMODEL_H
#define SHIFTDATAMODEL_H

#include <QObject>
#include <QDateTime>

#include "datamodels/Counter/denominationsdatamodel.h"
#include "datamodels/Counter/shiftledgersdatamodel.h"
#include<QMap>
class ShiftDataModel
{
public:
    ShiftDataModel();

    float OpeningCash   = 0;
    float CashSales     = 0;
    float CardSales     = 0;
    float CreditSales   = 0;
    float ZomatoSales   = 0;
    float ZomatoCash    = 0;
    float ZomatoCredit  = 0;


    float CarriageSales = 0;
    float deliverooSales = 0;
    float TalabatSales  = 0;
    float TalabatCredit = 0;
    float TalabatCash = 0;

    float SalesTotal    = 0;
    float PurchaseTotal = 0;
    float CashPurchase  = 0;
    float CreditPurchase= 0;
    float CardPurchase = 0;

    float CashReceipts  = 0;
    float CardReceipts = 0;
    float PaymentsTotal = 0;
    float ReceiptTotal  = 0;
    float CashPayments  = 0;
    float CardPayments = 0;

    float CashPurchaseReturn = 0;
    float CashSalesReturn = 0;
    float TotalPurchaseReturn = 0;
    float TotalSalesReturn = 0;


    float ClosingCash   = 0;
    float CashInCounter = 0;
    float CashWithDrawn = 0;
    float TillDifference= 0;

    float totalCash     = 0;
    float CashBalance   = 0;

    float taxableTotal  = 0;
    float vatTotal      = 0;

    float advanceRecieved = 0;

    int shiftID         = 0;

    QMap<QString,float> enteredValues;

    QString cashID;
    QString cashierName;
    int cashierID = 0;
    QString voucherPrefix = "%";
    QString POSNumber;
    QDateTime ShiftEndDateTime;
    QDateTime ShiftStartDateTime;
    QString Narration;
    QString MaciD;

    QString BillsFrom ="0";
    QString BillsTo ="0";
    int billsCount      = 0;
    int PaxCount        = 0;

    DenominationsDataModel *denominations;
    QList<ShiftLedgersDataModel *> ledgers;



    QString getShiftasHTML();


};

#endif // SHIFTDATAMODEL_H
