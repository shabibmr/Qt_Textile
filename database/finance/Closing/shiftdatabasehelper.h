#ifndef SHIFTDATABASEHELPER_H
#define SHIFTDATABASEHELPER_H

#include <QSqlDatabase>
#include <QSqlQuery>

#include "datamodels/Counter/shiftdatamodel.h"
#include "database/finance/salesvoucher/salesvoucherdatabasehelper.h"
#include "database/finance/purchasevoucher/purchasevoucherdatabasehelper.h"
#include "database/finance/paymentvoucher/paymentvoucherdatabasehelper.h"
#include "database/finance/receiptvoucher/receiptvoucherdatabasehelper.h"
#include "database/finance/TransactionHelper/transactionhelper.h"


class ShiftDatabaseHelper
{
public:
    ShiftDatabaseHelper();


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
    QString Shift_ZomatoSales = "ZomatoSales";
    QString Shift_ZomatoCash = "ZomatoCash";
    QString Shift_ZomatoCredit = "ZomatoCredit";
    QString Shift_PurchaseCash = "PurchaseCash";
    QString Shift_PurchaseCredit = "PurchaseCredit";
    QString Shift_PurchaseTotal = "PurchaseTotal";
    QString Shift_PaymentCash="PaymentCash";
    QString Shift_ReceiptCash = "ReceiptCash";
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

    QDateTime getVoucherDateTimebyBill(QString vno);

    QString getFirstBillinShift(ShiftDataModel *shift);

    QString getFirstBillByDate(ShiftDataModel *shift);

    QString getLastBill(ShiftDataModel *shift);

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
    float getPaymentTotal(ShiftDataModel *shift);

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
    GeneralVoucherDataObject getCreditPurchaseDetails(ShiftDataModel *shift);
    GeneralVoucherDataObject getCreditReceiptDetails(ShiftDataModel *shift);




    float getZomatoSalesTotal(ShiftDataModel *shift);



    float getZomatoSalesByCash(ShiftDataModel *shift);
    float getZomatoSalesByCredit(ShiftDataModel *shift);

    QList<ShiftDataModel> getAllShiftDetailsByDate(QDateTime dateFrom,QDateTime dateTo);

    ShiftDataModel getlastShiftDetailByVoucherPrefix(QString prefix);

    ShiftDataModel getlastClosingDetailByVoucherPrefix();


    QString getLastBillByDate(ShiftDataModel *shift);
    QString getLastPurchaseBillByDate(ShiftDataModel *shift);
    QString getFirstPurchaseBillByDate(ShiftDataModel *shift);
};

#endif // SHIFTDATABASEHELPER_H
