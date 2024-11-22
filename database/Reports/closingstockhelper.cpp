#include "closingstockhelper.h"
#include "database/Settings/ConfigurationSettingsDatabaseHelper.h"

ClosingStockHelper::ClosingStockHelper()
{
    db = new DatabaseHelper();
}

float ClosingStockHelper::getClosingStockOfItemTillDateWithBom(QString itemID, QDate date, bool bomFlag)
{
    // Closing stock of an item using transaction bom helper

    float balance=0;
    SalesInventoryItemDatabaseHelper items;
    ItemTransactionDatabaseHelper itemHelper;
    QString qStr = "";
    //    qStr = " select item." + itemHelper.Sales_Inventory_ItemName + ", item." + itemHelper.Sales_Inventory_OpeningStock + ",";
    //    qStr += " coalesce(" + itemHelper.Sales_Inventory_OpeningStock + ",0) - ";
    //    qStr += " coalesce(sum(trans." + trans.Transaction_Item_Helper_Cr_Qty + "),0)  + coalesce(sum(trans." + trans.Transaction_Item_Helper_Dr_Qty + "),0) - ";
    //    qStr += " coalesce(sum(stock." + trans.Transaction_Item_Helper_Cr_Qty + "),0)  + coalesce(sum(stock." + trans.Transaction_Item_Helper_Dr_Qty + "),0) as closing_stock ";
    //    qStr += " from " + itemHelper.Sales_Inventory_Table_Name + " item left join " + trans.Transaction_Item_Helper_TableName + " trans ";
    //    qStr += " on item." + itemHelper.Sales_Inventory_ItemId + " = trans." + trans.Transaction_Item_Helper_ItemID;
    //    qStr += " and trans." + trans.Transaction_Item_Helper_VoucherDate + " <='"+ db->getDateString(date);
    //    qStr += " left join " + trans.Transaction_Bom_Helper_TableName + " stock " ;
    //    qStr += " on item." + itemHelper.Sales_Inventory_ItemId + " = stock." + trans.Transaction_Item_Helper_ItemID;
    //    qStr += " and stock." + trans.Transaction_Item_Helper_VoucherDate + " <='"+ db->getDateString(date);
    //    qStr += " where item." + itemHelper.Sales_Inventory_ItemId + "  =:item ";


    qStr = " ( select  sum(debitQty - creditQty) from  ";
    qStr += " ( ";
    //    qStr += " select str_to_date('1900-01-01','%Y-%m-%d') as vdate, ";
    //    qStr += items.Sales_Inventory_ItemId + ", 0 as creditQty, coalesce(" + items.Sales_Inventory_OpeningStock + ",0) as debitQty, ";
    //    qStr += " 0 as cr_amount, " + items.Sales_Inventory_OpeningValue + " as dr_amount, ";
    //    qStr += " 'opening' as type from " + items.Sales_Inventory_Table_Name + " where " + items.Sales_Inventory_isPurchaseItem + " = 1 ";
    //    qStr += " UNION ALL ";
    qStr += " select trans." + itemHelper.Transaction_Item_Helper_VoucherDate + ", " +  itemHelper.Transaction_Item_Helper_ItemID + ", ";
    qStr += " (trans." + itemHelper.Transaction_Item_Helper_Cr_Qty + ") creditQty,(trans." + itemHelper.Transaction_Item_Helper_Dr_Qty + ") debitQty, ";
    qStr += "trans." + itemHelper.Transaction_Item_Helper_Cr_Qty + " * trans." + itemHelper.Transaction_Item_Helper_Price + " as cr_amount ";
    qStr += "trans." + itemHelper.Transaction_Item_Helper_Dr_Qty + " * trans." + itemHelper.Transaction_Item_Helper_Price + " as Dr_amount ";
    //    qStr += "trans." + itemHelper.Transaction_Item_Helper_Cr_Amount + ", trans." + itemHelper.Transaction_Item_Helper_Dr_Amount ;
    qStr += ", 'IN' as type ";
    qStr += " from " + itemHelper.Transaction_Item_Helper_TableName + " trans where ";
    qStr += " (select " + items.Sales_Inventory_isPurchaseItem + " from " + items.Sales_Inventory_Table_Name ;
    qStr += " where " + items.Sales_Inventory_ItemId + " = trans." + itemHelper.Transaction_Item_Helper_ItemID + ") = 1 ";
    qStr += " and " + itemHelper.Transaction_Item_Helper_VoucherDate + " <= " + db->getDateString(date);

    if(bomFlag){
        qStr += " UNION ALL ";
        qStr += " select trans." + itemHelper.Transaction_Item_Helper_VoucherDate + ", " +  itemHelper.Transaction_Item_Helper_ItemID + ", ";
        qStr += " (trans." + itemHelper.Transaction_Item_Helper_Cr_Qty + "), (trans." + itemHelper.Transaction_Item_Helper_Dr_Qty + "), ";
        qStr += "trans." + itemHelper.Transaction_Item_Helper_Cr_Qty + " * trans." + itemHelper.Transaction_Item_Helper_Price + " as cr_amount ";
        qStr += "trans." + itemHelper.Transaction_Item_Helper_Dr_Qty + " * trans." + itemHelper.Transaction_Item_Helper_Price + " as Dr_amount ";
        //    qStr += "trans." + itemHelper.Transaction_Item_Helper_Cr_Amount + ", trans." + itemHelper.Transaction_Item_Helper_Dr_Amount ;
        qStr += ", 'IN' as type ";
        qStr += " from " + itemHelper.Transaction_Bom_Helper_TableName + " trans where ";
        qStr +=  itemHelper.Transaction_Item_Helper_VoucherDate + " <=" + db->getDateString(date);
    }
    qStr += " ) A where item_id is not null";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);
    query.bindValue(":item",itemID);

    if(query.exec()){
        if(query.first()){
            balance = query.value(2).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }

    return balance;


}

float ClosingStockHelper::getClosingStockOfItemTillDate(QString itemID, QDate date)
{
    // Closing stock of item till date without Bom

    float balance=0;
    SalesInventoryItemDatabaseHelper itemHelper;
    ItemTransactionDatabaseHelper trans;
    QString qStr = "";
    qStr = " select round(";
    //    qStr += " coalesce(" + itemHelper.Sales_Inventory_OpeningStock + ",0) - ";
    qStr += " coalesce(sum(trans." + trans.Transaction_Item_Helper_Cr_Qty + " ),0)  + ";
    qStr += " coalesce(sum(trans." + trans.Transaction_Item_Helper_Dr_Qty + " ),0),3) ";
    qStr += " from " + itemHelper.Sales_Inventory_Table_Name + " item left join " ;
    qStr +=  trans.Transaction_Item_Helper_TableName + " trans on item." ;
    qStr += itemHelper.Sales_Inventory_ItemId + " = trans." + trans.Transaction_Item_Helper_ItemID;
    qStr += " and trans." + trans.Transaction_Item_Helper_VoucherDate + " <='"+date.toString("yyyy-MM-dd")+"'";
    qStr += " where item." + itemHelper.Sales_Inventory_ItemId + " = '" + itemID + "'";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);

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


QSqlQueryModel *ClosingStockHelper::getClosingStockListModelWithBom(QDate dateTo)
{
    // All items - stock & value only including bom data

    QSqlQueryModel* model = new QSqlQueryModel;
    SalesInventoryItemDatabaseHelper shelper;
    ItemTransactionDatabaseHelper itemHelper;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "";

    qStr = "SELECT " + shelper.Sales_Inventory_ItemName + ", ";
    //    qStr += " round(coalesce(" + shelper.Sales_Inventory_OpeningStock + ",0) + ";
    qStr += " sum(coalesce(trans." + itemHelper.Transaction_Item_Helper_Cr_Qty + " ,0)) - ";
    qStr += " sum(coalesce(trans." + itemHelper.Transaction_Item_Helper_Dr_Qty + ",0))+ ";
    qStr += " sum(coalesce(stock." + itemHelper.Transaction_Item_Helper_Cr_Qty + " ,0)) - ";
    qStr += " sum(coalesce(stock." + itemHelper.Transaction_Item_Helper_Dr_Qty + ",0)),2) as Stock,";
    qStr += " round(coalesce(" + shelper.Sales_Inventory_OpeningValue + ",0) + ";
    qStr += " sum(coalesce(trans." + itemHelper.Transaction_Item_Helper_Cr_Amount + " ,0)) - ";
    qStr += " sum(coalesce(trans." + itemHelper.Transaction_Item_Helper_Dr_Amount + ",0)),2) + ";
    qStr += " sum(coalesce(stock." + itemHelper.Transaction_Item_Helper_Cr_Amount + " ,0)) - ";
    qStr += " sum(coalesce(stock." + itemHelper.Transaction_Item_Helper_Dr_Amount + ",0)),2) as Value";
    qStr += " from " + shelper.Sales_Inventory_Table_Name + " left outer join " + itemHelper.Transaction_Item_Helper_TableName + " trans ";
    qStr += " on  " + shelper.Sales_Inventory_ItemId + " = trans." + itemHelper.Transaction_Item_Helper_ItemID;
    qStr += " left outer join " + itemHelper.Transaction_Bom_Helper_TableName + " stock " ;
    qStr += " on  " + shelper.Sales_Inventory_ItemId + " = stock." + itemHelper.Transaction_Item_Helper_ItemID;
    qStr += " where " + shelper.Sales_Inventory_isPurchaseItem + " =1 AND ";
    qStr += itemHelper.Transaction_Item_Helper_VoucherDate + "<= "+ db->getDateString(dateTo);
    qStr += " group by " + shelper.Sales_Inventory_ItemId;



    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}



QSqlQueryModel *ClosingStockHelper::getClosingStockListModelWithFifoPriceAndBom(QDate fromDate, QDate dateTo, bool bomFlag)
{
    // Stock value by fifo Including bom
    QSqlQueryModel* model = new QSqlQueryModel;
    SalesInventoryItemDatabaseHelper items;
    ItemTransactionDatabaseHelper itemHelper;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "";

    qStr = "select 'I' as Ind, item_id as Id, ";
    qStr += " (select distinct " + items.Sales_Inventory_ItemName + " from " + items.Sales_Inventory_Table_Name;
    qStr += " where " + items.Sales_Inventory_ItemId + " = B." + " item_ID) as Item, ";
    qStr += " cast(sum(Opening) as decimal(10,3)) as Opening,  ";
    qStr += " cast(sum(Inward) as decimal(10,3)) as Inward,  ";
    qStr += " cast(sum(Issue) as decimal(10,3)) as Issued ,  ";
    qStr += " cast(sum(Opening)+sum(Inward)-sum(Issue) as decimal(10,3)) AS 'Closing Stock',  ";
    qStr += " round(sum(coalesce(dr_amount,0) - coalesce(cr_amount,0)) ,2 ) as 'Stock Value'";
    qStr += " from  ";
    qStr += " ( select  item_id, ";
    qStr += " (case when vdate >= " + db->getDateString(fromDate) + " and vdate <= " + db->getDateString(dateTo) + " then (creditQty) else 0 end )as Issue, ";
    qStr += " (case when vdate >= " + db->getDateString(fromDate) + " and vdate <= " + db->getDateString(dateTo) + " then (debitQty)  else 0 end )as Inward, ";
    qStr += " (case when vdate < " + db->getDateString(fromDate) + "  or vdate is null then (debitQty) - (creditQty)  else 0 end) as Opening, ";
    qStr += " cr_amount, dr_amount from  ";
    qStr += " ( ";
    //    qStr += " select str_to_date('1900-01-01','%Y-%m-%d') as vdate, ";
    //    qStr += items.Sales_Inventory_ItemId + ", 0 as creditQty, coalesce(" + items.Sales_Inventory_OpeningStock + ",0) as debitQty, ";
    //    qStr += " 0 as cr_amount, " + items.Sales_Inventory_OpeningValue + " as dr_amount, ";
    //    qStr += " 'opening' as type from " + items.Sales_Inventory_Table_Name + " where " + items.Sales_Inventory_isPurchaseItem + " = 1 ";
    //    qStr += " UNION ALL ";
    qStr += " select trans." + itemHelper.Transaction_Item_Helper_VoucherDate + ", " +  itemHelper.Transaction_Item_Helper_ItemID + ", ";
    qStr += " (trans." + itemHelper.Transaction_Item_Helper_Cr_Qty + ") creditQty,(trans." + itemHelper.Transaction_Item_Helper_Dr_Qty + ") debitQty, ";
    qStr += "trans." + itemHelper.Transaction_Item_Helper_Cr_Qty + " * trans." + itemHelper.Transaction_Item_Helper_Price + " as cr_amount ";
    qStr += "trans." + itemHelper.Transaction_Item_Helper_Dr_Qty + " * trans." + itemHelper.Transaction_Item_Helper_Price + " as Dr_amount ";
    //    qStr += "trans." + itemHelper.Transaction_Item_Helper_Cr_Amount + ", trans." + itemHelper.Transaction_Item_Helper_Dr_Amount ;
    qStr += ", 'IN' as type ";
    qStr += " from " + itemHelper.Transaction_Item_Helper_TableName + " trans where ";
    qStr += " (select " + items.Sales_Inventory_isPurchaseItem + " from " + items.Sales_Inventory_Table_Name ;
    qStr += " where " + items.Sales_Inventory_ItemId + " = trans." + itemHelper.Transaction_Item_Helper_ItemID + ") = 1 ";
    qStr += " and " + itemHelper.Transaction_Item_Helper_VoucherDate + " <= str_to_date('" +dateTo.toString("yyyy-MM-dd")+ "','%Y-%m-%d')";

    if(bomFlag){
        qStr += " UNION ALL ";
        qStr += " select trans." + itemHelper.Transaction_Item_Helper_VoucherDate + ", " +  itemHelper.Transaction_Item_Helper_ItemID + ", ";
        qStr += " (trans." + itemHelper.Transaction_Item_Helper_Cr_Qty + "), (trans." + itemHelper.Transaction_Item_Helper_Dr_Qty + "), ";
        qStr += "trans." + itemHelper.Transaction_Item_Helper_Cr_Qty + " * trans." + itemHelper.Transaction_Item_Helper_Price + " as cr_amount ";
        qStr += "trans." + itemHelper.Transaction_Item_Helper_Dr_Qty + " * trans." + itemHelper.Transaction_Item_Helper_Price + " as Dr_amount ";
        //    qStr += "trans." + itemHelper.Transaction_Item_Helper_Cr_Amount + ", trans." + itemHelper.Transaction_Item_Helper_Dr_Amount ;
        qStr += ", 'IN' as type ";
        qStr += " from " + itemHelper.Transaction_Bom_Helper_TableName + " trans where ";
        qStr +=  itemHelper.Transaction_Item_Helper_VoucherDate + " <= str_to_date('" +dateTo.toString("yyyy-MM-dd")+ "','%Y-%m-%d')";
    }
    qStr += " ) A where item_id is not null ) B ";
    qStr += " group by item_id "  ;
    qStr += " order by 1";



    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}

