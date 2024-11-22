#ifndef OLDQUOTATIONDATABASEHELPER_H
#define OLDQUOTATIONDATABASEHELPER_H


#include "datamodels/transaction/generalvoucherdataobject.h"
#include "database/databasehelper/databasehelper.h"
#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"
#include "database/addressbook/addressbookdatabasehelper.h"

#include <QMap>
#include <QDebug>

class OldQuotationDatabaseHelper
{
public:
    OldQuotationDatabaseHelper();
    ~OldQuotationDatabaseHelper();
    QString Quotation_Main_TableName = "Quotation_Main_old";

    QString Quotation_Main_id = "_id";
    QString Quotation_Main_Voucher_No = "Voucher_No";
    QString Quotation_Main_Voucher_Prefix = "Voucher_Prefix";
    QString Quotation_Main_Date_Created = "Date_Created";
    QString Quotation_Main_Voucher_Date = "Voucher_Date";
    QString Quotation_Main_TimeStamp = "TimeStamp";
    QString Quotation_Main_Ledger_ID = "Ledger_ID";
    QString Quotation_Main_Billing_Name = "Billing_Name";
    QString Quotation_Main_Total = "Total";
    QString Quotation_Main_Narration = "Narration";
    QString Quotation_Main_Added_By = "Added_By";
    QString Quotation_Main_Delivery_Date = "Delivery_Date";
    QString Quotation_Main_Completion_Probability = "Completion_Probability";
    QString Quotation_Main_Credit_Period = "Credit_Period";
    QString Quotation_Main_POC_Name = "POC_Name";
    QString Quotation_Main_POC_Phone = "POC_Phone";
    QString Quotation_Main_POC_Email = "POC_Email";
    QString Quotation_Main_Project_ID = "Project_ID";
    QString Quotation_Main_Revision_Number = "Revision_Number";
    QString Quotation_Main_Converted_To_Sales_Order = "Converted_To_Sales_Order";
    QString Quotation_Main_Quotation_Dropped = "Quotation_Dropped";
    QString Quotation_Main_Quotation_Dropped_Reason = "Quotation_Dropped_Reason";
    QString Quotation_Main_Requirement_Voucher_No = "Requirement_Voucher_No";
    QString Quotation_Main_Salesman_ID = "Salesman_ID";
    QString Quotation_Main_Terms_And_Conditions_ID = "Terms_And_Conditions_ID";
    QString Quotation_Main_Manager_Approval_Status ="ManagerApprovalStatus";
    QString Quotation_Main_Client_Approval_Status ="ClientApprovalStatus";
    QString Quotation_Main_Contact_ID="ContactID";
    QString Quotation_Main_LPO_Ref="LPO_Ref";




    QString Quotation_Details_TableName = "Quotation_Details_old";

    QString Quotation_Details_id = "_id";
    QString Quotation_Details_Voucher_No = "Voucher_No";
    QString Quotation_Details_Voucher_Prefix = "Voucher_Prefix";
    QString Quotation_Details_Voucher_Date = "Voucher_Date";
    QString Quotation_Details_Inventory_Item_ID = "Inventory_Item_ID";
    QString Quotation_Details_Quantity = "Quantity";
    QString Quotation_Details_Price = "Price";
    QString Quotation_Details_PriceLevel = "PriceLevel";
    QString Quotation_Details_isCompoundItem = "isCompoundItem";
    QString Quotation_Details_subItemOff_list_ID = "subItemOff_list_ID";
    QString Quotation_Details_Discount_Percent = "Discount_Percent";
    QString Quotation_Details_Discount_Amount = "Discount_Amount";
    QString Quotation_Details_Subtotal = "Subtotal";
    QString Quotation_Details_Project_ID = "Project_ID";
    QString Quotation_Details_Item_Narration = "Item_Narration";
    QString Quotation_Details_Item_Description = "Item_Description";
    QString Quotation_Details_Salesman_ID = "Salesman_ID";
    QString Quotation_Details_Requirement_ItemID ="Requirement_ItemID";
    QString Quotation_Details_Length="Length";
    QString itemVoucherStatus ="ItemVoucherStatus";
    QString itemProductionStatus ="itemProductionStatus";


    QString Proxy_Transaction_Helper_TableName = "Proxy_Transaction_Helper_old";
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


    QString ledgerType = "QUOTATION";


    bool flag = true;
    DatabaseHelper *db;

    int startTransaction();

    int insertQuotation(GeneralVoucherDataObject *obj);

    int updateQuotation(GeneralVoucherDataObject *obj, QString oldID);

    int deleteQuotation(QString oldID);

    int commit();

    int getMaxID();

    GeneralVoucherDataObject getVoucherByVoucherNo(QString vNo);

    QList<GeneralVoucherDataObject> getVoucherListByDate(QDate dateFrom,QDate dateTo);


};



#endif // OLDQUOTATIONDATABASEHELPER_H
