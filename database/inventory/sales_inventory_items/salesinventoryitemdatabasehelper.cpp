#include "salesinventoryitemdatabasehelper.h"
#include "QMessageBox"

QString SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name = "Sales_Inventory_Items";
QString SalesInventoryItemDatabaseHelper::SKU_MASTER_TABLENAME = "SKU_MASTER";

int SalesInventoryItemDatabaseHelper::insertSKU(inventoryItemDataModel *item)
{
    QMap<QString,QVariant> map;
    for(int i=0;i<item->skuList.size();i++){
        map.clear();
        map.insert(SKU_ID,item->skuList[i]->skuID);
        map.insert(SKU_Name,item->skuList[i]->skuDescription);
        map.insert(SKU_ITEM_ID,item->ItemID);
        map.insert(SKU_PROPERTY_ID,item->skuList[i]->skuProperty_ID);
        map.insert(SKU_PROPRTY_VALUE,item->skuList[i]->skuID);
        map.insert(SKU_Description,item->skuList[i]->skuDescription);
        map.insert(SKU_Barcode,item->skuList[i]->SKUBarcode);
        map.insert(SKU_Uom,item->skuList[i]->skuUom);
        db->insertRecordsPlus(map,SKU_MASTER_TABLENAME);
    }
}

int SalesInventoryItemDatabaseHelper::deleteSKU(inventoryItemDataModel *item)
{
    QMap<QString,QVariant> clause;
    clause.insert(SKU_ITEM_ID,item->ItemID);
    db->deleteRecordsPlus(clause,SKU_MASTER_TABLENAME);
}

SalesInventoryItemDatabaseHelper::SalesInventoryItemDatabaseHelper(DatabaseHelper *db)
{
    this->db = db;
}

SalesInventoryItemDatabaseHelper::~SalesInventoryItemDatabaseHelper()
{

}

int SalesInventoryItemDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

int SalesInventoryItemDatabaseHelper::insertSalesInventoryItem(CompoundItemDataObject *obj,ItemImageDatamodel image)
{
    this->startTransaction();
    qDebug()<<"Insert Item Started";

    //
    qDebug()<<obj->BaseItem.ItemID<<LoginValues::voucherPrefix;
    if(obj->BaseItem.ItemID==""){
        obj->BaseItem.ItemID = obj->BaseItem.GroupID+DatabaseValues::idGen+getMaxID();
    }
    else{

    }
    image.itemID = obj->BaseItem.ItemID ;
    QMap<QString,QVariant> map;
    map.insert(Sales_Inventory_ItemId,obj->BaseItem.ItemID);
    map.insert(Sales_Inventory_ItemName,obj->BaseItem.ItemName);
    map.insert(Sales_Inventory_ItemName_Arabic,obj->BaseItem.itemNameArabic);
    map.insert(Sales_Inventory_GroupId,obj->BaseItem.GroupID);
    map.insert(Sales_Inventory_ItemAlias,obj->BaseItem.ItemAlias);
    map.insert(Sales_Inventory_ItemCode,obj->BaseItem.ItemCode);
    map.insert(Sales_Inventory_Narration,obj->BaseItem.narration);
    map.insert(Sales_Inventory_StdCost,obj->BaseItem.stdCost);
    map.insert(Sales_Inventory_PartNumber,obj->BaseItem.PartNumber);
    map.insert(Sales_Inventory_SerailNumber,obj->BaseItem.SerailNumber);
    map.insert(Sales_Inventory_OpeningStock,QString::number(obj->BaseItem.OpeningStock,'f',2));
    map.insert(Sales_Inventory_OpeningRate,QString::number(obj->BaseItem.OpeningStockPrice,'f',2));
    map.insert(Sales_Inventory_OpeningBalanceDate,obj->BaseItem.OpeningStockDate);
    map.insert(Sales_Inventory_OpeningValue,QString::number(obj->BaseItem.OpeningStockValue,'f',2));
    map.insert(Sales_Inventory_ClosingStock,QString::number(obj->BaseItem.ClosingStock));
    map.insert(Sales_Inventory_ReorderLevel,QString::number(obj->BaseItem.ReorderLevel,'f',2));
    map.insert(Sales_Inventory_defaultSalesLedgerID,obj->BaseItem.defaultSalesLedgerID);
    map.insert(Sales_Inventory_defaultPurchaseLedgerID,obj->BaseItem.defaultPurchaseLedgerID);
    map.insert(Sales_Inventory_defaultInputTaxLedgerID,obj->BaseItem.DefaultInputTaxLedgerID);
    map.insert(Sales_Inventory_defaultOutputTaxLedgerID,obj->BaseItem.DefaultOutputTaxLedgerID);
    map.insert(Sales_Inventory_defaultSalesReturnLedgerID,obj->BaseItem.DefaultSalesReturnLedgerID);
    map.insert(Sales_Inventory_defaultPurchaseReturnLedgerID,obj->BaseItem.DefaultPurchaseReturnLedgerID);

    map.insert(Sales_Inventory_Favourite,obj->BaseItem.favo);

    map.insert(Sales_Inventory_VatRate,QString::number(obj->BaseItem.taxRate,'f',2));
    map.insert(Sales_Inventory_defaultUOM,obj->BaseItem.defaultUOMID);
    map.insert(Sales_Inventory_ShelfLife,QString::number(obj->BaseItem.shelfLife,'f',2));
    map.insert(Sales_Inventory_WarrantyDays,QString::number(obj->BaseItem.warrantyDays));
    map.insert(Sales_Inventory_Price,QString::number(obj->BaseItem.price,'f',2));
    map.insert(Sales_Inventory_Price2,QString::number(obj->BaseItem.price_2,'f',2));
    map.insert(Sales_Inventory_BrandID,obj->BaseItem.brandID);
    map.insert(Sales_Inventory_Description,obj->BaseItem.ItemDescription);
    map.insert(Sales_Inventory_isCustomItem,obj->BaseItem.isCustomItem);
    map.insert(Sales_Inventory_Dimension,obj->BaseItem.Dimension);
    map.insert(Sales_Inventory_isPurchaseItem,obj->BaseItem.isPurchaseItem);
    map.insert(Sales_Inventory_isSalesItem,obj->BaseItem.isSalesItem);
    map.insert(Sales_Inventory_IsStockItem,obj->BaseItem.isStockItem);
    map.insert(Sales_Inventory_hideInBill,obj->BaseItem.hideInPrint);


    map.insert(Sales_Inventory_KOT_Printer,obj->BaseItem.KOTPrinter);
    map.insert(Sales_Inventory_From_Time,obj->BaseItem.fromTime);
    map.insert(Sales_Inventory_To_Time,obj->BaseItem.toTime);
    map.insert(Sales_Inventory_HSN_CODE,obj->BaseItem.hsnCode);
    map.insert(Sales_Inventory_Section, obj->BaseItem.section);
    map.insert(Sales_Inventory_Flags, obj->BaseItem.flags );
    map.insert(Sales_Inventory_Category, obj->BaseItem.Category);
    map.insert(Sales_Inventory_isSerailNumbered,obj->BaseItem.isSerailNumbered);
    map.insert(Sales_Inventory_isBatchProcessed,obj->BaseItem.isBatchProcessed);
    map.insert(Sales_Inventory_lastModified,QDateTime::currentDateTime());

    db->insertRecordsPlus(map,Sales_Inventory_Table_Name);


    ItemTransactionDatabaseHelper *itemTransaction = new ItemTransactionDatabaseHelper(db);
    ItemTransactionDataModel *ItemModel = new ItemTransactionDataModel;

    ItemModel->ItemID = obj->BaseItem.ItemID;
    //    ItemModel->From_Ledger = obj->BaseItem.defaultSalesLedgerID;
    //    ItemModel->To_Ledger = obj->ledgerObject.LedgerID;
    ItemModel->VoucherID = "0";
    ItemModel->VoucherPrefix = LoginValues::voucherPrefix;
    ItemModel->VoucherDate = obj->BaseItem.OpeningStockDate;
    ItemModel->VoucherType = "OPENING STOCK";
    ItemModel->From_Location = "OUT";
    //    ItemModel->To_Location = "Godown";
    ItemModel->To_Location = obj->BaseItem.section;
    if(obj->BaseItem.OpeningStock >0){
        ItemModel->drQty = obj->BaseItem.OpeningStock;
        ItemModel->drAmount = obj->BaseItem.OpeningStockValue;
        ItemModel->crQty = 0;
        ItemModel->Price = obj->BaseItem.OpeningStockValue/obj->BaseItem.OpeningStock;
    }
    else if(obj->BaseItem.OpeningStock <0){
        ItemModel->crQty = -1*obj->BaseItem.OpeningStock;
        ItemModel->drAmount = -1* obj->BaseItem.OpeningStockValue;
        ItemModel->drQty = 0;
        ItemModel->Price = obj->BaseItem.OpeningStockValue/obj->BaseItem.OpeningStock;
    }
    ItemModel->itemReqId = "X";
    ItemModel->Narration = " Opening Stock";

    itemTransaction->insertItemTransaction(ItemModel, obj->BaseItem.godownList);



    for(PropertyDataModel dat:obj->BaseItem.baseProperties){
        map.clear();
        // map.insert(Item_Properties_id,dat.itemProperty_ID);
        map.insert(Item_Properties_Item_ID,obj->BaseItem.ItemID);
        map.insert(Item_Properties_PropertyID,dat.ItemProperty_PropertyID);

        db->insertRecordsPlus(map,Item_Properties_TableName);
    }
    if(image.imageFilename.size()>0){
        InventoryItemImagesDatabaseHelper *imageHelper = new InventoryItemImagesDatabaseHelper;
        imageHelper->insertItemImage(image);
    }
    return commit();
}

int SalesInventoryItemDatabaseHelper::updateSalesInventoryItem(CompoundItemDataObject *obj,QString oldID,ItemImageDatamodel image)
{

    this->startTransaction();

    db = new DatabaseHelper();
    QMap<QString,QVariant> map;
    map.insert(Sales_Inventory_ItemName,obj->BaseItem.ItemName);
    map.insert(Sales_Inventory_ItemName_Arabic,obj->BaseItem.itemNameArabic);
    map.insert(Sales_Inventory_GroupId,obj->BaseItem.GroupID);
    map.insert(Sales_Inventory_ItemAlias,obj->BaseItem.ItemAlias);
    map.insert(Sales_Inventory_ItemCode,obj->BaseItem.ItemCode);
    map.insert(Sales_Inventory_Narration,obj->BaseItem.narration);
    map.insert(Sales_Inventory_StdCost,obj->BaseItem.stdCost);
    map.insert(Sales_Inventory_PartNumber,obj->BaseItem.PartNumber);
    map.insert(Sales_Inventory_SerailNumber,obj->BaseItem.SerailNumber);
    map.insert(Sales_Inventory_OpeningStock,QString::number(obj->BaseItem.OpeningStock,'f',2));
    map.insert(Sales_Inventory_OpeningRate,QString::number(obj->BaseItem.OpeningStockPrice,'f',2));
    //    map.insert(Sales_Inventory_OpeningBalanceDate,obj->BaseItem.OpeningStockDate);
    map.insert(Sales_Inventory_OpeningValue,QString::number(obj->BaseItem.OpeningStockValue,'f',2));
    map.insert(Sales_Inventory_ClosingStock,QString::number(obj->BaseItem.ClosingStock,'f',2));
    map.insert(Sales_Inventory_ReorderLevel,QString::number(obj->BaseItem.ReorderLevel,'f',2));
    map.insert(Sales_Inventory_defaultSalesLedgerID,obj->BaseItem.defaultSalesLedgerID);
    map.insert(Sales_Inventory_defaultPurchaseLedgerID,obj->BaseItem.defaultPurchaseLedgerID);
    map.insert(Sales_Inventory_defaultInputTaxLedgerID,obj->BaseItem.DefaultInputTaxLedgerID);
    map.insert(Sales_Inventory_defaultOutputTaxLedgerID,obj->BaseItem.DefaultOutputTaxLedgerID);
    map.insert(Sales_Inventory_defaultSalesReturnLedgerID,obj->BaseItem.DefaultSalesReturnLedgerID);
    map.insert(Sales_Inventory_defaultPurchaseReturnLedgerID,obj->BaseItem.DefaultPurchaseReturnLedgerID);
    map.insert(Sales_Inventory_VatRate,QString::number(obj->BaseItem.taxRate,'f',2));
    map.insert(Sales_Inventory_defaultUOM,obj->BaseItem.defaultUOMID);
    map.insert(Sales_Inventory_ShelfLife,QString::number(obj->BaseItem.shelfLife,'f',2));
    map.insert(Sales_Inventory_WarrantyDays,QString::number(obj->BaseItem.warrantyDays));
    map.insert(Sales_Inventory_Price,QString::number(obj->BaseItem.price,'f',2));
    map.insert(Sales_Inventory_Price2,QString::number(obj->BaseItem.price_2,'f',2));

    map.insert(Sales_Inventory_BrandID,obj->BaseItem.brandID);
    map.insert(Sales_Inventory_Description,obj->BaseItem.ItemDescription);
    map.insert(Sales_Inventory_isCustomItem,obj->BaseItem.isCustomItem);
    map.insert(Sales_Inventory_Dimension,obj->BaseItem.Dimension);
    map.insert(Sales_Inventory_isPurchaseItem,obj->BaseItem.isPurchaseItem);
    map.insert(Sales_Inventory_isSalesItem,obj->BaseItem.isSalesItem);
    map.insert(Sales_Inventory_IsStockItem,obj->BaseItem.isStockItem);
    map.insert(Sales_Inventory_KOT_Printer,obj->BaseItem.KOTPrinter);
    map.insert(Sales_Inventory_hideInBill,obj->BaseItem.hideInPrint);

    map.insert(Sales_Inventory_Favourite,obj->BaseItem.favo);

    map.insert(Sales_Inventory_From_Time,obj->BaseItem.fromTime);
    map.insert(Sales_Inventory_To_Time,obj->BaseItem.toTime);
    map.insert(Sales_Inventory_HSN_CODE,obj->BaseItem.hsnCode);
    qDebug()<<"selected FLAGS"<<obj->BaseItem.flags;
    map.insert(Sales_Inventory_Section, obj->BaseItem.section);

    QJsonDocument doc(obj->BaseItem.flags);
    QString flagsString(doc.toJson(QJsonDocument::Compact));
    map.insert(Sales_Inventory_Flags, flagsString );
    map.insert(Sales_Inventory_Category, obj->BaseItem.Category);
    map.insert(Sales_Inventory_isSerailNumbered,obj->BaseItem.isSerailNumbered);
    map.insert(Sales_Inventory_isBatchProcessed,obj->BaseItem.isBatchProcessed);
    map.insert(Sales_Inventory_lastModified,QDateTime::currentDateTime());
    QMap<QString,QVariant> clause;
    qDebug()<<"OLD ID"<<oldID;
    clause.insert(Sales_Inventory_ItemId,oldID);
    db->updateRecordsPlus(map,clause,Sales_Inventory_Table_Name);


    ItemTransactionDatabaseHelper *itemTransaction = new ItemTransactionDatabaseHelper(db);
    ItemTransactionDataModel *ItemModel = new ItemTransactionDataModel;

    ItemModel->ItemID = obj->BaseItem.ItemID;
    //    ItemModel->From_Ledger = obj->BaseItem.defaultSalesLedgerID;
    //    ItemModel->To_Ledger = obj->ledgerObject.LedgerID;
    ItemModel->VoucherID = "0";
    ItemModel->VoucherPrefix = LoginValues::voucherPrefix;
    ItemModel->VoucherDate = obj->BaseItem.OpeningStockDate;
    ItemModel->VoucherType = "OPENING STOCK";
    ItemModel->From_Location = "OUT";
    //    ItemModel->To_Location = "Godown";
    ItemModel->To_Location = obj->BaseItem.section;
    if(obj->BaseItem.OpeningStock >0){
        ItemModel->drQty = obj->BaseItem.OpeningStock;
        ItemModel->drAmount = obj->BaseItem.OpeningStockValue;
        ItemModel->crQty = 0;
        ItemModel->Price = obj->BaseItem.OpeningStockValue/obj->BaseItem.OpeningStock;
    }
    else if(obj->BaseItem.OpeningStock < 0) {
        ItemModel->crQty = -1 * obj->BaseItem.OpeningStock;
        ItemModel->crAmount = -1 * obj->BaseItem.OpeningStockValue;
        ItemModel->drQty = 0;
        ItemModel->Price = obj->BaseItem.OpeningStockValue/obj->BaseItem.OpeningStock;
    }
    ItemModel->itemReqId = "X";
    ItemModel->Narration = " Opening Stock";

    itemTransaction->insertItemTransaction(ItemModel, obj->BaseItem.godownList);



    QMap<QString,QString> cl;
    cl.insert(Item_Properties_Item_ID,oldID);

    db->deleteRecords(cl,Item_Properties_TableName);
    qDebug()<<"Updating properties of "<<obj->BaseItem.ItemID;
    for(PropertyDataModel dat:obj->BaseItem.baseProperties){
        map.clear();
        // map.insert(Item_Properties_id,dat.itemProperty_ID);
        map.insert(Item_Properties_Item_ID,obj->BaseItem.ItemID);
        map.insert(Item_Properties_PropertyID,dat.ItemProperty_PropertyID);
        db->insertRecordsPlus(map,Item_Properties_TableName);
    }

    //    if(image.imageFilename.length()>2){
    InventoryItemImagesDatabaseHelper *imageHelper = new InventoryItemImagesDatabaseHelper;
    imageHelper->updateItemImage(image);
    //    }

    return commit();
}