QSqlQueryModel *ClosingStockHelper::getClosingStockByGodown(QDate fromDate, QDate toDate, QString godownID)
{

    QString qStr;

    SalesInventoryGroupsDatabaseHelper groupHelper;
    SalesInventoryItemDatabaseHelper items;
    ItemTransactionDatabaseHelper itemHelper;

    qStr = "SELECT (SELECT " + groupHelper.Sales_Inventory_Group_GroupName + " FROM " + groupHelper.Sales_Inventory_Group_TableName;
    qStr += " WHERE " + groupHelper.Sales_Inventory_Group_GroupId + " = INV." +  items.Sales_Inventory_GroupId + ")";
    qStr += " AS 'Group Name' , ";
    qStr +=  "  CONCAT(INV.item_name) as 'Item Name',  ";
    qStr += "  sum(coalesce(inQty.qty,0) - coalesce(outQty.qty,0) ) as Quantity  ";
    qStr += " FROM " +  items.Sales_Inventory_Table_Name + " INV LEFT OUTER JOIN ";
    qStr += " (SELECT " + itemHelper.Transaction_Item_Helper_ItemID + ", ";
    //    qStr += itemHelper.Transaction_Item_Helper_Serial_No + ", ";
    qStr += itemHelper.Transaction_Item_Helper_To_Location + ", ";
    qStr += "sum(" + itemHelper.Transaction_Item_Helper_Cr_Qty + ") qty";
    qStr += " FROM " + itemHelper.Transaction_Item_Helper_TableName ;
    qStr += " GROUP BY " + itemHelper.Transaction_Item_Helper_ItemID + ", ";
    //    qStr += itemHelper.Transaction_Item_Helper_Serial_No + ", ";
    qStr += itemHelper.Transaction_Item_Helper_To_Location + ") as inQty ";
    qStr += " ON " + items.Sales_Inventory_ItemId + " = inQty." + itemHelper.Transaction_Item_Helper_ItemID;
    qStr += " LEFT OUTER JOIN ";

    qStr += " (SELECT " + itemHelper.Transaction_Item_Helper_ItemID + ", ";
    //    qStr += itemHelper.Transaction_Item_Helper_Serial_No + ", ";
    qStr += itemHelper.Transaction_Item_Helper_From_Location + ", ";
    qStr += "sum(" + itemHelper.Transaction_Item_Helper_Dr_Qty + ") qty";
    qStr += " FROM " + itemHelper.Transaction_Item_Helper_TableName ;
    qStr += " GROUP BY " + itemHelper.Transaction_Item_Helper_ItemID + ", ";
    //    qStr += itemHelper.Transaction_Item_Helper_Serial_No + ", ";
    qStr += itemHelper.Transaction_Item_Helper_From_Location + ") as outQty ";
    qStr += " ON " + items.Sales_Inventory_ItemId + " = outQty." + itemHelper.Transaction_Item_Helper_ItemID;
    //    qStr += " AND inQty.serial_no = outQty.serial_no ";
    qStr += " And inQty.to_Location = outQty.from_Location";
    //    qStr += " Where InQty.to_location <> 'OUT'";
    qStr += " And coalesce(inQty.qty,0) - coalesce(outQty.qty,0) >0 ";
    //    if(godownID != "NULL")
    qStr += " AND " + itemHelper.Transaction_Item_Helper_To_Location + " = '" + godownID + "' ";
    qStr += " GROUP BY ITEM_ID ";

    qDebug()<<qStr;

    //    QSqlQueryModel *model = new QSqlQueryModel();
    //    model = DatabaseHelper::getQueryModel(qStr);
    //    return model;

    return DatabaseHelper::getQueryModel(qStr);
}

QSqlQueryModel *ClosingStockHelper::getGroupwiseClosingStockByGodownAndLPP(QDate fromDate, QDate dateTo,
                                                                           QString godownID, QString GroupId,
                                                                           bool show0s, bool bomFlag,
                                                                           bool showTransWithinPeriod ,
                                                                           bool showGodownTransfers,
                                                                           bool showVarianceVal)
{
    SalesInventoryItemDatabaseHelper items;
    ItemTransactionDatabaseHelper itemHelper;
    PurchaseVoucherDatabaseHelper pHelper;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "";

    bool showVariance = showVarianceVal;

    qDebug()<<showGodownTransfers;

    qStr += " select Ind,  Id,  Name ,  ";
    qStr += " coalesce(cast(sum(Opening) as decimal(10,3)),0) as Opening,   ";
    qStr += " coalesce(cast(sum(Inward) as decimal(10,3)),0) as Inward,   ";
    qStr += " coalesce(cast(sum(Issue) as decimal(10,3)),0) as Issued ,   ";
    qStr += " coalesce(cast(sum(Opening)+sum(Inward)-sum(Issue) as decimal(10,3)),0) AS 'Closing Stock',  ";
    qStr += " coalesce(sum(coalesce(purch.price , coalesce(std_cost,0)) *((Opening)+(Inward)-(Issue))),0)  as 'Stock Value' ";
    if(showVariance){
        qStr += ", '' as Variance, '' as PourCost, '' as ParLevel ";
    }
    qStr += " from   ";
    qStr += " (   ";
    qStr += " select trans.VoucherDate vDate, ItemID as item_id, ";
    qStr += " (case when VoucherDate < " + db->getDateString(fromDate);
    qStr += " or VoucherDate is null then (dr_qty) - (cr_qty)  else 0 end) as Opening, ";
    if(godownID != "%")
        qStr += " 0 as Inward, ";
    else
    {
        qStr += " (case when VoucherDate >= " + db->getDateString(fromDate);
        qStr += " and VoucherDate <= " + db->getDateString(dateTo) + " then (dr_qty)  else 0 end )as Inward , ";
    }
    qStr += " (case when VoucherDate >= "+ db->getDateString(fromDate);
    qStr += " and VoucherDate <= " + db->getDateString(dateTo) + " then (cr_qty) else 0 end )as Issue,  ";
    qStr += " trans.Dr_Amount , trans.Cr_Amount ";
    qStr += " from Transaction_Item_Helper trans where     ";
    qStr += " VoucherDate <= " + db->getDateString(dateTo) ;
    // for stock by godown, cr/dr qty should be taken separately corresponding to the from/to locations
    // hence using a union if search is by godown
    if(godownID != "%"){
        qStr += " and (";
        qStr += " From_Location = '" + godownID + "'";
        if(!showGodownTransfers)
            qStr += " AND To_Location  = 'OUT'";
        qStr += " ) UNION ALL ";
        qStr += " select trans.VoucherDate vDate, ItemID as item_id, ";
        qStr += " (case when VoucherDate < " + db->getDateString(fromDate);
        qStr += " or VoucherDate is null then (dr_qty) - (cr_qty)  else 0 end) as Opening, ";
        qStr += " (case when VoucherDate >= " + db->getDateString(fromDate);
        qStr += " and VoucherDate <= " + db->getDateString(dateTo) + " then (dr_qty)  else 0 end )as Inward , ";
        if(godownID != "%")
            qStr += " 0 as Issue, ";
        else
        {
            qStr += " (case when VoucherDate >= "+ db->getDateString(fromDate);
            qStr += " and VoucherDate <= " + db->getDateString(dateTo) + " then (cr_qty) else 0 end )as Issue,  ";
        }
        qStr += " trans.Dr_Amount , trans.Cr_Amount ";
        qStr += " from Transaction_Item_Helper trans where     ";
        qStr += " VoucherDate <= " + db->getDateString(dateTo) ;

        qStr += " and (";
        qStr += " To_Location = '" + godownID + "' ";
        if(!showGodownTransfers)
            qStr += " AND From_Location  = 'OUT' ";
        qStr += " )  ";
    }
    if(bomFlag)
    {
        qStr += " UNION ALL ";
        qStr += " select trans.VoucherDate vDate, ItemID as item_id, ";
        qStr += " (case when VoucherDate < " + db->getDateString(fromDate);
        qStr += " or VoucherDate is null then (dr_qty) - (cr_qty)  else 0 end) as Opening, ";
        if(godownID != "%")
            qStr += " 0 as Inward, ";
        else
        {
            qStr += " (case when VoucherDate >= " + db->getDateString(fromDate);
            qStr += " and VoucherDate <= " + db->getDateString(dateTo) + " then (dr_qty)  else 0 end )as Inward , ";
        }
        qStr += " (case when VoucherDate >= "+ db->getDateString(fromDate);
        qStr += " and VoucherDate <= " + db->getDateString(dateTo) + " then (cr_qty) else 0 end )as Issue,  ";
        qStr += " trans.Dr_Amount , trans.Cr_Amount ";
        qStr += " from Transaction_BOM_Helper trans where     ";
        qStr += " VoucherDate <= " + db->getDateString(dateTo) ;
        if(godownID != "%"){
            qStr += " and (";
            qStr += " From_Location = '" + godownID + "'";
            if(!showGodownTransfers)
                qStr += " AND To_Location  = 'OUT'";
            qStr += " ) UNION ALL ";
            qStr += " select trans.VoucherDate vDate, ItemID as item_id, ";
            qStr += " (case when VoucherDate < " + db->getDateString(fromDate);
            qStr += " or VoucherDate is null then (dr_qty) - (cr_qty)  else 0 end) as Opening, ";
            qStr += " (case when VoucherDate >= " + db->getDateString(fromDate);
            qStr += " and VoucherDate <= " + db->getDateString(dateTo) + " then (dr_qty)  else 0 end )as Inward , ";
            if(godownID != "%")
                qStr += " 0 as Issue, ";
            else
            {
                qStr += " (case when VoucherDate >= "+ db->getDateString(fromDate);
                qStr += " and VoucherDate <= " + db->getDateString(dateTo) + " then (cr_qty) else 0 end )as Issue,  ";
            }
            qStr += " trans.Dr_Amount , trans.Cr_Amount ";
            qStr += " from Transaction_Bom_Helper trans where     ";
            qStr += " VoucherDate <= " + db->getDateString(dateTo) ;

            qStr += " and (";
            qStr += " To_Location = '" + godownID + "' ";
            if(!showGodownTransfers)
                qStr += " AND From_Location  = 'OUT' ";
            qStr += " )  ";
        }
    }
    qStr += " ) A ";
    qStr += " right outer join ";
    qStr += " (select ItemGrp1.group_name as Name, Item.item_id, 'G' as Ind, ItemGrp1.group_id as Id, Item.std_cost from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
    qStr += " inner join ";
    qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
    qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
    qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName + " grp ";
    qStr += "  on item.group_id = grp.group_id";
    qStr += "  union all ";
    qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
    qStr += "  from       " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName + " p ";
    qStr += "  inner join cte ";
    qStr += "  on p.group_id = cte.parent_id )";
    qStr += " select cte.group_name, cte.item_id, cte.group_id ";
    qStr += " from cte where cte.parent_id = '"+ GroupId +"'";
    qStr += " ) itemGrp1 ";
    qStr += " on itemGrp1.item_id = item.item_id and item.isStoCkItem = 1 ";
    qStr += " union ";
    qStr += " select Item2.Item_name as Name, Item2.Item_id, 'I' as ind, Item2.Item_Id as Id, Item2.std_cost from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
    qStr += " where Item2.group_id = '"+ GroupId +"' AND item2.isStockItem = 1";
    qStr += " ) inv";
    qStr += " on A.item_id = inv.item_id ";
    qStr += " left outer join (SELECT Inventory_Item_ID, Voucher_Date, Price ";
    qStr += " FROM (SELECT  *, ROW_NUMBER() OVER (PARTITION BY Inventory_Item_ID ORDER By Voucher_Date desc) AS rn ";
    qStr += " FROM purchase_invoice_details where Voucher_Date <= " + db->getDateString(dateTo) + " ) x ";
    qStr += " WHERE rn = 1) purch ";
    qStr += " on A.item_id = purch.inventory_item_id ";
    qStr += " group by Name "  ;
    if(!show0s){
        if(showTransWithinPeriod)
            qStr += " having sum(Inward) <> 0 or sum(Issue) <> 0 or coalesce(sum(Opening)+sum(Inward)-sum(Issue),0) <> 0";
        else
            qStr += " having coalesce(sum(Opening)+sum(Inward)-sum(Issue),0) <> 0 ";
    }
    qStr += " order by 3";

    qDebug()<<qStr;
    return DatabaseHelper::getQueryModel(qStr);
}


