#include "inventoryhelper.h"

InventoryHelper::InventoryHelper()
{
    db = new DatabaseHelper();
}

float InventoryHelper::getClosingStock(QString itemID, QDate date)
{
    float balance;
    SalesInventoryItemDatabaseHelper itemHelper;
    ItemTransactionDatabaseHelper trans;
    QString qStr = "";
    qStr += "SELECT ";
    qStr += itemHelper.Sales_Inventory_OpeningStock ;
    qStr += "-( ";
    qStr = "SELECT ";

    qStr += "SUM("+trans.Transaction_Item_Helper_Cr_Qty+"-"+trans.Transaction_Item_Helper_Dr_Qty +") " ;

    qStr +=" from ";
    qStr += trans.Transaction_Item_Helper_TableName;
    qStr += " WHERE ";
    qStr += trans.Transaction_Item_Helper_ItemID + "=:item";
    qStr += " and ";
    qStr += trans.Transaction_Item_Helper_VoucherDate +"<='"+date.toString("yyyy-MM-dd")+"'";

    qDebug()<<"Closing stock : ";
    qDebug()<<qStr;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);
    query.bindValue(":item",itemID);

    if(query.exec()){
        if(query.first()){
            balance = query.value(0).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }

    return balance;
}

QList<inventoryItemDataModel> InventoryHelper::getClosingStockList(QDate dateFrom, QDate dateTo)
{

    QList<inventoryItemDataModel> itemsList;
    QString qStr = "";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);
    query.bindValue(":dateFrom",dateFrom);
    query.bindValue(":dateTo",dateTo);

    int i=0;
    if(query.exec()){
        while(query.next()){
            inventoryItemDataModel item;
            i=0;
            item.ItemID         = query.value(i++).toString();
            item.ItemName       = query.value(i++).toString();
            item.GroupName      = query.value(i++).toString();
            item.GroupID        = query.value(i++).toString();
            item.ClosingStock   = query.value(i++).toFloat();
            itemsList.append(item);
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return itemsList;
}

QSqlQueryModel *InventoryHelper::getClosingStockListModel(QDate dateFrom, QDate dateTo)
{
    QSqlQueryModel* model = new QSqlQueryModel;
    SalesInventoryItemDatabaseHelper shelper;
    ItemTransactionDatabaseHelper itemHelper;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "";
    qStr += "SELECT " + shelper.Sales_Inventory_ItemName;
    qStr += ", ("+shelper.Sales_Inventory_OpeningStock + "- " ;
    qStr += " (SELECT SUM("+itemHelper.Transaction_Item_Helper_Cr_Qty +"-";
    qStr += itemHelper.Transaction_Item_Helper_Dr_Qty +") FROM ";
    qStr += itemHelper.Transaction_Item_Helper_TableName;
    qStr += " WHERE ";
    qStr += itemHelper.Transaction_Item_Helper_VoucherDate + "<='"+dateTo.toString("yyyy-MM-dd")+"'";
    qStr += " AND ";
    qStr += itemHelper.Transaction_Item_Helper_ItemID + " = INV." + shelper.Sales_Inventory_ItemId +"))";
    qStr += " FROM "+ shelper.Sales_Inventory_Table_Name;
    qStr += " INV";
    qStr +=  " ORDER BY "+shelper.Sales_Inventory_ItemName;

    model->setQuery(qStr,QSqlDatabase::database(DatabaseValues::connectionString));
    return model;

}

QSqlQueryModel *InventoryHelper::getClosingStockListByEndDateModel(QDate dateTo)
{
    QSqlQueryModel* model = new QSqlQueryModel;
    ItemTransactionDatabaseHelper itemHelper;
    SalesInventoryItemDatabaseHelper shelper;

    QString qStr = "";
    qStr += "SELECT " ;
    qStr += shelper.Sales_Inventory_ItemId +", ";
    qStr += shelper.Sales_Inventory_ItemName + " as Items";
    qStr += ", (INV."+shelper.Sales_Inventory_OpeningStock + "- ";
    qStr += " (SELECT SUM("+itemHelper.Transaction_Item_Helper_Cr_Qty +"-";
    qStr += itemHelper.Transaction_Item_Helper_Dr_Qty +") FROM ";
    qStr += itemHelper.Transaction_Item_Helper_TableName;
    qStr += " WHERE ";
    qStr += itemHelper.Transaction_Item_Helper_VoucherDate + "<= '"+dateTo.toString("yyyy-MM-dd")+"'";
    qStr += " AND ";
    qStr += itemHelper.Transaction_Item_Helper_ItemID + " = INV." + shelper.Sales_Inventory_ItemId +")) as Closing";
    qStr += " FROM "+ shelper.Sales_Inventory_Table_Name;
    qStr += " INV";
    qStr +=  " Group BY "+shelper.Sales_Inventory_ItemName;
    qStr +=  " ORDER BY "+shelper.Sales_Inventory_ItemName;

    qDebug()<<qStr;
    model->setQuery(qStr,QSqlDatabase::database(DatabaseValues::connectionString));

    return model;

}

QList<inventoryItemDataModel> InventoryHelper::getClosingStockListByGroup(QString InventoryGroupID, QDate dateFrom, QDate dateTo)
{
    QList<inventoryItemDataModel> itemsList;
    QString qStr = "";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);

    query.bindValue(":id",InventoryGroupID);
    query.bindValue(":dateFrom",dateFrom);
    query.bindValue(":dateTo",dateTo);

    int i=0;
    if(query.exec()){
        while(query.next()){
            inventoryItemDataModel item;
            i=0;

            itemsList.append(item);
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return itemsList;

}

QSqlQueryModel *InventoryHelper::getClosingStockListByGroupModel(QString InventoryGroupID, QDate dateFrom, QDate dateTo)
{
    QSqlQueryModel* model;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "";
    qStr +="SELECT ";

    model->setQuery(query);
    return model;

}

QList<GeneralVoucherDataObject> InventoryHelper::getItemTransactionsList(QString itemID, QDate dateFrom, QDate dateTo)
{
    QList<GeneralVoucherDataObject> itemsList;
    QString qStr = "";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);
    query.bindValue(":id",itemID);
    query.bindValue(":dateFrom",dateFrom);
    query.bindValue(":dateTo",dateTo);

    int i=0;
    if(query.exec()){
        while(query.next()){
            GeneralVoucherDataObject item;
            i=0;

            itemsList.append(item);
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return itemsList;
}

QSqlQueryModel *InventoryHelper::getItemTransactionsListModel(QString itemID, QDate dateFrom, QDate dateTo)
{
    QSqlQueryModel* model;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "";
    qStr +="SELECT ";

    model->setQuery(query);
    return model;

}

QList<inventoryItemDataModel> InventoryHelper::getItemWiseSalesByDate(QDate fromDate, QDate toDate)
{
    QList<inventoryItemDataModel> itemsList;
    QString qStr = "";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);

    query.bindValue(":dateFrom",fromDate);
    query.bindValue(":dateTo",toDate);

    int i=0;
    if(query.exec()){
        while(query.next()){
            inventoryItemDataModel item;
            i=0;

            itemsList.append(item);
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return itemsList;
}

QSqlQueryModel *InventoryHelper::getItemWiseSalesByDateModel(QDateTime fromDate, QDateTime toDate)
{
    salesVoucherDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper itemHelper;
    QSqlQueryModel* model = new QSqlQueryModel;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    qDebug()<<"253";
    QString qStr = "";
    qStr +="SELECT ";
    qStr += "";
    qStr += "(SELECT "+itemHelper.Sales_Inventory_ItemName+" from "+ itemHelper.Sales_Inventory_Table_Name;
    qStr += " invTab where "+itemHelper.Sales_Inventory_ItemId + "=stable."+shelper.Sales_Invoice_Details_Inventory_Item_ID;
    qStr += ") as Item,";
    qStr += "SUM("+shelper.Sales_Invoice_Details_Quantity+") as Quantity" +", ";
    qStr += "CAST( SUM("+shelper.Sales_Invoice_Details_Subtotal+") as DECIMAL(10,2)) as Amount";
    qStr += " From ";
    qStr += shelper.Sales_Invoice_Details_TableName + " stable";
    qStr += " WHERE ";
    qStr += shelper.Sales_Invoice_Details_Voucher_No + " IN (";

    qStr += " SELECT "+shelper.Sales_Invoice_Main_Voucher_No;
    qStr += " FROM " + shelper.Sales_Invoice_Main_TableName;
    qStr += " WHERE "+ shelper.Sales_Invoice_Main_TimeStamp;
    qStr += " >='"+fromDate.toString("yyyy-MM-dd hh:mm:ss")+"'";
    qStr += " and ";
    qStr += shelper.Sales_Invoice_Main_TimeStamp;

    qStr += " <='"+toDate.toString("yyyy-MM-dd hh:mm:ss")+"')";

    qStr += " GROUP BY ";
    qStr += shelper.Sales_Invoice_Details_Inventory_Item_ID;
    qStr += " ORDER BY ";
    qStr += "Item";

    if(query.exec(qStr)){
        qDebug()<<qStr;
    }

    qDebug()<<qStr;

    //    query.prepare(qStr);

    model->setQuery(qStr,QSqlDatabase::database(DatabaseValues::connectionString));
    return model;

}

QSqlQueryModel *InventoryHelper::getItemWiseWaiterWise(QDateTime fromDate, QDateTime toDate,int salesmanID)
{
    salesVoucherDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper itemHelper;
    QSqlQueryModel* model = new QSqlQueryModel;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    qDebug()<<"253";
    QString qStr = "";
    qStr +="SELECT ";
    qStr += "";
    qStr += "(SELECT "+itemHelper.Sales_Inventory_ItemName+" from "+ itemHelper.Sales_Inventory_Table_Name;
    qStr += " invTab where "+itemHelper.Sales_Inventory_ItemId + "=stable."+shelper.Sales_Invoice_Details_Inventory_Item_ID;
    qStr += ") as Item,";
    qStr += "SUM("+shelper.Sales_Invoice_Details_Quantity+") as Quantity" +", ";
    qStr += "SUM("+shelper.Sales_Invoice_Details_Subtotal+") as Amount";
    qStr += " From ";
    qStr += shelper.Sales_Invoice_Details_TableName + " stable";
    qStr += " WHERE ";
    qStr += shelper.Sales_Invoice_Details_Voucher_No + " IN (";

    qStr += " SELECT "+shelper.Sales_Invoice_Main_Voucher_No;
    qStr += " FROM " + shelper.Sales_Invoice_Main_TableName;
    qStr += " WHERE "+ shelper.Sales_Invoice_Main_TimeStamp;
    qStr += " >='"+fromDate.toString("yyyy-MM-dd hh:mm:ss")+"'";
    qStr += " and ";
    qStr += shelper.Sales_Invoice_Main_TimeStamp;

    qStr += " <='"+toDate.toString("yyyy-MM-dd hh:mm:ss");
    qStr += "' and ";
    qStr += shelper.Sales_Invoice_Main_Salesman_ID + "='"+QString::number(salesmanID)+"'";

    qStr += ")";

    qStr += " GROUP BY ";
    qStr += shelper.Sales_Invoice_Details_Inventory_Item_ID;
    qStr += " ORDER BY ";
    qStr += "Item";

    if(query.exec(qStr)){
        qDebug()<<qStr;
    }

    qDebug()<<qStr;

    //    query.prepare(qStr);

    model->setQuery(qStr,QSqlDatabase::database(DatabaseValues::connectionString));
    return model;

}

QSqlQueryModel *InventoryHelper::getReOrderLevelReport(QDate dateTo)
{
    QSqlQueryModel* model = new QSqlQueryModel;
    ItemTransactionDatabaseHelper itemHelper;
    SalesInventoryItemDatabaseHelper shelper;

    QString qStr = "";
    qStr += "SELECT inv.ID,inv.items, inv.closing, inv.level from ";
    qStr += "(";

    qStr += "SELECT " ;
    qStr += shelper.Sales_Inventory_ItemId + " as ID,";
    qStr += shelper.Sales_Inventory_ItemName + " as Items";
    qStr += ", ("+shelper.Sales_Inventory_OpeningStock + "- ";
    qStr += " (SELECT SUM("+itemHelper.Transaction_Item_Helper_Cr_Qty +"-";
    qStr += itemHelper.Transaction_Item_Helper_Dr_Qty +") FROM ";
    qStr += itemHelper.Transaction_Item_Helper_TableName;
    qStr += " WHERE ";
    qStr += itemHelper.Transaction_Item_Helper_VoucherDate + "<= '"+dateTo.toString("yyyy-MM-dd")+"'";
    qStr += " AND ";
    qStr += itemHelper.Transaction_Item_Helper_ItemID + " =";
    qStr += shelper.Sales_Inventory_Table_Name+"." + shelper.Sales_Inventory_ItemId +")) as Closing ,";
    qStr += shelper.Sales_Inventory_ReorderLevel + " as Level ";
    qStr += " FROM "+ shelper.Sales_Inventory_Table_Name;
    qStr += ") INV";
    qStr += " WHERE ";
    qStr += "INV.Level > INV.Closing ";

    qStr +=  " ORDER BY INV.Items";

    qDebug()<<qStr;
    model->setQuery(qStr,QSqlDatabase::database(DatabaseValues::connectionString));

    return model;

}

QSqlQueryModel *InventoryHelper::getRestClosingStock(QDate dfrom, QDate dto)
{
    QSqlQueryModel* model = new QSqlQueryModel;
    ItemTransactionDatabaseHelper itemHelper;
    SalesInventoryItemDatabaseHelper shelper;
    TransactionHelper trans;

    QString qStr = "";


    qDebug()<<qStr;
    model->setQuery(qStr,QSqlDatabase::database(DatabaseValues::connectionString));

    return model;

}

QSqlQueryModel *InventoryHelper::getGroupWiseItemSale(QDateTime fromDate, QDateTime toDate)
{
    QSqlQueryModel* model = new QSqlQueryModel;
    ItemTransactionDatabaseHelper itemHelper;
    salesVoucherDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper items;
    SalesInventoryGroupsDatabaseHelper gpHelper;
    TransactionHelper trans;

    QString qStr = "";



    qStr = "SELECT ";

    qStr += "(SELECT "+gpHelper.Sales_Inventory_Group_GroupName;
    qStr += " FROM " +gpHelper.Sales_Inventory_Group_TableName;
    qStr += " where "+gpHelper.Sales_Inventory_Group_GroupId +" in ";
    qStr += "(SELECT "+items.Sales_Inventory_GroupId + " FROM " +items.Sales_Inventory_Table_Name +" Inv";
    qStr += " where "+ items.Sales_Inventory_ItemId +"= sal."+shelper.Sales_Invoice_Details_Inventory_Item_ID;
    qStr += " ))";
    qStr += " AS GroupName,";

    qStr += " SUM("+shelper.Sales_Invoice_Details_Quantity+")  as Qty,";
    qStr += " SUM("+shelper.Sales_Invoice_Details_Subtotal+")  as Amount";
    qStr += " FROM "+shelper.Sales_Invoice_Details_TableName;
    qStr += " SAL WHERE "+shelper.Sales_Invoice_Details_Voucher_No + " IN (";
    qStr += " SELECT "+shelper.Sales_Invoice_Main_Voucher_No;
    qStr += " FROM " + shelper.Sales_Invoice_Main_TableName;
    qStr += " WHERE "+ shelper.Sales_Invoice_Main_TimeStamp;
    qStr += " >='"+fromDate.toString("yyyy-MM-dd hh:mm:ss")+"'";
    qStr += " and ";
    qStr += shelper.Sales_Invoice_Main_TimeStamp;

    qStr += " <='"+toDate.toString("yyyy-MM-dd hh:mm:ss")+"')";
    qStr += " GROUP BY  GroupName";
    qStr += " ORDER BY  GroupName";

    qDebug()<<qStr;
    model->setQuery(qStr,QSqlDatabase::database(DatabaseValues::connectionString));

    return model;

}

QSqlQueryModel *InventoryHelper::getClosingForRestaurant(QDateTime fromDate, QDateTime toDate)
{
    QSqlQueryModel* model = new QSqlQueryModel;
    ItemTransactionDatabaseHelper itemHelper;
    salesVoucherDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper items;
    SalesInventoryGroupsDatabaseHelper gpHelper;
    TransactionHelper trans;

    QString qStr = "";

    qStr += " SELECT item_name as item_name, opening_stock,  inward_qty, issue_qty,  (opening_stock+inward_qty-issue_qty) as closing_stock ";
    qStr += " from  ";
    qStr += " ( ";
    qStr += " SELECT sales_inventory_items.item_name, coalesce(transaction_item_helper.inward_qty,0) as inward_qty,  ";
    qStr += " coalesce(sales_invoice_details.sales_qty*(ingredients.purchase_item_qty/ingredients.sales_item_qty),0) as issue_qty, ";
    qStr += " Opening.opening_stock as opening_stock, sales_invoice_details.inventory_item_id as sales_item_name ";
    qStr += " from sales_inventory_items  ";
    qStr += " LEFT OUTER JOIN  ";
    qStr += " (select sum(Dr_Qty)-sum(Cr_Qty) as inward_qty, ItemID from transaction_item_helper ";
    qStr += " where substr(VoucherDate,1,10) >= '"+fromDate.toString("yyyy-MM-dd")+"' and substr(VoucherDate,1,10) <= '"+toDate.toString("yyyy-MM-dd")+"' ";
    qStr += " group by ItemID) transaction_item_helper ";
    qStr += " ON transaction_item_helper.ItemID = sales_inventory_items.item_id ";
    qStr += " LEFT OUTER JOIN  ";
    qStr += " (select item_name,  opening_stock from  ";
    qStr += " (SELECT item_name, (opening_balance+opening_stock-sum(A_issue_qty)) as opening_stock ";
    qStr += " from  ";
    qStr += " ( ";
    qStr += " SELECT sales_inventory_items.item_id item_id, sales_inventory_items.item_name, coalesce(sales_inventory_items.opening_stock,0) as opening_balance, ";
    qStr += " coalesce(sales_invoice_details.sales_qty*(ingredients.purchase_item_qty/ingredients.sales_item_qty),0) as A_issue_qty, ";
    qStr += " coalesce(Opening.stock,0) as opening_stock, sales_invoice_details.inventory_item_id as sales_item_name ";
    qStr += " from sales_inventory_items  ";
    qStr += " LEFT OUTER JOIN  ";
    qStr += " (select sum(Dr_Qty)-sum(Cr_Qty) as stock, ItemID from transaction_item_helper ";
    qStr += " where substr(VoucherDate,1,10) < '"+fromDate.toString("yyyy-MM-dd")+"' ";
    qStr += " group by ItemID) Opening ";
    qStr += " ON Opening.ItemID = sales_inventory_items.item_id ";
    qStr += "  LEFT OUTER JOIN ingredients ";
    qStr += "  ON sales_inventory_items.item_id = ingredients.purchase_item  ";
    qStr += "  LEFT OUTER JOIN   ";
    qStr += " (select sales_invoice_details.inventory_item_id, sum(sales_invoice_details.quantity) sales_qty ";
    qStr += " from  ";
    qStr += " ingredients LEFT OUTER JOIN sales_invoice_details ";
    qStr += " ON ingredients.sales_item = sales_invoice_details.inventory_item_id ";
    qStr += " LEFT OUTER JOIN sales_invoice_main  ";
    qStr += " ON sales_invoice_details.voucher_no = sales_invoice_main.Voucher_No ";
    qStr += " WHERE ((ingredients.TAKEAWAY = 1 and sales_invoice_main.mode_of_service = '3')  ";
    qStr += " or (ingredients.TAKEAWAY not in (1)) or ingredients.TAKEAWAY is null) ";
    qStr += " AND  sales_invoice_details.voucher_date  < '"+fromDate.toString("yyyy-MM-dd")+"'  ";
    qStr += " group by inventory_item_id ";
    qStr += " )sales_invoice_details  ";
    qStr += " ON ";
    qStr += " sales_invoice_details.inventory_item_id = ingredients.sales_item ";
    qStr += " )A ";
    qStr += " group by item_id ";
    qStr += " ) B ";
    qStr += " where opening_stock <> '0' ";
    qStr += " group by item_name ";
    qStr += " order by item_name ";
    qStr += " ) as Opening ";
    qStr += "  ON Opening.item_name = sales_inventory_items.item_name ";
    qStr += "  LEFT OUTER JOIN ingredients ";
    qStr += "  ON sales_inventory_items.item_id = ingredients.purchase_item  ";
    qStr += " LEFT OUTER JOIN   ";
    qStr += " (select sales_invoice_details.inventory_item_id, sum(sales_invoice_details.quantity) sales_qty ";
    qStr += " from  ";
    qStr += " ingredients LEFT OUTER JOIN sales_invoice_details ";
    qStr += " ON ingredients.sales_item = sales_invoice_details.inventory_item_id ";
    qStr += " LEFT OUTER JOIN sales_invoice_main  ";
    qStr += " ON sales_invoice_details.voucher_no = sales_invoice_main.Voucher_No ";
    qStr += " WHERE ((ingredients.TAKEAWAY = 1 and sales_invoice_main.mode_of_service = '3')  ";
    qStr += " or (ingredients.TAKEAWAY not in (1)) or ingredients.TAKEAWAY is null) ";
    qStr += " AND  sales_invoice_details.voucher_date  >= '"+fromDate.toString("yyyy-MM-dd")+"' and sales_invoice_details.voucher_date <= '"+toDate.toString("yyyy-MM-dd")+"' ";
    qStr += " group by inventory_item_id ";
    qStr += " )sales_invoice_details ";
    qStr += " ON ";
    qStr += " sales_invoice_details.inventory_item_id = ingredients.sales_item ";
    qStr += " WHERE sales_inventory_items.isPurchaseItem = 1 ";
    qStr += " )A ";
    qStr += " group by item_name ";






    qDebug()<<qStr;
    model->setQuery(qStr,QSqlDatabase::database(DatabaseValues::connectionString));

    return model;

}




