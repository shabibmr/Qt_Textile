#include "stockentrydatabasehelper.h"

StockEntryDatabaseHelper::StockEntryDatabaseHelper(QString connString)
{
    db = new DatabaseHelper(connString);
    itemTransaction = new ItemTransactionDatabaseHelper(db);

}

StockEntryDatabaseHelper::~StockEntryDatabaseHelper()
{

}

int StockEntryDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

QString StockEntryDatabaseHelper::getNextVoucherNo(QString voucherPrefix)
{
    GeneralVoucherDatabaseHelper vHelper;
    return vHelper.getNextVoucherNoByType(VoucherType, voucherPrefix, Stock_Main_TableName);
}


int StockEntryDatabaseHelper::insertVoucher(GeneralVoucherDataObject *obj)
{
    this->startTransaction();
    QMap<QString,QVariant> map;
    GeneralVoucherDatabaseHelper *vHelper = new GeneralVoucherDatabaseHelper;
    obj->voucherNumber = vHelper->setNextVoucherNoByType(VoucherType, obj->VoucherPrefix, Stock_Main_TableName);

    map.insert(Stock_Main_Voucher_No,obj->voucherNumber);
    map.insert(Stock_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Stock_Main_Date_Created,obj->VoucherDate);
    map.insert(Stock_Main_Voucher_Date,obj->VoucherDate);
    //    map.insert(Stock_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    //    map.insert(Stock_Main_Billing_Name,obj->ledgerObject.LedgerName);
    map.insert(Stock_Main_Total,obj->grandTotal);
    map.insert(Stock_Main_Narration,obj->narration);
    map.insert(Stock_Main_Added_By,obj->AddedBy);
    map.insert(Stock_Main_TimeStampC,obj->timestamp);
    map.insert(Stock_Main_Last_Updated,QDateTime::currentDateTime());

    //    map.insert(Stock_Main_Delivery_Date,obj->DeliveryDate);
    //    map.insert(Stock_Main_Completion_Probability,obj->CompletionProbability);
    //    map.insert(Stock_Main_Credit_Period,obj->ledgerObject.Credit_Period);
    //    map.insert(Stock_Main_POC_Name,obj->ledgerObject.ContactPersonName);
    //    map.insert(Stock_Main_POC_Phone,obj->ledgerObject.ContactPersonNumber);
    //    map.insert(Stock_Main_POC_Email,obj->ledgerObject.ContactPersonEmail);
    //    map.insert(Stock_Main_Project_ID,obj->ProjectId);
    //    map.insert(Stock_Main_Revision_Number,obj->RevisionNo);
    //    map.insert(Stock_Main_Converted_To_Sales_Order,obj->ConvertedToSalesOrder);
    //    map.insert(Stock_Main_Quotation_Prepared,obj->QuotationPrepared);
    //    map.insert(Stock_Main_Salesman_ID,obj->SalesmanID);
    qint64 lastId= db->insertRecordsPlus(map,Stock_Main_TableName);

    qDebug()<<"New ID = "<<lastId;
//    QMap<QString,QVariant> idmap;
//    idmap.insert(Stock_Main_Voucher_No,QString::number(lastId));
//    QMap<QString,QVariant> idmapClause;
//    idmapClause.insert(Stock_Main_id,lastId);
//    db->updateRecordsPlus(idmap,idmapClause,Stock_Main_TableName);
//    qDebug()<<"New ID updated = "<<lastId;


    for(CompoundItemDataObject compitem:obj->InventoryItems){
        QMap<QString,QVariant> map1;
        inventoryItemDataModel item = compitem.BaseItem;

        SalesInventoryItemDatabaseHelper s;

        if(item.drQty >0)
            item.price = SalesInventoryItemDatabaseHelper::getLastPurchPrice(item.ItemID, obj->VoucherDate);
        map1.insert(Stock_Details_Voucher_No,obj->voucherNumber);
        map1.insert(Stock_Details_Voucher_Prefix,obj->VoucherPrefix);
        map1.insert(Stock_Details_Voucher_Date,obj->VoucherDate);
        map1.insert(Stock_Details_Inventory_Item_ID,item.ItemID);
        map1.insert(Stock_Details_Quantity,item.quantity);
        map1.insert(Stock_Details_Uom, item.uomObject.UomName);
        map1.insert(Stock_Details_Difference, item.crQty-item.drQty);
        map1.insert(Stock_Details_Price,item.price);
        if(item.ItemReqUuid == "X")
            item.ItemReqUuid = db->getUUID();
        map1.insert(Stock_Detials_ItemReqUuid, item.ItemReqUuid);
        //        map1.insert(Stock_Details_PriceLevel,item.PriceLevel);
        //        map1.insert(Stock_Details_isCompoundItem,item.IsCompoundItem);
        //        map1.insert(Stock_Details_subItemOff_list_ID,0);
        //        map1.insert(Stock_Details_Discount_Percent,item.discountPercentage);
        //        map1.insert(Stock_Details_Discount_Amount,item.discount);
        map1.insert(Stock_Details_Subtotal,item.subTotal);
        //        map1.insert(Stock_Details_Project_ID,obj->ProjectId);
        map1.insert(Stock_Details_Item_Narration,item.narration);
        map1.insert(Stock_Details_Item_Description,item.ItemDescription);
        //        map1.insert(Stock_Details_Salesman_ID,obj->SalesmanID);

        ItemModel = new ItemTransactionDataModel();
        ItemModel->ItemID = item.ItemID;
        ItemModel->VoucherID = obj->voucherNumber;
        ItemModel->VoucherPrefix = obj->VoucherPrefix;
        ItemModel->VoucherDate = obj->VoucherDate;
        ItemModel->VoucherType = VoucherType;

        if(item.crQty > 0){
            ItemModel->From_Location = item.section;
            ItemModel->To_Location = "OUT";
        }
        if(item.drQty > 0){
            ItemModel->To_Location = item.section;
            ItemModel->From_Location = "OUT";
        }

//        ItemModel->From_Location = "Stock";
//        ItemModel->To_Location = "Godown";
        ItemModel->crQty = item.crQty;
        ItemModel->drQty = item.drQty;
        ItemModel->drAmount = item.subTotal;
        ItemModel->itemReqId = item.ItemReqUuid;
        ItemModel->Narration = obj->narration;

        ItemModel->consumed = item.consumedQty;
        ItemModel->itemReqId = item.ItemReqUuid;
        ItemModel->Price = item.price;
        itemTransaction->insertItemTransaction(ItemModel, item.godownList);
        int basid = db->insertRecordsPlus(map1,Stock_Details_TableName);
        for(inventoryItemDataModel subitem:compitem.subItems){
            QMap<QString,QVariant> map2;
            map2.insert(Stock_Details_Voucher_No,obj->voucherNumber);
            map2.insert(Stock_Details_Voucher_Prefix,obj->VoucherPrefix);
            map2.insert(Stock_Details_Voucher_Date,obj->VoucherDate);
            map2.insert(Stock_Details_Inventory_Item_ID,subitem.ItemID);
            map2.insert(Stock_Details_Quantity,subitem.quantity);
            map2.insert(Stock_Details_Price,subitem.price);
            //            map2.insert(Stock_Details_PriceLevel,subitem.PriceLevel);
            //            map2.insert(Stock_Details_isCompoundItem,subitem.IsCompoundItem);
            //            map2.insert(Stock_Details_subItemOff_list_ID,basid);
            //            map2.insert(Stock_Details_Discount_Percent,subitem.discountPercentage);
            //            map2.insert(Stock_Details_Discount_Amount,subitem.discount);
            map2.insert(Stock_Details_Subtotal,subitem.subTotal);
            //            map2.insert(Stock_Details_Project_ID,obj->ProjectId);
            map2.insert(Stock_Details_Item_Narration,subitem.narration);
            map2.insert(Stock_Details_Item_Description,subitem.ItemDescription);
            //            map2.insert(Stock_Details_Salesman_ID,obj->SalesmanID);

            db->insertRecordsPlus(map2,Stock_Details_TableName);
        }
    }
    qDebug()<<"Ledgers Size = "<<obj->ledgersList.size();
    //    for(LedgerMasterDataModel ledger:obj->ledgersList){
    //        QMap <QString,QVariant> ledMap;
    //        ledMap.insert(Proxy_Transaction_Helper_VoucherType,VoucherType);
    //        ledMap.insert(Proxy_Transaction_Helper_VoucherNo,obj->voucherNumber);
    //        ledMap.insert(Proxy_Transaction_Helper_VoucherPrefix,obj->VoucherPrefix);
    //        ledMap.insert(Proxy_Transaction_Helper_VoucherDate,obj->VoucherDate);
    //        ledMap.insert(Proxy_Transaction_Helper_ProjectID,obj->ProjectId);
    //        ledMap.insert(Proxy_Transaction_Helper_SalesManID,obj->SalesmanID);
    //        ledMap.insert(Proxy_Transaction_Helper_TranactionAmount,ledger.amount);
    //        ledMap.insert(Proxy_Transaction_Helper_CreditLedgerID,ledger.LedgerID);
    //        ledMap.insert(Proxy_Transaction_Helper_DebitLedgerID,obj->ledgerObject.LedgerID);
    //        ledMap.insert(Proxy_Transaction_Helper_Narration,obj->ledgerObject.narration);
    //        db->insertRecordsPlus(ledMap,Proxy_Transaction_Helper_TableName);
    //    }

    return commit();
}