QSqlQueryModel *ClosingStockHelper::getGroupwiseClosingStockByGodownAndStdCost(QDate fromDate, QDate dateTo, QString godownID, QString GroupId, bool show0s, bool bomFlag)
{
    SalesInventoryItemDatabaseHelper items;
    ItemTransactionDatabaseHelper itemHelper;
    PurchaseVoucherDatabaseHelper pHelper;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "";

    qStr += " SELECT Ind, Id, Name, Opening, Inward, Issued, ClosingStock as 'Closing Stock', StockValue 'Stock Value' from ( ";
    qStr += "select ItemGrp.Ind, ItemGrp.Id, Name,   ";
    //    qStr += " (select distinct " + items.Sales_Inventory_ItemName + " from " + items.Sales_Inventory_Table_Name;
    //    qStr += " where " + items.Sales_Inventory_ItemId + " = B." + " item_ID) as Item, ";
    qStr += " cast(sum(Opening) as decimal(10,3)) as Opening,  ";
    qStr += " cast(sum(Inward) as decimal(10,3)) as Inward,  ";
    qStr += " cast(sum(Issue) as decimal(10,3)) as Issued ,  ";
    qStr += " cast(sum(Opening)+sum(Inward)-sum(Issue) as decimal(10,3)) AS 'ClosingStock',  ";
    //    qStr += "coalesce((SELECT "+ pHelper.Purchase_Invoice_Details_Price+ " from "+ pHelper.Purchase_Invoice_Details_TableName;
    //    qStr += " where "+pHelper.Purchase_Invoice_Details_Inventory_Item_ID + "= B.item_ID order by ";
    //    qStr += pHelper.Purchase_Invoice_Details_Voucher_Date + " desc limit 1),0) * ";
    qStr += " cast(sum(Price) as decimal(10,3)) as 'StockValue'";
    qStr += " from  ";
    qStr += " ( select  item_id, ";
    qStr += " (case when vdate >= " + db->getDateString(fromDate) + " and vdate <= str_to_date('" + dateTo.toString("yyyy-MM-dd") + "','%Y-%m-%d') then (creditQty) else 0 end )as Issue, ";
    qStr += " (case when vdate >= str_to_date('" + fromDate.toString("yyyy-MM-dd") + "','%Y-%m-%d') and vdate <= str_to_date('" + dateTo.toString("yyyy-MM-dd") + "','%Y-%m-%d') then (debitQty)  else 0 end )as Inward, ";
    qStr += " (case when vdate <  str_to_date('" + fromDate.toString("yyyy-MM-dd") + "','%Y-%m-%d')  or vdate is null then (debitQty) - (creditQty)  else 0 end) as Opening, ";
    qStr += " cr_amount, dr_amount, ";
    qStr += " coalesce((SELECT "+ items.Sales_Inventory_StdCost+ " from "+ items.Sales_Inventory_Table_Name;
    qStr += " where "+items.Sales_Inventory_ItemId + "= item_ID),0) * (debitQty - creditQty) as Price ";
    qStr += " from  ";
    qStr += " ( ";
    //    qStr += " select str_to_date('1900-01-01','%Y-%m-%d') as vdate, ";
    //    qStr += items.Sales_Inventory_ItemId + ", coalesce(" + items.Sales_Inventory_OpeningStock + ",0) as debitQty, 0 as creditQty, ";
    //    qStr += " 0 as dr_amount, " + items.Sales_Inventory_OpeningValue + " as cr_amount, ";
    //    qStr += " 'opening' as type from " + items.Sales_Inventory_Table_Name + " where " + items.Sales_Inventory_isPurchaseItem + " = 1 ";
    //    qStr += " UNION ALL ";
    qStr += " select trans.VoucherDate vDate, ItemID as item_id,  0 , (trans.Cr_Qty) creditQty,  trans.Dr_Amount , trans.Cr_Amount, 'FROM' as type  ";
    qStr += " from Transaction_Item_Helper trans where  From_Location like '" + godownID + "'  and VoucherDate <= str_to_date('2019-10-16','%Y-%m-%d') ";
    qStr += " and (select isStockItem FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where itemID = item_ID) = 1 ";
    qStr += " UNION ALL   ";
    qStr += " select trans.VoucherDate, ItemID,  (trans.Dr_Qty) , 0,  trans.Dr_Amount , trans.Cr_Amount, 'TO' as type  ";
    qStr += " from Transaction_Item_Helper trans where  To_Location like '" + godownID + "'  and VoucherDate <= str_to_date('2019-10-16','%Y-%m-%d') ";
    qStr += " and (select isStockItem FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where itemID = item_ID) = 1 ";

    if(bomFlag){
        qStr += " UNION ALL";
        qStr += " select trans.VoucherDate, ItemID as item_id,  0 , (trans.Cr_Qty),  trans.Dr_Amount , trans.Cr_Amount, 'FROM' as type  ";
        qStr += " from Transaction_Bom_Helper trans where  From_Location like '" + godownID + "'  and VoucherDate <= str_to_date('2019-10-16','%Y-%m-%d') ";
        qStr += " and (select isStockItem FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where item_ID = itemID) = 1 ";
        qStr += " UNION ALL   ";
        qStr += " select trans.VoucherDate, ItemID,  (trans.Dr_Qty) , 0,  trans.Dr_Amount , trans.Cr_Amount, 'TO' as type  ";
        qStr += " from Transaction_Bom_Helper trans where  To_Location like '" + godownID + "'  and VoucherDate <= str_to_date('2019-10-16','%Y-%m-%d') ";
        qStr += " and (select isStockItem FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where item_ID = itemID) = 1 ";

    }
    qStr += " ) A where item_id is not null ) B ";
    qStr += " inner join ";
    qStr += " (select ItemGrp1.group_name as Name, Item.item_id, 'G' as Ind, ItemGrp1.group_id as Id from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
    qStr += " inner join ";
    qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
    qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
    qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName + " grp ";
    qStr += "  on item.group_id = grp.group_id";
    qStr += "  union all ";
    qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
    qStr += "  from       " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName + " p ";
    qStr += "  inner join cte ";
    qStr += "  on p.group_id = cte.parent_id )";
    qStr += " select cte.group_name, cte.item_id, cte.group_id ";
    qStr += " from cte where cte.parent_id = '"+ GroupId +"'";
    qStr += " ) itemGrp1 ";
    qStr += " on itemGrp1.item_id = item.item_id ";
    qStr += " union ";
    qStr += " select Item2.Item_name as Name, Item2.Item_id, 'I' as ind, Item2.Item_Id as Id from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
    qStr += " where Item2.group_id = '"+ GroupId +"'";
    qStr += " ) ItemGrp";
    qStr += " on B.Item_Id = ItemGrp.Item_Id  ";
    qStr += " group by Name "  ;
    qStr += ") X ";

    if(!show0s){
        qStr += "  where ClosingStock <>0 ";
    }
    qStr += " order by 1";

    qDebug()<<qStr;
    return DatabaseHelper::getQueryModel(qStr);
}


QSqlQueryModel *ClosingStockHelper::getGroupwiseClosingStockByGodownAndFifoPrice(QDate fromDate, QDate dateTo, QString godownID, QString GroupId, bool show0s, bool bomFlag)
{
    SalesInventoryItemDatabaseHelper items;
    ItemTransactionDatabaseHelper itemHelper;
    PurchaseVoucherDatabaseHelper pHelper;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "";

    qStr += " SELECT Ind, Id, Name, Opening, Inward, Issued, ClosingStock as 'Closing Stock', StockValue 'Stock Value' from ( ";
    qStr += "select ItemGrp.Ind, ItemGrp.Id, Name,   ";
    //    qStr += " (select distinct " + items.Sales_Inventory_ItemName + " from " + items.Sales_Inventory_Table_Name;
    //    qStr += " where " + items.Sales_Inventory_ItemId + " = B." + " item_ID) as Item, ";
    qStr += " cast(sum(Opening) as decimal(10,3)) as Opening,  ";
    qStr += " cast(sum(Inward) as decimal(10,3)) as Inward,  ";
    qStr += " cast(sum(Issue) as decimal(10,3)) as Issued ,  ";
    qStr += " cast(sum(Opening)+sum(Inward)-sum(Issue) as decimal(10,3)) AS 'ClosingStock',  ";
    //    qStr += "coalesce((SELECT "+ pHelper.Purchase_Invoice_Details_Price+ " from "+ pHelper.Purchase_Invoice_Details_TableName;
    //    qStr += " where "+pHelper.Purchase_Invoice_Details_Inventory_Item_ID + "= B.item_ID order by ";
    //    qStr += pHelper.Purchase_Invoice_Details_Voucher_Date + " desc limit 1),0) * ";
    qStr += " round(sum(coalesce(dr_amount,0) - coalesce(cr_amount,0)) ,2 ) as 'StockValue'";
    qStr += " from  ";
    qStr += " ( select  item_id, ";
    qStr += " (case when vdate >= " + db->getDateString(fromDate) + " and vdate <= str_to_date('" + dateTo.toString("yyyy-MM-dd") + "','%Y-%m-%d') then (creditQty) else 0 end )as Issue, ";
    qStr += " (case when vdate >= str_to_date('" + fromDate.toString("yyyy-MM-dd") + "','%Y-%m-%d') and vdate <= str_to_date('" + dateTo.toString("yyyy-MM-dd") + "','%Y-%m-%d') then (debitQty)  else 0 end )as Inward, ";
    qStr += " (case when vdate <  str_to_date('" + fromDate.toString("yyyy-MM-dd") + "','%Y-%m-%d')  or vdate is null then (debitQty) - (creditQty)  else 0 end) as Opening, ";
    qStr += " cr_amount, dr_amount, ";
    qStr += " coalesce((SELECT "+ items.Sales_Inventory_StdCost+ " from "+ items.Sales_Inventory_Table_Name;
    qStr += " where "+items.Sales_Inventory_ItemId + "= item_ID),0) * (debitQty - creditQty) as Price ";
    qStr += " from  ";
    qStr += " ( ";

    qStr += " select trans.VoucherDate vDate, ItemID as item_id,  0 debitQty, (trans.Cr_Qty) creditQty,  trans.Dr_Amount , trans.Cr_Amount, 'FROM' as type  ";
    qStr += " from Transaction_Item_Helper trans where  From_Location like '" + godownID + "'  and VoucherDate <= " + db->getDateString(dateTo);
    qStr += " and (select isStockItem FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where itemID = item_ID) = 1 ";
    qStr += " UNION ALL   ";
    qStr += " select trans.VoucherDate, ItemID,  (trans.Dr_Qty) , (trans.Cr_Qty), 0 , trans.Cr_Amount, 'TO' as type  ";
    qStr += " from Transaction_Item_Helper trans where  To_Location like '" + godownID + "'  and VoucherDate <= " + db->getDateString(dateTo);
    qStr += " and (select isStockItem FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where itemID = item_ID) = 1 ";

    if(bomFlag){
        qStr += " UNION ALL";
        qStr += " select trans.VoucherDate, ItemID as item_id,  0, (trans.Cr_Qty),  trans.Dr_Amount , trans.Cr_Amount, 'FROM' as type  ";
        qStr += " from Transaction_Bom_Helper trans where  From_Location like '" + godownID + "'  and VoucherDate <= " + db->getDateString(dateTo);
        qStr += " and (select isStockItem FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where item_ID = itemID) = 1 ";
        qStr += " UNION ALL   ";
        qStr += " select trans.VoucherDate, ItemID,  (trans.Dr_Qty) , 0,  trans.Dr_Amount , trans.Cr_Amount, 'TO' as type  ";
        qStr += " from Transaction_Bom_Helper trans where  To_Location like '" + godownID + "'  and VoucherDate <= " + db->getDateString(dateTo);
        qStr += " and (select isStockItem FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where item_ID = itemID) = 1 ";

    }
    qStr += " ) A where item_id is not null ) B ";
    qStr += " inner join ";
    qStr += " (select ItemGrp1.group_name as Name, Item.item_id, 'G' as Ind, ItemGrp1.group_id as Id from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
    qStr += " inner join ";
    qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
    qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
    qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName + " grp ";
    qStr += "  on item.group_id = grp.group_id";
    qStr += "  union all ";
    qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
    qStr += "  from       " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName + " p ";
    qStr += "  inner join cte ";
    qStr += "  on p.group_id = cte.parent_id )";
    qStr += " select cte.group_name, cte.item_id, cte.group_id ";
    qStr += " from cte where cte.parent_id = '"+ GroupId +"'";
    qStr += " ) itemGrp1 ";
    qStr += " on itemGrp1.item_id = item.item_id ";
    qStr += " union ";
    qStr += " select Item2.Item_name as Name, Item2.Item_id, 'I' as ind, Item2.Item_Id as Id from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
    qStr += " where Item2.group_id = '"+ GroupId +"'";
    qStr += " ) ItemGrp";
    qStr += " on B.Item_Id = ItemGrp.Item_Id  ";
    qStr += " group by Name "  ;
    qStr += ") X ";

    if(!show0s){
        qStr += "  where ClosingStock <>0 ";
    }
    qStr += " order by 1";

    qDebug()<<qStr;
    return DatabaseHelper::getQueryModel(qStr);
}

