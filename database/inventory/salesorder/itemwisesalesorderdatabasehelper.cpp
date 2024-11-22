#include "itemwisesalesorderdatabasehelper.h"

QString ItemwiseSalesOrderDatabaseHelper::Order_Itemwise_TableName = "Sales_Order_Itemwise";
QString ItemwiseSalesOrderDatabaseHelper::Purchase_Order_Itemwise_TableName = "Purchase_Order_Itemwise";
QString ItemwiseSalesOrderDatabaseHelper::Sales_Order_Itemwise_TableName = "Sales_Order_Itemwise";
QString ItemwiseSalesOrderDatabaseHelper::Sales_Order_Itemwise_id = "_id";
QString ItemwiseSalesOrderDatabaseHelper::Sales_Order_Itemwise_Voucher_No = "Voucher_No";
QString ItemwiseSalesOrderDatabaseHelper::Sales_Order_Itemwise_Voucher_Prefix = "Voucher_Prefix";
QString ItemwiseSalesOrderDatabaseHelper::Sales_Order_Itemwise_Date_Created = "Date_Created";
QString ItemwiseSalesOrderDatabaseHelper::Sales_Order_Itemwise_Voucher_Date = "Voucher_Date";
QString ItemwiseSalesOrderDatabaseHelper::Sales_Order_Itemwise_TimeStamp = "TimeStamp";
QString ItemwiseSalesOrderDatabaseHelper::Sales_Order_Itemwise_SalesmanID = "Salesman_ID";
QString ItemwiseSalesOrderDatabaseHelper::Sales_Order_Itemwise_Item_ID = "Item_ID";
QString ItemwiseSalesOrderDatabaseHelper::Sales_Order_Itemwise_Qty_Total = "Qty_Total";
QString ItemwiseSalesOrderDatabaseHelper::Sales_Order_Itemwise_Narration = "Narration";
QString ItemwiseSalesOrderDatabaseHelper::Sales_Order_Itemwise_Added_By = "Added_By";
QString ItemwiseSalesOrderDatabaseHelper::Sales_Order_Itemwise_Damage = "Damage";
QString ItemwiseSalesOrderDatabaseHelper::Sales_Order_Itemwise_Total_Amount = "Total_Amount";
QString ItemwiseSalesOrderDatabaseHelper::Sales_Order_Itemwise_Voucher_Status = "Voucher_Status";
QString ItemwiseSalesOrderDatabaseHelper::main_TableName = "";
QString  ItemwiseSalesOrderDatabaseHelper::details_TableName="";



ItemwiseSalesOrderDatabaseHelper::ItemwiseSalesOrderDatabaseHelper(QString mainTableName, QString detailsTable)
{
    db = new DatabaseHelper();
    main_TableName = mainTableName;
    details_TableName = detailsTable;
}

void ItemwiseSalesOrderDatabaseHelper::setDetails_TableName(const QString &value)
{
    details_TableName = value;
}

void ItemwiseSalesOrderDatabaseHelper::setMain_TableName(const QString &value)
{
    main_TableName = value;
}

void ItemwiseSalesOrderDatabaseHelper::setOrder_Itemwise_TableName(const QString &value)
{
    Order_Itemwise_TableName = value;
}

void ItemwiseSalesOrderDatabaseHelper::setSales_Order_Itemwise_TableName(const QString &value)
{
    Order_Itemwise_TableName = value;
}

void ItemwiseSalesOrderDatabaseHelper::startTransaction()
{
    db->startTransaction();
}


int ItemwiseSalesOrderDatabaseHelper::commit()
{
    if(flag){
        db->commitTransaction();
        return 1;
    }
    else{
        db->rollBackTransaction();
        flag=true;
        return 0;
    }
    return 0;
}

int ItemwiseSalesOrderDatabaseHelper::getMaxID()
{
    QString qStr = "SELECT MAX("+Sales_Order_Itemwise_id+") FROM " + Order_Itemwise_TableName;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    int vNo = 1;
    query.prepare(qStr);
    if(query.exec()){
        while(query.next())
            vNo = query.value(0).toString().toInt();
    }
    else{
        qDebug()<<query.lastError();
    }
    qDebug()<<"MAx V No = "<<vNo;
    return vNo;
}

