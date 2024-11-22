#ifndef USERGROUPDATAMODEL_H
#define USERGROUPDATAMODEL_H

#include <QString>
#include <QList>

#include "datamodels/hr/uiaccesscontroldatamodel.h"

class UserGroupDataModel
{
public:
    UserGroupDataModel();

    QString Name;
    int _id = -1;

    QList<UiAccessControlDataModel> permissions;


    bool Requirements_View = false;
    bool Requirements_Add = false;
    bool Requirements_Update = false;
    bool Requirements_Delete = false;

    bool Quotes_View = false;
    bool Quotes_Add = false;
    bool Quotes_Update = false;
    bool Quotes_Delete = false;

    bool exportQuote = false;
    bool clientApprove = false;
    bool managerApprove = false;
    bool reviseQuote = false;

    bool DraftsMan_View = false;
    bool DraftsMan_Add = false;
    bool DraftsMan_Update = false;
    bool DraftsMan_Delete = false;

    bool Sales_Orders_View = false;
    bool Sales_Orders_Add = false;
    bool Sales_Orders_Update = false;
    bool Sales_Orders_Delete = false;

    bool Sales_Invoices_View = false;
    bool Sales_Invoices_Add = false;
    bool Sales_Invoices_Update = false;
    bool Sales_Invoices_Delete = false;

    bool Purchase_Orders_View = false;
    bool Purchase_Orders_Add = false;
    bool Purchase_Orders_Update = false;
    bool Purchase_Orders_Delete = false;

    bool Purchase_Invoices_View = false;
    bool Purchase_Invoices_Add = false;
    bool Purchase_Invoices_Update = false;
    bool Purchase_Invoices_Delete = false;

    bool Delivery_Notes_View = false;
    bool Delivery_Notes_Add = false;
    bool Delivery_Notes_Update = false;
    bool Delivery_Notes_Delete = false;

    bool Credit_Notes_View = false;
    bool Credit_Notes_Add = false;
    bool Credit_Notes_Update = false;
    bool Credit_Notes_Delete = false;

    bool Debit_Notes_View = false;
    bool Debit_Notes_Add = false;
    bool Debit_Notes_Update = false;
    bool Debit_Notes_Delete = false;

    bool Receipt_Notes_View = false;
    bool Receipt_Notes_Add = false;
    bool Receipt_Notes_Update = false;
    bool Receipt_Notes_Delete = false;

    bool Journals_View = false;
    bool Journals_Add = false;
    bool Journals_Update = false;
    bool Journals_Delete = false;

    bool Payments_View = false;
    bool Payments_Add = false;
    bool Payments_Update = false;
    bool Payments_Delete = false;

    bool Purchases_View = false;
    bool Purchases_Add = false;
    bool Purchases_Update = false;
    bool Purchases_Delete = false;

    bool Receipts_View = false;
    bool Receipts_Add = false;
    bool Receipts_Update = false;
    bool Receipts_Delete = false;

    bool Stock_Entries_View = false;
    bool Stock_Entries_Add = false;
    bool Stock_Entries_Update = false;
    bool Stock_Entries_Delete = false;

    bool Stock_Journals_View = false;
    bool Stock_Journals_Add = false;
    bool Stock_Journals_Update = false;
    bool Stock_Journals_Delete = false;

    bool Material_In_Out_View = false;
    bool Material_In_Out_Add = false;
    bool Material_In_Out_Update = false;
    bool Material_In_Out_Delete = false;

    bool Godown_Transfer_View = false;
    bool Godown_Transfer_Add = false;
    bool Godown_Transfer_Update = false;
    bool Godown_Transfer_Delete = false;

    bool Account_Groups_View = false;
    bool Account_Groups_Add = false;
    bool Account_Groups_Update = false;
    bool Account_Groups_Delete = false;

    bool Ledgers_View = false;
    bool Ledgers_Add = false;
    bool Ledgers_Update = false;
    bool Ledgers_Delete = false;

    bool Bank_View = false;
    bool Bank_Add = false;
    bool Bank_Update = false;
    bool Bank_Delete = false;

    bool Brands_View = false;
    bool Brands_Add = false;
    bool Brands_Update = false;
    bool Brands_Delete = false;

    bool Godowns_View = false;
    bool Godowns_Add = false;
    bool Godowns_Update = false;
    bool Godowns_Delete = false;

    bool Properties_View = false;
    bool Properties_Add = false;
    bool Properties_Update = false;
    bool Properties_Delete = false;

    bool Sales_Inventory_Groups_View = false;
    bool Sales_Inventory_Groups_Add = false;
    bool Sales_Inventory_Groups_Update = false;
    bool Sales_Inventory_Groups_Delete = false;

    bool Sales_Inventory_Items_View = false;
    bool Sales_Inventory_Items_Add = false;
    bool Sales_Inventory_Items_Update = false;
    bool Sales_Inventory_Items_Delete = false;

    bool UOM_View = false;
    bool UOM_Add = false;
    bool UOM_Update = false;
    bool UOM_Delete = false;

    bool Price_Lists_View = false;
    bool Price_Lists_Add = false;
    bool Price_Lists_Update = false;
    bool Price_Lists_Delete = false;

    bool InventoryMasters = false;
    bool FinanceMasters = false;
    bool InventoryTransactions = false;
    bool FinanceTransactions = false;

    bool HR = false;
    bool Purchases = false;
    bool WareHouse = false;
    bool HelpDesk = false;
    bool Services = false;
    bool Finances = false;
    bool Inventory = false;

    bool Projects = false;
    bool Projects_View = false;
    bool Projects_Add = false;
    bool Projects_Update = false;
    bool Projects_Delete = false;

    bool Doucuments_View = false;
    bool Doucuments_Add = false;
    bool Doucuments_Update = false;
    bool Doucuments_Delete = false;

    bool WorkOrder_View = false;
    bool WorkOrder_Add = false;
    bool WorkOrder_Update = false;
    bool WorkOrder_Delete = false;

    bool Drafts_ApprovalStatus = false;
    bool Drafts_ExtraDocs = false;

    float  MaxDiscountPercent =0;


};

#endif // USERGROUPDATAMODEL_H
