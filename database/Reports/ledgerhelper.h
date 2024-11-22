#ifndef LEDGERHELPER_H
#define LEDGERHELPER_H

#include "database/finance/salesvoucher/salesvoucherdatabasehelper.h"
#include "database/finance/salesvoucher/salesvoucherdeleteddatabasehelper.h"
#include "database/finance/purchasevoucher/purchasevoucherdatabasehelper.h"
#include "database/finance/creditnote/creditnotevoucherdatabasehelper.h"
#include "database/finance/debitnote/debitnotevoucherdatabasehelper.h"
#include "datamodels/transaction/generalvoucherdataobject.h"
#include "database/finance/TransactionHelper/transactionhelper.h"
#include "database/hr/userprofiledatabasehelper.h"
#include "datamodels/users/userprofiledataobject.h"

#include <QSqlQueryModel>



class LedgerHelper
{
public:
    LedgerHelper();
    DatabaseHelper *db;

    LedgerMasterDatabaseHelper *ledHelper;
    AccountGroupMasterDatabaseHelper *accHelper;

    QString Transaction_Helper_TableName = "Transaction_Helper";
    QString Transaction_Helper_id = "_id";
    QString Transaction_Helper_VoucherType = "Voucher_Type";
    QString Transaction_Helper_VoucherPrefix = "Voucher_Prefix";
    QString Transaction_Helper_VoucherNo = "Voucher_ID";
    QString Transaction_Helper_VoucherDate = "Voucher_Date";
    QString Transaction_Helper_ProjectID = "Project_ID";
    QString Transaction_Helper_TranactionAmount = "Transaction_Amount";
    QString Transaction_Helper_CreditLedgerID = "Credit_Ledger_ID";
    QString Transaction_Helper_DebitLedgerID = "Debit_Ledger_ID";
    QString Transaction_Helper_Narration = "Narration";
    QString Transaction_Helper_SalesManID = "Salesman_ID";
    QString Transaction_Helper_TimeStamp = "TIMESTAMP";
    QString Transaction_Helper_isAdditional ="isAdditional";
    QString Transaction_Helper_LedgerID ="Ledger_ID";
    QString Transaction_Helper_CrAmount ="Credit_Amount";
    QString Transaction_Helper_DrAmount ="Debit_Amount";


    QString getDateTimeString(QDateTime dateTime)
    {
        return "str_to_date('" + dateTime.toString("yyyy-MM-dd hh:mm:ss") + "','%Y-%m-%d %H:%i:%s')";
    }


    float getClosingBalanceOfLedger(QString ledGerID,QDate date);

    float getOpeningBalanceOfLedger(QString ledGerID,QDate date);





    QSqlQueryModel* getLedgerReportModel(QString ledgerID, QDate fromDate, QDate toDate, int limit=0);


    QSqlQueryModel *getDayBookReportModel(QDate fromDate,QDate toDate);

    QSqlQueryModel *getSalesReportModelByDate(QDateTime fromDate,QDateTime toDate);

    QSqlQueryModel *getPurchaseReportModelByDate(QDateTime fromDate,QDateTime toDate);

    QSqlQueryModel *getComplimentarySales(QDateTime fromDate,QDateTime toDate);

    QSqlQueryModel *getSalesManwiseReport(QDateTime dateFrom,QDateTime dateTo);

    QSqlQueryModel *getSalesManwiseReportByService(QDateTime dateFrom,QDateTime dateTo, int service);

    QSqlQueryModel *getDiscountedSales(QDateTime fromDate,QDateTime toDate);

    QSqlQueryModel *getTimeBasedSales(QDateTime fromDate,QDateTime toDate);


    QSqlQueryModel* getServiceBasedSalesModel(QDateTime fromDate,QDateTime toDate);

    QSqlQueryModel *getShiftModel(QDateTime fromDate,QDateTime toDate);

    QSqlQueryModel *getItemwiseMovementByLedger(QString ledgerID, QDate fromDate, QDate toDate);
    QSqlQueryModel *getItemGroupMovementByLedger(QString ledgerID, QDate fromDate, QDate toDate);
    QSqlQueryModel *getItemwiseMovementByAccGroup(QString groupID, QDate fromDate, QDate toDate);
    QSqlQueryModel *getItemGroupMovementByAccountGroup(QString groupID, QDate fromDate, QDate toDate);
    QSqlQueryModel *getOutstandingBalanceByGroup(QString groupID, QDate fromDate, QDate toDate);

