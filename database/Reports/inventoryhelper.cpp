#include "inventoryhelper.h"

#include <database/Settings/ConfigurationSettingsDatabaseHelper.h>

InventoryHelper::InventoryHelper()
{
    db = new DatabaseHelper();
}

float InventoryHelper::getClosingStockofItemByDate(QString itemID, QDate date)
{
    float balance=0;
    SalesInventoryItemDatabaseHelper itemHelper;
    ItemTransactionDatabaseHelper trans;
    QString qStr = "";
    qStr = " select round(";
    qStr += " coalesce(" + itemHelper.Sales_Inventory_OpeningStock + ",0) - ";
    qStr += " coalesce(sum(trans." + trans.Transaction_Item_Helper_Cr_Qty + " ),0)  + ";
    qStr += " coalesce(sum(trans." + trans.Transaction_Item_Helper_Dr_Qty + " ),0),3) ";
    qStr += " from " + itemHelper.Sales_Inventory_Table_Name + " item left join " ;
    qStr +=  trans.Transaction_Item_Helper_TableName + " trans on item." ;
    qStr += itemHelper.Sales_Inventory_ItemId + " = trans." + trans.Transaction_Item_Helper_ItemID;
    qStr += " and trans." + trans.Transaction_Item_Helper_VoucherDate + " <='"+date.toString("yyyy-MM-dd")+"'";
    qStr += " where item.item_id = '" + itemID + "'";

    qDebug()<<"Closing stock : ";
    qDebug()<<qStr;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);
    //    query.bindValue(":item",itemID);

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

QSqlQueryModel *InventoryHelper::getIngredientsByWorkOrders(QStringList voucherList)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString vNoList ;
    for(QString vNo:voucherList)
        vNoList += "'" + vNo + "',";
    vNoList.truncate(vNoList.lastIndexOf(QChar('\'')));

    WorkOrderDatabaseHelper* soHelper = new WorkOrderDatabaseHelper;
    SalesInventoryItemDatabaseHelper *itemHelper = new SalesInventoryItemDatabaseHelper;
    BillOfMaterialsIngredientsDataBaseHelper *bomHelper = new BillOfMaterialsIngredientsDataBaseHelper;

    QString qStr = "SELECT (SELECT " + itemHelper->Sales_Inventory_ItemName + " FROM ";
    qStr += itemHelper->Sales_Inventory_Table_Name + " WHERE " + itemHelper->Sales_Inventory_ItemId + " = purchase_item) as Item,";
    qStr += " round(sum(qty*purch_qty),3) as Quantity, round(sum(price*purch_qty),3) as price from ( ";
    qStr += " SELECT " + soHelper->WorkOrder_itemID + ", SUM(" +  soHelper->WorkOrder_qty + ") qty,";
    qStr += " SUM(" + soHelper->WorkOrder_Price + ") price  , ";
    qStr += " COALESCE(ing." + bomHelper->Bill_Of_Materials_Ingredients_Purchase_Item_Qty + ",1) purch_qty," ;
    qStr += bomHelper->Bill_Of_Materials_Ingredients_Purchase_Item + " FROM " +  soHelper->WorkOrder_tableName + " wo ";
    qStr += " left outer join " + bomHelper->Bill_Of_Materials_Ingredients_TableName + " ing ";
    qStr += " on wo." + soHelper->WorkOrder_itemID + " = ing." + bomHelper->Bill_Of_Materials_Ingredients_Sales_Item;
    qStr += " where " +  soHelper->WorkOrder_WorkOrder_id + " in (" + vNoList + "') ";
    qStr += " group by " + soHelper->WorkOrder_itemID + " )X group by " + bomHelper->Bill_Of_Materials_Ingredients_Purchase_Item;

    qDebug()<<qStr;

    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}

QSqlQueryModel *InventoryHelper::getItemWiseSalesByDateModel(QDateTime fromDate, QDateTime toDate)
{
    salesVoucherDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper itemHelper;

    QString qStr = "";

    qStr = " SELECT INV." + itemHelper.Sales_Inventory_ItemName + " as ITEM,";
    qStr += " SALES." + shelper.Sales_Invoice_Details_Quantity + ", ";
    qStr += " SALES." + shelper.Sales_Invoice_Details_Subtotal + " from " ;
    qStr += itemHelper.Sales_Inventory_Table_Name + " as INV LEFT outer JOIN ";
    qStr += " (SELECT " + shelper.Sales_Invoice_Details_Inventory_Item_ID + ",SUM(SALES." ;
    qStr += shelper.Sales_Invoice_Details_Quantity + " ) as Quantity, ";
    qStr += " SUM(SALES." + shelper.Sales_Invoice_Details_Subtotal + " )  as SubTotal ";
    qStr += " from " + shelper.Sales_Invoice_Details_TableName + " as SALES ";
    qStr += " JOIN " + shelper.Sales_Invoice_Main_TableName + " main ";
    qStr += " ON SALES." + shelper.Sales_Invoice_Details_Voucher_No + " = main." + shelper.Sales_Invoice_Main_Voucher_No;
    qStr += " WHERE str_to_Date(concat(main." + shelper.Sales_Invoice_Main_Voucher_Date ;
    qStr += ", substr(" + shelper.Sales_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;
    qStr += " >= "+ db->getDateTimeString(fromDate)+" ";
    qStr += " AND str_to_Date(concat(main." + shelper.Sales_Invoice_Main_Voucher_Date ;
    qStr += ", substr(" + shelper.Sales_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;
    qStr += " <= "+ db->getDateTimeString(toDate)+" ";
    qStr += " group by SALES." + shelper.Sales_Invoice_Details_Inventory_Item_ID + " ) as SALES on ";
    qStr += " INV." + itemHelper.Sales_Inventory_ItemId + "=SALES." + shelper.Sales_Invoice_Details_Inventory_Item_ID;
    qStr += " where COALESCE(SALES." + shelper.Sales_Invoice_Details_Quantity + ",0)>0 ";

    qDebug()<<qStr;
    return DatabaseHelper::getQueryModel(qStr);

}

QSqlQueryModel *InventoryHelper::getGroupwiseMovementByLedger(QString groupID, QString ledgerID,
                                                             QDate fromDate, QDate toDate)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    TransactionHelper *t= new TransactionHelper();
    salesVoucherDatabaseHelper shelper;


    QString qStr =" select Ind,ID,itemGrp.group_name as Name,";
    qStr += " Sum(Cr) as 'Outward \n Qty', Sum(CrAmt) 'Outward \n Amt', Sum(Dr) as 'Inward \n Qty', Sum(DrAmt) 'Inward \n Amt' FROM (";
    qStr += " SELECT TRANS.ItemID,  Inv.Item_Name, COALESCE(CR_QTY,0) AS Cr, ";
    qStr += " case when COALESCE(CR_Amount,0) = 0 then (CR_QTY * INV.PRICE) else COALESCE(CR_Amount,0) end AS 'CrAmt',";
    qStr += " 0 AS Dr , 0 AS 'DrAmt' ";
    qStr += " FROM TRANSACTION_ITEM_HELPER TRANS JOIN SALES_INVENTORY_ITEMS INV ";
    qStr += " ON ITEMID = ITEM_ID ";
    qStr += " WHERE (To_Ledger like '"+ ledgerID + "')";
    qStr += " AND TRANS.VOUCHERDATE >= '" + fromDate.toString("yyyy-MM-dd")+ "'";
    qStr += " AND TRANS.VOUCHERDATE <= '" + toDate.toString("yyyy-MM-dd")+ "'";
    qStr += " UNION ALL";
    qStr += " SELECT TRANS.ItemID, Inv.Item_Name, 0 AS Cr,0 AS 'CrAmt',";
    qStr += " COALESCE((DR_QTY),0) AS Dr ,";
    qStr += " case when COALESCE(DR_Amount,0) = 0 then (DR_QTY * INV.PRICE) else COALESCE(DR_Amount,0) end AS 'DrAmt'";
    qStr += " FROM TRANSACTION_ITEM_HELPER TRANS JOIN SALES_INVENTORY_ITEMS INV ";
    qStr += " ON ITEMID = ITEM_ID ";
    qStr += " WHERE ( From_Ledger like '" + ledgerID + "')";
    qStr += " AND TRANS.VOUCHERDATE >= '" + fromDate.toString("yyyy-MM-dd")+ "'";
    qStr += " AND TRANS.VOUCHERDATE <= '" + toDate.toString("yyyy-MM-dd")+ "'";
    qStr += " UNION ALL ";
    qStr += " SELECT TRANS.ItemID,  Inv.Item_Name, COALESCE(CR_QTY,0) AS Cr, ";
    qStr += " case when COALESCE(CR_Amount,0) = 0 then (CR_QTY * INV.PRICE) else COALESCE(CR_Amount,0) end AS 'CrAmt',";
    qStr += " 0 AS Dr , 0 AS 'DrAmt' ";
    qStr += " FROM TRANSACTION_BOM_HELPER TRANS JOIN SALES_INVENTORY_ITEMS INV ";
    qStr += " ON ITEMID = ITEM_ID ";
    qStr += " WHERE (To_Ledger like '"+ ledgerID + "')";
    qStr += " AND TRANS.VOUCHERDATE >= '" + fromDate.toString("yyyy-MM-dd")+ "'";
    qStr += " AND TRANS.VOUCHERDATE <= '" + toDate.toString("yyyy-MM-dd")+ "'";
    qStr += " UNION ALL";
    qStr += " SELECT TRANS.ItemID, Inv.Item_Name, 0 AS Cr,0 AS 'CrAmt',";
    qStr += " COALESCE((DR_QTY),0) AS Dr ,";
    qStr += " case when COALESCE(DR_Amount,0) = 0 then (DR_QTY * INV.PRICE) else COALESCE(DR_Amount,0) end AS 'DrAmt'";
    qStr += " FROM TRANSACTION_BOM_HELPER TRANS JOIN SALES_INVENTORY_ITEMS INV ";
    qStr += " ON ITEMID = ITEM_ID ";
    qStr += " WHERE ( From_Ledger like '" + ledgerID + "')";
    qStr += " AND TRANS.VOUCHERDATE >= '" + fromDate.toString("yyyy-MM-dd")+ "'";
    qStr += " AND TRANS.VOUCHERDATE <= '" + toDate.toString("yyyy-MM-dd")+ "'";
    qStr += " ) trans ";
    qStr += " inner join ";
    if(groupID !="0"){
        qStr += " (select ItemGrp1.group_name, Item.item_id, 'G' as Ind,ItemGrp1.group_id as ID from ";
        qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
        qStr += " inner join ";
        qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
        qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
        qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  grp";
        qStr += "  on item.group_id = grp.group_id";
        qStr += "  union all ";
        qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
        qStr += "  from      " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  p";
        qStr += "  inner join cte ";
        qStr += "  on p.group_id = cte.parent_id )";
        qStr += " select cte.group_name, cte.item_id, cte.group_id ";
        qStr += " from cte where cte.parent_id = '"+ groupID +"'";
        qStr += " ) itemGrp1 ";
        qStr += " on itemGrp1.item_id = item.item_id ";
        qStr += " union ";
        qStr += " select Item2.Item_name, Item2.Item_id, 'I' as ind,Item2.Item_id as ID  from ";
        qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
        qStr += " where Item2.group_id = '"+ groupID +"'";
        qStr += " ) itemGrp ";
    }
    else {
        qStr += "(SELECT (SELECT group_Name from " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName;
        qStr += " grp WHERE group_id = inv.group_id) as group_name, inv.item_id, 'G' as Ind, inv.group_id as ID from ";
        qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" inv) itemGrp";
    }
    qStr += " on trans.itemid = itemGrp.item_id ";
    qStr += " GROUP BY itemGrp.group_name";


    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}



QSqlQueryModel *InventoryHelper::getItemwiseMovementByLedgerAndItemGroup(QString groupID, QString ledgerID,
                                                             QDate fromDate, QDate toDate)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    TransactionHelper *t= new TransactionHelper();
    salesVoucherDatabaseHelper shelper;


    QString qStr =" select Ind,itemGrp.item_id ,";
    qStr += " (select item_name FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where item_id = itemGrp.item_id  ) as Item,";
    qStr += " Sum(Cr) as 'Outward \n Qty', Sum(CrAmt) 'Outward \n Amt', Sum(Dr) as 'Inward \n Qty', Sum(DrAmt) 'Inward \n Amt' FROM (";
    qStr += " SELECT TRANS.ItemID,  Inv.Item_Name, COALESCE(CR_QTY,0) AS Cr, ";
    qStr += " case when COALESCE(CR_Amount,0) = 0 then (CR_QTY * INV.PRICE) else COALESCE(CR_Amount,0) end AS 'CrAmt',";
    qStr += " 0 AS Dr , 0 AS 'DrAmt' ";
    qStr += " FROM TRANSACTION_ITEM_HELPER TRANS JOIN SALES_INVENTORY_ITEMS INV ";
    qStr += " ON ITEMID = ITEM_ID ";
    qStr += " WHERE (To_Ledger like '"+ ledgerID + "')";
    qStr += " AND TRANS.VOUCHERDATE >= '" + fromDate.toString("yyyy-MM-dd")+ "'";
    qStr += " AND TRANS.VOUCHERDATE <= '" + toDate.toString("yyyy-MM-dd")+ "'";
    qStr += " UNION ALL";
    qStr += " SELECT TRANS.ItemID, Inv.Item_Name, 0 AS Cr,0 AS 'CrAmt',";
    qStr += " COALESCE((DR_QTY),0) AS Dr ,";
    qStr += " case when COALESCE(DR_Amount,0) = 0 then (DR_QTY * INV.PRICE) else COALESCE(DR_Amount,0) end AS 'DrAmt'";
    qStr += " FROM TRANSACTION_ITEM_HELPER TRANS JOIN SALES_INVENTORY_ITEMS INV ";
    qStr += " ON ITEMID = ITEM_ID ";
    qStr += " WHERE ( From_Ledger like '" + ledgerID + "')";
    qStr += " AND TRANS.VOUCHERDATE >= '" + fromDate.toString("yyyy-MM-dd")+ "'";
    qStr += " AND TRANS.VOUCHERDATE <= '" + toDate.toString("yyyy-MM-dd")+ "'";
    qStr += " UNION ALL ";
    qStr += " SELECT TRANS.ItemID,  Inv.Item_Name, COALESCE(CR_QTY,0) AS Cr, ";
    qStr += " case when COALESCE(CR_Amount,0) = 0 then (CR_QTY * INV.PRICE) else COALESCE(CR_Amount,0) end AS 'CrAmt',";
    qStr += " 0 AS Dr , 0 AS 'DrAmt' ";
    qStr += " FROM TRANSACTION_BOM_HELPER TRANS JOIN SALES_INVENTORY_ITEMS INV ";
    qStr += " ON ITEMID = ITEM_ID ";
    qStr += " WHERE (To_Ledger like '"+ ledgerID + "')";
    qStr += " AND TRANS.VOUCHERDATE >= '" + fromDate.toString("yyyy-MM-dd")+ "'";
    qStr += " AND TRANS.VOUCHERDATE <= '" + toDate.toString("yyyy-MM-dd")+ "'";
    qStr += " UNION ALL";
    qStr += " SELECT TRANS.ItemID, Inv.Item_Name, 0 AS Cr,0 AS 'CrAmt',";
    qStr += " COALESCE((DR_QTY),0) AS Dr ,";
    qStr += " case when COALESCE(DR_Amount,0) = 0 then (DR_QTY * INV.PRICE) else COALESCE(DR_Amount,0) end AS 'DrAmt'";
    qStr += " FROM TRANSACTION_BOM_HELPER TRANS JOIN SALES_INVENTORY_ITEMS INV ";
    qStr += " ON ITEMID = ITEM_ID ";
    qStr += " WHERE ( From_Ledger like '" + ledgerID + "')";
    qStr += " AND TRANS.VOUCHERDATE >= '" + fromDate.toString("yyyy-MM-dd")+ "'";
    qStr += " AND TRANS.VOUCHERDATE <= '" + toDate.toString("yyyy-MM-dd")+ "'";
    qStr += " ) trans ";
    qStr += " inner join ";
    if(groupID !="0"){
        qStr += " (select ItemGrp1.group_name, Item.item_id, 'G' as Ind,ItemGrp1.group_id as ID from ";
        qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
        qStr += " inner join ";
        qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
        qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
        qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  grp";
        qStr += "  on item.group_id = grp.group_id";
        qStr += "  union all ";
        qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
        qStr += "  from      " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  p";
        qStr += "  inner join cte ";
        qStr += "  on p.group_id = cte.parent_id )";
        qStr += " select cte.group_name, cte.item_id, cte.group_id ";
        qStr += " from cte where cte.parent_id = '"+ groupID +"'";
        qStr += " ) itemGrp1 ";
        qStr += " on itemGrp1.item_id = item.item_id ";
        qStr += " union ";
        qStr += " select Item2.Item_name, Item2.Item_id, 'I' as ind,Item2.Item_id as ID  from ";
        qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
        qStr += " where Item2.group_id = '"+ groupID +"'";
        qStr += " ) itemGrp ";
    }
    else {
        qStr += "(SELECT (SELECT group_Name from " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName;
        qStr += " grp WHERE group_id = inv.group_id) as group_name, inv.item_id, 'G' as Ind, inv.group_id as ID from ";
        qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" inv) itemGrp";
    }
    qStr += " on trans.itemid = itemGrp.item_id ";
    qStr += " GROUP BY itemGrp.item_id";


    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}



QSqlQueryModel *InventoryHelper::getGroupwiseMovementByGodown(QString groupID, QString godownId, QDate fromDate, QDate toDate)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    TransactionHelper *t= new TransactionHelper();
    salesVoucherDatabaseHelper shelper;

    QString qStr =" select Ind,ID,itemGrp.group_name as Name,";
    qStr += " Sum(Cr) as 'Outward \n Qty', Sum(CrAmt) 'Outward \n Amt', Sum(Dr) as 'Inward \n Qty', Sum(DrAmt) 'Inward \n Amt' FROM (";
    qStr += " SELECT TRANS.ItemID,  Inv.Item_Name, COALESCE(CR_QTY,0) AS Cr, ";
    qStr += " case when COALESCE(CR_Amount,0) = 0 then (CR_QTY * INV.PRICE) else COALESCE(CR_Amount,0) end AS 'CrAmt',";
    qStr += " 0 AS Dr , 0 AS 'DrAmt' ";
    qStr += " FROM TRANSACTION_ITEM_HELPER TRANS JOIN SALES_INVENTORY_ITEMS INV ";
    qStr += " ON ITEMID = ITEM_ID ";
    qStr += " WHERE (FROM_LOCATION like '"+ godownId + "')";
    qStr += " AND TRANS.VOUCHERDATE >= '" + fromDate.toString("yyyy-MM-dd")+ "'";
    qStr += " AND TRANS.VOUCHERDATE <= '" + toDate.toString("yyyy-MM-dd")+ "'";
    qStr += " UNION ALL";
    qStr += " SELECT TRANS.ItemID, Inv.Item_Name, 0 AS Cr,0 AS 'CrAmt',";
    qStr += " COALESCE((DR_QTY),0) AS Dr ,";
    qStr += " case when COALESCE(DR_Amount,0) = 0 then (DR_QTY * INV.PRICE) else COALESCE(DR_Amount,0) end AS 'DrAmt'";
    qStr += " FROM TRANSACTION_ITEM_HELPER TRANS JOIN SALES_INVENTORY_ITEMS INV ";
    qStr += " ON ITEMID = ITEM_ID ";
    qStr += " WHERE ( TO_LOCATION like '" + godownId + "')";
    qStr += " AND TRANS.VOUCHERDATE >= '" + fromDate.toString("yyyy-MM-dd")+ "'";
    qStr += " AND TRANS.VOUCHERDATE <= '" + toDate.toString("yyyy-MM-dd")+ "'";
    qStr += " UNION ALL ";
    qStr += " SELECT TRANS.ItemID,  Inv.Item_Name, COALESCE(CR_QTY,0) AS Cr, ";
    qStr += " case when COALESCE(CR_Amount,0) = 0 then (CR_QTY * INV.PRICE) else COALESCE(CR_Amount,0) end AS 'CrAmt',";
    qStr += " 0 AS Dr , 0 AS 'DrAmt' ";
    qStr += " FROM TRANSACTION_BOM_HELPER TRANS JOIN SALES_INVENTORY_ITEMS INV ";
    qStr += " ON ITEMID = ITEM_ID ";
    qStr += " WHERE (FROM_LOCATION like '"+ godownId + "')";
    qStr += " AND TRANS.VOUCHERDATE >= '" + fromDate.toString("yyyy-MM-dd")+ "'";
    qStr += " AND TRANS.VOUCHERDATE <= '" + toDate.toString("yyyy-MM-dd")+ "'";
    qStr += " UNION ALL";
    qStr += " SELECT TRANS.ItemID, Inv.Item_Name, 0 AS Cr,0 AS 'CrAmt',";
    qStr += " COALESCE((DR_QTY),0) AS Dr ,";
    qStr += " case when COALESCE(DR_Amount,0) = 0 then (DR_QTY * INV.PRICE) else COALESCE(DR_Amount,0) end AS 'DrAmt'";
    qStr += " FROM TRANSACTION_BOM_HELPER TRANS JOIN SALES_INVENTORY_ITEMS INV ";
    qStr += " ON ITEMID = ITEM_ID ";
    qStr += " WHERE ( TO_LOCATION like '" + godownId + "')";
    qStr += " AND TRANS.VOUCHERDATE >= '" + fromDate.toString("yyyy-MM-dd")+ "'";
    qStr += " AND TRANS.VOUCHERDATE <= '" + toDate.toString("yyyy-MM-dd")+ "'";
    qStr += " ) trans ";
    qStr += " inner join ";
    if(groupID !="0"){
        qStr += " (select ItemGrp1.group_name, Item.item_id, 'G' as Ind,ItemGrp1.group_id as ID from ";
        qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
        qStr += " inner join ";
        qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
        qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
        qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  grp";
        qStr += "  on item.group_id = grp.group_id";
        qStr += "  union all ";
        qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
        qStr += "  from      " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  p";
        qStr += "  inner join cte ";
        qStr += "  on p.group_id = cte.parent_id )";
        qStr += " select cte.group_name, cte.item_id, cte.group_id ";
        qStr += " from cte where cte.parent_id = '"+ groupID +"'";
        qStr += " ) itemGrp1 ";
        qStr += " on itemGrp1.item_id = item.item_id ";
        qStr += " union ";
        qStr += " select Item2.Item_name, Item2.Item_id, 'I' as ind,Item2.Item_id as ID  from ";
        qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
        qStr += " where Item2.group_id = '"+ groupID +"'";
        qStr += " ) itemGrp ";
    }
    else {
        qStr += "(SELECT (SELECT group_Name from " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName;
        qStr += " grp WHERE group_id = inv.group_id) as group_name, inv.item_id, 'G' as Ind, inv.group_id as ID from ";
        qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" inv) itemGrp";
    }
    qStr += " on trans.itemid = itemGrp.item_id ";
    qStr += " GROUP BY itemGrp.group_name";

    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}


QSqlQueryModel *InventoryHelper::getItemwiseMovementByGodownAndGroup(QString groupID, QString godownId, QDate fromDate, QDate toDate)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    TransactionHelper *t= new TransactionHelper();
    salesVoucherDatabaseHelper shelper;

    QString qStr =" select Ind,itemGrp.item_id ,";
    qStr += " (select item_name FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where item_id = itemGrp.item_id  ) as Item,";
    qStr += " Sum(Cr) as 'Outward \n Qty', Sum(CrAmt) 'Outward \n Amt', Sum(Dr) as 'Inward \n Qty', Sum(DrAmt) 'Inward \n Amt' FROM (";
    qStr += " SELECT TRANS.ItemID,  Inv.Item_Name, COALESCE(CR_QTY,0) AS Cr, ";
    qStr += " case when COALESCE(CR_Amount,0) = 0 then (CR_QTY * INV.PRICE) else COALESCE(CR_Amount,0) end AS 'CrAmt',";
    qStr += " 0 AS Dr , 0 AS 'DrAmt' ";
    qStr += " FROM TRANSACTION_ITEM_HELPER TRANS JOIN SALES_INVENTORY_ITEMS INV ";
    qStr += " ON ITEMID = ITEM_ID ";
    qStr += " WHERE (FROM_LOCATION like '"+ godownId + "')";
    qStr += " AND TRANS.VOUCHERDATE >= '" + fromDate.toString("yyyy-MM-dd")+ "'";
    qStr += " AND TRANS.VOUCHERDATE <= '" + toDate.toString("yyyy-MM-dd")+ "'";
    qStr += " UNION ALL";
    qStr += " SELECT TRANS.ItemID, Inv.Item_Name, 0 AS Cr,0 AS 'CrAmt',";
    qStr += " COALESCE((DR_QTY),0) AS Dr ,";
    qStr += " case when COALESCE(DR_Amount,0) = 0 then (DR_QTY * INV.PRICE) else COALESCE(DR_Amount,0) end AS 'DrAmt'";
    qStr += " FROM TRANSACTION_ITEM_HELPER TRANS JOIN SALES_INVENTORY_ITEMS INV ";
    qStr += " ON ITEMID = ITEM_ID ";
    qStr += " WHERE ( TO_LOCATION like '" + godownId + "')";
    qStr += " AND TRANS.VOUCHERDATE >= '" + fromDate.toString("yyyy-MM-dd")+ "'";
    qStr += " AND TRANS.VOUCHERDATE <= '" + toDate.toString("yyyy-MM-dd")+ "'";
    qStr += " UNION ALL ";
    qStr += " SELECT TRANS.ItemID,  Inv.Item_Name, COALESCE(CR_QTY,0) AS Cr, ";
    qStr += " case when COALESCE(CR_Amount,0) = 0 then (CR_QTY * INV.PRICE) else COALESCE(CR_Amount,0) end AS 'CrAmt',";
    qStr += " 0 AS Dr , 0 AS 'DrAmt' ";
    qStr += " FROM TRANSACTION_BOM_HELPER TRANS JOIN SALES_INVENTORY_ITEMS INV ";
    qStr += " ON ITEMID = ITEM_ID ";
    qStr += " WHERE (FROM_LOCATION like '"+ godownId + "')";
    qStr += " AND TRANS.VOUCHERDATE >= '" + fromDate.toString("yyyy-MM-dd")+ "'";
    qStr += " AND TRANS.VOUCHERDATE <= '" + toDate.toString("yyyy-MM-dd")+ "'";
    qStr += " UNION ALL";
    qStr += " SELECT TRANS.ItemID, Inv.Item_Name, 0 AS Cr,0 AS 'CrAmt',";
    qStr += " COALESCE((DR_QTY),0) AS Dr ,";
    qStr += " case when COALESCE(DR_Amount,0) = 0 then (DR_QTY * INV.PRICE) else COALESCE(DR_Amount,0) end AS 'DrAmt'";
    qStr += " FROM TRANSACTION_BOM_HELPER TRANS JOIN SALES_INVENTORY_ITEMS INV ";
    qStr += " ON ITEMID = ITEM_ID ";
    qStr += " WHERE ( TO_LOCATION like '" + godownId + "')";
    qStr += " AND TRANS.VOUCHERDATE >= '" + fromDate.toString("yyyy-MM-dd")+ "'";
    qStr += " AND TRANS.VOUCHERDATE <= '" + toDate.toString("yyyy-MM-dd")+ "'";
    qStr += " ) trans ";
    qStr += " inner join ";
    if(groupID !="0"){
        qStr += " (select ItemGrp1.group_name, Item.item_id, 'G' as Ind,ItemGrp1.group_id as ID from ";
        qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
        qStr += " inner join ";
        qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
        qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
        qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  grp";
        qStr += "  on item.group_id = grp.group_id";
        qStr += "  union all ";
        qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
        qStr += "  from      " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  p";
        qStr += "  inner join cte ";
        qStr += "  on p.group_id = cte.parent_id )";
        qStr += " select cte.group_name, cte.item_id, cte.group_id ";
        qStr += " from cte where cte.parent_id = '"+ groupID +"'";
        qStr += " ) itemGrp1 ";
        qStr += " on itemGrp1.item_id = item.item_id ";
        qStr += " union ";
        qStr += " select Item2.Item_name, Item2.Item_id, 'I' as ind,Item2.Item_id as ID  from ";
        qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
        qStr += " where Item2.group_id = '"+ groupID +"'";
        qStr += " ) itemGrp ";
    }
    else {
        qStr += "(SELECT (SELECT group_Name from " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName;
        qStr += " grp WHERE group_id = inv.group_id) as group_name, inv.item_id, 'G' as Ind, inv.group_id as ID from ";
        qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" inv) itemGrp";
    }
    qStr += " on trans.itemid = itemGrp.item_id ";
    qStr += " GROUP BY itemGrp.item_id";

    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}