int StockEntryDatabaseHelper::updateVoucher(GeneralVoucherDataObject *obj, QString oldID)
{
    this->startTransaction();

    QMap<QString,QVariant> map;
    map.insert(Stock_Main_Voucher_No,obj->voucherNumber);
    map.insert(Stock_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Stock_Main_Date_Created,obj->VoucherDate);
    map.insert(Stock_Main_Voucher_Date,obj->VoucherDate);
    //    map.insert(Stock_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    //    map.insert(Stock_Main_Billing_Name,obj->ledgerObject.LedgerName);
    //    map.insert(Stock_Main_Total,obj->grandTotal);
    map.insert(Stock_Main_Narration,obj->narration);
    map.insert(Stock_Main_Added_By,obj->AddedBy);
    //    map.insert(Stock_Main_Delivery_Date,obj->DeliveryDate);
    //    map.insert(Stock_Main_Completion_Probability,obj->CompletionProbability);
    //    map.insert(Stock_Main_Credit_Period,obj->ledgerObject.Credit_Period);
    //    map.insert(Stock_Main_POC_Name,obj->ledgerObject.ContactPersonName);
    //    map.insert(Stock_Main_POC_Phone,obj->ledgerObject.ContactPersonNumber);
    //    map.insert(Stock_Main_POC_Email,obj->ledgerObject.ContactPersonEmail);
    //    map.insert(Stock_Main_Project_ID,obj->ProjectId);
    //    map.insert(Stock_Main_Revision_Number,obj->RevisionNo);
    //    map.insert(Stock_Main_Converted_To_Sales_Order,obj->ConvertedToSalesOrder);
    //    map.insert(Stock_Main_Quotation_Prepared,obj->QuotationPrepared);
    //    map.insert(Stock_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Stock_Main_Last_Updated,QDateTime::currentDateTime());

    QMap<QString,QVariant> map3;
    map3.insert(Stock_Main_Voucher_No,oldID);
    db->updateRecordsPlus(map,map3,Stock_Main_TableName);

    QMap<QString,QVariant> map5;
    map5.insert(Stock_Details_Voucher_No,oldID);
    db->deleteRecordsPlus(map5,Stock_Details_TableName);

    itemTransaction->deleteItemTransaction(obj);

    for(CompoundItemDataObject compitem:obj->InventoryItems){
        QMap<QString,QVariant> map1;
        inventoryItemDataModel item = compitem.BaseItem;
        if(item.ItemReqUuid == "X")
            item.ItemReqUuid = db->getUUID();
        map1.insert(Stock_Details_Voucher_No,obj->voucherNumber);
        map1.insert(Stock_Details_Voucher_Prefix,obj->VoucherPrefix);
        map1.insert(Stock_Details_Voucher_Date,obj->VoucherDate);
        map1.insert(Stock_Details_Inventory_Item_ID,item.ItemID);
        map1.insert(Stock_Details_Quantity,item.quantity);
        map1.insert(Stock_Details_Uom, item.uomObject.UomName);
        map1.insert(Stock_Details_Difference, item.crQty-item.drQty);
        map1.insert(Stock_Details_Price,item.price);
        map1.insert(Stock_Detials_ItemReqUuid, item.ItemReqUuid);
        //        map1.insert(Stock_Details_PriceLevel,item.PriceLevel);
        //        map1.insert(Stock_Details_isCompoundItem,item.IsCompoundItem);
        //        map1.insert(Stock_Details_subItemOff_list_ID,0);
        //        map1.insert(Stock_Details_Discount_Percent,item.discountPercentage);
        //        map1.insert(Stock_Details_Discount_Amount,item.discount);
        map1.insert(Stock_Details_Subtotal,item.subTotal);
        //        map1.insert(Stock_Details_Project_ID,obj->ProjectId);
        map1.insert(Stock_Details_Item_Narration,item.narration);
        map1.insert(Stock_Details_Item_Description,item.ItemDescription);
        //        map1.insert(Stock_Details_Salesman_ID,obj->SalesmanID);


        ItemModel = new ItemTransactionDataModel();
        ItemModel->ItemID = item.ItemID;
        ItemModel->VoucherID = obj->voucherNumber;
        ItemModel->VoucherPrefix = obj->VoucherPrefix;
        ItemModel->VoucherDate = obj->VoucherDate;
        ItemModel->VoucherType = VoucherType;
        if(item.crQty > 0){
            ItemModel->From_Location = item.section;
            ItemModel->To_Location = "Stock";
        }
        if(item.drQty > 0){
            ItemModel->To_Location = item.section;
            ItemModel->From_Location = "Stock";
        }
        ItemModel->crQty = item.crQty;
        ItemModel->drQty = item.drQty;
        ItemModel->drAmount = item.subTotal;
        ItemModel->itemReqId = item.ItemReqUuid;
        ItemModel->Narration = obj->narration;
        ItemModel->consumed = item.consumedQty;

        itemTransaction->insertItemTransaction(ItemModel, item.godownList);

        int basid = db->insertRecordsPlus(map1,Stock_Details_TableName);

        for(inventoryItemDataModel subitem:compitem.subItems){
            QMap<QString,QVariant> map2;

            map2.insert(Stock_Details_Voucher_No,obj->voucherNumber);
            map2.insert(Stock_Details_Voucher_Prefix,obj->VoucherPrefix);
            map2.insert(Stock_Details_Voucher_Date,obj->VoucherDate);
            map2.insert(Stock_Details_Inventory_Item_ID,item.ItemID);
            map2.insert(Stock_Details_Quantity,item.quantity);
            map2.insert(Stock_Details_Uom, item.uomObject.UomName);
            map2.insert(Stock_Details_Difference, item.crQty-item.drQty);
            map2.insert(Stock_Details_Price,item.price);
            //        map1.insert(Stock_Details_PriceLevel,item.PriceLevel);
            //        map1.insert(Stock_Details_isCompoundItem,item.IsCompoundItem);
            //        map1.insert(Stock_Details_subItemOff_list_ID,0);
            //        map1.insert(Stock_Details_Discount_Percent,item.discountPercentage);
            //        map1.insert(Stock_Details_Discount_Amount,item.discount);
            map2.insert(Stock_Details_Subtotal,item.subTotal);
            //        map1.insert(Stock_Details_Project_ID,obj->ProjectId);
            map2.insert(Stock_Details_Item_Narration,item.narration);
            map2.insert(Stock_Details_Item_Description,item.ItemDescription);
            //        map1.insert(Stock_Details_Salesman_ID,obj->SalesmanID);
            db->insertRecordsPlus(map2,Stock_Details_TableName);
        }

    }

    //    QMap<QString,QVariant> ledDelMap;
    //    ledDelMap.insert(Proxy_Transaction_Helper_VoucherNo,oldID);
    //    ledDelMap.insert(Proxy_Transaction_Helper_VoucherType,VoucherType);
    //    db->deleteRecordsPlus(ledDelMap,Proxy_Transaction_Helper_TableName);

    //    for(LedgerMasterDataModel ledger:obj->ledgersList){
    //        QMap <QString,QVariant> ledMap;
    //        ledMap.insert(Proxy_Transaction_Helper_VoucherType,VoucherType);
    //        ledMap.insert(Proxy_Transaction_Helper_VoucherNo,obj->voucherNumber);
    //        ledMap.insert(Proxy_Transaction_Helper_VoucherPrefix,obj->VoucherPrefix);
    //        ledMap.insert(Proxy_Transaction_Helper_VoucherDate,obj->VoucherDate);
    //        ledMap.insert(Proxy_Transaction_Helper_ProjectID,obj->ProjectId);
    //        ledMap.insert(Proxy_Transaction_Helper_SalesManID,obj->SalesmanID);
    //        ledMap.insert(Proxy_Transaction_Helper_TranactionAmount,ledger.amount);
    //        ledMap.insert(Proxy_Transaction_Helper_CreditLedgerID,ledger.LedgerID);
    //        ledMap.insert(Proxy_Transaction_Helper_DebitLedgerID,obj->ledgerObject.LedgerID);
    //        ledMap.insert(Proxy_Transaction_Helper_Narration,obj->ledgerObject.narration);
    //        db->insertRecordsPlus(ledMap,Proxy_Transaction_Helper_TableName);
    //    }

    return commit();
}

