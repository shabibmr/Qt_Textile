#include "billofmaterialsingredientsdatabasehelper.h"
#include "database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h"

QString BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_TableName = "ingredients";
QString BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_id = "_id";
QString BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_Sales_Item = "sales_item";
QString BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_Sales_Item_Qty = "sales_item_qty";
QString BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_Purchase_Item = "purchase_item";
QString BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_Purchase_Item_Qty = "purchase_item_qty";
QString BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_Purchase_Unit = "Purchase_unit";
QString BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_TakeAway = "TakeAway";

BillOfMaterialsIngredientsDataBaseHelper::BillOfMaterialsIngredientsDataBaseHelper()
{
    db = new DatabaseHelper();
}

int BillOfMaterialsIngredientsDataBaseHelper::startTransaction()
{
    db->startTransaction();
}

int BillOfMaterialsIngredientsDataBaseHelper::insertBillOfMaterials(QList<RestaurantIngredientsDataModel> objlist)
{
    this->startTransaction();
    for(RestaurantIngredientsDataModel obj:objlist){
        QMap<QString,QVariant> map;

        map.insert(Bill_Of_Materials_Ingredients_Sales_Item,obj.SalesItem);
        map.insert(Bill_Of_Materials_Ingredients_Sales_Item_Qty,obj.SalesItemQty);
        map.insert(Bill_Of_Materials_Ingredients_Purchase_Item,obj.PurchaseItem);
        map.insert(Bill_Of_Materials_Ingredients_Purchase_Item_Qty,obj.PurchaseItemQty);
        map.insert(Bill_Of_Materials_Ingredients_TakeAway,obj.TakeAway);
        map.insert(Bill_Of_Materials_Ingredients_Purchase_Unit, obj.PurchaseUom);

        db->insertRecordsPlus(map,Bill_Of_Materials_Ingredients_TableName);
    }
    return commit();
}

int BillOfMaterialsIngredientsDataBaseHelper::deleteBillOfMaterials(QString SalesInventoryItemID)
{
    this->startTransaction();
    QMap<QString,QVariant> map;

    map.insert(Bill_Of_Materials_Ingredients_Sales_Item,SalesInventoryItemID);

    db->deleteRecordsPlus(map,Bill_Of_Materials_Ingredients_TableName);
    return commit();
}