void ItemwiseSalesOrderDatabaseHelper::insertOrder(GeneralVoucherDataObject *voucher)
{

    startTransaction();
    QMap<QString,QVariant> map;
    voucher->voucherNumber = GeneralVoucherDatabaseHelper::setNextVoucherNoByType(voucher->voucherType,voucher->VoucherPrefix, Order_Itemwise_TableName);
    map.insert(Sales_Order_Itemwise_Voucher_No , voucher->voucherNumber);
    map.insert(Sales_Order_Itemwise_Voucher_Prefix , voucher->VoucherPrefix);
    map.insert(Sales_Order_Itemwise_Date_Created , voucher->DateCreated);
    map.insert(Sales_Order_Itemwise_Voucher_Date , voucher->VoucherDate);
    map.insert(Sales_Order_Itemwise_SalesmanID , voucher->SalesmanID);
    map.insert(Sales_Order_Itemwise_Item_ID ,voucher->InventoryItems[0].BaseItem.ItemID);
    map.insert(Sales_Order_Itemwise_Qty_Total , voucher->quantityTotal);
    map.insert(Sales_Order_Itemwise_Narration , voucher->narration);
    map.insert(Sales_Order_Itemwise_Added_By , voucher->AddedById);
    map.insert(Sales_Order_Itemwise_Damage , voucher->totalWeight);
    map.insert(Sales_Order_Itemwise_Total_Amount , voucher->grandTotal);
    map.insert(Sales_Order_Itemwise_Voucher_Status , voucher->status);
    db->insertRecordsPlus(map,Order_Itemwise_TableName);

    //    for(int i=0;i<voucher->InventoryItems.length();i++){
    //        QMap<QString,QVariant> map2;
    //        map2.insert(Sales_Order_Itemwise_Det_VoucherNO,voucher->voucherNumber);
    //        map2.insert(Sales_Order_Itemwise_Det_VoucherPref,voucher->voucherNumber);
    //        map2.insert(Sales_Order_Itemwise_Det_Item_Id,voucher->InventoryItems[i].BaseItem.ItemID);
    //        map2.insert(Sales_Order_Itemwise_Det_Led_Id,voucher->InventoryItems[i].BaseItem.defaultSalesLedgerID);
    //        if(voucher->InventoryItems[i].BaseItem.bomList.length()>0)
    //            map2.insert(Sales_Order_Itemwise_Det_Item_Id_1,voucher->InventoryItems[i].BaseItem.bomList[0].PurchaseItem);
    //        map2.insert(Sales_Order_Itemwise_Det_Qty_1,voucher->InventoryItems[i].BaseItem.quantity);
    //        map2.insert(Sales_Order_Itemwise_Det_Qty_2,voucher->InventoryItems[i].BaseItem.discQuantity);
    //        map2.insert(Sales_Order_Itemwise_Det_Rate,voucher->InventoryItems[i].BaseItem.price);
    //        map2.insert(Sales_Order_Itemwise_Det_SubTotal,voucher->InventoryItems[i].BaseItem.subTotal);
    //        map2.insert(Sales_Order_Itemwise_Det_Uom,voucher->voucherNumber);
    //        map2.insert(Sales_Order_Itemwise_Det_PriceLevel,voucher->ModeOfService);
    //        db->insertRecordsPlus(map2,Sales_Order_Itemwise_Det_TableName);
    //    }
    commit();
}

