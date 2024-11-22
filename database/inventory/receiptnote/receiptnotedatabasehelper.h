#ifndef RECEIPTNOTEDATABASEHELPER_H
#define RECEIPTNOTEDATABASEHELPER_H


#include "database/voucherdbabstract.h"


class ReceiptNoteDatabaseHelper :public VoucherDBAbstract
{
public:

    ReceiptNoteDatabaseHelper(QString connString=DatabaseValues::connectionString);
    ~ReceiptNoteDatabaseHelper();

    QString Receipt_Note_Main_TableName = "Receipt_Note_Main";

    QString Receipt_Note_Main_id = "_id";
    QString Receipt_Note_Main_Voucher_No = "Voucher_No";
    QString Receipt_Note_Main_Voucher_Prefix = "Voucher_Prefix";
    QString Receipt_Note_Main_Date_Created = "Date_Created";
    QString Receipt_Note_Main_Voucher_Date = "Voucher_Date";
    QString Receipt_Note_Main_TimeStamp = "`TimeStamp`";
    QString Receipt_Note_Main_TimeStampC = "TimeStamp";
    QString Receipt_Note_Main_Last_Updated = "Last_Updated";
    QString Receipt_Note_Main_Ledger_ID = "Ledger_ID";
    QString Receipt_Note_Main_Billing_Name = "Billing_Name";
    QString Receipt_Note_Main_Total = "Total";
    QString Receipt_Note_Main_Narration = "Narration";
    QString Receipt_Note_Main_Added_By = "Added_By";
    QString Receipt_Note_Main_Delivery_Date = "Delivery_Date";
    QString Receipt_Note_Main_Completion_Probability = "Completion_Probability";
    QString Receipt_Note_Main_Credit_Period = "Credit_Period";
    QString Receipt_Note_Main_POC_Name = "POC_Name";
    QString Receipt_Note_Main_POC_Phone = "POC_Phone";
    QString Receipt_Note_Main_POC_Email = "POC_Email";
    QString Receipt_Note_Main_Project_ID = "Project_ID";
    QString Receipt_Note_Main_Revision_Number = "Revision_Number";
    QString Receipt_Note_Main_Converted_To_Sales_Order = "Converted_To_Sales_Order";
    QString Receipt_Note_Main_Quotation_Dropped = "Quotation_Dropped";
    QString Receipt_Note_Main_Quotation_Dropped_Reason = "Quotation_Dropped_Reason";
    QString Receipt_Note_Main_Requirement_Voucher_No = "Requirement_Voucher_No";
    QString Receipt_Note_Main_Salesman_ID = "Salesman_ID";
    QString Receipt_Note_Main_LPORef="LPO_Ref";
    QString Receipt_Note_Main_ModeOfService = "MODE_OF_SERVICE";
    QString Receipt_Note_Main_Discount_In_Amount = "Discount_In_Amount";
    QString Receipt_Note_Main_Discount_In_Percent = "Discount_In_Percent";
    QString Receipt_Note_Main_VATAmount = "VAT_Amount";
    QString Receipt_Note_Main_VoucherStatus = "VoucherStatus";
    QString Receipt_Note_Main_Contact_ID = "ContactID";
    QString Receipt_Note_Main_Gross_Total = "Gross_Total";
    QString Receipt_Note_Main_Reference ="Reference";
    QString Receipt_Note_Main_Customer_Expecting_Date = "Customer_Expected_Date";
    QString Receipt_Note_Main_Currency = "Currency";
    QString Receipt_Note_Main_BoxesCount = "Boxes_Count";
    QString Receipt_Note_Main_TotalWeight = "Total_Weight";
    QString Receipt_Note_Main_LRNo = "LR_No";
    QString Receipt_Note_Main_VoucherNoDisplay = "Voucher_No_Display";
    QString Receipt_Note_Main_ReqVoucherList = "Requirement_Voucher_List";
    QString Receipt_Note_Main_Origin = "Origin";
    QString Receipt_Note_Main_Round_Off = "Round_Off";
    QString Receipt_Note_Main_DN_Signed = "Sign_Status";
    QString Receipt_Note_Main_Godown = "PaymentCompleted";
    QString Receipt_Note_Main_GeneratedTransactionId = "TransactionId";

    QString Receipt_Note_Details_TableName = "Receipt_Note_Details";

