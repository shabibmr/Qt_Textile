#ifndef USERGROUPDATABASEHELPER_H
#define USERGROUPDATABASEHELPER_H


#include "datamodels/hr/usergroupdatamodel.h"
#include "database/databasehelper/databasehelper.h"

#include <QMap>
#include <QDebug>
class UserGroupDatabaseHelper
{
public:
    UserGroupDatabaseHelper(DatabaseHelper *db = new DatabaseHelper());
    ~UserGroupDatabaseHelper();
    QString User_Group_Permissions_TableName = "User_Group_Permissions";

    QString User_Group_Permissions_id = "_id";
    QString User_Group_Permissions_Name = "Name";
    QString User_Group_Permissions_Requirements_View = "Requirements_View";
    QString User_Group_Permissions_Requirements_Add = "Requirements_Add";
    QString User_Group_Permissions_Requirements_Update = "Requirements_Update";
    QString User_Group_Permissions_Requirements_Delete = "Requirements_Delete";
    QString User_Group_Permissions_Quotes_View = "Quotes_View";
    QString User_Group_Permissions_Quotes_Add = "Quotes_Add";
    QString User_Group_Permissions_Quotes_Update = "Quotes_Update";
    QString User_Group_Permissions_Quotes_Delete = "Quotes_Delete";


    QString User_Group_Permissions_exportQuote="exportQuote";
    QString User_Group_Permissions_clientApprove="clientApprove";
    QString User_Group_Permissions_managerApprove="managerApprove";
    QString User_Group_Permissions_reviseQuote="reviseQuote";


    QString User_Group_Permissions_DraftsMan_View="DraftsMan_View";
    QString User_Group_Permissions_DraftsMan_Add="DraftsMan_Add";
    QString User_Group_Permissions_DraftsMan_Update="DraftsMan_Update";
    QString User_Group_Permissions_DraftsMan_Delete="DraftsMan_Delete";

