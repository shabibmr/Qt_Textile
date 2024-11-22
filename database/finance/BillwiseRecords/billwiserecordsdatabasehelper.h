#ifndef BILLWISERECORDSDATABASEHELPER_H
#define BILLWISERECORDSDATABASEHELPER_H

#include "database/databasehelper/databasehelper.h"
#include "datamodels/transaction/billwiserecorddatamodel.h"
#include "customwidgets/Models/gmsqlquerymodel.h"
#include "datamodels/transaction/generalvoucherdataobject.h"

class BillwiseRecordsDatabaseHelper
{
public:
    BillwiseRecordsDatabaseHelper();

    QString Billwise_Records_TableName = "Billwise_Records";
    QString Billwise_Records_id = "_id";
    QString Billwise_Records_VoucherPrefix = "VoucherPrefix";
    QString Billwise_Records_VoucherType = "VoucherType";
    QString Billwise_Records_VoucherNo = "VoucherNo";
    QString Billwise_Records_LedgerId = "LedgerId";
    QString Billwise_Records_RefPrefix = "RefPrefix";
    QString Billwise_Records_RefType = "RefType";
    QString Billwise_Records_RefNo = "RefNo";
    QString Billwise_Records_DebitAmount = "DebitAmount";
    QString Billwise_Records_CreditAmount = "CreditAmount";
    QString Billwise_Records_MethodOfAdj = "Method_Of_Adj";
    QString Billwise_Records_VoucherDate = "VoucherDate";
    QString Billwise_Records_DueDate = "Due_Date";

    DatabaseHelper* db;

    bool flag = true;
    int startTransaction();

    int commit();

    int insertBillwiseRecords(BillwiseRecordDataModel *obj);
    int updateBillwiseRecords(BillwiseRecordDataModel *obj,QString id);
    int deleteBillwiseRecordsByLedger(QString ledger);
    int deleteBillwiseRecordsByVoucher(QString voucherNo, QString voucherType, QString voucherPrefix);
    QString getMaxID();

    QList<BillwiseRecordDataModel*> getAllBillwiseRecords();
    QList<BillwiseRecordDataModel*> getAllBillwiseRecordsByLedger(QString ledger);
    QList<BillwiseRecordDataModel*> getPendingBillsByLedger(QString ledger, QDate date);
    QSqlQueryModel *getPendingBillsModelByLedger(QString ledger, GeneralVoucherDataObject *voucher);
    QList<BillwiseRecordDataModel*> getOpeningBalanceMapping(QString ledger);
    QList<BillwiseRecordDataModel*> getBillMappingsByLedger(QString ledgerId, QString voucherNo, QDate date);

    QSqlQueryModel *getBillwiseMappingByVoucher(QString voucherNo, QString voucherType, QString voucherPrefix);
};

#endif // BILLWISERECORDSDATABASEHELPER_H