int StockEntryDatabaseHelper::deleteVoucher(GeneralVoucherDataObject *obj)
{
    this->startTransaction();

    QMap<QString,QVariant> map3;
    map3.insert(Stock_Main_Voucher_No,obj->voucherNumber);
    db->deleteRecordsPlus(map3,Stock_Main_TableName);

    QMap<QString,QVariant> map4;
    map4.insert(Stock_Details_Voucher_No,obj->voucherNumber);
    db->deleteRecordsPlus(map4,Stock_Details_TableName);

    //    QMap<QString,QVariant> ledDelMap;
    //    ledDelMap.insert(Proxy_Transaction_Helper_VoucherNo,obj->voucherNumber);
    //    ledDelMap.insert(Proxy_Transaction_Helper_VoucherType,obj->voucherType);
    //    ledDelMap.insert(Proxy_Transaction_Helper_VoucherPrefix,obj->VoucherPrefix);
    //    db->deleteRecordsPlus(ledDelMap,Proxy_Transaction_Helper_TableName);

    itemTransaction->deleteItemTransaction(obj);

    return commit();
}

int StockEntryDatabaseHelper::commit()
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

int StockEntryDatabaseHelper::getMaxID()
{
    QString qStr = "SELECT MAX("+Stock_Main_id+") FROM " + Stock_Main_TableName;
    qDebug()<<qStr;
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
    return vNo;
}

