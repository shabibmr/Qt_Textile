#ifndef RESTAURANTSHIFTDATABASEHELPER_H
#define RESTAURANTSHIFTDATABASEHELPER_H

#include <QSqlDatabase>
#include <QSqlQuery>

#include "datamodels/Counter/shiftdatamodel.h"
#include "database/finance/salesvoucher/salesvoucherdatabasehelper.h"
#include "database/finance/purchasevoucher/purchasevoucherdatabasehelper.h"
#include "database/finance/paymentvoucher/paymentvoucherdatabasehelper.h"
#include "database/finance/receiptvoucher/receiptvoucherdatabasehelper.h"
#include "database/finance/TransactionHelper/transactionhelper.h"
#include "database/inventory/salesorder/salesorderdatabasehelper.h"
#include "database/finance/creditnote/creditnotevoucherdatabasehelper.h"
#include "database/finance/debitnote/debitnotevoucherdatabasehelper.h"

class RestaurantShiftDatabaseHelper
{
public:
    RestaurantShiftDatabaseHelper();


    DatabaseHelper *db;
    bool flag=true;

    salesVoucherDatabaseHelper shelper;

    TransactionHelper* trans;



    QString Shifts_TableName = "shifttable";
    QString Shift_ID    ="_id";
    QString Shift_ShiftTimeStamp = "ShiftDate";
    QString Shift_StartVoucher_No ="StartVoucherNo";
    QString Shift_Stopvoucher_No ="StopVoucherNo";
    QString Shift_SalesValue ="SalesValue";
    QString Shift_OpeningBalance ="OpeningBalance";
    QString Shift_CashSales ="CashSales";
    QString Shift_CardSales = "CardSales";
    QString Shift_CreditSales = "CreditSales";
//    QString Shift_ZomatoSales = "ZomatoSales";
//    QString Shift_ZomatoCash = "ZomatoCash";
//    QString Shift_ZomatoCredit = "ZomatoCredit";
    QString Shift_PurchaseCash = "PurchaseCash";
    QString Shift_PurchaseCredit = "PurchaseCredit";
    QString Shift_PurchaseCard = "PurchaseCard";
    QString Shift_PurchaseTotal = "PurchaseTotal";
    QString Shift_PaymentCash="PaymentCash";
    QString Shift_PaymentCard = "PaymentCard";
    QString Shift_PaymentTotal = "PaymentTotal";
    QString Shift_ReceiptCash = "ReceiptCash";
    QString Shift_ReceiptCard = "ReceiptCard";
    QString Shift_ReceiptTotal = "ReceiptTotal";
    QString Shift_cashierID = "cashierID";
    QString Shift_voucherPrefix= "voucherPrefix";
    QString Shift_MacId ="MacID";
    QString Shift_POSNumber ="POSNumber";
    QString Shift_paxCount ="paxCount";
    QString Shift_Narration = "Narration";
    QString Shift_Start_DateTime = "ShiftStart_DateTime";
    QString Shift_End_DateTime = "ShiftEnd_DateTime";
    QString Shift_TotalCash="TotalCash";
    QString Shift_CasinCounter="CasinCounter";
    QString Shift_CashWithDrawn="CashWithDrawn";
    QString Shift_CashBalance="CashBalance";
    QString Shift_TillDifference="TillDifference";


    int startTransaction();
    int insertShiftData(ShiftDataModel *shiftData);
    void updateShiftData(ShiftDataModel *shiftDate,int shiftID);
    void deleteShift(int shiftID);

    QString getMaxID();

    int commit();


    QString getFirstBillinShift(ShiftDataModel *shift);

    QString getFirstBillByDate(ShiftDataModel *shift);


    QString getLastBill(ShiftDataModel *shift,QDateTime dateTime=QDateTime::currentDateTime());


    void truncateKotNumberTable();

    int getBillsCount(ShiftDataModel *shift);

    float getOpeningCash(ShiftDataModel *shift);

    float getSalesTotal(ShiftDataModel *shift);
    float getVatTotal(ShiftDataModel *shift);
    float getInputVatTotal(ShiftDataModel *shift);


    float getSalesByCash(ShiftDataModel *shift);
    float getSalesByCredit(ShiftDataModel *shift);
    float getSalesByCard(ShiftDataModel *shift);

    float getPurchaseTotal(ShiftDataModel *shift);
    float getPurchaseByCash(ShiftDataModel *shift);
    float getPurchaseByCredit(ShiftDataModel *shift);
    float getPaymentByCash(ShiftDataModel *shift);
    float getReceiptByCash(ShiftDataModel *shift);
    float getReceiptTotal(ShiftDataModel *shift);
    float getPaymentTotal(ShiftDataModel *shift, QString ledger = "%");

