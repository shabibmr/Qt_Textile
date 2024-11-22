#ifndef PURCHASEORDERDATABASEHELPER_H
#define PURCHASEORDERDATABASEHELPER_H



#include "database/voucherdbabstract.h"

class PurchaseOrderDatabaseHelper : public VoucherDBAbstract
{
public:
    PurchaseOrderDatabaseHelper(QString connString=DatabaseValues::connectionString);
    ~PurchaseOrderDatabaseHelper() override;

    QString static Purchase_Order_Main_TableName;
    QString static Purchase_Order_Details_TableName;

    QString Purchase_Order_Main_id = "_id";
    QString Purchase_Order_Main_Voucher_No = "Voucher_No";
    QString Purchase_Order_Main_Voucher_Prefix = "Voucher_Prefix";
    QString Purchase_Order_Main_Date_Created = "Date_Created";
    QString Purchase_Order_Main_Voucher_Date = "Voucher_Date";
    QString Purchase_Order_Main_TimeStamp = "`TimeStamp`";
    QString Purchase_Order_Main_TimeStampC = "TimeStamp";

    QString Purchase_Order_Main_Last_Updated = "Last_Updated";
    QString Purchase_Order_Main_Ledger_ID = "Ledger_ID";
    QString Purchase_Order_Main_Billing_Name = "Billing_Name";
    QString Purchase_Order_Main_Total = "Total";
    QString Purchase_Order_Main_Narration = "Narration";
    QString Purchase_Order_Main_Added_By = "Added_By";
    QString Purchase_Order_Main_Delivery_Date = "Delivery_Date";
    QString Purchase_Order_Main_Completion_Probability = "Completion_Probability";
    QString Purchase_Order_Main_Credit_Period = "Credit_Period";
    QString Purchase_Order_Main_POC_Name = "POC_Name";
    QString Purchase_Order_Main_POC_Phone = "POC_Phone";
    QString Purchase_Order_Main_POC_Email = "POC_Email";
    QString Purchase_Order_Main_Project_ID = "Project_ID";
    QString Purchase_Order_Main_Revision_Number = "Revision_Number";
    QString Purchase_Order_Main_Converted_To_Sales_Order = "Converted_To_Sales_Order";
    QString Purchase_Order_Main_Quotation_Dropped = "Quotation_Dropped";
    QString Purchase_Order_Main_Quotation_Dropped_Reason = "Quotation_Dropped_Reason";
    QString Purchase_Order_Main_Requirement_Voucher_No = "Requirement_Voucher_No";
    QString Purchase_Order_Main_Salesman_ID = "Salesman_ID";
    QString Purchase_Order_Main_LPO="LPO_Ref";
    QString Purchase_Order_Main_ModeOfService = "MODE_OF_SERVICE";
    QString Purchase_Order_Main_Discount_In_Amount = "Discount_In_Amount";
    QString Purchase_Order_Main_Discount_In_Percent = "Discount_In_Percent";
    QString Purchase_Order_Main_VATAmount = "VAT_Amount";
    QString Purchase_Order_Main_VoucherStatus = "VoucherStatus";
    QString Purchase_Order_Main_Contact_ID = "ContactID";
    QString Purchase_Order_Main_Gross_Total = "Gross_Total";
    QString Purchase_Order_Main_Reference ="Reference";
    QString Purchase_Order_Main_Customer_Expecting_Date = "Customer_Expected_Date";
    QString Purchase_Order_Main_Currency = "Currency";
    QString Purchase_Order_Main_BoxesCount = "Boxes_Count";
    QString Purchase_Order_Main_TotalWeight = "Total_Weight";
    QString Purchase_Order_Main_LRNo = "LR_No";
    QString Purchase_Order_Main_VoucherNoDisplay = "Voucher_No_Display";
    QString Purchase_Order_Main_ReqVoucherList = "Requirement_Voucher_List";
    QString Purchase_Order_Main_Origin = "Origin";
    QString Purchase_Order_Main_Round_Off = "Round_Off";
    QString Purchase_Order_Main_TransactionId = "TransactionId";