void ItemwiseSalesOrderDatabaseHelper::updateOrder(GeneralVoucherDataObject *voucher, QString oldID)
{
    startTransaction();
    QMap<QString,QVariant> map;
    map.insert(Sales_Order_Itemwise_Voucher_No , voucher->voucherNumber);
    map.insert(Sales_Order_Itemwise_Voucher_Prefix , voucher->VoucherPrefix);
    map.insert(Sales_Order_Itemwise_Voucher_Date , voucher->VoucherDate);
    map.insert(Sales_Order_Itemwise_SalesmanID , voucher->SalesmanID);
    map.insert(Sales_Order_Itemwise_Item_ID ,voucher->InventoryItems[0].BaseItem.ItemID);
    map.insert(Sales_Order_Itemwise_Qty_Total , voucher->quantityTotal);
    map.insert(Sales_Order_Itemwise_Narration , voucher->narration);
    map.insert(Sales_Order_Itemwise_Added_By , voucher->AddedById);
    map.insert(Sales_Order_Itemwise_Damage , voucher->totalWeight);
    map.insert(Sales_Order_Itemwise_Total_Amount , voucher->grandTotal);
    map.insert(Sales_Order_Itemwise_Voucher_Status , voucher->status);

    QMap<QString,QVariant> map2;
    map2.insert(Sales_Order_Itemwise_Voucher_No , voucher->voucherNumber);



    db->updateRecordsPlus(map,map2,Order_Itemwise_TableName);
    commit();
}

void ItemwiseSalesOrderDatabaseHelper::deleteOrder(QString oldID)
{
    startTransaction();
    QMap<QString,QVariant> map;
    map.insert(Sales_Order_Itemwise_Voucher_No , oldID);
    db->deleteRecordsPlus(map,Order_Itemwise_TableName);
    commit();
}



QSqlQueryModel *ItemwiseSalesOrderDatabaseHelper::getiSalesOrdersbetweenDates(QDate fromDate, QDate toDate)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    SalesInventoryItemDatabaseHelper invHelper;
    UserProfileDatabaseHelper userHelper;

    QString qStr = "";

    qStr += "SELECT ";
    qStr += Sales_Order_Itemwise_Voucher_Date +" as 'Voucher Date', ";
    qStr += Sales_Order_Itemwise_Voucher_No +" as 'Voucher no.',";
    qStr += "( SELECT " + invHelper.Sales_Inventory_ItemName + " from ";
    qStr += invHelper.Sales_Inventory_Table_Name + " si WHERE ";
    qStr += "si."+invHelper.Sales_Inventory_ItemId + "= soi.";
    qStr += Sales_Order_Itemwise_Item_ID    +") as 'Item ',";
    qStr += Sales_Order_Itemwise_Qty_Total  +" as 'Total Quantity',";
    qStr += "(SELECT " + userHelper.Employee_Details_Name+ " From ";
    qStr += userHelper.Employee_Details_TableName + " ed WHERE ";
    qStr += "ed."+userHelper.Employee_Details_id +  " = soi.";
    qStr += Sales_Order_Itemwise_SalesmanID +") as 'Sales Man'";
    qStr += " FROM " + Order_Itemwise_TableName + " soi ";
    qStr += " WHERE ";
    qStr += Sales_Order_Itemwise_Voucher_Date +">='" + fromDate.toString("yyyy-MM-dd")+ "'";
    qStr += " AND ";
    qStr += Sales_Order_Itemwise_Voucher_Date +"<='" + toDate.toString("yyyy-MM-dd")+ "'";

    model = DatabaseHelper::getQueryModel(qStr);

    return  model;
}

GMSQLQUeryModel *ItemwiseSalesOrderDatabaseHelper::getiiSalesOrdersbetweenDates(QDate fromDate, QDate toDate)
{
    QMap<int,Qt::CheckState> m;

    QMap<int, Qt::CheckState> check_state_map1;

    GMSQLQUeryModel *model = new GMSQLQUeryModel(check_state_map1);
    SalesInventoryItemDatabaseHelper invHelper;
    UserProfileDatabaseHelper userHelper;

    QString qStr = "";
    qStr += "SELECT '', ";
    qStr += Sales_Order_Itemwise_Voucher_Date +" as 'Voucher Date', ";
    qStr += Sales_Order_Itemwise_Voucher_No +" as 'Voucher no.',";
    qStr += "( SELECT " + invHelper.Sales_Inventory_ItemName + " from ";
    qStr += invHelper.Sales_Inventory_Table_Name + " si WHERE ";
    qStr += "si."+invHelper.Sales_Inventory_ItemId + "= soi.";
    qStr += Sales_Order_Itemwise_Item_ID    +") as 'Item ',";
    qStr += Sales_Order_Itemwise_Qty_Total  +" as 'Total Quantity',";
    qStr += "(SELECT " + userHelper.Employee_Details_Name+ " From ";
    qStr += userHelper.Employee_Details_TableName + " ed WHERE ";
    qStr += "ed."+userHelper.Employee_Details_id +  " = soi.";
    qStr += Sales_Order_Itemwise_SalesmanID +") as 'Sales Man',";
    qStr += Sales_Order_Itemwise_Total_Amount +" ";
    qStr += " FROM " + Order_Itemwise_TableName + " soi ";
    qStr += " WHERE ";
    qStr += Sales_Order_Itemwise_Voucher_Date +">='" + fromDate.toString("yyyy-MM-dd")+ "'";
    qStr += " AND ";
    qStr += Sales_Order_Itemwise_Voucher_Date +"<='" + toDate.toString("yyyy-MM-dd")+ "'";
    qDebug()<<"Setting model";

    model = (GMSQLQUeryModel*) (DatabaseHelper::getQueryModel(qStr));


    return  model;
}

