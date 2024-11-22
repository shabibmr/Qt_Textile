#ifndef CONTRAVOUCHERDATABASEHELPER_H
#define CONTRAVOUCHERDATABASEHELPER_H

#include "datamodels/transaction/generalvoucherdataobject.h"
#include "database/databasehelper/databasehelper.h"
#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"
#include "database/finance/TransactionHelper/transactionhelper.h"
#include "database/finance/TransactionHelper/generalvoucherdatabasehelper.h"

#include "database/voucherdbabstract.h"


class ContraVoucherDatabaseHelper : public  VoucherDBAbstract
{
public:
    ContraVoucherDatabaseHelper(QString connString=DatabaseValues::connectionString);
    ~ContraVoucherDatabaseHelper();

    QString Contra_Voucher_Main_TableName = "Contra_Voucher_Main";

    QString Contra_Voucher_Main_id = "_id";
    QString Contra_Voucher_Main_Voucher_No = "Voucher_No";
    QString Contra_Voucher_Main_Voucher_Prefix = "Voucher_Prefix";
    QString Contra_Voucher_Main_Date_Created = "Date_Created";
    QString Contra_Voucher_Main_Voucher_Date = "Voucher_Date";
    QString Contra_Voucher_Main_TimeStamp = "`TimeStamp`";
    QString Contra_Voucher_Main_TimeStampC = "TimeStamp";

    QString Contra_Voucher_Main_Last_Updated = "Last_Updated";
    QString Contra_Voucher_Main_Ledger_ID = "Ledger_ID";
    QString Contra_Voucher_Main_Billing_Name = "Billing_Name";
    QString Contra_Voucher_Main_Total = "Total";
    QString Contra_Voucher_Main_Narration = "Narration";
    QString Contra_Voucher_Main_Added_By = "Added_By";
    QString Contra_Voucher_Main_Delivery_Date = "Delivery_Date";
    QString Contra_Voucher_Main_Completion_Probability = "Completion_Probability";
    QString Contra_Voucher_Main_Credit_Period = "Credit_Period";
    QString Contra_Voucher_Main_POC_Name = "POC_Name";
    QString Contra_Voucher_Main_POC_Phone = "POC_Phone";
    QString Contra_Voucher_Main_POC_Email = "POC_Email";
    QString Contra_Voucher_Main_Project_ID = "Project_ID";
    QString Contra_Voucher_Main_Revision_Number = "Revision_Number";
    QString Contra_Voucher_Main_Converted_To_Sales_Order = "Converted_To_Sales_Order";
    QString Contra_Voucher_Main_Quotation_Dropped = "Quotation_Dropped";
    QString Contra_Voucher_Main_Quotation_Dropped_Reason = "Quotation_Dropped_Reason";
    QString Contra_Voucher_Main_Requirement_Voucher_No = "Requirement_Voucher_No";
    QString Contra_Voucher_Main_Salesman_ID = "Salesman_ID";
    QString Contra_Voucher_Main_LPORef="LPO_Ref";
    QString Contra_Voucher_Main_ModeOfService = "MODE_OF_SERVICE";
    QString Contra_Voucher_Main_Discount_In_Amount = "Discount_In_Amount";
    QString Contra_Voucher_Main_Discount_In_Percent = "Discount_In_Percent";
    QString Contra_Voucher_Main_VATAmount = "VAT_Amount";
    QString Contra_Voucher_Main_VoucherStatus = "VoucherStatus";
    QString Contra_Voucher_Main_Contact_ID = "ContactID";
    QString Contra_Voucher_Main_Gross_Total = "Gross_Total";
    QString Contra_Voucher_Main_Reference ="Reference";
    QString Contra_Voucher_Main_Customer_Expecting_Date = "Customer_Expected_Date";
    QString Contra_Voucher_Main_Currency = "Currency";
    QString Contra_Voucher_Main_BoxesCount = "Boxes_Count";
    QString Contra_Voucher_Main_TotalWeight = "Total_Weight";
    QString Contra_Voucher_Main_LRNo = "LR_No";
    QString Contra_Voucher_Main_VoucherNoDisplay = "Voucher_No_Display";
    QString Contra_Voucher_Main_ReqVoucherList = "Requirement_Voucher_List";
    QString Contra_Voucher_Main_Origin = "Origin";
    QString Contra_Voucher_Main_Round_Off = "Round_Off";

    QString Contra_Voucher_Details_TableName = "Contra_Voucher_Details";

    QString Contra_Voucher_Details_id = "_id";
    QString Contra_Voucher_Details_Voucher_No = "Voucher_No";
    QString Contra_Voucher_Details_Voucher_Prefix = "Voucher_Prefix";
    QString Contra_Voucher_Details_Voucher_Date = "Voucher_Date";
    QString Contra_Voucher_Details_Ledger_ID = "Ledger_ID";
    QString Contra_Voucher_Details_CrAmount = "Cr_Amount";
    QString Contra_Voucher_Details_DrAmount = "Dr_Amount";
    QString Contra_Voucher_Details_Project_ID = "Project_ID";
    QString Contra_Voucher_Details_Narration = "Narration";
    QString Contra_Voucher_Details_Salesman_ID = "Salesman_ID";

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

    QString VoucherType = GMVoucherTypes::getVoucherString(GMVoucherTypes::ContraVoucher); // "CONTRAVOUCHER";

    bool flag = true;
    DatabaseHelper *db;
    TransactionHelper *ledTransaction;


    int startTransaction();

    int insertVoucher(GeneralVoucherDataObject *obj) override;

    int updateVoucher(GeneralVoucherDataObject *obj, QString oldID)override;

    int deleteVoucher(GeneralVoucherDataObject *obj) override;

    int commit();

    int getMaxID();

    GeneralVoucherDataObject getVoucherByVoucherNo(QString vNo);

    GeneralVoucherDataObject *getVoucherByVoucherNoPtr(QString vNo, QString voucherPrefix) override;


    QList<GeneralVoucherDataObject> getVoucherListByDate(QDate dateFrom, QDate dateTo);
    QString getNextVoucherNo(QString voucherPrefix) override;
    QSqlQueryModel *getVoucherListByDateModel(QDate dateFrom, QDate dateTo) override;

    QString getVoucherNumberByLedID(QString LedID,QDate Date) override;

    QPair<QString, QString> getVoucherNoByTransactionId(QString transId) override;

    QSqlQueryModel *getVoucherListByDateTimeModel(QDateTime dateFrom,QDateTime dateTo) override;

};

#endif // CONTRAVOUCHERDATABASEHELPER_H