    QString User_Group_Permissions_Sales_Orders_View = "Sales_Orders_View";
    QString User_Group_Permissions_Sales_Orders_Add = "Sales_Orders_Add";
    QString User_Group_Permissions_Sales_Orders_Update = "Sales_Orders_Update";
    QString User_Group_Permissions_Sales_Orders_Delete = "Sales_Orders_Delete";
    QString User_Group_Permissions_Sales_Invoices_View = "Sales_Invoices_View";
    QString User_Group_Permissions_Sales_Invoices_Add = "Sales_Invoices_Add";
    QString User_Group_Permissions_Sales_Invoices_Update = "Sales_Invoices_Update";
    QString User_Group_Permissions_Sales_Invoices_Delete = "Sales_Invoices_Delete";
    QString User_Group_Permissions_Purchase_Orders_View = "Purchase_Orders_View";
    QString User_Group_Permissions_Purchase_Orders_Add = "Purchase_Orders_Add";
    QString User_Group_Permissions_Purchase_Orders_Update = "Purchase_Orders_Update";
    QString User_Group_Permissions_Purchase_Orders_Delete = "Purchase_Orders_Delete";
    QString User_Group_Permissions_Purchase_Invoices_View = "Purchase_Invoices_View";
    QString User_Group_Permissions_Purchase_Invoices_Add = "Purchase_Invoices_Add";
    QString User_Group_Permissions_Purchase_Invoices_Update = "Purchase_Invoices_Update";
    QString User_Group_Permissions_Purchase_Invoices_Delete = "Purchase_Invoices_Delete";
    QString User_Group_Permissions_Delivery_Notes_View = "Delivery_Notes_View";
    QString User_Group_Permissions_Delivery_Notes_Add = "Delivery_Notes_Add";
    QString User_Group_Permissions_Delivery_Notes_Update = "Delivery_Notes_Update";
    QString User_Group_Permissions_Delivery_Notes_Delete = "Delivery_Notes_Delete";
    QString User_Group_Permissions_Credit_Notes_View = "Credit_Notes_View";
    QString User_Group_Permissions_Credit_Notes_Add = "Credit_Notes_Add";
    QString User_Group_Permissions_Credit_Notes_Update = "Credit_Notes_Update";
    QString User_Group_Permissions_Credit_Notes_Delete = "Credit_Notes_Delete";
    QString User_Group_Permissions_Debit_Notes_View = "Debit_Notes_View";
    QString User_Group_Permissions_Debit_Notes_Add = "Debit_Notes_Add";
    QString User_Group_Permissions_Debit_Notes_Update = "Debit_Notes_Update";
    QString User_Group_Permissions_Debit_Notes_Delete = "Debit_Notes_Delete";
    QString User_Group_Permissions_Receipt_Notes_View = "Receipt_Notes_View";
    QString User_Group_Permissions_Receipt_Notes_Add = "Receipt_Notes_Add";
    QString User_Group_Permissions_Receipt_Notes_Update = "Receipt_Notes_Update";
    QString User_Group_Permissions_Receipt_Notes_Delete = "Receipt_Notes_Delete";
    QString User_Group_Permissions_Journals_View = "Journals_View";
    QString User_Group_Permissions_Journals_Add = "Journals_Add";
    QString User_Group_Permissions_Journals_Update = "Journals_Update";
    QString User_Group_Permissions_Journals_Delete = "Journals_Delete";
    QString User_Group_Permissions_Payments_View = "Payments_View";
    QString User_Group_Permissions_Payments_Add = "Payments_Add";
    QString User_Group_Permissions_Payments_Update = "Payments_Update";
    QString User_Group_Permissions_Payments_Delete = "Payments_Delete";
    QString User_Group_Permissions_Purchases_View = "Purchases_View";
    QString User_Group_Permissions_Purchases_Add = "Purchases_Add";
    QString User_Group_Permissions_Purchases_Update = "Purchases_Update";
    QString User_Group_Permissions_Purchases_Delete = "Purchases_Delete";
    QString User_Group_Permissions_Receipts_View = "Receipts_View";
    QString User_Group_Permissions_Receipts_Add = "Receipts_Add";
    QString User_Group_Permissions_Receipts_Update = "Receipts_Update";
    QString User_Group_Permissions_Receipts_Delete = "Receipts_Delete";
    QString User_Group_Permissions_Stock_Entries_View = "Stock_Entries_View";
    QString User_Group_Permissions_Stock_Entries_Add = "Stock_Entries_Add";
    QString User_Group_Permissions_Stock_Entries_Update = "Stock_Entries_Update";
    QString User_Group_Permissions_Stock_Entries_Delete = "Stock_Entries_Delete";
    QString User_Group_Permissions_Stock_Journals_View = "Stock_Journals_View";
    QString User_Group_Permissions_Stock_Journals_Add = "Stock_Journals_Add";
    QString User_Group_Permissions_Stock_Journals_Update = "Stock_Journals_Update";
    QString User_Group_Permissions_Stock_Journals_Delete = "Stock_Journals_Delete";
    QString User_Group_Permissions_Material_In_Out_View = "Material_In_Out_View";
    QString User_Group_Permissions_Material_In_Out_Add = "Material_In_Out_Add";
    QString User_Group_Permissions_Material_In_Out_Update = "Material_In_Out_Update";
    QString User_Group_Permissions_Material_In_Out_Delete = "Material_In_Out_Delete";
    QString User_Group_Permissions_Godown_Transfer_View = "Godown_Transfer_View";
    QString User_Group_Permissions_Godown_Transfer_Add = "Godown_Transfer_Add";
    QString User_Group_Permissions_Godown_Transfer_Update = "Godown_Transfer_Update";
    QString User_Group_Permissions_Godown_Transfer_Delete = "Godown_Transfer_Delete";
    QString User_Group_Permissions_Account_Groups_View = "Account_Groups_View";
    QString User_Group_Permissions_Account_Groups_Add = "Account_Groups_Add";
    QString User_Group_Permissions_Account_Groups_Update = "Account_Groups_Update";
    QString User_Group_Permissions_Account_Groups_Delete = "Account_Groups_Delete";
    QString User_Group_Permissions_Ledgers_View = "Ledgers_View";
    QString User_Group_Permissions_Ledgers_Add = "Ledgers_Add";
    QString User_Group_Permissions_Ledgers_Update = "Ledgers_Update";
    QString User_Group_Permissions_Ledgers_Delete = "Ledgers_Delete";
    QString User_Group_Permissions_Bank_View = "Bank_View";
    QString User_Group_Permissions_Bank_Add = "Bank_Add";
    QString User_Group_Permissions_Bank_Update = "Bank_Update";
    QString User_Group_Permissions_Bank_Delete = "Bank_Delete";
    QString User_Group_Permissions_Brands_View = "Brands_View";
    QString User_Group_Permissions_Brands_Add = "Brands_Add";
    QString User_Group_Permissions_Brands_Update = "Brands_Update";
    QString User_Group_Permissions_Brands_Delete = "Brands_Delete";
    QString User_Group_Permissions_Godowns_View = "Godowns_View";
    QString User_Group_Permissions_Godowns_Add = "Godowns_Add";
    QString User_Group_Permissions_Godowns_Update = "Godowns_Update";
    QString User_Group_Permissions_Godowns_Delete = "Godowns_Delete";
    QString User_Group_Permissions_Properties_View = "Properties_View";
    QString User_Group_Permissions_Properties_Add = "Properties_Add";
    QString User_Group_Permissions_Properties_Update = "Properties_Update";
    QString User_Group_Permissions_Properties_Delete = "Properties_Delete";
    QString User_Group_Permissions_Sales_Inventory_Groups_View = "Sales_Inventory_Groups_View";
    QString User_Group_Permissions_Sales_Inventory_Groups_Add = "Sales_Inventory_Groups_Add";
    QString User_Group_Permissions_Sales_Inventory_Groups_Update = "Sales_Inventory_Groups_Update";
    QString User_Group_Permissions_Sales_Inventory_Groups_Delete = "Sales_Inventory_Groups_Delete";
    QString User_Group_Permissions_Sales_Inventory_Items_View = "Sales_Inventory_Items_View";
    QString User_Group_Permissions_Sales_Inventory_Items_Add = "Sales_Inventory_Items_Add";
    QString User_Group_Permissions_Sales_Inventory_Items_Update = "Sales_Inventory_Items_Update";
    QString User_Group_Permissions_Sales_Inventory_Items_Delete = "Sales_Inventory_Items_Delete";
    QString User_Group_Permissions_UOM_View = "UOM_View";
    QString User_Group_Permissions_UOM_Add = "UOM_Add";
    QString User_Group_Permissions_UOM_Update = "UOM_Update";
    QString User_Group_Permissions_UOM_Delete = "UOM_Delete";
    QString User_Group_Permissions_Price_Lists_View = "Price_Lists_View";
    QString User_Group_Permissions_Price_Lists_Add = "Price_Lists_Add";
    QString User_Group_Permissions_Price_Lists_Update = "Price_Lists_Update";
    QString User_Group_Permissions_Price_Lists_Delete = "Price_Lists_Delete";