QSqlQueryModel *ItemwiseSalesOrderDatabaseHelper::getDatesOfPurchaseByItem(QString itemID, QDate salesDate)
{
    QString qStr = "SELECT";

    return DatabaseHelper::getQueryModel(qStr);
}

QSqlQueryModel *ItemwiseSalesOrderDatabaseHelper::getSOitemsByPDate(QDate purchDate)
{
    QString qStr = " SELECT ";
    qStr += " voucher_date as Date,";
    qStr += " (SELECT ITEM_NAME FROM SALES_INVENTORY_ITEMS SI WHERE SOD.inventory_item_id=SI.ITEM_ID)";
    qStr += " as Item,";
    qStr += " SUM(Quantity_Full) AS Full, ";
    qStr += " SUM(Quantity_Disc) AS 'Wt.disc', ";
    qStr += " SUM(Quantity) AS' Net.Wt', ";
    qStr += " SUM(ContainerCount) AS 'Tray', ";
    qStr += " SUM(subTotal) AS 'Total' ";
    qStr += " FROM Sales_Order_Details SOD";
    qStr += " WHERE TimeDate = " + db->getDateString(purchDate);
    qStr += " GROUP BY SOD.inventory_item_id, SOD.voucher_date";
    qStr += " ORDER BY 1";
    return DatabaseHelper::getQueryModel(qStr);
}


QSqlQueryModel *ItemwiseSalesOrderDatabaseHelper::getSOitemsBySDate(QDate salesDate)
{
    QString qStr = " SELECT ";
    qStr += " inventory_item_id, ";
    qStr += " Cast(TimeDate as DATE) as 'P.Date',";
    qStr += " (SELECT ITEM_NAME FROM SALES_INVENTORY_ITEMS SI WHERE SOD.inventory_item_id=SI.ITEM_ID)";
    qStr += " as Item,";
    qStr += " SUM(Quantity_Full) AS Full, ";
    qStr += " SUM(Quantity_Disc) AS 'Wt.disc', ";
    qStr += " SUM(Quantity) AS' Net.Wt', ";
    qStr += " SUM(ContainerCount) AS 'Tray', ";
    qStr += " SUM(subTotal) AS 'Total' ";
    qStr += " FROM Sales_Order_Details SOD";
    qStr += " WHERE voucher_date = " + db->getDateString(salesDate);
    qStr += " GROUP BY SOD.inventory_item_id, SOD.TimeDate";
    qStr += " ORDER BY 1";
    return DatabaseHelper::getQueryModel(qStr);
}

QSqlQueryModel *ItemwiseSalesOrderDatabaseHelper::getItemDistinctDatesSO(QString itemID, QDate salesDate)
{
    QString qStr = " SELECT ";
    qStr += " DISTINCT(TimeDate) ";
    qStr += " FROM Sales_Order_Details ";
    qStr += " WHERE voucher_date = " + db->getDateString(salesDate);
    qStr += " AND inventory_item_id = '" + itemID + "'";
    qStr += " ORDER BY 1";

    return DatabaseHelper::getQueryModel(qStr);
}