QSqlQueryModel *ClosingStockHelper::getItemwiseClosingStockOfGrpByGodownAndLPP(QDate fromDate, QDate dateTo,
                                                                               QString godownID,
                                                                               QString GroupId, bool show0s,
                                                                               bool showTransWithinPeriod,
                                                                               bool showGodownTransfers,
                                                                               bool bomFlag,
                                                                               bool showVariance)
{

    qDebug()<<Q_FUNC_INFO<<showVariance;
    PurchaseVoucherDatabaseHelper pHelper;
    QSqlQueryModel* model = new QSqlQueryModel;
    SalesInventoryItemDatabaseHelper items;
    ItemTransactionDatabaseHelper itemHelper;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "";
    float showStockAdj = ConfigurationSettingsDatabaseHelper::getValue(showStockAdjInReport, false).toBool();
    if(showVariance)
        showStockAdj = true;

    qStr += " select 'I' as Ind, A.item_id as Id,  Item_Name as Item,  "; //0,1,2
    qStr += " coalesce(cast(sum(Opening) as decimal(10,3)),0) as Opening,   "; //3
    qStr += " coalesce(cast(sum(Inward) as decimal(10,3)),0) as Inward,   "; //4
    qStr += " coalesce(cast(sum(Issue) as decimal(10,3)),0) as Issued ,   "; //5
    if(showStockAdj)
        qStr += " cast(coalesce(adj_value,0) as decimal(10,3)) as 'Stock \n Adjustment', "; //6
    qStr += " coalesce(cast(sum(Opening)+sum(Inward)-sum(Issue) as decimal(10,3)),0) AS 'Closing Stock',  "; //7
    qStr += " coalesce(round(coalesce(purch.price , coalesce(std_cost,0)) *(sum(Opening)+sum(Inward)-sum(Issue)),2),0)   as 'Stock Value' "; //8
    if(showVariance){
        qStr += ", coalesce(purch.price , coalesce(std_cost,0)) as LPP, '' as Variance, '' as 'Variance%', '' as PourCost, '' as ParLevel  "; //9,10,11, 12
    }
    qStr += " from   ";
    qStr += " (   ";
    qStr += " select trans.VoucherDate vDate, ItemID as item_id, ";
    qStr += " (case when VoucherDate < " + db->getDateString(fromDate) + "or VoucherDate is null then ";
    if( godownID != "%"){
        qStr += " - (cr_qty) ";
    }
    else {

        qStr += "  (dr_qty) - (cr_qty)  ";
    }
    qStr += " else 0 end) as Opening, ";

    if(godownID != "%")
        qStr += " 0 as Inward, ";
    else
    {
        qStr += " (case when VoucherDate >= " + db->getDateString(fromDate);
        qStr += " and VoucherDate <= " + db->getDateString(dateTo) + " then (dr_qty)  else 0 end )as Inward , ";
    }
    qStr += " (case when VoucherDate >= "+ db->getDateString(fromDate);
    qStr += " and VoucherDate <= " + db->getDateString(dateTo) + " then (cr_qty) else 0 end )as Issue,  ";
    qStr += " trans.Dr_Amount , trans.Cr_Amount ";
    qStr += " from Transaction_Item_Helper trans where     ";
    qStr += " VoucherDate <= " + db->getDateString(dateTo) ;
    // for stock by godown, cr/dr qty should be taken separately corresponding to the from/to locations
    // hence using a union if search is by godown
    if(godownID != "%"){
        qStr += " and (";
        qStr += " From_Location = '" + godownID + "'";
        if(!showGodownTransfers)
            qStr += " AND To_Location  = 'OUT'";
        qStr += " ) UNION ALL ";
        qStr += " select trans.VoucherDate vDate, ItemID as item_id, ";
        qStr += " (case when VoucherDate < " + db->getDateString(fromDate) + "or VoucherDate is null then ";
        if( godownID != "%"){
            qStr += " (dr_qty) ";
        }
        else {

            qStr += "  (dr_qty) - (cr_qty)  ";
        }
        qStr += " else 0 end) as Opening, ";
        qStr += " (case when VoucherDate >= " + db->getDateString(fromDate);
        qStr += " and VoucherDate <= " + db->getDateString(dateTo) + " then (dr_qty)  else 0 end )as Inward , ";
        if(godownID != "%")
            qStr += " 0 as Issue, ";
        else
        {
            qStr += " (case when VoucherDate >= "+ db->getDateString(fromDate);
            qStr += " and VoucherDate <= " + db->getDateString(dateTo) + " then (cr_qty) else 0 end )as Issue,  ";
        }
        qStr += " trans.Dr_Amount , trans.Cr_Amount ";
        qStr += " from Transaction_Item_Helper trans where     ";
        qStr += " VoucherDate <= " + db->getDateString(dateTo) ;

        qStr += " and (";
        qStr += " To_Location = '" + godownID + "' ";
        if(!showGodownTransfers)
            qStr += " AND From_Location  = 'OUT' ";
        qStr += " )  ";
    }
    if(bomFlag)
    {
        qStr += " UNION ALL ";
        qStr += " select trans.VoucherDate vDate, ItemID as item_id, ";
        qStr += " (case when VoucherDate < " + db->getDateString(fromDate) + "or VoucherDate is null then ";
        if( godownID != "%"){
            qStr += " - (cr_qty) ";
        }
        else {

            qStr += "  (dr_qty) - (cr_qty)  ";
        }
        qStr += " else 0 end) as Opening, ";
        if(godownID != "%")
            qStr += " 0 as Inward, ";
        else
        {
            qStr += " (case when VoucherDate >= " + db->getDateString(fromDate);
            qStr += " and VoucherDate <= " + db->getDateString(dateTo) + " then (dr_qty)  else 0 end )as Inward , ";
        }
        qStr += " (case when VoucherDate >= "+ db->getDateString(fromDate);
        qStr += " and VoucherDate <= " + db->getDateString(dateTo) + " then (cr_qty) else 0 end )as Issue,  ";
        qStr += " trans.Dr_Amount , trans.Cr_Amount ";
        qStr += " from Transaction_BOM_Helper trans where     ";
        qStr += " VoucherDate <= " + db->getDateString(dateTo) ;
        if(godownID != "%"){
            qStr += " and (";
            qStr += " From_Location = '" + godownID + "'";
            if(!showGodownTransfers)
                qStr += " AND To_Location  = 'OUT'";
            qStr += " ) UNION ALL ";
            qStr += " select trans.VoucherDate vDate, ItemID as item_id, ";
            qStr += " (case when VoucherDate < " + db->getDateString(fromDate) + "or VoucherDate is null then ";
            if( godownID != "%"){
                qStr += "(dr_qty) ";
            }
            else {

                qStr += "  (dr_qty) - (cr_qty)  ";
            }
            qStr += " else 0 end) as Opening, ";
            qStr += " (case when VoucherDate >= " + db->getDateString(fromDate);
            qStr += " and VoucherDate <= " + db->getDateString(dateTo) + " then (dr_qty)  else 0 end )as Inward , ";
            if(godownID != "%")
                qStr += " 0 as Issue, ";
            else
            {
                qStr += " (case when VoucherDate >= "+ db->getDateString(fromDate);
                qStr += " and VoucherDate <= " + db->getDateString(dateTo) + " then (cr_qty) else 0 end )as Issue,  ";
            }
            qStr += " trans.Dr_Amount , trans.Cr_Amount ";
            qStr += " from Transaction_Bom_Helper trans where     ";
            qStr += " VoucherDate <= " + db->getDateString(dateTo) ;

            qStr += " and (";
            qStr += " To_Location = '" + godownID + "' ";
            if(!showGodownTransfers)
                qStr += " AND From_Location  = 'OUT' ";
            qStr += " )  ";
        }
    }
    qStr += " ) A ";
    qStr += " right outer join ";
    if(GroupId != "0"){
        qStr += "(select Item.item_id, Item.isStockItem, Item.Item_Name, ";
        qStr += " Item.Std_Cost from  " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name + " item inner join  ";
        qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as ";
        qStr += " (  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id  ";
        qStr += " FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name + " item left outer join ";
        qStr +=  SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName + " grp  ";
        qStr += " on item.group_id = grp.group_id  ";
        qStr += " union all   select     p.group_id, p.group_name,p.parent_id, item_id  from ";
        qStr +=  SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName + " p   inner join cte ";
        qStr += " on p.group_id = cte.parent_id ) ";
        qStr += " select cte.group_name, cte.item_id from cte where cte.parent_id = '" + GroupId + "' ) itemGrp1  ";
        qStr += " on itemGrp1.item_id = item.item_id  union  ";
        qStr += " select Item2.Item_id, Item2.isStockItem, Item2.Item_Name, Item2.Std_Cost from ";
        qStr +=  SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name + " Item2  ";
        qStr += " where Item2.group_id = '" + GroupId + "' ) inv ";
    }
    else {
        qStr += SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name + " inv ";
    }
    qStr += " on A.item_id = inv.item_id ";
    qStr += " left outer join (SELECT Inventory_Item_ID, Voucher_Date, Price ";
    qStr += " FROM (SELECT  *, ROW_NUMBER() OVER (PARTITION BY Inventory_Item_ID ORDER By Voucher_Date desc, _id desc) AS rn ";
    qStr += " FROM purchase_invoice_details where Voucher_Date <= " + db->getDateString(dateTo) + " ) x ";
    qStr += " WHERE rn = 1) purch ";
    qStr += " on A.item_id = purch.inventory_item_id ";
    if(showStockAdj){
        qStr += " left outer join (SELECT Inventory_Item_Id, sum(Adj_Value) as adj_value from ";
        qStr += " stock_details where Voucher_Date <= " + db->getDateString(dateTo) + " and Voucher_Date >= " + db->getDateString(fromDate);
        qStr += " group by inventory_item_id ";
        qStr += " union select itemId, sum( dr_qty - cr_qty) from transaction_bom_helper where ";
        qStr += " voucherType = 'Stock' and VoucherDate <= " + db->getDateString(dateTo) + " and VoucherDate >= " + db->getDateString(fromDate);
        qStr += " group by itemID ";
        qStr += " ) stock_adj on A.item_id = stock_adj.inventory_item_id ";

    }
    qStr += " where inv.isStockItem = 1 ";
    qStr += " group by inv.item_id  ";
    if(!show0s){
        if(showTransWithinPeriod)
            qStr += " having sum(Inward) <> 0 or sum(Issue) <> 0 or sum(Opening)+sum(Inward)-sum(Issue) <> 0 ";
        else
            qStr += " having sum(Opening)+sum(Inward)-sum(Issue) <> 0 ";
    }
    qStr += " order by 3 ";


    qDebug()<<Q_FUNC_INFO<<qStr;
    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}

QSqlQueryModel *ClosingStockHelper::getItemwiseClosingStockOfGrpByGodownAndStdCost(QDate fromDate, QDate dateTo, QString godownID, QString GroupId, bool show0s, bool bomFlag)
{
    QSqlQueryModel* model = new QSqlQueryModel;
    SalesInventoryItemDatabaseHelper items;
    ItemTransactionDatabaseHelper itemHelper;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "";
    qStr += " select 'I' as Ind, A.item_id as Id,  Item_Name as Item,  ";
    qStr += " cast(sum(Opening) as decimal(10,3)) as Opening,   ";
    qStr += " cast(sum(Inward) as decimal(10,3)) as Inward,   ";
    qStr += " cast(sum(Issue) as decimal(10,3)) as Issued ,   ";
    qStr += " cast(sum(Opening)+sum(Inward)-sum(Issue) as decimal(10,3)) AS 'Closing Stock',  ";
    qStr += " std_cost *(sum(Opening)+sum(Inward)-sum(Issue))   as 'Stock Value' from   ";
    qStr += " (   ";
    qStr += " select trans.VoucherDate vDate, ItemID as item_id, ";
    qStr += " (case when VoucherDate < " + db->getDateString(fromDate);
    qStr += " or VoucherDate is null then (dr_qty) - (cr_qty)  else 0 end) as Opening, ";
    qStr += " (case when VoucherDate >= " + db->getDateString(fromDate);
    qStr += " and VoucherDate <= " + db->getDateString(dateTo) + " then (dr_qty)  else 0 end )as Inward , ";
    qStr += " (case when VoucherDate >= "+ db->getDateString(fromDate);
    qStr += " and VoucherDate <= " + db->getDateString(fromDate) + " then (cr_qty) else 0 end )as Issue,  ";
    qStr += " trans.Dr_Amount , trans.Cr_Amount ";
    qStr += " from Transaction_Item_Helper trans where     ";
    qStr += " VoucherDate <= " + db->getDateString(dateTo) + " and ";
    qStr += " ((";
    if(godownID != "%")
        qStr += " From_Location = '" + godownID + "' AND ";
    qStr += " To_Location  = 'OUT'";
    qStr += " ) or (";
    if(godownID != "%")
        qStr += " To_Location = '" + godownID + "' AND ";
    qStr += " From_Location  = 'OUT' ";
    qStr += " ))  ";
    if(bomFlag)
    {
        qStr += " UNION ALL ";
        qStr += " select trans.VoucherDate vDate, ItemID as item_id, ";
        qStr += " (case when VoucherDate < " + db->getDateString(fromDate);
        qStr += " or VoucherDate is null then (dr_qty) - (cr_qty)  else 0 end) as Opening, ";
        qStr += " (case when VoucherDate >= " + db->getDateString(fromDate);
        qStr += " and VoucherDate <= " + db->getDateString(dateTo) + " then (dr_qty)  else 0 end )as Inward , ";
        qStr += " (case when VoucherDate >= "+ db->getDateString(fromDate);
        qStr += " and VoucherDate <= " + db->getDateString(fromDate) + " then (cr_qty) else 0 end )as Issue,  ";
        qStr += " trans.Dr_Amount , trans.Cr_Amount ";
        qStr += " from Transaction_BOM_Helper trans where     ";
        qStr += " VoucherDate <= " + db->getDateString(fromDate) + " and ";
        qStr += " ((";
        if(godownID != "%")
            qStr += " From_Location = '" + godownID + "' AND ";
        qStr += " To_Location  = 'OUT'";
        qStr += " ) or (";
        if(godownID != "%")
            qStr += " To_Location = '" + godownID + "' AND ";
        qStr += " From_Location  = 'OUT' ";
        qStr += " ))  ";
    }
    qStr += " ) A ";
    qStr += " right outer join ";
    if(GroupId != "0"){
        qStr += "(select Item.item_id, Item.isStockItem, Item.Item_Name, ";
        qStr += " Item.Std_Cost from  " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name + " item inner join  ";
        qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as ";
        qStr += " (  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id  ";
        qStr += " FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name + " item left outer join ";
        qStr +=  SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName + " grp  ";
        qStr += " on item.group_id = grp.group_id  ";
        qStr += " union all   select     p.group_id, p.group_name,p.parent_id, item_id  from ";
        qStr +=  SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName + " p   inner join cte ";
        qStr += " on p.group_id = cte.parent_id ) ";
        qStr += " select cte.group_name, cte.item_id from cte where cte.parent_id = '" + GroupId + "' ) itemGrp1  ";
        qStr += " on itemGrp1.item_id = item.item_id  union  ";
        qStr += " select Item2.Item_id, Item2.isStockItem, Item2.Item_Name, Item2.Std_Cost from ";
        qStr +=  SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name + " Item2  ";
        qStr += " where Item2.group_id = '" + GroupId + "' ) inv ";
    }
    else {
        qStr += SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name + " inv ";
    }
    qStr += " on A.item_id = inv.item_id ";
    qStr += " where inv.isStockItem = 1 ";
    qStr += " group by inv.item_id  ";
    if(!show0s){
        qStr += " having sum(Inward) <> 0 or sum(Issue) <> 0 or sum(Opening)+sum(Inward)-sum(Issue) <> 0 ";
    }
    qStr += " order by 3 ";

    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}