QSqlQueryModel *InventoryHelper::getItemwiseSalesByDateAndPoc(QDateTime fromDate, QDateTime toDate, QString PhNo)
{
    salesVoucherDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper itemHelper;
    QSqlQueryModel* model = new QSqlQueryModel;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "";

    qStr = " SELECT INV." + itemHelper.Sales_Inventory_ItemName + " as ITEM,";
    qStr += " SALES." + shelper.Sales_Invoice_Details_Quantity + ", ";
    qStr += " SALES." + shelper.Sales_Invoice_Details_Subtotal + " from " ;
    qStr += itemHelper.Sales_Inventory_Table_Name + " as INV LEFT outer JOIN ";
    qStr += " (SELECT " + shelper.Sales_Invoice_Details_Inventory_Item_ID + ",SUM(SALES." ;
    qStr += shelper.Sales_Invoice_Details_Quantity + " ) as Quantity, ";
    qStr += " SUM(SALES." + shelper.Sales_Invoice_Details_Subtotal + " )  as SubTotal ";
    qStr += " from " + shelper.Sales_Invoice_Details_TableName + " as SALES ";
    qStr += " JOIN " + shelper.Sales_Invoice_Main_TableName + " main ";
    qStr += " ON SALES." + shelper.Sales_Invoice_Details_Voucher_No + " = main." + shelper.Sales_Invoice_Main_Voucher_No;
    qStr += " WHERE str_to_Date(concat(main." + shelper.Sales_Invoice_Main_Voucher_Date ;
    qStr += ", substr(" + shelper.Sales_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;
    qStr += " >='"+fromDate.toString("yyyy-MM-dd hh:mm:ss")+"'";
    qStr += " AND str_to_Date(concat(main." + shelper.Sales_Invoice_Main_Voucher_Date ;
    qStr += ", substr(" + shelper.Sales_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;
    qStr += " <='"+toDate.toString("yyyy-MM-dd hh:mm:ss")+"'";
    qStr += " AND main." + shelper.Sales_Invoice_Main_POC_Phone + " = '" + PhNo + "'";
    qStr += " group by SALES." + shelper.Sales_Invoice_Details_Inventory_Item_ID + " ) as SALES on ";
    qStr += " INV." + itemHelper.Sales_Inventory_ItemId + "=SALES." + shelper.Sales_Invoice_Details_Inventory_Item_ID;
    qStr += " where COALESCE(SALES." + shelper.Sales_Invoice_Details_Quantity + ",0)>0 ";
    //    qStr += " ORDER by 2,1";



    if(query.exec(qStr)){
        qDebug()<<qStr;
    }



    if(query.exec(qStr)){
        qDebug()<<qStr;
    }

    //    qDebug()<<qStr;

    //    query.prepare(qStr);

    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}

QSqlQueryModel *InventoryHelper::getDistinctCustomersPOC(QDate from ,QDate to)
{
    QString qStr ;
    salesVoucherDatabaseHelper shelper;
    qStr += "SELECT ";
    qStr += "DISTINCT(" + shelper.Sales_Invoice_Main_POC_Phone;
    qStr += ") as 'Phone' ,";
    qStr += "addr.name Name,";
    qStr += "addr.address Address";
    qStr += " FROM "+ shelper.Sales_Invoice_Main_TableName +" tab ";
    qStr += " JOIN address_book addr on poc_phone = addr.phone_number ";
    qStr += " where ";
    qStr += " tab."+shelper.Sales_Invoice_Main_Voucher_Date +">="+db->getDateString(from);
    qStr += " and ";
    qStr += " tab."+shelper.Sales_Invoice_Main_Voucher_Date +"<="+db->getDateString(to);
    qStr += " and ";
    qStr += shelper.Sales_Invoice_Main_POC_Phone+ " <> ''";

    QSqlQueryModel* model = new QSqlQueryModel;

    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}

QSqlQueryModel *InventoryHelper::getDatewiseSalesByGroup(QString groupID, QDateTime fromDate, QDateTime toDate, int x)
{
    int lang = ConfigurationSettingsDatabaseHelper::getValue(POSLang,0).toInt();
    QString grpName = "Group_Name";
    QString itemName = "Item_Name";
    if(lang==1){
        grpName = "Group_Name_Arabic";
        itemName = "Item_Name_Arabic";
    }

    salesVoucherDatabaseHelper shelper;

    QString timestamp = "str_to_Date(concat(stable." + shelper.Sales_Invoice_Main_Voucher_Date ;
    timestamp += ", substr(" + shelper.Sales_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;

    QString qStr = "SELECT Ind, ID,  groupName, ";
    QDate date2 = fromDate.date();
    int j=0;
    while( date2 <= toDate.date() ){
        qStr += "COALESCE(SUM(date"+QString::number(j)+"),0.00) as '" + date2.toString("yyyy-MM-dd") +"'";
        if (date2.daysTo(toDate.date()) >=x ){
            qStr += ", ";
        }
        date2 = date2.addDays(x);
        j+=x;
    }
    qStr += " FROM (SELECT Ind,ID, groupName, ";
    QDate date1 = fromDate.date();
    int i=0;
    while( date1 <= toDate.date() ){
        qStr += "CASE WHEN voucher_Date >= '" + date1.toString("yyyy-MM-dd") + "' AND voucher_Date < '" + date1.addDays(x).toString("yyyy-MM-dd") + "'";
        qStr += " THEN format(SUM(Amount),2) END ";
        qStr += "as date" + QString::number(i) ;
        if (date1.daysTo(toDate.date()) >=x ){
            qStr += ", ";
        }
        date1 = date1.addDays(x);
        i+=x;
    }
    qStr += " FROM ( ";

    qStr += "  select Ind, ID, itemGrp.group_name as groupName,";
    qStr += "  Subtotal as Amount, voucher_Date ";
    qStr += " from sales_invoice_details det";
    qStr += " inner join ";
    qStr += " (select ItemGrp1.group_name, Item.item_id, 'G' as Ind, ItemGrp1.group_id as ID from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
    qStr += " inner join ";
    qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
    qStr += "  select     grp.group_id,grp."+grpName+",grp.parent_id, item.item_id";
    qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  grp";
    qStr += "  on item.group_id = grp.group_id";
    qStr += "  union all ";
    qStr += "  select     p.group_id, p."+grpName+",p.parent_id, item_id";
    qStr += "  from       " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  p";
    qStr += "  inner join cte ";
    qStr += "  on p.group_id = cte.parent_id )";
    qStr += " select cte.group_id, cte.group_name, cte.item_id";
    qStr += " from cte where cte.parent_id = '"+ groupID +"'";
    qStr += " ) itemGrp1 ";
    qStr += " on itemGrp1.item_id = item.item_id ";
    qStr += " union ";
    qStr += " select (select "+grpName+" from " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  where group_id = Item2.group_id), Item2.Item_id, 'I' as ind, Item2.Group_Id as ID from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
    qStr += " where Item2.group_id = '"+ groupID +"'";
    qStr += " ) itemGrp ";
    qStr += " on det.inventory_item_id = itemGrp.item_id ";
    qStr += " where det.Voucher_Date >= '" + fromDate.toString("yyyy-MM-dd") ;
    qStr += "' and det.Voucher_Date <= '" + toDate.toString("yyyy-MM-dd") + "'";
    qStr += " )  as Sales ";
    qStr += " group by groupName , (Voucher_Date)) as final";
    qStr += " GROUP BY groupName";
    qStr += " ORDER BY 1";


    qDebug()<<Q_FUNC_INFO<<__LINE__<<qStr;

    return DatabaseHelper::getQueryModel(qStr);

}

QSqlQueryModel *InventoryHelper::getDatewiseDetailedSalesByGroup(QString groupID, QDateTime fromDate, QDateTime toDate, int x)
{

    int lang = ConfigurationSettingsDatabaseHelper::getValue(POSLang,0).toInt();
    QString grpName = "Group_Name";
    QString itemName = "Item_Name";
    if(lang==1){
        grpName = "Group_Name_Arabic";
        itemName = "Item_Name_Arabic";
    }
    salesVoucherDatabaseHelper shelper;

    QString timestamp = "str_to_Date(concat(stable." + shelper.Sales_Invoice_Main_Voucher_Date ;
    timestamp += ", substr(" + shelper.Sales_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;

    QString qStr = "SELECT Ind, ID,  Item, ";
    QDate date2 = fromDate.date();
    int j=0;
    while( date2 <= toDate.date() ){
        qStr += "COALESCE(SUM(date"+QString::number(j)+"),0.00) as '" + date2.toString("yyyy-MM-dd") +"'";
        if (date2.daysTo(toDate.date()) >=x ){
            qStr += ", ";
        }
        date2 = date2.addDays(x);
        j+=x;
    }
    qStr += " FROM (SELECT Ind,ID, Item, ";
    QDate date1 = fromDate.date();
    int i=0;
    while( date1 <= toDate.date() ){
        qStr += "CASE WHEN voucher_Date >= '" + date1.toString("yyyy-MM-dd") + "' AND voucher_Date < '" + date1.addDays(x).toString("yyyy-MM-dd") + "'";
        qStr += " THEN format(SUM(Amount),2) END ";
        qStr += "as date" + QString::number(i) ;
        if (date1.daysTo(toDate.date()) >=x ){
            qStr += ", ";
        }
        date1 = date1.addDays(x);
        i+=x;
    }
    qStr += " FROM ( ";

    qStr += "  select 'I' Ind, det.inventory_item_id ID,";
    qStr += " (select item_name FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where item_id = det.inventory_item_id ) as Item,";
    qStr += "  Subtotal as Amount, voucher_Date ";
    qStr += " from sales_invoice_details det";
    qStr += " inner join ";
    qStr += " (select ItemGrp1.group_name, Item.item_id, 'G' as Ind, ItemGrp1.group_id as ID from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
    qStr += " inner join ";
    qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
    qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
    qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  grp";
    qStr += "  on item.group_id = grp.group_id";
    qStr += "  union all ";
    qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
    qStr += "  from      " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  p";
    qStr += "  inner join cte ";
    qStr += "  on p.group_id = cte.parent_id )";
    qStr += " select cte.group_id, cte.group_name, cte.item_id";
    qStr += " from cte where cte.parent_id = '"+ groupID +"'";
    qStr += " ) itemGrp1 ";
    qStr += " on itemGrp1.item_id = item.item_id ";
    qStr += " union ";
    qStr += " select (select group_name from " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  where group_id = Item2.group_id), Item2.Item_id, 'I' as ind, Item2.Group_Id as ID from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
    qStr += " where Item2.group_id = '"+ groupID +"'";
    qStr += " ) itemGrp ";
    qStr += " on det.inventory_item_id = itemGrp.item_id ";
    qStr += " where det.Voucher_Date >= '" + fromDate.toString("yyyy-MM-dd") ;
    qStr += "' and det.Voucher_Date <= '" + toDate.toString("yyyy-MM-dd") + "'";
    qStr += " )  as Sales ";
    qStr += " group by Item , Voucher_Date) as final";
    qStr += " GROUP BY Item";
    qStr += " ORDER BY 1";

    qDebug()<<Q_FUNC_INFO<<__LINE__<<qStr;


    return DatabaseHelper::getQueryModel(qStr);

}


QSqlQueryModel *InventoryHelper::getDatewiseDetailedPurchaseByGroup(QString groupID, QDateTime fromDate, QDateTime toDate, int x)
{

    salesVoucherDatabaseHelper shelper;

    QString timestamp = "str_to_Date(concat(stable." + shelper.Sales_Invoice_Main_Voucher_Date ;
    timestamp += ", substr(" + shelper.Sales_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;

    QString qStr = "SELECT Ind, ID,  Item, ";
    QDate date2 = fromDate.date();
    int j=0;
    while( date2 <= toDate.date() ){
        qStr += "COALESCE(SUM(date"+QString::number(j)+"),0.00) as '" + date2.toString("yyyy-MM-dd") +"'";
        if (date2.daysTo(toDate.date()) >=x ){
            qStr += ", ";
        }
        date2 = date2.addDays(x);
        j+=x;
    }
    qStr += " FROM (SELECT Ind,ID, Item, ";
    QDate date1 = fromDate.date();
    int i=0;
    while( date1 <= toDate.date() ){
        qStr += "CASE WHEN voucher_Date >= '" + date1.toString("yyyy-MM-dd") + "' AND voucher_Date < '" + date1.addDays(x).toString("yyyy-MM-dd") + "'";
        qStr += " THEN format(SUM(Amount),2) END ";
        qStr += "as date" + QString::number(i) ;
        if (date1.daysTo(toDate.date()) >=x ){
            qStr += ", ";
        }
        date1 = date1.addDays(x);
        i+=x;
    }
    qStr += " FROM ( ";

    qStr += "  select 'I' Ind, det.inventory_item_id ID,";
    qStr += " (select item_name FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where item_id = det.inventory_item_id ) as Item,";
    qStr += "  Subtotal as Amount, voucher_Date ";
    qStr += " from purchase_invoice_details det";
    qStr += " inner join ";
    qStr += " (select ItemGrp1.group_name, Item.item_id, 'G' as Ind, ItemGrp1.group_id as ID from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
    qStr += " inner join ";
    qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
    qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
    qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  grp";
    qStr += "  on item.group_id = grp.group_id";
    qStr += "  union all ";
    qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
    qStr += "  from      " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  p";
    qStr += "  inner join cte ";
    qStr += "  on p.group_id = cte.parent_id )";
    qStr += " select cte.group_id, cte.group_name, cte.item_id";
    qStr += " from cte where cte.parent_id = '"+ groupID +"'";
    qStr += " ) itemGrp1 ";
    qStr += " on itemGrp1.item_id = item.item_id ";
    qStr += " union ";
    qStr += " select (select group_name from " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  where group_id = Item2.group_id), Item2.Item_id, 'I' as ind, Item2.Group_Id as ID from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
    qStr += " where Item2.group_id = '"+ groupID +"'";
    qStr += " ) itemGrp ";
    qStr += " on det.inventory_item_id = itemGrp.item_id ";
    qStr += " where det.Voucher_Date >= '" + fromDate.toString("yyyy-MM-dd") ;
    qStr += "' and det.Voucher_Date <= '" + toDate.toString("yyyy-MM-dd") + "'";
    qStr += " )  as Sales ";
    qStr += " group by Item , Voucher_Date) as final";
    qStr += " GROUP BY Item";
    qStr += " ORDER BY 1";



    return DatabaseHelper::getQueryModel(qStr);

}


QSqlQueryModel *InventoryHelper::getDatewiseDetailedWOByGroup(QString groupID, QDateTime fromDate, QDateTime toDate, int x)
{

    WorkOrderDatabaseHelper shelper;

    QString timestamp = "str_to_Date(concat(stable." + shelper.Work_Order_Main_Voucher_Date ;
    timestamp += ", substr(" + shelper.Work_Order_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;

    QString qStr = "SELECT Ind, ID,  Item, ";
    QDate date2 = fromDate.date();
    int j=0;
    while( date2 <= toDate.date() ){
        qStr += "COALESCE(SUM(date"+QString::number(j)+"),0.00) as '" + date2.toString("yyyy-MM-dd") +"'";
        if (date2.daysTo(toDate.date()) >=x ){
            qStr += ", ";
        }
        date2 = date2.addDays(x);
        j+=x;
    }
    qStr += " FROM (SELECT Ind,ID, Item, ";
    QDate date1 = fromDate.date();
    int i=0;
    while( date1 <= toDate.date() ){
        qStr += "CASE WHEN timestamp >= '" + date1.toString("yyyy-MM-dd") + "' AND voucher_Date < '" + date1.addDays(x).toString("yyyy-MM-dd") + "'";
        qStr += " THEN format(SUM(Amount),2) END ";
        qStr += "as date" + QString::number(i) ;
        if (date1.daysTo(toDate.date()) >=x ){
            qStr += ", ";
        }
        date1 = date1.addDays(x);
        i+=x;
    }
    qStr += " FROM ( ";

    qStr += "  select 'I' Ind, det.inventory_item_id ID,";
    qStr += " (select item_name FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where item_id = det." + shelper.WorkOrder_itemID + " ) as Item,";
    qStr += "  Quantity * det.Price as Amount, voucher_Date ";
    qStr += " from Work_Order_Detail det";
    qStr += " inner join ";
    qStr += " (select ItemGrp1.group_name, Item.item_id, 'G' as Ind, ItemGrp1.group_id as ID from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
    qStr += " inner join ";
    qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
    qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
    qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  grp";
    qStr += "  on item.group_id = grp.group_id";
    qStr += "  union all ";
    qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
    qStr += "  from      " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  p";
    qStr += "  inner join cte ";
    qStr += "  on p.group_id = cte.parent_id )";
    qStr += " select cte.group_id, cte.group_name, cte.item_id";
    qStr += " from cte where cte.parent_id = '"+ groupID +"'";
    qStr += " ) itemGrp1 ";
    qStr += " on itemGrp1.item_id = item.item_id ";
    qStr += " union ";
    qStr += " select (select group_name from " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  where group_id = Item2.group_id), Item2.Item_id, 'I' as ind, Item2.Group_Id as ID from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
    qStr += " where Item2.group_id = '"+ groupID +"'";
    qStr += " ) itemGrp ";
    qStr += " on det." + shelper.WorkOrder_itemID + " = itemGrp.item_id ";
    qStr += " where det.Voucher_Date >= '" + fromDate.toString("yyyy-MM-dd") ;
    qStr += "' and det.Voucher_Date <= '" + toDate.toString("yyyy-MM-dd") + "'";
    qStr += " )  as Sales ";
    qStr += " group by Item , Voucher_Date) as final";
    qStr += " GROUP BY Item";
    qStr += " ORDER BY 1";



    return DatabaseHelper::getQueryModel(qStr);

}


QSqlQueryModel *InventoryHelper::getDatewiseSalesByGroupAndModeOfService(QString groupID, QDateTime fromDate, QDateTime toDate, int x)
{
    int lang = ConfigurationSettingsDatabaseHelper::getValue(POSLang,0).toInt();
    QString grpName = "Group_Name";
    QString itemName = "Item_Name";
    if(lang==1){
        grpName = "Group_Name_Arabic";
        itemName = "Item_Name_Arabic";
    }
    salesVoucherDatabaseHelper shelper;

    QString timestamp = "str_to_Date(concat(stable." + shelper.Sales_Invoice_Main_Voucher_Date ;
    timestamp += ", substr(" + shelper.Sales_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;

    QString qStr = "SELECT Ind, ID,  groupName,";
    qStr += " case when Mode_Of_Service = 1 then 'Delivery' when Mode_Of_Service = 2 then 'TakeAway' ";
    qStr += " when Mode_Of_Service = 3 then 'DineIn' when Mode_Of_service = 4 then 'Store' else 'Unknown' end as Mode, ";
    QDate date2 = fromDate.date();

    int j=0;

    while( date2 <= toDate.date() ){
        qStr += "COALESCE(MAX(date"+QString::number(j)+"),0.00) as '" + date2.toString("yyyy-MM-dd") +"'";
        if (date2 < toDate.date()){
            qStr += ", ";
        }
        date2 = date2.addDays(x);
        j+=x;
    }
    qStr += " FROM (SELECT Ind, ID, groupName, Mode_Of_Service, ";
    QDate date1 = fromDate.date();
    int i=0;
    while( date1 <= toDate.date() ){
        qStr += "CASE WHEN voucher_Date = '" + date1.toString("yyyy-MM-dd") + "' THEN format(SUM(Amount),2) END ";
        qStr += "as date" + QString::number(i) ;
        if (date1 < toDate.date()){
            qStr += ", ";
        }
        date1 = date1.addDays(x);
        i+=x;
    }
    qStr += " FROM ( ";

    qStr += "  select Ind, ID,   itemGrp.group_name as groupName, main.Mode_Of_Service, ";
    qStr += "  Subtotal as Amount, main.voucher_Date ";
    qStr += " from sales_invoice_details det";
    qStr += " inner join sales_invoice_main main on det.voucher_no = main.voucher_no and det.voucher_prefix = main.voucher_prefix ";
    qStr += " inner join ";
    qStr += " (select ItemGrp1.group_name, Item.item_id, 'G' as Ind, ItemGrp1.group_id as ID from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
    qStr += " inner join ";
    qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
    qStr += "  select     grp.group_id,grp."+grpName+",grp.parent_id, item.item_id";
    qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  grp";
    qStr += "  on item.group_id = grp.group_id";
    qStr += "  union all ";
    qStr += "  select p.group_id, p."+grpName+",p.parent_id, item_id";
    qStr += "  from      " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  p";
    qStr += "  inner join cte ";
    qStr += "  on p.group_id = cte.parent_id )";
    qStr += " select cte.group_id, cte.group_name, cte.item_id";
    qStr += " from cte where cte.parent_id = '"+ groupID +"'";
    qStr += " ) itemGrp1 ";
    qStr += " on itemGrp1.item_id = item.item_id ";
    qStr += " union ";
    qStr += " select (select "+grpName+" from " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  where group_id = Item2.group_id), Item2.Item_id, 'I' as ind, Item2.Group_Id as ID from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
    qStr += " where Item2.group_id = '"+ groupID +"'";
    qStr += " ) itemGrp ";
    qStr += " on det.inventory_item_id = itemGrp.item_id ";
    qStr += " where det.Voucher_Date >= '" + fromDate.toString("yyyy-MM-dd") ;
    qStr += "' and det.Voucher_Date <= '" + toDate.toString("yyyy-MM-dd") + "'";
    qStr += " )  as Sales ";
    qStr += " group by groupName , Voucher_Date, mode_of_service ) as final";
    qStr += " GROUP BY groupName, mode_of_service";
    qStr += " ORDER BY 1,2";

   qDebug()<<Q_FUNC_INFO<<__LINE__<<qStr;

    return DatabaseHelper::getQueryModel(qStr);

}

QSqlQueryModel *InventoryHelper::getDatewiseTransactionByVoucherTypeByGroupAndReason(QString voucherType,QString groupID, QDateTime fromDate, QDateTime toDate, QString LedgerID, int x)
{

    QString tableName;
    if(voucherType == "DebitNote")
        tableName = "Debit_Note_Main";
    else if (voucherType == "CreditNote")
        tableName = "Credit_Note_Main";
    else if(voucherType == "Wastage")
        tableName = "Wastage_Entry_Main";

    salesVoucherDatabaseHelper shelper;

    QString timestamp = "str_to_Date(concat(stable." + shelper.Sales_Invoice_Main_Voucher_Date ;
    timestamp += ", substr(" + shelper.Sales_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;

    QString qStr = "SELECT Ind, ID,  groupName,";
    qStr += " case when Mode_Of_Service = 1 then 'Delivery' when Mode_Of_Service = 2 then 'TakeAway' ";
    qStr += " when Mode_Of_Service = 3 then 'DineIn' when Mode_Of_service = 4 then 'Store' else Mode_Of_service end as Mode, ";
    QDate date2 = fromDate.date();
    int j=0;
    while( date2 <= toDate.date() ){
        qStr += "COALESCE(MAX(date"+QString::number(j)+"),0.00) as '" + date2.toString("yyyy-MM-dd") +"'";
        if (date2 < toDate.date()){
            qStr += ", ";
        }
        date2 = date2.addDays(x);
        j+=x;
    }
    qStr += " FROM (SELECT Ind, ID, groupName, Mode_Of_Service, ";
    QDate date1 = fromDate.date();
    int i=0;
    while( date1 <= toDate.date() ){
        qStr += "CASE WHEN voucher_Date = '" + date1.toString("yyyy-MM-dd") + "' THEN format(SUM(Amount),2) END ";
        qStr += "as date" + QString::number(i) ;
        if (date1 < toDate.date()){
            qStr += ", ";
        }
        date1 = date1.addDays(x);
        i+=x;
    }
    qStr += " FROM ( ";

    qStr += "  select Ind, ID,   itemGrp.group_name as groupName, main.Mode_Of_Service, ";
    qStr += "  Subtotal as Amount, main.voucher_Date ";
    qStr += " from sales_invoice_details det";
    qStr += " inner join sales_invoice_main main on det.voucher_no = main.voucher_no and det.voucher_prefix = main.voucher_prefix ";
    qStr += " inner join ";
    qStr += " (select ItemGrp1.group_name, Item.item_id, 'G' as Ind, ItemGrp1.group_id as ID from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
    qStr += " inner join ";
    qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
    qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
    qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  grp";
    qStr += "  on item.group_id = grp.group_id";
    qStr += "  union all ";
    qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
    qStr += "  from      " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  p";
    qStr += "  inner join cte ";
    qStr += "  on p.group_id = cte.parent_id )";
    qStr += " select cte.group_id, cte.group_name, cte.item_id";
    qStr += " from cte where cte.parent_id = '"+ groupID +"'";
    qStr += " ) itemGrp1 ";
    qStr += " on itemGrp1.item_id = item.item_id ";
    qStr += " union ";
    qStr += " select (select group_name from " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  where group_id = Item2.group_id), Item2.Item_id, 'I' as ind, Item2.Group_Id as ID from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
    qStr += " where Item2.group_id = '"+ groupID +"'";
    qStr += " ) itemGrp ";
    qStr += " on det.inventory_item_id = itemGrp.item_id ";
    qStr += " where det.Voucher_Date >= '" + fromDate.toString("yyyy-MM-dd") ;
    qStr += "' and det.Voucher_Date <= '" + toDate.toString("yyyy-MM-dd") + "'";
    qStr += " )  as Sales ";
    qStr += " group by groupName , Voucher_Date, mode_of_service ) as final";
    qStr += " GROUP BY groupName, mode_of_service";
    qStr += " ORDER BY 1,2";


    return DatabaseHelper::getQueryModel(qStr);

}

