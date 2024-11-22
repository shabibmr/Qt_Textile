#ifndef MATERIALINOUTDATABASEHELPER_H
#define MATERIALINOUTDATABASEHELPER_H


#include "datamodels/transaction/generalvoucherdataobject.h"
#include "database/databasehelper/databasehelper.h"
#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"
#include "database/finance/TransactionHelper/generalvoucherdatabasehelper.h"

#include <QMap>
#include <QDebug>

class materialinoutDatabaseHelper
{
public:
    materialinoutDatabaseHelper();
    ~materialinoutDatabaseHelper();
    QString Material_In_Main_TableName = "Material_In_Main";

    QString Material_In_Main_id = "_id";
    QString Material_In_Main_Voucher_No = "Voucher_No";
    QString Material_In_Main_Voucher_Prefix = "Voucher_Prefix";
    QString Material_In_Main_Date_Created = "Date_Created";
    QString Material_In_Main_Voucher_Date = "Voucher_Date";
    QString Material_In_Main_TimeStamp = "TimeStamp";
    QString Material_In_Main_TimeStampC = "TimeStamp";

    QString Material_In_Main_Last_Updated = "Last_Updated";
    QString Material_In_Main_Ledger_ID = "Ledger_ID";
    QString Material_In_Main_Billing_Name = "Billing_Name";
    QString Material_In_Main_Total = "Total";
    QString Material_In_Main_Narration = "Narration";
    QString Material_In_Main_Added_By = "Added_By";
    QString Material_In_Main_Delivery_Date = "Delivery_Date";
    QString Material_In_Main_Completion_Probability = "Completion_Probability";
    QString Material_In_Main_Credit_Period = "Credit_Period";
    QString Material_In_Main_POC_Name = "POC_Name";
    QString Material_In_Main_POC_Phone = "POC_Phone";
    QString Material_In_Main_POC_Email = "POC_Email";
    QString Material_In_Main_Project_ID = "Project_ID";
    QString Material_In_Main_Revision_Number = "Revision_Number";
    QString Material_In_Main_Converted_To_Sales_Order = "Converted_To_Sales_Order";
    QString Material_In_Main_Quotation_Dropped = "Quotation_Dropped";
    QString Material_In_Main_Quotation_Dropped_Reason = "Quotation_Dropped_Reason";
    QString Material_In_Main_Requirement_Voucher_No = "Requirement_Voucher_No";
    QString Material_In_Main_Salesman_ID = "Salesman_ID";
    QString Material_In_Main_VoucherStatus = "VoucherStatus";

    QString Material_In_Details_TableName = "Material_In_Details";

    QString Material_In_Details_id = "_id";
    QString Material_In_Details_Voucher_No = "Voucher_No";
    QString Material_In_Details_Voucher_Prefix = "Voucher_Prefix";
    QString Material_In_Details_Voucher_Date = "Voucher_Date";
    QString Material_In_Details_Inventory_Item_ID = "Inventory_Item_ID";
    QString Material_In_Details_Quantity = "Quantity";
    QString Material_In_Details_UOMId = "Uom_Id";
    QString Material_In_Details_CrQty = "crQty";
    QString Material_In_Details_DrQty = "drQty";
    QString Material_In_Details_Price = "Price";
    QString Material_In_Details_PriceLevel = "PriceLevel";
    QString Material_In_Details_isCompoundItem = "isCompoundItem";
    QString Material_In_Details_subItemOff_list_ID = "subItemOff_list_ID";
    QString Material_In_Details_Discount_Percent = "Discount_Percent";
    QString Material_In_Details_Discount_Amount = "Discount_Amount";
    QString Material_In_Details_Subtotal = "Subtotal";
    QString Material_In_Details_Project_ID = "Project_ID";
    QString Material_In_Details_Item_Narration = "Item_Narration";
    QString Material_In_Details_Item_Description = "Item_Description";
    QString Material_In_Details_Salesman_ID = "Salesman_ID";
    QString Material_In_Details_Requirement_ItemID ="Requirement_ItemID";

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

    QString ledgerType = "SALESORDER";
    QString VoucherType = "MaterialIn";


    bool flag = true;
    DatabaseHelper *db;
    ItemTransactionDatabaseHelper *itemTransaction;
    ItemTransactionDataModel *ItemModel;

    int startTransaction();

    int insertMaterialInOut(GeneralVoucherDataObject *obj);

    int updateMaterialInOut(GeneralVoucherDataObject *obj, QString oldID);

    int deleteMaterialInOut(GeneralVoucherDataObject *obj);

    int commit();

    int getMaxID();

    GeneralVoucherDataObject getVoucherByVoucherNo(QString vNo);
    GeneralVoucherDataObject *getVoucherByVoucherNoPtr(QString vNo, QString voucherPrefix);

    QList<GeneralVoucherDataObject> getVoucherListByDate(QDate dateFrom,QDate dateTo);
    QSqlQueryModel *getVoucherListByDateModel(QDate dateFrom,QDate dateTo);

    QString getNextVoucherNo(QString voucherPrefix);
};

#endif // MATERIALINOUTDATABASEHELPER_H