    QSqlQueryModel *getDatewiseSales(QDateTime fromDate, QDateTime toDate);

    QSqlQueryModel *getDiscountedSalesForMail(QDateTime fromDate, QDateTime toDate);

    QSqlQueryModel *getLedgerwiseTransactionSummaryByVoucherType(QDateTime fromDate, QDateTime toDate, int voucherType, int limit = 999999);
    QSqlQueryModel *getDebitNoteReportModelByDate(QDateTime fromDate, QDateTime toDate);
    QSqlQueryModel *getCreditNoteReportModelByDate(QDateTime fromDate, QDateTime toDate);
    QSqlQueryModel *getSalesReportWithTaxModelByDate(QDateTime fromDate, QDateTime toDate);
    QSqlQueryModel *getPurchaseReportWithTaxModelByDate(QDateTime fromDate, QDateTime toDate);
    QSqlQueryModel *getContactwiseLedgerReportModel(QString phone, QDate fromDate, QDate toDate);
    float getOpeningBalanceOfContact(QString phone, QDate date);
    QSqlQueryModel *getVisitorsList(QDateTime fromDate, QDateTime toDate);

    QSqlQueryModel *getLedgerTotalsModel(QString ledgerID, QDate fromDate, QDate toDate);
    QSqlQueryModel *getTopCustomers(QDate fromDate, QDate toDate, int limit=5);
    QSqlQueryModel *getTopVendors(QDate fromDate, QDate toDate, int limit=5);
    QSqlQueryModel *getAverageCreditPeriodByCustomer(QString LedgerId, QDate fromDate, QDate toDate);
    QSqlQueryModel *getLedgerTransValueByFrequency(int voucherType, QDate fromDate, QDate toDate, QString LedgerId="%");
    QSqlQueryModel *getTopItemsByCustomer(QString ledgerID, QDate fromDate, QDate toDate, int limit =10);
    QSqlQueryModel *getTopItemsByVendor(QString ledgerID, QDate fromDate, QDate toDate, int limit =10);
    QSqlQueryModel *getOverDueBillsByLedger(QString LedgerId, QDate fromDate, QDate toDate, int limit =10);
    QSqlQueryModel *getCreditInfoByLedger(QString LedgerId, QDate fromDate, QDate toDate);
    QSqlQueryModel *getRankBySalesValue(  QString ledgerId, QDate fromDate, QDate toDate);
    QSqlQueryModel *getOverDueBillsByGroup(QString groupID, QDate fromDate, QDate toDate);
    int getCustomerCount(QDate fromDate, QDate toDate);
    QString getTotalOutstandingBalance(QDate fromDate, QDate toDate);
    QSqlQueryModel *getTopDebtors(QDate fromDate, QDate toDate, int limit = 5);
    QSqlQueryModel *getDeletedSalesReportModelByDate(QDateTime fromDate, QDateTime toDate);
    QSqlQueryModel *getB2BSalesReportWithTaxModelByDate(QDateTime fromDate, QDateTime toDate);
    QSqlQueryModel *getB2CSalesReportWithTaxModelByDate(QDateTime fromDate, QDateTime toDate);
    QSqlQueryModel *getAccGroupSummaryModel(QString groupID, QDateTime fromDate, QDateTime toDate);
    QSqlQueryModel *getLedgerwiseOutstandingModel(QString ledgerID, QDate toDate);

    float getTotalPurchaseValue(QDate fromDate, QDate toDate);
   QSqlQueryModel *getPendingVouchersByVoucherType(int Reftype, QDate fromDate, QDate toDate, int limit=5);
   float  getTotalSalesValue(QDate fromDate, QDate toDate);

   QSqlQueryModel *getDatewisePurchase(QDateTime fromDate, QDateTime toDate);
   QSqlQueryModel *getSalesManwiseServicewiseReport(QDateTime dateFrom, QDateTime dateTo);

};

#endif // LEDGERHELPER_H
