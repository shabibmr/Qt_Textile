#ifndef INVENTORYTEMPLATEDATABASEHELPER_H
#define INVENTORYTEMPLATEDATABASEHELPER_H

#include "datamodels/transaction/generalvoucherdataobject.h"
#include "database/databasehelper/databasehelper.h"
#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"
#include "database/finance/TransactionHelper/generalvoucherdatabasehelper.h"

#include <QMap>
#include <QDebug>


class InventoryTemplateDatabaseHelper
{
public:
    InventoryTemplateDatabaseHelper();
    ~InventoryTemplateDatabaseHelper();

    QString Inventory_Template_Main_TableName = "Inventory_Template_Main";

    QString Inventory_Template_Main_id = "_id";
    QString Inventory_Template_Main_Voucher_No = "Voucher_No";
    QString Inventory_Template_Main_Voucher_Prefix = "Voucher_Prefix";
    QString Inventory_Template_Main_Date_Created = "Date_Created";
    QString Inventory_Template_Main_Voucher_Date = "Voucher_Date";
    QString Inventory_Template_Main_TimeStamp = "`TimeStamp`";
    QString Inventory_Template_Main_TimeStampC = "TimeStamp";

    QString Inventory_Template_Main_Last_Updated = "Last_Updated";
    QString Inventory_Template_Main_Ledger_ID = "Ledger_ID";
    QString Inventory_Template_Main_Billing_Name = "Billing_Name";
    QString Inventory_Template_Main_Total = "Total";
    QString Inventory_Template_Main_Narration = "Narration";
    QString Inventory_Template_Main_Added_By = "Added_By";
    QString Inventory_Template_Main_Delivery_Date = "Delivery_Date";
    QString Inventory_Template_Main_Completion_Probability = "Completion_Probability";
    QString Inventory_Template_Main_Credit_Period = "Credit_Period";
    QString Inventory_Template_Main_POC_Name = "POC_Name";
    QString Inventory_Template_Main_POC_Phone = "POC_Phone";
    QString Inventory_Template_Main_POC_Email = "POC_Email";
    QString Inventory_Template_Main_Project_ID = "Project_ID";
    QString Inventory_Template_Main_Revision_Number = "Revision_Number";
    QString Inventory_Template_Main_Converted_To_Sales_Order = "Converted_To_Sales_Order";
    QString Inventory_Template_Main_Quotation_Dropped = "Quotation_Dropped";
    QString Inventory_Template_Main_Quotation_Dropped_Reason = "Quotation_Dropped_Reason";
    QString Inventory_Template_Main_Requirement_Voucher_No = "Requirement_Voucher_No";
    QString Inventory_Template_Main_Salesman_ID = "Salesman_ID";
    QString Inventory_Template_Main_LPORef="LPO_Ref";
    QString Inventory_Template_Main_ModeOfService = "MODE_OF_SERVICE";
    QString Inventory_Template_Main_Discount_In_Amount = "Discount_In_Amount";
    QString Inventory_Template_Main_Discount_In_Percent = "Discount_In_Percent";
    QString Inventory_Template_Main_VATAmount = "VAT_Amount";
    QString Inventory_Template_Main_VoucherStatus = "VoucherStatus";
    QString Inventory_Template_Main_Contact_ID = "ContactID";
    QString Inventory_Template_Main_Gross_Total = "Gross_Total";
    QString Inventory_Template_Main_Reference ="Reference";
    QString Inventory_Template_Main_Customer_Expecting_Date = "Customer_Expected_Date";
    QString Inventory_Template_Main_Currency = "Currency";
    QString Inventory_Template_Main_BoxesCount = "Boxes_Count";
    QString Inventory_Template_Main_TotalWeight = "Total_Weight";
    QString Inventory_Template_Main_LRNo = "LR_No";
    QString Inventory_Template_Main_VoucherNoDisplay = "Voucher_No_Display";
    QString Inventory_Template_Main_ReqVoucherList = "Requirement_Voucher_List";
    QString Inventory_Template_Main_Origin = "Origin";
    QString Inventory_Template_Main_Round_Off = "Round_Off";
    QString Inventory_Template_Main_TransactionId = "TransactionId";


    QString Inventory_Template_Details_TableName = "Inventory_Template_Details";

    QString Inventory_Template_Details_id = "_id";
    QString Inventory_Template_Details_Voucher_No = "Voucher_No";
    QString Inventory_Template_Details_Voucher_Prefix = "Voucher_Prefix";
    QString Inventory_Template_Details_Voucher_Date = "Voucher_Date";
    QString Inventory_Template_Details_Inventory_Item_ID = "Inventory_Item_ID";
    QString Inventory_Template_Details_Quantity = "Quantity";
    QString Inventory_Template_Details_Price = "Price";
    QString Inventory_Template_Details_UOM_ID ="UOM_ID" ;
    QString Inventory_Template_Details_PriceLevel = "PriceLevel";
    QString Inventory_Template_Details_isCompoundItem = "isCompoundItem";
    QString Inventory_Template_Details_subItemOff_list_ID = "subItemOff_list_ID";
    QString Inventory_Template_Details_Discount_Percent = "Discount_Percent";
    QString Inventory_Template_Details_Discount_Amount = "Discount_Amount";
    QString Inventory_Template_Details_Subtotal = "Subtotal";
    QString Inventory_Template_Details_Project_ID = "Project_ID";
    QString Inventory_Template_Details_Item_Narration = "Item_Narration";
    QString Inventory_Template_Details_Item_Description = "Item_Description";
    QString Inventory_Template_Details_Salesman_ID = "Salesman_ID";
    QString Inventory_Template_Details_Requirement_ItemID ="Requirement_ItemID";
    QString Inventory_Template_Details_itemVoucherStatus ="ItemVoucherStatus";
    QString Inventory_Template_Details_itemProductionStatus ="itemProductionStatus";
    QString Inventory_Template_Details_Time = "Time";
    QString Inventory_Template_Details_Length = "Length";
    QString Inventory_Template_Details_Dimension = "Dimension";
    QString Inventory_Template_Details_TaxRate = "Tax_Rate";
    QString Inventory_Template_Details_TaxAmount = "Tax_Amount";


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

    QString VoucherType = "TEMPLATE";


    bool flag = true;
    DatabaseHelper *db;

    int startTransaction();

    int insertInventoryTemplate(GeneralVoucherDataObject *obj);

    int updateInventoryTemplate(GeneralVoucherDataObject *obj, QString oldID);

    int deleteInventoryTemplate(QString oldID);

    int commit();

    int getMaxID();

    QSqlQueryModel *getTemplatesAsModel();

    GeneralVoucherDataObject *getVoucherByVoucherNoAsPtr(QString vNo);



};

#endif // INVENTORYTEMPLATEDATABASEHELPER_H
