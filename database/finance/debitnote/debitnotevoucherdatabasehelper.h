#ifndef DEBITNOTEVOUCHERDATABASEHELPER_H
#define DEBITNOTEVOUCHERDATABASEHELPER_H


#include "database/voucherdbabstract.h"

class DebitNoteVoucherDatabaseHelper : public VoucherDBAbstract
{
public:
    DebitNoteVoucherDatabaseHelper(QString connString=DatabaseValues::connectionString);
    ~DebitNoteVoucherDatabaseHelper();
    static QString Debit_Note_Main_TableName ;
    QString Debit_Note_Main_id = "_id";
    QString Debit_Note_Main_Voucher_No = "Voucher_No";
    QString Debit_Note_Main_Voucher_Prefix = "Voucher_Prefix";
    QString Debit_Note_Main_Date_Created = "Date_Created";
    QString Debit_Note_Main_Voucher_Date = "Voucher_Date";
    QString Debit_Note_Main_TimeStamp = "TimeStamp";
    QString Debit_Note_Main_TimeStampC = "TimeStamp";
    QString Debit_Note_Main_Last_Updated = "Last_Updated";
    QString Debit_Note_Main_Ledger_ID = "Ledger_ID";
    QString Debit_Note_Main_Billing_Name = "Billing_Name";
    QString Debit_Note_Main_Total = "Total";
    QString Debit_Note_Main_Narration = "Narration";
    QString Debit_Note_Main_Added_By = "Added_By";
    QString Debit_Note_Main_Delivery_Date = "Delivery_Date";
    QString Debit_Note_Main_Completion_Probability = "Completion_Probability";
    QString Debit_Note_Main_Credit_Period = "Credit_Period";
    QString Debit_Note_Main_POC_Name = "POC_Name";
    QString Debit_Note_Main_POC_Phone = "POC_Phone";
    QString Debit_Note_Main_POC_Email = "POC_Email";
    QString Debit_Note_Main_Project_ID = "Project_ID";
    QString Debit_Note_Main_Revision_Number = "Revision_Number";
    QString Debit_Note_Main_Converted_To_Sales_Order = "Converted_To_Sales_Order";
    QString Debit_Note_Main_Quotation_Dropped = "Quotation_Dropped";
    QString Debit_Note_Main_Quotation_Dropped_Reason = "Quotation_Dropped_Reason";
    QString Debit_Note_Main_Requirement_Voucher_No = "Requirement_Voucher_No";
    QString Debit_Note_Main_Delivery_By = "Delivery_By";
    QString Debit_Note_Main_Salesman_ID = "Salesman_ID";
    QString Debit_Note_Main_LPORef="LPO_Ref";
    QString Debit_Note_Main_ModeOfService = "MODE_OF_SERVICE";
    QString Debit_Note_Main_Discount_In_Amount = "Discount_In_Amount";
    QString Debit_Note_Main_Discount_In_Percent = "Discount_In_Percent";
    QString Debit_Note_Main_VATAmount = "VAT_Amount";
    QString Debit_Note_Main_VoucherStatus = "VoucherStatus";
    QString Debit_Note_Main_Contact_ID = "ContactID";
    QString Debit_Note_Main_Gross_Total = "Gross_Total";
    QString Debit_Note_Main_Reference ="Reference";
    QString Debit_Note_Main_Customer_Expecting_Date = "Customer_Expected_Date";
    QString Debit_Note_Main_Currency = "Currency";
    QString Debit_Note_Main_BoxesCount = "Boxes_Count";
    QString Debit_Note_Main_TotalWeight = "Total_Weight";
    QString Debit_Note_Main_LRNo = "LR_No";
    QString Debit_Note_Main_VoucherNoDisplay = "Voucher_No_Display";
    QString Debit_Note_Main_ReqVoucherList = "Requirement_Voucher_List";
    QString Debit_Note_Main_Origin = "Origin";
    QString Debit_Note_Main_Round_Off = "Round_Off";
    QString Debit_Note_Main_Generated_TransactionId = "TransactionId";
    QString Debit_Note_Main_Godown = "PaymentCompleted";
    QString Debit_Note_Details_TableName = "Debit_Note_Details";

    QString Debit_Note_Details_id = "_id";
    QString Debit_Note_Details_Voucher_No = "Voucher_No";
    QString Debit_Note_Details_Voucher_Prefix = "Voucher_Prefix";
    QString Debit_Note_Details_Voucher_Date = "Voucher_Date";
    QString Debit_Note_Details_Inventory_Item_ID = "Inventory_Item_ID";
    QString Debit_Note_Details_Quantity = "Quantity";
    QString Debit_Note_Details_Price = "Price";
    QString Debit_Note_Details_PriceLevel = "PriceLevel";
    QString Debit_Note_Details_isCompoundItem = "isCompoundItem";
    QString Debit_Note_Details_subItemOff_list_ID = "subItemOff_list_ID";
    QString Debit_Note_Details_Discount_Percent = "Discount_Percent";
    QString Debit_Note_Details_Discount_Amount = "Discount_Amount";
    QString Debit_Note_Details_Subtotal = "Subtotal";
    QString Debit_Note_Details_Project_ID = "Project_ID";
    QString Debit_Note_Details_Item_Narration = "Item_Narration";
    QString Debit_Note_Details_Item_Description = "Item_Description";
    QString Debit_Note_Details_Salesman_ID = "Salesman_ID";
    QString Debit_Note_Details_Requirement_ItemID = "Requirement_ItemID";
    QString Debit_Note_Details_Dimension = "Dimension";
    QString Debit_Note_Details_Length = "Length";
    QString Debit_Note_Details_UomId = "Uom_Id";
    QString Debit_Note_Details_itemVoucherStatus = "itemVoucherStatus";
    QString Debit_Note_Details_itemProductionStatus = "itemProductionStatus";
    QString Debit_Note_Details_Time = "Time";
    QString Debit_Note_Details_TaxRate = "Tax_Rate";
    QString Debit_Note_Details_TaxAmount = "Tax_Amount";

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
    QString Transaction_Helper_Debit_Amount = "Debit_Amount";
    QString Transaction_Helper_Credit_Amount = "Credit_Amount";
    QString Transaction_Helper_LedgerID = "Ledger_ID";

    QString VoucherType = GMVoucherTypes::getVoucherString(GMVoucherTypes::DebitNote); //"DEBITNOTE";

    bool flag = true;
    DatabaseHelper *db;
    ItemTransactionDatabaseHelper *itemTransaction;
    ItemTransactionDataModel *ItemModel;
    TransactionHelper *ledTransaction;

    int startTransaction();

    int insertVoucher(GeneralVoucherDataObject *obj) override;

    int updateVoucher(GeneralVoucherDataObject *obj, QString oldID) override;

    int deleteVoucher(GeneralVoucherDataObject *obj) override;

    int commit();

    int getMaxID();

    QString getNextVoucherNo(QString voucherPrefix) override;


    QSqlQueryModel *getVoucherListByDateModel(QDate dateFrom, QDate dateTo);

    GeneralVoucherDataObject *getVoucherByVoucherNoPtr(QString vNo, QString voucherPrefix = LoginValues::voucherPrefix) override;

    QPair<QString, QString> getVoucherNoByTransactionId(QString transId) override;

    QString getVoucherNumberByLedID(QString LedID, QDate Date) override;

    // VoucherDBAbstract interface
public:
    QSqlQueryModel *getVoucherListByDateTimeModel(QDateTime dateFrom, QDateTime dateTo) override;
};

#endif // DEBITNOTEVOUCHERDATABASEHELPER_H