QSqlQueryModel *InventoryHelper::getDatewisePurchaseByGroup(QString groupID, QDateTime fromDate, QDateTime toDate, int x)
{

    PurchaseVoucherDatabaseHelper shelper;

    QString timestamp = "str_to_Date(concat(stable." + shelper.Purchase_Invoice_Main_Voucher_Date ;
    timestamp += ", substr(" + shelper.Purchase_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;

    QString qStr = "SELECT Ind, ID, groupName, ";
    QDate date2 = fromDate.date();
    int j=0;
    while( date2 <= toDate.date() ){
        qStr += "COALESCE(MAX(date"+QString::number(j)+"),0.00) as '" + date2.toString("yyyy-MM-dd") +"'";
        if (date2 < toDate.date()){
            qStr += ", ";
        }
        date2 = date2.addDays(x);
        j+=x;
    }
    qStr += " FROM (SELECT Ind, ID, groupName, ";
    QDate date1 = fromDate.date();
    int i=0;
    while( date1 <= toDate.date() ){
        qStr += "CASE WHEN voucher_Date = '" + date1.toString("yyyy-MM-dd") + "' THEN format(SUM(Amount),2) END ";
        qStr += "as date" + QString::number(i) ;
        if (date1 < toDate.date()){
            qStr += ", ";
        }
        date1 = date1.addDays(1);
        i++;
    }
    qStr += " FROM ( ";

    qStr += "  select Ind,  ID, itemGrp.group_name as groupName,";
    qStr += "  Subtotal as Amount, voucher_Date ";
    qStr += " from purchase_invoice_details det";
    qStr += " inner join ";
    qStr += " (select ItemGrp1.group_name, Item.item_id, 'G' as Ind, ItemGrp1.group_id as ID from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
    qStr += " inner join ";
    qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
    qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
    qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  grp";
    qStr += "  on item.group_id = grp.group_id";
    qStr += "  union all ";
    qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
    qStr += "  from      " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  p";
    qStr += "  inner join cte ";
    qStr += "  on p.group_id = cte.parent_id )";
    qStr += " select cte.group_id, cte.group_name, cte.item_id";
    qStr += " from cte where cte.parent_id = '"+ groupID +"'";
    qStr += " ) itemGrp1 ";
    qStr += " on itemGrp1.item_id = item.item_id ";
    qStr += " union ";
    qStr += " select (select group_name from " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  where group_id = Item2.group_id), Item2.Item_id, 'I' as ind, Item2.Group_Id as ID from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
    qStr += " where Item2.group_id = '"+ groupID +"'";
    qStr += " ) itemGrp ";
    qStr += " on det.inventory_item_id = itemGrp.item_id ";
    qStr += " where det.Voucher_Date >= '" + fromDate.toString("yyyy-MM-dd") ;
    qStr += "' and det.Voucher_Date <= '" + toDate.toString("yyyy-MM-dd") + "'";
    qStr += " )  as Purchase ";
    qStr += " group by groupName , Voucher_Date) as final";
    qStr += " GROUP BY groupName";
    qStr += " ORDER BY 1";



    return DatabaseHelper::getQueryModel(qStr);

}


QSqlQueryModel *InventoryHelper::getDatewiseWOByGroup(QString groupID, QDateTime fromDate, QDateTime toDate, int x)
{

    WorkOrderDatabaseHelper shelper;

    QString timestamp = "str_to_Date(concat(stable." + shelper.Work_Order_Main_Voucher_Date ;
    timestamp += ", substr(" + shelper.Work_Order_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;

    QString qStr = "SELECT Ind, ID, groupName, ";
    QDate date2 = fromDate.date();
    int j=0;
    while( date2 <= toDate.date() ){
        qStr += "COALESCE(MAX(date"+QString::number(j)+"),0.00) as '" + date2.toString("yyyy-MM-dd") +"'";
        if (date2 < toDate.date()){
            qStr += ", ";
        }
        date2 = date2.addDays(x);
        j+=x;
    }
    qStr += " FROM (SELECT Ind, ID, groupName, ";
    QDate date1 = fromDate.date();
    int i=0;
    while( date1 <= toDate.date() ){
        qStr += "CASE WHEN timestamp = '" + date1.toString("yyyy-MM-dd") + "' THEN format(SUM(Amount),2) END ";
        qStr += "as date" + QString::number(i) ;
        if (date1 < toDate.date()){
            qStr += ", ";
        }
        date1 = date1.addDays(1);
        i++;
    }
    qStr += " FROM ( ";

    qStr += "  select Ind,  ID, itemGrp.group_name as groupName,";
    qStr += "  det.Price*Quantity as Amount, voucher_Date ";
    qStr += " from Work_order_detail det";
    qStr += " inner join ";
    qStr += " (select ItemGrp1.group_name, Item.item_id, 'G' as Ind, ItemGrp1.group_id as ID from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
    qStr += " inner join ";
    qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
    qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
    qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  grp";
    qStr += "  on item.group_id = grp.group_id";
    qStr += "  union all ";
    qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
    qStr += "  from      " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  p";
    qStr += "  inner join cte ";
    qStr += "  on p.group_id = cte.parent_id )";
    qStr += " select cte.group_id, cte.group_name, cte.item_id";
    qStr += " from cte where cte.parent_id = '"+ groupID +"'";
    qStr += " ) itemGrp1 ";
    qStr += " on itemGrp1.item_id = item.item_id ";
    qStr += " union ";
    qStr += " select (select group_name from " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  where group_id = Item2.group_id), Item2.Item_id, 'I' as ind, Item2.Group_Id as ID from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
    qStr += " where Item2.group_id = '"+ groupID +"'";
    qStr += " ) itemGrp ";
    qStr += " on det." + shelper.WorkOrder_itemID + " = itemGrp.item_id ";
    qStr += " where det.Voucher_Date >= '" + fromDate.toString("yyyy-MM-dd") ;
    qStr += "' and det.Voucher_Date <= '" + toDate.toString("yyyy-MM-dd") + "'";
    qStr += " )  as WO ";
    qStr += " group by groupName , Voucher_Date) as final";
    qStr += " GROUP BY groupName";
    qStr += " ORDER BY 1";



    return DatabaseHelper::getQueryModel(qStr);

}


QSqlQueryModel *InventoryHelper::getDatewiseTransactionsByGroup(QString groupID, QDateTime fromDate, QDateTime toDate, int x)
{

    PurchaseVoucherDatabaseHelper shelper;

    //    QString timestamp = "str_to_Date(concat(stable." + shelper.Purchase_Invoice_Main_Voucher_Date ;
    //    timestamp += ", substr(" + shelper.Purchase_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;

    QString qStr = "SELECT Ind, ID, groupName, ";
    QDate date2 = fromDate.date();
    int j=0;
    while( date2 <= toDate.date() ){
        qStr += "COALESCE(MAX(CR_date"+QString::number(j)+"),0.00) as 'CR on " + date2.toString("yyyy-MM-dd") +"', ";


        qStr += "COALESCE(MAX(DR_date"+QString::number(j)+"),0.00) as 'DR on " + date2.toString("yyyy-MM-dd") +"'";
        if (date2 < toDate.date()){
            qStr += ", ";
        }

        date2 = date2.addDays(x);
        j+=x;
    }
    qStr += " FROM (SELECT Ind, ID, groupName, ";
    QDate date1 = fromDate.date();
    int i=0;
    while( date1 <= toDate.date() ){
        qStr += "CASE WHEN voucherDate = '" + date1.toString("yyyy-MM-dd") + "' THEN format(SUM(Cr_Amount),2) END ";
        qStr += "as CR_date" + QString::number(i) + ", ";

        //        if (date1 < toDate.date()){
        //            qStr += ", ";
        //        }

        qStr += "CASE WHEN voucherDate = '" + date1.toString("yyyy-MM-dd") + "' THEN format(SUM(Dr_Amount),2) END ";
        qStr += "as DR_date" + QString::number(i) ;

        if (date1 < toDate.date()){
            qStr += ", ";
        }
        date1 = date1.addDays(x);
        i+=x;
    }
    qStr += " FROM ( ";

    qStr += "  select Ind,  ID, itemGrp.group_name as groupName,";
    qStr += "  Cr_Amount, Dr_Amount, voucherDate ";
    qStr += " from transaction_item_Helper det";
    qStr += " inner join ";
    qStr += " (select ItemGrp1.group_name, Item.item_id, 'G' as Ind, ItemGrp1.group_id as ID from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
    qStr += " inner join ";
    qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
    qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
    qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  grp";
    qStr += "  on item.group_id = grp.group_id";
    qStr += "  union all ";
    qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
    qStr += "  from      " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  p";
    qStr += "  inner join cte ";
    qStr += "  on p.group_id = cte.parent_id )";
    qStr += " select cte.group_id, cte.group_name, cte.item_id";
    qStr += " from cte where cte.parent_id = '"+ groupID +"'";
    qStr += " ) itemGrp1 ";
    qStr += " on itemGrp1.item_id = item.item_id ";
    qStr += " union ";
    qStr += " select (select group_name from " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  where group_id = Item2.group_id), Item2.Item_id, 'I' as ind, Item2.Group_Id as ID from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
    qStr += " where Item2.group_id = '"+ groupID +"'";
    qStr += " ) itemGrp ";
    qStr += " on det.itemID = itemGrp.item_id ";
    qStr += " where det.VoucherDate >= '" + fromDate.toString("yyyy-MM-dd") ;
    qStr += "' and det.VoucherDate <= '" + toDate.toString("yyyy-MM-dd") + "'";
    qStr += " )  as Trans ";
    qStr += " group by groupName , VoucherDate) as final";
    qStr += " GROUP BY groupName";
    qStr += " ORDER BY 1";



    return DatabaseHelper::getQueryModel(qStr);

}

QSqlQueryModel *InventoryHelper::getItemWiseSalesWithGroupByDateModel(QDateTime fromDate, QDateTime toDate)
{
    salesVoucherDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper itemHelper;
    SalesInventoryGroupsDatabaseHelper gHelper;
    QSqlQueryModel* model = new QSqlQueryModel;



    QString timestamp = "str_to_Date(concat(stable." + shelper.Sales_Invoice_Main_Voucher_Date ;
    timestamp += ", substr(" + shelper.Sales_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;

    QString qStr = "";

    qStr = " SELECT INV." + itemHelper.Sales_Inventory_ItemName + " as ITEM,";
    qStr += " (SELECT " + gHelper.Sales_Inventory_Group_GroupName + " FROM " + gHelper.Sales_Inventory_Group_TableName ;
    qStr +=  " where " + gHelper.Sales_Inventory_Group_GroupId + " = INV." + itemHelper.Sales_Inventory_GroupId + ") as 'Group' , ";
    qStr += " SALES." + shelper.Sales_Invoice_Details_Quantity + ", ";
    qStr += " SALES." + shelper.Sales_Invoice_Details_Subtotal + " from " ;
    qStr += itemHelper.Sales_Inventory_Table_Name + " as INV LEFT outer JOIN ";
    qStr += " (SELECT " + shelper.Sales_Invoice_Details_Inventory_Item_ID + ",SUM(SALES." ;
    qStr += shelper.Sales_Invoice_Details_Quantity + " ) as Quantity, ";
    qStr += " SUM(SALES." + shelper.Sales_Invoice_Details_Subtotal + " )  as SubTotal ";
    qStr += " from " + shelper.Sales_Invoice_Details_TableName + " as SALES ";
    qStr += " JOIN " + shelper.Sales_Invoice_Main_TableName + " main ";
    qStr += " ON SALES." + shelper.Sales_Invoice_Details_Voucher_No + " = main." + shelper.Sales_Invoice_Main_Voucher_No;
    //    qStr += " WHERE main." + shelper.Sales_Invoice_Main_TimeStamp + " >='"+fromDate.toString("yyyy-MM-dd hh:mm:ss")+"'";
    //    qStr += " AND main." + shelper.Sales_Invoice_Main_TimeStamp + " <='"+toDate.toString("yyyy-MM-dd hh:mm:ss")+"'";
    qStr += " WHERE str_to_Date(concat(main." + shelper.Sales_Invoice_Main_Voucher_Date ;
    qStr += ", substr(" + shelper.Sales_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;
    qStr += " >='"+fromDate.toString("yyyy-MM-dd hh:mm:ss")+"'";
    qStr += " AND str_to_Date(concat(main." + shelper.Sales_Invoice_Main_Voucher_Date ;
    qStr += ", substr(" + shelper.Sales_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;
    qStr += " <='"+toDate.toString("yyyy-MM-dd hh:mm:ss")+"'";

    qStr += " group by SALES." + shelper.Sales_Invoice_Details_Inventory_Item_ID + " ) as SALES on ";
    qStr += " INV." + itemHelper.Sales_Inventory_ItemId + "=SALES." + shelper.Sales_Invoice_Details_Inventory_Item_ID;
    qStr += " where COALESCE(SALES." + shelper.Sales_Invoice_Details_Quantity + ",0)>0 ";
    qStr += " ORDER by 2,1";

    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}

QSqlQueryModel *InventoryHelper::getItemWiseSalesWithGroupAndModeByDateModel(QDateTime fromDate, QDateTime toDate)
{
    salesVoucherDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper itemHelper;
    SalesInventoryGroupsDatabaseHelper gHelper;
    QSqlQueryModel* model = new QSqlQueryModel;



    QString timestamp = "str_to_Date(concat(stable." + shelper.Sales_Invoice_Main_Voucher_Date ;
    timestamp += ", substr(" + shelper.Sales_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;

    QString qStr = "";

    qStr = " SELECT INV." + itemHelper.Sales_Inventory_ItemName + " as ITEM,";
    qStr += " (SELECT " + gHelper.Sales_Inventory_Group_GroupName + " FROM " + gHelper.Sales_Inventory_Group_TableName ;
    qStr +=  " where " + gHelper.Sales_Inventory_Group_GroupId + " = INV." + itemHelper.Sales_Inventory_GroupId + ") as 'Group' , ";
    qStr += " case when Mode_Of_Service = 1 then 'Delivery' when Mode_Of_Service = 2 then 'TakeAway' ";
    qStr += " when Mode_Of_Service = 3 then 'DineIn' when Mode_Of_service = 4 then 'Store' else Mode_Of_service end as 'Mode Of Service', ";
    qStr += " SALES." + shelper.Sales_Invoice_Details_Quantity + ", ";
    qStr += " SALES." + shelper.Sales_Invoice_Details_Subtotal + " from " ;
    qStr += itemHelper.Sales_Inventory_Table_Name + " as INV LEFT outer JOIN ";
    qStr += " (SELECT " + shelper.Sales_Invoice_Details_Inventory_Item_ID + ",SUM(SALES." ;
    qStr += shelper.Sales_Invoice_Details_Quantity + " ) as Quantity, ";
    qStr += " main." + shelper.Sales_Invoice_Main_ModeOfService + ", ";
    qStr += " SUM(SALES." + shelper.Sales_Invoice_Details_Subtotal + " )  as SubTotal ";
    qStr += " from " + shelper.Sales_Invoice_Details_TableName + " as SALES ";
    qStr += " JOIN " + shelper.Sales_Invoice_Main_TableName + " main ";
    qStr += " ON SALES." + shelper.Sales_Invoice_Details_Voucher_No + " = main." + shelper.Sales_Invoice_Main_Voucher_No;
    //    qStr += " WHERE main." + shelper.Sales_Invoice_Main_TimeStamp + " >='"+fromDate.toString("yyyy-MM-dd hh:mm:ss")+"'";
    //    qStr += " AND main." + shelper.Sales_Invoice_Main_TimeStamp + " <='"+toDate.toString("yyyy-MM-dd hh:mm:ss")+"'";
    qStr += " WHERE str_to_Date(concat(main." + shelper.Sales_Invoice_Main_Voucher_Date ;
    qStr += ", substr(" + shelper.Sales_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;
    qStr += " >='"+fromDate.toString("yyyy-MM-dd hh:mm:ss")+"'";
    qStr += " AND str_to_Date(concat(main." + shelper.Sales_Invoice_Main_Voucher_Date ;
    qStr += ", substr(" + shelper.Sales_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;
    qStr += " <='"+toDate.toString("yyyy-MM-dd hh:mm:ss")+"'";

    qStr += " group by SALES." + shelper.Sales_Invoice_Details_Inventory_Item_ID + " ) as SALES on ";
    qStr += " INV." + itemHelper.Sales_Inventory_ItemId + "=SALES." + shelper.Sales_Invoice_Details_Inventory_Item_ID;
    qStr += " where COALESCE(SALES." + shelper.Sales_Invoice_Details_Quantity + ",0)>0 ";
    qStr += " ORDER by 2,1";

    qDebug()<<qStr;



    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}

QSqlQueryModel *InventoryHelper::getGroupWiseSalesDetailsByGroup(QString groupID,
                                                                 QDateTime fromDate, QDateTime toDate,
                                                                 QString godownId )
{
    int lang = ConfigurationSettingsDatabaseHelper::getValue(POSLang,0).toInt();
    QString grpName = "Group_Name";
    QString itemName = "Item_Name";
    if(lang==1){
        grpName = "Group_Name_Arabic";
        itemName = "Item_Name_Arabic";
    }
    salesVoucherDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper itemHelper;
    QSqlQueryModel* model = new QSqlQueryModel;

    QString qStr = "";
    QString     timestamp = "str_to_Date(concat(main." + shelper.Sales_Invoice_Main_Voucher_Date ;
    timestamp += ", substr(" + shelper.Sales_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;

    qStr += "  select Ind,ID,itemGrp.Group_Name as Name,";
    if(godownId == "%")
        qStr += " SUM(Quantity) as Quantity, SUM(Subtotal) as Value ";
    else {
        qStr += " SUM(trans.cr_qty) as Quantity, SUM(trans.cr_amount) as Value ";
    }
    qStr += " from sales_invoice_details det";
    if(godownId != "%"){
        qStr += " inner join (select ItemID, VoucherID, voucherPrefix, voucherType, Requirement_ItemID, SUM(cr_qty) cr_qty, Sum(cr_amount) cr_amount FROM transaction_item_helper trans ";
        qStr += " where from_location like '" + godownId + "' group by ItemID, VoucherID, voucherPrefix ) trans ";
        qStr += " ON trans.ItemID = det.Inventory_Item_Id AND trans.VoucherType = 'SALESVOUCHER' ";
        qStr += " AND trans.VoucherID = det.Voucher_No AND trans.VoucherPrefix = det.Voucher_Prefix";
        qStr += " AND trans.requirement_itemID = det.requirement_itemID";
    }
    qStr += " inner join ";
    if(groupID !="0"){
        qStr += " (select ItemGrp1.group_name, Item.item_id, 'G' as Ind,ItemGrp1.group_id as ID from ";
        qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
        qStr += " inner join ";
        qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
        qStr += "  select     grp.group_id,grp." + grpName + ",grp.parent_id, item.item_id";
        qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  grp";
        qStr += "  on item.group_id = grp.group_id";
        qStr += "  union all ";
        qStr += "  select     p.group_id, p." + grpName + ",p.parent_id, item_id";
        qStr += "  from      " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  p";
        qStr += "  inner join cte ";
        qStr += "  on p.group_id = cte.parent_id )";
        qStr += " select cte.group_name, cte.item_id, cte.group_id ";
        qStr += " from cte where cte.parent_id = '"+ groupID +"'";
        qStr += " ) itemGrp1 ";
        qStr += " on itemGrp1.item_id = item.item_id ";
        qStr += " union ";
        qStr += " select Item2."+itemName+", Item2.Item_id, 'I' as ind,Item2.Item_id as ID  from ";
        qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
        qStr += " where Item2.group_id = '"+ groupID +"'";
        qStr += " ) itemGrp ";
    }
    else {
        qStr += "(SELECT (SELECT " + grpName + " from " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName;
        qStr += " grp WHERE group_id = inv.group_id) as group_name, inv.item_id, 'G' as Ind, inv.group_id as ID from ";
        qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" inv) itemGrp";
    }
    qStr += " on det.inventory_item_id = itemGrp.item_id ";
    //    qStr += " where det.Voucher_Date >= '" + fromDate.toString("yyyy-MM-dd") + "' and det.Voucher_Date <= '" + toDate.toString("yyyy-MM-dd") + "'";
    qStr += " where det.Voucher_no in (" ;
    qStr += "SELECT "+shelper.Sales_Invoice_Main_Voucher_No ;
    //    qStr += ", "+shelper.Sales_Invoice_Main_Reference;
    qStr += " from ";
    qStr += shelper.Sales_Invoice_Main_TableName + " main ";
    qStr += " where ";
    qStr += " ( ";
    qStr += timestamp + ">=" + db->getDateTimeString(fromDate);
    qStr += " and ";
    qStr += timestamp + "<= " + db->getDateTimeString(toDate);
    qStr += ")";
    qStr += " )" ;
    //    qStr += " AND trans.from_Location like '" + godownId + "'";

    qStr += " group by itemGrp.group_name";
    qStr += " ORDER BY 1,2";

    //    query.prepare(qStr);
    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}

QSqlQueryModel *InventoryHelper::getGroupWiseWODetailsByGroup(QString groupID,
                                                                 QDateTime fromDate, QDateTime toDate,
                                                                 QString godownId )
{
    WorkOrderDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper itemHelper;
    QSqlQueryModel* model = new QSqlQueryModel;

    QString qStr = "";
    QString     timestamp = "str_to_Date(concat(main." + shelper.Work_Order_Main_Voucher_Date ;
    timestamp += ", substr(" + shelper.Work_Order_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;

    qStr += "  select Ind,ID,itemGrp.group_name as Name,";
    if(godownId == "%")
        qStr += " SUM(Quantity) as Quantity, SUM(Quantity* det.Price) as Value ";
    else {
        qStr += " SUM(trans.cr_qty) as Quantity, SUM(trans.cr_amount) as Value ";
    }
    qStr += " from work_order_Detail det";
    if(godownId != "%"){
        qStr += " inner join (select ItemID, VoucherID, voucherPrefix, voucherType, Requirement_ItemID, SUM(cr_qty) cr_qty, Sum(cr_amount) cr_amount FROM transaction_item_helper trans ";
        qStr += " where from_location like '" + godownId + "' group by ItemID, VoucherID, voucherPrefix ) trans ";
        qStr += " ON trans.ItemID = det." + shelper.WorkOrder_itemID + " AND ";
        qStr += "(trans.VoucherType = '" + shelper.VoucherType + "' or trans.VoucherType = 'CONSUMPTION') ";
        qStr += " AND trans.VoucherID = det.Voucher_No AND trans.VoucherPrefix = det.Voucher_Prefix";
        qStr += " AND trans.requirement_itemID = det.requirement_itemID";
    }
    qStr += " inner join ";
    if(groupID !="0"){
        qStr += " (select ItemGrp1.group_name, Item.item_id, 'G' as Ind,ItemGrp1.group_id as ID from ";
        qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
        qStr += " inner join ";
        qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
        qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
        qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  grp";
        qStr += "  on item.group_id = grp.group_id";
        qStr += "  union all ";
        qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
        qStr += "  from      " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  p";
        qStr += "  inner join cte ";
        qStr += "  on p.group_id = cte.parent_id )";
        qStr += " select cte.group_name, cte.item_id, cte.group_id ";
        qStr += " from cte where cte.parent_id = '"+ groupID +"'";
        qStr += " ) itemGrp1 ";
        qStr += " on itemGrp1.item_id = item.item_id ";
        qStr += " union ";
        qStr += " select Item2.Item_name, Item2.Item_id, 'I' as ind,Item2.Item_id as ID  from ";
        qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
        qStr += " where Item2.group_id = '"+ groupID +"'";
        qStr += " ) itemGrp ";
    }
    else {
        qStr += "(SELECT (SELECT group_Name from " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName;
        qStr += " grp WHERE group_id = inv.group_id) as group_name, inv.item_id, 'G' as Ind, inv.group_id as ID from ";
        qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" inv) itemGrp";
    }
    qStr += " on det." + shelper.WorkOrder_itemID + " = itemGrp.item_id ";
    //    qStr += " where det.Voucher_Date >= '" + fromDate.toString("yyyy-MM-dd") + "' and det.Voucher_Date <= '" + toDate.toString("yyyy-MM-dd") + "'";
    qStr += " where det." + shelper.WorkOrder_WorkOrder_id + " in (" ;
    qStr += "SELECT "+shelper.Work_Order_Main_Voucher_No ;
    //    qStr += ", "+shelper.Sales_Invoice_Main_Reference;
    qStr += " from ";
    qStr += shelper.Work_Order_Main_TableName + " main ";
    qStr += " where ";
    qStr += " ( ";
    qStr += timestamp + ">=" + db->getDateTimeString(fromDate);
    qStr += " and ";
    qStr += timestamp + "<= " + db->getDateTimeString(toDate);
    qStr += ")";
    qStr += " )" ;
    //    qStr += " AND trans.from_Location like '" + godownId + "'";

    qStr += " group by itemGrp.group_name";
    qStr += " ORDER BY 1,2";

    qDebug()<<qStr;
    //    query.prepare(qStr);
    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}

QSqlQueryModel *InventoryHelper::getGroupWiseSalesFilterByLedger(QString groupID,QDateTime fromDate, QDateTime toDate, QString LedgerId)
{
    salesVoucherDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper itemHelper;
    QSqlQueryModel* model = new QSqlQueryModel;

    qDebug()<<"253";
    QString qStr = "";
    QString     timestamp = "str_to_Date(concat(main." + shelper.Sales_Invoice_Main_Voucher_Date ;
    timestamp += ", substr(" + shelper.Sales_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;

    qStr += "  select Ind,ID,itemGrp.group_name as Name,";
    qStr += " SUM(Quantity) as Quantity, SUM(Subtotal) as Value ";
    qStr += " from sales_invoice_details det join sales_invoice_main main on det.voucher_no = main.voucher_no and det.voucher_prefix = main.voucher_prefix ";
    qStr += " inner join ";

    qStr += " (select ItemGrp1.group_name, Item.item_id, 'G' as Ind,ItemGrp1.group_id as ID from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
    qStr += " inner join ";
    qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
    qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
    qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  grp";
    qStr += "  on item.group_id = grp.group_id";
    qStr += "  union all ";
    qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
    qStr += "  from      " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  p";
    qStr += "  inner join cte ";
    qStr += "  on p.group_id = cte.parent_id )";
    qStr += " select cte.group_name, cte.item_id, cte.group_id ";
    qStr += " from cte where cte.parent_id = '"+ groupID +"'";
    qStr += " ) itemGrp1 ";
    qStr += " on itemGrp1.item_id = item.item_id ";
    qStr += " union ";
    qStr += " select Item2.Item_name, Item2.Item_id, 'I' as ind,Item2.Item_id as ID  from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
    qStr += " where Item2.group_id = '"+ groupID +"'";
    qStr += " ) itemGrp ";
    qStr += " on det.inventory_item_id = itemGrp.item_id ";

    qStr += " where ";
    qStr += " ( ";
    qStr += timestamp + ">=" + db->getDateTimeString(fromDate);
    qStr += " and ";
    qStr += timestamp + "<= " + db->getDateTimeString(toDate);
    qStr += ")";
    qStr += " and main.ledger_id = '" + LedgerId + "'";

    qStr += " group by itemGrp.group_name";
    qStr += " ORDER BY 1,2";

    qDebug()<<qStr;
    //    query.prepare(qStr);
    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}

QSqlQueryModel *InventoryHelper::getGroupWisePurchaseFilterByLedger(QString groupID,QDateTime fromDate, QDateTime toDate, QString LedgerId)
{
    PurchaseVoucherDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper itemHelper;
    QSqlQueryModel* model = new QSqlQueryModel;

    qDebug()<<"253";
    QString qStr = "";
    QString     timestamp = "str_to_Date(concat(main." + shelper.Purchase_Invoice_Main_Voucher_Date ;
    timestamp += ", substr(" + shelper.Purchase_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;

    qStr += "  select Ind,ID,itemGrp.group_name as Name,";
    qStr += " SUM(Quantity) as Quantity, SUM(Subtotal) as Value ";
    qStr += " from Purchase_invoice_details det join Purchase_invoice_main main on det.voucher_no = main.voucher_no and det.voucher_prefix = main.voucher_prefix ";
    qStr += " inner join ";

    qStr += " (select ItemGrp1.group_name, Item.item_id, 'G' as Ind,ItemGrp1.group_id as ID from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
    qStr += " inner join ";
    qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
    qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
    qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  grp";
    qStr += "  on item.group_id = grp.group_id";
    qStr += "  union all ";
    qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
    qStr += "  from      " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  p";
    qStr += "  inner join cte ";
    qStr += "  on p.group_id = cte.parent_id )";
    qStr += " select cte.group_name, cte.item_id, cte.group_id ";
    qStr += " from cte where cte.parent_id = '"+ groupID +"'";
    qStr += " ) itemGrp1 ";
    qStr += " on itemGrp1.item_id = item.item_id ";
    qStr += " union ";
    qStr += " select Item2.Item_name, Item2.Item_id, 'I' as ind,Item2.Item_id as ID  from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
    qStr += " where Item2.group_id = '"+ groupID +"'";
    qStr += " ) itemGrp ";
    qStr += " on det.inventory_item_id = itemGrp.item_id ";

    qStr += " where ";
    qStr += " ( ";
    qStr += timestamp + ">=" + db->getDateTimeString(fromDate);
    qStr += " and ";
    qStr += timestamp + "<= " + db->getDateTimeString(toDate);
    qStr += ")";
    qStr += " and main.ledger_id = '" + LedgerId + "'";

    qStr += " group by itemGrp.group_name";
    qStr += " ORDER BY 1,2";

    qDebug()<<qStr;
    //    query.prepare(qStr);
    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}