GeneralVoucherDataObject StockEntryDatabaseHelper::getVoucherByVoucherNo(QString vNo)
{
    GeneralVoucherDataObject obj;
    SalesInventoryItemDatabaseHelper *itemHelper = new SalesInventoryItemDatabaseHelper();
    LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper();
    //    QString qStrLedgers ="SELECT "
    //            + Proxy_Transaction_Helper_id +", "
    //            + Proxy_Transaction_Helper_VoucherNo +", "
    //            + Proxy_Transaction_Helper_VoucherPrefix +", "
    //            + Proxy_Transaction_Helper_ProjectID +", "
    //            + Proxy_Transaction_Helper_TranactionAmount +", "
    //            + Proxy_Transaction_Helper_CreditLedgerID +", "
    //            + Proxy_Transaction_Helper_DebitLedgerID + ", "
    //            + Proxy_Transaction_Helper_Narration +", "
    //            + Proxy_Transaction_Helper_SalesManID
    //            + " FROM " + Proxy_Transaction_Helper_TableName
    //            + " WHERE " + Proxy_Transaction_Helper_VoucherNo + "=:id";

    QString qStrDetail = "SELECT "
            + Stock_Details_id +", "
            + Stock_Details_Voucher_No +", "
            + Stock_Details_Voucher_Prefix +", "
            + Stock_Details_Voucher_Date +", "
            + Stock_Details_Inventory_Item_ID +", "
            + Stock_Details_Price +", "
            + Stock_Details_Quantity +", "
            //            + Stock_Details_Discount_Percent +", "
            + Stock_Details_Subtotal +", "
            //            + Stock_Details_isCompoundItem +", "
            //            + Stock_Details_subItemOff_list_ID +", "
            //            + Stock_Details_PriceLevel +", "
            + Stock_Details_Item_Description +", "
            + Stock_Details_Item_Narration +" "
            + " FROM " + Stock_Details_TableName
            + " WHERE " + Stock_Details_Voucher_No + "= :vno";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    //    query.prepare(qStrLedgers);
    //    query.bindValue(":id",vNo);
    //    if(!query.exec()){
    //        qDebug()<<query.lastError();
    //        qDebug()<<query.lastQuery();
    //    }
    //    else{
    //        while(query.next()){
    //            LedgerMasterDataModel ledger;
    //            ledger = ledHelper->getLedgerObjectByID(query.value(6).toString());
    //            ledger.amount = query.value(4).toFloat();
    //            obj.ledgersList.append(ledger);
    //        }
    //    }
    query.prepare(qStrDetail);
    query.bindValue(":vno",vNo);
    if(!query.exec()){
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    else{
        while(query.next()){

            obj.voucherNumber = query.value(1).toString();
            obj.VoucherPrefix = query.value(2).toString();
            obj.VoucherDate = query.value(3).toDate();
            inventoryItemDataModel item = itemHelper->getInventoryItemByID(query.value(4).toString());
            item.price = query.value(5).toFloat();
            item.quantity = query.value(6).toFloat();
            item.subTotal = query.value(7).toFloat();
            item.PriceLevel = query.value(11).toString();
            item.narration = query.value(12).toString();
            item.ItemDescription = query.value(13).toString();

            int itemSize = obj.InventoryItems.size();
            obj.InventoryItems[itemSize-1].subItems.append(item);
            obj.InventoryItems[itemSize-1].subItemsTotal +=item.subTotal;
        }
    }
    return obj;
}

GeneralVoucherDataObject *StockEntryDatabaseHelper::getVoucherByVoucherNoPtr(QString vNo, QString voucherPrefix)
{
    GeneralVoucherDataObject *obj = new GeneralVoucherDataObject();
    SalesInventoryItemDatabaseHelper *itemHelper = new SalesInventoryItemDatabaseHelper();
    LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper();
    //    QString qStrLedgers ="SELECT "
    //            + Proxy_Transaction_Helper_id +", "
    //            + Proxy_Transaction_Helper_VoucherNo +", "
    //            + Proxy_Transaction_Helper_VoucherPrefix +", "
    //            + Proxy_Transaction_Helper_ProjectID +", "
    //            + Proxy_Transaction_Helper_TranactionAmount +", "
    //            + Proxy_Transaction_Helper_CreditLedgerID +", "
    //            + Proxy_Transaction_Helper_DebitLedgerID + ", "
    //            + Proxy_Transaction_Helper_Narration +", "
    //            + Proxy_Transaction_Helper_SalesManID
    //            + " FROM " + Proxy_Transaction_Helper_TableName
    //            + " WHERE " + Proxy_Transaction_Helper_VoucherNo + "=:id";

    QString qStrDetail = "SELECT "
            + Stock_Details_id +", "
            + Stock_Details_Voucher_No +", "
            + Stock_Details_Voucher_Prefix +", "
            + Stock_Details_Voucher_Date +", "
            + Stock_Details_Inventory_Item_ID +", "
            + Stock_Details_Price +", "
            + Stock_Details_Quantity +", "
            //            + Stock_Details_Discount_Percent +", "
            + Stock_Details_Subtotal +", "
            //            + Stock_Details_isCompoundItem +", "
            //            + Stock_Details_subItemOff_list_ID +", "
            //            + Stock_Details_PriceLevel +", "
            + Stock_Details_Item_Description +", "
            + Stock_Details_Difference +","
            + Stock_Detials_ItemReqUuid + ", "
            + Stock_Details_Item_Narration +" "
            + " FROM " + Stock_Details_TableName
            + " WHERE " + Stock_Details_Voucher_No + "= :vno"
            + " AND " + Stock_Details_Voucher_Prefix + "= :voucherprefix";


    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    //    query.prepare(qStrLedgers);
    //    query.bindValue(":id",vNo);
    //    if(!query.exec()){
    //        qDebug()<<query.lastError();
    //        qDebug()<<query.lastQuery();
    //    }
    //    else{
    //        while(query.next()){
    //            LedgerMasterDataModel ledger;
    //            ledger = ledHelper->getLedgerObjectByID(query.value(6).toString());
    //            ledger.amount = query.value(4).toFloat();
    //            obj.ledgersList.append(ledger);
    //        }
    //    }
    query.prepare(qStrDetail);
    query.bindValue(":vno",vNo);
    query.bindValue(":voucherprefix", voucherPrefix);
    if(!query.exec()){
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    else{
        while(query.next()){

            obj->voucherNumber = query.value(1).toString();
            obj->VoucherPrefix = query.value(2).toString();
            obj->VoucherDate = query.value(3).toDate();
            inventoryItemDataModel item = itemHelper->getInventoryItemByID(query.value(4).toString());
            qDebug()<<"ItemName = "<<item.ItemName;
            item.price = query.value(5).toFloat();
            item.quantity = query.value(6).toFloat();
            item.subTotal = query.value(7).toFloat();
            item.ItemDescription = query.value(8).toString();
            item.ItemReqUuid = query.value(10).toString();
            item.narration = query.value(11).toString();
            item.godownList = itemTransaction->getGodownAllocationByVoucherAndItem
                    (vNo, VoucherType, voucherPrefix,item.ItemReqUuid);

            for(GodownTransactionDataModel entry:item.godownList){
                qDebug()<<"godown entry"<<entry.fromGodown<<entry.toGodown<<entry.crQty<<entry.drQty;
            }

            if(query.value(9).toFloat()>0)
                item.crQty = query.value(9).toFloat();
            else
                item.drQty = -1 * query.value(9).toFloat();

            int itemSize = obj->InventoryItems.size();

            ItemTransactionDatabaseHelper transHelper;
            item.consumedQty = transHelper.getConsumedQtyByVoucher(item.ItemID, vNo, VoucherType, voucherPrefix, item.ItemReqUuid);

            CompoundItemDataObject comp; comp.BaseItem = item;
            obj->InventoryItems.append(comp);

            //            obj->InventoryItems[itemSize-1].subItemsTotal +=item.subTotal;
        }
    }
    obj->voucherType = VoucherType;
    return obj;
}

QList<GeneralVoucherDataObject> StockEntryDatabaseHelper::getVoucherListByDate(QDate dateFrom, QDate dateTo)
{
    QList<GeneralVoucherDataObject> vouchersList;
    QString qStr ="SELECT "
            + Stock_Main_Voucher_No+ ", "
            + Stock_Main_Voucher_Prefix+ ", "
            + Stock_Main_Voucher_Date+", "
            + Stock_Main_Ledger_ID+ ", "
            + Stock_Main_Total
            + " FROM " + Stock_Main_TableName
            + " WHERE "
            + Stock_Main_Voucher_Date
            + " >= '"
            + dateFrom.toString("yyyy-MM-dd")
            + "' AND "
            + Stock_Main_Voucher_Date
            + " <= '"
            + dateTo.toString("yyyy-MM-dd")
            + "'"
            ;
    LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper();
    int i;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    qDebug()<<qStr;
    query.prepare(qStr);
    //    query.bindValue(":dateFrom",dateFrom.toString("yyyy-MM-dd"));
    //    query.bindValue("dateTo",dateTo.toString("yyyy-MM-dd"));
    if(!query.exec()){
        qDebug()<<query.lastQuery();
        qDebug()<<query.lastError();
    }
    else{
        while(query.next()){
            GeneralVoucherDataObject item;
            i =0;
            item.voucherNumber = query.value(i++).toString();
            item.VoucherPrefix = query.value(i++).toString();
            item.VoucherDate = query.value(i++).toDate();
            item.ledgerObject.LedgerID = query.value(i++).toString();
            item.ledgerObject.LedgerName = ledHelper->getLedgerNameByID(item.ledgerObject.LedgerID );
            item.grandTotal = query.value(i++).toFloat();
            vouchersList.append(item);
        }
    }
    qDebug()<<"Query Size = "<<query.size();

    return vouchersList;
}

QSqlQueryModel *StockEntryDatabaseHelper::getVoucherListByDateModel(QDate dateFrom, QDate dateTo)
{
    LedgerMasterDatabaseHelper ledHelper;
    QString qStr ="SELECT ";
    qStr += "'V' , ";
    qStr += Stock_Main_Voucher_No +", ";
    qStr += "'" + VoucherType + "', " ;
    qStr += Stock_Main_Voucher_Prefix + ", ";
    qStr += Stock_Main_Voucher_Date+" as 'Voucher Date', ";
    qStr += " CONCAT(" + Stock_Main_Voucher_Prefix + ", '-', " + Stock_Main_Voucher_No+ ") as 'Voucher No', ";
    qStr += "(SELECT "+ledHelper.Ledger_Master_LedgerName + " FROM "+ ledHelper.Ledger_Master_TableName;
    qStr += " WHERE "+ledHelper.Ledger_Master_LedgerID + "= som.";
    qStr += Stock_Main_Ledger_ID + ") Ledger, ";
    qStr += Stock_Main_Total ;
    qStr += " FROM " + Stock_Main_TableName;
    qStr += " som WHERE ";
    qStr += Stock_Main_Voucher_Date;
    qStr += " >= '";
    qStr += dateFrom.toString("yyyy-MM-dd");
    qStr += "' AND ";
    qStr += Stock_Main_Voucher_Date;
    qStr += " <= '";
    qStr += dateTo.toString("yyyy-MM-dd");
    qStr += "'";

    return DatabaseHelper::getQueryModel(qStr);

}

QSqlQueryModel *StockEntryDatabaseHelper::getVoucherListByDateTimeModel(QDateTime dateFrom, QDateTime dateTo)
{

}

QPair<QString, QString> StockEntryDatabaseHelper::getVoucherNoByTransactionId(QString transId)
{

}

QString StockEntryDatabaseHelper::getVoucherNumberByLedID(QString LedID, QDate Date)
{

}