QSqlQueryModel *ClosingStockHelper::getItemwiseClosingStockOfGrpByGodownAndFifoPrice(QDate fromDate, QDate dateTo, QString godownID, QString GroupId, bool show0s, bool bomFlag)
{

    QSqlQueryModel* model = new QSqlQueryModel;
    SalesInventoryItemDatabaseHelper items;
    ItemTransactionDatabaseHelper itemHelper;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "";

    qStr = "select 'I' as Ind, item_id as Id, ";
    qStr += " (select distinct " + items.Sales_Inventory_ItemName + " from " + items.Sales_Inventory_Table_Name;
    qStr += " where " + items.Sales_Inventory_ItemId + " = B." + " item_ID) as Item, ";
    qStr += " cast(sum(Opening) as decimal(10,3)) as Opening,  ";
    qStr += " cast(sum(Inward) as decimal(10,3)) as Inward,  ";
    qStr += " cast(sum(Issue) as decimal(10,3)) as Issued ,  ";
    qStr += " cast(sum(Opening)+sum(Inward)-sum(Issue) as decimal(10,3)) AS 'Closing Stock',  ";
    qStr += " round(sum(coalesce(dr_amount,0) - coalesce(cr_amount,0)) ,2 ) as 'Stock Value'";
    qStr += " from  ";
    qStr += " ( select  item_id, ";
    qStr += " (case when vdate >= " + db->getDateString(fromDate) + " and vdate <= " + db->getDateString(dateTo) + " then (creditQty) else 0 end )as Issue, ";
    qStr += " (case when vdate >= " + db->getDateString(fromDate) + " and vdate <= " + db->getDateString(dateTo) + " then (debitQty)  else 0 end )as Inward, ";
    qStr += " (case when vdate < " + db->getDateString(fromDate) + "  or vdate is null then (debitQty) - (creditQty)  else 0 end) as Opening, ";
    qStr += " cr_amount, dr_amount from  ";
    qStr += " ( ";

    qStr += " select trans.VoucherDate vDate, ItemID as item_id,  0 debitQty , (trans.Cr_Qty) creditQty,  trans.Dr_Amount , trans.Cr_Amount, 'FROM' as type  ";
    qStr += " from Transaction_Item_Helper trans where  From_Location like '" + godownID + "'  and VoucherDate <= " + db->getDateString(dateTo);
    qStr += " and (select isStockItem FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where itemID = item_ID) = 1 ";
    qStr += " UNION ALL   ";
    qStr += " select trans.VoucherDate, ItemID,  (trans.Dr_Qty) , 0,  trans.Dr_Amount , trans.Cr_Amount, 'TO' as type  ";
    qStr += " from Transaction_Item_Helper trans where  To_Location like '" + godownID + "'  and VoucherDate <= " + db->getDateString(dateTo);
    qStr += " and (select isStockItem FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where itemID = item_ID) = 1 ";

    if(bomFlag){
        qStr += " UNION ALL";
        qStr += " select trans.VoucherDate, ItemID as item_id,  0 , (trans.Cr_Qty),  trans.Dr_Amount , trans.Cr_Amount, 'FROM' as type  ";
        qStr += " from Transaction_Bom_Helper trans where  From_Location like '" + godownID + "'  and VoucherDate <= " + db->getDateString(dateTo);
        qStr += " and (select isStockItem FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where item_ID = itemID) = 1 ";
        qStr += " UNION ALL   ";
        qStr += " select trans.VoucherDate, ItemID,  (trans.Dr_Qty) , 0,  trans.Dr_Amount , trans.Cr_Amount, 'TO' as type  ";
        qStr += " from Transaction_Bom_Helper trans where  To_Location like '" + godownID + "'  and VoucherDate <= " + db->getDateString(dateTo);
        qStr += " and (select isStockItem FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where item_ID = itemID) = 1 ";

    }
    qStr += " ) A where item_id is not null ) B ";
    qStr += " where item_id in ";
    qStr += " (select Item.item_id from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
    qStr += " inner join ";
    qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
    qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
    qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  grp";
    qStr += "  on item.group_id = grp.group_id";
    qStr += "  union all ";
    qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
    qStr += "  from       " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName + " p ";
    qStr += "  inner join cte ";
    qStr += "  on p.group_id = cte.parent_id )";
    qStr += " select cte.group_name, cte.item_id";
    qStr += " from cte where cte.parent_id = '"+ GroupId +"'";
    qStr += " ) itemGrp1 ";
    qStr += " on itemGrp1.item_id = item.item_id ";
    qStr += " union ";
    qStr += " select Item2.Item_id from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
    qStr += " where Item2.group_id = '"+ GroupId +"'";
    qStr += " ) ";
    qStr += " group by item_id "  ;
    if(!show0s)
        qStr += " having sum(Opening)+sum(Inward)-sum(Issue) <> 0 ";
    qStr += " order by 1";



    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}

QSqlQueryModel *ClosingStockHelper::getGodownwiseStockOfItem(QString itemId, QDate toDate)
{
    qDebug()<<Q_FUNC_INFO<<GodownDatabaseHelper::Godown_Tablename;

    QString qStr = "select Godown_ID, Godown_Name, coalesce(sum(qty),0) FROM ";
    qStr += " (select godown_id, godown_name, isDefault,  qty,";
    qStr += " loc, itemID";
    qStr += " from " +GodownDatabaseHelper::Godown_Tablename +" left outer join ( ";

    qStr += "  select itemID, -1*cr_qty as qty, From_Location as loc from transaction_item_helper where itemID = '" + itemId + "' ";
    qStr += " and voucherDate <= " + db->getDateString(toDate);
    qStr += " UNION ALL ";
    qStr += "  select itemID, dr_qty as qty, to_Location as loc from transaction_item_helper where itemID = '" + itemId + "' ";
    qStr += " and voucherDate <= " + db->getDateString(toDate);

    if(LoginValues::company.useBomForStockCalc)
    {
        qStr += " UNION ALL ";
        qStr += "  select itemID, -1*cr_qty as qty, From_Location as loc from transaction_bom_helper where itemID = '" + itemId + "' ";
        qStr += " and voucherDate <= " + db->getDateString(toDate);
        qStr += " UNION ALL ";
        qStr += "  select itemID, dr_qty as qty, to_Location as loc from transaction_bom_helper where itemID = '" + itemId + "' ";
        qStr += " and voucherDate <= " + db->getDateString(toDate);

    }
    qStr += ") trans" ;
    qStr += " on (godown_id = loc) ";
    //    qStr += " and trans.itemID = '" + itemId + "' ";
    //    qStr += " and trans.voucherDate <= " + db->getDateString(toDate);
    qStr += "  ) AllTrans ";
    qStr += "  Group By Godown_ID ";
    qStr += "  Order By isDefault desc, 1";


    return DatabaseHelper::getQueryModel(qStr);

}

QSqlQueryModel *ClosingStockHelper::getClsStkByGroupWithFifoPriceAndBomSummary(QString GroupId, QDate fromDate, QDate dateTo, bool bomFlag)
{
    // Stock value by fifo Including bom
    QSqlQueryModel* model = new QSqlQueryModel;
    SalesInventoryItemDatabaseHelper items;
    ItemTransactionDatabaseHelper itemHelper;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "";

    qStr = "select ItemGrp.Ind, ItemGrp.Id, ";
    qStr += " (select distinct " + items.Sales_Inventory_ItemName + " from " + items.Sales_Inventory_Table_Name;
    qStr += " where " + items.Sales_Inventory_ItemId + " = B." + " item_ID) as Item, ";
    qStr += " cast(sum(Opening) as decimal(10,3)) as Opening,  ";
    qStr += " cast(sum(Inward) as decimal(10,3)) as Inward,  ";
    qStr += " cast(sum(Issue) as decimal(10,3)) as Issued ,  ";
    qStr += " cast(sum(Opening)+sum(Inward)-sum(Issue) as decimal(10,3)) AS 'Closing Stock',  ";
    qStr += " round(sum(coalesce(dr_amount,0) - coalesce(cr_amount,0)) ,2 ) as 'Stock Value'";
    qStr += " from  ";
    qStr += " ( select  item_id, ";
    qStr += " (case when vdate >= " + db->getDateString(fromDate) + " and vdate <= " + db->getDateString(dateTo) + " then (creditQty) else 0 end )as Issue, ";
    qStr += " (case when vdate >= " + db->getDateString(fromDate) + " and vdate <= " + db->getDateString(dateTo) + " then (debitQty)  else 0 end )as Inward, ";
    qStr += " (case when vdate < " + db->getDateString(fromDate) + "  or vdate is null then (debitQty) - (creditQty)  else 0 end) as Opening, ";
    qStr += " cr_amount, dr_amount from  ";
    qStr += " ( ";
    //    qStr += " select str_to_date('1900-01-01','%Y-%m-%d') as vdate, ";
    //    qStr += items.Sales_Inventory_ItemId + ", 0 as creditQty, coalesce(" + items.Sales_Inventory_OpeningStock + ",0) as debitQty, ";
    //    qStr += " 0 as cr_amount, " + items.Sales_Inventory_OpeningValue + " as dr_amount, ";
    //    qStr += " 'opening' as type from " + items.Sales_Inventory_Table_Name + " where " + items.Sales_Inventory_isPurchaseItem + " = 1 ";
    //    qStr += " UNION ALL ";
    qStr += " select trans." + itemHelper.Transaction_Item_Helper_VoucherDate + ", " +  itemHelper.Transaction_Item_Helper_ItemID + ", ";
    qStr += " (trans." + itemHelper.Transaction_Item_Helper_Cr_Qty + ") creditQty,(trans." + itemHelper.Transaction_Item_Helper_Dr_Qty + ") debitQty, ";
    qStr += "trans." + itemHelper.Transaction_Item_Helper_Cr_Qty + " * trans." + itemHelper.Transaction_Item_Helper_Price + " as cr_amount ";
    qStr += "trans." + itemHelper.Transaction_Item_Helper_Dr_Qty + " * trans." + itemHelper.Transaction_Item_Helper_Price + " as Dr_amount ";
    //    qStr += "trans." + itemHelper.Transaction_Item_Helper_Cr_Amount + ", trans." + itemHelper.Transaction_Item_Helper_Dr_Amount ;
    qStr += ", 'IN' as type ";
    qStr += " from " + itemHelper.Transaction_Item_Helper_TableName + " trans where ";
    qStr += " (select " + items.Sales_Inventory_isPurchaseItem + " from " + items.Sales_Inventory_Table_Name ;
    qStr += " where " + items.Sales_Inventory_ItemId + " = trans." + itemHelper.Transaction_Item_Helper_ItemID + ") = 1 ";
    qStr += " and " + itemHelper.Transaction_Item_Helper_VoucherDate + " <= str_to_date('" +dateTo.toString("yyyy-MM-dd")+ "','%Y-%m-%d')";

    if(bomFlag){
        qStr += " UNION ALL ";
        qStr += " select trans." + itemHelper.Transaction_Item_Helper_VoucherDate + ", " +  itemHelper.Transaction_Item_Helper_ItemID + ", ";
        qStr += " (trans." + itemHelper.Transaction_Item_Helper_Cr_Qty + "), (trans." + itemHelper.Transaction_Item_Helper_Dr_Qty + "), ";
        qStr += "trans." + itemHelper.Transaction_Item_Helper_Cr_Qty + " * trans." + itemHelper.Transaction_Item_Helper_Price + " as cr_amount ";
        qStr += "trans." + itemHelper.Transaction_Item_Helper_Dr_Qty + " * trans." + itemHelper.Transaction_Item_Helper_Price + " as Dr_amount ";
        //    qStr += "trans." + itemHelper.Transaction_Item_Helper_Cr_Amount + ", trans." + itemHelper.Transaction_Item_Helper_Dr_Amount ;
        qStr += ", 'IN' as type ";
        qStr += " from " + itemHelper.Transaction_Bom_Helper_TableName + " trans where ";
        qStr +=  itemHelper.Transaction_Item_Helper_VoucherDate + " <= str_to_date('" +dateTo.toString("yyyy-MM-dd")+ "','%Y-%m-%d')";
    }
    qStr += " ) A where item_id is not null ) B ";
    qStr += " where item_id in ";
    qStr += " (select Item.item_id from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
    qStr += " inner join ";
    qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
    qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
    qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  grp";
    qStr += "  on item.group_id = grp.group_id";
    qStr += "  union all ";
    qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
    qStr += "  from       " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName + " p ";
    qStr += "  inner join cte ";
    qStr += "  on p.group_id = cte.parent_id )";
    qStr += " select cte.group_name, cte.item_id, cte.group_id ";
    qStr += " from cte where cte.parent_id = '"+ GroupId +"'";
    qStr += " ) itemGrp1 ";
    qStr += " on itemGrp1.item_id = item.item_id ";
    qStr += " union ";
    qStr += " select Item2.Item_id from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
    qStr += " where Item2.group_id = '"+ GroupId +"'";
    qStr += " ) ItemGrp ";
    qStr += " on B.Item_Id = ItemGrp.Item_Id  ";
    qStr += " group by Name "  ;
    qStr += " order by 1";



    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}