QSqlQueryModel *InventoryHelper::getDetailedSalesByGroup(QString groupID,
                                                         QDateTime fromDate, QDateTime toDate,
                                                         QString godownId )
{
    int lang = ConfigurationSettingsDatabaseHelper::getValue(POSLang,0).toInt();
    QString grpName = "Group_Name";
    QString itemName = "Item_Name";
    if(lang==1){
        grpName = "Group_Name_Arabic";
        itemName = "Item_Name_Arabic";
    }
    salesVoucherDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper itemHelper;
    QSqlQueryModel* model = new QSqlQueryModel;

    qDebug()<<"253";
    QString qStr = "";
    QString     timestamp = "str_to_Date(concat(main." + shelper.Sales_Invoice_Main_Voucher_Date ;
    timestamp += ", substr(" + shelper.Sales_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;

    qStr += "  select 'I' Ind, det.inventory_item_id ID,";
    qStr += " (select item_name FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where item_id = det.inventory_item_id ) as Item,";
    if(godownId == "%")
        qStr += " SUM(Quantity) as Quantity, SUM(Subtotal) as Value ";
    else {
        qStr += " SUM(trans.cr_qty) as Quantity, SUM(trans.cr_amount) as Value ";
    }
    qStr += " from sales_invoice_details det";
    if(godownId != "%"){
        qStr += " inner join (select ItemID, VoucherID, voucherPrefix, voucherType, Requirement_ItemID, SUM(cr_qty) cr_qty, Sum(cr_amount) cr_amount FROM transaction_item_helper trans ";
        qStr += " where from_location like '" + godownId + "' group by ItemID, VoucherID, voucherPrefix ) trans ";
        qStr += " ON trans.ItemID = det.Inventory_Item_Id AND trans.VoucherType = 'SALESVOUCHER' ";
        qStr += " AND trans.VoucherID = det.Voucher_No AND trans.VoucherPrefix = det.Voucher_Prefix";
        qStr += " AND trans.requirement_itemID = det.requirement_itemID";
    }
    if(groupID != "0"){
        qStr += " inner join ";
        qStr += " (select ItemGrp1.group_name, Item.item_id, 'G' as Ind,ItemGrp1.group_id as ID from ";
        qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
        qStr += " inner join ";
        qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
        qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
        qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  grp";
        qStr += "  on item.group_id = grp.group_id";
        qStr += "  union all ";
        qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
        qStr += "  from      " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  p";
        qStr += "  inner join cte ";
        qStr += "  on p.group_id = cte.parent_id )";
        qStr += " select cte.group_name, cte.item_id, cte.group_id ";
        qStr += " from cte where cte.parent_id = '"+ groupID +"'";
        qStr += " ) itemGrp1 ";
        qStr += " on itemGrp1.item_id = item.item_id ";
        qStr += " union ";
        qStr += " select Item2.Item_name, Item2.Item_id, 'I' as ind,Item2.Item_id as ID  from ";
        qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
        qStr += " where Item2.group_id = '"+ groupID +"'";
        qStr += " ) itemGrp ";
        qStr += " on det.inventory_item_id = itemGrp.item_id ";
    }

    //    qStr += " where det.Voucher_Date >= '" + fromDate.toString("yyyy-MM-dd") + "' and det.Voucher_Date <= '" + toDate.toString("yyyy-MM-dd") + "'";
    qStr += " where det.Voucher_no in (" ;
    qStr += "SELECT "+shelper.Sales_Invoice_Main_Voucher_No ;
    //    qStr += ", "+shelper.Sales_Invoice_Main_Reference;
    qStr += " from ";
    qStr += shelper.Sales_Invoice_Main_TableName + " main ";
    qStr += " where ";
    qStr += " ( ";
    qStr += timestamp + ">=" + db->getDateTimeString(fromDate);
    qStr += " and ";
    qStr += timestamp + "<= " + db->getDateTimeString(toDate);
    qStr += ")";
    qStr += " )" ;
    qStr += " group by det.inventory_item_id";
    qStr += " ORDER BY 3";

    qDebug()<<qStr;
    //    query.prepare(qStr);
    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}

QSqlQueryModel *InventoryHelper::getDetailedSalesByGroupFilterByLedger(QString groupID,QDateTime fromDate, QDateTime toDate, QString LedgerId)
{
    salesVoucherDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper itemHelper;
    QSqlQueryModel* model = new QSqlQueryModel;

    QString qStr = "";
    QString     timestamp = "str_to_Date(concat(main." + shelper.Sales_Invoice_Main_Voucher_Date ;
    timestamp += ", substr(" + shelper.Sales_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;

    qStr += "  select 'I' Ind, det.inventory_item_id ID,";
    qStr += " (select item_name FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where item_id = det.inventory_item_id ) as Item,";
    qStr += " SUM(Quantity) as Quantity, SUM(Subtotal) as Value ";
    qStr += " from sales_invoice_details det join sales_invoice_main main on det.voucher_no = main.voucher_no and det.voucher_prefix = main.voucher_prefix ";
    qStr += " inner join ";
    qStr += " (select ItemGrp1.group_name, Item.item_id, 'G' as Ind,ItemGrp1.group_id as ID from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
    qStr += " inner join ";
    qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
    qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
    qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  grp";
    qStr += "  on item.group_id = grp.group_id";
    qStr += "  union all ";
    qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
    qStr += "  from      " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  p";
    qStr += "  inner join cte ";
    qStr += "  on p.group_id = cte.parent_id )";
    qStr += " select cte.group_name, cte.item_id, cte.group_id ";
    qStr += " from cte where cte.parent_id = '"+ groupID +"'";
    qStr += " ) itemGrp1 ";
    qStr += " on itemGrp1.item_id = item.item_id ";
    qStr += " union ";
    qStr += " select Item2.Item_name, Item2.Item_id, 'I' as ind,Item2.Item_id as ID  from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
    qStr += " where Item2.group_id = '"+ groupID +"'";
    qStr += " ) itemGrp ";
    qStr += " on det.inventory_item_id = itemGrp.item_id ";
    qStr += " where ";
    qStr += " ( ";
    qStr += timestamp + ">=" + db->getDateTimeString(fromDate);
    qStr += " and ";
    qStr += timestamp + "<= " + db->getDateTimeString(toDate);
    qStr += ")";
    qStr += " and main.Ledger_ID = '" + LedgerId + "'";

    qStr += " group by det.inventory_item_id";
    qStr += " ORDER BY 3";

    qDebug()<<qStr;
    //    query.prepare(qStr);
    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}


QSqlQueryModel *InventoryHelper::getDetailedPurchaseByGroupFilterByLedger(QString groupID,QDateTime fromDate, QDateTime toDate, QString LedgerId)
{
    PurchaseVoucherDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper itemHelper;
    QSqlQueryModel* model = new QSqlQueryModel;

    QString qStr = "";
    QString     timestamp = "str_to_Date(concat(main." + shelper.Purchase_Invoice_Main_Voucher_Date ;
    timestamp += ", substr(" + shelper.Purchase_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;

    qStr += "  select 'I' Ind, det.inventory_item_id ID,";
    qStr += " (select item_name FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where item_id = det.inventory_item_id ) as Item,";
    qStr += " SUM(Quantity) as Quantity, SUM(Subtotal) as Value ";
    qStr += " from Purchase_invoice_details det join Purchase_invoice_main main on det.voucher_no = main.voucher_no and det.voucher_prefix = main.voucher_prefix ";
    qStr += " inner join ";
    qStr += " (select ItemGrp1.group_name, Item.item_id, 'G' as Ind,ItemGrp1.group_id as ID from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
    qStr += " inner join ";
    qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
    qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
    qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  grp";
    qStr += "  on item.group_id = grp.group_id";
    qStr += "  union all ";
    qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
    qStr += "  from      " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  p";
    qStr += "  inner join cte ";
    qStr += "  on p.group_id = cte.parent_id )";
    qStr += " select cte.group_name, cte.item_id, cte.group_id ";
    qStr += " from cte where cte.parent_id = '"+ groupID +"'";
    qStr += " ) itemGrp1 ";
    qStr += " on itemGrp1.item_id = item.item_id ";
    qStr += " union ";
    qStr += " select Item2.Item_name, Item2.Item_id, 'I' as ind,Item2.Item_id as ID  from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
    qStr += " where Item2.group_id = '"+ groupID +"'";
    qStr += " ) itemGrp ";
    qStr += " on det.inventory_item_id = itemGrp.item_id ";
    qStr += " where ";
    qStr += " ( ";
    qStr += timestamp + ">=" + db->getDateTimeString(fromDate);
    qStr += " and ";
    qStr += timestamp + "<= " + db->getDateTimeString(toDate);
    qStr += ")";
    qStr += " and main.Ledger_ID = '" + LedgerId + "'";

    qStr += " group by det.inventory_item_id";
    qStr += " ORDER BY 3";

    qDebug()<<qStr;
    //    query.prepare(qStr);
    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}

QSqlQueryModel *InventoryHelper::getGroupwiseTransactionByVoucherType(QString voucherType, QString groupID, QDateTime fromDate, QDateTime toDate, QString ledgerID)
{

    PurchaseVoucherDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper itemHelper;
    QSqlQueryModel* model = new QSqlQueryModel;

    qDebug()<<"253";
    QString qStr = "";
    QString     timestamp = "str_to_Date(addTime(det.VoucherDate, det.Time) ,'%Y-%m-%d %H:%i:%s')" ;

    qStr += "  select Ind,ID,itemGrp.group_name as Name,";
    qStr += " ABS(SUM(CR_QTY - DR_QTY)) as Quantity, ABS(SUM(CR_AMOUNT - DR_AMOUNT)) as Value ";
    qStr += " from Transaction_Item_Helper det ";
    //    qStr += " join Transaction_Helper main ";
    //    qStr += " on det.voucherID = main.voucher_id and det.voucherprefix = main.voucher_prefix and det.voucherType = main.voucher_type";
    qStr += " inner join ";

    qStr += " (select ItemGrp1.group_name, Item.item_id, 'G' as Ind,ItemGrp1.group_id as ID from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
    qStr += " inner join ";
    qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
    qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
    qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  grp";
    qStr += "  on item.group_id = grp.group_id";
    qStr += "  union all ";
    qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
    qStr += "  from      " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  p";
    qStr += "  inner join cte ";
    qStr += "  on p.group_id = cte.parent_id )";
    qStr += " select cte.group_name, cte.item_id, cte.group_id ";
    qStr += " from cte where cte.parent_id = '"+ groupID +"'";
    qStr += " ) itemGrp1 ";
    qStr += " on itemGrp1.item_id = item.item_id ";
    qStr += " union ";
    qStr += " select Item2.Item_name, Item2.Item_id, 'I' as ind,Item2.Item_id as ID  from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
    qStr += " where Item2.group_id = '"+ groupID +"'";
    qStr += " ) itemGrp ";
    qStr += " on det.ItemID = itemGrp.item_id ";

    qStr += " where ";
    qStr += " ( ";
    qStr += timestamp + ">=" + db->getDateTimeString(fromDate);
    qStr += " and ";
    qStr += timestamp + "<= " + db->getDateTimeString(toDate);
    qStr += ")";
    if(ledgerID != "%")
        qStr += " and det.to_ledger like '" + ledgerID + "'";

    qStr += " AND det.voucherType = '" + voucherType + "'";
    qStr += " group by itemGrp.group_name";
    qStr += " ORDER BY 1,3";

    qDebug()<<qStr;
    //    query.prepare(qStr);
    model = DatabaseHelper::getQueryModel(qStr);
    return model;


}

QSqlQueryModel *InventoryHelper::getDetailedPurchaseByGroup(QString groupID,QDateTime fromDate, QDateTime toDate)
{
    PurchaseVoucherDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper itemHelper;
    QSqlQueryModel* model = new QSqlQueryModel;

    qDebug()<<"253";
    QString qStr = "";
    QString     timestamp = "str_to_Date(concat(main." + shelper.Purchase_Invoice_Main_Voucher_Date ;
    timestamp += ", substr(" + shelper.Purchase_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;

    qStr += "  select 'I' Ind, det.inventory_item_id ID,";
    qStr += " (select item_name FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where item_id = det.inventory_item_id ) as Item,";
    qStr += " SUM(Quantity) as Quantity, SUM(Subtotal) as Value ";
    qStr += " from purchase_invoice_details det";
    qStr += " inner join ";
    qStr += " (select ItemGrp1.group_name, Item.item_id, 'G' as Ind,ItemGrp1.group_id as ID from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
    qStr += " inner join ";
    qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
    qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
    qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  grp";
    qStr += "  on item.group_id = grp.group_id";
    qStr += "  union all ";
    qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
    qStr += "  from      " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  p";
    qStr += "  inner join cte ";
    qStr += "  on p.group_id = cte.parent_id )";
    qStr += " select cte.group_name, cte.item_id, cte.group_id ";
    qStr += " from cte where cte.parent_id = '"+ groupID +"'";
    qStr += " ) itemGrp1 ";
    qStr += " on itemGrp1.item_id = item.item_id ";
    qStr += " union ";
    qStr += " select Item2.Item_name, Item2.Item_id, 'I' as ind,Item2.Item_id as ID  from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
    qStr += " where Item2.group_id = '"+ groupID +"'";
    qStr += " ) itemGrp ";
    qStr += " on det.inventory_item_id = itemGrp.item_id ";
    //    qStr += " where det.Voucher_Date >= '" + fromDate.toString("yyyy-MM-dd") + "' and det.Voucher_Date <= '" + toDate.toString("yyyy-MM-dd") + "'";
    qStr += " where det.Voucher_no in (" ;
    qStr += "SELECT "+shelper.Purchase_Invoice_Main_Voucher_No ;
    //    qStr += ", "+shelper.Sales_Invoice_Main_Reference;
    qStr += " from ";
    qStr += shelper.Purchase_Invoice_Main_TableName + " main ";
    qStr += " where ";
    qStr += " ( ";
    qStr += timestamp + ">=" + db->getDateTimeString(fromDate);
    qStr += " and ";
    qStr += timestamp + "<= " + db->getDateTimeString(toDate);
    qStr += ")";
    qStr += " )" ;

    qStr += " group by det.inventory_item_id";
    qStr += " ORDER BY 3";

    qDebug()<<qStr;
    //    query.prepare(qStr);
    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}

QSqlQueryModel *InventoryHelper::getDetailedWOByGroup(QString groupID,QDateTime fromDate, QDateTime toDate)
{
    WorkOrderDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper itemHelper;
    QSqlQueryModel* model = new QSqlQueryModel;

    qDebug()<<"253";
    QString qStr = "";
    QString     timestamp = "str_to_Date(concat(main." + shelper.Work_Order_Main_Voucher_Date ;
    timestamp += ", substr(" + shelper.WorkOrder_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;

    qStr += "  select 'I' Ind, det." + shelper.WorkOrder_itemID + " ID,";
    qStr += " (select item_name FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where item_id = det." + shelper.WorkOrder_itemID + " ) as Item,";
    qStr += " SUM(Quantity) as Quantity, SUM(Quantity * det.Price) as Value ";
    qStr += " from work_order_detail det";
    qStr += " inner join ";
    qStr += " (select ItemGrp1.group_name, Item.item_id, 'G' as Ind,ItemGrp1.group_id as ID from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
    qStr += " inner join ";
    qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
    qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
    qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  grp";
    qStr += "  on item.group_id = grp.group_id";
    qStr += "  union all ";
    qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
    qStr += "  from      " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  p";
    qStr += "  inner join cte ";
    qStr += "  on p.group_id = cte.parent_id )";
    qStr += " select cte.group_name, cte.item_id, cte.group_id ";
    qStr += " from cte where cte.parent_id = '"+ groupID +"'";
    qStr += " ) itemGrp1 ";
    qStr += " on itemGrp1.item_id = item.item_id ";
    qStr += " union ";
    qStr += " select Item2.Item_name, Item2.Item_id, 'I' as ind,Item2.Item_id as ID  from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
    qStr += " where Item2.group_id = '"+ groupID +"'";
    qStr += " ) itemGrp ";
    qStr += " on det." + shelper.WorkOrder_itemID + " = itemGrp.item_id ";
    //    qStr += " where det.Voucher_Date >= '" + fromDate.toString("yyyy-MM-dd") + "' and det.Voucher_Date <= '" + toDate.toString("yyyy-MM-dd") + "'";
    qStr += " where det." + shelper.WorkOrder_WorkOrder_id + " in (" ;
    qStr += "SELECT "+shelper.Work_Order_Main_Voucher_No ;
    //    qStr += ", "+shelper.Sales_Invoice_Main_Reference;
    qStr += " from ";
    qStr += shelper.Work_Order_Main_TableName + " main ";
    qStr += " where ";
    qStr += " ( ";
    qStr += timestamp + ">=" + db->getDateTimeString(fromDate);
    qStr += " and ";
    qStr += timestamp + "<= " + db->getDateTimeString(toDate);
    qStr += ")";
    qStr += " )" ;

    qStr += " group by det." + shelper.WorkOrder_itemID + "";
    qStr += " ORDER BY 3";

    qDebug()<<qStr;
    //    query.prepare(qStr);
    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}

QSqlQueryModel *InventoryHelper::getDetailedTransactionByGroupByVoucherType(QString voucherType,QString groupID,QDateTime fromDate, QDateTime toDate, QString ledgerID)
{
    PurchaseVoucherDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper itemHelper;
    QSqlQueryModel* model = new QSqlQueryModel;

    qDebug()<<"253";
    QString qStr = "";
    QString     timestamp = "str_to_Date(addTime(det.VoucherDate, det.Time) ,'%Y-%m-%d %H:%i:%s')" ;

    qStr += "  select 'I' Ind, det.itemid ID,";
    qStr += " (select item_name FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where item_id = det.itemID ) as Item,";
    qStr += " ABS(SUM(CR_QTY - DR_QTY)) as Quantity, ABS(SUM(CR_AMOUNT - DR_AMOUNT)) as Value ";
    qStr += " from transaction_item_helper det";
    qStr += " inner join ";
    qStr += " (select ItemGrp1.group_name, Item.item_id, 'G' as Ind,ItemGrp1.group_id as ID from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
    qStr += " inner join ";
    qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
    qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
    qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  grp";
    qStr += "  on item.group_id = grp.group_id";
    qStr += "  union all ";
    qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
    qStr += "  from      " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  p";
    qStr += "  inner join cte ";
    qStr += "  on p.group_id = cte.parent_id )";
    qStr += " select cte.group_name, cte.item_id, cte.group_id ";
    qStr += " from cte where cte.parent_id = '"+ groupID +"'";
    qStr += " ) itemGrp1 ";
    qStr += " on itemGrp1.item_id = item.item_id ";
    qStr += " union ";
    qStr += " select Item2.Item_name, Item2.Item_id, 'I' as ind,Item2.Item_id as ID  from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
    qStr += " where Item2.group_id = '"+ groupID +"'";
    qStr += " ) itemGrp ";
    qStr += " on det.itemid = itemGrp.item_id ";
    //    qStr += " where det.Voucher_Date >= '" + fromDate.toString("yyyy-MM-dd") + "' and det.Voucher_Date <= '" + toDate.toString("yyyy-MM-dd") + "'";
    qStr += " where ";
    qStr += " ( ";
    qStr += timestamp + ">=" + db->getDateTimeString(fromDate);
    qStr += " and ";
    qStr += timestamp + "<= " + db->getDateTimeString(toDate);
    qStr += ")";

    if(ledgerID != "%")
        qStr += " and det.to_ledger like '" + ledgerID + "'";

    qStr += " AND det.voucherType = '" + voucherType + "'";
    qStr += " group by det.itemID";
    qStr += " ORDER BY 3";

    qDebug()<<qStr;
    //    query.prepare(qStr);
    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}


QSqlQueryModel *InventoryHelper::getGroupWisePurchaseDetailsByGroup(QString groupID,QDateTime fromDate, QDateTime toDate)
{
    PurchaseVoucherDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper itemHelper;
    QSqlQueryModel* model = new QSqlQueryModel;

    QString qStr;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    QString     timestamp = "str_to_Date(concat(main." + shelper.Purchase_Invoice_Main_Voucher_Date ;
    timestamp += ", substr(" + shelper.Purchase_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;

    qStr += "  select Ind,ID,itemGrp.group_name as Name,";
    qStr += " SUM(Quantity) as Quantity, SUM(Subtotal) as Amount ";
    qStr += " from Purchase_invoice_details det";
    qStr += " inner join ";
    qStr += " (select ItemGrp1.group_name, Item.item_id, 'G' as Ind,ItemGrp1.group_id as ID from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
    qStr += " inner join ";
    qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
    qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
    qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  grp";
    qStr += "  on item.group_id = grp.group_id";
    qStr += "  union all ";
    qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
    qStr += "  from      " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  p";
    qStr += "  inner join cte ";
    qStr += "  on p.group_id = cte.parent_id )";
    qStr += " select cte.group_name, cte.item_id, cte.group_id ";
    qStr += " from cte where cte.parent_id = '"+ groupID +"'";
    qStr += " ) itemGrp1 ";
    qStr += " on itemGrp1.item_id = item.item_id ";
    qStr += " union ";
    qStr += " select Item2.Item_name, Item2.Item_id, 'I' as ind,Item2.Item_id as ID  from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
    qStr += " where Item2.group_id = '"+ groupID +"'";
    qStr += " ) itemGrp ";
    qStr += " on det.inventory_item_id = itemGrp.item_id ";
    //    qStr += " where det.Voucher_Date >= '" + fromDate.toString("yyyy-MM-dd") + "' and det.Voucher_Date <= '" + toDate.toString("yyyy-MM-dd") + "'";
    qStr += " where det.Voucher_no in (" ;
    qStr += "SELECT "+shelper.Purchase_Invoice_Main_Voucher_No ;
    //    qStr += ", "+shelper.Sales_Invoice_Main_Reference;
    qStr += " from ";
    qStr += shelper.Purchase_Invoice_Main_TableName + " main ";
    qStr += " where ";
    qStr += " ( ";
    qStr += timestamp + ">=" + db->getDateTimeString(fromDate);
    qStr += " and ";
    qStr += timestamp + "<= " + db->getDateTimeString(toDate);
    qStr += ")";
    qStr += " )" ;

    qStr += " group by itemGrp.group_name";
    qStr += " ORDER BY 1,2";

    qDebug()<<qStr;
    //    query.prepare(qStr);
    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}

QSqlQueryModel *InventoryHelper::getAllTransactionsByGroup(QString groupID,QDateTime fromDate, QDateTime toDate)
{
    PurchaseVoucherDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper itemHelper;
    QSqlQueryModel* model = new QSqlQueryModel;

    QString qStr;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    //    QString     timestamp = "str_to_Date(concat(main." + shelper.Purchase_Invoice_Main_Voucher_Date ;
    //    timestamp += ", substr(" + shelper.Purchase_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;

    qStr += "  select Ind,ID,itemGrp.group_name as Name,";
    qStr += " SUM(Cr_Qty) as 'CR QTY', SUM(Dr_Qty) as 'DR QTY', SUM(Cr_Amount) as 'CR AMOUNT', SUM(Dr_Amount) as 'DR AMOUNT' ";
    qStr += " from Transaction_Item_Helper det";
    qStr += " inner join ";
    qStr += " (select ItemGrp1.group_name, Item.item_id, 'G' as Ind,ItemGrp1.group_id as ID from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
    qStr += " inner join ";
    qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
    qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
    qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  grp";
    qStr += "  on item.group_id = grp.group_id";
    qStr += "  union all ";
    qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
    qStr += "  from      " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  p";
    qStr += "  inner join cte ";
    qStr += "  on p.group_id = cte.parent_id )";
    qStr += " select cte.group_name, cte.item_id, cte.group_id ";
    qStr += " from cte where cte.parent_id = '"+ groupID +"'";
    qStr += " ) itemGrp1 ";
    qStr += " on itemGrp1.item_id = item.item_id ";
    qStr += " union ";
    qStr += " select Item2.Item_name, Item2.Item_id, 'I' as ind,Item2.Item_id as ID  from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
    qStr += " where Item2.group_id = '"+ groupID +"'";
    qStr += " ) itemGrp ";
    qStr += " on det.itemID = itemGrp.item_id ";
    qStr += " where det.VoucherDate >= '" + fromDate.toString("yyyy-MM-dd") + "' and det.VoucherDate <= '" + toDate.toString("yyyy-MM-dd") + "'";
    //    qStr += " where det.Voucher_no in (" ;
    //    qStr += "SELECT "+shelper.Purchase_Invoice_Main_Voucher_No ;
    //    //    qStr += ", "+shelper.Sales_Invoice_Main_Reference;
    //    qStr += " from ";
    //    qStr += shelper.Purchase_Invoice_Main_TableName + " main ";
    //    qStr += " where ";
    //    qStr += " ( ";
    //    qStr += timestamp + ">=" + db->getDateTimeString(fromDate);
    //    qStr += " and ";
    //    qStr += timestamp + "<= " + db->getDateTimeString(toDate);
    //    qStr += ")";
    qStr += " )" ;

    qStr += " group by itemGrp.group_name";
    qStr += " ORDER BY 1,2";

    qDebug()<<qStr;
    //    query.prepare(qStr);
    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}

