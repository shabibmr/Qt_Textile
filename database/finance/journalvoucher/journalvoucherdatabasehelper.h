#ifndef JOURNALVOUCHERDATABASEHELPER_H
#define JOURNALVOUCHERDATABASEHELPER_H

#include "datamodels/transaction/generalvoucherdataobject.h"
#include "database/databasehelper/databasehelper.h"
#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"
#include "database/finance/TransactionHelper/transactionhelper.h"
#include "database/finance/TransactionHelper/generalvoucherdatabasehelper.h"
#include <QMap>
#include <QDebug>

#include "database/voucherdbabstract.h"

class JournalVoucherDatabaseHelper : public VoucherDBAbstract
{
public:
    JournalVoucherDatabaseHelper(QString connString=DatabaseValues::connectionString);
    ~JournalVoucherDatabaseHelper();

    TransactionHelper *ledTransaction;

    QString Journal_Voucher_Main_TableName = "Journal_Voucher_Main";

    QString Journal_Voucher_Main_id = "_id";
    QString Journal_Voucher_Main_Voucher_No = "Voucher_No";
    QString Journal_Voucher_Main_Voucher_Prefix = "Voucher_Prefix";
    QString Journal_Voucher_Main_Date_Created = "Date_Created";
    QString Journal_Voucher_Main_Voucher_Date = "Voucher_Date";
    QString Journal_Voucher_Main_TimeStamp = "TimeStamp";
    QString Journal_Voucher_Main_TimeStampC = "TimeStamp";

    QString Journal_Voucher_Main_Last_Updated = "Last_Updated";
    QString Journal_Voucher_Main_Ledger_ID = "Ledger_ID";
    QString Journal_Voucher_Main_Billing_Name = "Billing_Name";
    QString Journal_Voucher_Main_Total = "Total";
    QString Journal_Voucher_Main_Narration = "Narration";
    QString Journal_Voucher_Main_Added_By = "Added_By";
    QString Journal_Voucher_Main_Credit_Period = "Credit_Period";
    QString Journal_Voucher_Main_POC_Name = "POC_Name";
    QString Journal_Voucher_Main_POC_Phone = "POC_Phone";
    QString Journal_Voucher_Main_POC_Email = "POC_Email";
    QString Journal_Voucher_Main_Project_ID = "Project_ID";
    QString Journal_Voucher_Main_Revision_Number = "Revision_Number";
    QString Journal_Voucher_Main_Salesman_ID = "Salesman_ID";
    QString Journal_Voucher_Main_VoucherNoDisplay = "Voucher_No_Display";
    QString Journal_Voucher_Main_Currency = "Currency";
    QString Journal_Voucher_Main_Origin = "Origin";
    QString Journal_Voucher_Main_Status = "VoucherStatus";

    QString Journal_Voucher_Details_TableName = "Journal_Voucher_Details";

    QString Journal_Voucher_Details_id = "_id";
    QString Journal_Voucher_Details_Voucher_No = "Voucher_No";
    QString Journal_Voucher_Details_Voucher_Prefix = "Voucher_Prefix";
    QString Journal_Voucher_Details_Voucher_Date = "Voucher_Date";
    QString Journal_Voucher_Details_Ledger_ID = "Ledger_ID";
    QString Journal_Voucher_Details_CR_Amount = "CR_Amount";
    QString Journal_Voucher_Details_DR_Amount = "DR_Amount";
    QString Journal_Voucher_Details_Narration = "Narration";
    QString Journal_Voucher_Details_Salesman_ID = "Salesman_ID";

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

    QString VoucherType = GMVoucherTypes::getVoucherString(GMVoucherTypes::JournalVoucher); //"JOURNALVOUCHER";

    bool flag = true;
    DatabaseHelper *db;

    int startTransaction();

    int insertVoucher(GeneralVoucherDataObject *obj) override;

    int updateVoucher(GeneralVoucherDataObject *obj, QString oldID) override;

    int deleteVoucher(GeneralVoucherDataObject *obj) override;

    int commit();

    int getMaxID();

    GeneralVoucherDataObject *getVoucherByVoucherNoPtr(QString vNo, QString voucherPrefix) override;

    QList<GeneralVoucherDataObject> getVoucherListByDate(QDate dateFrom, QDate dateTo);
    QSqlQueryModel *getVoucherListByDateModel(QDate dateFrom, QDate dateTo) override;
    QString getNextVoucherNo(QString voucherPrefix) override;

    virtual QString getVoucherNumberByLedID(QString LedID,QDate Date) override;

    virtual QPair<QString, QString> getVoucherNoByTransactionId(QString transId) override;

    virtual QSqlQueryModel *getVoucherListByDateTimeModel(QDateTime dateFrom,QDateTime dateTo) override;

};

#endif // JOURNALVOUCHERDATABASEHELPER_H
