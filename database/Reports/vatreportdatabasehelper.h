#ifndef VATREPORTDATABASEHELPER_H
#define VATREPORTDATABASEHELPER_H

#include <QObject>

#include "database/finance/salesvoucher/salesvoucherdatabasehelper.h"
#include "database/finance/purchasevoucher/purchasevoucherdatabasehelper.h"

#include "datamodels/transaction/generalvoucherdataobject.h"
#include "database/finance/TransactionHelper/transactionhelper.h"

#include <QSqlQueryModel>


class VatReportDatabaseHelper : public QObject
{
    Q_OBJECT
public:
    explicit VatReportDatabaseHelper(QObject *parent = nullptr);

    DatabaseHelper *db;



    QSqlQueryModel* getSalesVatReport(QDate fromDate, QDate toDate);

    QSqlQueryModel* getPurchaseVatReport(QDate fromDate, QDate toDate);

    QSqlQueryModel* getSalesReturnVatReport(QDate fromDate, QDate toDate);

    QSqlQueryModel* getPurchaseReturnVatReport(QDate fromDate, QDate toDate);

    float getPendingPayableReceivable(QDate fromDate, QDate toDate);

    float getSalesTotal(QString billsFrom,QString billsTo);
    float outputVatTotal(QString billsFrom,QString billsTo);
    float getSales5Total(QString billsFrom,QString billsTo);

    float getPurchaseTotal(QDate fromDate,QDate toDate);
    float inputVatTotal(QDate fromDate,QDate toDate);
    float getPurchase5Total(QDate fromDate,QDate toDate);





    float getCFBF(QDate date);
    float getAmountByLedger(QString ledID, QDate fromDate, QDate toDate, QString voucherType);

//    float getAmountByLedgerAndHSN(QString ledID, QString billsFrom, QString billsTo);
    QSqlQueryModel *getTaxReportModelByHSN(QDate fromDate, QDate toDate);
    QSqlQueryModel *getCessReportModel(QString billsFrom, QString billsTo);

    QSqlQueryModel *getB2BOutputModel(QDate fromDate, QDate toDate);

signals:

public slots:
};

#endif // VATREPORTDATABASEHELPER_H