QSqlQueryModel *ClosingStockHelper::getClsStkByGroupWithFifoPriceAndBomDetailed(QString GroupId, QDate fromDate, QDate dateTo, bool bomFlag)
{
    QSqlQueryModel* model = new QSqlQueryModel;
    SalesInventoryItemDatabaseHelper items;
    ItemTransactionDatabaseHelper itemHelper;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "";

    qStr = "select 'I' as Ind, item_id as Id, ";
    qStr += " (select distinct " + items.Sales_Inventory_ItemName + " from " + items.Sales_Inventory_Table_Name;
    qStr += " where " + items.Sales_Inventory_ItemId + " = B." + " item_ID) as Item, ";
    qStr += " cast(sum(Opening) as decimal(10,3)) as Opening,  ";
    qStr += " cast(sum(Inward) as decimal(10,3)) as Inward,  ";
    qStr += " cast(sum(Issue) as decimal(10,3)) as Issued ,  ";
    qStr += " cast(sum(Opening)+sum(Inward)-sum(Issue) as decimal(10,3)) AS 'Closing Stock',  ";
    qStr += " round(sum(coalesce(dr_amount,0) - coalesce(cr_amount,0)) ,2 ) as 'Stock Value'";
    qStr += " from  ";
    qStr += " ( select  item_id, ";
    qStr += " (case when vdate >= " + db->getDateString(fromDate) + " and vdate <= " + db->getDateString(dateTo) + " then (creditQty) else 0 end )as Issue, ";
    qStr += " (case when vdate >= " + db->getDateString(fromDate) + " and vdate <= " + db->getDateString(dateTo) + " then (debitQty)  else 0 end )as Inward, ";
    qStr += " (case when vdate < " + db->getDateString(fromDate) + "  or vdate is null then (debitQty) - (creditQty)  else 0 end) as Opening, ";
    qStr += " cr_amount, dr_amount from  ";
    qStr += " ( ";
    //    qStr += " select str_to_date('1900-01-01','%Y-%m-%d') as vdate, ";
    //    qStr += items.Sales_Inventory_ItemId + ", 0 as creditQty, coalesce(" + items.Sales_Inventory_OpeningStock + ",0) as debitQty, ";
    //    qStr += " 0 as cr_amount, " + items.Sales_Inventory_OpeningValue + " as dr_amount, ";
    //    qStr += " 'opening' as type from " + items.Sales_Inventory_Table_Name + " where " + items.Sales_Inventory_isPurchaseItem + " = 1 ";
    //    qStr += " UNION ALL ";
    qStr += " select trans." + itemHelper.Transaction_Item_Helper_VoucherDate + ", " +  itemHelper.Transaction_Item_Helper_ItemID + ", ";
    qStr += " (trans." + itemHelper.Transaction_Item_Helper_Cr_Qty + ") creditQty,(trans." + itemHelper.Transaction_Item_Helper_Dr_Qty + ") debitQty, ";
    qStr += "trans." + itemHelper.Transaction_Item_Helper_Cr_Qty + " * trans." + itemHelper.Transaction_Item_Helper_Price + " as cr_amount ";
    qStr += "trans." + itemHelper.Transaction_Item_Helper_Dr_Qty + " * trans." + itemHelper.Transaction_Item_Helper_Price + " as Dr_amount ";
    //    qStr += "trans." + itemHelper.Transaction_Item_Helper_Cr_Amount + ", trans." + itemHelper.Transaction_Item_Helper_Dr_Amount ;
    qStr += ", 'IN' as type ";
    qStr += " from " + itemHelper.Transaction_Item_Helper_TableName + " trans where ";
    qStr += " (select " + items.Sales_Inventory_isPurchaseItem + " from " + items.Sales_Inventory_Table_Name ;
    qStr += " where " + items.Sales_Inventory_ItemId + " = trans." + itemHelper.Transaction_Item_Helper_ItemID + ") = 1 ";
    qStr += " and " + itemHelper.Transaction_Item_Helper_VoucherDate + " <= str_to_date('" +dateTo.toString("yyyy-MM-dd")+ "','%Y-%m-%d')";

    if(bomFlag){
        qStr += " UNION ALL ";
        qStr += " select trans." + itemHelper.Transaction_Item_Helper_VoucherDate + ", " +  itemHelper.Transaction_Item_Helper_ItemID + ", ";
        qStr += " (trans." + itemHelper.Transaction_Item_Helper_Cr_Qty + "), (trans." + itemHelper.Transaction_Item_Helper_Dr_Qty + "), ";
        qStr += "trans." + itemHelper.Transaction_Item_Helper_Cr_Qty + " * trans." + itemHelper.Transaction_Item_Helper_Price + " as cr_amount ";
        qStr += "trans." + itemHelper.Transaction_Item_Helper_Dr_Qty + " * trans." + itemHelper.Transaction_Item_Helper_Price + " as Dr_amount ";
        //    qStr += "trans." + itemHelper.Transaction_Item_Helper_Cr_Amount + ", trans." + itemHelper.Transaction_Item_Helper_Dr_Amount ;
        qStr += ", 'IN' as type ";
        qStr += " from " + itemHelper.Transaction_Bom_Helper_TableName + " trans where ";
        qStr +=  itemHelper.Transaction_Item_Helper_VoucherDate + " <= str_to_date('" +dateTo.toString("yyyy-MM-dd")+ "','%Y-%m-%d')";
    }
    qStr += " ) A where item_id is not null ) B ";
    qStr += " where item_id in ";
    qStr += " (select Item.item_id from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
    qStr += " inner join ";
    qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
    qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
    qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  grp";
    qStr += "  on item.group_id = grp.group_id";
    qStr += "  union all ";
    qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
    qStr += "  from       " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName + " p ";
    qStr += "  inner join cte ";
    qStr += "  on p.group_id = cte.parent_id )";
    qStr += " select cte.group_name, cte.item_id";
    qStr += " from cte where cte.parent_id = '"+ GroupId +"'";
    qStr += " ) itemGrp1 ";
    qStr += " on itemGrp1.item_id = item.item_id ";
    qStr += " union ";
    qStr += " select Item2.Item_id from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
    qStr += " where Item2.group_id = '"+ GroupId +"'";
    qStr += " ) ";
    qStr += " group by item_id "  ;
    qStr += " order by 1";



    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}


QSqlQueryModel *ClosingStockHelper::getClosingStockListModelWithLastPurchasePrice(QDate fromDate, QDate dateTo, bool bomFlag)
{
    QSqlQueryModel* model = new QSqlQueryModel;
    SalesInventoryItemDatabaseHelper items;
    ItemTransactionDatabaseHelper itemHelper;
    PurchaseVoucherDatabaseHelper pHelper;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "";

    qStr = "select 'I' as Ind, item_id as Id, ";
    qStr += " (select distinct " + items.Sales_Inventory_ItemName + " from " + items.Sales_Inventory_Table_Name;
    qStr += " where " + items.Sales_Inventory_ItemId + " = B." + " item_ID) as Item, ";
    qStr += " cast(sum(Opening) as decimal(10,3)) as Opening,  ";
    qStr += " cast(sum(Inward) as decimal(10,3)) as Inward,  ";
    qStr += " cast(sum(Issue) as decimal(10,3)) as Issued ,  ";
    qStr += " cast(sum(Opening)+sum(Inward)-sum(Issue) as decimal(10,3)) AS 'Closing Stock',  ";
    qStr += "coalesce((SELECT "+ pHelper.Purchase_Invoice_Details_Price+ " from "+ pHelper.Purchase_Invoice_Details_TableName;
    qStr += " where "+pHelper.Purchase_Invoice_Details_Inventory_Item_ID + "= B.item_ID order by ";
    qStr += pHelper.Purchase_Invoice_Details_Voucher_Date + " desc limit 1),0) * ";
    qStr += " cast(sum(Opening)+sum(Inward)-sum(Issue) as decimal(10,3)) as 'Stock Value'";
    qStr += " from  ";
    qStr += " ( select  item_id, ";
    qStr += " (case when vdate >= " + db->getDateString(fromDate) + " and vdate <= str_to_date('" + dateTo.toString("yyyy-MM-dd") + "','%Y-%m-%d') then (creditQty) else 0 end )as Issue, ";
    qStr += " (case when vdate >= str_to_date('" + fromDate.toString("yyyy-MM-dd") + "','%Y-%m-%d') and vdate <= str_to_date('" + dateTo.toString("yyyy-MM-dd") + "','%Y-%m-%d') then (debitQty)  else 0 end )as Inward, ";
    qStr += " (case when vdate <  str_to_date('" + fromDate.toString("yyyy-MM-dd") + "','%Y-%m-%d')  or vdate is null then (debitQty) - (creditQty)  else 0 end) as Opening, ";
    qStr += " cr_amount, dr_amount from  ";
    qStr += " ( ";
    //    qStr += " select str_to_date('1900-01-01','%Y-%m-%d') as vdate, ";
    //    qStr += items.Sales_Inventory_ItemId + ", 0 as creditQty, coalesce(" + items.Sales_Inventory_OpeningStock + ",0) as debitQty, ";
    //    qStr += " 0 as cr_amount, " + items.Sales_Inventory_OpeningValue + " as dr_amount, ";
    //    qStr += " 'opening' as type from " + items.Sales_Inventory_Table_Name + " where " + items.Sales_Inventory_isPurchaseItem + " = 1 ";
    //    qStr += " UNION ALL ";
    qStr += " select trans." + itemHelper.Transaction_Item_Helper_VoucherDate + ", " +  itemHelper.Transaction_Item_Helper_ItemID + ", ";
    qStr += " (trans." + itemHelper.Transaction_Item_Helper_Cr_Qty + ") creditQty,(trans." + itemHelper.Transaction_Item_Helper_Dr_Qty + ") debitQty, ";
    qStr += "trans." + itemHelper.Transaction_Item_Helper_Cr_Amount + ", trans." + itemHelper.Transaction_Item_Helper_Dr_Amount + ", 'IN' as type ";
    qStr += " from " + itemHelper.Transaction_Item_Helper_TableName + " trans where ";
    qStr += " (select " + items.Sales_Inventory_isPurchaseItem + " from " + items.Sales_Inventory_Table_Name ;
    qStr += " where " + items.Sales_Inventory_ItemId + " = trans." + itemHelper.Transaction_Item_Helper_ItemID + ") = 1 ";
    qStr += " and " + itemHelper.Transaction_Item_Helper_VoucherDate + " <= str_to_date('" +dateTo.toString("yyyy-MM-dd")+ "','%Y-%m-%d')";
    if(bomFlag){
        qStr += " UNION ALL ";
        qStr += " select trans." + itemHelper.Transaction_Item_Helper_VoucherDate + ", " +  itemHelper.Transaction_Item_Helper_ItemID + ", ";
        qStr += " (trans." + itemHelper.Transaction_Item_Helper_Cr_Qty + "), (trans." + itemHelper.Transaction_Item_Helper_Dr_Qty + "), ";
        qStr += "trans." + itemHelper.Transaction_Item_Helper_Cr_Amount + ", trans." + itemHelper.Transaction_Item_Helper_Dr_Amount + ", 'IN' as type ";
        qStr += " from " + itemHelper.Transaction_Bom_Helper_TableName + " trans where ";
        qStr +=  itemHelper.Transaction_Item_Helper_VoucherDate + " <= str_to_date('" +dateTo.toString("yyyy-MM-dd")+ "','%Y-%m-%d')";
    }
    qStr += " ) A where item_id is not null ) B ";
    qStr += " group by item_id "  ;
    qStr += " order by 1";



    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}

