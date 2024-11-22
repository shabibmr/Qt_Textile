#ifndef PAYMENTVOUCHERDATABASEHELPER_H
#define PAYMENTVOUCHERDATABASEHELPER_H

#include "datamodels/transaction/generalvoucherdataobject.h"
#include "database/databasehelper/databasehelper.h"
#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"
#include "database/finance/TransactionHelper/transactionhelper.h"
#include "database/finance/TransactionHelper/generalvoucherdatabasehelper.h"

#include "database/voucherdbabstract.h"

#include <QMap>
#include <QDebug>

class PaymentVoucherDatabaseHelper: public VoucherDBAbstract
{
public:
    PaymentVoucherDatabaseHelper(QString connString=DatabaseValues::connectionString);
    ~PaymentVoucherDatabaseHelper();
    QString Payment_Voucher_Main_TableName = "Payment_Voucher_Main";

    QString Payment_Voucher_Main_id = "_id";
    QString Payment_Voucher_Main_Voucher_No = "Voucher_No";
    QString Payment_Voucher_Main_Voucher_Prefix = "Voucher_Prefix";
    QString Payment_Voucher_Main_Date_Created = "Date_Created";
    QString Payment_Voucher_Main_Voucher_Date = "Voucher_Date";
    QString Payment_Voucher_Main_TimeStamp = "TimeStamp";
    QString Payment_Voucher_Main_TimeStampC = "TimeStamp";
    QString Payment_Voucher_Main_Last_Updated = "Last_Updated";
    QString Payment_Voucher_Main_Ledger_ID = "Ledger_ID";
    QString Payment_Voucher_Main_Billing_Name = "Billing_Name";
    QString Payment_Voucher_Main_Total = "Total";
    QString Payment_Voucher_Main_Narration = "Narration";
    QString Payment_Voucher_Main_Added_By = "Added_By";
    QString Payment_Voucher_Main_Delivery_Date = "Delivery_Date";
    QString Payment_Voucher_Main_Completion_Probability = "Completion_Probability";
    QString Payment_Voucher_Main_Credit_Period = "Credit_Period";
    QString Payment_Voucher_Main_POC_Name = "POC_Name";
    QString Payment_Voucher_Main_POC_Phone = "POC_Phone";
    QString Payment_Voucher_Main_POC_Email = "POC_Email";
    QString Payment_Voucher_Main_Project_ID = "Project_ID";
    QString Payment_Voucher_Main_Revision_Number = "Revision_Number";
    QString Payment_Voucher_Main_Converted_To_Sales_Order = "Converted_To_Sales_Order";
    QString Payment_Voucher_Main_Quotation_Dropped = "Quotation_Dropped";
    QString Payment_Voucher_Main_Quotation_Dropped_Reason = "Quotation_Dropped_Reason";
    QString Payment_Voucher_Main_Requirement_Voucher_No = "Requirement_Voucher_No";
    QString Payment_Voucher_Main_Delivery_By = "Delivery_By";
    QString Payment_Voucher_Main_Salesman_ID = "Salesman_ID";
    QString Payment_Voucher_Main_LPO = "LPO_Ref";
    QString Payment_Voucher_Main_ModeOfService = "MODE_OF_SERVICE";
    QString Payment_Voucher_Main_Discount_In_Amount = "Discount_In_Amount";
    QString Payment_Voucher_Main_Discount_In_Percent = "Discount_In_Percent";
    QString Payment_Voucher_Main_VATAmount = "VAT_Amount";
    QString Payment_Voucher_Main_VoucherStatus = "VoucherStatus";
    QString Payment_Voucher_Main_Contact_ID = "ContactID";
    QString Payment_Voucher_Main_Gross_Total = "Gross_Total";
    QString Payment_Voucher_Main_Reference ="Reference";
    QString Payment_Voucher_Main_Customer_Expecting_Date = "Customer_Expected_Date";
    QString Payment_Voucher_Main_Currency = "Currency";
    QString Payment_Voucher_Main_BoxesCount = "Boxes_Count";
    QString Payment_Voucher_Main_TotalWeight = "Total_Weight";
    QString Payment_Voucher_Main_LRNo = "LR_No";
    QString Payment_Voucher_Main_VoucherNoDisplay = "Voucher_No_Display";
    QString Payment_Voucher_Main_ReqVoucherList = "Requirement_Voucher_List";
    QString Payment_Voucher_Main_Origin = "Origin";
    QString Payment_Voucher_Main_Round_Off = "Round_Off";
    QString Payment_Voucher_Main_GeneratedTransactionId = "TransactionId";

    QString Payment_Voucher_Details_TableName = "Payment_Voucher_Details";

    QString Payment_Voucher_Details_id = "_id";
    QString Payment_Voucher_Details_Voucher_No = "Voucher_No";
    QString Payment_Voucher_Details_Voucher_Prefix = "Voucher_Prefix";
    QString Payment_Voucher_Details_Voucher_Date = "Voucher_Date";
    QString Payment_Voucher_Details_Ledger_ID = "Ledger_ID";
    QString Payment_Voucher_Details_CrAmount = "Cr_Amount";
    QString Payment_Voucher_Details_DrAmount = "Dr_Amount";
    QString Payment_Voucher_Details_Project_ID = "Project_ID";
    QString Payment_Voucher_Details_Narration = "Narration";

    QString Payment_Voucher_Details_Salesman_ID = "Salesman_ID";

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

    QString VoucherType = GMVoucherTypes::getVoucherString(GMVoucherTypes::PaymentVoucher); // "PAYMENTVOUCHER";

    bool flag = true;
    DatabaseHelper *db;
    TransactionHelper *ledTransaction;


    int startTransaction();

    int insertVoucher(GeneralVoucherDataObject *obj) override;

    int updateVoucher(GeneralVoucherDataObject *obj, QString oldID) override;

    int deleteVoucher(GeneralVoucherDataObject *obj) override;

    int commit();

    int getMaxID();


    GeneralVoucherDataObject *getVoucherByVoucherNoPtr(QString vNo, QString voucherPrefix) override;


    QList<GeneralVoucherDataObject> getVoucherListByDate(QDate dateFrom, QDate dateTo) ;
    QString getNextVoucherNo(QString voucherPrefix) override;
    QSqlQueryModel *getVoucherListByDateModel(QDateTime dateFrom, QDateTime dateTo);
    QPair<QString, QString> getVoucherNoByTransactionId(QString transId);

    QSqlQueryModel* getVoucherListByDateTimeModel(QDateTime dateFrom, QDateTime dateTo) override;
    QSqlQueryModel *getVoucherListByDateModel(QDate dateFrom, QDate dateTo) override;
    QString getVoucherNumberByLedID(QString LedID,QDate Date) override;

    QSqlQueryModel *getDetailsList(QDate dateFrom, QDate dateTo);

};

#endif // PAYMENTVOUCHERDATABASEHELPER_H
