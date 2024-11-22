#ifndef PURCHASEVOUCHERDATABASEHELPER_H
#define PURCHASEVOUCHERDATABASEHELPER_H


#include "database/voucherdbabstract.h"


class PurchaseVoucherDatabaseHelper : public VoucherDBAbstract
{
public:
    PurchaseVoucherDatabaseHelper(QString connString=DatabaseValues::connectionString);
    ~PurchaseVoucherDatabaseHelper();

    static QString Purchase_Invoice_Main_TableName ;
    QString Purchase_Invoice_Main_id = "_id";
    QString Purchase_Invoice_Main_Voucher_No = "Voucher_No";
    QString Purchase_Invoice_Main_Voucher_Prefix = "Voucher_Prefix";
    QString Purchase_Invoice_Main_Date_Created = "Date_Created";
    QString Purchase_Invoice_Main_Voucher_Date = "Voucher_Date";
    QString Purchase_Invoice_Main_TimeStamp = "`TimeStamp`";
    QString Purchase_Invoice_Main_TimeStampC = "TimeStamp";

    QString Purchase_Invoice_Main_Last_Updated = "Last_Updated";
    QString Purchase_Invoice_Main_Ledger_ID = "Ledger_ID";
    QString Purchase_Invoice_Main_Billing_Name = "Billing_Name";
    QString Purchase_Invoice_Main_Total = "Total";
    QString Purchase_Invoice_Main_Narration = "Narration";
    QString Purchase_Invoice_Main_Added_By = "Added_By";
    QString Purchase_Invoice_Main_Delivery_Date = "Delivery_Date";
    QString Purchase_Invoice_Main_Completion_Probability = "Completion_Probability";
    QString Purchase_Invoice_Main_Credit_Period = "Credit_Period";
    QString Purchase_Invoice_Main_POC_Name = "POC_Name";
    QString Purchase_Invoice_Main_POC_Phone = "POC_Phone";
    QString Purchase_Invoice_Main_POC_Email = "POC_Email";
    QString Purchase_Invoice_Main_Project_ID = "Project_ID";
    QString Purchase_Invoice_Main_Revision_Number = "Revision_Number";
    QString Purchase_Invoice_Main_Converted_To_Sales_Order = "Converted_To_Sales_Order";
    QString Purchase_Invoice_Main_Quotation_Dropped = "Quotation_Dropped";
    QString Purchase_Invoice_Main_Quotation_Dropped_Reason = "Quotation_Dropped_Reason";
    QString Purchase_Invoice_Main_Requirement_Voucher_No = "Requirement_Voucher_No";
    QString Purchase_Invoice_Main_Salesman_ID = "Salesman_ID";
    QString Purchase_Invoice_Main_LPORef="LPO_Ref";
    QString Purchase_Invoice_Main_ModeOfService = "MODE_OF_SERVICE";
    QString Purchase_Invoice_Main_Discount_In_Amount = "Discount_In_Amount";
    QString Purchase_Invoice_Main_Discount_In_Percent = "Discount_In_Percent";
    QString Purchase_Invoice_Main_VATAmount = "VAT_Amount";
    QString Purchase_Invoice_Main_VoucherStatus = "VoucherStatus";
    QString Purchase_Invoice_Main_Contact_ID = "ContactID";
    QString Purchase_Invoice_Main_Gross_Total = "Gross_Total";
    QString Purchase_Invoice_Main_Reference ="Reference";
    QString Purchase_Invoice_Main_Customer_Expecting_Date = "Customer_Expected_Date";
    QString Purchase_Invoice_Main_Currency = "Currency";
    QString Purchase_Invoice_Main_BoxesCount = "Boxes_Count";
    QString Purchase_Invoice_Main_TotalWeight = "Total_Weight";
    QString Purchase_Invoice_Main_LRNo = "LR_No";
    QString Purchase_Invoice_Main_VoucherNoDisplay = "Voucher_No_Display";
    QString Purchase_Invoice_Main_ReqVoucherList = "Requirement_Voucher_List";
    QString Purchase_Invoice_Main_Origin = "Origin";
    QString Purchase_Invoice_Main_Round_Off = "Round_Off";
    QString Purchase_Invoice_Main_GeneratedTransactionId = "TransactionId";
    QString Purchase_Invoice_Main_InvoiceNo = "Invoice_No";
    QString Purchase_Invoice_Main_InvoiceDate = "Invoice_Date";
    QString Purchase_Invoice_Main_Godown = "PaymentCompleted";

    QString Purchase_Invoice_Details_TableName = "Purchase_Invoice_Details";

    QString Purchase_Invoice_Details_id = "_id";
    QString Purchase_Invoice_Details_Voucher_No = "Voucher_No";
    QString Purchase_Invoice_Details_Voucher_Prefix = "Voucher_Prefix";
    QString Purchase_Invoice_Details_Voucher_Date = "Voucher_Date";
    QString Purchase_Invoice_Details_Inventory_Item_ID = "Inventory_Item_ID";
    QString Purchase_Invoice_Details_Quantity = "Quantity";
    QString Purchase_Invoice_Details_Price = "Price";
    QString Purchase_Invoice_Details_PriceLevel = "PriceLevel";
    QString Purchase_Invoice_Details_isCompoundItem = "isCompoundItem";
    QString Purchase_Invoice_Details_subItemOff_list_ID = "subItemOff_list_ID";
    QString Purchase_Invoice_Details_Discount_Percent = "Discount_Percent";
    QString Purchase_Invoice_Details_Discount_Amount = "Discount_Amount";
    QString Purchase_Invoice_Details_Subtotal = "Subtotal";
    QString Purchase_Invoice_Details_Project_ID = "Project_ID";
    QString Purchase_Invoice_Details_Item_Narration = "Item_Narration";
    QString Purchase_Invoice_Details_Item_Description = "Item_Description";
    QString Purchase_Invoice_Details_Salesman_ID = "Salesman_ID";
    QString Purchase_Invoice_Details_Requirement_ItemID ="Requirement_ItemID";
    QString Purchase_Invoice_Details_Dimension ="Dimension";
    QString Purchase_Invoice_Details_Length ="Length";
    QString Purchase_Invoice_Details_UomID = "UOM_id";
    QString Purchase_Invoice_Details_itemVoucherStatus = "itemVoucherStatus";
    QString Purchase_Invoice_Details_itemProductionStatus = "itemProductionStatus";
    QString Purchase_Invoice_Details_Time = "Time";
    QString Purchase_Invoice_Details_TaxRate = "Tax_Rate";
    QString Purchase_Invoice_Details_TaxAmount = "Tax_Amount";

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
    QString VoucherType = GMVoucherTypes::getVoucherString(GMVoucherTypes::PurchaseVoucher); //= "PURCHASEVOUCHER";
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

    QSqlQueryModel *getVoucherListByDateModel(QDate dateFrom,QDate dateTo) override;

    GeneralVoucherDataObject *getVoucherByVoucherNoPtr(QString vNo, QString voucherPrefix) override;

    float getLastPurchPrice(QString itemID);

    QString getNextVoucherNo(QString voucherPrefix) override;

    QPair<QString, QString> getVoucherNoByTransactionId(QString transId) override;


    QString getVoucherNumberByLedID(QString LedID, QDate Date) override;

    // VoucherDBAbstract interface
public:
    QSqlQueryModel *getVoucherListByDateTimeModel(QDateTime dateFrom, QDateTime dateTo) override;
};

#endif // PURCHASEVOUCHERDATABASEHELPER_H