QSqlQueryModel *ClosingStockHelper::getClsStkByGroupWithLastPurchPriceAndBomDetailed(QString GroupId, QDate fromDate, QDate dateTo, bool bomFlag)
{
    QSqlQueryModel* model = new QSqlQueryModel;
    SalesInventoryItemDatabaseHelper items;
    ItemTransactionDatabaseHelper itemHelper;
    PurchaseVoucherDatabaseHelper pHelper;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "";

    qStr = "select 'I' as Ind, item_id as Id, ";
    qStr += " (select distinct " + items.Sales_Inventory_ItemName + " from " + items.Sales_Inventory_Table_Name;
    qStr += " where " + items.Sales_Inventory_ItemId + " = B." + " item_ID) as Item, ";
    qStr += " cast(sum(Opening) as decimal(10,3)) as Opening,  ";
    qStr += " cast(sum(Inward) as decimal(10,3)) as Inward,  ";
    qStr += " cast(sum(Issue) as decimal(10,3)) as Issued ,  ";
    qStr += " cast(sum(Opening)+sum(Inward)-sum(Issue) as decimal(10,3)) AS 'Closing Stock',  ";
    qStr += "coalesce((SELECT "+ pHelper.Purchase_Invoice_Details_Price+ " from "+ pHelper.Purchase_Invoice_Details_TableName;
    qStr += " where "+pHelper.Purchase_Invoice_Details_Inventory_Item_ID + "= B.item_ID order by ";
    qStr += pHelper.Purchase_Invoice_Details_Voucher_Date + " desc limit 1),0) * ";
    qStr += " cast(sum(Opening)+sum(Inward)-sum(Issue) as decimal(10,3)) as 'Stock Value'";
    qStr += " from  ";
    qStr += " ( select  item_id, ";
    qStr += " (case when vdate >= " + db->getDateString(fromDate) + " and vdate <= str_to_date('" + dateTo.toString("yyyy-MM-dd") + "','%Y-%m-%d') then (creditQty) else 0 end )as Issue, ";
    qStr += " (case when vdate >= str_to_date('" + fromDate.toString("yyyy-MM-dd") + "','%Y-%m-%d') and vdate <= str_to_date('" + dateTo.toString("yyyy-MM-dd") + "','%Y-%m-%d') then (debitQty)  else 0 end )as Inward, ";
    qStr += " (case when vdate <  str_to_date('" + fromDate.toString("yyyy-MM-dd") + "','%Y-%m-%d')  or vdate is null then (debitQty) - (creditQty)  else 0 end) as Opening, ";
    qStr += " cr_amount, dr_amount from  ";
    qStr += " ( ";
    //    qStr += " select str_to_date('1900-01-01','%Y-%m-%d') as vdate, ";
    //    qStr += items.Sales_Inventory_ItemId + ", 0 as creditQty, coalesce(" + items.Sales_Inventory_OpeningStock + ",0) as debitQty, ";
    //    qStr += " 0 as cr_amount, " + items.Sales_Inventory_OpeningValue + " as dr_amount, ";
    //    qStr += " 'opening' as type from " + items.Sales_Inventory_Table_Name + " where " + items.Sales_Inventory_isPurchaseItem + " = 1 ";
    //    qStr += " UNION ALL ";
    qStr += " select trans." + itemHelper.Transaction_Item_Helper_VoucherDate + ", " +  itemHelper.Transaction_Item_Helper_ItemID + ", ";
    qStr += " (trans." + itemHelper.Transaction_Item_Helper_Cr_Qty + ") creditQty,(trans." + itemHelper.Transaction_Item_Helper_Dr_Qty + ") debitQty, ";
    qStr += "trans." + itemHelper.Transaction_Item_Helper_Cr_Amount + ", trans." + itemHelper.Transaction_Item_Helper_Dr_Amount + ", 'IN' as type ";
    qStr += " from " + itemHelper.Transaction_Item_Helper_TableName + " trans where ";
    qStr += " (select " + items.Sales_Inventory_isPurchaseItem + " from " + items.Sales_Inventory_Table_Name ;
    qStr += " where " + items.Sales_Inventory_ItemId + " = trans." + itemHelper.Transaction_Item_Helper_ItemID + ") = 1 ";
    qStr += " and " + itemHelper.Transaction_Item_Helper_VoucherDate + " <= str_to_date('" +dateTo.toString("yyyy-MM-dd")+ "','%Y-%m-%d')";
    if(bomFlag){
        qStr += " UNION ALL ";
        qStr += " select trans." + itemHelper.Transaction_Item_Helper_VoucherDate + ", " +  itemHelper.Transaction_Item_Helper_ItemID + ", ";
        qStr += " (trans." + itemHelper.Transaction_Item_Helper_Cr_Qty + "), (trans." + itemHelper.Transaction_Item_Helper_Dr_Qty + "), ";
        qStr += "trans." + itemHelper.Transaction_Item_Helper_Cr_Amount + ", trans." + itemHelper.Transaction_Item_Helper_Dr_Amount + ", 'IN' as type ";
        qStr += " from " + itemHelper.Transaction_Bom_Helper_TableName + " trans where ";
        qStr +=  itemHelper.Transaction_Item_Helper_VoucherDate + " <= str_to_date('" +dateTo.toString("yyyy-MM-dd")+ "','%Y-%m-%d')";
    }
    qStr += " ) A where item_id is not null ) B ";
    qStr += " where item_id in ";
    qStr += " (select Item.item_id from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
    qStr += " inner join ";
    qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
    qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
    qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  grp";
    qStr += "  on item.group_id = grp.group_id";
    qStr += "  union all ";
    qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
    qStr += "  from       " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName + " p ";
    qStr += "  inner join cte ";
    qStr += "  on p.group_id = cte.parent_id )";
    qStr += " select cte.group_name, cte.item_id";
    qStr += " from cte where cte.parent_id = '"+ GroupId +"'";
    qStr += " ) itemGrp1 ";
    qStr += " on itemGrp1.item_id = item.item_id ";
    qStr += " union ";
    qStr += " select Item2.Item_id from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
    qStr += " where Item2.group_id = '"+ GroupId +"'";
    qStr += " ) ";
    qStr += " group by item_id "  ;
    qStr += " order by 1";



    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}

QSqlQueryModel *ClosingStockHelper::getClsStkByGroupWithLastPurchPriceAndBomSummary(QString GroupId, QDate fromDate, QDate dateTo, bool bomFlag)
{
    QSqlQueryModel* model = new QSqlQueryModel;
    SalesInventoryItemDatabaseHelper items;
    ItemTransactionDatabaseHelper itemHelper;
    PurchaseVoucherDatabaseHelper pHelper;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "";

    qStr = "select ItemGrp.Ind, ItemGrp.Id, Name,   ";
    //    qStr += " (select distinct " + items.Sales_Inventory_ItemName + " from " + items.Sales_Inventory_Table_Name;
    //    qStr += " where " + items.Sales_Inventory_ItemId + " = B." + " item_ID) as Item, ";
    qStr += " cast(sum(Opening) as decimal(10,3)) as Opening,  ";
    qStr += " cast(sum(Inward) as decimal(10,3)) as Inward,  ";
    qStr += " cast(sum(Issue) as decimal(10,3)) as Issued ,  ";
    qStr += " cast(sum(Opening)+sum(Inward)-sum(Issue) as decimal(10,3)) AS 'Closing Stock',  ";
    //    qStr += "coalesce((SELECT "+ pHelper.Purchase_Invoice_Details_Price+ " from "+ pHelper.Purchase_Invoice_Details_TableName;
    //    qStr += " where "+pHelper.Purchase_Invoice_Details_Inventory_Item_ID + "= B.item_ID order by ";
    //    qStr += pHelper.Purchase_Invoice_Details_Voucher_Date + " desc limit 1),0) * ";
    qStr += " cast(sum(Price) as decimal(10,3)) as 'Stock Value'";
    qStr += " from  ";
    qStr += " ( select  item_id, ";
    qStr += " (case when vdate >= " + db->getDateString(fromDate) + " and vdate <= str_to_date('" + dateTo.toString("yyyy-MM-dd") + "','%Y-%m-%d') then (creditQty) else 0 end )as Issue, ";
    qStr += " (case when vdate >= str_to_date('" + fromDate.toString("yyyy-MM-dd") + "','%Y-%m-%d') and vdate <= str_to_date('" + dateTo.toString("yyyy-MM-dd") + "','%Y-%m-%d') then (debitQty)  else 0 end )as Inward, ";
    qStr += " (case when vdate <  str_to_date('" + fromDate.toString("yyyy-MM-dd") + "','%Y-%m-%d')  or vdate is null then (debitQty) - (creditQty)  else 0 end) as Opening, ";
    qStr += " cr_amount, dr_amount, ";
    qStr += " coalesce((SELECT "+ pHelper.Purchase_Invoice_Details_Price+ " from "+ pHelper.Purchase_Invoice_Details_TableName;
    qStr += " where "+pHelper.Purchase_Invoice_Details_Inventory_Item_ID + "= item_ID order by ";
    qStr += pHelper.Purchase_Invoice_Details_Voucher_Date + " desc limit 1),0) * (debitQty - creditQty) as Price ";
    qStr += " from  ";
    qStr += " ( ";
    //    qStr += " select str_to_date('1900-01-01','%Y-%m-%d') as vdate, ";
    //    qStr += items.Sales_Inventory_ItemId + ", 0 as creditQty, coalesce(" + items.Sales_Inventory_OpeningStock + ",0) as debitQty, ";
    //    qStr += " 0 as cr_amount, " + items.Sales_Inventory_OpeningValue + " as dr_amount, ";
    //    qStr += " 'opening' as type from " + items.Sales_Inventory_Table_Name + " where " + items.Sales_Inventory_isPurchaseItem + " = 1 ";
    //    qStr += " UNION ALL ";
    qStr += " select trans." + itemHelper.Transaction_Item_Helper_VoucherDate + ", " +  itemHelper.Transaction_Item_Helper_ItemID + ", ";
    qStr += " (trans." + itemHelper.Transaction_Item_Helper_Cr_Qty + ") creditQty,(trans." + itemHelper.Transaction_Item_Helper_Dr_Qty + ") debitQty, ";
    qStr += "trans." + itemHelper.Transaction_Item_Helper_Cr_Amount + ", trans." + itemHelper.Transaction_Item_Helper_Dr_Amount + ", 'IN' as type ";
    qStr += " from " + itemHelper.Transaction_Item_Helper_TableName + " trans where ";
    qStr += " (select " + items.Sales_Inventory_isPurchaseItem + " from " + items.Sales_Inventory_Table_Name ;
    qStr += " where " + items.Sales_Inventory_ItemId + " = trans." + itemHelper.Transaction_Item_Helper_ItemID + ") = 1 ";
    qStr += " and " + itemHelper.Transaction_Item_Helper_VoucherDate + " <= str_to_date('" +dateTo.toString("yyyy-MM-dd")+ "','%Y-%m-%d')";
    if(bomFlag){
        qStr += " UNION ALL ";
        qStr += " select trans." + itemHelper.Transaction_Item_Helper_VoucherDate + ", " +  itemHelper.Transaction_Item_Helper_ItemID + ", ";
        qStr += " (trans." + itemHelper.Transaction_Item_Helper_Cr_Qty + "), (trans." + itemHelper.Transaction_Item_Helper_Dr_Qty + "), ";
        qStr += "trans." + itemHelper.Transaction_Item_Helper_Cr_Amount + ", trans." + itemHelper.Transaction_Item_Helper_Dr_Amount + ", 'IN' as type ";
        qStr += " from " + itemHelper.Transaction_Bom_Helper_TableName + " trans where ";
        qStr +=  itemHelper.Transaction_Item_Helper_VoucherDate + " <= str_to_date('" +dateTo.toString("yyyy-MM-dd")+ "','%Y-%m-%d')";
    }
    qStr += " ) A where item_id is not null ) B ";
    qStr += " inner join ";
    qStr += " (select ItemGrp1.group_name as Name, Item.item_id, 'G' as Ind, ItemGrp1.group_id as Id from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
    qStr += " inner join ";
    qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
    qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
    qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName +"  grp";
    qStr += "  on item.group_id = grp.group_id";
    qStr += "  union all ";
    qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
    qStr += "  from       " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName + " p ";
    qStr += "  inner join cte ";
    qStr += "  on p.group_id = cte.parent_id )";
    qStr += " select cte.group_name, cte.item_id, cte.group_id ";
    qStr += " from cte where cte.parent_id = '"+ GroupId +"'";
    qStr += " ) itemGrp1 ";
    qStr += " on itemGrp1.item_id = item.item_id ";
    qStr += " union ";
    qStr += " select Item2.Item_name as Name, Item2.Item_id, 'I' as ind, Item2.Item_Id as Id from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
    qStr += " where Item2.group_id = '"+ GroupId +"'";
    qStr += " ) ItemGrp";
    qStr += " on B.Item_Id = ItemGrp.Item_Id  ";
    qStr += " group by Name "  ;
    qStr += " order by 1";



    model = DatabaseHelper::getQueryModel(qStr);
    return model;


}

QSqlQueryModel *ClosingStockHelper::getClosingStockListModelWithStdCost(QDate fromDate, QDate dateTo, bool bomFlag)
{
    QSqlQueryModel* model = new QSqlQueryModel;
    SalesInventoryItemDatabaseHelper items;
    ItemTransactionDatabaseHelper itemHelper;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "";

    qStr = "select 'I' Ind, item_id as Id, ";
    qStr += " (select distinct " + items.Sales_Inventory_ItemName + " from " + items.Sales_Inventory_Table_Name;
    qStr += " where " + items.Sales_Inventory_ItemId + " = B." + " item_ID) as Item, ";
    qStr += " cast(sum(Opening) as decimal(10,3)) as Opening,  ";
    qStr += " cast(sum(Inward) as decimal(10,3)) as Inward,  ";
    qStr += " cast(sum(Issue) as decimal(10,3)) as Issued ,  ";
    qStr += " cast(sum(Opening)+sum(Inward)-sum(Issue) as decimal(10,3)) AS 'Closing Stock',  ";
    qStr += "(SELECT distinct "+items.Sales_Inventory_StdCost+ " from "+ items.Sales_Inventory_Table_Name;
    qStr += " where "+items.Sales_Inventory_ItemId + "= B.item_ID )* ";
    qStr += " cast(sum(Opening)+sum(Inward)-sum(Issue) as decimal(10,3)) as 'Stock Value'";
    qStr += " from  ";
    qStr += " ( select  item_id, ";
    qStr += " (case when vdate >= " + db->getDateString(fromDate) + " and vdate <= str_to_date('" + dateTo.toString("yyyy-MM-dd") + "','%Y-%m-%d') then (creditQty) else 0 end )as Issue, ";
    qStr += " (case when vdate >= str_to_date('" + fromDate.toString("yyyy-MM-dd") + "','%Y-%m-%d') and vdate <= str_to_date('" + dateTo.toString("yyyy-MM-dd") + "','%Y-%m-%d') then (debitQty)  else 0 end )as Inward, ";
    qStr += " (case when vdate <  str_to_date('" + fromDate.toString("yyyy-MM-dd") + "','%Y-%m-%d')  or vdate is null then (debitQty) - (creditQty)  else 0 end) as Opening, ";
    qStr += " cr_amount, dr_amount from  ";
    qStr += " ( ";
    //    qStr += " select str_to_date('1900-01-01','%Y-%m-%d') as vdate, ";
    //    qStr += items.Sales_Inventory_ItemId + ", 0 as creditQty, coalesce(" + items.Sales_Inventory_OpeningStock + ",0) as debitQty, ";
    //    qStr += " 0 as cr_amount, " + items.Sales_Inventory_OpeningValue + " as dr_amount, ";
    //    qStr += " 'opening' as type from " + items.Sales_Inventory_Table_Name + " where " + items.Sales_Inventory_isPurchaseItem + " = 1 ";
    //    qStr += " UNION ALL ";
    qStr += " select trans." + itemHelper.Transaction_Item_Helper_VoucherDate + ", " +  itemHelper.Transaction_Item_Helper_ItemID + ", ";
    qStr += " (trans." + itemHelper.Transaction_Item_Helper_Cr_Qty + ") creditQty,(trans." + itemHelper.Transaction_Item_Helper_Dr_Qty + ") debitQty, ";
    qStr += "trans." + itemHelper.Transaction_Item_Helper_Cr_Amount + ", trans." + itemHelper.Transaction_Item_Helper_Dr_Amount + ", 'IN' as type ";
    qStr += " from " + itemHelper.Transaction_Item_Helper_TableName + " trans where ";
    qStr += " (select " + items.Sales_Inventory_isPurchaseItem + " from " + items.Sales_Inventory_Table_Name ;
    qStr += " where " + items.Sales_Inventory_ItemId + " = trans." + itemHelper.Transaction_Item_Helper_ItemID + ") = 1 ";
    qStr += " and " + itemHelper.Transaction_Item_Helper_VoucherDate + " <= str_to_date('" +dateTo.toString("yyyy-MM-dd")+ "','%Y-%m-%d')";

    if(bomFlag){
        qStr += " UNION ALL ";
        qStr += " select trans." + itemHelper.Transaction_Item_Helper_VoucherDate + ", " +  itemHelper.Transaction_Item_Helper_ItemID + ", ";
        qStr += " (trans." + itemHelper.Transaction_Item_Helper_Cr_Qty + "), (trans." + itemHelper.Transaction_Item_Helper_Dr_Qty + "), ";
        qStr += "trans." + itemHelper.Transaction_Item_Helper_Cr_Amount + ", trans." + itemHelper.Transaction_Item_Helper_Dr_Amount + ", 'IN' as type ";
        qStr += " from " + itemHelper.Transaction_Bom_Helper_TableName + " trans where ";
        qStr +=  itemHelper.Transaction_Item_Helper_VoucherDate + " <= str_to_date('" +dateTo.toString("yyyy-MM-dd")+ "','%Y-%m-%d')";
    }
    qStr += " ) A where item_id is not null ) B ";
    qStr += " group by item_id "  ;
    qStr += " order by 1";



    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}