QSqlQueryModel *ItemwiseSalesOrderDatabaseHelper::getItemSOPendingReportByitem(QString itemID, QDate salesDate, QDate PurchDate)
{
    QString qStr = " SELECT main.Ledger_Id,det.Project_id,det.uom_id,";
    qStr += " (SELECT Ledger_Name FROM " + LedgerMasterDatabaseHelper::Ledger_Master_TableName;
    qStr += " WHERE Ledger_Id = main.Ledger_Id) as Ledger, ";
    qStr += " det.Quantity_Full as 'Wt Total',";
    qStr += " det.Quantity_Disc as 'Wt. Disc',";
    qStr += " det.quantity as 'Net Qty',";
    qStr += " det.ContainerCount as 'Tray',";
    qStr += " det.price as 'Rate', det.subTotal as 'Total'";
    qStr += " FROM Sales_Order_Main Main JOIN Sales_Order_Details det ON ";
    qStr += " main.voucher_no = det.voucher_no and main.voucher_prefix = det.voucher_prefix ";
    qStr += " WHERE main.voucher_date = " + db->getDateString(salesDate);
    qStr += " AND det.TimeDate = " + db->getDateString(PurchDate);
    qStr += " AND det.inventory_item_id = '" + itemID + "'";
    qStr += " ORDER BY 2 desc";

    return DatabaseHelper::getQueryModel(qStr);
}

QSqlQueryModel *ItemwiseSalesOrderDatabaseHelper::getPurchaseQuantitiesBYITEMDATE(QString itemID, QDate pDate)
{
    //    QString qStr = "SELECT";
    //    qStr += " sum(Quantity_Full),sum(Quantity_Disc),";
    //    qStr += " sum(quantity),sum(ContainerCount),Voucher_no,voucher_prefix from purchase_order_details where voucher_date="+db->getDateString(pDate);
    //    qStr += " and ";
    //    qStr += " inventory_item_id = '"+ itemID +"'";


    QString qStr = " SELECT main.Ledger_Id,det.Project_id,det.uom_id,";
    qStr += " (SELECT Ledger_Name FROM " + LedgerMasterDatabaseHelper::Ledger_Master_TableName;
    qStr += " WHERE Ledger_Id = main.Ledger_Id) as Ledger, ";
    qStr += " sum(det.Quantity_Full) as 'Wt Total',";
    qStr += " sum(det.Quantity_Disc) as 'Wt. Disc',";
    qStr += " sum(det.quantity) as 'Net Qty',";
    qStr += " sum(det.ContainerCount) as 'Tray',";
    qStr += " det.price as 'Rate', det.subTotal as 'Total'";
    qStr += " FROM Purchase_Order_Main Main JOIN Purchase_Order_Details det ON ";
    qStr += " main.voucher_no = det.voucher_no and main.voucher_prefix = det.voucher_prefix ";
    qStr += " WHERE main.voucher_date = " + db->getDateString(pDate);
    qStr += " AND det.inventory_item_id = '" + itemID + "'";
    qStr += " ORDER BY 2 desc";

    qDebug()<<qStr;

    return DatabaseHelper::getQueryModel(qStr);
}