int SalesInventoryItemDatabaseHelper::deleteSalesInventoryItem(QString id)
{
    this->startTransaction();
    //    QSqlQuery q(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "select count(*)  from transaction_item_helper where itemid='"+id+"'  and voucherType not like 'OPENING STOCK'";
    int n = DatabaseHelper::getSingletonQueryResult(qStr).toInt();
    //    q.exec("select count(*)  from transaction_item_helper where itemid='"+id+"'  and voucherType not like 'OPENING STOCK'  ");
    if(n>0){
        qDebug()<<qStr;
        QMessageBox box;box.setText("Inventory in Use.\nCannot Delete Item!!!");
        box.setWindowFlag(Qt::WindowStaysOnTopHint); box.exec();
        return 0;
    }

    QMap<QString,QString> clause;

    clause.insert(Sales_Inventory_ItemId,id);
    db->deleteRecords(clause,Sales_Inventory_Table_Name);

    return commit();
}

int SalesInventoryItemDatabaseHelper::commit()
{
    if(flag){
        if(db->commitTransaction())
            return true;
        else
            return false;
    }
    else{
        db->rollBackTransaction();
        return 0;
    }
}

QString SalesInventoryItemDatabaseHelper::getMaxID()
{
    QSqlDatabase db = QSqlDatabase::database(DatabaseValues::connectionString);
    QSqlQuery query(db);
    int id =0;
    QString qStr = "select max(_id) from "+ Sales_Inventory_Table_Name;
    query.exec(qStr);
    while(query.next()){
        id = query.value(0).toInt();
    }
    return QString::number(DatabaseHelper::getSingletonQueryResult(qStr).toInt()+1);

}

//QList<inventoryItemDataModel> SalesInventoryItemDatabaseHelper::getALLInventoryObjects()
//{
//    QList<inventoryItemDataModel> data;

//    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
//    LedgerMasterDatabaseHelper* ledgerHelper= new LedgerMasterDatabaseHelper();
//    UomDataBaseHelper* uomHelper = new UomDataBaseHelper();
//    BrandsDatabaseHelper* brandHelper = new BrandsDatabaseHelper();

//    QString qStr="SELECT "
//                 ""+Sales_Inventory_ItemId
//            +", "+Sales_Inventory_ItemName
//            +", "+Sales_Inventory_GroupId
//            +", "+Sales_Inventory_OpeningStock
//            +", "+Sales_Inventory_ClosingStock
//            +", "+Sales_Inventory_PartNumber
//            +", "+Sales_Inventory_VatRate
//            +", "+Sales_Inventory_ItemAlias
//            +", "+Sales_Inventory_Narration
//            +", "+Sales_Inventory_ReorderLevel
//            +", "+Sales_Inventory_OpeningRate
//            +", "+Sales_Inventory_defaultUOM
//            +", "+Sales_Inventory_defaultSalesLedgerID
//            +", "+Sales_Inventory_defaultPurchaseLedgerID
//            +", "+Sales_Inventory_ItemCode
//            +", "+Sales_Inventory_OpeningBalanceDate
//            +", "+Sales_Inventory_ShelfLife
//            +", "+Sales_Inventory_WarrantyDays
//            +", "+Sales_Inventory_Price
//            +", "+Sales_Inventory_OpeningValue
//            +", "+Sales_Inventory_BrandID
//            +", "+Sales_Inventory_Description
//            +", "+Sales_Inventory_isCustomItem
//            +", "+Sales_Inventory_Dimension
//            +", "+Sales_Inventory_isPurchaseItem
//            +", "+Sales_Inventory_isSalesItem
//            +", "+Sales_Inventory_KOT_Printer
//            +", "+Sales_Inventory_defaultInputTaxLedgerID
//            +", "+Sales_Inventory_defaultOutputTaxLedgerID
//            +", "+Sales_Inventory_defaultSalesReturnLedgerID
//            +", "+Sales_Inventory_defaultPurchaseReturnLedgerID
//            +", "+Sales_Inventory_ItemName_Arabic
//            +", "+Sales_Inventory_From_Time
//            +", "+Sales_Inventory_To_Time
//            +", "+Sales_Inventory_SerailNumber




//            +" FROM "+ Sales_Inventory_Table_Name
//            +" ORDER BY "+Sales_Inventory_ItemName;
//    if(query.exec(qStr)){

//        SalesInventoryGroupsDatabaseHelper* gpHelper = new SalesInventoryGroupsDatabaseHelper();
//        while(query.next()){
//            inventoryItemDataModel obj;
//            obj.ItemID = query.value(0).toString();
//            obj.ItemName = query.value(1).toString();
//            obj.GroupName = gpHelper->getGroupNameByID(query.value(2).toString());
//            obj.GroupID = query.value(2).toString();
//            obj.OpeningStock = query.value(3).toFloat();
//            obj.ClosingStock = query.value(4).toFloat();
//            obj.PartNumber = query.value(5).toString();
//            obj.vatRate = query.value(6).toFloat();
//            obj.ItemAlias = query.value(7).toString();
//            obj.narration = query.value(8).toString();
//            obj.ReorderLevel = query.value(9).toFloat();
//            obj.OpeningStockPrice = query.value(10).toFloat();
//            obj.defaultUOMID = query.value(11).toString();
//            obj.defaultSalesLedgerID =  query.value(12).toString();
//            obj.defaultPurchaseLedgerID =  query.value(13).toString();
//            obj.ItemCode = query.value(14).toString();
//            obj.OpeningStockDate = query.value(15).toDate();
//            obj.shelfLife = query.value(16).toFloat();
//            obj.warrantyDays = query.value(17).toInt();
//            obj.price = query.value(18).toFloat();
//            obj.OpeningStockValue = query.value(19).toFloat();
//            obj.uomObject.UomName = uomHelper ->getUOMNameById(obj.defaultUOMID);
//            obj.brandID = query.value(20).toString();
//            obj.brandName = brandHelper->getBrandNameByID(obj.brandID);
//            obj.ItemDescription=query.value(21).toString();
//            obj.isCustomItem = query.value(22).toBool();
//            obj.Dimension = query.value(23).toString();
//            obj.isPurchaseItem = query.value(24).toBool();
//            obj.isSalesItem = query.value(25).toBool();
//            obj.KOTPrinter = query.value(26).toString();
//            obj.DefaultInputTaxLedgerID = query.value(27).toString();
//            obj.DefaultOutputTaxLedgerID = query.value(28).toString();
//            obj.DefaultSalesReturnLedgerID = query.value(29).toString();
//            obj.DefaultPurchaseReturnLedgerID = query.value(30).toString();
//            obj.itemNameArabic = query.value(31).toString().toUtf8();
//            obj.fromTime = query.value(32).toTime();
//            obj.toTime  = query.value(33).toTime();
//            obj.SerailNumber = query.value(34).toString();

//            //qDebug()<<"Property name : "<<obj.brandName;
//            data.append(obj);
//        }
//    }
//    else{
//        qDebug()<<query.lastError();
//        qDebug()<<query.lastQuery();
//    }
//    qDebug()<<"ret inv datalist";
//    return data;
//}