    QString Receipt_Note_Details_id = "_id";
    QString Receipt_Note_Details_Voucher_No = "Voucher_No";
    QString Receipt_Note_Details_Voucher_Prefix = "Voucher_Prefix";
    QString Receipt_Note_Details_Voucher_Date = "Voucher_Date";
    QString Receipt_Note_Details_Inventory_Item_ID = "Inventory_Item_ID";
    QString Receipt_Note_Details_Quantity = "Quantity";
    QString Receipt_Note_Details_Price = "Price";
    QString Receipt_Note_Details_UOM_ID ="UOM_ID" ;
    QString Receipt_Note_Details_PriceLevel = "PriceLevel";
    QString Receipt_Note_Details_isCompoundItem = "isCompoundItem";
    QString Receipt_Note_Details_subItemOff_list_ID = "subItemOff_list_ID";
    QString Receipt_Note_Details_Discount_Percent = "Discount_Percent";
    QString Receipt_Note_Details_Discount_Amount = "Discount_Amount";
    QString Receipt_Note_Details_Subtotal = "Subtotal";
    QString Receipt_Note_Details_Project_ID = "Project_ID";
    QString Receipt_Note_Details_Item_Narration = "Item_Narration";
    QString Receipt_Note_Details_Item_Description = "Item_Description";
    QString Receipt_Note_Details_Salesman_ID = "Salesman_ID";
    QString Receipt_Note_Details_Requirement_ItemID ="Requirement_ItemID";
    QString Receipt_Note_Details_Length="Length";
    QString Receipt_Note_Details_Dimension = "Dimension";
    QString Receipt_Note_Details_itemVoucherStatus ="ItemVoucherStatus";
    QString Receipt_Note_Details_itemProductionStatus ="itemProductionStatus";
    QString Receipt_Note_Details_Time = "Time";
    QString Receipt_Note_Details_TaxRate = "Tax_Rate";
    QString Receipt_Note_Details_TaxAmount = "Tax_Amount";


    QString Proxy_Transaction_Helper_TableName = "Proxy_Transaction_Helper";
    QString Proxy_Transaction_Helper_id = "_id";
    QString Proxy_Transaction_Helper_VoucherType = "Voucher_Type";
    QString Proxy_Transaction_Helper_VoucherPrefix = "Voucher_Prefix";
    QString Proxy_Transaction_Helper_VoucherNo = "Voucher_ID";
    QString Proxy_Transaction_Helper_VoucherDate = "Voucher_Date";
    QString Proxy_Transaction_Helper_ProjectID = "Project_ID";
    QString Proxy_Transaction_Helper_TranactionAmount = "Transaction_Amount";
    QString Proxy_Transaction_Helper_CreditLedgerID = "Credit_Ledger_ID";
    QString Proxy_Transaction_Helper_DebitLedgerID = "Debit_Ledger_ID";
    QString Proxy_Transaction_Helper_Narration = "Narration";
    QString Proxy_Transaction_Helper_SalesManID = "Salesman_ID";
    QString Proxy_Transaction_Helper_TimeStamp = "TIMESTAMP";

    QString VoucherType = GMVoucherTypes::getVoucherString(GMVoucherTypes::ReceiptNote); // "RECEIPTNOTE";

    bool flag = true;
    DatabaseHelper *db;

    ItemTransactionDatabaseHelper *itemTransaction;
    ItemTransactionDataModel *ItemModel;

    int startTransaction();

    int insertVoucher(GeneralVoucherDataObject *obj) override;

    int updateVoucher(GeneralVoucherDataObject *obj, QString oldID) override;

    int deleteVoucher(GeneralVoucherDataObject *obj) override;

    int commit();

    int getMaxID();

    GeneralVoucherDataObject *getVoucherByVoucherNoPtr(QString vNo, QString voucherPrefix)override;
    QSqlQueryModel *getVoucherListByDateModel(QDate dateFrom, QDate dateTo)override;
    QString getNextVoucherNo(QString voucherPrefix)override;
    QPair<QString, QString> getVoucherNoByTransactionId(QString transId) override;
    QString getVoucherNumberByLedID(QString LedID, QDate Date) override;

    void updateSignedStatus(bool status,QString dnNO);

    bool getSignatureStatus(QString dnNO);

    void setItemVoucherStatus(float qty,QString id);

    void resetItemVoucherStatus(float qty,QString id);

    // VoucherDBAbstract interface
public:
    QSqlQueryModel *getVoucherListByDateTimeModel(QDateTime dateFrom, QDateTime dateTo) override;
};


#endif // RECEIPTNOTEDATABASEHELPER_H