QSqlQueryModel *InventoryHelper::getGroupwiseSalesByModeOfService(QString groupID, QDateTime fromDate, QDateTime toDate)
{
    int lang = ConfigurationSettingsDatabaseHelper::getValue(POSLang,0).toInt();
    QString grpName = "Group_Name";
    QString itemName = "Item_Name";
    if(lang==1){
        grpName = "Group_Name_Arabic";
        itemName = "Item_Name_Arabic";
    }

    salesVoucherDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper itemHelper;
    QSqlQueryModel* model = new QSqlQueryModel;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));


    QString     timestamp = "str_to_Date(concat(main." + shelper.Sales_Invoice_Main_Voucher_Date ;
    timestamp += ", substr(" + shelper.Sales_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;


    //    QString qStr = "SELECT 'G' AS IND, ITEMS.GROUP_ID AS ID, ";
    //    qStr += "   (SELECT group_name from " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  WHERE GROUP_ID = ITEMS.GROUP_ID) AS 'NAME', ";
    //    qStr += " MODE_OF_SERVICE, ";
    //    qStr += " SUM(COALESCE(TRANS.Quantity,0)) as Qty  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" ITEMS ";
    //    qStr += " LEFT OUTER JOIN  ";
    //    qStr += " SALES_INVOICE_DETAILS TRANS ";
    //    qStr += " ON  ITEMS.ITEM_ID = TRANS.INVENTORY_ITEM_ID ";
    //    qStr += " INNER JOIN SALES_INVOICE_MAIN MAIN";
    //    qStr += " ON TRANS.VOUCHER_NO = MAIN.VOUCHER_NO AND TRANS.VOUCHER_PREFIX = MAIN.VOUCHER_PREFIX";
    //    qStr += " WHERE GROUP_ID IN  ";
    //    qStr += " (SELECT  GROUP_ID ";
    //    qStr += " FROM    (SELECT * FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +"  ";
    //    qStr += "          ORDER BY PARENT_ID, GROUP_ID) GROUPS_SORTED, ";
    //    qStr += "         (SELECT @PV := '" + groupID + "') INITIALISATION ";
    //    qStr += " WHERE   FIND_IN_SET(PARENT_ID, @PV) ";
    //    qStr += " AND     LENGTH(@PV := CONCAT(@PV, ',', GROUP_ID))) ";
    //    qStr += " AND VOUCHER_DATE >= '" + fromDate.toString("yyyy-MM-dd") + "' AND VOUCHER_DATE <= '" + toDate.toString("yyyy-MM-dd") + "' ";
    //    qStr += " GROUP BY ITEMS.GROUP_ID ";
    //    qStr += "  ";
    //    qStr += " UNION ";
    //    qStr += "  ";
    //    qStr += " SELECT  ITEMS.ITEM_NAME  AS NAME, SUM(COALESCE(TRANS.QUANTITY,0)) as Qty,ITEMS.ITEM_ID AS ID,  'I' AS IDENTIFIER  ";
    //    qStr += " FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" ITEMS ";
    //    qStr += " LEFT OUTER JOIN  ";
    //    qStr += " SALES_INVOICE_DETAILS TRANS ";
    //    qStr += " ON  ITEMS.ITEM_ID = TRANS.INVENTORY_ITEM_ID ";
    //    qStr += " WHERE GROUP_ID = '" + groupID + "'";
    //    qStr += " AND VOUCHER_DATE >= '" + fromDate.toString("yyyy-MM-dd") + "' AND VOUCHER_DATE <= '" + toDate.toString("yyyy-MM-dd") + "' ";
    //    qStr += " GROUP BY ITEMS.ITEM_ID ";

    QString qStr = "";

    //    qStr += " select Ind, ID, Name, "
    qStr += " select Ind, ID, Name, max(DeliveryQty) as 'Delivery \n Qty', max(DeliveryAmount) as 'Delivery \n Value',";
    qStr += " max(TakeAwayQty) as 'TakeAway \n Qty', max(TakeAwayAmount) as 'TakeAway \n Value',";
    qStr += " max(DineInQty) as 'DineIn \n Qty', max(DineInAmount) as 'DineIn \n Value'";
    qStr += " from (";
    qStr += " select Ind,ID,itemGrp.group_name as Name,  ";
    qStr += " (case when Main.Mode_Of_Service = 1 then  SUM(Quantity) else 0 end ) as 'DeliveryQty',";
    qStr += " (case when Main.Mode_Of_Service = 1 then  SUM(Subtotal) else 0 end ) as 'DeliveryAmount',";
    qStr += " (case when Main.Mode_Of_Service = 2 then  SUM(Quantity) else 0 end ) as 'TakeAwayQty' , ";
    qStr += " (case when Main.Mode_Of_Service = 2 then  SUM(Subtotal) else 0 end ) as 'TakeAwayAmount' ,";
    qStr += " (case when Main.Mode_Of_Service = 3 then  SUM(Quantity) else 0 end ) as 'DineInQty' ,";
    qStr += " (case when Main.Mode_Of_Service = 3 then  SUM(Subtotal) else 0 end ) as 'DineInAmount' ,";
    qStr += " (case when Main.Mode_Of_service = 4 then  SUM(Quantity) else 0 end ) as 'StoreQty' ,";
    qStr += " (case when Main.Mode_Of_service = 4 then  SUM(Subtotal) else 0 end ) as 'StoreAmount' ";
    qStr += " from sales_invoice_details det";
    qStr += " INNER JOIN SALES_INVOICE_MAIN MAIN";
    qStr += " ON det.VOUCHER_NO = MAIN.VOUCHER_NO AND det.VOUCHER_PREFIX = MAIN.VOUCHER_PREFIX";
    qStr += " inner join ";
    qStr += " (select ItemGrp1.group_name, Item.item_id, 'G' as Ind,ItemGrp1.group_id as ID from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
    qStr += " inner join ";
    qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
    qStr += "  select     grp.group_id,grp."+grpName+",grp.parent_id, item.item_id";
    qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  grp";
    qStr += "  on item.group_id = grp.group_id";
    qStr += "  union all ";
    qStr += "  select     p.group_id, p."+grpName+",p.parent_id, item_id";
    qStr += "  from      " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  p";
    qStr += "  inner join cte ";
    qStr += "  on p.group_id = cte.parent_id )";
    qStr += " select cte.group_name, cte.item_id, cte.group_id ";
    qStr += " from cte where cte.parent_id = '"+ groupID +"'";
    qStr += " ) itemGrp1 ";
    qStr += " on itemGrp1.item_id = item.item_id ";
    qStr += " union ";
    qStr += " select Item2."+itemName+", Item2.Item_id, 'I' as ind,Item2.Item_id as ID  from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
    qStr += " where Item2.group_id = '"+ groupID +"'";
    qStr += " ) itemGrp ";
    qStr += " on det.inventory_item_id = itemGrp.item_id ";
    //    qStr += " where det.Voucher_Date >= '" + fromDate.toString("yyyy-MM-dd") + "' and det.Voucher_Date <= '" + toDate.toString("yyyy-MM-dd") + "'";
    qStr += " and " + timestamp + ">=" + db->getDateTimeString(fromDate);
    qStr += " and " + timestamp + "<= " + db->getDateTimeString(toDate);
    qStr += " group by itemGrp.group_name, main.mode_of_Service";
    qStr += " ) X group by Name ";
    qStr += " ORDER BY 3";



   qDebug()<<Q_FUNC_INFO<<__LINE__<<qStr;

    //    query.prepare(qStr);

    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}


QSqlQueryModel *InventoryHelper::getDetailedGroupwiseSalesByModeOfService(QString groupID, QDateTime fromDate, QDateTime toDate)
{
    int lang = ConfigurationSettingsDatabaseHelper::getValue(POSLang,0).toInt();
    QString grpName = "Group_Name";
    QString itemName = "Item_Name";
    if(lang==1){
        grpName = "Group_Name_Arabic";
        itemName = "Item_Name_Arabic";
    }
    qDebug()<<Q_FUNC_INFO;
    salesVoucherDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper itemHelper;
    QSqlQueryModel* model = new QSqlQueryModel;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));


    QString     timestamp = "str_to_Date(concat(main." + shelper.Sales_Invoice_Main_Voucher_Date ;
    timestamp += ", substr(" + shelper.Sales_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;

    QString qStr = "";

    qStr += "  select 'I' Ind, ItemID ID,";
    qStr += " (select item_name FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where item_id = ItemID ) as Item,";
    qStr += "  max(DeliveryQty) as 'Delivery \n Qty', max(DeliveryAmount) as 'Delivery \n Value',";
    qStr += " max(TakeAwayQty) as 'TakeAway \n Qty', max(TakeAwayAmount) as 'TakeAway \n Value',";
    qStr += " max(DineInQty) as 'DineIn \n Qty', max(DineInAmount) as 'DineIn \n Value'";
    qStr += " from (";
    qStr += " select Ind,ID,det.inventory_item_id as ItemID,  ";
    qStr += " (case when Main.Mode_Of_Service = 1 then  SUM(Quantity) else 0 end ) as 'DeliveryQty',";
    qStr += " (case when Main.Mode_Of_Service = 1 then  SUM(Subtotal) else 0 end ) as 'DeliveryAmount',";
    qStr += " (case when Main.Mode_Of_Service = 2 then  SUM(Quantity) else 0 end ) as 'TakeAwayQty' , ";
    qStr += " (case when Main.Mode_Of_Service = 2 then  SUM(Subtotal) else 0 end ) as 'TakeAwayAmount' ,";
    qStr += " (case when Main.Mode_Of_Service = 3 then  SUM(Quantity) else 0 end ) as 'DineInQty' ,";
    qStr += " (case when Main.Mode_Of_Service = 3 then  SUM(Subtotal) else 0 end ) as 'DineInAmount' ,";
    qStr += " (case when Main.Mode_Of_service = 4 then  SUM(Quantity) else 0 end ) as 'StoreQty' ,";
    qStr += " (case when Main.Mode_Of_service = 4 then  SUM(Subtotal) else 0 end ) as 'StoreAmount' ";
    qStr += " from sales_invoice_details det";
    qStr += " INNER JOIN SALES_INVOICE_MAIN MAIN";
    qStr += " ON det.VOUCHER_NO = MAIN.VOUCHER_NO AND det.VOUCHER_PREFIX = MAIN.VOUCHER_PREFIX";
    qStr += " inner join ";
    qStr += " (select ItemGrp1.group_name, Item.item_id, 'G' as Ind,ItemGrp1.group_id as ID from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
    qStr += " inner join ";
    qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
    qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
    qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  grp";
    qStr += "  on item.group_id = grp.group_id";
    qStr += "  union all ";
    qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
    qStr += "  from      " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  p";
    qStr += "  inner join cte ";
    qStr += "  on p.group_id = cte.parent_id )";
    qStr += " select cte.group_name, cte.item_id, cte.group_id ";
    qStr += " from cte where cte.parent_id = '"+ groupID +"'";
    qStr += " ) itemGrp1 ";
    qStr += " on itemGrp1.item_id = item.item_id ";
    qStr += " union ";
    qStr += " select Item2.Item_name, Item2.Item_id, 'I' as ind,Item2.Item_id as ID  from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
    qStr += " where Item2.group_id = '"+ groupID +"'";
    qStr += " ) itemGrp ";
    qStr += " on det.inventory_item_id = itemGrp.item_id ";
    //    qStr += " where det.Voucher_Date >= '" + fromDate.toString("yyyy-MM-dd") + "' and det.Voucher_Date <= '" + toDate.toString("yyyy-MM-dd") + "'";
    qStr += " and " + timestamp + ">=" + db->getDateTimeString(fromDate);
    qStr += " and " + timestamp + "<= " + db->getDateTimeString(toDate);
    qStr += " group by det.inventory_item_id, main.mode_of_Service";
    qStr += " ) X group by ItemID ";
    qStr += " ORDER BY 3";

    qDebug()<<Q_FUNC_INFO<<__LINE__<<qStr;

    //    query.prepare(qStr);

    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}

float InventoryHelper::getPurchaseCostAverageByItem(QString itemID)
{
    float cost = 0.0;
    QString qStr = " SELECT round(coalesce(sum(subtotal)/sum(quantity),std_cost),2) FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +"";
    qStr += " left outer join purchase_invoice_details on item_id = inventory_item_id "   ;
    qStr += " WHERE INVENTORY_ITEM_ID = '" + itemID + "'";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);

    if(query.exec())    {
        if(query.first())
            cost = query.value(0).toFloat();
    }

    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }

    return cost;
}

QSqlQueryModel *InventoryHelper::getItemWisePurchaseByDateModel(QDateTime fromDate, QDateTime toDate)
{
    PurchaseVoucherDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper itemHelper;
    QSqlQueryModel* model = new QSqlQueryModel;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    qDebug()<<"253";
    QString qStr = "";
    qStr +="SELECT ";
    qStr += "";
    qStr += "(SELECT "+itemHelper.Sales_Inventory_ItemName+" from "+ itemHelper.Sales_Inventory_Table_Name;
    qStr += " invTab where "+itemHelper.Sales_Inventory_ItemId + "=stable."+shelper.Purchase_Invoice_Details_Inventory_Item_ID;
    qStr += ") as Item,";
    qStr += "SUM("+shelper.Purchase_Invoice_Details_Quantity+") as Quantity" +", ";
    qStr += "SUM("+shelper.Purchase_Invoice_Details_Subtotal+") as Amount";
    qStr += " From ";
    qStr += shelper.Purchase_Invoice_Details_TableName + " stable";
    qStr += " WHERE ";
    qStr += shelper.Purchase_Invoice_Details_Voucher_Date + ">='"+fromDate.toString("yyyy-MM-dd")+"'";
    qStr += " and ";
    qStr += shelper.Purchase_Invoice_Details_Voucher_Date + "<='"+toDate.toString("yyyy-MM-dd")+"'";
    qStr += " GROUP BY ";
    qStr += shelper.Purchase_Invoice_Details_Inventory_Item_ID;
    qStr += " ORDER BY ";
    qStr += "Item";


    qStr = " SELECT ";
    qStr += " (SELECT ITEM_NAME FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" WHERE ITEM_ID = INVENTORY_ITEM_ID) AS Item, ";
    qStr += "  SUM(QUANTITY) as Qty, SUM(SUBTOTAL) as Amount FROM PURCHASE_INVOICE_DETAILS";
    qStr += " WHERE VOUCHER_DATE >= '" + fromDate.toString("yyyy-MM-dd") + "' AND VOUCHER_DATE <= '" + toDate.toString("yyyy-MM-dd") + "'";
    qStr += " GROUP BY INVENTORY_ITEM_ID";
    qStr += " ORDER BY 1 ";

    if(query.exec(qStr)){
        qDebug()<<qStr;
    }



    if(query.exec(qStr)){
        qDebug()<<qStr;
    }

    qDebug()<<qStr;

    //    query.prepare(qStr);

    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}

QSqlQueryModel *InventoryHelper::getItemWisePurchaseOrderByDateModel(QDateTime fromDate, QDateTime toDate)
{
    PurchaseVoucherDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper itemHelper;
    QSqlQueryModel* model = new QSqlQueryModel;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    qDebug()<<"253";
    QString qStr = "";

    qStr = " SELECT ";
    qStr += " (SELECT ITEM_NAME FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" WHERE ITEM_ID = INVENTORY_ITEM_ID) AS Item, ";
    qStr += "  SUM(QUANTITY) as Qty, SUM(SUBTOTAL) as Amount FROM PURCHASE_ORDER_DETAILS";
    qStr += " WHERE VOUCHER_DATE >= '" + fromDate.toString("yyyy-MM-dd") + "' AND VOUCHER_DATE <= '" + toDate.toString("yyyy-MM-dd") + "'";
    qStr += " GROUP BY INVENTORY_ITEM_ID";
    qStr += " ORDER BY 1 ";

    if(query.exec(qStr)){
        qDebug()<<qStr;
    }



    if(query.exec(qStr)){
        qDebug()<<qStr;
    }

    qDebug()<<qStr;

    //    query.prepare(qStr);

    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}

QSqlQueryModel *InventoryHelper::getItemwiseDatewiseTransactionsModel(QDateTime fromDate, QDateTime toDate)
{
    QSqlQueryModel* model = new QSqlQueryModel;
    SalesInventoryItemDatabaseHelper itemHelper ;
    salesVoucherDatabaseHelper shelper;
    QString qStr = "SELECT Item, ";
    QDate date2 = fromDate.date();
    int j=0;
    while( date2 < toDate.date() ){
        qStr += "COALESCE(MAX(date"+QString::number(j)+"),0.00) as '" + date2.toString("yyyy-MM-dd") +"'";
        if (date2 < toDate.date().addDays(-1)){
            qStr += ", ";
        }
        date2 = date2.addDays(1);
        j++;
    }
    qStr += " FROM (SELECT Item, ";
    QDate date1 = fromDate.date();
    int i=0;
    while( date1 < toDate.date() ){
        qStr += "CASE WHEN voucher_Date = '" + date1.toString("yyyy-MM-dd") + "' THEN format(SUM(total),2) END ";
        qStr += "as date" + QString::number(i) ;
        if (date1 < toDate.date().addDays(-1)){
            qStr += ", ";
        }
        date1 = date1.addDays(1);
        i++;
    }
    qStr += " FROM (SELECT ";
    qStr += "(SELECT "+itemHelper.Sales_Inventory_ItemName+" from "+ itemHelper.Sales_Inventory_Table_Name;
    qStr += " invTab where "+itemHelper.Sales_Inventory_ItemId + "=stable."+shelper.Sales_Invoice_Details_Inventory_Item_ID;
    qStr += ") as Item,";
    //        qStr += "DATE(SUBTIME(main." + shelper.Sales_Invoice_Main_TimeStamp + ", '" + startTime.toString("hh:mm:ss") + "')) as Voucher_Date,";
    qStr += shelper.Sales_Invoice_Details_Quantity + " as Quantity" +", ";
    qStr += shelper.Sales_Invoice_Details_Subtotal + " as total";
    qStr += " FROM " + shelper.Sales_Invoice_Details_TableName + " stable";
    qStr += " JOIN " + shelper.Sales_Invoice_Main_TableName + " main ";
    qStr += " ON stable." + shelper.Sales_Invoice_Details_Voucher_No + " = main." + shelper.Sales_Invoice_Main_Voucher_No;
    qStr += " WHERE main." + shelper.Sales_Invoice_Main_TimeStamp + " >='"+fromDate.toString("yyyy-MM-dd hh:mm:ss")+"'";
    qStr += " AND main." + shelper.Sales_Invoice_Main_TimeStamp + " <='"+toDate.toString("yyyy-MM-dd hh:mm:ss")+"'";
    qStr += " ) AS sales" ;
    qStr += " GROUP BY Item, voucher_Date) as final";
    qStr += " GROUP BY Item";
    qStr += " ORDER BY 1";


    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}

QSqlQueryModel *InventoryHelper::getItemwiseDatewisePurchasePriceModel(QDateTime fromDate, QDateTime toDate)
{
    QSqlQueryModel* model = new QSqlQueryModel;
    SalesInventoryItemDatabaseHelper itemHelper ;
    PurchaseVoucherDatabaseHelper shelper;
    QString qStr = "SELECT Item, ";
    QDate date2 = fromDate.date();
    int j=0;
    while( date2 <= toDate.date() ){
        qStr += "COALESCE(MAX(date"+QString::number(j)+"),0.00) as '" + date2.toString("yyyy-MM-dd") +"'";
//        if (date2 < toDate.date().addDays(-1)){
            qStr += ", ";
//        }
        date2 = date2.addDays(1);
        j++;
    }
    qStr.chop(2);
    qStr += " FROM (";


    qStr += " SELECT Item, ";
    QDate date1 = fromDate.date();
    int i=0;
    while( date1 <= toDate.date() ){
        qStr += "CASE WHEN voucher_Date = '" + date1.toString("yyyy-MM-dd") + "' THEN format(AVG(coalesce(price,0)),2) END ";
        qStr += "as date" + QString::number(i) ;
//        if (date1 < toDate.date().addDays(-1)){
            qStr += ", ";
//        }
        date1 = date1.addDays(1);
        i++;
    }
    qStr.chop(2);
    qStr += " FROM (SELECT ";
    qStr += "(SELECT "+itemHelper.Sales_Inventory_ItemName+" from "+ itemHelper.Sales_Inventory_Table_Name;
    qStr += " invTab where "+itemHelper.Sales_Inventory_ItemId + "=stable."+shelper.Purchase_Invoice_Details_Inventory_Item_ID;
    qStr += ") as Item,";
    //        qStr += "DATE(SUBTIME(main." + shelper.Sales_Invoice_Main_TimeStamp + ", '" + startTime.toString("hh:mm:ss") + "')) as Voucher_Date,";
    qStr += shelper.Purchase_Invoice_Details_Price + " as Price, " ;
    qStr += " stable." + shelper.Purchase_Invoice_Main_Voucher_Date ;
    qStr += " FROM " + shelper.Purchase_Invoice_Details_TableName + " stable";
    qStr += " JOIN " + shelper.Purchase_Invoice_Main_TableName + " main ";
    qStr += " ON stable." + shelper.Purchase_Invoice_Details_Voucher_No + " = main." + shelper.Purchase_Invoice_Main_Voucher_No;
    qStr += " WHERE main." + shelper.Purchase_Invoice_Main_TimeStamp + " >='"+fromDate.toString("yyyy-MM-dd hh:mm:ss")+"'";
    qStr += " AND main." + shelper.Purchase_Invoice_Main_TimeStamp + " <='"+toDate.toString("yyyy-MM-dd hh:mm:ss")+"'";
    qStr += " ) AS sales" ;
    qStr += " GROUP BY Item, voucher_Date";
    qStr += " ) as final";
    qStr += " GROUP BY Item";
    qStr += " ORDER BY 1";

    qDebug()<<qStr;
    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}
QSqlQueryModel *InventoryHelper::getItemwiseTransactionSummaryByVoucherType(QDateTime fromDate, QDateTime toDate, int voucherType)
{

    ItemTransactionDatabaseHelper itemHelper;
    SalesInventoryItemDatabaseHelper shelper;
    QSqlQueryModel *model = new QSqlQueryModel();

    QString qStr = "SELECT ";
    qStr += " 'I', " + itemHelper.Transaction_Item_Helper_ItemID + ", ";
    qStr += " (SELECT " + shelper.Sales_Inventory_ItemName + " FROM " + shelper.Sales_Inventory_Table_Name ;
    qStr += " WHERE " + shelper.Sales_Inventory_ItemId + " = " + itemHelper.Transaction_Item_Helper_ItemID + ") as 'Item',";
    qStr += " SUM(" + itemHelper.Transaction_Item_Helper_Cr_Qty + ") as 'Cr Qty',"; //3
    qStr += " SUM(" + itemHelper.Transaction_Item_Helper_Dr_Qty + ") as 'Dr Qty' ,"; //4
    qStr += " SUM(" + itemHelper.Transaction_Item_Helper_Cr_Amount + ") as 'Credit Amount',"; //5
    qStr += " SUM( " + itemHelper.Transaction_Item_Helper_Dr_Amount + ") as 'Debit Amount'"; // 6
    qStr += " FROM " + itemHelper.Transaction_Item_Helper_TableName;
    qStr += " WHERE " + itemHelper.Transaction_Item_Helper_VoucherDate + " >='"+fromDate.toString("yyyy-MM-dd hh:mm:ss")+"'";
    qStr += " AND " + itemHelper.Transaction_Item_Helper_VoucherDate + " <='"+toDate.toString("yyyy-MM-dd hh:mm:ss")+"'";
    qStr += " AND " + itemHelper.Transaction_Item_Helper_VoucherType + " = '" + GMVoucherTypes::getVoucherString(voucherType) + "'";
    qStr += " GROUP BY " + itemHelper.Transaction_Item_Helper_ItemID + " ORDER BY Item";

    return DatabaseHelper::getQueryModel(qStr);

}

