#include "materialinoutdatabasehelper.h"

materialinoutDatabaseHelper::materialinoutDatabaseHelper()
{
    db = new DatabaseHelper();
    itemTransaction = new ItemTransactionDatabaseHelper(db);
}

materialinoutDatabaseHelper::~materialinoutDatabaseHelper()
{

}

int materialinoutDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

QString materialinoutDatabaseHelper::getNextVoucherNo(QString voucherPrefix)
{
    GeneralVoucherDatabaseHelper vHelper;
    QString voucherNumber = vHelper.getNextVoucherNoByType(ledgerType, voucherPrefix, Material_In_Main_TableName);
    return voucherNumber;

}


int materialinoutDatabaseHelper::insertMaterialInOut(GeneralVoucherDataObject *obj)
{
    this->startTransaction();
    QMap<QString,QVariant> map;

    GeneralVoucherDatabaseHelper vHelper;
    obj->voucherNumber = vHelper.setNextVoucherNoByType(ledgerType, obj->VoucherPrefix, Material_In_Main_TableName );

    map.insert(Material_In_Main_Voucher_No,obj->voucherNumber);
    map.insert(Material_In_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Material_In_Main_Date_Created,obj->VoucherDate);
    map.insert(Material_In_Main_Voucher_Date,obj->VoucherDate);
    map.insert(Material_In_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    map.insert(Material_In_Main_Billing_Name,obj->ledgerObject.LedgerName);
    map.insert(Material_In_Main_Total,obj->grandTotal);
    map.insert(Material_In_Main_Narration,obj->narration);
    map.insert(Material_In_Main_Added_By,obj->AddedBy);
    map.insert(Material_In_Main_Delivery_Date,obj->DeliveryDate);
    map.insert(Material_In_Main_Completion_Probability,obj->CompletionProbability);
    map.insert(Material_In_Main_Credit_Period,obj->ledgerObject.Credit_Period);
    map.insert(Material_In_Main_POC_Name,obj->ledgerObject.ContactPersonName);
    map.insert(Material_In_Main_POC_Phone,obj->ledgerObject.ContactPersonNumber);
    map.insert(Material_In_Main_POC_Email,obj->ledgerObject.ContactPersonEmail);
    map.insert(Material_In_Main_Project_ID,obj->ProjectId);
    map.insert(Material_In_Main_Revision_Number,obj->RevisionNo);
    map.insert(Material_In_Main_Converted_To_Sales_Order,obj->ConvertedToSalesOrder);
    map.insert(Material_In_Main_Quotation_Dropped,obj->QuotationDropped);
    map.insert(Material_In_Main_Quotation_Dropped_Reason,obj->QuotationDroppedReason);
    map.insert(Material_In_Main_Requirement_Voucher_No,obj->RequirementVoucherNo);
    map.insert(Material_In_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Material_In_Main_TimeStampC,obj->timestamp);
    map.insert(Material_In_Main_Last_Updated,QDateTime::currentDateTime());
    map.insert(Material_In_Main_VoucherStatus, obj->status);


    db->insertRecordsPlus(map,Material_In_Main_TableName);

    for(CompoundItemDataObject compitem:obj->InventoryItems){
        QMap<QString,QVariant> map1;
        inventoryItemDataModel item = compitem.BaseItem;
        if(item.ItemReqUuid == "X"){
            item.ItemReqUuid = db->getUUID();
        }
        map1.insert(Material_In_Details_Voucher_No,obj->voucherNumber);
        map1.insert(Material_In_Details_Voucher_Prefix,obj->VoucherPrefix);
        map1.insert(Material_In_Details_Voucher_Date,obj->VoucherDate);
        map1.insert(Material_In_Details_Inventory_Item_ID,item.ItemID);
        map1.insert(Material_In_Details_Quantity,item.quantity);
        map1.insert(Material_In_Details_UOMId, item.uomObject._id);
        map1.insert(Material_In_Details_CrQty,item.crQty);
        map1.insert(Material_In_Details_DrQty,item.drQty);
        map1.insert(Material_In_Details_Price,item.price);
        map1.insert(Material_In_Details_PriceLevel,item.PriceLevel);
        map1.insert(Material_In_Details_isCompoundItem,item.IsCompoundItem);
        map1.insert(Material_In_Details_subItemOff_list_ID,0);
        map1.insert(Material_In_Details_Discount_Percent,item.discountPercentage);
        map1.insert(Material_In_Details_Discount_Amount,item.discount);
        map1.insert(Material_In_Details_Subtotal,item.subTotal);
        map1.insert(Material_In_Details_Project_ID,obj->ProjectId);
        map1.insert(Material_In_Details_Item_Narration,item.narration);
        map1.insert(Material_In_Details_Item_Description,item.ItemDescription);
        map1.insert(Material_In_Details_Salesman_ID,obj->SalesmanID);
        map1.insert(Material_In_Details_Requirement_ItemID,item.ItemReqUuid);

        int basid = db->insertRecordsPlus(map1,Material_In_Details_TableName);

        ItemModel = new ItemTransactionDataModel();
        ItemModel->ItemID = item.ItemID;
        ItemModel->VoucherID = obj->voucherNumber;
        ItemModel->VoucherPrefix = obj->VoucherPrefix;
        ItemModel->VoucherDate = obj->VoucherDate;
        ItemModel->VoucherType = VoucherType;
        if(item.crQty>0){
            ItemModel->From_Location = item.section;
            ItemModel->To_Location = "OUT";
        }
        else{
            ItemModel->From_Location = "OUT";
            ItemModel->To_Location = item.section;
        }
        ItemModel->crQty = item.crQty;
        ItemModel->drQty = item.drQty;
        ItemModel->itemReqId = item.ItemReqUuid;
        ItemModel->Narration = obj->narration;
        ItemModel->consumed = item.consumedQty;

        itemTransaction->insertItemTransaction(ItemModel, item.godownList);

        for(inventoryItemDataModel subitem:compitem.subItems){
            QMap<QString,QVariant> map2;
            if(subitem.ItemReqUuid == "X"){
                subitem.ItemReqUuid = db->getUUID();
            }
            map2.insert(Material_In_Details_Voucher_No,obj->voucherNumber);
            map2.insert(Material_In_Details_Voucher_Prefix,obj->VoucherPrefix);
            map2.insert(Material_In_Details_Voucher_Date,obj->VoucherDate);
            map2.insert(Material_In_Details_Inventory_Item_ID,subitem.ItemID);
            map2.insert(Material_In_Details_Quantity,subitem.quantity);
            map2.insert(Material_In_Details_Price,subitem.price);
            map2.insert(Material_In_Details_PriceLevel,subitem.PriceLevel);
            map2.insert(Material_In_Details_isCompoundItem,subitem.IsCompoundItem);
            map2.insert(Material_In_Details_subItemOff_list_ID,basid);
            map2.insert(Material_In_Details_Discount_Percent,subitem.discountPercentage);
            map2.insert(Material_In_Details_Discount_Amount,subitem.discount);
            map2.insert(Material_In_Details_Subtotal,subitem.subTotal);
            map2.insert(Material_In_Details_Project_ID,obj->ProjectId);
            map2.insert(Material_In_Details_Item_Narration,subitem.narration);
            map2.insert(Material_In_Details_Item_Description,subitem.ItemDescription);
            map2.insert(Material_In_Details_Salesman_ID,obj->SalesmanID);

            db->insertRecordsPlus(map2,Material_In_Details_TableName);
        }
    }
    for(LedgerMasterDataModel ledger:obj->ledgersList){
        QMap <QString,QVariant> ledMap;
        ledMap.insert(Proxy_Transaction_Helper_VoucherType,ledgerType);
        ledMap.insert(Proxy_Transaction_Helper_VoucherNo,obj->voucherNumber);
        ledMap.insert(Proxy_Transaction_Helper_VoucherPrefix,obj->VoucherPrefix);
        ledMap.insert(Proxy_Transaction_Helper_VoucherDate,obj->VoucherDate);
        ledMap.insert(Proxy_Transaction_Helper_ProjectID,obj->ProjectId);
        ledMap.insert(Proxy_Transaction_Helper_SalesManID,obj->SalesmanID);
        ledMap.insert(Proxy_Transaction_Helper_TranactionAmount,ledger.amount);
        ledMap.insert(Proxy_Transaction_Helper_CreditLedgerID,ledger.LedgerID);
        ledMap.insert(Proxy_Transaction_Helper_DebitLedgerID,obj->ledgerObject.LedgerID);
        ledMap.insert(Proxy_Transaction_Helper_Narration,obj->ledgerObject.narration);
        db->insertRecordsPlus(ledMap,Proxy_Transaction_Helper_TableName);
    }

    return commit();
}

int materialinoutDatabaseHelper::updateMaterialInOut(GeneralVoucherDataObject *obj,
                                                     QString oldID)
{
    this->startTransaction();
//    QMap<QString,QVariant> map5;
//    map5.insert(Material_In_Main_Voucher_No,oldID);
//    map5.insert(Material_In_Main_Voucher_Prefix,obj->VoucherPrefix);
//    db->deleteRecordsPlus(map5,Material_In_Main_TableName);

    QMap<QString,QVariant> map;
    map.insert(Material_In_Main_Voucher_No,obj->voucherNumber);
    map.insert(Material_In_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Material_In_Main_Date_Created,obj->VoucherDate);
    map.insert(Material_In_Main_Voucher_Date,obj->VoucherDate);
    map.insert(Material_In_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    map.insert(Material_In_Main_Billing_Name,obj->ledgerObject.LedgerName);
    map.insert(Material_In_Main_Total,obj->grandTotal);
    map.insert(Material_In_Main_Narration,obj->narration);
    map.insert(Material_In_Main_Added_By,obj->AddedBy);
    map.insert(Material_In_Main_Delivery_Date,obj->DeliveryDate);
    map.insert(Material_In_Main_Completion_Probability,obj->CompletionProbability);
    map.insert(Material_In_Main_Credit_Period,obj->ledgerObject.Credit_Period);
    map.insert(Material_In_Main_POC_Name,obj->ledgerObject.ContactPersonName);
    map.insert(Material_In_Main_POC_Phone,obj->ledgerObject.ContactPersonNumber);
    map.insert(Material_In_Main_POC_Email,obj->ledgerObject.ContactPersonEmail);
    map.insert(Material_In_Main_Project_ID,obj->ProjectId);
    map.insert(Material_In_Main_Revision_Number,obj->RevisionNo);
    map.insert(Material_In_Main_Converted_To_Sales_Order,obj->ConvertedToSalesOrder);
    map.insert(Material_In_Main_Quotation_Dropped,obj->QuotationDropped);
    map.insert(Material_In_Main_Quotation_Dropped_Reason,obj->QuotationDroppedReason);
    map.insert(Material_In_Main_Requirement_Voucher_No,obj->RequirementVoucherNo);
    map.insert(Material_In_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Material_In_Main_Last_Updated,QDateTime::currentDateTime());
    map.insert(Material_In_Main_VoucherStatus, obj->status);



    //    QMap<QString,QVariant> map3;
    //    map3.insert(Material_In_Main_id,oldID);
    //    db->updateRecordsPlus(map,map3,Material_In_Main_TableName);

    QMap<QString,QVariant> map6;
    map6.insert(Material_In_Main_Voucher_No,oldID);
    map6.insert(Material_In_Main_Voucher_Prefix,obj->VoucherPrefix);
    db->updateRecordsPlus(map,map6,Material_In_Main_TableName);

    db->deleteRecordsPlus(map6,Material_In_Details_TableName);

    for(CompoundItemDataObject compitem:obj->InventoryItems){
        QMap<QString,QVariant> map1;
        inventoryItemDataModel item = compitem.BaseItem;
        if(item.ItemReqUuid == "X"){
            item.ItemReqUuid = db->getUUID();
        }
        map1.insert(Material_In_Details_Voucher_No,obj->voucherNumber);
        map1.insert(Material_In_Details_Voucher_Prefix,obj->VoucherPrefix);
        map1.insert(Material_In_Details_Voucher_Date,obj->VoucherDate);
        map1.insert(Material_In_Details_Inventory_Item_ID,item.ItemID);
        map1.insert(Material_In_Details_Quantity,item.quantity);
        map1.insert(Material_In_Details_UOMId, item.uomObject._id);
        map1.insert(Material_In_Details_CrQty,item.crQty);
        map1.insert(Material_In_Details_DrQty,item.drQty);
        map1.insert(Material_In_Details_Price,item.price);
        map1.insert(Material_In_Details_PriceLevel,item.PriceLevel);
        map1.insert(Material_In_Details_isCompoundItem,item.IsCompoundItem);
        map1.insert(Material_In_Details_subItemOff_list_ID,0);
        map1.insert(Material_In_Details_Discount_Percent,item.discountPercentage);
        map1.insert(Material_In_Details_Discount_Amount,item.discount);
        map1.insert(Material_In_Details_Subtotal,item.subTotal);
        map1.insert(Material_In_Details_Project_ID,obj->ProjectId);
        map1.insert(Material_In_Details_Item_Narration,item.narration);
        map1.insert(Material_In_Details_Item_Description,item.ItemDescription);
        map1.insert(Material_In_Details_Salesman_ID,obj->SalesmanID);
        map1.insert(Material_In_Details_Requirement_ItemID,item.ItemReqUuid);

        int basid = db->insertRecordsPlus(map1,Material_In_Details_TableName);

        ItemModel = new ItemTransactionDataModel();
        ItemModel->ItemID = item.ItemID;
        ItemModel->VoucherID = obj->voucherNumber;
        ItemModel->VoucherPrefix = obj->VoucherPrefix;
        ItemModel->VoucherDate = obj->VoucherDate;
        ItemModel->VoucherType = VoucherType;
        if(item.crQty>0){
            ItemModel->From_Location = item.section;
            ItemModel->To_Location = "OUT";
        }
        else{
            ItemModel->From_Location = "OUT";
            ItemModel->To_Location = item.section;
        }
        ItemModel->crQty = item.crQty;
        ItemModel->drQty = item.drQty;
        ItemModel->itemReqId = item.ItemReqUuid;
        ItemModel->Narration = obj->narration;
        ItemModel->consumed = item.consumedQty;

        itemTransaction->insertItemTransaction(ItemModel, item.godownList);

        for(inventoryItemDataModel subitem:compitem.subItems){
            QMap<QString,QVariant> map2;
            if(subitem.ItemReqUuid == "X"){
                subitem.ItemReqUuid = db->getUUID();
            }
            map2.insert(Material_In_Details_Voucher_No,obj->voucherNumber);
            map2.insert(Material_In_Details_Voucher_Prefix,obj->VoucherPrefix);
            map2.insert(Material_In_Details_Voucher_Date,obj->VoucherDate);
            map2.insert(Material_In_Details_Inventory_Item_ID,subitem.ItemID);
            map2.insert(Material_In_Details_Quantity,subitem.quantity);
            map2.insert(Material_In_Details_Price,subitem.price);
            map2.insert(Material_In_Details_PriceLevel,subitem.PriceLevel);
            map2.insert(Material_In_Details_isCompoundItem,subitem.IsCompoundItem);
            map2.insert(Material_In_Details_subItemOff_list_ID,basid);
            map2.insert(Material_In_Details_Discount_Percent,subitem.discountPercentage);
            map2.insert(Material_In_Details_Discount_Amount,subitem.discount);
            map2.insert(Material_In_Details_Subtotal,subitem.subTotal);
            map2.insert(Material_In_Details_Project_ID,obj->ProjectId);
            map2.insert(Material_In_Details_Item_Narration,subitem.narration);
            map2.insert(Material_In_Details_Item_Description,subitem.ItemDescription);
            map2.insert(Material_In_Details_Salesman_ID,obj->SalesmanID);

            db->insertRecordsPlus(map2,Material_In_Details_TableName);
        }
    }
    for(LedgerMasterDataModel ledger:obj->ledgersList){
        QMap <QString,QVariant> ledMap;
        ledMap.insert(Proxy_Transaction_Helper_VoucherType,ledgerType);
        ledMap.insert(Proxy_Transaction_Helper_VoucherNo,obj->voucherNumber);
        ledMap.insert(Proxy_Transaction_Helper_VoucherPrefix,obj->VoucherPrefix);
        ledMap.insert(Proxy_Transaction_Helper_VoucherDate,obj->VoucherDate);
        ledMap.insert(Proxy_Transaction_Helper_ProjectID,obj->ProjectId);
        ledMap.insert(Proxy_Transaction_Helper_SalesManID,obj->SalesmanID);
        ledMap.insert(Proxy_Transaction_Helper_TranactionAmount,ledger.amount);
        ledMap.insert(Proxy_Transaction_Helper_CreditLedgerID,ledger.LedgerID);
        ledMap.insert(Proxy_Transaction_Helper_DebitLedgerID,obj->ledgerObject.LedgerID);
        ledMap.insert(Proxy_Transaction_Helper_Narration,obj->ledgerObject.narration);
        db->insertRecordsPlus(ledMap,Proxy_Transaction_Helper_TableName);
    }

    return commit();
}

int materialinoutDatabaseHelper::deleteMaterialInOut(GeneralVoucherDataObject *obj)
{
    this->startTransaction();
    QMap<QString,QVariant> map3;
    map3.insert(Material_In_Main_Voucher_No,obj->voucherNumber);
    map3.insert(Material_In_Main_Voucher_Prefix,obj->VoucherPrefix);

    db->deleteRecordsPlus(map3,Material_In_Main_TableName);

    QMap<QString,QVariant> map4;
    map4.insert(Material_In_Details_Voucher_No,obj->voucherNumber);
    map3.insert(Material_In_Details_Voucher_Prefix,obj->VoucherPrefix);
    db->deleteRecordsPlus(map4,Material_In_Details_TableName);
    return commit();
}

int materialinoutDatabaseHelper::commit()
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

int materialinoutDatabaseHelper::getMaxID()
{
    QString qStr = "SELECT MAX("+Material_In_Main_id+") FROM " + Material_In_Main_TableName;
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

GeneralVoucherDataObject materialinoutDatabaseHelper::getVoucherByVoucherNo(QString vNo)
{
    qDebug()<<"256";
    GeneralVoucherDataObject obj;
    SalesInventoryItemDatabaseHelper *itemHelper = new SalesInventoryItemDatabaseHelper();
    LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper();
    QString qStrLedgers ="SELECT "
            + Proxy_Transaction_Helper_id +", "
            + Proxy_Transaction_Helper_VoucherNo +", "
            + Proxy_Transaction_Helper_VoucherPrefix +", "
            + Proxy_Transaction_Helper_ProjectID +", "
            + Proxy_Transaction_Helper_TranactionAmount +", "
            + Proxy_Transaction_Helper_CreditLedgerID +", "
            + Proxy_Transaction_Helper_DebitLedgerID + ", "
            + Proxy_Transaction_Helper_Narration +", "
            + Proxy_Transaction_Helper_SalesManID
            + " FROM " + Proxy_Transaction_Helper_TableName
            + " WHERE " + Proxy_Transaction_Helper_VoucherNo + "=:id";

    QString qStrDetail = "SELECT "
            + Material_In_Details_id +", "
            + Material_In_Details_Voucher_No +", "
            + Material_In_Details_Voucher_Prefix +", "
            + Material_In_Details_Voucher_Date +", "
            + Material_In_Details_Inventory_Item_ID +", "
            + Material_In_Details_Price +", "
            + Material_In_Details_Quantity +", "
            + Material_In_Details_Discount_Percent +", "
            + Material_In_Details_Subtotal +", "
            + Material_In_Details_isCompoundItem +", "
            + Material_In_Details_subItemOff_list_ID +", "
            + Material_In_Details_PriceLevel +", "
            + Material_In_Details_Item_Description +", "
            + Material_In_Details_Item_Narration +" "
            + " FROM " + Material_In_Details_TableName
            + " WHERE " + Material_In_Details_Voucher_No + "= :vno";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStrLedgers);
    query.bindValue(":id",vNo);
    qDebug()<<"294";
    if(!query.exec()){
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    else{
        while(query.next()){
            LedgerMasterDataModel ledger;
            ledger = ledHelper->getLedgerObjectByID(query.value(6).toString());
            ledger.amount = query.value(4).toFloat();
            obj.ledgersList.append(ledger);
        }
        qDebug()<<"305";
    }
    query.prepare(qStrDetail);
    query.bindValue(":vno",vNo);
    if(!query.exec()){
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    else{
        while(query.next()){
            qDebug()<<"317";
            inventoryItemDataModel item = itemHelper->getInventoryItemByID(query.value(4).toString());
            item.price = query.value(5).toFloat();
            item.quantity = query.value(6).toFloat();
            item.discountPercentage = query.value(7).toFloat();
            item.subTotal = query.value(8).toFloat();
            item.PriceLevel = query.value(11).toString();
            item.narration = query.value(12).toString();
            item.ItemDescription = query.value(13).toString();
            item.listId = query.value(0).toInt();
            qDebug()<<"327"<<query.value(9).toBool();
            if(query.value(9).toBool()||(query.value(10).toInt()==0)){
                CompoundItemDataObject cObj;
                cObj.BaseItem = item;
                obj.InventoryItems.append(cObj);
                qDebug()<<"332";
            }
            else{

                int itemSize = obj.InventoryItems.size();
                qDebug()<<itemSize;
                obj.InventoryItems[itemSize-1].subItems.append(item);
                obj.InventoryItems[itemSize-1].subItemsTotal +=item.subTotal;
                qDebug()<<"338";
            }
        }
    }
    qDebug()<<"342";
    return obj;
}

GeneralVoucherDataObject *materialinoutDatabaseHelper::getVoucherByVoucherNoPtr(QString vNo, QString voucherPrefix)
{
    qDebug()<<"256";
    GeneralVoucherDataObject *obj = new GeneralVoucherDataObject;
    SalesInventoryItemDatabaseHelper *itemHelper = new SalesInventoryItemDatabaseHelper();

    LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper();

    QString qStrMain = " SELECT "
            + Material_In_Main_Voucher_No + ", "
            + Material_In_Main_Voucher_Prefix + ", "
            + Material_In_Main_Voucher_Date + ", "
            + Material_In_Main_VoucherStatus + ", "
            + Material_In_Main_Salesman_ID + " "
            + " FROM " + Material_In_Main_TableName
            + " WHERE " + Material_In_Main_Voucher_No + " =:id"
            + " AND " + Material_In_Main_Voucher_Prefix + " =:vPrefix";

    QString qStrLedgers ="SELECT "
            + Proxy_Transaction_Helper_id +", "
            + Proxy_Transaction_Helper_VoucherNo +", "
            + Proxy_Transaction_Helper_VoucherPrefix +", "
            + Proxy_Transaction_Helper_ProjectID +", "
            + Proxy_Transaction_Helper_TranactionAmount +", "
            + Proxy_Transaction_Helper_CreditLedgerID +", "
            + Proxy_Transaction_Helper_DebitLedgerID + ", "
            + Proxy_Transaction_Helper_Narration +", "
            + Proxy_Transaction_Helper_SalesManID
            + " FROM " + Proxy_Transaction_Helper_TableName
            + " WHERE " + Proxy_Transaction_Helper_VoucherNo + "=:id";

    QString qStrDetail = "SELECT "
            + Material_In_Details_id +", "
            + Material_In_Details_Voucher_No +", "
            + Material_In_Details_Voucher_Prefix +", "
            + Material_In_Details_Voucher_Date +", "
            + Material_In_Details_Inventory_Item_ID +", "
            + Material_In_Details_Price +", "
            + Material_In_Details_Quantity +", "
            + Material_In_Details_CrQty +", "
            + Material_In_Details_DrQty +", "
            + Material_In_Details_Discount_Percent +", "
            + Material_In_Details_Subtotal +", "
            + Material_In_Details_isCompoundItem +", "
            + Material_In_Details_subItemOff_list_ID +", "
            + Material_In_Details_PriceLevel +", "
            + Material_In_Details_Item_Description +", "
            + Material_In_Details_Item_Narration +" "
            + " FROM " + Material_In_Details_TableName
            + " WHERE " + Material_In_Details_Voucher_No + "= :vno";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStrLedgers);
    query.bindValue(":id",vNo);
    qDebug()<<"294";
    if(!query.exec()){
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    else{
        while(query.next()){
            LedgerMasterDataModel ledger;
            ledger = ledHelper->getLedgerObjectByID(query.value(6).toString());
            ledger.amount = query.value(4).toFloat();
            obj->ledgersList.append(ledger);
        }
        qDebug()<<"305";
    }
    query.prepare(qStrDetail);
    query.bindValue(":vno",vNo);
    if(!query.exec()){
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    else{
        while(query.next()){
            qDebug()<<"317";
            inventoryItemDataModel item = itemHelper->getInventoryItemByID(query.value(4).toString());
            item.price = query.value(5).toFloat();
            item.quantity = query.value(6).toFloat();
            item.crQty = query.value(7).toFloat();
            item.drQty = query.value(8).toFloat();
            item.discountPercentage = query.value(9).toFloat();
            item.subTotal = query.value(10).toFloat();
            item.PriceLevel = query.value(13).toString();
            item.narration = query.value(15).toString();
            item.ItemDescription = query.value(14).toString();
            item.listId = query.value(0).toInt();
            qDebug()<<"327"<<query.value(11).toBool();
            if(query.value(11).toBool()||(query.value(12).toInt()==0)){
                CompoundItemDataObject cObj;
                cObj.BaseItem = item;
                obj->InventoryItems.append(cObj);
                qDebug()<<"332";
            }
            else{

                int itemSize = obj->InventoryItems.size();
                qDebug()<<itemSize;
                obj->InventoryItems[itemSize-1].subItems.append(item);
                obj->InventoryItems[itemSize-1].subItemsTotal +=item.subTotal;
                qDebug()<<"338";
            }
        }
    }
    query.prepare(qStrMain);
    query.bindValue(":id",vNo);
    query.bindValue(":vPrefix",voucherPrefix);
    if(!query.exec()){
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    else{
        while(query.next()){
            obj->voucherNumber = query.value(0).toString();
            obj->VoucherPrefix = query.value(1).toString();
            obj->voucherType = VoucherType;
            obj->VoucherDate = query.value(2).toDate();
            obj->status = query.value(3).toInt();
            obj->SalesmanID = query.value(4).toInt();


        }
    }
    qDebug()<<"342";
    return obj;
}

QList<GeneralVoucherDataObject> materialinoutDatabaseHelper::getVoucherListByDate(QDate dateFrom, QDate dateTo)
{
    QList<GeneralVoucherDataObject> vouchersList;
    QString qStr ="SELECT "
            + Material_In_Main_Voucher_No+ ", "
            + Material_In_Main_Voucher_Prefix+ ", "
            + Material_In_Main_Voucher_Date+", "
            + Material_In_Main_Ledger_ID+ ", "
            + Material_In_Main_Total
            + " FROM " + Material_In_Main_TableName
            + " WHERE "
            + Material_In_Main_Voucher_Date
            + " >= '"
            + dateFrom.toString("yyyy-MM-dd")
            + "' AND "
            + Material_In_Main_Voucher_Date
            + " <= '"
            + dateTo.toString("yyyy-MM-dd")
            + "'"
            ;
    LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper();
    int i;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

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

QSqlQueryModel *materialinoutDatabaseHelper::getVoucherListByDateModel(QDate dateFrom, QDate dateTo)
{
    LedgerMasterDatabaseHelper ledHelper;
    QString qStr ="SELECT ";
    qStr += "'V' , ";
    qStr += Material_In_Main_Voucher_No +", ";
    qStr += "'" + VoucherType + "', " ;
    qStr += Material_In_Main_Voucher_Prefix + ", ";
    qStr += Material_In_Main_Voucher_Date+" as 'Voucher Date', ";
    qStr += " CONCAT(" + Material_In_Main_Voucher_Prefix + ", '-', " + Material_In_Main_Voucher_No+ ") as 'Voucher No', ";
    qStr += "(SELECT "+ledHelper.Ledger_Master_LedgerName + " FROM "+ ledHelper.Ledger_Master_TableName;
    qStr += " WHERE "+ledHelper.Ledger_Master_LedgerID + "= som.";
    qStr += Material_In_Main_Ledger_ID + ") Ledger, ";
    qStr += Material_In_Main_Total ;
    qStr += " FROM " + Material_In_Main_TableName;
    qStr += " som WHERE ";
    qStr += Material_In_Main_Voucher_Date;
    qStr += " >= '";
    qStr += dateFrom.toString("yyyy-MM-dd");
    qStr += "' AND ";
    qStr += Material_In_Main_Voucher_Date;
    qStr += " <= '";
    qStr += dateTo.toString("yyyy-MM-dd");
    qStr += "'";
    return DatabaseHelper::getQueryModel(qStr);

}
