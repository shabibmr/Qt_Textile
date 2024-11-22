#ifndef TRANSACTIONHELPER_H
#define TRANSACTIONHELPER_H

#include "database/databasehelper/databasehelper.h"
#include "datamodels/transaction/generalvoucherdataobject.h"
#include "datamodels/transaction/billwiserecorddatamodel.h"
#include "database/finance/BillwiseRecords/billwiserecordsdatabasehelper.h"
#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"
#include "database/finance/banking/chequedetailsdatabasehelper.h"

class TransactionHelper
{
public:
    TransactionHelper(DatabaseHelper* db = new DatabaseHelper);

    DatabaseHelper *db;
    bool flag = true;

    QString Transaction_Helper_TableName = "Transaction_Helper";
    QString Proxy_Transaction_Helper_TableName = "Proxy_Transaction_Helper";
    QString Transaction_Helper_id = "_id";
    QString Transaction_Helper_VoucherType = "Voucher_Type";
    QString Transaction_Helper_VoucherPrefix = "Voucher_Prefix";
    QString Transaction_Helper_VoucherNo = "Voucher_ID";
    QString Transaction_Helper_VoucherDate = "Voucher_Date";
    QString Transaction_Helper_ProjectID = "Project_ID";
    QString Transaction_Helper_TransactionAmount = "Transaction_Amount";
    QString Transaction_Helper_CreditLedgerID = "Credit_Ledger_ID";
    QString Transaction_Helper_DebitLedgerID = "Debit_Ledger_ID";
    QString Transaction_Helper_Debit_Amount = "Debit_Amount";
    QString Transaction_Helper_Credit_Amount = "Credit_Amount";
    QString Transaction_Helper_LedgerID = "Ledger_ID";
    QString Transaction_Helper_Narration = "Narration";
    QString Transaction_Helper_SalesManID = "Salesman_ID";
    QString Transaction_Helper_TimeStamp = "TIMESTAMP";
    QString Transaction_Helper_AgainstLedger = "Against_Ledger";
    QString Transaction_Helper_isAdditional = "isAdditional";

    int startTransaction();

    int insertTransaction(GeneralVoucherDataObject *voucher);

    int updateTransaction(GeneralVoucherDataObject *voucher,QString oldID);

    int deleteTransaction(GeneralVoucherDataObject *obj);

    int commit();
    int insertProxyTransaction(GeneralVoucherDataObject *voucher);
    int deleteProxyTransaction(GeneralVoucherDataObject *obj);
};

#endif // TRANSACTIONHELPER_H
