#ifndef REQUIREMENTSDATABASEHELPER_H
#define REQUIREMENTSDATABASEHELPER_H
#include "datamodels/transaction/generalvoucherdataobject.h"
#include "database/databasehelper/databasehelper.h"
#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"
#include "database/finance/TransactionHelper/generalvoucherdatabasehelper.h"

class RequirementsDatabaseHelper
{
public:
    RequirementsDatabaseHelper();
    ~RequirementsDatabaseHelper();
    
    
    
    QString Requirement_Main_TableName = "Requirement_Main";
    
    QString Requirement_Main_id = "_id";
    QString Requirement_Main_Voucher_No = "Voucher_No";
    QString Requirement_Main_Voucher_Prefix = "Voucher_Prefix";
    QString Requirement_Main_Voucher_Date = "Voucher_Date";
    QString Requirement_Main_Date_Created = "Date_Created";
    QString Requirement_Main_TimeStamp = "TimeStamp";
    QString Requirement_Main_Ledger_ID = "Ledger_ID";
    QString Requirement_Main_Billing_Name = "Billing_Name";
    QString Requirement_Main_Total = "Total";
    QString Requirement_Main_Narration = "Narration";
    QString Requirement_Main_Added_By = "Added_By";
    QString Requirement_Main_Delivery_Date = "Delivery_Date";
    QString Requirement_Main_Completion_Probability = "Completion_Probability";
    QString Requirement_Main_Credit_Period = "Credit_Period";
    QString Requirement_Main_POC_Name = "POC_Name";
    QString Requirement_Main_POC_Phone = "POC_Phone";
    QString Requirement_Main_POC_Email = "POC_Email";
    QString Requirement_Main_Project_ID = "Project_ID";
    QString Requirement_Main_Revision_Number = "Revision_Number";
    QString Requirement_Main_Converted_To_Sales_Order = "Converted_To_Sales_Order";
    QString Requirement_Main_Quotation_Prepared = "Quotation_Prepared";
    QString Requirement_Main_Salesman_ID = "Salesman_ID";
    QString Requirement_Main_ContactID = "ContactID";
    
    QString Requirement_Details_TableName = "Requirement_Details";
    
    QString Requirement_Details_id = "_id";
    QString Requirement_Details_Voucher_No = "Voucher_No";
    QString Requirement_Details_Voucher_Prefix = "Voucher_Prefix";
    QString Requirement_Details_Voucher_Date = "Voucher_Date";
    QString Requirement_Details_Inventory_Item_ID = "Inventory_Item_ID";
    QString Requirement_Details_Quantity = "Quantity";
    QString Requirement_Details_Price = "Price";
    QString Requirement_Details_PriceLevel = "Price_Level";
    QString Requirement_Details_isCompoundItem = "isCompoundItem";
    QString Requirement_Details_subItemOff_list_ID = "subItemOff_list_ID";
    QString Requirement_Details_Discount_Percent = "Discount_Percent";
    QString Requirement_Details_Discount_Amount = "Discount_Amount";
    QString Requirement_Details_Subtotal = "Subtotal";
    QString Requirement_Details_Project_ID = "Project_ID";
    QString Requirement_Details_Item_Narration = "Item_Narration";
    QString Requirement_Details_Item_Description = "Item_Description";
    QString Requirement_Details_Salesman_ID = "Salesman_ID";
    QString Requirement_Details_Requirement_ItemID ="Requirement_ItemID";
    QString Requirement_Details_Length="Length";
    QString itemVoucherStatus ="ItemVoucherStatus";
    QString itemProductionStatus ="itemProductionStatus";

    
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
    
    QString ledgerType = "REQUIREMENT";
    
    
    
    
    
    bool flag = true;
    DatabaseHelper *db;
    
    int startTransaction();
    
    int insertRequirement(GeneralVoucherDataObject *obj);
    
    int updateRequirement(GeneralVoucherDataObject *obj, QString oldID);
    
    int deleteRequirement(QString oldID);
    
    int commit();
    
    int getMaxID();
    
    GeneralVoucherDataObject getVoucherByVoucherNo(QString vNo);
    
    QList<GeneralVoucherDataObject> getVoucherListByDate(QDate dateFrom,QDate dateTo);

    void setItemVoucherStatus(float qty,QString id);
    void resetItemVoucherStatus(float qty,QString id);
    QString getNextVoucherNo(QString voucherPrefix);
};

#endif // REQUIREMENTSDATABASEHELPER_H