    QString User_Group_Permissions_InventoryMasters="InventoryMasters";
    QString User_Group_Permissions_FinanceMasters="FinanceMasters";
    QString User_Group_Permissions_InventoryTransactions="InventoryTransactions";
    QString User_Group_Permissions_FinanceTransactions="FinanceTransactions";

    QString User_Group_Permissions_HR="HR";
    QString User_Group_Permissions_Purchases="Purchases";
    QString User_Group_Permissions_WareHouse="WareHouse";
    QString User_Group_Permissions_HelpDesk="HelpDesk";
    QString User_Group_Permissions_Services="Services";
    QString User_Group_Permissions_Finances="Finances";
    QString User_Group_Permissions_Inventory="Inventory";

    QString User_Group_Permissions_Projects="Projects";
    QString User_Group_Permissions_Projects_View="Projects_View";
    QString User_Group_Permissions_Projects_Add="Projects_Add";
    QString User_Group_Permissions_Projects_Update="Projects_Update";
    QString User_Group_Permissions_Projects_Delete="Projects_Delete";

    QString User_Group_Permissions_Doucuments_View="Doucuments_View";
    QString User_Group_Permissions_Doucuments_Add="Doucuments_Add";
    QString User_Group_Permissions_Doucuments_Update="Doucuments_Update";
    QString User_Group_Permissions_Doucuments_Delete="Doucuments_Delete";

    QString User_Group_Permissions_WorkOrder_View="WorkOrder_View";
    QString User_Group_Permissions_WorkOrder_Add="WorkOrder_Add";
    QString User_Group_Permissions_WorkOrder_Update="WorkOrder_Update";
    QString User_Group_Permissions_WorkOrder_Delete="WorkOrder_Delete";

    QString User_Group_Permissions_Drafts_ExtraDocs="DraftsExtraDocs";
    QString User_Group_Permissions_Drafts_Approval_Status="DraftStatus";

    QString User_Group_Permissions_MaxDiscountPercent = "MaxDiscountPercent";


    QString User_Group_Ui_Permissions_TableName = "User_Access_Permissions";
    QString User_Group_Ui_Permissions_ID = "_id";
    QString User_Group_Ui_Permissions_UserGroupId = "User_Group_Id";
    QString User_Group_Ui_Permissions_UiId = "Ui_Id";
    QString User_Group_Ui_Permissions_Create = "Allow_Create";
    QString User_Group_Ui_Permissions_Read = "Allow_Read";
    QString User_Group_Ui_Permissions_Update = "Allow_Update";
    QString User_Group_Ui_Permissions_Delete = "Allow_Delete";
    QString User_Group_Ui_Permissions_Share = "Allow_Share";
    QString User_Group_Ui_Permissions_Extras = "Extras";



    bool flag = true;
    DatabaseHelper *db;

    int startTransaction();

    int insertUserGroup(UserGroupDataModel *obj);

    int updateUserGroup(UserGroupDataModel *obj, int oldID);

    int deleteUserGroup(int oldID);

    int commit();

    int getMaxID();

    QMap<int,QString> getUserGroups();

    UserGroupDataModel getUserGroup(int id);

    QString getUserGroupIDByName(QString groupName);

    UserGroupDataModel *getUserGroupById(int id);

    UiAccessControlDataModel *getPermissionsByUserGroupAndUi(int usergroup, int ui_id);
    UiAccessControlDataModel *getPermissionsByUserGroupAndVoucherType(int usergroup, QString voucherType);
    QString getUserGroupNameById(int id);
};

#endif // USERGROUPDATABASEHELPER_H
