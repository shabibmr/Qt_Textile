#ifndef STOCKENTRYDATABASEHELPER_H
#define STOCKENTRYDATABASEHELPER_H

#include "database/voucherdbabstract.h"

class StockEntryDatabaseHelper : public VoucherDBAbstract
{
public:
    StockEntryDatabaseHelper(QString connString=DatabaseValues::connectionString);
    ~StockEntryDatabaseHelper() override;

    QString Stock_Main_TableName = "Stock_Main";

    QString Stock_Main_id = "_id";
    QString Stock_Main_Voucher_No = "Voucher_No";
    QString Stock_Main_Voucher_Prefix = "Voucher_Prefix";
    QString Stock_Main_Voucher_Date = "Voucher_Date";
    QString Stock_Main_Date_Created = "Date_Created";
    QString Stock_Main_TimeStamp = "TimeStamp";
    QString Stock_Main_TimeStampC = "TimeStamp";
    QString Stock_Main_Last_Updated = "Last_Updated";
    QString Stock_Main_Ledger_ID = "Ledger_ID";
    QString Stock_Main_Billing_Name = "Billing_Name";
    QString Stock_Main_Total = "Total";
    QString Stock_Main_Narration = "Narration";
    QString Stock_Main_Added_By = "Added_By";
    QString Stock_Main_Delivery_Date = "Delivery_Date";
    QString Stock_Main_Completion_Probability = "Completion_Probability";
    QString Stock_Main_Credit_Period = "Credit_Period";
    QString Stock_Main_POC_Name = "POC_Name";
    QString Stock_Main_POC_Phone = "POC_Phone";
    QString Stock_Main_POC_Email = "POC_Email";
    QString Stock_Main_Project_ID = "Project_ID";
    QString Stock_Main_Revision_Number = "Revision_Number";
    QString Stock_Main_Converted_To_Sales_Order = "Converted_To_Sales_Order";
    QString Stock_Main_Quotation_Prepared = "Quotation_Prepared";
    QString Stock_Main_Salesman_ID = "Salesman_ID";

    QString Stock_Details_TableName = "Stock_Details";

    QString Stock_Details_id = "_id";
    QString Stock_Details_Voucher_No = "Voucher_No";
    QString Stock_Details_Voucher_Prefix = "Voucher_Prefix";
    QString Stock_Details_Voucher_Date = "Voucher_Date";
    QString Stock_Details_Inventory_Item_ID = "Inventory_Item_ID";
    QString Stock_Details_Quantity = "Quantity";
    QString Stock_Details_Difference = "Adj_Value";
    QString Stock_Details_Price = "Price";
    QString Stock_Details_PriceLevel = "Price_Level";
    QString Stock_Details_isCompoundItem = "isCompoundItem";
    QString Stock_Details_subItemOff_list_ID = "subItemOff_list_ID";
    QString Stock_Details_Discount_Percent = "Discount_Percent";
    QString Stock_Details_Discount_Amount = "Discount_Amount";
    QString Stock_Details_Subtotal = "Subtotal";
    QString Stock_Details_Project_ID = "Project_ID";
    QString Stock_Details_Item_Narration = "Item_Narration";
    QString Stock_Details_Item_Description = "Item_Description";
    QString Stock_Details_Salesman_ID = "Salesman_ID";
    QString Stock_Details_Uom = "UOM_Id";
    QString Stock_Detials_ItemReqUuid = "Requirement_ItemID";

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

    QString VoucherType = GMVoucherTypes::getVoucherString(GMVoucherTypes::StockEntry); // "STOCK";


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

    GeneralVoucherDataObject getVoucherByVoucherNo(QString vNo);

    GeneralVoucherDataObject* getVoucherByVoucherNoPtr(QString vNo, QString voucherPrefix) override;

    QList<GeneralVoucherDataObject> getVoucherListByDate(QDate dateFrom,QDate dateTo);
    QString getNextVoucherNo(QString voucherPrefix) override;
    QSqlQueryModel *getVoucherListByDateModel(QDate dateFrom, QDate dateTo) override;

    QSqlQueryModel* getVoucherListByDateTimeModel(QDateTime dateFrom, QDateTime dateTo) override;
    QPair<QString, QString> getVoucherNoByTransactionId(QString transId) override;
    QString getVoucherNumberByLedID(QString LedID,QDate Date) override;
};

#endif // STOCKENTRYDATABASEHELPER_H