QSqlQueryModel *ClosingStockHelper::getClsStkByGroupWithStdCostAndBomSummary(QString GroupId, QDate fromDate, QDate dateTo, bool bomFlag)
{
    QSqlQueryModel* model = new QSqlQueryModel;
    SalesInventoryItemDatabaseHelper items;
    ItemTransactionDatabaseHelper itemHelper;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "";

    qStr = "select ItemGrp.Ind, ItemGrp.Id, Name, ";
    //    qStr += " (select distinct " + items.Sales_Inventory_ItemName + " from " + items.Sales_Inventory_Table_Name;
    //    qStr += " where " + items.Sales_Inventory_ItemId + " = B." + " item_ID) as Item, ";
    qStr += " cast(sum(Opening) as decimal(10,3)) as Opening,  ";
    qStr += " cast(sum(Inward) as decimal(10,3)) as Inward,  ";
    qStr += " cast(sum(Issue) as decimal(10,3)) as Issued ,  ";
    qStr += " cast(sum(Opening)+sum(Inward)-sum(Issue) as decimal(10,3)) AS 'Closing Stock',  ";
    qStr += " cast(Price) as decimal(10,3)) as 'Stock Value'";
    qStr += " from  ";
    qStr += " ( select  item_id, ";
    qStr += " (case when vdate >= " + db->getDateString(fromDate) + " and vdate <= str_to_date('" + dateTo.toString("yyyy-MM-dd") + "','%Y-%m-%d') then (creditQty) else 0 end )as Issue, ";
    qStr += " (case when vdate >= str_to_date('" + fromDate.toString("yyyy-MM-dd") + "','%Y-%m-%d') and vdate <= str_to_date('" + dateTo.toString("yyyy-MM-dd") + "','%Y-%m-%d') then (debitQty)  else 0 end )as Inward, ";
    qStr += " (case when vdate <  str_to_date('" + fromDate.toString("yyyy-MM-dd") + "','%Y-%m-%d')  or vdate is null then (debitQty) - (creditQty)  else 0 end) as Opening, ";
    qStr += " cr_amount, dr_amount,";
    qStr += "(SELECT distinct "+items.Sales_Inventory_StdCost+ " from "+ items.Sales_Inventory_Table_Name;
    qStr += " where "+items.Sales_Inventory_ItemId + "= item_ID )* (debitQty - creditQty) as Price";
    qStr += " from  ";
    qStr += " ( ";
    //    qStr += " select str_to_date('1900-01-01','%Y-%m-%d') as vdate, ";
    //    qStr += items.Sales_Inventory_ItemId + ", 0 as creditQty, coalesce(" + items.Sales_Inventory_OpeningStock + ",0) as debitQty, ";
    //    qStr += " 0 as cr_amount, " + items.Sales_Inventory_OpeningValue + " as dr_amount, ";
    //    qStr += " 'opening' as type from " + items.Sales_Inventory_Table_Name + " where " + items.Sales_Inventory_isPurchaseItem + " = 1 ";
    //    qStr += " UNION ALL ";
    qStr += " select trans." + itemHelper.Transaction_Item_Helper_VoucherDate + ", " +  itemHelper.Transaction_Item_Helper_ItemID + ", ";
    qStr += " (trans." + itemHelper.Transaction_Item_Helper_Cr_Qty + ")creditQty,(trans." + itemHelper.Transaction_Item_Helper_Dr_Qty + ") debitQty, ";
    qStr += "trans." + itemHelper.Transaction_Item_Helper_Cr_Amount + ", trans." + itemHelper.Transaction_Item_Helper_Dr_Amount + ", 'IN' as type ";
    qStr += " from " + itemHelper.Transaction_Item_Helper_TableName + " trans where ";
    qStr += " (select " + items.Sales_Inventory_isPurchaseItem + " from " + items.Sales_Inventory_Table_Name ;
    qStr += " where " + items.Sales_Inventory_ItemId + " = trans." + itemHelper.Transaction_Item_Helper_ItemID + ") = 1 ";
    qStr += " and " + itemHelper.Transaction_Item_Helper_VoucherDate + " <= str_to_date('" +dateTo.toString("yyyy-MM-dd")+ "','%Y-%m-%d')";

    if(bomFlag){
        qStr += " UNION ALL ";
        qStr += " select trans." + itemHelper.Transaction_Item_Helper_VoucherDate + ", " +  itemHelper.Transaction_Item_Helper_ItemID + ", ";
        qStr += " (trans." + itemHelper.Transaction_Item_Helper_Cr_Qty + "), (trans." + itemHelper.Transaction_Item_Helper_Dr_Qty + "), ";
        qStr += "trans." + itemHelper.Transaction_Item_Helper_Cr_Amount + ", trans." + itemHelper.Transaction_Item_Helper_Dr_Amount + ", 'IN' as type ";
        qStr += " from " + itemHelper.Transaction_Bom_Helper_TableName + " trans where ";
        qStr +=  itemHelper.Transaction_Item_Helper_VoucherDate + " <= str_to_date('" +dateTo.toString("yyyy-MM-dd")+ "','%Y-%m-%d')";
    }
    qStr += " ) A where item_id is not null ) B ";
    qStr += " inner join ";
    qStr += " (select ItemGrp1.group_name as Name, Item.item_id, 'G' as Ind, ItemGrp1.group_id as Id from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
    qStr += " inner join ";
    qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
    qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
    qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName + " grp ";
    qStr += "  on item.group_id = grp.group_id";
    qStr += "  union all ";
    qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
    qStr += "  from       " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName + " p ";
    qStr += "  inner join cte ";
    qStr += "  on p.group_id = cte.parent_id )";
    qStr += " select cte.group_name, cte.item_id, cte.group_id ";
    qStr += " from cte where cte.parent_id = '"+ GroupId +"'";
    qStr += " ) itemGrp1 ";
    qStr += " on itemGrp1.item_id = item.item_id ";
    qStr += " union ";
    qStr += " select Item2.Item_name as Name, Item2.Item_id, 'I' as ind, Item2.Item_Id as Id from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
    qStr += " where Item2.group_id = '"+ GroupId +"'";
    qStr += " ) itemGrp ";
    qStr += " on B.Item_Id = ItemGrp.Item_Id  ";
    qStr += " group by Name "  ;
    qStr += " order by 1";



    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}

QSqlQueryModel *ClosingStockHelper::getClsStkByGroupWithStdCostAndBomDetailed(QString GroupId, QDate fromDate, QDate dateTo, bool bomFlag)
{
    QSqlQueryModel* model = new QSqlQueryModel;
    SalesInventoryItemDatabaseHelper items;
    ItemTransactionDatabaseHelper itemHelper;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "";

    qStr = "select 'I' as Ind, Item_Id as Id,  ";
    qStr += " (select distinct " + items.Sales_Inventory_ItemName + " from " + items.Sales_Inventory_Table_Name;
    qStr += " where " + items.Sales_Inventory_ItemId + " = B." + " item_ID) as Item, ";
    qStr += " cast(sum(Opening) as decimal(10,3)) as Opening,  ";
    qStr += " cast(sum(Inward) as decimal(10,3)) as Inward,  ";
    qStr += " cast(sum(Issue) as decimal(10,3)) as Issued ,  ";
    qStr += " cast(sum(Opening)+sum(Inward)-sum(Issue) as decimal(10,3)) AS 'Closing Stock',  ";
    qStr += "(SELECT distinct "+items.Sales_Inventory_StdCost+ " from "+ items.Sales_Inventory_Table_Name;
    qStr += " where "+items.Sales_Inventory_ItemId + "= B.item_ID )* ";
    qStr += " cast(sum(Opening)+sum(Inward)-sum(Issue) as decimal(10,3)) as 'Stock Value'";
    qStr += " from  ";
    qStr += " ( select  item_id, ";
    qStr += " (case when vdate >= " + db->getDateString(fromDate) + " and vdate <= str_to_date('" + dateTo.toString("yyyy-MM-dd") + "','%Y-%m-%d') then (creditQty) else 0 end )as Issue, ";
    qStr += " (case when vdate >= str_to_date('" + fromDate.toString("yyyy-MM-dd") + "','%Y-%m-%d') and vdate <= str_to_date('" + dateTo.toString("yyyy-MM-dd") + "','%Y-%m-%d') then (debitQty)  else 0 end )as Inward, ";
    qStr += " (case when vdate <  str_to_date('" + fromDate.toString("yyyy-MM-dd") + "','%Y-%m-%d')  or vdate is null then (debitQty) - (creditQty)  else 0 end) as Opening, ";
    qStr += " cr_amount, dr_amount from  ";
    qStr += " ( ";
    //    qStr += " select str_to_date('1900-01-01','%Y-%m-%d') as vdate, ";
    //    qStr += items.Sales_Inventory_ItemId + ", 0 as creditQty, coalesce(" + items.Sales_Inventory_OpeningStock + ",0) as debitQty, ";
    //    qStr += " 0 as cr_amount, " + items.Sales_Inventory_OpeningValue + " as dr_amount, ";
    //    qStr += " 'opening' as type from " + items.Sales_Inventory_Table_Name + " where " + items.Sales_Inventory_isPurchaseItem + " = 1 ";
    //    qStr += " UNION ALL ";
    qStr += " select trans." + itemHelper.Transaction_Item_Helper_VoucherDate + ", " +  itemHelper.Transaction_Item_Helper_ItemID + ", ";
    qStr += " (trans." + itemHelper.Transaction_Item_Helper_Cr_Qty + ") creditQty,(trans." + itemHelper.Transaction_Item_Helper_Dr_Qty + ") debitQty, ";
    qStr += "trans." + itemHelper.Transaction_Item_Helper_Cr_Amount + ", trans." + itemHelper.Transaction_Item_Helper_Dr_Amount + ", 'IN' as type ";
    qStr += " from " + itemHelper.Transaction_Item_Helper_TableName + " trans where ";
    qStr += " (select " + items.Sales_Inventory_isPurchaseItem + " from " + items.Sales_Inventory_Table_Name ;
    qStr += " where " + items.Sales_Inventory_ItemId + " = trans." + itemHelper.Transaction_Item_Helper_ItemID + ") = 1 ";
    qStr += " and " + itemHelper.Transaction_Item_Helper_VoucherDate + " <= str_to_date('" +dateTo.toString("yyyy-MM-dd")+ "','%Y-%m-%d')";

    if(bomFlag){
        qStr += " UNION ALL ";
        qStr += " select trans." + itemHelper.Transaction_Item_Helper_VoucherDate + ", " +  itemHelper.Transaction_Item_Helper_ItemID + ", ";
        qStr += " (trans." + itemHelper.Transaction_Item_Helper_Cr_Qty + "), (trans." + itemHelper.Transaction_Item_Helper_Dr_Qty + "), ";
        qStr += "trans." + itemHelper.Transaction_Item_Helper_Cr_Amount + ", trans." + itemHelper.Transaction_Item_Helper_Dr_Amount + ", 'IN' as type ";
        qStr += " from " + itemHelper.Transaction_Bom_Helper_TableName + " trans where ";
        qStr +=  itemHelper.Transaction_Item_Helper_VoucherDate + " <= str_to_date('" +dateTo.toString("yyyy-MM-dd")+ "','%Y-%m-%d')";
    }
    qStr += " ) A where item_id is not null ) B ";
    qStr += " where item_id in ";
    qStr += " (select  Item.item_id from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item";
    qStr += " inner join ";
    qStr += " (with recursive cte (group_id, group_name, parent_id, item_id) as (";
    qStr += "  select     grp.group_id,grp.group_name,grp.parent_id, item.item_id";
    qStr += "  FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item left outer join " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName + " grp ";
    qStr += "  on item.group_id = grp.group_id";
    qStr += "  union all ";
    qStr += "  select     p.group_id, p.group_name,p.parent_id, item_id";
    qStr += "  from       " + SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName + " p ";
    qStr += "  inner join cte ";
    qStr += "  on p.group_id = cte.parent_id )";
    qStr += " select cte.group_name, cte.item_id";
    qStr += " from cte where cte.parent_id = '"+ GroupId +"'";
    qStr += " ) itemGrp1 ";
    qStr += " on itemGrp1.item_id = item.item_id ";
    qStr += " union ";
    qStr += " select Item2.Item_id from ";
    qStr += " " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Item2 ";
    qStr += " where Item2.group_id = '"+ GroupId +"'";
    qStr += " )  ";
    qStr += " group by item_id "  ;
    qStr += " order by 1";

    qDebug()<<qStr;

    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}