QList<QSharedPointer<inventoryItemDataModel>> SalesInventoryItemDatabaseHelper::getALLInventoryObjectsAsPtr(int filter)
{
    QList<QSharedPointer<inventoryItemDataModel>> data;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    LedgerMasterDatabaseHelper* ledgerHelper= new LedgerMasterDatabaseHelper();
    UomDataBaseHelper* uomHelper = new UomDataBaseHelper();
    SalesInventoryGroupsDatabaseHelper* gpHelper = new SalesInventoryGroupsDatabaseHelper();
    BrandsDatabaseHelper* brandHelper = new BrandsDatabaseHelper();

    QString whereClause = " WHERE 1";
    if(filter == 1){
        whereClause = " WHERE "+Sales_Inventory_isSalesItem +"= 1 ";
    }
    else if(filter == 2){
        whereClause = " WHERE "+Sales_Inventory_isPurchaseItem +"= 1 ";
    }

    QString qStr="SELECT "
                   ""+Sales_Inventory_ItemId //0
                   +", "+Sales_Inventory_ItemName
                   +", "+Sales_Inventory_GroupId
                   +", "+Sales_Inventory_OpeningStock
                   +", "+Sales_Inventory_ClosingStock
                   +", "+Sales_Inventory_PartNumber
                   +", "+Sales_Inventory_VatRate
                   +", "+Sales_Inventory_ItemAlias
                   +", "+Sales_Inventory_Narration
                   +", "+Sales_Inventory_ReorderLevel
                   +", "+Sales_Inventory_OpeningRate//10
                   +", "+Sales_Inventory_defaultUOM
                   +", "+Sales_Inventory_defaultSalesLedgerID
                   +", "+Sales_Inventory_defaultPurchaseLedgerID
                   +", "+Sales_Inventory_ItemCode
                   +", "+Sales_Inventory_OpeningBalanceDate
                   +", "+Sales_Inventory_ShelfLife
                   +", "+Sales_Inventory_WarrantyDays
                   +", "+Sales_Inventory_Price
                   +", "+Sales_Inventory_OpeningValue
                   +", "+Sales_Inventory_BrandID
                   +", "+Sales_Inventory_Description //21


                   +", "+ "(SELECT Group_Name from "+gpHelper->Sales_Inventory_Group_TableName
                   + " WHERE "+ gpHelper->Sales_Inventory_Group_GroupId+" = INV."+gpHelper->Sales_Inventory_Group_GroupId
                   + ") as Group_Name"
                   + ", "
                   + "(Select "+ ledgerHelper->Ledger_Master_LedgerName + " from "+ledgerHelper->Ledger_Master_TableName
                   + " WHERE "+ ledgerHelper->Ledger_Master_LedgerID +"= INV."
                   + Sales_Inventory_defaultSalesLedgerID
                   + ") AS SalesLedgerName"
                   + ", "
                   + "(Select "+ ledgerHelper->Ledger_Master_LedgerName + " from "+ledgerHelper->Ledger_Master_TableName
                   + " WHERE "+ ledgerHelper->Ledger_Master_LedgerID +"= INV."
                   + Sales_Inventory_defaultPurchaseLedgerID
                   + ") AS SalesLedgerName"

                   + ", "
                   + "(Select "+ uomHelper->UOM_UOMName + " from "+uomHelper->UOM_TableName
                   + " WHERE "+ uomHelper->UOM_UOMID +"= INV. "
                   + Sales_Inventory_defaultUOM
                   + ") AS UOMnaME"

                   + ", "
                   + "(Select "+ brandHelper->brandName+ " from "+brandHelper->brandTableName
                   + " WHERE "+ brandHelper->brandID +"= INV."
                   + Sales_Inventory_BrandID
                   + ") AS brandname"   //26
                   +", "+Sales_Inventory_isCustomItem
                   +", "+Sales_Inventory_Dimension
                   +", "+Sales_Inventory_isPurchaseItem
                   +", "+Sales_Inventory_isSalesItem
                   +", "+Sales_Inventory_KOT_Printer  //31
                   +", "+Sales_Inventory_defaultInputTaxLedgerID
                   +", "+Sales_Inventory_defaultOutputTaxLedgerID
                   +", "+Sales_Inventory_defaultSalesReturnLedgerID
                   +", "+Sales_Inventory_defaultPurchaseReturnLedgerID
                   +", "+Sales_Inventory_ItemName_Arabic//36

                   +", "+Sales_Inventory_OpeningRate
                   +", "+Sales_Inventory_From_Time
                   +", "+Sales_Inventory_To_Time//39
                   +", "+Sales_Inventory_SerailNumber
                   + ", "
                   + "(Select "+ uomHelper->UOM_UOMSymbol + " from "+uomHelper->UOM_TableName
                   + " WHERE "+ uomHelper->UOM_UOMID +"= INV. "
                   + Sales_Inventory_defaultUOM
                   + ") AS UOMSym, "
                   + "(Select "+ uomHelper->UOM_DecimalPoints + " from "+uomHelper->UOM_TableName
                   + " WHERE "+ uomHelper->UOM_UOMID +"= INV. "
                   + Sales_Inventory_defaultUOM
                   + ") AS UOMDec"
                   + ","
                   + Sales_Inventory_HSN_CODE+", "
                   + Sales_Inventory_IsStockItem +", "
                   + Sales_Inventory_isSerailNumbered +", "
                   + Sales_Inventory_isBatchProcessed
                   + " FROM "+ Sales_Inventory_Table_Name
                   + " INV"
                   + whereClause
                   + " ORDER BY " + Sales_Inventory_ItemName;

    int i=1;

    query.setForwardOnly(true);

    if(query.exec(qStr)){
        inventoryItemDataModel *obj;
        while(query.next()){
            obj = new inventoryItemDataModel();
            obj->ItemID = query.value(0).toString();
            obj->ItemName = query.value(1).toString();
            obj->GroupID = query.value(2).toString();
            obj->OpeningStock = query.value(3).toFloat();
            obj->ClosingStock = query.value(4).toFloat();
            obj->PartNumber = query.value(5).toString();
            obj->taxRate = query.value(6).toFloat();
            obj->ItemAlias = query.value(7).toString();
            obj->narration = query.value(8).toString();
            obj->ReorderLevel = query.value(9).toFloat();
            obj->OpeningStockPrice = query.value(10).toFloat();
            obj->defaultUOMID = query.value(11).toString();
            obj->uomObject._id = obj->defaultUOMID;
            obj->defaultSalesLedgerID =query.value(12).toString();
            obj->defaultPurchaseLedgerID =query.value(13).toString();
            obj->ItemCode = query.value(14).toString();
            obj->OpeningStockDate = query.value(15).toDate();
            obj->shelfLife = query.value(16).toFloat();
            obj->warrantyDays = query.value(17).toInt();
            obj->price = query.value(18).toFloat();
            obj->OpeningStockValue = query.value(19).toFloat();
            obj->brandID = query.value(20).toInt();
            obj->ItemDescription=query.value(21).toString();
            obj->GroupName =query.value(22).toString();
            obj->uomObject.UomName = query.value(25).toString();
            obj->brandName = query.value(26).toString();
            obj->isCustomItem = query.value(27).toBool();
            obj->Dimension = query.value(28).toString();
            obj->isPurchaseItem = query.value(29).toBool();
            obj->isSalesItem = query.value(30).toBool();
            obj->KOTPrinter = query.value(31).toString();
            obj->DefaultInputTaxLedgerID = query.value(32).toString();
            obj->DefaultOutputTaxLedgerID = query.value(33).toString();
            obj->DefaultSalesReturnLedgerID = query.value(34).toString();
            obj->DefaultPurchaseReturnLedgerID = query.value(35).toString();
            obj->OpeningStockPrice = query.value(37).toFloat();
            obj->itemNameArabic = query.value(36).toString().toUtf8();
            query.value(38).toFloat();
            obj->fromTime = query.value(38).toTime();
            obj->toTime = query.value(39).toTime();
            obj->SerailNumber = query.value(40).toString();
            obj->uomObject.UomSymbol = query.value(41).toString();
            obj->uomObject.decimalPoints = query.value(42).toInt();
            obj->hsnCode = query.value(43).toString();
            obj->isStockItem = query.value(44).toBool();
            obj->isSerailNumbered = query.value(45).toBool();
            obj->isBatchProcessed = query.value(46).toBool();



            //qDebug()<<"Property name : "<<obj->brandName;
            data.push_back(QSharedPointer<inventoryItemDataModel>(obj));

            i++;
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return data;
}



float SalesInventoryItemDatabaseHelper::getPrice(QString id)
{
    float name = 0;
    QString qStr = "SELECT "+Sales_Inventory_Price + " FROM " + Sales_Inventory_Table_Name
                   + " WHERE "+Sales_Inventory_ItemId + "=:id";
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);
    query.bindValue(":id",id);

    if(query.exec()){
        while (query.next()) {
            name =  query.value(0).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery(); qDebug()<<"219";
    }
    return name;
}

void SalesInventoryItemDatabaseHelper::updateKOTPrinter(QString itemID, QString kotprinter)
{
    QMap<QString,QVariant> map;
    QMap<QString,QVariant> clause;
    clause.insert(Sales_Inventory_ItemId,itemID);

    map.insert(Sales_Inventory_KOT_Printer,kotprinter);
    //    startTransaction();
    db->updateRecordsPlus(map,clause,Sales_Inventory_Table_Name);
    //    commit();
}

void SalesInventoryItemDatabaseHelper::updateOpening(QString itemID, float openingStk, float openingRate, float openingValue)
{
    QMap<QString,QVariant> map;
    QMap<QString,QVariant> clause;
    clause.insert(Sales_Inventory_ItemId,itemID);
    map.insert(Sales_Inventory_OpeningStock,openingStk);
    map.insert(Sales_Inventory_OpeningRate,openingRate);
    map.insert(Sales_Inventory_OpeningValue,openingValue);
    startTransaction();
    db->updateRecordsPlus(map,clause,Sales_Inventory_Table_Name);
    commit();
}

void SalesInventoryItemDatabaseHelper::updatePrice(QString itemID, float Price)
{

    QMap<QString,QVariant> map;
    QMap<QString,QVariant> clause;
    clause.insert(Sales_Inventory_ItemId,itemID);
    map.insert(Sales_Inventory_Price,Price);
    startTransaction();
    db->updateRecordsPlus(map,clause,Sales_Inventory_Table_Name);
    commit();

}

void SalesInventoryItemDatabaseHelper::updateGST(QString itemID, float taxRate)
{
    QMap<QString,QVariant> map;
    QMap<QString,QVariant> clause;
    clause.insert(Sales_Inventory_ItemId,itemID);
    map.insert(Sales_Inventory_VatRate,taxRate);
    startTransaction();
    db->updateRecordsPlus(map,clause,Sales_Inventory_Table_Name);
    commit();
}

QSqlQueryModel* SalesInventoryItemDatabaseHelper::getInventoryItemsQueryModelForScale(int filter)
{

    LedgerMasterDatabaseHelper* ledgerHelper= new LedgerMasterDatabaseHelper();
    UomDataBaseHelper* uomHelper = new UomDataBaseHelper();
    SalesInventoryGroupsDatabaseHelper* gpHelper = new SalesInventoryGroupsDatabaseHelper();
    BrandsDatabaseHelper* brandHelper = new BrandsDatabaseHelper();
    //    ItemTransactionDatabaseHelper itemHelper;

    QString whereClause = " WHERE length("+Sales_Inventory_SerailNumber +")>0 ";
    if(filter == 1){
        whereClause = " WHERE length("+Sales_Inventory_SerailNumber +") >0 ";
    }
    else if(filter == 2){
        whereClause = " WHERE "+Sales_Inventory_isPurchaseItem +"= 1 ";
    }

    QString qStr="SELECT "
                   ""+Sales_Inventory_ItemId
                   +", "+Sales_Inventory_ItemName      //1
                   +", "+Sales_Inventory_ItemCode      //2
                   +", "+Sales_Inventory_Price         //3
                   +", "+Sales_Inventory_SerailNumber  //4
                   +", "
                   + "(SELECT Group_Name from "+gpHelper->Sales_Inventory_Group_TableName
                   + " WHERE "+ gpHelper->Sales_Inventory_Group_GroupId+" = INV."+gpHelper->Sales_Inventory_Group_GroupId
                   + ") as Group_Name"                 //5

                   + ", "
                   + "(Select "+ uomHelper->UOM_UOMSymbol + " from "+uomHelper->UOM_TableName
                   + " WHERE "+ uomHelper->UOM_UOMID +"= INV. "
                   + Sales_Inventory_defaultUOM        //6
                   + ") AS UOMSymbol"

                   + " FROM "+ Sales_Inventory_Table_Name
                   + " INV"
                   + whereClause
                   + " ORDER BY cast("+Sales_Inventory_SerailNumber +" as Integer)";

    return DatabaseHelper::getQueryModel(qStr);

}



QString SalesInventoryItemDatabaseHelper::getInventoryItemNameByID(QString id)
{
    QString name = "";
    QString qStr = "SELECT "+Sales_Inventory_ItemName + " FROM " + Sales_Inventory_Table_Name
                   + " WHERE "+Sales_Inventory_ItemId + "='"+id+"'";
    name = DatabaseHelper::getSingletonQueryResult(qStr).toString();
    return name;
}

QString SalesInventoryItemDatabaseHelper::getInventoryGroupNameByItemID(QString id)
{
    SalesInventoryGroupsDatabaseHelper gpHelper;
    QString name = "";

    QString qStr = "SELECT "+Sales_Inventory_GroupId + " FROM " + Sales_Inventory_Table_Name
                   + " WHERE "+Sales_Inventory_ItemId + "='"+id+"'";
    name = DatabaseHelper::getSingletonQueryResult(qStr).toString();
    return name;
}

bool SalesInventoryItemDatabaseHelper::checkItemExists(QString itemID)
{
    QString qStr = " SELECT 1 FROM " + Sales_Inventory_Table_Name + " WHERE " + Sales_Inventory_ItemId + " = '" + itemID + "' ";

    if(db->getSingletonQueryResult(qStr).toInt() == 1)
        return true;
    else
        return false;

}


bool SalesInventoryItemDatabaseHelper::barcodeExist(QString barcode)
{
    bool name = false;
    QString qStr = "SELECT "+Sales_Inventory_ItemCode + " FROM " + Sales_Inventory_Table_Name
                   + " WHERE "+Sales_Inventory_ItemCode + "=:id";
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);
    query.bindValue(":id",barcode);

    if(query.exec()){
        if(query.size()>0){
            name = true;
        }

    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return name;
}

QList<PropertyDataModel> SalesInventoryItemDatabaseHelper::getAllPropertiesOfItem(QString itemID)
{
    QString qStr ="SELECT "+ Item_Properties_PropertyID + " FROM "+ Item_Properties_TableName
                   + " WHERE "+ Item_Properties_Item_ID + "=:id";
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    QList<PropertyDataModel> data;
    PropertyDatabaseHelper *propHelper = new PropertyDatabaseHelper();

    query.prepare(qStr);
    query.bindValue(":id",itemID);

    if(query.exec()){
        while(query.next()){
            PropertyDataModel obj;
            obj.PropertyID=query.value(0).toInt();
            obj.PropertyName=propHelper->getPropertyNameByID(query.value(0).toString());

            data.append(obj);
        }
    }
    else{
        qDebug()<<query.lastError();
    }
    return data;
}

QSqlQueryModel* SalesInventoryItemDatabaseHelper::getInventoryItemsQueryModel(int filter)
{
    QSqlQueryModel *query = new QSqlQueryModel();


    LedgerMasterDatabaseHelper* ledgerHelper= new LedgerMasterDatabaseHelper();
    UomDataBaseHelper* uomHelper = new UomDataBaseHelper();
    SalesInventoryGroupsDatabaseHelper* gpHelper = new SalesInventoryGroupsDatabaseHelper();
    BrandsDatabaseHelper* brandHelper = new BrandsDatabaseHelper();
    //    ItemTransactionDatabaseHelper itemHelper;

    QString whereClause = " WHERE 1";
    if(filter == 1){
        whereClause = " WHERE "+Sales_Inventory_isSalesItem +"= 1 ";
    }
    else if(filter == 2){
        whereClause = " WHERE "+Sales_Inventory_isPurchaseItem +"= 1 ";
    }
    else if(filter == 3){
        whereClause = " WHERE "+Sales_Inventory_IsStockItem +"= 1 ";
    }

    QString qStr="SELECT "
                   ""+Sales_Inventory_ItemId +" as 'ItemID' "
                   +", "+Sales_Inventory_ItemName+" as 'Item' "
                   //            +", "+Sales_Inventory_GroupId
                   //            +", "+Sales_Inventory_OpeningStock
                   //            +", "+Sales_Inventory_ClosingStock
                   //            +", "+Sales_Inventory_PartNumber
                   //           +", "+Sales_Inventory_VatRate
                   //            +", "+Sales_Inventory_ItemAlias
                   //            +", "+Sales_Inventory_Narration
                   //            +", "+Sales_Inventory_ReorderLevel
                   //            +", "+Sales_Inventory_OpeningRate
                   //            +", "+Sales_Inventory_defaultUOM
                   //            +", "+Sales_Inventory_defaultSalesLedgerID
                   //            +", "+Sales_Inventory_defaultPurchaseLedgerID
                   +", "+Sales_Inventory_ItemCode +" as 'Code' "
                   //            +", "+Sales_Inventory_OpeningBalanceDate
                   //            +", "+Sales_Inventory_ShelfLife
                   //            +", "+Sales_Inventory_WarrantyDays
                   +", "+Sales_Inventory_Price +" as 'Std Rate' "
                   //            +", "+Sales_Inventory_OpeningValue
                   //            +", "+Sales_Inventory_BrandID
                   //            +", "+Sales_Inventory_Description
                   + ", "
                   + "(Select "+ brandHelper->brandName+ " from "+brandHelper->brandTableName
                   + " WHERE "+ brandHelper->brandID +"= INV."
                   + Sales_Inventory_BrandID
                   + ") AS Brand"
                   +", "
                   + "(SELECT Group_Name from "+gpHelper->Sales_Inventory_Group_TableName
                   + " WHERE "+ gpHelper->Sales_Inventory_Group_GroupId+" = INV."+gpHelper->Sales_Inventory_Group_GroupId
                   + ") as 'Group'"
                   + " "
                   //            + "(Select "+ ledgerHelper->Ledger_Master_LedgerName + " from "+ledgerHelper->Ledger_Master_TableName
                   //            + " WHERE "+ ledgerHelper->Ledger_Master_LedgerID +"= INV."
                   //            + Sales_Inventory_defaultSalesLedgerID
                   //            + ") AS SalesLedgerName"
                   //            + ", "
                   //            + "(Select "+ ledgerHelper->Ledger_Master_LedgerName + " from "+ledgerHelper->Ledger_Master_TableName
                   //            + " WHERE "+ ledgerHelper->Ledger_Master_LedgerID +"= INV."
                   //            + Sales_Inventory_defaultPurchaseLedgerID
                   //            + ") AS SalesLedgerName"

                   //            + ", "
                   //            + "(Select "+ uomHelper->UOM_UOMName + " from "+uomHelper->UOM_TableName
                   //            + " WHERE "+ uomHelper->UOM_UOMID +"= INV. "
                   //            + Sales_Inventory_defaultUOM
                   //            + ") AS brandname"

                   //            + ", "

                   //            +", "+Sales_Inventory_isCustomItem
                   //            +", "+Sales_Inventory_Dimension
                   //            +", "+Sales_Inventory_isPurchaseItem
                   //            +", "+Sales_Inventory_isSalesItem
                   //            +", "+Sales_Inventory_KOT_Printer
                   //            +", "+Sales_Inventory_defaultInputTaxLedgerID
                   //            +", "+Sales_Inventory_defaultOutputTaxLedgerID
                   //            +", "+Sales_Inventory_defaultSalesReturnLedgerID
                   //            +", "+Sales_Inventory_defaultPurchaseReturnLedgerID

                   //            + ","
                   //            + "INV."+Sales_Inventory_OpeningStock +"-"
                   //            + " (SELECT SUM("+itemHelper.Transaction_Item_Helper_Cr_Qty +"-"
                   //            + itemHelper.Transaction_Item_Helper_Dr_Qty +") FROM "
                   //            + itemHelper.Transaction_Item_Helper_TableName
                   //            + " WHERE "
                   //            + itemHelper.Transaction_Item_Helper_ItemID + " = INV." + Sales_Inventory_ItemId +")"

                   + " FROM "+ Sales_Inventory_Table_Name
                   + " INV"
                   + whereClause
                   + " ORDER BY "+Sales_Inventory_ItemName;

    qDebug()<<qStr<<"inventory item";
    //    query->setQuery(qStr,QSqlDatabase::database(DatabaseValues::connectionString));


    //    return query;

    return db->getQueryModel(qStr);

}

QSqlQueryModel *SalesInventoryItemDatabaseHelper::getInventoryItemsQueryModelForSearch(int filter)
{
    LedgerMasterDatabaseHelper* ledgerHelper= new LedgerMasterDatabaseHelper();
    UomDataBaseHelper* uomHelper = new UomDataBaseHelper();
    SalesInventoryGroupsDatabaseHelper* gpHelper = new SalesInventoryGroupsDatabaseHelper();
    BrandsDatabaseHelper* brandHelper = new BrandsDatabaseHelper();
    //    ItemTransactionDatabaseHelper itemHelper;

    QString whereClause = " WHERE 1";
    if(filter == 1){
        whereClause = " WHERE "+Sales_Inventory_isSalesItem +"= 1 ";
    }
    else if(filter == 2){
        whereClause = " WHERE "+Sales_Inventory_isPurchaseItem +"= 1 ";
    }

    QString qStr="SELECT "
                   ""+Sales_Inventory_ItemId
                   +", "+Sales_Inventory_ItemName
                   +", "+Sales_Inventory_ItemCode
                   +", "+Sales_Inventory_Price
                   +", "+Sales_Inventory_SerailNumber
                   + ", "
                   + "(Select "+ brandHelper->brandName+ " from "+brandHelper->brandTableName
                   + " WHERE "+ brandHelper->brandID +"= INV."
                   + Sales_Inventory_BrandID
                   + ") AS Brand"
                   +", "
                   + "(SELECT Group_Name from "+gpHelper->Sales_Inventory_Group_TableName
                   + " WHERE "+ gpHelper->Sales_Inventory_Group_GroupId+" = INV."+gpHelper->Sales_Inventory_Group_GroupId
                   + ") as Group_Name"
                   + " "

                   + " FROM "+ Sales_Inventory_Table_Name
                   + " INV"
                   + whereClause
                   + " ORDER BY "+Sales_Inventory_ItemName;

    //    qDebug()<<qStr;


    return DatabaseHelper::getQueryModel(qStr);
}

QList<QSharedPointer<inventoryItemDataModel>> SalesInventoryItemDatabaseHelper::getALLInventoryObjectsByGroup(QString arg)
{
    QList<QSharedPointer<inventoryItemDataModel>> data;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    LedgerMasterDatabaseHelper* ledgerHelper= new LedgerMasterDatabaseHelper();
    UomDataBaseHelper* uomHelper = new UomDataBaseHelper();
    SalesInventoryGroupsDatabaseHelper* gpHelper = new SalesInventoryGroupsDatabaseHelper();
    BrandsDatabaseHelper* brandHelper = new BrandsDatabaseHelper();

    QString whereclause = " WHERE " + Sales_Inventory_GroupId + "='" + arg + "'";
    QString qStr="SELECT "
                   ""+Sales_Inventory_ItemId
                   +", "+Sales_Inventory_ItemName
                   +", "+Sales_Inventory_GroupId
                   +", "+Sales_Inventory_OpeningStock
                   +", "+Sales_Inventory_ClosingStock
                   +", "+Sales_Inventory_PartNumber
                   +", "+Sales_Inventory_VatRate
                   +", "+Sales_Inventory_ItemAlias
                   +", "+Sales_Inventory_Narration
                   +", "+Sales_Inventory_ReorderLevel
                   +", "+Sales_Inventory_OpeningRate
                   +", "+Sales_Inventory_defaultUOM
                   +", "+Sales_Inventory_defaultSalesLedgerID
                   +", "+Sales_Inventory_defaultPurchaseLedgerID
                   +", "+Sales_Inventory_ItemCode
                   +", "+Sales_Inventory_OpeningBalanceDate
                   +", "+Sales_Inventory_ShelfLife
                   +", "+Sales_Inventory_WarrantyDays
                   +", "+Sales_Inventory_Price
                   +", "+Sales_Inventory_OpeningValue
                   +", "+Sales_Inventory_BrandID
                   +", "+Sales_Inventory_Description


                   +", "+ "(SELECT Group_Name from "+gpHelper->Sales_Inventory_Group_TableName
                   + " WHERE "+ gpHelper->Sales_Inventory_Group_GroupId+" = INV."+gpHelper->Sales_Inventory_Group_GroupId
                   + ") as Group_Name"
                   + ", "
                   + "(Select "+ ledgerHelper->Ledger_Master_LedgerName + " from "+ledgerHelper->Ledger_Master_TableName
                   + " WHERE "+ ledgerHelper->Ledger_Master_LedgerID +"= INV. "
                   + Sales_Inventory_defaultSalesLedgerID
                   + ") AS SalesLedgerName"
                   + ", "
                   + "(Select "+ ledgerHelper->Ledger_Master_LedgerName + " from "+ledgerHelper->Ledger_Master_TableName
                   + " WHERE "+ ledgerHelper->Ledger_Master_LedgerID +"= INV. "
                   + Sales_Inventory_defaultPurchaseLedgerID
                   + ") AS SalesLedgerName"

                   + ", "
                   + "(Select "+ uomHelper->UOM_UOMName + " from "+uomHelper->UOM_TableName
                   + " WHERE "+ uomHelper->UOM_UOMID +"= INV. "
                   + Sales_Inventory_defaultUOM
                   + ") AS brandname"

                   + ", "
                   + "(Select "+ brandHelper->brandName+ " from "+brandHelper->brandTableName
                   + " WHERE "+ brandHelper->brandID +"= INV. "
                   + Sales_Inventory_BrandID
                   + ") AS UOMnaME"
                   +", "+Sales_Inventory_isCustomItem
                   +", "+Sales_Inventory_Dimension
                   +", "+Sales_Inventory_isPurchaseItem
                   +", "+Sales_Inventory_isSalesItem
                   +", "+Sales_Inventory_KOT_Printer
                   +", "+Sales_Inventory_defaultInputTaxLedgerID
                   +", "+Sales_Inventory_defaultOutputTaxLedgerID
                   +", "+Sales_Inventory_defaultSalesReturnLedgerID
                   +", "+Sales_Inventory_defaultPurchaseReturnLedgerID
                   +", "+Sales_Inventory_ItemName_Arabic
                   +", "+Sales_Inventory_From_Time
                   +", "+Sales_Inventory_To_Time
                   + ", "
                   + "(Select "+ uomHelper->UOM_DecimalPoints + " from "+uomHelper->UOM_TableName
                   + " WHERE "+ uomHelper->UOM_UOMID +"= INV. "
                   + Sales_Inventory_defaultUOM
                   + ") AS uomdeci"
                   + ", "
                   + "(Select "+ uomHelper->UOM_UOMSymbol + " from "+uomHelper->UOM_TableName
                   + " WHERE "+ uomHelper->UOM_UOMID +"= INV. "
                   + Sales_Inventory_defaultUOM
                   + ") AS uomsym"
                   + ","
                   + Sales_Inventory_HSN_CODE
                   + ", "+ Sales_Inventory_IsStockItem
                   + ", "+ Sales_Inventory_isSerailNumbered
                   + ", "+ Sales_Inventory_isBatchProcessed


                   + " FROM "+ Sales_Inventory_Table_Name
                   + " INV "
                   + whereclause
                   + " ORDER BY "+Sales_Inventory_ItemName;

    int i=1;
    query.setForwardOnly(true);

    if(query.exec(qStr)){
        inventoryItemDataModel *obj;
        while(query.next()){
            obj = new inventoryItemDataModel();
            obj->ItemID = query.value(0).toString();
            obj->ItemName = query.value(1).toString();
            obj->GroupID = query.value(2).toString();
            obj->OpeningStock = query.value(3).toFloat();
            obj->ClosingStock = query.value(4).toFloat();
            obj->PartNumber = query.value(5).toString();
            obj->taxRate = query.value(6).toFloat();
            obj->ItemAlias = query.value(7).toString();
            obj->narration = query.value(8).toString();
            obj->ReorderLevel = query.value(9).toFloat();
            obj->OpeningStockPrice = query.value(10).toFloat();
            obj->defaultUOMID = query.value(11).toString();
            obj->ItemCode = query.value(14).toString();
            obj->OpeningStockDate = query.value(15).toDate();
            obj->shelfLife = query.value(16).toFloat();
            obj->warrantyDays = query.value(17).toInt();
            obj->price = query.value(18).toFloat();
            obj->price_1 = obj->price;
            obj->OpeningStockValue = query.value(19).toFloat();
            obj->brandID = query.value(20).toInt();
            obj->ItemDescription=query.value(21).toString();
            obj->GroupName =query.value(22).toString();
            obj->defaultSalesLedgerID =query.value(12).toString();
            obj->defaultPurchaseLedgerID =query.value(13).toString();
            obj->uomObject.UomName = query.value(25).toString();
            obj->brandName = query.value(26).toString();
            obj->isCustomItem = query.value(27).toBool();
            obj->Dimension = query.value(28).toString();
            obj->isPurchaseItem = query.value(29).toBool();
            obj->isSalesItem = query.value(30).toBool();
            obj->KOTPrinter = query.value(31).toString();
            obj->DefaultInputTaxLedgerID = query.value(32).toString();
            obj->DefaultOutputTaxLedgerID = query.value(33).toString();
            obj->DefaultSalesReturnLedgerID = query.value(34).toString();
            obj->DefaultPurchaseReturnLedgerID = query.value(35).toString();
            obj->itemNameArabic = query.value(36).toString().toUtf8();
            obj->fromTime = query.value(37).toTime();
            obj->toTime    = query.value(38).toTime();
            obj->uomObject.decimalPoints = query.value(39).toInt();
            obj->uomObject.UomSymbol    = query.value(40).toString();
            obj->uomObject._id = obj->defaultUOMID;
            obj->hsnCode = query.value(41).toString();
            obj->isStockItem = query.value(42).toBool();
            obj->isSerailNumbered = query.value(43).toBool();
            obj->isBatchProcessed = query.value(44).toBool();
            obj->bomList =  BillOfMaterialsIngredientsDataBaseHelper::getBillOfMaterials(obj->ItemID);
            //qDebug()<<"Property name : "<<obj->brandName;
            data.push_back(QSharedPointer<inventoryItemDataModel>(obj));

            i++;
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return data;
}

QList<QSharedPointer<inventoryItemDataModel> > SalesInventoryItemDatabaseHelper::getALLInventoryObjectsFavourites()
{
    QList<QSharedPointer<inventoryItemDataModel>> data;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    LedgerMasterDatabaseHelper* ledgerHelper= new LedgerMasterDatabaseHelper();
    UomDataBaseHelper* uomHelper = new UomDataBaseHelper();
    SalesInventoryGroupsDatabaseHelper* gpHelper = new SalesInventoryGroupsDatabaseHelper();
    BrandsDatabaseHelper* brandHelper = new BrandsDatabaseHelper();

    QString whereclause = " WHERE " + Sales_Inventory_Favourite + "='" + QString::number(1) + "' ";
    QString qStr="SELECT "
                   ""+Sales_Inventory_ItemId
                   +", "+Sales_Inventory_ItemName
                   +", "+Sales_Inventory_GroupId
                   +", "+Sales_Inventory_OpeningStock
                   +", "+Sales_Inventory_ClosingStock
                   +", "+Sales_Inventory_PartNumber
                   +", "+Sales_Inventory_VatRate
                   +", "+Sales_Inventory_ItemAlias
                   +", "+Sales_Inventory_Narration
                   +", "+Sales_Inventory_ReorderLevel
                   +", "+Sales_Inventory_OpeningRate
                   +", "+Sales_Inventory_defaultUOM
                   +", "+Sales_Inventory_defaultSalesLedgerID
                   +", "+Sales_Inventory_defaultPurchaseLedgerID
                   +", "+Sales_Inventory_ItemCode
                   +", "+Sales_Inventory_OpeningBalanceDate
                   +", "+Sales_Inventory_ShelfLife
                   +", "+Sales_Inventory_WarrantyDays
                   +", "+Sales_Inventory_Price
                   +", "+Sales_Inventory_OpeningValue
                   +", "+Sales_Inventory_BrandID
                   +", "+Sales_Inventory_Description


                   +", "+ "(SELECT Group_Name from "+gpHelper->Sales_Inventory_Group_TableName
                   + " WHERE "+ gpHelper->Sales_Inventory_Group_GroupId+" = INV."+gpHelper->Sales_Inventory_Group_GroupId
                   + ") as Group_Name"
                   + ", "
                   + "(Select "+ ledgerHelper->Ledger_Master_LedgerName + " from "+ledgerHelper->Ledger_Master_TableName
                   + " WHERE "+ ledgerHelper->Ledger_Master_LedgerID +"= INV. "
                   + Sales_Inventory_defaultSalesLedgerID
                   + ") AS SalesLedgerName"
                   + ", "
                   + "(Select "+ ledgerHelper->Ledger_Master_LedgerName + " from "+ledgerHelper->Ledger_Master_TableName
                   + " WHERE "+ ledgerHelper->Ledger_Master_LedgerID +"= INV. "
                   + Sales_Inventory_defaultPurchaseLedgerID
                   + ") AS SalesLedgerName"

                   + ", "
                   + "(Select "+ uomHelper->UOM_UOMName + " from "+uomHelper->UOM_TableName
                   + " WHERE "+ uomHelper->UOM_UOMID +"= INV. "
                   + Sales_Inventory_defaultUOM
                   + ") AS brandname"

                   + ", "
                   + "(Select "+ brandHelper->brandName+ " from "+brandHelper->brandTableName
                   + " WHERE "+ brandHelper->brandID +"= INV. "
                   + Sales_Inventory_BrandID
                   + ") AS UOMnaME"
                   +", "+Sales_Inventory_isCustomItem
                   +", "+Sales_Inventory_Dimension
                   +", "+Sales_Inventory_isPurchaseItem
                   +", "+Sales_Inventory_isSalesItem
                   +", "+Sales_Inventory_KOT_Printer
                   +", "+Sales_Inventory_defaultInputTaxLedgerID
                   +", "+Sales_Inventory_defaultOutputTaxLedgerID
                   +", "+Sales_Inventory_defaultSalesReturnLedgerID
                   +", "+Sales_Inventory_defaultPurchaseReturnLedgerID
                   +", "+Sales_Inventory_ItemName_Arabic
                   +", "+Sales_Inventory_From_Time
                   +", "+Sales_Inventory_To_Time
                   + ", "
                   + "(Select "+ uomHelper->UOM_DecimalPoints + " from "+uomHelper->UOM_TableName
                   + " WHERE "+ uomHelper->UOM_UOMID +"= INV. "
                   + Sales_Inventory_defaultUOM
                   + ") AS uomdeci"
                   + ", "
                   + "(Select "+ uomHelper->UOM_UOMSymbol + " from "+uomHelper->UOM_TableName
                   + " WHERE "+ uomHelper->UOM_UOMID +"= INV. "
                   + Sales_Inventory_defaultUOM
                   + ") AS uomsym"
                   + ","
                   + Sales_Inventory_HSN_CODE
                   + +", "
                   + Sales_Inventory_IsStockItem
                   + " FROM "+ Sales_Inventory_Table_Name
                   + " INV "
                   + whereclause
                   + " ORDER BY "+Sales_Inventory_ItemName;

    int i=1;

    qDebug()<<qStr;

    query.setForwardOnly(true);

    if(query.exec(qStr)){
        inventoryItemDataModel *obj;
        while(query.next()){
            obj = new inventoryItemDataModel();
            obj->ItemID = query.value(0).toString();
            obj->ItemName = query.value(1).toString();
            obj->GroupID = query.value(2).toString();
            obj->OpeningStock = query.value(3).toFloat();
            obj->ClosingStock = query.value(4).toFloat();
            obj->PartNumber = query.value(5).toString();
            obj->taxRate = query.value(6).toFloat();
            obj->ItemAlias = query.value(7).toString();
            obj->narration = query.value(8).toString();
            obj->ReorderLevel = query.value(9).toFloat();
            obj->OpeningStockPrice = query.value(10).toFloat();
            obj->defaultUOMID = query.value(11).toString();
            obj->ItemCode = query.value(14).toString();
            obj->OpeningStockDate = query.value(15).toDate();
            obj->shelfLife = query.value(16).toFloat();
            obj->warrantyDays = query.value(17).toInt();
            obj->price = query.value(18).toFloat();
            obj->OpeningStockValue = query.value(19).toFloat();
            obj->brandID = query.value(20).toInt();
            obj->ItemDescription=query.value(21).toString();
            obj->GroupName =query.value(22).toString();
            obj->defaultSalesLedgerID =query.value(12).toString();
            obj->defaultPurchaseLedgerID =query.value(13).toString();
            obj->uomObject.UomName = query.value(25).toString();
            obj->brandName = query.value(26).toString();
            obj->isCustomItem = query.value(27).toBool();
            obj->Dimension = query.value(28).toString();
            obj->isPurchaseItem = query.value(29).toBool();
            obj->isSalesItem = query.value(30).toBool();
            obj->KOTPrinter = query.value(31).toString();
            obj->DefaultInputTaxLedgerID = query.value(32).toString();
            obj->DefaultOutputTaxLedgerID = query.value(33).toString();
            obj->DefaultSalesReturnLedgerID = query.value(34).toString();
            obj->DefaultPurchaseReturnLedgerID = query.value(35).toString();
            obj->itemNameArabic = query.value(36).toString().toUtf8();
            obj->fromTime = query.value(37).toTime();
            obj->toTime   = query.value(38).toTime();
            obj->uomObject.decimalPoints = query.value(39).toInt();
            obj->uomObject.UomSymbol    = query.value(40).toString();
            obj->uomObject._id = obj->defaultUOMID;
            obj->hsnCode = query.value(41).toString();
            obj->price_1 = obj->price;
            obj->isStockItem = query.value(42).toBool();
            obj->bomList =  BillOfMaterialsIngredientsDataBaseHelper::getBillOfMaterials(obj->ItemID);
            //qDebug()<<"Property name : "<<obj->brandName;
            data.push_back(QSharedPointer<inventoryItemDataModel>(obj));

            i++;
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return data;
}



inventoryItemDataModel SalesInventoryItemDatabaseHelper::getInventoryItemByID(QString id,
                                                                              bool withCsStk,
                                                                              QDate toDate,
                                                                              QString priceid)
{
    //    qDebug()<<"getInvventoryItemById"<<id;
    inventoryItemDataModel obj;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    SalesInventoryGroupsDatabaseHelper gpHelper ;
    UomDataBaseHelper uomHelper  ;
    BrandsDatabaseHelper brandHelper;
    PricelistDatabaseHelper *priceHelper = new PricelistDatabaseHelper;



    QString qStr="SELECT "
                   ""+Sales_Inventory_ItemId
                   + ", "+Sales_Inventory_ItemName
                   + ", "+Sales_Inventory_GroupId
                   + ", "+Sales_Inventory_OpeningStock
                   + ", "+Sales_Inventory_ClosingStock
                   + ", "+Sales_Inventory_PartNumber
                   + ", "+Sales_Inventory_VatRate
                   + ", "+Sales_Inventory_ItemAlias
                   + ", "+Sales_Inventory_Narration
                   + ", "+Sales_Inventory_ReorderLevel
                   + ", "+Sales_Inventory_OpeningRate
                   + ", "+Sales_Inventory_defaultUOM
                   + ", "+Sales_Inventory_defaultSalesLedgerID
                   + ", "+Sales_Inventory_defaultPurchaseLedgerID
                   + ", "+Sales_Inventory_ItemCode
                   + ", "+Sales_Inventory_OpeningBalanceDate
                   + ", "+Sales_Inventory_ShelfLife
                   + ", "+Sales_Inventory_WarrantyDays
                   + ", "+Sales_Inventory_Price
                   + ", "+Sales_Inventory_OpeningValue
                   + ", "+Sales_Inventory_BrandID
                   + ", "+Sales_Inventory_Description
                   +", "+Sales_Inventory_isCustomItem
                   +", "+Sales_Inventory_Dimension
                   +", "+Sales_Inventory_isPurchaseItem
                   +", "+Sales_Inventory_isSalesItem
                   +", "+Sales_Inventory_KOT_Printer
                   +", "+Sales_Inventory_defaultInputTaxLedgerID
                   +", "+Sales_Inventory_defaultOutputTaxLedgerID
                   +", "+Sales_Inventory_defaultSalesReturnLedgerID
                   +", "+Sales_Inventory_defaultPurchaseReturnLedgerID
                   +", "+Sales_Inventory_ItemName_Arabic
                   +", "+Sales_Inventory_OpeningValue

                   +", "+ "(SELECT Group_Name from "+gpHelper.Sales_Inventory_Group_TableName
                   + " WHERE "+ gpHelper.Sales_Inventory_Group_GroupId+" = INV."+gpHelper.Sales_Inventory_Group_GroupId
                   + ") as Group_Name"
                   + ", "

                   + "(Select "+ brandHelper.brandName+ " from "+brandHelper.brandTableName
                   + " WHERE "+ brandHelper.brandID +"= INV."
                   + Sales_Inventory_BrandID
                   + ") AS brandname"
                   + ", "

                   + "(Select "+ uomHelper.UOM_UOMName + " from "+uomHelper.UOM_TableName
                   + " WHERE "+ uomHelper.UOM_UOMID +"= INV."
                   + Sales_Inventory_defaultUOM
                   + ") AS UOMnaME"
                   + "," + Sales_Inventory_StdCost
                   + ", "+ Sales_Inventory_Favourite
                   + ", "+ Sales_Inventory_From_Time
                   + ", "+ Sales_Inventory_To_Time
                   + ","
                   + "(Select "+ uomHelper.UOM_DecimalPoints + " from "+uomHelper.UOM_TableName
                   + " WHERE "+ uomHelper.UOM_UOMID +"= INV."
                   + Sales_Inventory_defaultUOM
                   + ") AS UOMDecimal"
                   + ", "
                   + "(Select "+ uomHelper.UOM_UOMSymbol + " from "+uomHelper.UOM_TableName
                   + " WHERE "+ uomHelper.UOM_UOMID +"= INV."
                   + Sales_Inventory_defaultUOM
                   + ") AS UOMSym"
                   + ","
                   + Sales_Inventory_HSN_CODE + ", "
                   + Sales_Inventory_Section + ", "
                   + Sales_Inventory_Flags + ", "
                   + Sales_Inventory_Category +", "
                   + Sales_Inventory_IsStockItem +", "
                   + Sales_Inventory_Price2 + +", "
                   + Sales_Inventory_hideInBill +", "
                   + Sales_Inventory_isSerailNumbered +", "
                   + Sales_Inventory_isBatchProcessed

                   + " FROM "+ Sales_Inventory_Table_Name + " INV "
                   + " WHERE " + Sales_Inventory_ItemId +  "=:id";
    query.prepare(qStr);
    query.bindValue(":id",id);
    //    qDebug()<<qStr<<query.boundValues();
    if(query.exec()){
        while(query.next()){

            obj.ItemID                          = query.value(0) .toString();
            obj.ItemName                        = query.value(1) .toString();
            obj.GroupID                         = query.value(2) .toString();
            obj.OpeningStock                    = query.value(3) .toFloat();
            //            obj.ClosingStock                    = query.value(4) .toFloat();
            obj.PartNumber                      = query.value(5) .toString();
            obj.taxRate                         = query.value(6) .toFloat();
            obj.ItemAlias                       = query.value(7) .toString();
            obj.narration                       = query.value(8) .toString();
            obj.ReorderLevel                    = query.value(9) .toFloat();
            obj.OpeningStockPrice               = query.value(10).toFloat();
            obj.defaultUOMID                    = query.value(11).toString();
            obj.defaultSalesLedgerID            = query.value(12).toString();
            obj.defaultPurchaseLedgerID         = query.value(13).toString();
            obj.ItemCode                        = query.value(14).toString();
            obj.OpeningStockDate                = query.value(15).toDate();
            obj.shelfLife                       = query.value(16).toFloat();
            obj.warrantyDays                    = query.value(17).toInt();
            obj.price                           = query.value(18).toFloat();
            obj.OpeningStockValue               = query.value(19).toFloat();
            obj.brandID                         = query.value(20).toInt();
            obj.ItemDescription                 = query.value(21).toString();
            obj.isCustomItem                    = query.value(22).toBool();
            obj.Dimension                       = query.value(23).toString();
            obj.isPurchaseItem                  = query.value(24).toBool();
            obj.isSalesItem                     = query.value(25).toBool();
            obj.KOTPrinter                      = query.value(26).toString();
            obj.DefaultInputTaxLedgerID         = query.value(27).toString();
            obj.DefaultOutputTaxLedgerID        = query.value(28).toString();
            obj.DefaultSalesReturnLedgerID      = query.value(29).toString();
            obj.DefaultPurchaseReturnLedgerID   = query.value(30).toString();
            obj.itemNameArabic                  = query.value(31).toString().toUtf8();
            obj.OpeningStockValue               = query.value(32).toFloat();
            obj.GroupName                       = query.value(33).toString();
            obj.brandName                       = query.value(34).toString();
            obj.uomObject.UomName               = query.value(35).toString();
            obj.stdCost                         = query.value(36).toFloat();
            obj.favo                            = query.value(37).toBool();
            obj.fromTime                        = query.value(38).toTime();
            obj.toTime                          = query.value(39).toTime();
            obj.uomObject._id                   = obj.defaultUOMID;
            obj.uomObject.decimalPoints         = query.value(40).toInt();
            obj.uomObject.UomSymbol             = query.value(41).toString();
            obj.hsnCode                         = query.value(42).toString();
            obj.section                         = query.value(43).toString();
            obj.flags                           = query.value(44).toJsonObject();
            obj.Category                        = query.value(45).toString();
            obj.isStockItem                     = query.value(46).toBool();
            obj.price_2                         = query.value(47).toFloat();
            obj.price_1                         = obj.price;
            obj.hideInPrint                     = query.value(48).toBool();

            obj.isSerailNumbered                = query.value(49).toBool();
            obj.isBatchProcessed                = query.value(50).toBool();

            obj.isBundled                       =obj.flags["bundled"].toBool();

            if(withCsStk)
                obj.ClosingStock = getClsStkOfItemTillDate(obj.ItemID, toDate);
            //            qDebug()<<Q_FUNC_INFO<<__LINE__<<withCsStk<<obj.ClosingStock<<"closing stock";
            //                        qDebug()<<"getItemById 1382"<<priceid;
            if(priceid.length()>0)
                obj.price = priceHelper->getPriceByItemID(id,priceid);
            obj.priceLastPurchase = getLastPurchPrice(obj.ItemID,toDate);

            //                obj.ClosingStock = getClosingStockofItemByDate(obj.ItemID, toDate);
        }
        //        qDebug()<<"Item id retrieved"<<obj.ItemID;
        //        qDebug()<<"query size"<<query.record().count();
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();

    }
    obj.bomList =  BillOfMaterialsIngredientsDataBaseHelper::getBillOfMaterials(obj.ItemID);
    //    qDebug()<<qStr;
    //        qDebug()<<"Got obj : "<<obj.ItemName;
    return obj;
}

QString SalesInventoryItemDatabaseHelper::getItemIDBySKUID(QString SKUID)
{
    QString itemID;

    return itemID;
}

CompoundItemDataObject SalesInventoryItemDatabaseHelper::getInventoryItemByBarcode(QString barcode, bool isSerial, QDate toDate)
{
    qDebug()<<Q_FUNC_INFO<<barcode;
    CompoundItemDataObject obj;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    SalesInventoryGroupsDatabaseHelper* gpHelper = new SalesInventoryGroupsDatabaseHelper();
    UomDataBaseHelper* uomHelper = new UomDataBaseHelper();
    BrandsDatabaseHelper* brandHelper = new BrandsDatabaseHelper();
    LedgerMasterDatabaseHelper *ledgerHelper = new LedgerMasterDatabaseHelper();

    QString search = Sales_Inventory_ItemCode;
    //    search = " ( SELECT " + UomDataBaseHelper::UOM_Conversion_BarCode + " FROM " + UomDataBaseHelper::UOM_Conversion_TableName;

    if(isSerial)
        search = Sales_Inventory_SerailNumber;

    QString qStr="SELECT "
                   ""+Sales_Inventory_ItemId
                   + ", "+Sales_Inventory_ItemName
                   + ", "+Sales_Inventory_GroupId
                   + ", "+Sales_Inventory_OpeningStock
                   + ", "+Sales_Inventory_ClosingStock
                   + ", "+Sales_Inventory_PartNumber
                   + ", "+Sales_Inventory_VatRate
                   + ", "+Sales_Inventory_ItemAlias
                   + ", INV."+Sales_Inventory_Narration
                   + ", "+Sales_Inventory_ReorderLevel
                   + ", "+Sales_Inventory_OpeningRate
                   + ", ";
    if(isSerial)
        qStr += "INV." + Sales_Inventory_defaultUOM;
    else
        qStr+= "uom." + UomDataBaseHelper::UOM_Conversion_ToUnit;
    qStr += ", "+Sales_Inventory_defaultSalesLedgerID
            + ", "+Sales_Inventory_defaultPurchaseLedgerID
            + ", "+Sales_Inventory_ItemCode
            + ", "+Sales_Inventory_OpeningBalanceDate
            + ", "+Sales_Inventory_ShelfLife
            + ", "+Sales_Inventory_WarrantyDays
            + ", "+Sales_Inventory_Price
            + ", "+Sales_Inventory_OpeningValue
            + ", "+Sales_Inventory_BrandID
            + ", "+Sales_Inventory_Description


            +", "+ "(SELECT Group_Name from "+gpHelper->Sales_Inventory_Group_TableName
            + " WHERE "+ gpHelper->Sales_Inventory_Group_GroupId+" = INV."+gpHelper->Sales_Inventory_Group_GroupId
            + ") as Group_Name"
            + ", "
            + "(Select "+ ledgerHelper->Ledger_Master_LedgerName + " from "+ledgerHelper->Ledger_Master_TableName
            + " WHERE "+ ledgerHelper->Ledger_Master_LedgerID +"= INV. "
            + Sales_Inventory_defaultSalesLedgerID
            + ") AS SalesLedgerName"
            + ", "
            + "(Select "+ ledgerHelper->Ledger_Master_LedgerName + " from "+ledgerHelper->Ledger_Master_TableName
            + " WHERE "+ ledgerHelper->Ledger_Master_LedgerID +"= INV. "
            + Sales_Inventory_defaultPurchaseLedgerID
            + ") AS SalesLedgerName"

            + ", "
            + "(Select "+ uomHelper->UOM_UOMName + " from "+uomHelper->UOM_TableName
            + " WHERE "+ uomHelper->UOM_UOMID ;
    if(isSerial)
        qStr += " = INV. " + Sales_Inventory_defaultUOM;
    else
        qStr += " = uom." + UomDataBaseHelper::UOM_Conversion_ToUnit;
    qStr += ") AS UOMnaME, ";


    qStr += "(Select "+ brandHelper->brandName+ " from "+brandHelper->brandTableName
            + " WHERE "+ brandHelper->brandID +"= INV. "
            + Sales_Inventory_BrandID
            + ") AS brandname"
            + ", "+Sales_Inventory_isCustomItem
            + ", "+Sales_Inventory_Dimension
            + ", "+Sales_Inventory_isPurchaseItem
            + ", "+Sales_Inventory_isSalesItem
            + ", "+Sales_Inventory_KOT_Printer
            + ", "+Sales_Inventory_defaultInputTaxLedgerID
            + ", "+Sales_Inventory_defaultOutputTaxLedgerID
            + ", "+Sales_Inventory_defaultSalesReturnLedgerID
            + ", "+Sales_Inventory_defaultPurchaseReturnLedgerID
            + ", "+Sales_Inventory_ItemName_Arabic
            + ", "+Sales_Inventory_StdCost
            +", "+Sales_Inventory_From_Time
            +", "+Sales_Inventory_To_Time

            + ", "
            + "(Select "+ uomHelper->UOM_DecimalPoints + " from "+uomHelper->UOM_TableName
            + " WHERE "+ uomHelper->UOM_UOMID ;
    if(isSerial)
        qStr += " = INV. " + Sales_Inventory_defaultUOM;
    else
        qStr += " = uom." + UomDataBaseHelper::UOM_Conversion_ToUnit;
    qStr += ") AS uomdeci, ";


    qStr  += "(Select "+ uomHelper->UOM_UOMSymbol + " from "+uomHelper->UOM_TableName
            + " WHERE "+ uomHelper->UOM_UOMID ;
    if(isSerial)
        qStr += " = INV. " + Sales_Inventory_defaultUOM;
    else
        qStr += " = uom." + UomDataBaseHelper::UOM_Conversion_ToUnit;
    qStr += " ) AS uomsymbol, ";

    qStr += Sales_Inventory_HSN_CODE +", "
            + Sales_Inventory_Section + ", "
            + Sales_Inventory_Flags + ", "
            + Sales_Inventory_Category +", "
            + Sales_Inventory_IsStockItem + ", "
            + Sales_Inventory_Price2 + +", "
            + Sales_Inventory_hideInBill +", "

            + Sales_Inventory_isSerailNumbered +", "
            + Sales_Inventory_isBatchProcessed

            + " FROM "+ Sales_Inventory_Table_Name
            + " INV ";
    if(isSerial)
        qStr += " WHERE "+search + "=:ID";
    else{
        qStr += " JOIN " + UomDataBaseHelper::UOM_Conversion_TableName + " uom ";
        qStr += " ON INV." + Sales_Inventory_ItemId + " = uom." + UomDataBaseHelper::UOM_Conversion_ItemId;
        qStr += " WHERE uom."  + UomDataBaseHelper::UOM_Conversion_BarCode + " = :ID ";
    }

    qDebug()<<Q_FUNC_INFO<<qStr;

    int i=1;

    query.prepare(qStr);
    query.bindValue(":ID",barcode);
    if(query.exec()){
        if(query.first()){
            obj.BaseItem.ItemID = query.value(0).toString();
            obj.BaseItem.ItemName = query.value(1).toString();
            obj.BaseItem.GroupID = query.value(2).toString();
            obj.BaseItem.OpeningStock = query.value(3).toFloat();
            obj.BaseItem.ClosingStock = query.value(4).toFloat();
            obj.BaseItem.PartNumber = query.value(5).toString();
            obj.BaseItem.taxRate = query.value(6).toFloat();
            obj.BaseItem.ItemAlias = query.value(7).toString();
            obj.BaseItem.narration = query.value(8).toString();
            obj.BaseItem.ReorderLevel = query.value(9).toFloat();
            obj.BaseItem.OpeningStockPrice = query.value(10).toFloat();
            obj.BaseItem.defaultUOMID = query.value(11).toString();
            obj.BaseItem.ItemCode = query.value(14).toString();
            obj.BaseItem.OpeningStockDate = query.value(15).toDate();
            obj.BaseItem.shelfLife = query.value(16).toFloat();
            obj.BaseItem.warrantyDays = query.value(17).toInt();
            obj.BaseItem.price = query.value(18).toFloat();
            obj.BaseItem.OpeningStockValue = query.value(19).toFloat();
            obj.BaseItem.brandID = query.value(20).toInt();
            obj.BaseItem.ItemDescription=query.value(21).toString();
            obj.BaseItem.GroupName =query.value(22).toString();
            obj.BaseItem.defaultSalesLedgerID =query.value(12).toString();
            obj.BaseItem.defaultPurchaseLedgerID =query.value(13).toString();
            obj.BaseItem.uomObject.UomName = query.value(25).toString();
            obj.BaseItem.brandName = query.value(26).toString();
            obj.BaseItem.isCustomItem = query.value(27).toBool();
            obj.BaseItem.Dimension = query.value(28).toString();
            obj.BaseItem.isPurchaseItem = query.value(29).toBool();
            obj.BaseItem.isSalesItem = query.value(30).toBool();
            obj.BaseItem.KOTPrinter = query.value(31).toString();
            obj.BaseItem.DefaultInputTaxLedgerID = query.value(32).toString();
            obj.BaseItem.DefaultOutputTaxLedgerID = query.value(33).toString();
            obj.BaseItem.DefaultSalesReturnLedgerID = query.value(34).toString();
            obj.BaseItem.DefaultPurchaseReturnLedgerID = query.value(35).toString();
            obj.BaseItem.itemNameArabic = query.value(36).toString().toUtf8();
            obj.BaseItem.stdCost = query.value(37).toFloat();
            obj.BaseItem.fromTime = query.value(38).toTime();
            obj.BaseItem.toTime = query.value(39).toTime();
            obj.BaseItem.uomObject._id = obj.BaseItem.defaultUOMID;
            obj.BaseItem.uomObject.decimalPoints = query.value(40).toInt();
            obj.BaseItem.uomObject.UomSymbol = query.value(41).toString();
            obj.BaseItem.hsnCode             = query.value(42).toString();
            obj.BaseItem.section                         = query.value(43).toString();
            obj.BaseItem.flags                           = query.value(44).toJsonObject();
            obj.BaseItem.Category                        = query.value(45).toString();
            obj.BaseItem.isStockItem         = query.value(46).toBool();
            obj.BaseItem.price_2            = query.value(47).toFloat();
            obj.BaseItem.hideInPrint            = query.value(48).toBool();
            obj.BaseItem.isSerailNumbered            = query.value(49).toBool();
            obj.BaseItem.isBatchProcessed             = query.value(50).toBool();
            obj.BaseItem.price_1             = obj.BaseItem.price;
            obj.BaseItem.ClosingStock = getClsStkOfItemTillDate(obj.BaseItem.ItemID, toDate);
            //            obj.BaseItem.ClosingStock = getClosingStockofItemByDate(obj.BaseItem.ItemID, toDate);

        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }

    obj.BaseItem.bomList =  BillOfMaterialsIngredientsDataBaseHelper::getBillOfMaterials(obj.BaseItem.ItemID);

    return obj;
}

inventoryItemDataModel *SalesInventoryItemDatabaseHelper::getInventoryItemByIDasPtr(QString id, QDate toDate, QString dbName)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__<<id;
    inventoryItemDataModel* obj = new inventoryItemDataModel;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    SalesInventoryGroupsDatabaseHelper gpHelper ;
    UomDataBaseHelper uomHelper  ;
    BrandsDatabaseHelper brandHelper;

    QString qStr="SELECT " + Sales_Inventory_ItemId
                   + ", "+Sales_Inventory_ItemName
                   + ", "+Sales_Inventory_GroupId
                   + ", "+Sales_Inventory_OpeningStock
                   + ", "+Sales_Inventory_ClosingStock
                   + ", "+Sales_Inventory_PartNumber
                   + ", "+Sales_Inventory_VatRate
                   + ", "+Sales_Inventory_ItemAlias
                   + ", "+Sales_Inventory_Narration
                   + ", "+Sales_Inventory_ReorderLevel
                   + ", "+Sales_Inventory_OpeningRate
                   + ", "+Sales_Inventory_defaultUOM
                   + ", "+Sales_Inventory_defaultSalesLedgerID
                   + ", "+Sales_Inventory_defaultPurchaseLedgerID
                   + ", "+Sales_Inventory_ItemCode
                   + ", "+Sales_Inventory_OpeningBalanceDate
                   + ", "+Sales_Inventory_ShelfLife
                   + ", "+Sales_Inventory_WarrantyDays
                   + ", "+Sales_Inventory_Price
                   + ", "+Sales_Inventory_OpeningValue
                   + ", "+Sales_Inventory_BrandID
                   + ", "+Sales_Inventory_Description
                   +", "+Sales_Inventory_isCustomItem
                   +", "+Sales_Inventory_Dimension
                   +", "+Sales_Inventory_isPurchaseItem
                   +", "+Sales_Inventory_isSalesItem
                   +", "+Sales_Inventory_KOT_Printer
                   +", "+Sales_Inventory_defaultInputTaxLedgerID
                   +", "+Sales_Inventory_defaultOutputTaxLedgerID
                   +", "+Sales_Inventory_defaultSalesReturnLedgerID
                   +", "+Sales_Inventory_defaultPurchaseReturnLedgerID
                   +", "+Sales_Inventory_ItemName_Arabic
                   +", "+Sales_Inventory_OpeningValue

                   +", "+ "(SELECT Group_Name from "+gpHelper.Sales_Inventory_Group_TableName
                   + " WHERE "+ gpHelper.Sales_Inventory_Group_GroupId+" = INV."+gpHelper.Sales_Inventory_Group_GroupId
                   + ") as Group_Name"
                   + ", "

                   + "(Select "+ brandHelper.brandName+ " from "+brandHelper.brandTableName
                   + " WHERE "+ brandHelper.brandID +"= INV. "
                   + Sales_Inventory_BrandID
                   + ") AS brandname"
                   + ", "

                   + "(Select "+ uomHelper.UOM_UOMName + " from "+uomHelper.UOM_TableName
                   + " WHERE "+ uomHelper.UOM_UOMID +"= INV. "
                   + Sales_Inventory_defaultUOM
                   + ") AS UOMnaME"
                   + "," + Sales_Inventory_StdCost
                   + ", "+ Sales_Inventory_Favourite
                   + ", "+ Sales_Inventory_From_Time
                   + ", "+ Sales_Inventory_To_Time
                   + ", "

                   + "(Select "+ uomHelper.UOM_DecimalPoints + " from "+uomHelper.UOM_TableName
                   + " WHERE "+ uomHelper.UOM_UOMID +"= INV. "
                   + Sales_Inventory_defaultUOM
                   + ") AS decim"+ ", "

                   + "(Select "+ uomHelper.UOM_UOMSymbol + " from "+uomHelper.UOM_TableName
                   + " WHERE "+ uomHelper.UOM_UOMID +"= INV. "
                   + Sales_Inventory_defaultUOM
                   + ") AS UOMsym"
                   + ","
                   + Sales_Inventory_HSN_CODE + ", "
                   + Sales_Inventory_Section + ", "
                   + Sales_Inventory_Flags + ", "
                   + Sales_Inventory_Category +", "
                   + Sales_Inventory_IsStockItem +", "
                   + Sales_Inventory_SerailNumber +", "
                   + Sales_Inventory_Price2 +", "
                   + Sales_Inventory_hideInBill + ", "
                   + Sales_Inventory_isSerailNumbered +", "
                   + Sales_Inventory_isBatchProcessed
                   + " FROM ";
    if(dbName.length() != 0)
        qStr += dbName + ".";

    qStr += Sales_Inventory_Table_Name + " INV "
            + " WHERE " + Sales_Inventory_ItemId +  "=:id";

    query.prepare(qStr);
    query.bindValue(":id",id);
    if(query.exec()){
        qDebug()<<Q_FUNC_INFO<<__LINE__<<id;
        if(query.first()){
            qDebug()<<Q_FUNC_INFO<<__LINE__<<id;

            obj->ItemID                          = query.value(0) .toString();
            obj->ItemName                        = query.value(1) .toString();
            obj->GroupID                         = query.value(2) .toString();
            obj->OpeningStock                    = query.value(3) .toFloat();
            obj->ClosingStock                    = query.value(4) .toFloat();
            obj->PartNumber                      = query.value(5) .toString();
            obj->taxRate                         = query.value(6) .toFloat();
            obj->ItemAlias                       = query.value(7) .toString();
            obj->narration                       = query.value(8) .toString();
            obj->ReorderLevel                    = query.value(9) .toFloat();
            obj->OpeningStockPrice               = query.value(10).toFloat();
            obj->defaultUOMID                    = query.value(11).toString();
            obj->defaultSalesLedgerID            = query.value(12).toString();
            obj->defaultPurchaseLedgerID         = query.value(13).toString();
            obj->ItemCode                        = query.value(14).toString();
            obj->OpeningStockDate                = query.value(15).toDate();
            obj->shelfLife                       = query.value(16).toFloat();
            obj->warrantyDays                    = query.value(17).toInt();
            obj->price                           = query.value(18).toFloat();
            obj->OpeningStockValue               = query.value(19).toFloat();
            obj->brandID                         = query.value(20).toInt();
            obj->ItemDescription                 = query.value(21).toString();
            obj->isCustomItem                    = query.value(22).toBool();
            obj->Dimension                       = query.value(23).toString();
            obj->isPurchaseItem                  = query.value(24).toBool();
            obj->isSalesItem                     = query.value(25).toBool();
            obj->KOTPrinter                      = query.value(26).toString();
            obj->DefaultInputTaxLedgerID         = query.value(27).toString();
            obj->DefaultOutputTaxLedgerID        = query.value(28).toString();
            obj->DefaultSalesReturnLedgerID      = query.value(29).toString();
            obj->DefaultPurchaseReturnLedgerID   = query.value(30).toString();
            obj->itemNameArabic                  = query.value(31).toString().toUtf8();
            obj->OpeningStockValue               = query.value(32).toFloat();
            obj->GroupName                       = query.value(33).toString();
            obj->brandName                       = query.value(34).toString();
            obj->uomObject.UomName               = query.value(35).toString();
            obj->stdCost                         = query.value(36).toFloat();
            obj->favo                            = query.value(37).toBool();
            obj->fromTime                        = query.value(38).toTime();
            obj->toTime                          = query.value(39).toTime();
            obj->uomObject.decimalPoints         = query.value(40).toInt();
            obj->uomObject.UomSymbol             = query.value(41).toString();
            obj->hsnCode                         = query.value(42).toString();
            obj->section                         = query.value(43).toString();
            obj->flags                           = query.value(44).toJsonObject();
            obj->Category                        = query.value(45).toString();
            obj->isStockItem                     = query.value(46).toBool();
            obj->SerailNumber                    = query.value(47).toString();
            obj->uomObject._id                   = obj->defaultUOMID;
            obj->price_1                         = obj->price;
            obj->price_2                         = query.value(48).toFloat();
            obj->hideInPrint                     = query.value(49).toBool();
            obj->isSerailNumbered                = query.value(50).toBool();
            obj->isBatchProcessed                = query.value(51).toBool();
            obj->isBundled                       = obj->flags["bundled"].toBool();


            ItemTransactionDatabaseHelper *itemTransaction = new ItemTransactionDatabaseHelper(db);

            obj->consumedQty = itemTransaction->getConsumedQtyByVoucher(obj->ItemID, "0",
                                                                        "OPENING STOCK", LoginValues::voucherPrefix, obj->ItemReqUuid);
            qDebug()<<"query size"<<query.record().count();
            //            ClosingStockHelper *clsHelper = new ClosingStockHelper();
            obj->ClosingStock = getClsStkOfItemTillDate(id, toDate);  // use this
            //            obj->ClosingStock = clsHelper->getClosingStockOfItemTillDate(obj->ItemID, toDate);
            //            obj->ClosingStock = getClosingStockofItemByDate(obj->ItemID, toDate);
            qDebug()<<obj->ClosingStock<<obj->Category;
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();

    }
    obj->bomList =  BillOfMaterialsIngredientsDataBaseHelper::getBillOfMaterials(obj->ItemID);
    qDebug()<<Q_FUNC_INFO<<__LINE__<<id<<obj->flags<<obj->isBundled;
    return obj;

}

bool SalesInventoryItemDatabaseHelper::isSalesItemByItemId(QString itemID)
{
    bool name = false;
    QString qStr = "SELECT "+Sales_Inventory_isSalesItem + " FROM " + Sales_Inventory_Table_Name
                   + " WHERE "+Sales_Inventory_ItemId + "=:id";
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);
    query.bindValue(":id",itemID);

    if(query.exec()){
        while (query.next()) {
            name =  query.value(0).toBool();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery(); qDebug()<<"219";
    }
    return name;
}

float SalesInventoryItemDatabaseHelper::getCostByItem(QString ItemId)
{
    float cost = 0;
    QString qStr = "select ";
    qStr += "coalesce(sum((coalesce(i.purchase_item_qty,1))/coalesce(i.sales_item_qty,1)*(p.Std_Cost)),s.std_cost) as purch_cost ";
    qStr += " from " + Sales_Inventory_Table_Name + " s ";
    qStr += " left outer join  " + BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_TableName +"  i ";
    qStr += " on  s.item_id = i.sales_item ";
    qStr += " left outer join "+ Sales_Inventory_Table_Name +" p ";
    qStr += " on i.purchase_item = p.Item_ID ";
    qStr += " where s." + Sales_Inventory_ItemId + " = '" + ItemId + "' ";
    qStr += " group by s." + Sales_Inventory_ItemId;

    //    qDebug()<<qStr;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);

    if(query.exec()){
        while (query.next()) {
            cost =  query.value(0).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery(); qDebug()<<"219";
    }
    return cost;

}


float SalesInventoryItemDatabaseHelper::getClsStkOfItemTillDate(QString  ItemID, QDate dateTo, QString godownID)
{
    //    qDebug()<<Q_FUNC_INFO;

    float clsStock=0;
    QString qStr;

    QSqlQueryModel* model = new QSqlQueryModel;
    SalesInventoryItemDatabaseHelper items;
    ItemTransactionDatabaseHelper itemHelper;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    qStr += " select ";
    qStr += " cast(sum(Inward)-sum(Issue) as decimal(10,3)) AS 'Closing Stock' FROM  ";
    qStr += " (   ";
    qStr += " select trans.VoucherDate vDate, ItemID as item_id, ";
    if(godownID != "%")
        qStr += " 0 as Inward, ";
    else
    {
        qStr += " (dr_qty)  as Inward , ";
    }
    qStr += "  (cr_qty) as Issue,  ";
    qStr += " trans.Dr_Amount , trans.Cr_Amount ";
    qStr += " from Transaction_Item_Helper trans where     ";
    qStr += " VoucherDate <= " + db->getDateString(dateTo) ;
    qStr += " and ItemID = '" + ItemID + "'";

    if(godownID != "%"){
        qStr += " and (";
        qStr += " From_Location like '" + godownID + "'";
        qStr += " ) UNION ALL ";
        qStr += " select trans.VoucherDate vDate, ItemID as item_id, ";
        qStr += "  (dr_qty) as Inward , ";
        if(godownID != "%")
            qStr += " 0 as Issue, ";
        else
        {
            qStr += " (cr_qty) as Issue,  ";
        }
        qStr += " trans.Dr_Amount , trans.Cr_Amount ";
        qStr += " from Transaction_Item_Helper trans where     ";
        qStr += " VoucherDate <= " + db->getDateString(dateTo) ;
        qStr += " and ItemID = '" + ItemID + "'";

        qStr += " and (";
        qStr += " To_Location like '" + godownID + "' ";
        qStr += " )  ";
    }
    qStr += " UNION ALL ";
    qStr += " select trans.VoucherDate vDate, ItemID as item_id, ";
    if(godownID != "%")
        qStr += " 0 as Inward, ";
    else
    {
        qStr += " (dr_qty) as Inward , ";
    }
    qStr += "  (cr_qty) as Issue,  ";
    qStr += " trans.Dr_Amount , trans.Cr_Amount ";
    qStr += " from Transaction_BOM_Helper trans where     ";
    qStr += " VoucherDate <= " + db->getDateString(dateTo) ;
    qStr += " and ItemID = '" + ItemID + "'";
    if(godownID != "%"){
        qStr += " and (";
        qStr += " From_Location = '" + godownID + "'";
        qStr += " ) UNION ALL ";
        qStr += " select trans.VoucherDate vDate, ItemID as item_id, ";
        qStr += "  (dr_qty)  as Inward , ";
        if(godownID != "%")
            qStr += " 0 as Issue, ";
        else
        {
            qStr += "  (cr_qty) as Issue,  ";
        }
        qStr += " trans.Dr_Amount , trans.Cr_Amount ";
        qStr += " from Transaction_Bom_Helper trans where     ";
        qStr += " VoucherDate <= " + db->getDateString(dateTo) ;
        qStr += " and ItemID = '" + ItemID + "'";

        qStr += " and (";
        qStr += " To_Location = '" + godownID + "' ";
        qStr += " )  ";
    }

    qStr += " ) A ";

    return DatabaseHelper::getSingletonQueryResult(qStr).toFloat();
}

bool SalesInventoryItemDatabaseHelper::updateItemIdInMasters(QString oldId, QString newId)
{
    startTransaction();
    QMap<QString, QVariant>map;
    map.insert(Sales_Inventory_ItemId, newId);
    QMap<QString, QVariant>clause;
    clause.insert(Sales_Inventory_ItemId, oldId);
    db->updateRecordsPlus(map, clause, Sales_Inventory_Table_Name);

    QMap<QString, QVariant>map1;
    map1.insert(BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_Sales_Item, newId);
    QMap<QString, QVariant>clause1;
    clause1.insert(BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_Sales_Item, oldId);
    db->updateRecordsPlus(map1, clause1,
                          BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_TableName);

    QMap<QString, QVariant>map2;
    map2.insert(BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_Purchase_Item, newId);
    QMap<QString, QVariant>clause2;
    clause2.insert(BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_Purchase_Item, oldId);
    db->updateRecordsPlus(map2, clause2,
                          BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_TableName);

    QMap<QString, QVariant>map3;
    map3.insert(UomDataBaseHelper::UOM_Conversion_ItemId, newId);
    QMap<QString, QVariant>clause3;
    clause3.insert(UomDataBaseHelper::UOM_Conversion_ItemId, oldId);
    db->updateRecordsPlus(map3, clause3,
                          UomDataBaseHelper::UOM_Conversion_TableName);

    QMap<QString, QVariant>map4;
    map4.insert(PricelistDatabaseHelper::PriceListEntries_ItemID, newId);
    QMap<QString, QVariant>clause4;
    clause4.insert(PricelistDatabaseHelper::PriceListEntries_ItemID, oldId);
    db->updateRecordsPlus(map4, clause4,
                          PricelistDatabaseHelper::PriceListEntries_TableName );

    return commit();

}

bool SalesInventoryItemDatabaseHelper::updateItemName(QString itmName, QString itemID)
{
    startTransaction();
    QMap<QString, QVariant>map;
    map.insert(Sales_Inventory_ItemName, itmName);
    QMap<QString, QVariant>clause;
    clause.insert(Sales_Inventory_ItemId, itemID);
    db->updateRecordsPlus(map, clause, Sales_Inventory_Table_Name);

    return commit();
}

bool SalesInventoryItemDatabaseHelper::updateItemIdInTrans(QString oldId, QString newId)
{
    qDebug()<<Q_FUNC_INFO<<newId<<oldId;
    this->startTransaction();
    QString itemIDCol = "Inventory_Item_Id";
    QMap<QString, QVariant>map;
    map.insert(itemIDCol, newId);
    QMap<QString, QVariant>clause;
    clause.insert(itemIDCol, oldId);
    db->updateRecordsPlus(map, clause,
                          GMVoucherTypes::getVoucherDetailsTablename(GMVoucherTypes::SalesVoucher));
    db->updateRecordsPlus(map, clause,
                          GMVoucherTypes::getVoucherDetailsTablename(GMVoucherTypes::PurchaseVoucher));
    db->updateRecordsPlus(map, clause,
                          GMVoucherTypes::getVoucherDetailsTablename(GMVoucherTypes::SalesOrder));
    db->updateRecordsPlus(map, clause,
                          GMVoucherTypes::getVoucherDetailsTablename(GMVoucherTypes::PurchaseOrder));
    db->updateRecordsPlus(map, clause,
                          GMVoucherTypes::getVoucherDetailsTablename(GMVoucherTypes::CreditNote));
    db->updateRecordsPlus(map, clause,
                          GMVoucherTypes::getVoucherDetailsTablename(GMVoucherTypes::DebitNote));
    db->updateRecordsPlus(map, clause,
                          GMVoucherTypes::getVoucherDetailsTablename(GMVoucherTypes::DeliveryNote));
    db->updateRecordsPlus(map, clause,
                          GMVoucherTypes::getVoucherDetailsTablename(GMVoucherTypes::ReceiptNote));
    db->updateRecordsPlus(map, clause,
                          GMVoucherTypes::getVoucherDetailsTablename(GMVoucherTypes::StockJournal));
    db->updateRecordsPlus(map, clause,
                          GMVoucherTypes::getVoucherDetailsTablename(GMVoucherTypes::StockEntry));
    db->updateRecordsPlus(map, clause,
                          GMVoucherTypes::getVoucherDetailsTablename(GMVoucherTypes::GodownTransfer));


    QMap<QString, QVariant>map1;
    map1.insert("item_ID", newId);
    QMap<QString, QVariant>clause1;
    clause1.insert("item_ID", oldId);
    db->updateRecordsPlus(map1, clause1,
                          GMVoucherTypes::getVoucherDetailsTablename(GMVoucherTypes::IndentRequest));

    QMap<QString, QVariant>map2;
    map2.insert("itemID", newId);
    QMap<QString, QVariant>clause2;
    clause2.insert("itemID", oldId);
    db->updateRecordsPlus(map2, clause2,
                          GMVoucherTypes::getVoucherDetailsTablename(GMVoucherTypes::WorkOrder));
    db->updateRecordsPlus(map2, clause2,"Transaction_Item_Helper");
    db->updateRecordsPlus(map2, clause2, "Transaction_Bom_Helper");

    return commit();

}

bool SalesInventoryItemDatabaseHelper::deleteItemFromMasters(QString id)
{
    qDebug()<<Q_FUNC_INFO<<id;
    this->startTransaction();
    //    QSqlQuery q(QSqlDatabase::database("gmdb"));
    QString q = "select count(*)  from transaction_item_helper where itemid='"+id+"'";
    if(DatabaseHelper::getSingletonQueryResult(q).toInt()>0){
        QMessageBox box;box.setText("Inventory in Use.\nCannot Delete Item!!!"); box.exec();
        return 0;
    }

    QMap<QString,QString> clause;

    clause.insert(Sales_Inventory_ItemId,id);
    db->deleteRecords(clause,Sales_Inventory_Table_Name);

    QMap<QString, QVariant>clause1;
    clause1.insert(BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_Sales_Item, id);
    db->deleteRecordsPlus( clause1,
                          BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_TableName);

    QMap<QString, QVariant>clause2;
    clause2.insert(BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_Purchase_Item, id);
    db->deleteRecordsPlus( clause2,
                          BillOfMaterialsIngredientsDataBaseHelper::Bill_Of_Materials_Ingredients_TableName);

    QMap<QString, QVariant>clause3;
    clause3.insert(UomDataBaseHelper::UOM_Conversion_ItemId, id);
    db->deleteRecordsPlus( clause3,
                          UomDataBaseHelper::UOM_Conversion_TableName);

    QMap<QString, QVariant>clause4;
    clause4.insert(PricelistDatabaseHelper::PriceListEntries_ItemID, id);
    db->deleteRecordsPlus( clause4,
                          PricelistDatabaseHelper::PriceListEntries_TableName );

    return commit();


}

QString SalesInventoryItemDatabaseHelper::checkItemNameInRefDb(QString itemName, QString refDb)
{
    QString qStr = " SELECT item_id from " + refDb + ".sales_inventory_items where ";
    qStr += " replace(item_name, ' ', '') like replace('" + itemName + "', ' ', '')";

    return DatabaseHelper::getSingletonQueryResult(qStr).toString();

}

QString SalesInventoryItemDatabaseHelper::createItemID(QString groupID)
{

    int maxId = getMaxID().toInt();
    QString itemID =  groupID+DatabaseValues::idGen+QString::number(maxId);
    while (true) {
        qDebug()<<Q_FUNC_INFO<<__LINE__<<itemID;
        if(!checkItemExists(itemID))
            return itemID;
        else {
            itemID = groupID+ DatabaseValues::idGen+ QString::number(maxId++);
            qDebug()<<Q_FUNC_INFO<<__LINE__<<itemID;
        }
    }


}

QList<inventoryItemDataModel *> SalesInventoryItemDatabaseHelper::getMissingItemsFromRefDb(QString refDb, QString currDb)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    QList<inventoryItemDataModel *> itemsList;

    QString qStr = " SELECT ref.item_ID from " + refDb + "." + Sales_Inventory_Table_Name + " ref ";
    qStr += " WHERE " + Sales_Inventory_ItemId + " not in  ";
    qStr += " ( SELECT curr.item_ID from " + currDb + "." + Sales_Inventory_Table_Name + " curr ) ";

    QSqlQuery query(QSqlDatabase::database("gmdb"));
    query.prepare(qStr);
    if(query.exec()){
        while (query.next()) {
            inventoryItemDataModel *item = getInventoryItemByIDasPtr(query.value(0).toString(), QDate::currentDate(), refDb);
            item->action  = 1001;
            item->fromExternal = true;
            itemsList.append(item);

        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }

    qDebug()<<Q_FUNC_INFO<<__LINE__<<itemsList.size();
    return itemsList;

}


bool SalesInventoryItemDatabaseHelper::checkItemTransactionExists(QString itemID)
{

    QSqlQuery q(QSqlDatabase::database("gmdb"));
    q.exec("select count(*)  from transaction_item_helper where itemid='"+itemID+"' and voucherType not like 'OPENING STOCK' ");
    if(q.size()>0){
        return true;
    }
    else {
        return false;
    }

}

QMap<int, QList<QPair<QString, QString> > > SalesInventoryItemDatabaseHelper::getDuplicateItems()
{
    QString qStr = "SELECT items.item_id, items.item_name, ";
    qStr += " @rank := CASE WHEN @rankval = string THEN @rank ";
    qStr += " WHEN (@rankval := string) IS NOT NULL THEN @rank + 1 END AS rnk ";
    qStr += " FROM (select item_name, item_id, upper(REPLACE(Item_Name, ' ','')) as string ";
    qStr += " from sales_inventory_items where REPLACE(item_name, ' ', '') in ( ";
    qStr += " select REPLACE(item_name, ' ', '') from sales_inventory_items ";
    qStr += " group by REPLACE(item_name, ' ', '') ";
    qStr += " HAVING COUNT(*) > 1 ";
    qStr += " ))items , (SELECT @rank := 0,@rankval := NULL) AS x ";
    qStr += " order by Item_Name";

    QMap<int, QList<QPair<QString, QString> > > data;

    QSqlQuery query(QSqlDatabase::database("gmdb"));
    if(query.exec(qStr)){
        int prevRank=0;
        while (query.next()) {
            int rank = query.value(2).toInt();


            QPair<QString, QString> itemIdNamePair;
            itemIdNamePair.first = query.value(0).toString();
            itemIdNamePair.second = query.value(1).toString();

            if(rank == prevRank)
                data[rank].append(itemIdNamePair);
            else {
                QList<QPair<QString, QString> > itemList;
                itemList.append(itemIdNamePair);
                data.insert(rank, itemList);
                prevRank = rank;
            }

        }
    }

    return data;

}

QSqlQueryModel *SalesInventoryItemDatabaseHelper::getDuplicateItemsBetweenDbs(QString currDB, QString refDb)
{
    QString qStr = "select ref.item_name new_name, ref.item_id new_id, old.item_name, old.item_id old_id ";
    qStr += " from "+currDB + ".sales_inventory_items old join " + refDb +".sales_inventory_items ref ";
    qStr += " on REPLACE(old.item_name, ' ', '') = REPLACE(ref.item_name, ' ', '') ";
    qStr += " and old.item_id not like ref.item_id ";

    return DatabaseHelper::getQueryModel(qStr);

}

QSqlQueryModel *SalesInventoryItemDatabaseHelper::getSameItemIdForDiffItemsBetweenDbs(QString currDB, QString refDb)
{
    QString qStr = "select ref.item_name new_name, ref.item_id new_id, old.item_name, old.item_id old_id ";
    qStr += " from "+currDB + ".sales_inventory_items old join " + refDb +".sales_inventory_items ref ";
    qStr += " on REPLACE(old.item_name, ' ', '') not like REPLACE(ref.item_name, ' ', '') ";
    qStr += " and old.item_id = ref.item_id ";

    return DatabaseHelper::getQueryModel(qStr);

}



float SalesInventoryItemDatabaseHelper::getItemStockByFifo(QString  ItemID, QDate toDate)
{
    //    QSqlQueryModel* model = new QSqlQueryModel;
    //    ItemTransactionDatabaseHelper itemHelper;
    //    SalesInventoryItemDatabaseHelper items;

    float clsStock=0;
    QString qStr;

    qStr = "";
    qStr += " select round(sum(dr_Qty - consumed),3) from " + Transaction_Item_Helper_TableName ;
    qStr += " where " + Transaction_Item_Helper_VoucherDate + " <= " + db->getDateString(toDate);
    qStr += " and " + Transaction_Item_Helper_ItemID + " = '" + ItemID + "'";
    qStr += " group by " + Transaction_Item_Helper_ItemID;


    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);
    if(query.exec()){
        if(query.first())
            clsStock = query.value(0).toFloat();
    }
    else{
        qDebug()<<query.lastError()<<"###closing stock error";
        qDebug()<<query.lastQuery();
    }
    return clsStock;
}

float SalesInventoryItemDatabaseHelper::getTaxRate(QString id)
{
    float name = 0;
    QString qStr = "SELECT "+Sales_Inventory_VatRate + " FROM " + Sales_Inventory_Table_Name
                   + " WHERE "+Sales_Inventory_ItemId + "=:id";
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);
    query.bindValue(":id",id);

    if(query.exec()){
        while (query.next()) {
            name =  query.value(0).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery(); qDebug()<<"219";
    }
    return name;
}


float SalesInventoryItemDatabaseHelper::getClosingStockofItemByDate(QString itemID, QDate date)
{
    float balance=0;
    //    ItemTransactionDatabaseHelper trans;
    QString qStr = "";
    qStr = " select round(";
    qStr += " coalesce(" + Sales_Inventory_OpeningStock + ",0) - ";
    qStr += " coalesce(sum(trans." + Transaction_Item_Helper_Cr_Qty + " ),0)  + ";
    qStr += " coalesce(sum(trans." + Transaction_Item_Helper_Dr_Qty + " ),0),3) ";
    qStr += " from " + Sales_Inventory_Table_Name + " item left join " ;
    qStr +=  Transaction_Item_Helper_TableName + " trans on item." ;
    qStr += Sales_Inventory_ItemId + " = trans." + Transaction_Item_Helper_ItemID;
    qStr += " and trans." + Transaction_Item_Helper_VoucherDate + " <='"+date.toString("yyyy-MM-dd")+"'";
    qStr += " where item.item_id = '" + itemID + "'";

    //    qDebug()<<"Closing stock : ";
    //    qDebug()<<qStr;
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

QString SalesInventoryItemDatabaseHelper::getMaxSerialNo()
{
    QSqlDatabase db = QSqlDatabase::database(DatabaseValues::connectionString);
    QSqlQuery query(db);
    int id =0;
    QString qStr = "select max(_id) from "+ Sales_Inventory_Table_Name;
    query.exec(qStr);
    while(query.next()){
        id = query.value(0).toInt();
    }
    return QString::number(id+1);
}

QString SalesInventoryItemDatabaseHelper::getMaxCode()
{
    QSqlDatabase db = QSqlDatabase::database(DatabaseValues::connectionString);
    QSqlQuery query(db);
    int id = 0;
    QString qStr = "select max(cast("+Sales_Inventory_SerailNumber+" as integer)) from "+ Sales_Inventory_Table_Name;
    query.exec(qStr);
    while(query.next()){
        id = query.value(0).toInt();
    }

    return QString::number(id+1);
}

void SalesInventoryItemDatabaseHelper::updateItemCost(QString itemID, float cost)
{
    QMap<QString,QVariant> map;
    QMap<QString,QVariant> clause;
    clause.insert(Sales_Inventory_ItemId,itemID);
    map.insert(Sales_Inventory_StdCost,cost);
    startTransaction();
    db->updateRecordsPlus(map,clause,Sales_Inventory_Table_Name);
    commit();
}

bool SalesInventoryItemDatabaseHelper::SerialExist(QString serial)
{
    bool flag1 = false;
    QSqlDatabase db = QSqlDatabase::database(DatabaseValues::connectionString);
    QSqlQuery query(db);
    int id =0;
    QString qStr = "select "+Sales_Inventory_SerailNumber+" from "+ Sales_Inventory_Table_Name;
    qStr += " where "+Sales_Inventory_SerailNumber+" = '"+serial+"'";
    query.exec(qStr);
    if(query.size()>0){
        flag1 = true;
        qDebug()<<qStr;
    }
    return flag1;
}

float SalesInventoryItemDatabaseHelper::getLastPurchPrice(QString itemID, QDate vDate)
{
    //    qDebug()<<Q_FUNC_INFO;
    DatabaseHelper *db = new DatabaseHelper();
    float val = 0;
    QString qStr = "SELECT price from purchase_invoice_details ";
    qStr += " where Inventory_Item_ID ='"+itemID+"' and voucher_date <= " + db->getDateString(vDate) ;
    qStr += " order by Voucher_Date  desc limit 1";
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    if(query.exec(qStr)){
        while(query.next()){
            val = query.value(0).toFloat();
        }
    }
    return val;
}

float SalesInventoryItemDatabaseHelper::getLastSalesPrice(QString itemID, QDate vDate)
{
    qDebug()<<Q_FUNC_INFO;

    DatabaseHelper *db = new DatabaseHelper();
    float val = 0;
    QString qStr = "SELECT price from sales_invoice_details ";
    qStr += " where Inventory_Item_ID ='"+itemID+"' and voucher_date <= " + db->getDateString(vDate) ;
    qStr += " order by Voucher_Date  desc limit 1";
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    if(query.exec(qStr)){
        while(query.next()){
            val = query.value(0).toFloat();
        }
    }
    return val;
}

QSqlQueryModel *SalesInventoryItemDatabaseHelper::getInventoryItemsByGroupModel(QString groupId)
{
    InventoryItemImagesDatabaseHelper img;
    QString qStr = " SELECT Item_ID, Item_Name, (select Image from " + img.tablename + " where ItemId = Item_ID) as Image  ";
    qStr += " FROM " + Sales_Inventory_Table_Name +" where group_ID = '" + groupId + "'";

    return DatabaseHelper::getQueryModel(qStr);
}

QSqlQueryModel *SalesInventoryItemDatabaseHelper::getInventoryItemsAllFavos()
{
    QString qStr = " SELECT Item_ID, Item_Name ";
    qStr += " FROM " + Sales_Inventory_Table_Name +" where "+Sales_Inventory_Favourite+"=1";
    return DatabaseHelper::getQueryModel(qStr);
}

bool SalesInventoryItemDatabaseHelper::checkBarCodeExists(QString itemCode, QString itemID)
{
    QString qStr = " SELECT 1 FROM " + Sales_Inventory_Table_Name + " WHERE " + Sales_Inventory_ItemCode + " = '" + itemCode + "'";
    if(itemID.length() > 0){
        qStr += " AND " + Sales_Inventory_ItemId + " not like '" + itemID + "'";
    }

    if(db->getSingletonQueryResult(qStr).toInt() == 1)
        return true;
    else
        return false;

}

void SalesInventoryItemDatabaseHelper::updateLastModified(QString itemID)
{
    QMap<QString, QVariant> map;
    map.insert(Sales_Inventory_lastModified, QDateTime::currentDateTime());
    QMap<QString, QVariant> map1;
    map1.insert(Sales_Inventory_ItemId, itemID);

    db->updateRecordsPlus(map, map1, Sales_Inventory_Table_Name);

}