QSqlQueryModel *ItemwiseSalesOrderDatabaseHelper::getSalesOrderReportByPO(QString itemID, QDate salesDate)
{
    QString qStr = "select  B.TimeDate as 'Purch Date', Round(Sum(Sold_Full),2) as 'Sold Full', ";
    qStr += " Round(Sum(Sold_Split),2) as 'Sold Split', Round(COALESCE(Quantity ,0) ,2) as 'Total Purchase',";
    qStr += " Round(COALESCE(Quantity ,0)  - (Sum(Sold_Full) + Sum(Sold_Split)),2) as Balance, ";
    qStr += " coalesce(sum(PO.ContainerCount),0) - coalesce(B.ContCount,0) as 'Container Count' ";
    qStr += " FROM ";
    qStr += " ( select  TimeDate, (case when type = 'Full' then (Quantity) else 0 end ) as Sold_Full, ";
    qStr += " (case when type = 'Split' then (Quantity) else 0 end ) as Sold_Split, sum(ContCount) ContCount FROM ";
    qStr += " (select inventory_item_id, timeDate,det.Uom_Id, det.Project_ID, sum(quantity) Quantity, sum(det.ContainerCount) as ContCount, 'Full' as type from ";
    qStr += " sales_order_details det ";
    qStr += " inner join " + SalesOrderDatabaseHelper::Sales_Order_Main_TableName + " main ";
    qStr += " on det.voucher_no = main.voucher_no and det.voucher_prefix = main.voucher_prefix ";
    qStr += " and main.Ledger_ID not like (SELECT ledger_id from " + LedgerMasterDatabaseHelper::Ledger_Master_TableName ;
    qStr += " where ledger_name like 'Split') ";
    qStr += " and main.voucher_date = " + db->getDateString(salesDate) ;
    qStr += " and det.inventory_item_id = '" + itemID + "'";
    qStr += " group by  timeDate, det.Project_ID ";
    qStr += " union all ";
    qStr += " select inventory_item_id, timeDate,det.Uom_Id, det.Project_ID, sum(quantity) Quantity,  sum(det.ContainerCount) as ContCount, 'Split' from ";
    qStr += " sales_order_details det ";
    qStr += " inner join " + SalesOrderDatabaseHelper::Sales_Order_Main_TableName + " main  ";
    qStr += " on det.voucher_no = main.voucher_no and det.voucher_prefix = main.voucher_prefix ";
    qStr += " and main.Ledger_ID = (SELECT ledger_id from " + LedgerMasterDatabaseHelper::Ledger_Master_TableName ;
    qStr += " where ledger_name like 'Split')";
    qStr += " and main.voucher_date >= " + db->getDateString(salesDate) ;
    qStr += " and det.inventory_item_id = '" + itemID + "'";
    qStr += " group by  timeDate ";
    qStr += "  ) A ";
    qStr += "  ) B JOIN Purchase_Order_Details PO ";
    qStr += "  ON PO.Voucher_Date = B.TimeDate ";
    qStr += "  AND PO.Inventory_Item_Id = '" + itemID + "'";
    qStr += "  group by  B.TimeDate" ;

    return DatabaseHelper::getQueryModel(qStr);
}