QSqlQueryModel *InventoryHelper::getReOrderLevelReport(QDate toDate)
{
    QSqlQueryModel* model = new QSqlQueryModel;
    ItemTransactionDatabaseHelper itemHelper;
    SalesInventoryItemDatabaseHelper shelper;

    QString godownID = "%";
    QString qStr = "";
    qStr += "SELECT " ;
    qStr += "inv." + shelper.Sales_Inventory_ItemId + " as ID,";
    qStr += "inv." + shelper.Sales_Inventory_ItemName + " as Items, ";
    qStr += "inv." + shelper.Sales_Inventory_ReorderLevel + " as Level, ";
    qStr += " coalesce(SO.qty,0) as 'Sales Order\n Pending', ";
    qStr += " inv." + shelper.Sales_Inventory_ReorderLevel + " + COALESCE(SO.qty,0) as 'Required\n Total', ";
    qStr += " coalesce(stock,0) as 'Current Stock' ";
    qStr += " FROM "+ shelper.Sales_Inventory_Table_Name + " inv " ;
    qStr += " left outer join (";

    qStr += " select itemID, round(sum(debitQty - creditQty),3) as stock from ( ";
    qStr += " select trans." + itemHelper.Transaction_Item_Helper_VoucherDate + ", " +  itemHelper.Transaction_Item_Helper_ItemID + ", ";
    qStr += " sum(trans." + itemHelper.Transaction_Item_Helper_Cr_Qty + ") as creditQty, sum(trans." + itemHelper.Transaction_Item_Helper_Dr_Qty + ") as debitQty, 'purchase' as type ";
    qStr += " from " + itemHelper.Transaction_Item_Helper_TableName + " trans where ";
    qStr +=  itemHelper.Transaction_Item_Helper_VoucherDate + " <= " + db->getDateString(toDate);
    //    qStr += " AND " + itemHelper.Transaction_Item_Helper_ItemID + " = '"+ItemID+"' ";
    if(godownID != "%"){
        qStr += " and From_Location like '" + godownID + "' ";
        qStr += " group by " + itemHelper.Transaction_Item_Helper_ItemID;
        qStr += " UNION ALL ";
        qStr += " select trans." + itemHelper.Transaction_Item_Helper_VoucherDate + ", " +  itemHelper.Transaction_Item_Helper_ItemID + ", ";
        qStr += " sum(trans." + itemHelper.Transaction_Item_Helper_Cr_Qty + ") as creditQty, sum(trans." + itemHelper.Transaction_Item_Helper_Dr_Qty + ") as debitQty, 'purchase' as type ";
        qStr += " from " + itemHelper.Transaction_Item_Helper_TableName + " trans where ";
        qStr +=  itemHelper.Transaction_Item_Helper_VoucherDate + " <= " + db->getDateString(toDate);
        //        qStr += " AND " + itemHelper.Transaction_Item_Helper_ItemID + " = '"+ItemID+"' ";
        qStr += " and To_Location like '" + godownID + "' ";
    }
    qStr += " group by " + itemHelper.Transaction_Item_Helper_ItemID;
    qStr += " UNION ALL ";
    qStr += " select trans." + itemHelper.Transaction_Item_Helper_VoucherDate + ", " +  itemHelper.Transaction_Item_Helper_ItemID + ", ";
    qStr += " sum(trans." + itemHelper.Transaction_Item_Helper_Cr_Qty + "), sum(trans." + itemHelper.Transaction_Item_Helper_Dr_Qty + "), 'sales' as type ";
    qStr += " from " + itemHelper.Transaction_Bom_Helper_TableName + " trans where ";
    qStr +=  itemHelper.Transaction_Item_Helper_VoucherDate + " <= " + db->getDateString(toDate);
    //    qStr += " AND " + itemHelper.Transaction_Item_Helper_ItemID + " = '"+ItemID+"' ";
    if(godownID != "%"){
        qStr += " and From_Location like '" + godownID + "' ";
        qStr += " group by " + itemHelper.Transaction_Item_Helper_ItemID;
        qStr += " UNION ALL ";
        qStr += " select trans." + itemHelper.Transaction_Item_Helper_VoucherDate + ", " +  itemHelper.Transaction_Item_Helper_ItemID + ", ";
        qStr += " sum(trans." + itemHelper.Transaction_Item_Helper_Cr_Qty + "), sum(trans." + itemHelper.Transaction_Item_Helper_Dr_Qty + "), 'sales' as type ";
        qStr += " from " + itemHelper.Transaction_Bom_Helper_TableName + " trans where ";
        qStr +=  itemHelper.Transaction_Item_Helper_VoucherDate + " <= " + db->getDateString(toDate);
        //        qStr += " AND " + itemHelper.Transaction_Item_Helper_ItemID + " = '"+ItemID+"' ";
        qStr += " and To_Location like '" + godownID + "' ";
    }
    qStr += " group by " + itemHelper.Transaction_Item_Helper_ItemID;
    qStr += " ) A group by itemID ) stk ";
    qStr += " on inv.item_id = stk.itemID ";
    qStr += " left outer join ";
    qStr += " ( select inventory_item_id, sum(qty) as qty from ";
    qStr += " (select inventory_item_id , sum(quantity) as qty from sales_order_details ";
    qStr += " where itemProductionStatus = " + QString::number(QuotationStatus::SalesOrderCreated) + " group by inventory_item_id";
    qStr += " union select ing.purchase_item, sum(quantity) * ing.purchase_item_qty / ing.sales_item_qty from ";
    qStr += " sales_order_Details inner join  " + BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_TableName +"  ing on inventory_item_id = ing.sales_item ";
    qStr += " where itemProductionStatus = " + QString::number(QuotationStatus::SalesOrderCreated) + " group by purchase_item) X ";
    qStr += " group by inventory_item_id ) SO ";
    qStr += "on inv.Item_ID = SO.inventory_item_id";
    qStr += " WHERE isPurchaseItem = 1";
    qStr += " and coalesce(Reorder_Level ,0) + coalesce(SO.qty ,0) - coalesce(stock ,0) > 0 ";
    qStr +=  " ORDER BY Items";

    //    qStr =  " SELECT INV.item_id,INV.item_name,COALESCE(INV.Opening_Stock,0)-trans.difference";
    //    qStr += " as Stock,INV.Reorder_Level FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" as INV LEFT OUTER JOIN ";
    //    qStr += " (SELECT SUM(COALESCE(Cr_Qty,0)-COALESCE(Dr_Qty,0)) AS difference,itemID FROM ";
    //    qStr += " Transaction_Item_Helper WHERE VoucherDate<= '"+dateTo.toString("yyyy-MM-dd")+"' Group BY ItemID) as ";
    //    qStr += " trans on INV.Item_Id=trans.ItemID where COALESCE(INV.Opening_Stock,0)-trans.difference";
    //    qStr += " <=inv.Reorder_Level";

    qDebug()<<qStr;
    model = DatabaseHelper::getQueryModel(qStr);
//    if(model == nullptr)
        qDebug()<<"Model At Error State : "
                  ""<<model;
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
    qStr += " SELECT " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +".item_name, coalesce(transaction_item_helper.inward_qty,0) as inward_qty,  ";
    qStr += " coalesce(sales_invoice_details.sales_qty*( " + BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_TableName +" .purchase_item_qty/ " + BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_TableName +" .sales_item_qty),0) as issue_qty, ";
    qStr += " Opening.opening_stock as opening_stock, sales_invoice_details.inventory_item_id as sales_item_name ";
    qStr += " FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +"  ";
    qStr += " LEFT OUTER JOIN  ";
    qStr += " (select sum(Dr_Qty)-sum(Cr_Qty) as inward_qty, ItemID from transaction_item_helper ";
    qStr += " where substr(VoucherDate,1,10) >= '"+fromDate.toString("yyyy-MM-dd")+"' and substr(VoucherDate,1,10) <= '"+toDate.toString("yyyy-MM-dd")+"' ";
    qStr += " group by ItemID) transaction_item_helper ";
    qStr += " ON transaction_item_helper.ItemID = " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +".item_id ";
    qStr += " LEFT OUTER JOIN  ";
    qStr += " (select item_name,  opening_stock from  ";
    qStr += " (SELECT item_name, (opening_balance+opening_stock-sum(A_issue_qty)) as opening_stock ";
    qStr += " from  ";
    qStr += " ( ";
    qStr += " SELECT " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +".item_id item_id, " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +".item_name, coalesce(" + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +".opening_stock,0) as opening_balance, ";
    qStr += " coalesce(sales_invoice_details.sales_qty*( " + BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_TableName +" .purchase_item_qty/ " + BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_TableName +" .sales_item_qty),0) as A_issue_qty, ";
    qStr += " coalesce(Opening.stock,0) as opening_stock, sales_invoice_details.inventory_item_id as sales_item_name ";
    qStr += " FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +"  ";
    qStr += " LEFT OUTER JOIN  ";
    qStr += " (select sum(Dr_Qty)-sum(Cr_Qty) as stock, ItemID from transaction_item_helper ";
    qStr += " where substr(VoucherDate,1,10) < '"+fromDate.toString("yyyy-MM-dd")+"' ";
    qStr += " group by ItemID) Opening ";
    qStr += " ON Opening.ItemID = " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +".item_id ";
    qStr += "  LEFT OUTER JOIN  " + BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_TableName +"  ";
    qStr += "  ON " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +".item_id =  " + BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_TableName +" .purchase_item  ";
    qStr += "  LEFT OUTER JOIN   ";
    qStr += " (select sales_invoice_details.inventory_item_id, sum(sales_invoice_details.quantity) sales_qty ";
    qStr += " from  ";
    qStr += "  " + BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_TableName +"  LEFT OUTER JOIN sales_invoice_details ";
    qStr += " ON  " + BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_TableName +" .sales_item = sales_invoice_details.inventory_item_id ";
    qStr += " LEFT OUTER JOIN sales_invoice_main  ";
    qStr += " ON sales_invoice_details.voucher_no = sales_invoice_main.Voucher_No ";
    qStr += " WHERE (( " + BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_TableName +" .TAKEAWAY = 1 and sales_invoice_main.mode_of_service = '3')  ";
    qStr += " or ( " + BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_TableName +" .TAKEAWAY not in (1)) or  " + BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_TableName +" .TAKEAWAY is null) ";
    qStr += " AND  sales_invoice_details.voucher_date  < '"+fromDate.toString("yyyy-MM-dd")+"'  ";
    qStr += " group by inventory_item_id ";
    qStr += " )sales_invoice_details  ";
    qStr += " ON ";
    qStr += " sales_invoice_details.inventory_item_id =  " + BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_TableName +" .sales_item ";
    qStr += " )A ";
    qStr += " group by item_id ";
    qStr += " ) B ";
    qStr += " where opening_stock <> '0' ";
    qStr += " group by item_name ";
    qStr += " order by item_name ";
    qStr += " ) as Opening ";
    qStr += "  ON Opening.item_name = " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +".item_name ";
    qStr += "  LEFT OUTER JOIN  " + BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_TableName +"  ";
    qStr += "  ON " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +".item_id =  " + BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_TableName +" .purchase_item  ";
    qStr += " LEFT OUTER JOIN   ";
    qStr += " (select sales_invoice_details.inventory_item_id, sum(sales_invoice_details.quantity) sales_qty ";
    qStr += " from  ";
    qStr += "  " + BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_TableName +"  LEFT OUTER JOIN sales_invoice_details ";
    qStr += " ON  " + BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_TableName +" .sales_item = sales_invoice_details.inventory_item_id ";
    qStr += " LEFT OUTER JOIN sales_invoice_main  ";
    qStr += " ON sales_invoice_details.voucher_no = sales_invoice_main.Voucher_No ";
    qStr += " WHERE (( " + BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_TableName +" .TAKEAWAY = 1 and sales_invoice_main.mode_of_service = '3')  ";
    qStr += " or ( " + BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_TableName +" .TAKEAWAY not in (1)) or  " + BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_TableName +" .TAKEAWAY is null) ";
    qStr += " AND  sales_invoice_details.voucher_date  >= '"+fromDate.toString("yyyy-MM-dd")+"' and sales_invoice_details.voucher_date <= '"+toDate.toString("yyyy-MM-dd")+"' ";
    qStr += " group by inventory_item_id ";
    qStr += " )sales_invoice_details ";
    qStr += " ON ";
    qStr += " sales_invoice_details.inventory_item_id =  " + BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_TableName +" .sales_item ";
    qStr += " WHERE " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +".isPurchaseItem = 1 ";
    qStr += " )A ";
    qStr += " group by item_name ";






    qDebug()<<qStr;
    model = DatabaseHelper::getQueryModel(qStr);

    return model;

}

QSqlQueryModel *InventoryHelper::getSalesMarginReport(QDateTime fromDate, QDateTime toDate)
{
    QSqlQueryModel* model = new QSqlQueryModel;
    ItemTransactionDatabaseHelper itemHelper;
    salesVoucherDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper items;
    SalesInventoryGroupsDatabaseHelper gpHelper;
    TransactionHelper trans;

    QString qStr = "";

    qStr += " select item_name as 'Item', round(Quantity,3) as 'Sales Qty',";
    qStr += " round(Subtotal,2) as 'Sales Amount', round(Std_Cost,2) as 'Std Cost', ";
    qStr += " round(TotalCost,2) as 'Cost Total',  round(subTotal - Totalcost,2) as 'Margin Amount', ";
    qStr += " round(COALESCE((COALESCE(SubTotal,0)-COALESCE(TotalCost,0))*100/coalesce(TotalCost,0),0),2) ";
    qStr += " as 'Margin %' from ";
    qStr += " (  select inv.item_id, inv.item_name, det.Quantity, det.Subtotal ,  INV.STD_COST , ";
    qStr += " INV.STD_COST * DET.QUANTITY as TotalCost ";
    qStr += " FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name + " inv ";
    qStr += " inner join  (select inventory_item_id, sum(quantity) as Quantity, sum(subTotal) as SubTotal ";
    qStr += " from " + shelper.Sales_Invoice_Details_TableName;
    qStr += " where voucher_Date >= " + db->getDateTimeString(fromDate);
    qStr += " and voucher_Date <= "+ db->getDateTimeString(toDate);
    qStr += " group by inventory_item_id)det  on ";
    qStr += " inv.Item_ID = det.Inventory_Item_ID    group by inv.Item_ID) A ";

    qDebug()<<qStr;
    model = DatabaseHelper::getQueryModel(qStr);

    return model;

}

QSqlQueryModel *InventoryHelper::getLedgerwiseItemwise(QString itemID, QDateTime fromDate, QDateTime toDate)
{
    // total qty in/out for item by ledger

    QSqlQueryModel *model = new QSqlQueryModel();
    TransactionHelper *t= new TransactionHelper();
    salesVoucherDatabaseHelper shelper;
    LedgerMasterDatabaseHelper led;

    QString qStr = "SELECT (SELECT LEDGER_NAME FROM " + led.Ledger_Master_TableName + " WHERE LEDGER_ID = TRANS.TO_LEDGER ) AS LEDGER, ";
    qStr += " COALESCE(SUM(CR_QTY),0) AS CREDIT, COALESCE(SUM(DR_QTY),0) AS DEBIT ";
    qStr += " FROM TRANSACTION_ITEM_HELPER TRANS ";
    qStr += " WHERE ITEMID = '" + itemID + "'";
    qStr += " AND TRANS.VOUCHERDATE >= '" + fromDate.toString("yyyy-MM-dd") + "' AND TRANS.VOUCHERDATE <= '" + toDate.toString("yyyy-MM-dd") + "'";
    qStr += " GROUP BY TO_LEDGER ";
    qStr += " UNION ";
    qStr += " SELECT (SELECT LEDGER_NAME FROM " + led.Ledger_Master_TableName + " WHERE LEDGER_ID = TRANS.FROM_LEDGER) AS LEDGER, ";
    qStr += " COALESCE(SUM(CR_QTY),0) AS CREDIT, COALESCE(SUM(DR_QTY),0) AS DEBIT ";
    qStr += " FROM TRANSACTION_ITEM_HELPER TRANS ";
    qStr += " WHERE ITEMID = '" + itemID + "'";
    qStr += " AND TRANS.VOUCHERDATE >= '" + fromDate.toString("yyyy-MM-dd") + "' AND TRANS.VOUCHERDATE <= '" + toDate.toString("yyyy-MM-dd") + "'";
    qStr += " GROUP BY FROM_LEDGER ";

    qDebug()<<qStr;

    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}

QSqlQueryModel *InventoryHelper::getLedgerwiseItemwiseDetailed(QString itemID, QDateTime fromDate, QDateTime toDate)
{
    // totals by voucher

    QSqlQueryModel *model = new QSqlQueryModel();
    TransactionHelper *t= new TransactionHelper();
    salesVoucherDatabaseHelper shelper;
    LedgerMasterDatabaseHelper led;

    QString qStr = " SELECT VOUCHERDATE, VOUCHERTYPE, VOUCHERID, ";
    qStr += " (SELECT LEDGER_NAME FROM " + led.Ledger_Master_TableName + " WHERE LEDGER_ID = TRANS.TO_LEDGER ) AS LEDGER, ";
    qStr += " COALESCE(SUM(CR_QTY),0) AS CREDIT, COALESCE(SUM(DR_QTY),0) AS DEBIT ";
    qStr += " FROM TRANSACTION_ITEM_HELPER TRANS ";
    qStr += " WHERE ITEMID = '" + itemID + "'";
    qStr += " AND TRANS.VOUCHERDATE >= '" + fromDate.toString("yyyy-MM-dd") + "' AND TRANS.VOUCHERDATE <= '" + toDate.toString("yyyy-MM-dd") + "'";
    qStr += " GROUP BY TO_LEDGER ";
    qStr += " UNION ";
    qStr += " SELECT VOUCHERDATE, VOUCHERTYPE, VOUCHERID, ";
    qStr += " (SELECT LEDGER_NAME FROM " + led.Ledger_Master_TableName + " WHERE LEDGER_ID = TRANS.FROM_LEDGER) AS LEDGER, ";
    qStr += " COALESCE(SUM(CR_QTY),0) AS CREDIT, COALESCE(SUM(DR_QTY),0) AS DEBIT ";
    qStr += " FROM TRANSACTION_ITEM_HELPER TRANS ";
    qStr += " WHERE ITEMID = '" + itemID + "'";
    qStr += " AND TRANS.VOUCHERDATE >= '" + fromDate.toString("yyyy-MM-dd") + "' AND TRANS.VOUCHERDATE <= '" + toDate.toString("yyyy-MM-dd") + "'";
    qStr += " GROUP BY FROM_LEDGER ";


    qDebug()<<qStr;

    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}

QSqlQueryModel *InventoryHelper::getAccGroupwiseItemMovement(QString invGroupID, QString accGroupId, QDateTime fromDate, QDateTime toDate)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    TransactionHelper *t= new TransactionHelper();
    salesVoucherDatabaseHelper shelper;
    LedgerMasterDatabaseHelper led;

    QString ledgerIdList ;
    if(accGroupId != "0")
            ledgerIdList = led.getAllLedgerIDsUnderGroupRecursiveAsString(accGroupId);

    QString qStr ;

    qStr =" select Ind,ID,itemGrp.group_name as Name,";
    qStr += " Sum(Cr) as Cr, Sum(CrAmt) 'Cr Amt', Sum(Dr) as Dr, Sum(DrAmt) 'Dr Amt' FROM (";
    qStr += " SELECT TRANS.ItemID,  Inv.Item_Name, COALESCE(CR_QTY,0) AS Cr, ";
    qStr += " case when COALESCE(CR_Amount,0) = 0 then (CR_QTY * INV.PRICE) else COALESCE(CR_Amount,0) end AS 'CrAmt',";
    qStr += " 0 AS Dr , 0 AS 'DrAmt' ";
    qStr += " FROM TRANSACTION_ITEM_HELPER TRANS JOIN SALES_INVENTORY_ITEMS INV ";
    qStr += " ON ITEMID = ITEM_ID WHERE (1=1)";
    if(accGroupId != "0")
        qStr += " AND (To_Ledger in (" + ledgerIdList + "))";
    qStr += " AND TRANS.VOUCHERDATE >= '" + fromDate.toString("yyyy-MM-dd")+ "'";
    qStr += " AND TRANS.VOUCHERDATE <= '" + toDate.toString("yyyy-MM-dd")+ "'";
    qStr += " UNION ALL";
    qStr += " SELECT TRANS.ItemID, Inv.Item_Name, 0 AS Cr,0 AS 'CrAmt',";
    qStr += " COALESCE((DR_QTY),0) AS Dr ,";
    qStr += " case when COALESCE(DR_Amount,0) = 0 then (DR_QTY * INV.PRICE) else COALESCE(DR_Amount,0) end AS 'DrAmt'";
    qStr += " FROM TRANSACTION_ITEM_HELPER TRANS JOIN SALES_INVENTORY_ITEMS INV ";
    qStr += " ON ITEMID = ITEM_ID WHERE (1=1)";
    if(accGroupId != "0")
        qStr += "  AND ( From_LEDGER in (" + ledgerIdList + "))";
    qStr += " AND TRANS.VOUCHERDATE >= '" + fromDate.toString("yyyy-MM-dd")+ "'";
    qStr += " AND TRANS.VOUCHERDATE <= '" + toDate.toString("yyyy-MM-dd")+ "'";
    qStr += " UNION ALL ";
    qStr += " SELECT TRANS.ItemID,  Inv.Item_Name, COALESCE(CR_QTY,0) AS Cr, ";
    qStr += " case when COALESCE(CR_Amount,0) = 0 then (CR_QTY * INV.PRICE) else COALESCE(CR_Amount,0) end AS 'CrAmt',";
    qStr += " 0 AS Dr , 0 AS 'DrAmt' ";
    qStr += " FROM TRANSACTION_BOM_HELPER TRANS JOIN SALES_INVENTORY_ITEMS INV ";
    qStr += " ON ITEMID = ITEM_ID  WHERE (1=1)";
    if(accGroupId != "0")
        qStr += "  AND (To_LEDGER in (" + ledgerIdList + "))";
    qStr += " AND TRANS.VOUCHERDATE >= '" + fromDate.toString("yyyy-MM-dd")+ "'";
    qStr += " AND TRANS.VOUCHERDATE <= '" + toDate.toString("yyyy-MM-dd")+ "'";
    qStr += " UNION ALL";
    qStr += " SELECT TRANS.ItemID, Inv.Item_Name, 0 AS Cr,0 AS 'CrAmt',";
    qStr += " COALESCE((DR_QTY),0) AS Dr ,";
    qStr += " case when COALESCE(DR_Amount,0) = 0 then (DR_QTY * INV.PRICE) else COALESCE(DR_Amount,0) end AS 'DrAmt'";
    qStr += " FROM TRANSACTION_BOM_HELPER TRANS JOIN SALES_INVENTORY_ITEMS INV ";
    qStr += " ON ITEMID = ITEM_ID WHERE (1=1)";
    if(accGroupId != "0")
        qStr += "  AND ( From_LEDGER in (" + ledgerIdList + "))";
    qStr += " AND TRANS.VOUCHERDATE >= '" + fromDate.toString("yyyy-MM-dd")+ "'";
    qStr += " AND TRANS.VOUCHERDATE <= '" + toDate.toString("yyyy-MM-dd")+ "'";
    qStr += " ) trans ";
    qStr += " inner join ";
    if(invGroupID !="0"){
        qStr += " (select ItemGrp1.group_name, Item.item_id, 'G' as Ind,ItemGrp1.group_id as ID from ";
        qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
        qStr += " inner join ";
        qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
        qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
        qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  grp";
        qStr += "  on item.group_id = grp.group_id";
        qStr += "  union all ";
        qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
        qStr += "  from      " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  p";
        qStr += "  inner join cte ";
        qStr += "  on p.group_id = cte.parent_id )";
        qStr += " select cte.group_name, cte.item_id, cte.group_id ";
        qStr += " from cte where cte.parent_id = '"+ invGroupID +"'";
        qStr += " ) itemGrp1 ";
        qStr += " on itemGrp1.item_id = item.item_id ";
        qStr += " union ";
        qStr += " select Item2.Item_name, Item2.Item_id, 'I' as ind,Item2.Item_id as ID  from ";
        qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
        qStr += " where Item2.group_id = '"+ invGroupID +"'";
        qStr += " ) itemGrp ";
    }
    else {
        qStr += "(SELECT (SELECT group_Name from " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName;
        qStr += " grp WHERE group_id = inv.group_id) as group_name, inv.item_id, 'G' as Ind, inv.group_id as ID from ";
        qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" inv) itemGrp";
    }
    qStr += " on trans.itemid = itemGrp.item_id ";
    qStr += " GROUP BY itemGrp.group_name";

    qDebug()<<qStr;

    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}


QSqlQueryModel *InventoryHelper::getDetailedAccGroupwiseItemMovement(QString invGroupID, QString accGroupId, QDateTime fromDate, QDateTime toDate)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    TransactionHelper *t= new TransactionHelper();
    salesVoucherDatabaseHelper shelper;
    LedgerMasterDatabaseHelper led;

    QString ledgerIdList ;
    if(accGroupId != "0")
            ledgerIdList = led.getAllLedgerIDsUnderGroupRecursiveAsString(accGroupId);

    QString qStr ;

    qStr =" select Ind,itemGrp.item_id ,";
    qStr += " (select item_name FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where item_id = itemGrp.item_id  ) as Item,";
    qStr += " Sum(Cr) as Cr, Sum(CrAmt) 'Cr Amt', Sum(Dr) as Dr, Sum(DrAmt) 'Dr Amt' FROM (";
    qStr += " SELECT TRANS.ItemID,  Inv.Item_Name, COALESCE(CR_QTY,0) AS Cr, ";
    qStr += " case when COALESCE(CR_Amount,0) = 0 then (CR_QTY * INV.PRICE) else COALESCE(CR_Amount,0) end AS 'CrAmt',";
    qStr += " 0 AS Dr , 0 AS 'DrAmt' ";
    qStr += " FROM TRANSACTION_ITEM_HELPER TRANS JOIN SALES_INVENTORY_ITEMS INV ";
    qStr += " ON ITEMID = ITEM_ID WHERE (1=1)";
    if(accGroupId != "0")
        qStr += " AND (To_Ledger in (" + ledgerIdList + "))";
    qStr += " AND TRANS.VOUCHERDATE >= '" + fromDate.toString("yyyy-MM-dd")+ "'";
    qStr += " AND TRANS.VOUCHERDATE <= '" + toDate.toString("yyyy-MM-dd")+ "'";
    qStr += " UNION ALL";
    qStr += " SELECT TRANS.ItemID, Inv.Item_Name, 0 AS Cr,0 AS 'CrAmt',";
    qStr += " COALESCE((DR_QTY),0) AS Dr ,";
    qStr += " case when COALESCE(DR_Amount,0) = 0 then (DR_QTY * INV.PRICE) else COALESCE(DR_Amount,0) end AS 'DrAmt'";
    qStr += " FROM TRANSACTION_ITEM_HELPER TRANS JOIN SALES_INVENTORY_ITEMS INV ";
    qStr += " ON ITEMID = ITEM_ID WHERE (1=1)";
    if(accGroupId != "0")
        qStr += "  AND ( From_LEDGER in (" + ledgerIdList + "))";
    qStr += " AND TRANS.VOUCHERDATE >= '" + fromDate.toString("yyyy-MM-dd")+ "'";
    qStr += " AND TRANS.VOUCHERDATE <= '" + toDate.toString("yyyy-MM-dd")+ "'";
    qStr += " UNION ALL ";
    qStr += " SELECT TRANS.ItemID,  Inv.Item_Name, COALESCE(CR_QTY,0) AS Cr, ";
    qStr += " case when COALESCE(CR_Amount,0) = 0 then (CR_QTY * INV.PRICE) else COALESCE(CR_Amount,0) end AS 'CrAmt',";
    qStr += " 0 AS Dr , 0 AS 'DrAmt' ";
    qStr += " FROM TRANSACTION_BOM_HELPER TRANS JOIN SALES_INVENTORY_ITEMS INV ";
    qStr += " ON ITEMID = ITEM_ID  WHERE (1=1)";
    if(accGroupId != "0")
        qStr += "  AND (To_LEDGER in (" + ledgerIdList + "))";
    qStr += " AND TRANS.VOUCHERDATE >= '" + fromDate.toString("yyyy-MM-dd")+ "'";
    qStr += " AND TRANS.VOUCHERDATE <= '" + toDate.toString("yyyy-MM-dd")+ "'";
    qStr += " UNION ALL";
    qStr += " SELECT TRANS.ItemID, Inv.Item_Name, 0 AS Cr,0 AS 'CrAmt',";
    qStr += " COALESCE((DR_QTY),0) AS Dr ,";
    qStr += " case when COALESCE(DR_Amount,0) = 0 then (DR_QTY * INV.PRICE) else COALESCE(DR_Amount,0) end AS 'DrAmt'";
    qStr += " FROM TRANSACTION_BOM_HELPER TRANS JOIN SALES_INVENTORY_ITEMS INV ";
    qStr += " ON ITEMID = ITEM_ID WHERE (1=1)";
    if(accGroupId != "0")
        qStr += "  AND ( From_LEDGER in (" + ledgerIdList + "))";
    qStr += " AND TRANS.VOUCHERDATE >= '" + fromDate.toString("yyyy-MM-dd")+ "'";
    qStr += " AND TRANS.VOUCHERDATE <= '" + toDate.toString("yyyy-MM-dd")+ "'";
    qStr += " ) trans ";
    qStr += " inner join ";
    if(invGroupID !="0"){
        qStr += " (select ItemGrp1.group_name, Item.item_id, 'G' as Ind,ItemGrp1.group_id as ID from ";
        qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
        qStr += " inner join ";
        qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
        qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
        qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  grp";
        qStr += "  on item.group_id = grp.group_id";
        qStr += "  union all ";
        qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
        qStr += "  from      " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  p";
        qStr += "  inner join cte ";
        qStr += "  on p.group_id = cte.parent_id )";
        qStr += " select cte.group_name, cte.item_id, cte.group_id ";
        qStr += " from cte where cte.parent_id = '"+ invGroupID +"'";
        qStr += " ) itemGrp1 ";
        qStr += " on itemGrp1.item_id = item.item_id ";
        qStr += " union ";
        qStr += " select Item2.Item_name, Item2.Item_id, 'I' as ind,Item2.Item_id as ID  from ";
        qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
        qStr += " where Item2.group_id = '"+ invGroupID +"'";
        qStr += " ) itemGrp ";
    }
    else {
        qStr += "(SELECT (SELECT group_Name from " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName;
        qStr += " grp WHERE group_id = inv.group_id) as group_name, inv.item_id, 'G' as Ind, inv.group_id as ID from ";
        qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" inv) itemGrp";
    }
    qStr += " on trans.itemid = itemGrp.item_id ";
    qStr += " GROUP BY itemGrp.item_id";

    qDebug()<<qStr;

    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}


QSqlQueryModel *InventoryHelper::getItemTransaction(QString itemID, QDateTime fromDate, QDateTime toDate)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    TransactionHelper *t= new TransactionHelper();
    salesVoucherDatabaseHelper shelper;

    QString qStr ;

    qStr += " SELECT date(VOUCHERDATE) as 'Voucher Date' , cast(VOUCHERID as INteger) as 'Voucher No' , VOUCHERTYPE as 'Voucher Type' , VOUCHERPREFIX as 'Voucher Prefix', ";
    qStr += " CAST(SUM(CR_QTY) AS DECIMAL(10,2)) as 'Outward' , CAST(SUM(DR_QTY) AS DECIMAL(10,2)) as 'Inward' ";
    qStr += " FROM TRANSACTION_ITEM_HELPER";
    qStr += " WHERE ITEMID = '" + itemID+ "'";
    qStr += " AND VOUCHERDATE >= '" + fromDate.toString("yyyy-MM-dd") + "' AND VOUCHERDATE <= '" + toDate.toString("yyyy-MM-dd") + "'";
    qStr += " GROUP BY  VOUCHERTYPE, VOUCHERID, VOUCHERPREFIX " ;
    qStr += " ORDER BY 1, VOUCHERTYPE, 2, VOUCHERPREFIX";


    qDebug()<<qStr;

    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}