    float DayBookgetPurchaseTotal(ShiftDataModel *shift);
    float DayBookgetPurchaseByCash(ShiftDataModel *shift);
    float DayBookgetPurchaseByCredit(ShiftDataModel *shift);
    float DayBookgetPaymentByCash(ShiftDataModel *shift);
    float DayBookgetReceiptByCash(ShiftDataModel *shift);
    float DayBookgetReceiptTotal(ShiftDataModel *shift);
    float DayBookgetPaymentTotal(ShiftDataModel *shift);


    GeneralVoucherDataObject getPaymentDetails(ShiftDataModel *shift);

    GeneralVoucherDataObject getCreditPaymentDetails(ShiftDataModel *shift);
    GeneralVoucherDataObject getCreditSalesDetails(ShiftDataModel *shift);
    GeneralVoucherDataObject getBankAmountDetails(ShiftDataModel *shift);

    GeneralVoucherDataObject getCreditPurchaseDetails(ShiftDataModel *shift);
    GeneralVoucherDataObject getCreditReceiptDetails(ShiftDataModel *shift);


    QList<ShiftLedgersDataModel *> getSalesByLedgersList(ShiftDataModel *shift);





    float getZomatoSalesTotal(ShiftDataModel *shift);
    float getZomatoSalesByCash(ShiftDataModel *shift);
    float getZomatoSalesByCredit(ShiftDataModel *shift);

    float getTalabatSalesTotal(ShiftDataModel *shift);
    float getTalabatSalesByCash(ShiftDataModel *shift);
    float getTalabatSalesByCredit(ShiftDataModel *shift);

    float getCarriageSalesTotal(ShiftDataModel *shift);

    float getDeliverooSalesTotal(ShiftDataModel *shift);





    QList<ShiftDataModel> getAllShiftDetailsByDate(QDateTime dateFrom,QDateTime dateTo);

    ShiftDataModel getlastShiftDetailByVoucherPrefix(QString prefix);

    ShiftDataModel getShiftDetailByshiftID(int id);

    ShiftDataModel getlastClosingDetailByVoucherPrefix(QString vPref = LoginValues::voucherPrefix);

    void ClearSales();

    void updateShiftWithdrawAmt(int shiftid,float withdraw,float balance);


    QString getLastBillByDate(ShiftDataModel *shift);
    QDateTime getVoucherDateTimebyBill(QString vno);

    float getAdvancePaymentFromSalesOrders(QDateTime fromDate, QDateTime toDate,QString vpref=LoginValues::voucherPrefix);


    GeneralVoucherDataObject getPaymentDetailsByVDate(ShiftDataModel *shift);
    QSqlQueryModel *AllLedgerSummary(ShiftDataModel *shift, bool includeCash = true );
    QSqlQueryModel *GetExpensesByShift(ShiftDataModel *shift);
    QSqlQueryModel *getVouchersByLedger(ShiftDataModel *shift, QString ledgerID);
    QSqlQueryModel *getSalesSplitModel(ShiftDataModel *shift);
    float getSalesTotalByVoucherDate(ShiftDataModel *shift);
    float getSalesByCashByVoucherDate(ShiftDataModel *shift);
    float getSalesByCreditByVoucherDate(ShiftDataModel *shift);
    float getSalesByCardByVoucherDate(ShiftDataModel *shift);
    QSqlQueryModel *AllLedgerSummaryByVoucherDate(ShiftDataModel *shift, bool includeCash);

    float getTotalCashWithDrawn(ShiftDataModel *shift);
    float getTotalCashInCounter(ShiftDataModel *shift);
    float getOpeningCashByShift(ShiftDataModel *shift);
    QSqlQueryModel *getCreditSummaryByLedger(ShiftDataModel *shift, QString voucherType, bool includeCash = true);
    QSqlQueryModel *getDebitSummaryByLedger(ShiftDataModel *shift, QString voucherType, bool includeCash = true);
    float getSalesReturnByCash(ShiftDataModel *shift);
    float getPurchaseReturnByCash(ShiftDataModel *shift);
    float getTotalSalesReturn(ShiftDataModel *shift);
    float getTotalPurchaseReturn(ShiftDataModel *shift);

    QSqlQueryModel *getShiftReportModelByVoucherPrefix(QDateTime fromDateTime, QDateTime toDateTime);
    float getPaymentByCard(ShiftDataModel *shift);
    float getReceiptByCard(ShiftDataModel *shift);
    float getPurchaseByCard(ShiftDataModel *shift);
};

#endif // RESTAURANTSHIFTDATABASEHELPER_H