    QString Purchase_Order_Details_id = "_id";
    QString Purchase_Order_Details_Voucher_No = "Voucher_No";
    QString Purchase_Order_Details_Voucher_Prefix = "Voucher_Prefix";
    QString Purchase_Order_Details_Voucher_Date = "Voucher_Date";
    QString Purchase_Order_Details_Inventory_Item_ID = "Inventory_Item_ID";
    QString Purchase_Order_Details_Quantity = "Quantity";
    QString Purchase_Order_Details_QuantityFull = "Quantity_Full";
    QString Purchase_Order_Details_QuantityDisc = "Quantity_Disc";

    QString Purchase_Order_Details_Price = "Price";
    QString Purchase_Order_Details_UOM_ID ="UOM_ID" ;
    QString Purchase_Order_Details_PriceLevel = "PriceLevel";
    QString Purchase_Order_Details_isCompoundItem = "isCompoundItem";
    QString Purchase_Order_Details_subItemOff_list_ID = "subItemOff_list_ID";
    QString Purchase_Order_Details_Discount_Percent = "Discount_Percent";
    QString Purchase_Order_Details_Discount_Amount = "Discount_Amount";
    QString Purchase_Order_Details_Subtotal = "Subtotal";
    QString Purchase_Order_Details_Project_ID = "Project_ID";
    QString Purchase_Order_Details_Item_Narration = "Item_Narration";
    QString Purchase_Order_Details_Item_Description = "Item_Description";
    QString Purchase_Order_Details_Salesman_ID = "Salesman_ID";
    QString Purchase_Order_Details_Requirement_ItemID ="Requirement_ItemID";
    QString Purchase_Order_Details_itemVoucherStatus ="ItemVoucherStatus";
    QString Purchase_Order_Details_itemProductionStatus ="itemProductionStatus";
    QString Purchase_Order_Details_Length = "Length";
    QString Purchase_Order_Details_Dimension = "Dimension";
    QString Purchase_Order_Details_TaxRate = "Tax_Rate";
    QString Purchase_Order_Details_TaxAmount = "Tax_Amount";
    QString Purchase_Order_Details_TimeDate ="TimeDate";
    QString Purchase_Order_Details_ContainerCount = "ContainerCount";

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
    QString Proxy_Transaction_Helper_isAdditional ="isAdditional";

    QString VoucherType = GMVoucherTypes::getVoucherString( GMVoucherTypes::PurchaseOrder); // "PURCHASEORDER";
    TransactionHelper *ledTransaction;

    bool flag = true;
    DatabaseHelper *db;

    int startTransaction();
    int insertVoucher(GeneralVoucherDataObject *obj) override;
    int updateVoucher(GeneralVoucherDataObject *obj, QString oldID) override;
    int deleteVoucher(GeneralVoucherDataObject *obj) override;
    int commit();
    QString getNextVoucherNo(QString voucherPrefix) override;


    QSqlQueryModel *getVoucherListByDateModel(QDate dateFrom,QDate dateTo) override;
    GeneralVoucherDataObject *getVoucherByVoucherNoPtr(QString vNo, QString voucherPrefix) override;
    QPair<QString, QString> getVoucherNoByTransactionId(QString transId) override;
    int updateVoucherStatus(GeneralVoucherDataObject *voucher);

    // VoucherDBAbstract interface

    QString getVoucherNumberByLedID(QString LedID, QDate Date) override;

    // VoucherDBAbstract interface

    QSqlQueryModel *getVoucherListByDateTimeModel(QDateTime dateFrom, QDateTime dateTo) override;
    QSqlQueryModel *getItemwisePendingOrders(QDate fromDate, QDate toDate);
    QSqlQueryModel *getPendingPurchaseOrdersByDeliveryDate(QDateTime fromDate, QDateTime toDate);
};

#endif // PURCHASEORDERDATABASEHELPER_H