QSqlQueryModel *InventoryHelper::getNegativeStockList(QDateTime toDate, int limit )
{
    QSqlQueryModel *model = new QSqlQueryModel();

    QString qStr ;

    qStr =  " SELECT INV.item_name as Item ,COALESCE(INV.Opening_Stock,0)-trans.difference";
    qStr +=  " as Stock FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" as INV LEFT OUTER JOIN (SELECT SUM(COALESCE(Cr_Qty,0)";
    qStr +=  " -COALESCE(Dr_Qty,0)) AS difference,itemID FROM Transaction_Item_Helper  ";
    qStr +=  " WHERE VoucherDate<= '"+toDate.toString("yyyy-MM-dd")+"' Group BY ItemID) as trans on INV.Item_Id=trans.ItemID ";
    qStr +=  " WHERE COALESCE(INV.Opening_Stock,0)-trans.difference<0 ";
    qStr += " AND Inv.isPurchaseItem = 1 order by 2 ";
    if(limit < 9999999)
        qStr += " LIMIT " + QString::number(limit);

    qDebug()<<qStr;
    model = DatabaseHelper::getQueryModel(qStr);

    return model;
}

QSqlQueryModel *InventoryHelper::getWastageQueryByDate(QDateTime dfrom, QDateTime dto)
{
    QSqlQueryModel* model = new QSqlQueryModel;
    ItemTransactionDatabaseHelper itemHelper;
    SalesInventoryItemDatabaseHelper shelper;
    TransactionHelper trans;

    QString qStr = "";
    qStr = " select (select item_name FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where item_id = inventory_item_id) as item,";
    qStr += " crQty, subTotal from wastage_entry_details";

    qStr += " where Voucher_no in (";
    qStr += "SELECT VOUCHER_NO FROM wastage_entry_main";
    qStr += " where (`timestamp` <= str_to_date('" +dto.toString("yyyy-MM-dd hh:mm:ss")+ "','%Y-%m-%d %H:%i:%s')";//'"+dto.toString("yyyy-MM-dd hh:mm:ss")+"'";
    qStr += " and `timestamp` >= str_to_date('" +dfrom.toString("yyyy-MM-dd hh:mm:ss")+ "','%Y-%m-%d %H:%i:%s')))";//'"+dfrom.toString("yyyy-MM-dd hh:mm:ss")+"'))";

    qDebug()<<"Wastage";

    qDebug()<<qStr;

    model = DatabaseHelper::getQueryModel(qStr);

    return model;

}

QSqlQueryModel *InventoryHelper::getComplimentarySalesForMail(QDateTime fromDate, QDateTime toDate)
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

    //Change Amt Calc;

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

    qStr += " <='"+toDate.toString("yyyy-MM-dd hh:mm:ss");
    qStr += "')";
    qStr += " and "+shelper.Sales_Invoice_Details_Price + "= 0 ";

    qStr += " GROUP BY ";
    qStr += shelper.Sales_Invoice_Details_Inventory_Item_ID;
    qStr += " ORDER BY ";
    qStr += "Item";

    if(query.exec(qStr)){
        qDebug()<<qStr;
    }

    qDebug()<<qStr;

    //    query.prepare(qStr);

    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}

QSqlQueryModel *InventoryHelper::getWastageQueryByDateVDATE(QDateTime dfrom, QDateTime dto, int limit)
{
    ItemTransactionDatabaseHelper itemHelper;
    SalesInventoryItemDatabaseHelper shelper;
    TransactionHelper trans;

    QString qStr = "";
    qStr = " select (select item_name FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +
            " where item_id = inventory_item_id) as item,";
    qStr += " crQty, subTotal from wastage_entry_details";

    qStr += " where Voucher_no in (";
    qStr += "SELECT VOUCHER_NO FROM wastage_entry_main";
    qStr += " where (`VOUCHER_DATE` <= str_to_date('" +dto.toString("yyyy-MM-dd")+ "','%Y-%m-%d')";
    qStr += " and `VOUCHER_DATE` >= str_to_date('" +dfrom.toString("yyyy-MM-dd")+ "','%Y-%m-%d')))";
    qStr += " ORDER BY 2 desc LIMIT " + QString::number(limit);


    return DatabaseHelper::getQueryModel(qStr);

}

QSqlQueryModel *InventoryHelper::getProductionRateModel(QDate fromDate, QDate toDate)
{
    QSqlQueryModel* model = new QSqlQueryModel;
    ItemTransactionDatabaseHelper itemHelper;
    SalesInventoryItemDatabaseHelper shelper;


    QString qStr = "SELECT ";
    qStr += " 'I', " + itemHelper.Transaction_Item_Helper_ItemID + ", ";
    qStr += " (SELECT " + shelper.Sales_Inventory_ItemName + " FROM " + shelper.Sales_Inventory_Table_Name ;
    qStr += " WHERE " + shelper.Sales_Inventory_ItemId + " = " + itemHelper.Transaction_Item_Helper_ItemID + ") as 'Item',";
    qStr += " SUM(" + itemHelper.Transaction_Item_Helper_Dr_Qty + ") as 'Qty' ,";
    qStr += " SUM(" + itemHelper.Transaction_Item_Helper_Dr_Qty + ") / ";
    qStr += " cast(DATEDIFF(" + db->getDateString(toDate) + "," + db->getDateString(fromDate) + " ) as Int) as 'Production Rate' ";
    qStr += " FROM " + itemHelper.Transaction_Item_Helper_TableName;
    qStr += " WHERE " + itemHelper.Transaction_Item_Helper_VoucherDate + " >='"+fromDate.toString("yyyy-MM-dd hh:mm:ss")+"'";
    qStr += " AND " + itemHelper.Transaction_Item_Helper_VoucherDate + " <='"+toDate.toString("yyyy-MM-dd hh:mm:ss")+"'";
    qStr += " AND " + itemHelper.Transaction_Item_Helper_VoucherType + " = '" + GMVoucherTypes::getVoucherString(GMVoucherTypes::WorkOrder) + "'";
    qStr += " GROUP BY " + itemHelper.Transaction_Item_Helper_ItemID + " ORDER BY Item";



    return DatabaseHelper::getQueryModel(qStr);
}

QSqlQueryModel *InventoryHelper::getConsumptionModel(QDate fromDate, QDate toDate, int limit)
{
    QSqlQueryModel* model = new QSqlQueryModel;
    ItemTransactionDatabaseHelper itemHelper;
    SalesInventoryItemDatabaseHelper shelper;


    QString qStr = "SELECT ";
    qStr += " 'I', " + itemHelper.Transaction_Item_Helper_ItemID + ", ";
    qStr += " (SELECT " + shelper.Sales_Inventory_ItemName + " FROM " + shelper.Sales_Inventory_Table_Name ;
    qStr += " WHERE " + shelper.Sales_Inventory_ItemId + " = " + itemHelper.Transaction_Item_Helper_ItemID + ") as 'Item',";
    qStr += " SUM(" + itemHelper.Transaction_Item_Helper_Cr_Qty + ") as 'Qty' ,";
    qStr += " SUM(" + itemHelper.Transaction_Item_Helper_Cr_Qty + ") / ";
    qStr += " cast(DATEDIFF(" + db->getDateString(toDate) + "," + db->getDateString(fromDate) + " ) as Int) as 'Consumption Rate' ";
    qStr += " FROM " + itemHelper.Transaction_Item_Helper_TableName;
    qStr += " WHERE " + itemHelper.Transaction_Item_Helper_VoucherDate + " >='"+fromDate.toString("yyyy-MM-dd hh:mm:ss")+"'";
    qStr += " AND " + itemHelper.Transaction_Item_Helper_VoucherDate + " <='"+toDate.toString("yyyy-MM-dd hh:mm:ss")+"'";
    qStr += " AND " + itemHelper.Transaction_Item_Helper_VoucherType + " = 'Stock Request'";
    qStr += " AND " + itemHelper.Transaction_Item_Helper_Dr_Qty + " =0";
    qStr += " GROUP BY " + itemHelper.Transaction_Item_Helper_ItemID + " ORDER BY Item";

    //    qDebug()<<qStr;

    return DatabaseHelper::getQueryModel(qStr);
}

QSqlQueryModel *InventoryHelper::getDatewiseTransactionByGroupByVoucherType(QString voucherType, QString groupID, QDateTime fromDate, QDateTime toDate,QString ledgerID, int x)
{

    salesVoucherDatabaseHelper shelper;

    QString timestamp = "str_to_Date(addTime(det.VoucherDate, det.Time) ,'%Y-%m-%d %H:%i:%s')" ;

    QString qStr = "SELECT Ind, ID,  groupName, ";
    QDate date2 = fromDate.date();
    int j=0;
    while( date2 <= toDate.date() ){
        qStr += "COALESCE(SUM(date"+QString::number(j)+"),0.00) as '" + date2.toString("yyyy-MM-dd") +"'";
        if (date2.daysTo(toDate.date()) >=x ){
            qStr += ", ";
        }
        date2 = date2.addDays(x);
        j+=x;
    }
    qStr += " FROM (SELECT Ind,ID, groupName, ";
    QDate date1 = fromDate.date();
    int i=0;
    while( date1 <= toDate.date() ){
        qStr += "CASE WHEN voucher_Date >= '" + date1.toString("yyyy-MM-dd") + "' AND voucher_Date < '" + date1.addDays(x).toString("yyyy-MM-dd") + "'";
        qStr += " THEN format(SUM(Amount),2) END ";
        qStr += "as date" + QString::number(i) ;
        if (date1.daysTo(toDate.date()) >=x ){
            qStr += ", ";
        }
        date1 = date1.addDays(x);
        i+=x;
    }
    qStr += " FROM ( ";

    qStr += "  select Ind, ID, itemGrp.group_name as groupName,";
    qStr += "  ABS(CR_AMOUNT - DR_AMOUNT) as Amount, voucherDate voucher_Date ";
    qStr += " from transaction_item_helper det";
    qStr += " inner join ";
    qStr += " (select ItemGrp1.group_name, Item.item_id, 'G' as Ind, ItemGrp1.group_id as ID from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
    qStr += " inner join ";
    qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
    qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
    qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  grp";
    qStr += "  on item.group_id = grp.group_id";
    qStr += "  union all ";
    qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
    qStr += "  from      " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  p";
    qStr += "  inner join cte ";
    qStr += "  on p.group_id = cte.parent_id )";
    qStr += " select cte.group_id, cte.group_name, cte.item_id";
    qStr += " from cte where cte.parent_id = '"+ groupID +"'";
    qStr += " ) itemGrp1 ";
    qStr += " on itemGrp1.item_id = item.item_id ";
    qStr += " union ";
    qStr += " select (select group_name from " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  where group_id = Item2.group_id), Item2.Item_id, 'I' as ind, Item2.Group_Id as ID from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
    qStr += " where Item2.group_id = '"+ groupID +"'";
    qStr += " ) itemGrp ";
    qStr += " on det.itemId = itemGrp.item_id ";
    qStr += " where str_to_Date(addTime(det.VoucherDate, det.Time) ,'%Y-%m-%d %H:%i:%s') >= " + db->getDateTimeString(fromDate) ;
    qStr += " and str_to_Date(addTime(det.VoucherDate, det.Time) ,'%Y-%m-%d %H:%i:%s') <= " + db->getDateTimeString(toDate) ;
    qStr += " and det.voucherType = '" + voucherType + "'";
    if(ledgerID != "%")
        qStr += " and det.to_ledger = '" + ledgerID + "'";
    qStr += " )  as Sales ";
    qStr += " group by groupName , (Voucher_Date)) as final";
    qStr += " GROUP BY groupName";
    qStr += " ORDER BY 3";



    return DatabaseHelper::getQueryModel(qStr);

}

QSqlQueryModel *InventoryHelper::getDetailedTransactionByVoucherTypeAndReason(QString voucherType, QString groupID, QDateTime fromDate, QDateTime toDate, QString ledgerID)
{

    QString     timestamp = "str_to_Date(addTime(det.VoucherDate, det.Time) ,'%Y-%m-%d %H:%i:%s')" ;
    QString qStr = "";

    qStr += "  select 'I' Ind, ItemID ID,";
    qStr += " (select item_name FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where item_id = ItemID ) as Item,";
    qStr += "  max(Reason1Qty) as 'Used in kitchen \n Qty', max(Reason1Amount) as 'Used in kitchen \n Value',";
    qStr += " max(Reason2Qty) as 'Expiry \n Qty', max(Reason2Amount) as 'Expiry \n Value',";
    qStr += " max(Reason3Qty) as 'Near expiry \n Qty', max(Reason3Amount) as 'Near expiry \n Value',";
    qStr += " max(Reason4Qty) as 'Rejected \n Qty', max(Reason4Amount) as 'Rejected \n Value',";
    qStr += " max(Reason5Qty) as 'Too much stock \n Qty', max(Reason5Amount) as 'Too much stock \n Value',";
    qStr += " max(Reason6Qty) as 'Packing damage \n Qty', max(Reason6Amount) as 'Packing damage \n Value',";
    qStr += " max(Reason7Qty) as 'Production Damage \n Qty', max(Reason7Amount) as 'Production Damage \n Value',";
    qStr += " max(Reason8Qty) as 'Others \n Qty', max(Reason8Amount) as 'Others \n Value'";
    qStr += " from (";
    qStr += " select Ind,ID,det.ItemID,  ";
    qStr += " (case when main.Quotation_Dropped_Reason = 'Used in kitchen' then  ABS(SUM(CR_QTY - DR_QTY)) else 0 end ) as 'Reason1Qty',";
    qStr += " (case when main.Quotation_Dropped_Reason = 'Used in kitchen' then  ABS(SUM(CR_AMOUNT - DR_AMOUNT)) else 0 end ) as 'Reason1Amount',";
    qStr += " (case when main.Quotation_Dropped_Reason = 'Expiry' then  ABS(SUM(CR_QTY - DR_QTY)) else 0 end ) as 'Reason2Qty' , ";
    qStr += " (case when main.Quotation_Dropped_Reason = 'Expiry' then  ABS(SUM(CR_AMOUNT - DR_AMOUNT)) else 0 end ) as 'Reason2Amount' ,";
    qStr += " (case when main.Quotation_Dropped_Reason = 'Near expiry' then  ABS(SUM(CR_QTY - DR_QTY)) else 0 end ) as 'Reason3Qty' ,";
    qStr += " (case when main.Quotation_Dropped_Reason = 'Near expiry' then  ABS(SUM(CR_AMOUNT - DR_AMOUNT)) else 0 end ) as 'Reason3Amount' ,";
    qStr += " (case when main.Quotation_Dropped_Reason = 'Rejected as not per order' then  ABS(SUM(CR_QTY - DR_QTY)) else 0 end ) as 'Reason4Qty' ,";
    qStr += " (case when main.Quotation_Dropped_Reason = 'Rejected as not per order' then  ABS(SUM(CR_AMOUNT - DR_AMOUNT)) else 0 end ) as 'Reason4Amount', ";
    qStr += " (case when main.Quotation_Dropped_Reason = 'Too much stock' then  ABS(SUM(CR_QTY - DR_QTY)) else 0 end ) as 'Reason5Qty' ,";
    qStr += " (case when main.Quotation_Dropped_Reason = 'Too much stock' then  ABS(SUM(CR_AMOUNT - DR_AMOUNT)) else 0 end ) as 'Reason5Amount', ";
    qStr += " (case when main.Quotation_Dropped_Reason = 'Packing damage' then  ABS(SUM(CR_QTY - DR_QTY)) else 0 end ) as 'Reason6Qty' ,";
    qStr += " (case when main.Quotation_Dropped_Reason = 'Packing damage' then  ABS(SUM(CR_AMOUNT - DR_AMOUNT)) else 0 end ) as 'Reason6Amount', ";
    qStr += " (case when main.Quotation_Dropped_Reason = 'Production Damage' then  ABS(SUM(CR_QTY - DR_QTY)) else 0 end ) as 'Reason7Qty' ,";
    qStr += " (case when main.Quotation_Dropped_Reason = 'Production Damage' then  ABS(SUM(CR_AMOUNT - DR_AMOUNT)) else 0 end ) as 'Reason7Amount', ";
    qStr += " (case when main.Quotation_Dropped_Reason = 'Others' then ABS(SUM(CR_QTY - DR_QTY)) else 0 end ) as 'Reason8Qty' ,";
    qStr += " (case when main.Quotation_Dropped_Reason = 'Others' then  ABS(SUM(CR_AMOUNT - DR_AMOUNT)) else 0 end ) as 'Reason8Amount' ";
    qStr += " from transaction_item_helper det join ";
    if(voucherType == "DebitNote")
        qStr += " Debit_Note_Main ";
    else if(voucherType == "CreditNote")
        qStr += " Credit_Note_Main ";
    else if(voucherType == "WastageEntry")
        qStr += " Wastage_Entry_Main ";
    qStr += " main on det.voucherID = main.voucher_No and det.voucherPrefix = main.voucher_prefix ";
    qStr += " inner join ";
    qStr += " (select ItemGrp1.group_name, Item.item_id, 'G' as Ind,ItemGrp1.group_id as ID from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
    qStr += " inner join ";
    qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
    qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
    qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  grp";
    qStr += "  on item.group_id = grp.group_id";
    qStr += "  union all ";
    qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
    qStr += "  from      " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  p";
    qStr += "  inner join cte ";
    qStr += "  on p.group_id = cte.parent_id )";
    qStr += " select cte.group_name, cte.item_id, cte.group_id ";
    qStr += " from cte where cte.parent_id = '"+ groupID +"'";
    qStr += " ) itemGrp1 ";
    qStr += " on itemGrp1.item_id = item.item_id ";
    qStr += " union ";
    qStr += " select Item2.Item_name, Item2.Item_id, 'I' as ind,Item2.Item_id as ID  from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
    qStr += " where Item2.group_id = '"+ groupID +"'";
    qStr += " ) itemGrp ";
    qStr += " on det.itemID = itemGrp.item_id ";
    qStr += " and " + timestamp + ">=" + db->getDateTimeString(fromDate);
    qStr += " and " + timestamp + "<= " + db->getDateTimeString(toDate);
    if(ledgerID != "%")
        qStr += " and det.to_ledger like '" + ledgerID + "'";

    qStr += " AND det.voucherType = '" + voucherType + "'";
    qStr += " group by det.itemID, main.Quotation_Dropped_Reason ";
    qStr += " ) X group by ItemID ";
    qStr += " ORDER BY 3";


    qDebug()<<qStr;

    //    query.prepare(qStr);

    return DatabaseHelper::getQueryModel(qStr);

}

QSqlQueryModel *InventoryHelper::getGroupwiseTransactionByVoucherTypeAndReason(QString voucherType, QString groupID, QDateTime fromDate, QDateTime toDate, QString ledgerID)
{
    QString     timestamp = "str_to_Date(addTime(det.VoucherDate, det.Time) ,'%Y-%m-%d %H:%i:%s')" ;
    QString qStr = "";

    qStr += "  select Ind, ID, groupName,";
    qStr += "  max(Reason1Qty) as 'Used in kitchen \n Qty', max(Reason1Amount) as 'Used in kitchen \n Value',";
    qStr += " max(Reason2Qty) as 'Expiry \n Qty', max(Reason2Amount) as 'Expiry \n Value',";
    qStr += " max(Reason3Qty) as 'Near expiry \n Qty', max(Reason3Amount) as 'Near expiry \n Value',";
    qStr += " max(Reason4Qty) as 'Rejected \n Qty', max(Reason4Amount) as 'Rejected \n Value',";
    qStr += " max(Reason5Qty) as 'Too much stock \n Qty', max(Reason5Amount) as 'Too much stock \n Value',";
    qStr += " max(Reason6Qty) as 'Packing damage \n Qty', max(Reason6Amount) as 'Packing damage \n Value',";
    qStr += " max(Reason7Qty) as 'Production Damage \n Qty', max(Reason7Amount) as 'Production Damage \n Value',";
    qStr += " max(Reason8Qty) as 'Others \n Qty', max(Reason8Amount) as 'Others \n Value'";
    qStr += " from (";
    qStr += " select Ind,ID,itemGrp.groupName,  ";
    qStr += " (case when main.Quotation_Dropped_Reason = 'Used in kitchen' then  ABS(SUM(CR_QTY - DR_QTY)) else 0 end ) as 'Reason1Qty',";
    qStr += " (case when main.Quotation_Dropped_Reason = 'Used in kitchen' then  ABS(SUM(CR_AMOUNT - DR_AMOUNT)) else 0 end ) as 'Reason1Amount',";
    qStr += " (case when main.Quotation_Dropped_Reason = 'Expiry' then  ABS(SUM(CR_QTY - DR_QTY)) else 0 end ) as 'Reason2Qty' , ";
    qStr += " (case when main.Quotation_Dropped_Reason = 'Expiry' then  ABS(SUM(CR_AMOUNT - DR_AMOUNT)) else 0 end ) as 'Reason2Amount' ,";
    qStr += " (case when main.Quotation_Dropped_Reason = 'Near expiry' then  ABS(SUM(CR_QTY - DR_QTY)) else 0 end ) as 'Reason3Qty' ,";
    qStr += " (case when main.Quotation_Dropped_Reason = 'Near expiry' then  ABS(SUM(CR_AMOUNT - DR_AMOUNT)) else 0 end ) as 'Reason3Amount' ,";
    qStr += " (case when main.Quotation_Dropped_Reason = 'Rejected as not per order' then  ABS(SUM(CR_QTY - DR_QTY)) else 0 end ) as 'Reason4Qty' ,";
    qStr += " (case when main.Quotation_Dropped_Reason = 'Rejected as not per order' then  ABS(SUM(CR_AMOUNT - DR_AMOUNT)) else 0 end ) as 'Reason4Amount', ";
    qStr += " (case when main.Quotation_Dropped_Reason = 'Too much stock' then  ABS(SUM(CR_QTY - DR_QTY)) else 0 end ) as 'Reason5Qty' ,";
    qStr += " (case when main.Quotation_Dropped_Reason = 'Too much stock' then  ABS(SUM(CR_AMOUNT - DR_AMOUNT)) else 0 end ) as 'Reason5Amount', ";
    qStr += " (case when main.Quotation_Dropped_Reason = 'Packing damage' then  ABS(SUM(CR_QTY - DR_QTY)) else 0 end ) as 'Reason6Qty' ,";
    qStr += " (case when main.Quotation_Dropped_Reason = 'Packing damage' then  ABS(SUM(CR_AMOUNT - DR_AMOUNT)) else 0 end ) as 'Reason6Amount', ";
    qStr += " (case when main.Quotation_Dropped_Reason = 'Production Damage' then  ABS(SUM(CR_QTY - DR_QTY)) else 0 end ) as 'Reason7Qty' ,";
    qStr += " (case when main.Quotation_Dropped_Reason = 'Production Damage' then  ABS(SUM(CR_AMOUNT - DR_AMOUNT)) else 0 end ) as 'Reason7Amount', ";
    qStr += " (case when main.Quotation_Dropped_Reason = 'Others' then ABS(SUM(CR_QTY - DR_QTY)) else 0 end ) as 'Reason8Qty' ,";
    qStr += " (case when main.Quotation_Dropped_Reason = 'Others' then  ABS(SUM(CR_AMOUNT - DR_AMOUNT)) else 0 end ) as 'Reason8Amount' ";
    qStr += " from transaction_item_helper det join ";
    if(voucherType == "DebitNote")
        qStr += " Debit_Note_Main ";
    else if(voucherType == "CreditNote")
        qStr += " Credit_Note_Main ";
    else if(voucherType == "WastageEntry")
        qStr += " Wastage_Entry_Main ";
    qStr += " main on det.voucherID = main.voucher_No and det.voucherPrefix = main.voucher_prefix ";
    qStr += " inner join ";
    qStr += " (select ItemGrp1.group_name as groupName, Item.item_id, 'G' as Ind,ItemGrp1.group_id as ID from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
    qStr += " inner join ";
    qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
    qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
    qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  grp";
    qStr += "  on item.group_id = grp.group_id";
    qStr += "  union all ";
    qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
    qStr += "  from      " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  p";
    qStr += "  inner join cte ";
    qStr += "  on p.group_id = cte.parent_id )";
    qStr += " select cte.group_name, cte.item_id, cte.group_id ";
    qStr += " from cte where cte.parent_id = '"+ groupID +"'";
    qStr += " ) itemGrp1 ";
    qStr += " on itemGrp1.item_id = item.item_id ";
    qStr += " union ";
    qStr += " select Item2.Item_name, Item2.Item_id, 'I' as ind,Item2.Item_id as ID  from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
    qStr += " where Item2.group_id = '"+ groupID +"'";
    qStr += " ) itemGrp ";
    qStr += " on det.ItemID = itemGrp.item_id ";

    qStr += " where ";
    qStr += " ( ";
    qStr += timestamp + ">=" + db->getDateTimeString(fromDate);
    qStr += " and ";
    qStr += timestamp + "<= " + db->getDateTimeString(toDate);
    qStr += ")";
    if(ledgerID != "%")
        qStr += " and det.to_ledger like '" + ledgerID + "'";

    qStr += " AND det.voucherType = '" + voucherType + "'";
    qStr += " group by itemGrp.groupName, main.Quotation_Dropped_Reason ";
    qStr += " ) X group by groupName ";
    qStr += " ORDER BY 3";



    //    query.prepare(qStr);
    return DatabaseHelper::getQueryModel(qStr);

}

QSqlQueryModel *InventoryHelper::getDatewiseDetailedTransactionByVoucherType(QString voucherType, QString groupID, QDateTime fromDate, QDateTime toDate, QString ledgerID, int x)
{
    QString     timestamp = "str_to_Date(addTime(det.VoucherDate, det.Time) ,'%Y-%m-%d %H:%i:%s')" ;
    QString qStr = "SELECT Ind, ID,  Item, ";
    QDate date2 = fromDate.date();
    int j=0;
    while( date2 <= toDate.date() ){
        qStr += "COALESCE(SUM(date"+QString::number(j)+"),0.00) as '" + date2.toString("yyyy-MM-dd") +"'";
        if (date2.daysTo(toDate.date()) >=x ){
            qStr += ", ";
        }
        date2 = date2.addDays(x);
        j+=x;
    }
    qStr += " FROM (SELECT Ind,ID, Item, ";
    QDate date1 = fromDate.date();
    int i=0;
    while( date1 <= toDate.date() ){
        qStr += "CASE WHEN voucher_Date >= '" + date1.toString("yyyy-MM-dd") + "' AND voucher_Date < '" + date1.addDays(x).toString("yyyy-MM-dd") + "'";
        qStr += " THEN format(SUM(Amount),2) END ";
        qStr += "as date" + QString::number(i) ;
        if (date1.daysTo(toDate.date()) >=x ){
            qStr += ", ";
        }
        date1 = date1.addDays(x);
        i+=x;
    }
    qStr += " FROM ( ";

    qStr += "  select 'I' Ind, det.itemID ID,";
    qStr += " (select item_name FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where item_id = det.itemID ) as Item,";
    qStr += "  ABS(SUM(CR_AMOUNT - DR_AMOUNT)) as Amount, voucherDate as voucher_Date ";
    qStr += " from transaction_item_helper det";
    qStr += " inner join ";
    qStr += " (select ItemGrp1.group_name, Item.item_id, 'G' as Ind, ItemGrp1.group_id as ID from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
    qStr += " inner join ";
    qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
    qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
    qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  grp";
    qStr += "  on item.group_id = grp.group_id";
    qStr += "  union all ";
    qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
    qStr += "  from      " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  p";
    qStr += "  inner join cte ";
    qStr += "  on p.group_id = cte.parent_id )";
    qStr += " select cte.group_id, cte.group_name, cte.item_id";
    qStr += " from cte where cte.parent_id = '"+ groupID +"'";
    qStr += " ) itemGrp1 ";
    qStr += " on itemGrp1.item_id = item.item_id ";
    qStr += " union ";
    qStr += " select (select group_name from " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  where group_id = Item2.group_id), Item2.Item_id, 'I' as ind, Item2.Group_Id as ID from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
    qStr += " where Item2.group_id = '"+ groupID +"'";
    qStr += " ) itemGrp ";
    qStr += " on det.itemID = itemGrp.item_id ";
    qStr += " where ( ";
    qStr += timestamp + ">=" + db->getDateTimeString(fromDate);
    qStr += " and ";
    qStr += timestamp + "<= " + db->getDateTimeString(toDate);
    qStr += ")";
    if(ledgerID != "%")
        qStr += " and det.to_ledger like '" + ledgerID + "'";

    qStr += " AND det.voucherType = '" + voucherType + "'";
    qStr += " )  as Trans ";
    qStr += " group by Item , Voucher_Date) as final";
    qStr += " GROUP BY Item";
    qStr += " ORDER BY 1";



    return DatabaseHelper::getQueryModel(qStr);

}