QList<RestaurantIngredientsDataModel> BillOfMaterialsIngredientsDataBaseHelper::getBillOfMaterials(QString SalesInventoryItemID)
{
    QList<RestaurantIngredientsDataModel> bomlist;
    UomDataBaseHelper *uomHelper = new UomDataBaseHelper();

    QString qStr = "SELECT ";
    //    qStr += Bill_Of_Materials_Ingredients_Sales_Item + ", ";
    //    qStr += Bill_Of_Materials_Ingredients_Sales_Item_Qty + ", ";
    //    qStr += Bill_Of_Materials_Ingredients_Purchase_Item + ", ";
    //    qStr += Bill_Of_Materials_Ingredients_Purchase_Item_Qty + ", ";
    //    qStr += Bill_Of_Materials_Ingredients_TakeAway + " ";
    //    qStr += " FROM "+ Bill_Of_Materials_Ingredients_TableName ;
    //    qStr += " where " + Bill_Of_Materials_Ingredients_Sales_Item + "='"+ SalesInventoryItemID +"'";

    qStr += Bill_Of_Materials_Ingredients_Sales_Item + ", ";
    qStr += Bill_Of_Materials_Ingredients_Sales_Item_Qty + ", ";
    qStr += Bill_Of_Materials_Ingredients_Purchase_Item + ", ";
    qStr += "( SELECT section FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where item_id = " + Bill_Of_Materials_Ingredients_Purchase_Item + "), ";
    qStr += Bill_Of_Materials_Ingredients_Purchase_Item_Qty + ", ";
    qStr += Bill_Of_Materials_Ingredients_Purchase_Unit + ", ";
    qStr += Bill_Of_Materials_Ingredients_TakeAway + ", ";
    qStr += "cast(coalesce(subq.ConValue,1) as decimal(10,3)) as conValue ";
    qStr += " FROM "+ Bill_Of_Materials_Ingredients_TableName + " ing ";
    qStr += " LEFT OUTER JOIN ";

    qStr += " (SELECT distinct " ;
    qStr += uomHelper->UOM_Conversion_ItemId + ", ";
    qStr += uomHelper->UOM_Conversion_ToUnit + " as UOM, ";
    qStr += uomHelper->UOM_Conversion_Value + " ";
    qStr += " FROM " + uomHelper->UOM_Conversion_TableName;
    qStr += " UNION ";
    qStr += " SELECT B." +  uomHelper->UOM_Conversion_ItemId + ", ";
    qStr += " case when A." + uomHelper->UOM_Conversion_BaseUnit + " = B." + uomHelper->UOM_Conversion_ToUnit ;
    qStr += " then A." + uomHelper->UOM_Conversion_ToUnit;
    qStr += " when A." + uomHelper->UOM_Conversion_ToUnit + " = B." + uomHelper->UOM_Conversion_ToUnit ;
    qStr += " then A." + uomHelper->UOM_Conversion_BaseUnit;
    qStr += " else '' END as UOM, ";
    qStr += " case when A." + uomHelper->UOM_Conversion_BaseUnit + " = B." + uomHelper->UOM_Conversion_ToUnit ;
    qStr += " then B." + uomHelper->UOM_Conversion_Value + " * A." + uomHelper->UOM_Conversion_Value;
    qStr += " when A." + uomHelper->UOM_Conversion_ToUnit + " = B." + uomHelper->UOM_Conversion_ToUnit ;
    qStr += " then cast(B." + uomHelper->UOM_Conversion_Value + " / A." +uomHelper-> UOM_Conversion_Value + " as decimal(10,3))";
    qStr += " else 1 END  ";
    qStr += " FROM " + uomHelper->UOM_Conversion_TableName + " A ";
    qStr += " INNER JOIN " +  uomHelper->UOM_Conversion_TableName + " B " ;
    qStr += " ON ( A." + uomHelper->UOM_Conversion_BaseUnit + " = B." + uomHelper->UOM_Conversion_ToUnit;
    qStr += " OR A." + uomHelper->UOM_Conversion_ToUnit + " = B." + uomHelper->UOM_Conversion_ToUnit + ")";
    qStr += " AND A." + uomHelper->UOM_Conversion_ItemId + " = '' ) as subq";
    qStr += " ON ing." + Bill_Of_Materials_Ingredients_Purchase_Item + " = subq.itemID";
    qStr += " AND cast(ing." + Bill_Of_Materials_Ingredients_Purchase_Unit + " as int) = cast(subq.uom as int)";
    qStr += " where " + Bill_Of_Materials_Ingredients_Sales_Item + "='"+ SalesInventoryItemID +"'";

//    qDebug()<<qStr;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);

    if(query.exec()){
        int i=0;
        while(query.next()){
            i=0;
            RestaurantIngredientsDataModel Ingredients;
            Ingredients.SalesItem = query.value(i++).toString();
            Ingredients.SalesItemQty = query.value(i++).toFloat();
            Ingredients.PurchaseItem = query.value(i++).toString();
            Ingredients.purchaseItemSection = query.value(i++).toString();
//            qDebug()<<"section"<<Ingredients.purchaseItemSection;
            Ingredients.PurchaseItemQty = query.value(i++).toFloat();
            Ingredients.PurchaseUom = query.value(i++).toString();
            Ingredients.TakeAway = query.value(i++).toBool();
            Ingredients.conValue = query.value(i++).toFloat();
            bomlist.append(Ingredients);
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return bomlist;
}

QSqlQueryModel *BillOfMaterialsIngredientsDataBaseHelper::getSalesItemByPurchaseItemModel(QString itemId)
{
    QSqlQueryModel* model = new QSqlQueryModel;

    QString qStr = " SELECT (SELECT Item_Name FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where Item_ID = " ;
    qStr += Bill_Of_Materials_Ingredients_Sales_Item + ") AS 'Sales Item', ";
    qStr += Bill_Of_Materials_Ingredients_Sales_Item_Qty + " AS 'Sales Qty', ";
    qStr += "Cast(" + Bill_Of_Materials_Ingredients_Purchase_Item_Qty + " AS Decimal(10,2)) AS 'Purchase Qty' ";
    qStr += " FROM " + Bill_Of_Materials_Ingredients_TableName ;
    qStr += " WHERE " + Bill_Of_Materials_Ingredients_Purchase_Item + " = '" + itemId + "'";
    qStr += "UNION SELECT ITEM_NAME, 1,1 FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" Where Item_id = '" + itemId + "'";
    qStr += " and isSalesItem = 1";

    qDebug()<<qStr;

    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}

int BillOfMaterialsIngredientsDataBaseHelper::commit()
{
    if(flag){
        db->commitTransaction();
        return 1;
    }
    else{
        db->rollBackTransaction();
        return 0;
    }
}

QList<CompoundItemDataObject> BillOfMaterialsIngredientsDataBaseHelper::getBOMBySalesItemList(QList<CompoundItemDataObject> itemList)
{
    QList<CompoundItemDataObject> returnlist;
    bool qStat = false;

    // create item list
    QString dropStr = " DROP TABLE IF EXISTS ITEM_TMP ";
    QString createStr = " CREATE TABLE ITEM_TMP (ItemID VARCHAR(20), Quantity FLOAT(10,3))";
    QSqlQuery queryDrop(QSqlDatabase::database(DatabaseValues::connectionString));
    QSqlQuery queryCreate(QSqlDatabase::database(DatabaseValues::connectionString));
    QSqlQuery queryInsert(QSqlDatabase::database(DatabaseValues::connectionString));

    queryDrop.prepare(dropStr);
    queryDrop.exec();

    SalesInventoryItemDatabaseHelper sHelper;
    queryCreate.prepare(createStr);
    if(queryCreate.exec()){
        qStat = true;
        for(CompoundItemDataObject item:itemList){
            QString qStrInsert = " INSERT INTO ITEM_TMP (ItemID, Quantity) values (:itemid, :qty) " ;
            queryInsert.prepare(qStrInsert);
            queryInsert.bindValue(":itemid", item.BaseItem.ItemID);
            queryInsert.bindValue(":qty", item.BaseItem.quantity);

            if(!queryInsert.exec()){
                qStat = false;
                qDebug()<<queryInsert.lastError();
                qDebug()<<queryInsert.lastQuery();
            }
        }
    }
    else{
        qDebug()<<queryCreate.lastError();
        qDebug()<<queryCreate.lastQuery();
    }

    if(qStat){
        QString qStr = "";
        qStr += " SELECT Item, SUM(Qty) FROM (";
        qStr += " SELECT ";
        qStr += Bill_Of_Materials_Ingredients_Purchase_Item + " as Item, ";
        qStr += "COALESCE(" + Bill_Of_Materials_Ingredients_Purchase_Item_Qty + ", 1)";
        qStr += "/ COALESCE(" +Bill_Of_Materials_Ingredients_Sales_Item_Qty + ", 1) * Item_Tmp.Quantity as Qty ";
//        qStr += Bill_Of_Materials_Ingredients_Purchase_Unit + ", ";
//        qStr += "cast(coalesce(subq.ConValue,1) as decimal(10,3)) as conValue ";
        qStr += " FROM "+ Bill_Of_Materials_Ingredients_TableName + " ing ";
//        qStr += " LEFT OUTER JOIN ";
//        qStr += UomDataBaseHelper::qStrUomForItemDetail + " as subq ";
//        qStr += " ON ing." + Bill_Of_Materials_Ingredients_Purchase_Item + " = subq.itemID";
//        qStr += " AND cast(ing." + Bill_Of_Materials_Ingredients_Purchase_Unit + " as int) = cast(subq.uom as int)";
        qStr += " RIGHT OUTER JOIN Item_Tmp  ON ing." + Bill_Of_Materials_Ingredients_Sales_Item + " = Item_Tmp.ItemID";
        qStr += " ) A where Item is not null GROUP BY Item" ;

        QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

//        qDebug()<<qStr;
        query.prepare(qStr);

        if(query.exec()){
            int i=0;
            while(query.next()){
                i=0;
                CompoundItemDataObject compItem;
                compItem.BaseItem = sHelper.getInventoryItemByID(query.value(i++).toString());
                compItem.BaseItem.calculatedQty = query.value(i++).toFloat();
                compItem.BaseItem.requestQty = compItem.BaseItem.calculatedQty;
                compItem.BaseItem.quantity = 0;
                compItem.BaseItem.prevQty = 0;
                returnlist.append(compItem);

            }
        }
        else{
            qDebug()<<query.lastError();
            qDebug()<<query.lastQuery();
        }
    }
    return returnlist;
}
