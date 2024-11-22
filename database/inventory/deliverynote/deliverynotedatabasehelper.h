#ifndef DELIVERYNOTEDATABASEHELPER_H
#define DELIVERYNOTEDATABASEHELPER_H


#include "database/voucherdbabstract.h"


class DeliveryNoteDatabaseHelper : public VoucherDBAbstract
{
public:
    DeliveryNoteDatabaseHelper(QString connString=DatabaseValues::connectionString);
    ~DeliveryNoteDatabaseHelper() override;

    QString Delivery_Note_Main_TableName = "Delivery_Note_Main";

    QString Delivery_Note_Main_id = "_id";
    QString Delivery_Note_Main_Voucher_No = "Voucher_No";
    QString Delivery_Note_Main_Voucher_Prefix = "Voucher_Prefix";
    QString Delivery_Note_Main_Date_Created = "Date_Created";
    QString Delivery_Note_Main_Voucher_Date = "Voucher_Date";
    QString Delivery_Note_Main_TimeStamp = "TimeStamp";
    QString Delivery_Note_Main_TimeStampC = "TimeStamp";

    QString Delivery_Note_Main_Last_Updated = "Last_Updated";
    QString Delivery_Note_Main_Ledger_ID = "Ledger_ID";
    QString Delivery_Note_Main_Billing_Name = "Billing_Name";
    QString Delivery_Note_Main_Total = "Total";
    QString Delivery_Note_Main_Narration = "Narration";
    QString Delivery_Note_Main_Added_By = "Added_By";
    QString Delivery_Note_Main_Delivery_Date = "Delivery_Date";
    QString Delivery_Note_Main_Completion_Probability = "Completion_Probability";
    QString Delivery_Note_Main_Credit_Period = "Credit_Period";
    QString Delivery_Note_Main_POC_Name = "POC_Name";
    QString Delivery_Note_Main_POC_Phone = "POC_Phone";
    QString Delivery_Note_Main_POC_Email = "POC_Email";
    QString Delivery_Note_Main_Project_ID = "Project_ID";
    QString Delivery_Note_Main_Revision_Number = "Revision_Number";
    QString Delivery_Note_Main_Converted_To_Sales_Order = "Converted_To_Sales_Order";
    QString Delivery_Note_Main_Quotation_Dropped = "Quotation_Dropped";
    QString Delivery_Note_Main_Quotation_Dropped_Reason = "Quotation_Dropped_Reason";
    QString Delivery_Note_Main_Requirement_Voucher_No = "Requirement_Voucher_No";
    QString Delivery_Note_Main_Delivery_By = "Delivery_By";
    QString Delivery_Note_Main_Salesman_ID = "Salesman_ID";
    QString Delivery_Note_Main_LPO = "LPO_Ref";
    QString Delivery_Note_Main_ModeOfService = "MODE_OF_SERVICE";
    QString Delivery_Note_Main_Discount_In_Amount = "Discount_In_Amount";
    QString Delivery_Note_Main_Discount_In_Percent = "Discount_In_Percent";
    QString Delivery_Note_Main_VATAmount = "VAT_Amount";
    QString Delivery_Note_Main_VoucherStatus = "VoucherStatus";
    QString Delivery_Note_Main_Contact_ID = "ContactID";
    QString Delivery_Note_Main_Gross_Total = "Gross_Total";
    QString Delivery_Note_Main_Reference ="Reference";
    QString Delivery_Note_Main_Customer_Expecting_Date = "Customer_Expected_Date";
    QString Delivery_Note_Main_Currency = "Currency";
    QString Delivery_Note_Main_BoxesCount = "Boxes_Count";
    QString Delivery_Note_Main_TotalWeight = "Total_Weight";
    QString Delivery_Note_Main_LRNo = "LR_No";
    QString Delivery_Note_Main_VoucherNoDisplay = "Voucher_No_Display";
    QString Delivery_Note_Main_ReqVoucherList = "Requirement_Voucher_List";
    QString Delivery_Note_Main_Origin = "Origin";
    QString Delivery_Note_Main_Round_Off = "Round_Off";
    QString Delivery_Note_Main_DN_Signed = "Sign_Status";
    QString Delivery_Note_Main_Godown = "PaymentCompleted";
    QString Delivery_Note_Main_GeneratedTransactionId = "TransactionId";

    QString Delivery_Note_Details_TableName = "Delivery_Note_Details";

    QString Delivery_Note_Details_id = "_id";
    QString Delivery_Note_Details_Voucher_No = "Voucher_No";
    QString Delivery_Note_Details_Voucher_Prefix = "Voucher_Prefix";
    QString Delivery_Note_Details_Voucher_Date = "Voucher_Date";
    QString Delivery_Note_Details_Inventory_Item_ID = "Inventory_Item_ID";
    QString Delivery_Note_Details_Quantity = "Quantity";
    QString Delivery_Note_Details_Price = "Price";
    QString Delivery_Note_Details_UOM_ID ="UOM_ID" ;
    QString Delivery_Note_Details_PriceLevel = "PriceLevel";
    QString Delivery_Note_Details_isCompoundItem = "isCompoundItem";
    QString Delivery_Note_Details_subItemOff_list_ID = "subItemOff_list_ID";
    QString Delivery_Note_Details_Discount_Percent = "Discount_Percent";
    QString Delivery_Note_Details_Discount_Amount = "Discount_Amount";
    QString Delivery_Note_Details_Subtotal = "Subtotal";
    QString Delivery_Note_Details_Project_ID = "Project_ID";
    QString Delivery_Note_Details_Item_Narration = "Item_Narration";
    QString Delivery_Note_Details_Item_Description = "Item_Description";
    QString Delivery_Note_Details_Salesman_ID = "Salesman_ID";
    QString Delivery_Note_Details_Requirement_ItemID ="Requirement_ItemID";
    QString Delivery_Note_Details_Length="Length";
    QString Delivery_Note_Details_Dimension = "Dimension";
    QString Delivery_Note_Details_itemVoucherStatus ="ItemVoucherStatus";
    QString Delivery_Note_Details_itemProductionStatus ="itemProductionStatus";
    QString Delivery_Note_Details_Time = "Time";
    QString Delivery_Note_Details_TaxRate = "Tax_Rate";
    QString Delivery_Note_Details_TaxAmount = "Tax_Amount";


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

    QString VoucherType = GMVoucherTypes::getVoucherString(GMVoucherTypes::DeliveryNote); //"DELIVERYNOTE";

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

    QString getNextVoucherNo(QString voucherPrefix) override;
    GeneralVoucherDataObject *getVoucherByVoucherNoPtr(QString vNo, QString voucherPrefix ) override;
    QPair<QString, QString> getVoucherNoByTransactionId(QString transId) override;
    QString getVoucherNumberByLedID(QString LedID, QDate Date) override ;

    QSqlQueryModel *getVoucherListByDateModel(QDate dateFrom, QDate dateTo) override ;

    void updateSignedStatus(bool status,QString dnNO);

    bool getSignatureStatus(QString dnNO);

    void setItemVoucherStatus(float qty,QString id);

    void resetItemVoucherStatus(float qty,QString id);

    // VoucherDBAbstract interface
public:

    QSqlQueryModel *getVoucherListByDateTimeModel(QDateTime dateFrom, QDateTime dateTo) override;
};

#endif // DELIVERYNOTEDATABASEHELPER_H