QSqlQueryModel *InventoryHelper::getItemwiseTimeBasedSales(QString itemID, QDateTime fromDate, QDateTime toDate)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    salesVoucherDatabaseHelper shelper;

    QString    timestamp = "str_to_Date(concat(main." + shelper.Sales_Invoice_Main_Voucher_Date ;
    timestamp += ", substr(" + shelper.Sales_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;


    QString qStr="SELECT ";
    qStr  += "CONCAT('Hour : ',HOUR("+shelper.Sales_Invoice_Main_TimeStamp+")) as Hour, ";
    qStr  += "FORMAT(SUM("+shelper.Sales_Invoice_Details_Quantity+"),2) as Quantity ";
    qStr += " FROM ";
    qStr += shelper.Sales_Invoice_Main_TableName + " main ";
    qStr += + " left outer join " + shelper.Sales_Invoice_Details_TableName + " det ";
    qStr += " on main." + shelper.Sales_Invoice_Main_Voucher_No + " = det." + shelper.Sales_Invoice_Details_Voucher_No;
    qStr += " and main." + shelper.Sales_Invoice_Main_Voucher_Prefix + " = det." + shelper.Sales_Invoice_Details_Voucher_Prefix;
    qStr += " Where ";
    qStr += " det." + shelper.Sales_Invoice_Details_Inventory_Item_ID + " = '" + itemID + "'";
    qStr += " and ";
    qStr += timestamp + " >=str_to_date('" + fromDate.toString("yyyy-MM-dd hh:mm:ss") + "','%Y-%m-%d %H:%i:%s')";
    qStr += " and ";
    qStr += timestamp + " <=str_to_date('" + toDate.toString("yyyy-MM-dd hh:mm:ss") + "','%Y-%m-%d %H:%i:%s')";

    qStr += " GROUP BY HOUR("+shelper.Sales_Invoice_Main_TimeStamp + ")";
    qStr += " ORDER BY HOUR("+shelper.Sales_Invoice_Main_TimeStamp + ")";

    model->setQuery(qStr,QSqlDatabase::database(DatabaseValues::connectionString));

    return model;
}

QSqlQueryModel *InventoryHelper::getItemCrDrBetDate(QString ItemID,
                                                    QDate fromDate,
                                                    QDate toDate,
                                                    QString voucherType,
                                                    QString godown)
{


    qDebug()<<Q_FUNC_INFO;

    QString qStr;

    qStr = " SELECT VOUCHERPREFIX, VoucherNo as 'Voucher No', VoucherType  as 'Voucher Type', sum(Inward) as Inward, Sum(Outward) as Outward FROM ( ";
    qStr +=  " SELECT VOUCHERPREFIX,date(VOUCHERDATE) as 'Voucher Date' , cast(VOUCHERID as INteger) as 'VoucherNo' , VOUCHERTYPE as 'VoucherType',"; //  VOUCHERPREFIX as 'Voucher Prefix', ";
    qStr += " 0 as 'Inward' , CAST(SUM(CR_QTY) AS DECIMAL(10,3)) as 'Outward' ";
    qStr += " FROM TRANSACTION_ITEM_HELPER";
    qStr += " WHERE ITEMID = '" + ItemID+ "'";
    qStr += " AND FROM_LOCATION like '" + godown + "'";
    qStr += " AND VOUCHERTYPE like '" + voucherType + "'";
    qStr += " AND VOUCHERDATE >= str_to_date('" + fromDate.toString("yyyy-MM-dd") + "', '%Y-%m-%d')";
    qStr += " AND VOUCHERDATE <= str_to_date('" + toDate.toString("yyyy-MM-dd") + "', '%Y-%m-%d')";
    qStr += " GROUP BY  VOUCHERTYPE, VOUCHERID, VOUCHERPREFIX " ;
    qStr += " UNION ALL";
    qStr +=  " SELECT VOUCHERPREFIX,date(VOUCHERDATE) as 'Voucher Date' , cast(VOUCHERID as INteger) as 'Voucher No' , VOUCHERTYPE as 'Voucher Type',"; //  VOUCHERPREFIX as 'Voucher Prefix', ";
    qStr += " CAST(SUM(DR_QTY) AS DECIMAL(10,3)) as 'Inward' , 0 as 'Outward' ";
    qStr += " FROM TRANSACTION_ITEM_HELPER";
    qStr += " WHERE ITEMID = '" + ItemID+ "'";
    qStr += " AND TO_LOCATION like '" + godown + "'";
    qStr += " AND VOUCHERTYPE like '" + voucherType + "'";
    qStr += " AND VOUCHERDATE >= str_to_date('" + fromDate.toString("yyyy-MM-dd") + "', '%Y-%m-%d')";
    qStr += " AND VOUCHERDATE <= str_to_date('" + toDate.toString("yyyy-MM-dd") + "', '%Y-%m-%d')";
    qStr += " GROUP BY  VOUCHERTYPE, VOUCHERID, VOUCHERPREFIX " ;
    qStr += " UNION ALL";
    qStr +=  " SELECT VOUCHERPREFIX,date(VOUCHERDATE) as 'Voucher Date' , cast(VOUCHERID as INteger) as 'VoucherNo' , VOUCHERTYPE as 'VoucherType',"; //  VOUCHERPREFIX as 'Voucher Prefix', ";
    qStr += " 0 as 'Inward' , CAST(SUM(CR_QTY) AS DECIMAL(10,3)) as 'Outward' ";
    qStr += " FROM TRANSACTION_BOM_HELPER";
    qStr += " WHERE ITEMID = '" + ItemID+ "'";
    qStr += " AND FROM_LOCATION like '" + godown + "'";
    qStr += " AND VOUCHERTYPE like '" + voucherType + "'";
    qStr += " AND VOUCHERDATE >= str_to_date('" + fromDate.toString("yyyy-MM-dd") + "', '%Y-%m-%d')";
    qStr += " AND VOUCHERDATE <= str_to_date('" + toDate.toString("yyyy-MM-dd") + "', '%Y-%m-%d')";
    qStr += " GROUP BY  VOUCHERTYPE, VOUCHERID, VOUCHERPREFIX " ;
    qStr += " UNION ALL";
    qStr +=  " SELECT VOUCHERPREFIX,date(VOUCHERDATE) as 'Voucher Date' , cast(VOUCHERID as INteger) as 'VoucherNo' , VOUCHERTYPE as 'VoucherType',"; //  VOUCHERPREFIX as 'Voucher Prefix', ";
    qStr += " CAST(SUM(DR_QTY) AS DECIMAL(10,3)) as 'Inward' , 0 as 'Outward' ";
    qStr += " FROM TRANSACTION_BOM_HELPER";
    qStr += " WHERE ITEMID = '" + ItemID+ "'";
    qStr += " AND TO_LOCATION like '" + godown + "'";
    qStr += " AND VOUCHERTYPE like '" + voucherType + "'";
    qStr += " AND VOUCHERDATE >= str_to_date('" + fromDate.toString("yyyy-MM-dd") + "', '%Y-%m-%d')";
    qStr += " AND VOUCHERDATE <= str_to_date('" + toDate.toString("yyyy-MM-dd") + "', '%Y-%m-%d')";
    qStr += " GROUP BY  VOUCHERTYPE, VOUCHERID, VOUCHERPREFIX " ;
    qStr += " ) A Group by VoucherType, VoucherNo, VOUCHERPREFIX ";
    qStr += " ORDER BY 1,2";

    qDebug()<<qStr;


//    QSqlQueryModel* model = new QSqlQueryModel;
//    model->setQuery(qStr,QSqlDatabase::database(DatabaseValues::connectionString));
    return DatabaseHelper::getQueryModel(qStr);


}

QSqlQueryModel *InventoryHelper::getTopCustomerByItem(QString itemID, QDate fromDate, QDate toDate, int limit)
{
    qDebug()<<Q_FUNC_INFO;

    LedgerMasterDatabaseHelper led;
    QString qStr = "SELECT  ";
    qStr += " (select ledger_name FROM " + led.Ledger_Master_TableName + " where ledger_id = ";
    qStr += " (select Ledger_ID from sales_invoice_main ";
    qStr += " WHERE  voucher_no = det.Voucher_No and voucher_prefix = det.Voucher_Prefix)) as Vendor, ";
    qStr += " round(COALESCE(SUM(SubTotal),0),2) as Value ";
    qStr += " FROM sales_invoice_details det ";
    qStr += " WHERE Inventory_Item_ID = '" + itemID +"'";
    qStr += " AND Voucher_Date >= " + db->getDateString(fromDate);
    qStr += " AND Voucher_Date <= " + db->getDateString(toDate);
    qStr += " GROUP BY (select Ledger_ID from sales_invoice_main ";
    qStr += " WHERE  voucher_no = det.Voucher_No and voucher_prefix = det.Voucher_Prefix) ";
    qStr += " ORDER BY SUM(SubTotal) desc ";
    qStr += " LIMIT " + QString::number(limit) ;

    return DatabaseHelper::getQueryModel(qStr);
}

QSqlQueryModel *InventoryHelper::getTopVendorByItem(QString itemID, QDate fromDate, QDate toDate, int limit)
{
    LedgerMasterDatabaseHelper led;
    QString qStr = "SELECT  ";
    qStr += " (select ledger_name FROM " + led.Ledger_Master_TableName + " where ledger_id = ";
    qStr += " (select Ledger_ID from purchase_invoice_main ";
    qStr += " WHERE  voucher_no = det.Voucher_No and voucher_prefix = det.Voucher_Prefix)) as Vendor, ";
    qStr += " round(COALESCE(SUM(SubTotal),0),2) as Value ";
    qStr += " FROM purchase_invoice_details det ";
    qStr += " WHERE Inventory_Item_ID = '" + itemID +"'";
    qStr += " AND Voucher_Date >= " + db->getDateString(fromDate);
    qStr += " AND Voucher_Date <= " + db->getDateString(toDate);
    qStr += " GROUP BY (select Ledger_ID from purchase_invoice_main ";
    qStr += " WHERE  voucher_no = det.Voucher_No and voucher_prefix = det.Voucher_Prefix) ";
    qStr += " ORDER BY SUM(SubTotal) desc ";
    qStr += " LIMIT " + QString::number(limit) ;

    return DatabaseHelper::getQueryModel(qStr);
}

QSqlQueryModel *InventoryHelper::getLastTransactionsByItem(QString itemID, int voucherType, QDate fromDate, QDate toDate, int limit)
{
    LedgerMasterDatabaseHelper led;
    QString tableName = GMVoucherTypes::getVoucherMainTablename(voucherType);
    QString detTable = GMVoucherTypes::getVoucherDetailsTablename(voucherType);

    QString qStr = "SELECT  ";
    qStr += " (select ledger_name FROM " + led.Ledger_Master_TableName + " where ledger_id = ";
    qStr += " (select Ledger_ID from "  + tableName ;
    qStr += " WHERE  voucher_no = det.Voucher_No and voucher_prefix = det.Voucher_Prefix)) as Ledger, ";
    qStr += " round(COALESCE(Price,0),2) as Rate ";
    qStr += " FROM " + detTable + " det ";
    qStr += " WHERE Inventory_Item_ID = '" + itemID +"'";
    qStr += " AND Voucher_Date >= " + db->getDateString(fromDate);
    qStr += " AND Voucher_Date <= " + db->getDateString(toDate);
    qStr += " ORDER BY Voucher_Date desc, Voucher_No desc ";
    qStr += " LIMIT " + QString::number(limit) ;

    return DatabaseHelper::getQueryModel(qStr);
}

QSqlQueryModel *InventoryHelper::getMostSoldItems(QDate fromDate, QDate toDate, int limit)
{
    QString qStr = " SELECT (SELECT Item_Name FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" ";
    qStr += " where item_id = Inventory_Item_Id ) as Item, ";
    qStr += " SUM(quantity) as 'Total Sold', SUM(subTotal) as 'Sale Value' ";
    qStr += " FROM Sales_Invoice_Details ";
    qStr += " WHERE Voucher_Date >= " + db->getDateString(fromDate);
    qStr += " AND Voucher_Date <= " + db->getDateString(toDate);
    qStr += " GROUP BY Inventory_Item_Id ";
    qStr += " ORDER BY SUM(Quantity) desc ";
    qStr += " LIMIT " + QString::number(limit);

    return DatabaseHelper::getQueryModel(qStr);
}

QSqlQueryModel *InventoryHelper::getMostBoughtItems(QDate fromDate, QDate toDate, int limit)
{
    QString qStr = " SELECT (SELECT Item_Name FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" ";
    qStr += " where item_id = Inventory_Item_Id ) as Item, ";
    qStr += " SUM(quantity) as 'Total Sold', SUM(subTotal) as 'Purchase Value' ";
    qStr += " FROM Purchase_Invoice_Details ";
    qStr += " WHERE Voucher_Date >= " + db->getDateString(fromDate);
    qStr += " AND Voucher_Date <= " + db->getDateString(toDate);
    qStr += " GROUP BY Inventory_Item_Id ";
    qStr += " ORDER BY SUM(Quantity) desc ";
    qStr += " LIMIT " + QString::number(limit);

    return DatabaseHelper::getQueryModel(qStr);
}

QSqlQueryModel *InventoryHelper::getAveragePurchaseInterval(QString itemID, QDate fromDate, QDate toDate)
{
    QString qStr = " select round(sum(diff)/count(*),0), max(Voucher_Date) from ( ";
    qStr += " select  @a:=@a+1 serial_number, Voucher_Date, ";
    qStr += " if(@tmp =-1, 0, DateDiff(Voucher_Date, @tmp)) as diff, ";
    qStr += " @tmp:=Voucher_Date as dummy from purchase_invoice_details, ";
    qStr += " (SELECT @a:= 0) AS a, (SELECT @tmp:= -1) as tmp ";
    qStr += " where Inventory_Item_ID = '" + itemID + "' ";
    qStr += " AND Voucher_Date >= " + db->getDateString(fromDate);
    qStr += " AND Voucher_Date <= " + db->getDateString(toDate);
    qStr += " order by Voucher_Date) A";

    return DatabaseHelper::getQueryModel(qStr);
}

QSqlQueryModel *InventoryHelper::getGodownwiseSalesModel(QDate fromDate, QDate toDate, QString godownId)
{
    QString qStr = " SELECT  ItemID, ";
    qStr += " ( SELECT Item_Name FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where item_id = itemID ) as Item, ";
    qStr += " SUM(CR_QTY) as Quantity, SUM(Cr_Amount) as Value ";
    qStr += " FROM transaction_item_helper ";
    qStr += " Where from_location like '" + godownId + "'";
    qStr += " AND VoucherDate >= " + db->getDateString(fromDate);
    qStr += " AND VoucherDate <= " + db->getDateString(toDate);
    qStr += " GROUP BY ItemID";

    return DatabaseHelper::getQueryModel(qStr);
}

QSqlQueryModel *InventoryHelper::getItemWiseWaiterWise(QDateTime fromDate, QDateTime toDate,int salesmanID)
{
    salesVoucherDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper itemHelper;

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


    return DatabaseHelper::getQueryModel(qStr);

}

QSqlQueryModel *InventoryHelper::getItemTransValueByFrequency(int voucherType, QDate fromDate, QDate toDate, QString itemId)
{
    qDebug()<<Q_FUNC_INFO;
    QString tableName = GMVoucherTypes::getVoucherDetailsTablename(voucherType);
    QString qStr = " SELECT 'Daily' as Period, ";
    qStr += " (SELECT round(COALESCE(SUM(SubTotal),0),2) as 'Todays Sales' FROM " + tableName ;
    qStr += " WHERE Inventory_Item_ID like '" + itemId + "' AND Voucher_Date = CURRENT_DATE) as 'Current SubTotal', ";
    qStr += " (SELECT COALESCE(round(SUM(SubTotal)/Count(*),2),0) as 'Daily Average Sales' ";
    qStr += " FROM ( SELECT COALESCE(SUM(SubTotal),0) as SubTotal FROM " + tableName ;
    qStr += " where Inventory_Item_ID like '" + itemId + "' ";
    qStr += " AND Voucher_Date >= " + db->getDateString(fromDate);
    qStr += " AND Voucher_Date <= " + db->getDateString(toDate);
    qStr += " group by Voucher_Date )A) as 'Average'";
    qStr += " UNION ";
    qStr += " SELECT 'Week' , (SELECT round(COALESCE(SUM(SubTotal),0),2) as WeeklySum ";
    qStr += " FROM " + tableName + " where  Inventory_Item_ID like '" + itemId + "' ";
    qStr += " AND Voucher_Date >= subDate(CURRENT_DATE, DayOfWeek(CURRENT_DATE)) ";
    qStr += " AND Voucher_Date <= CURRENT_DATE) as 'CurrentSubTotal', ";
    qStr += " (SELECT COALESCE(round(SUM(WeeklySum)/COUNT(*),2),0) as WeeklyAvg ";
    qStr += " FROM (SELECT SUM(SubTotal) as WeeklySum, week(Voucher_Date) ";
    qStr += " FROM " + tableName + " where Inventory_Item_ID like '" + itemId + "' ";
    qStr += " AND Voucher_Date >= " + db->getDateString(fromDate);
    qStr += " AND Voucher_Date <= " + db->getDateString(toDate);
    qStr += " group by YEARWEEK(Voucher_Date) ) A ) as 'Average'";
    qStr += " UNION ";
    qStr += " SELECT 'Month', (SELECT round(COALESCE(SUM(SubTotal),0),2) as MonthSum ";
    qStr += " FROM " + tableName + " where  Inventory_Item_ID like '" + itemId + "' ";
    qStr += " AND Voucher_Date >= DATE_FORMAT(NOW() ,'%Y-%m-01') ";
    qStr += " AND Voucher_Date <= CURRENT_DATE) as 'MonthSubTotal', ";
    qStr += " (SELECT COALESCE(round(SUM(MonthSum)/COUNT(*),2),0) as WeeklyAvg ";
    qStr += " FROM (SELECT SUM(SubTotal) as MonthSum FROM " + tableName ;
    qStr += " where Inventory_Item_ID like '" + itemId + "' ";
    qStr += " AND Voucher_Date >= " + db->getDateString(fromDate);
    qStr += " AND Voucher_Date <= " + db->getDateString(toDate);
    qStr += " group by MONTH(Voucher_Date) ) A ) as 'Average' ";
    qStr += " UNION ";
    qStr += " SELECT 'Quarter', (SELECT round(COALESCE(SUM(SubTotal),0),2) as QuarterSum ";
    qStr += " FROM " + tableName + " where  Inventory_Item_ID like '" + itemId + "' ";
    qStr += " AND Voucher_Date >= MAKEDATE(YEAR(CURDATE()), 1) + INTERVAL QUARTER(CURDATE()) QUARTER";
    qStr += " - INTERVAL 1 QUARTER AND Voucher_Date <= CURRENT_DATE) as 'QuarterSubTotal', ";
    qStr += " (SELECT COALESCE(round(SUM(QuarterSum)/COUNT(*),2),0) as WeeklyAvg ";
    qStr += " FROM (SELECT SUM(SubTotal) as QuarterSum FROM " + tableName ;
    qStr += " where Inventory_Item_ID like '" + itemId + "' ";
    qStr += " AND Voucher_Date >= " + db->getDateString(fromDate);
    qStr += " AND Voucher_Date <= " + db->getDateString(toDate);
    qStr += " group by QUARTER(Voucher_Date) ) A ) as 'Average' ";
    qStr += " UNION ";
    qStr += " SELECT 'Year', (SELECT round(COALESCE(SUM(SubTotal),0),2) as AnnualSum ";
    qStr += " FROM " + tableName + " where  Inventory_Item_ID like '" + itemId + "' ";
    qStr += " AND Voucher_Date >= DATE_FORMAT(NOW() ,'%Y-01-01') ";
    qStr += " AND Voucher_Date <= CURRENT_DATE) as 'AnnualSubTotal', ";
    qStr += " (SELECT COALESCE(round(SUM(AnnualSum)/COUNT(*),2),0) as AnnualAvg ";
    qStr += " FROM (SELECT SUM(SubTotal) as AnnualSum FROM " + tableName ;
    qStr += " where Inventory_Item_ID like '" + itemId + "' ";
    qStr += " AND Voucher_Date >= " + db->getDateString(fromDate);
    qStr += " AND Voucher_Date <= " + db->getDateString(toDate);
    qStr += " group by YEAR(Voucher_Date) ) A ) as 'Average' ";

    return DatabaseHelper::getQueryModel(qStr);
}


QSqlQueryModel *InventoryHelper::getRankBySalesValue(QString itemId ,QDate fromDate, QDate toDate)
{
    qDebug()<<Q_FUNC_INFO;

    QString qStr = "Select serial_Number, totalSales, billCount FROM (";

    qStr += " SELECT  @a:=@a+1 serial_number, Item_ID, totalSales, billCount FROM ";
    qStr += " (SELECT inv.Item_ID , ";
    qStr += " SUM(SubTotal) as totalSales, COUNT(distinct Voucher_No) as billCount ";
    qStr +=  " FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name + " inv " ;
    qStr += " LEFT OUTER JOIN sales_invoice_details trans";
    qStr += " ON inv.Item_ID = trans.Inventory_Item_ID ";
    qStr += " WHERE Voucher_Date <= " + db->getDateString(toDate);
    qStr += " GROUP BY trans.Inventory_Item_ID ";
    qStr += " ORDER BY SUM(SubTotal) desc )A, ";
    qStr += " (SELECT @a:= 0) AS slNo )B ";
    qStr += " WHERE Item_ID like '" + itemId + "'";


    return DatabaseHelper::getQueryModel(qStr);

}

QSqlQueryModel *InventoryHelper::getItemwiseDeliveryNoteModel(QDateTime fromDate, QDateTime toDate)
{
    qDebug()<<Q_FUNC_INFO;

    QString qStr = "SELECT  'I', inventory_item_id,  ";
    qStr += " (SELECT Item_Name FROM Sales_Inventory_Items WHERE Item_Id = inventory_item_id) as 'Item', ";
    qStr += " round(SUM(Quantity),3) as 'Quantity' , round(SUM( SubTotal),2) as 'Amount' ";
    qStr += " FROM delivery_note_details ";
    qStr += " WHERE addTime(Voucher_Date,COALESCE(Time,'00:00:00')) >= " + DatabaseHelper::getDateTimeStringStatic(fromDate);
    qStr += " AND  addTime(Voucher_Date,COALESCE(Time,'00:00:00')) <= " + DatabaseHelper::getDateTimeStringStatic(toDate);
    qStr += " GROUP BY inventory_item_id ORDER BY Item ";


    return DatabaseHelper::getQueryModel(qStr);

}

QSqlQueryModel *InventoryHelper::getItemwiseReceiptNoteModel(QDateTime fromDate, QDateTime toDate)
{
    qDebug()<<Q_FUNC_INFO;

    QString qStr = "SELECT  'I', inventory_item_id,  ";
    qStr += " (SELECT Item_Name FROM Sales_Inventory_Items WHERE Item_Id = inventory_item_id) as 'Item', ";
    qStr += " round(SUM(Quantity),3) as 'Quantity' , round(SUM( SubTotal),2) as 'Amount' ";
    qStr += " FROM receipt_note_details ";
    qStr += " WHERE addTime(Voucher_Date,COALESCE(Time,'00:00:00')) >= " + DatabaseHelper::getDateTimeStringStatic(fromDate);
    qStr += " AND  addTime(Voucher_Date,COALESCE(Time,'00:00:00')) <= " + DatabaseHelper::getDateTimeStringStatic(toDate);
    qStr += " GROUP BY inventory_item_id ORDER BY Item ";


    return DatabaseHelper::getQueryModel(qStr);

}




QString InventoryHelper::getSalesOrderByModeOfService(QDate fromDate, QDate toDate)
{
    SalesOrderDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper itemHelper;
    PricelistDatabaseHelper priceHelper;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    QList<PriceListDataObject> prices = priceHelper.getAllPriceLists();
    //    QString     timestamp = "str_to_Date(concat(main." + shelper.Sales_Order_Main_Voucher_Date ;
    //    timestamp += ", substr(" + shelper.Sales_Order_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;

    QString qStr = "";
    qStr += "  select ";
    qStr += " ItemID, (select item_name FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where item_id = ItemID ) as Item,";
    int j=0;
    for(PriceListDataObject price:prices)
    {
        qStr += "max(Qty_" + QString::number(j) + ") as '" + price.priceListName + " Qty', ";
        j++;
    }

    qStr.truncate(qStr.lastIndexOf(","));

    //    qStr += "  max(DeliveryQty) as 'Delivery \n Qty',";
    //    qStr += " max(TakeAwayQty) as 'TakeAway \n Qty', ";
    //    qStr += " max(DineInQty) as 'DineIn \n Qty'";
    qStr += " from (";
    qStr += " select det.inventory_item_id as ItemID,  ";
    int i=0;
    for(PriceListDataObject price:prices)
    {
        qDebug()<<price.priceListID<<price.priceListName;
        qStr += "(case when Main.Mode_Of_Service = '" + QString::number(price.priceListID) ;
        qStr += "' then round(SUM(Quantity),3) else 0 end ) as 'Qty_" + QString::number(i) + "', ";
        i++;
    }
    qStr += " 'dummy' ";
    qStr += " from " + shelper.Sales_Order_Details_TableName + " det";
    qStr += " INNER JOIN " + shelper.Sales_Order_Main_TableName + " MAIN";
    qStr += " ON det.VOUCHER_NO = MAIN.VOUCHER_NO AND det.VOUCHER_PREFIX = MAIN.VOUCHER_PREFIX";
    qStr += " and det.voucher_Date >= " + db->getDateString(fromDate);
    qStr += " and det.voucher_Date <= " + db->getDateString(toDate);
    qStr += " group by det.inventory_item_id, main.mode_of_Service";
    qStr += " ) X group by ItemID ";

    return qStr;

}


QSqlQueryModel *InventoryHelper::getSalesOrderByTime(QDate fromDate, QDate toDate)
{
    SalesOrderDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper itemHelper;
    QSqlQueryModel* model = new QSqlQueryModel;
    PricelistDatabaseHelper priceHelper;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    //    QList<PriceListDataObject> prices = priceHelper.getAllPriceLists();

    QString     timestamp = "str_to_Date(concat(main." + shelper.Sales_Order_Main_Voucher_Date ;
    timestamp += ", substr(" + shelper.Sales_Order_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;

    QString qStr = "";

    qStr += "  select ";
    qStr += " ItemID, (select item_name FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where item_id = ItemID ) as Item,";
    qStr += " max(Lunch) as 'Lunch \n Qty',";
    qStr += " max(Dinner) as 'Dinner \n Qty' ";
    qStr += " from (";
    qStr += " select det.inventory_item_id as ItemID,  ";
    qStr += "(case when Time(Main.Delivery_Date) <= str_to_date('15:00:00', '%H:%i:%s')" ;
    qStr += " then round(SUM(Quantity),3) else 0 end ) as Lunch, ";
    qStr += "(case when Time(Main.Delivery_Date) > str_to_date('15:00:00', '%H:%i:%s')" ;
    qStr += " then round(SUM(Quantity),3) else 0 end ) as Dinner ";
    qStr += " from " + shelper.Sales_Order_Details_TableName + " det";
    qStr += " INNER JOIN " + shelper.Sales_Order_Main_TableName + " MAIN";
    qStr += " ON det.VOUCHER_NO = MAIN.VOUCHER_NO AND det.VOUCHER_PREFIX = MAIN.VOUCHER_PREFIX";
    qStr += " and det.voucher_Date >= " + db->getDateString(fromDate);
    qStr += " and det.voucher_Date <= " + db->getDateString(toDate);
    qStr += " group by det.inventory_item_id";
    qStr += " , (case when Time(Main.Delivery_Date) <= str_to_date('15:00:00', '%H:%i:%s')" ;
    qStr += " then 1 else 0 end ) ";
    qStr += " ) X group by ItemID ";
    qStr += " ORDER BY 2";

    qDebug()<<qStr;

    //    query.prepare(qStr);

    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}

