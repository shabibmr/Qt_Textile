#ifndef CHEQUEDETAILSDATABASEHELPER_H
#define CHEQUEDETAILSDATABASEHELPER_H

#include "datamodels/accounts/chequedetail.h"
#include "database/databasehelper/databasehelper.h"
#include "QVariant"
#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"

class ChequeDetailsDatabaseHelper
{
public:
    ChequeDetailsDatabaseHelper();

    QString Cheque_Details_TableName = "Cheque_Details";
    QString Cheque_Details_id = "_id";
    QString Cheque_Details_LedgerId = "Ledger_ID";
    QString Cheque_Details_VoucherNo = "VoucherNo";
    QString Cheque_Details_VoucherType = "VoucherType";
    QString Cheque_Details_VoucherPrefix = "VoucherPrefix";
    QString Cheque_Details_IssuedOn = "Issued_On";
    QString Cheque_Details_Amount = "Amount";
    QString Cheque_Details_DebitAmount = "Debit_Amount";
    QString Cheque_Details_CreditAmount = "Credit_Amount";
    QString Cheque_Details_BankName = "Bank_Name";
    QString Cheque_Details_BranchName = "Branch_Name";
    QString Cheque_Details_ChequeNo = "Instrument_No";
    QString Cheque_Details_ChequeDate = "Instrument_Date";
    QString Cheque_Details_IsPresented = "is_Presented";
    QString Cheque_Details_PresentedOn = "Presented_On";
    QString Cheque_Details_IsCleared = "is_Cleared";
    QString Cheque_Details_ClearedOn = "Cleared_On";
    QString Cheque_Details_IsReturned = "is_Returned";
    QString Cheque_Details_ReconciledOn = "Reconciled_On";
    QString Cheque_Details_Narration = "Narration";
    QString Cheque_Details_OurBank = "Our_Bank_Id";

    DatabaseHelper* db;

    bool flag = true;
    int startTransaction();

    int commit();

    int insertChequeDetails(ChequeDetail *obj);
    int updateChequeDetails(ChequeDetail *obj,QString id);
    int deleteChequeDetails(QString id);
    QList<ChequeDetail> getAllChequeDetails();
    ChequeDetail *getChequeById(QString id);
    QList<ChequeDetail> getAllChequesByDate(QDate date);
    QList<ChequeDetail> getPendingChequesByLedgerId(QString ledgerID, QDate date);
    QSqlQueryModel *getReconciliationReport(QDate date);
    QString getMaxID();


    int deleteChequeDetailsByVoucher(ChequeDetail *obj);
};

#endif // CHEQUEDETAILSDATABASEHELPER_H