GeneralVoucherDataObject *ItemwiseSalesOrderDatabaseHelper::getVoucherByVoucherNoAsPtr(QString vNo, QString vPrefix)
{
    SalesInventoryItemDatabaseHelper* itemHelper = new SalesInventoryItemDatabaseHelper;
    LedgerMasterDatabaseHelper ledHelper;
    GeneralVoucherDataObject *voucher = new GeneralVoucherDataObject();
    QString voucherItemID="";
    float damage =0;
    QString qStrMain = "SELECT ";
    qStrMain += Sales_Order_Itemwise_Voucher_No + ", ";
    qStrMain += Sales_Order_Itemwise_Voucher_Date + ", ";
    qStrMain += Sales_Order_Itemwise_Item_ID + ", ";
    qStrMain += Sales_Order_Itemwise_Damage+", ";
    qStrMain += Sales_Order_Itemwise_SalesmanID+", ";
    qStrMain += Sales_Order_Itemwise_Voucher_Status+", ";
    qStrMain += Sales_Order_Itemwise_Qty_Total;
    qStrMain += " FROM ";
    qStrMain += Order_Itemwise_TableName;
    qStrMain += " WHERE ";
    qStrMain += Sales_Order_Itemwise_Voucher_No + "=:A1";


    QString qStrDetail;
    qStrDetail = "SELECT distinct ";
    qStrDetail += SalesOrderDatabaseHelper::Sales_Order_Details_id + ", ";
    qStrDetail += SalesOrderDatabaseHelper::Sales_Order_Details_Inventory_Item_ID + ", ";
    qStrDetail += SalesOrderDatabaseHelper::Sales_Order_Details_Quantity + ", ";
    qStrDetail += SalesOrderDatabaseHelper::Sales_Order_Details_Price + ", ";
    qStrDetail += SalesOrderDatabaseHelper::Sales_Order_Details_Voucher_No + ", ";
    qStrDetail += SalesOrderDatabaseHelper::Sales_Order_Details_Voucher_Prefix + ", ";
    qStrDetail += SalesOrderDatabaseHelper::Sales_Order_Details_Voucher_Date + ", ";
    qStrDetail += SalesOrderDatabaseHelper::Sales_Order_Details_Discount_Percent +", ";
    qStrDetail += SalesOrderDatabaseHelper::Sales_Order_Details_Discount_Amount +", ";
    qStrDetail += SalesOrderDatabaseHelper::Sales_Order_Details_Subtotal +", ";
    qStrDetail += SalesOrderDatabaseHelper::Sales_Order_Details_isCompoundItem +", ";
    qStrDetail += SalesOrderDatabaseHelper::Sales_Order_Details_subItemOff_list_ID +", ";
    qStrDetail += SalesOrderDatabaseHelper::Sales_Order_Details_PriceLevel +", ";
    qStrDetail += SalesOrderDatabaseHelper::Sales_Order_Details_Item_Description +", ";
    qStrDetail += SalesOrderDatabaseHelper::Sales_Order_Details_Item_Narration +", ";
    qStrDetail += SalesOrderDatabaseHelper::Sales_Order_Details_Requirement_ItemID +", ";
    qStrDetail += SalesOrderDatabaseHelper::Sales_Order_Details_Dimension + ", ";
    qStrDetail += SalesOrderDatabaseHelper::Sales_Order_Details_Length + ", ";
    qStrDetail += "( SELECT "+SalesOrderDatabaseHelper::Sales_Order_Main_Ledger_ID + " FROM " ;
    qStrDetail += main_TableName+ " som WHERE ";
    qStrDetail += "som."+SalesOrderDatabaseHelper::Sales_Order_Main_Voucher_No + "=";
    qStrDetail += "sod."+SalesOrderDatabaseHelper::Sales_Order_Details_Voucher_No + ") as 'ledid', ";
    qStrDetail += "subq.convalue , subq.UomName, subq.UomSymbol, ";
    qStrDetail += SalesOrderDatabaseHelper::Sales_Order_Details_QuantityFull +", ";
    qStrDetail += SalesOrderDatabaseHelper::Sales_Order_Details_QuantityDisc +", ";
    qStrDetail += SalesOrderDatabaseHelper::Sales_Order_Details_itemVoucherStatus +", ";
    qStrDetail += SalesOrderDatabaseHelper::Sales_Order_Details_itemProductionStatus +", ";
    qStrDetail += SalesOrderDatabaseHelper::Sales_Order_Details_TimeDate +", ";
    qStrDetail += SalesOrderDatabaseHelper::Sales_Order_Details_ContainerCount +", ";
    qStrDetail += SalesOrderDatabaseHelper::Sales_Order_Details_Project_ID +", ";
    qStrDetail += "subq2.UomSymbol as 'UOM2' ";

    qStrDetail += " FROM "+ details_TableName +" sod ";
    qStrDetail += " LEFT OUTER JOIN ";

    qStrDetail += UomDataBaseHelper::qStrUomForItemDetail + " as subq ";
    qStrDetail += " ON sod." + SalesOrderDatabaseHelper::Sales_Order_Details_Inventory_Item_ID + " = subq.itemID ";
    qStrDetail += " AND sod." + SalesOrderDatabaseHelper::Sales_Order_Details_UOM_ID + " = subq.UOM ";
    qStrDetail += " LEFT OUTER JOIN ";

    qStrDetail += UomDataBaseHelper::qStrUomForItemDetail + " as subq2 ";
    qStrDetail += " ON sod." + SalesOrderDatabaseHelper::Sales_Order_Details_Inventory_Item_ID + " = subq2.itemID ";
    qStrDetail += " AND sod." + SalesOrderDatabaseHelper::Sales_Order_Details_Project_ID + " = subq2.UOM ";
    qStrDetail += " WHERE " + SalesOrderDatabaseHelper::Sales_Order_Details_Dimension +"='"+vNo+"'";
    qStrDetail += " order by  " + SalesOrderDatabaseHelper::Sales_Order_Details_Length +" asc";


    qDebug()<<qStrDetail;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QSqlQuery query2(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStrMain);
    query.bindValue(":A1",vNo);
    if(query.exec()){
        while (query.next()) {
            int i=0;
            voucher->voucherNumber = query.value(i++).toString();
            voucher->VoucherDate = query.value(i++).toDate();
            voucherItemID = query.value(i++).toString();
            damage = query.value(i++).toFloat(); // DAMAGE ENTERED;
            voucher->SalesmanID = query.value(i++).toInt();
            voucher->status = query.value(i++).toInt();
            voucher->quantityTotal = query.value(i++).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }

    //    qDebug()<<qStrDetail;
    query2.prepare(qStrDetail);
    //    query2.bindValue(":vno",vNo);
    if(query2.exec()){



        while(query2.next()){

            inventoryItemDataModel item = itemHelper->getInventoryItemByID(query2.value(1).toString());

            if( query2.value(17).toFloat()<0)
                continue;
            int i =0;
            item.listId = query2.value(0).toInt();

            item.quantity = query2.value(2).toFloat();
            item.price = query2.value(3).toFloat();
            i=6;
            item.discountPercentage = query2.value(7).toFloat();
            item.discountinAmount = query2.value(8).toFloat();
            item.subTotal = query2.value(9).toFloat();
            i=10;
            item.PriceLevel = query2.value(12).toString();
            item.ItemDescription = query2.value(13).toString();
            item.narration = query2.value(14).toString();
            item.ItemReqUuid = query2.value(15).toString();
            item.Dimension = query2.value(16).toString();
            item.length = query2.value(17).toFloat();
            item.defaultSalesLedgerID = query2.value(18).toString();
            item.ItemAlias =ledHelper.getLedgerNameByID(item.defaultSalesLedgerID);

            item.uomObject.convRate = query.value(19).toFloat();
            item.uomObject.UomName = query.value(20).toString();
            item.uomObject.UomSymbol = query.value(21).toString();



            item.quantityFull = query2.value(22).toFloat();
            item.discQuantity = query2.value(23).toFloat();
            item.itemVoucherStatus = query2.value(24).toInt();
            item.itemProductionStatus = query2.value(25).toInt();
            item.manufactureDate = query2.value(26).toDate();
            item.requestQty = query2.value(27).toFloat();
            item.ProjectID = query2.value(28).toString();
            item.brandName = query2.value(29).toString();
            qDebug()<<"Item status of "<<item.ItemName<<" is "<< item.itemVoucherStatus;

            item.bomList = BillOfMaterialsIngredientsDataBaseHelper::getBillOfMaterials(item.ItemID);
            if(query2.value(10).toBool()||(query2.value(11).toInt()==0)){
                CompoundItemDataObject cObj;
                cObj.BaseItem = item;
                voucher->InventoryItems.append(cObj);
            }
            else{
                int itemSize = voucher->InventoryItems.size();
                voucher->InventoryItems[itemSize-1].subItems.append(item);
                voucher->InventoryItems[itemSize-1].subItemsTotal +=item.subTotal;
            }
        }
    }
    else{
        qDebug()<<"Query Error";
        qDebug()<<query2.lastError();
        qDebug()<<query2.lastQuery();
    }



    if(voucher->InventoryItems.size()>0)
        voucher->InventoryItems[0].BaseItem.prevQty = damage;

    return  voucher;

}

QStringList ItemwiseSalesOrderDatabaseHelper::getVoucherswithOrderID(QString vID)
{
    QStringList vouchers;
    QString qStr = "SELECT DISTINCT("+SalesOrderDatabaseHelper::Sales_Order_Details_Voucher_No+")";
    qStr += " FROM ";
    qStr += details_TableName;
    qStr += " WHERE " + SalesOrderDatabaseHelper::Sales_Order_Details_Dimension +"='"+vID+"'";

    qDebug()<<qStr;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    if(query.exec(qStr)){
        while (query.next()) {
            vouchers<<query.value(0).toString();
        }
    }
    else{
        qDebug()<<query.lastError();
    }
    return vouchers;
}
