#include "godowntransferdatabasehelper.h"

GodownTransferDatabaseHelper::GodownTransferDatabaseHelper()
{
    db = new DatabaseHelper();
    itemTransaction = new ItemTransactionDatabaseHelper(db);
    ItemModel = new ItemTransactionDataModel();
    ledTransaction = new TransactionHelper();
}

int GodownTransferDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

QString GodownTransferDatabaseHelper::getNextVoucherNo(QString voucherPrefix)
{
    GeneralVoucherDatabaseHelper vHelper;
    QString voucherNumber = vHelper.getNextVoucherNoByType(VoucherType, voucherPrefix, Godown_Transfer_Main_TableName);
    return voucherNumber;

}

int GodownTransferDatabaseHelper::insertGodownTransfer(GeneralVoucherDataObject *obj)
{
    this->startTransaction();

    QMap<QString,QVariant> map;
    GeneralVoucherDatabaseHelper vHelper;
    obj->voucherNumber = vHelper.setNextVoucherNoByType(VoucherType, obj->VoucherPrefix, Godown_Transfer_Main_TableName );
    qDebug()<<"New voucher is"<<obj->voucherNumber;
    map.insert(Godown_Transfer_Main_Voucher_No,obj->voucherNumber);
    map.insert(Godown_Transfer_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Godown_Transfer_Main_Date_Created,obj->VoucherDate);
    map.insert(Godown_Transfer_Main_Voucher_Date,obj->VoucherDate);
    map.insert(Godown_Transfer_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    map.insert(Godown_Transfer_Main_Billing_Name,obj->BillingName);
    map.insert(Godown_Transfer_Main_Total,obj->grandTotal);
    map.insert(Godown_Transfer_Main_Narration,obj->narration);
    map.insert(Godown_Transfer_Main_Added_By,obj->AddedBy);
    map.insert(Godown_Transfer_Main_Delivery_Date,obj->DeliveryDate);
    map.insert(Godown_Transfer_Main_Completion_Probability,obj->CompletionProbability);
    map.insert(Godown_Transfer_Main_Credit_Period,obj->ledgerObject.Credit_Period);
    map.insert(Godown_Transfer_Main_POC_Name,obj->ledgerObject.ContactPersonName);
    map.insert(Godown_Transfer_Main_POC_Phone,obj->ledgerObject.ContactPersonNumber);
    map.insert(Godown_Transfer_Main_POC_Email,obj->ledgerObject.ContactPersonEmail);
    map.insert(Godown_Transfer_Main_Project_ID,obj->ProjectId);
    map.insert(Godown_Transfer_Main_Revision_Number,obj->RevisionNo);
    map.insert(Godown_Transfer_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Godown_Transfer_Main_TimeStamp,obj->timestamp);
    map.insert(Godown_Transfer_Main_Last_Updated,QDateTime::currentDateTime());
    map.insert(Godown_Transfer_Main_FromLocation, obj->fromGodownID);
    map.insert(Godown_Transfer_Main_ToLocation, obj->toGodownID);
    map.insert(Godown_Transfer_Main_VoucherStatus, obj->status);
//    map.insert(Godown_Transfer_Main_ReqVoucherList,obj->ReqVoucherList);



    qint64 lastId= db->insertRecordsPlus(map,Godown_Transfer_Main_TableName);

    qDebug()<<"New ID = "<<lastId;

    qDebug()<<"New ID updated = "<<lastId;


    for(CompoundItemDataObject compitem:obj->InventoryItems){
        QMap<QString,QVariant> map1;
        inventoryItemDataModel item = compitem.BaseItem;
        if(item.ItemReqUuid == "X"){
            item.ItemReqUuid = db->getUUID();
        }
        map1.insert(Godown_Transfer_Details_Voucher_No,obj->voucherNumber);
        map1.insert(Godown_Transfer_Details_Voucher_Prefix,obj->VoucherPrefix);
        map1.insert(Godown_Transfer_Details_Voucher_Date,obj->VoucherDate);
        map1.insert(Godown_Transfer_Details_Inventory_Item_ID,item.ItemID);
        map1.insert(Godown_Transfer_Details_Quantity,item.quantity);
        map1.insert(Godown_Transfer_Details_Price,item.price);
        map1.insert(Godown_Transfer_Details_PriceLevel,item.PriceLevel);
        map1.insert(Godown_Transfer_Details_isCompoundItem,item.IsCompoundItem);
        map1.insert(Godown_Transfer_Details_subItemOff_list_ID,0);
        map1.insert(Godown_Transfer_Details_Discount_Percent,item.discountPercentage);
        map1.insert(Godown_Transfer_Details_Discount_Amount,item.discountinAmount);
        map1.insert(Godown_Transfer_Details_Subtotal,item.subTotal);
        map1.insert(Godown_Transfer_Details_Project_ID,obj->ProjectId);
        map1.insert(Godown_Transfer_Details_Item_Narration,item.narration);
        map1.insert(Godown_Transfer_Details_Item_Description,item.ItemDescription);
        map1.insert(Godown_Transfer_Details_Salesman_ID,obj->SalesmanID);
        map1.insert(Godown_Transfer_Details_Requirement_ItemID,item.ItemReqUuid);
        map1.insert(Godown_Transfer_Details_Uom, item.uomObject._id);
        map1.insert(Godown_Transfer_Details_Time, QTime::currentTime());


        ItemModel->ItemID = item.ItemID;
        ItemModel->From_Ledger = item.defaultPurchaseLedgerID;
        ItemModel->To_Ledger = obj->ledgerObject.LedgerID;
        ItemModel->VoucherID = obj->voucherNumber;
        ItemModel->VoucherPrefix = obj->VoucherPrefix;
        ItemModel->VoucherDate = obj->VoucherDate;
        ItemModel->VoucherType = VoucherType;
        ItemModel->From_Location = obj->fromGodownID;
        ItemModel->To_Location = obj->toGodownID;
        ItemModel->crQty = item.quantity;
        ItemModel->drQty = item.quantity;
        ItemModel->drAmount = item.subTotal;
        ItemModel->itemReqId = item.ItemReqUuid;
        ItemModel->Narration = obj->narration;
        ItemModel->consumed = item.consumedQty;

        itemTransaction->insertItemTransaction(ItemModel, item.godownList);

        int basid = db->insertRecordsPlus(map1,Godown_Transfer_Details_TableName);

        for(inventoryItemDataModel subitem:compitem.subItems){
            QMap<QString,QVariant> map2;
            if(subitem.ItemReqUuid == "X"){
                subitem.ItemReqUuid = db->getUUID();
            }
            map2.insert(Godown_Transfer_Details_Voucher_No,obj->voucherNumber);
            map2.insert(Godown_Transfer_Details_Voucher_Prefix,obj->VoucherPrefix);
            map2.insert(Godown_Transfer_Details_Voucher_Date,obj->VoucherDate);
            map2.insert(Godown_Transfer_Details_Inventory_Item_ID,item.ItemID);
            map2.insert(Godown_Transfer_Details_Quantity,item.quantity);
            map2.insert(Godown_Transfer_Details_Price,item.price);
            map2.insert(Godown_Transfer_Details_PriceLevel,item.PriceLevel);
            map2.insert(Godown_Transfer_Details_isCompoundItem,item.IsCompoundItem);
            map2.insert(Godown_Transfer_Details_subItemOff_list_ID,0);
            map2.insert(Godown_Transfer_Details_Discount_Percent,item.discountPercentage);
            map2.insert(Godown_Transfer_Details_Discount_Amount,item.discountinAmount);
            map2.insert(Godown_Transfer_Details_Subtotal,item.subTotal);
            map2.insert(Godown_Transfer_Details_Project_ID,obj->ProjectId);
            map2.insert(Godown_Transfer_Details_Item_Narration,item.narration);
            map2.insert(Godown_Transfer_Details_Item_Description,item.ItemDescription);
            map2.insert(Godown_Transfer_Details_Salesman_ID,obj->SalesmanID);
            map2.insert(Godown_Transfer_Details_Requirement_ItemID,item.ItemReqUuid);
            map2.insert(Godown_Transfer_Details_Uom, item.uomObject._id);
            map2.insert(Godown_Transfer_Details_Time, QTime::currentTime());

            db->insertRecordsPlus(map2,Godown_Transfer_Details_TableName);
        }

    }

    int c=commit();

//    if(obj->ledgerObject.dbName != "" && !obj->fromExternal && obj->sendFlag){
//        GeneralVoucherDataObject v;
//        NotificationDataModel *notification = new NotificationDataModel();
//        QJsonDocument doc(v.ObjToJson(obj));
//        QString strJson(doc.toJson(QJsonDocument::Compact));

//        notification->NotificationData = strJson;
//        notification->ToDBName=obj->ledgerObject.dbName;
//        notification->TypeOfData="Voucher";
//        notification->Action=1001;
//        notification->GeneratedTransactionId=QString::number(QDateTime::currentDateTime().toTime_t()) + obj->VoucherPrefix;

//        sendChannelDatabaseHelper *sendHelper = new sendChannelDatabaseHelper();
//        sendHelper->insertNotification(notification);
//    }

    return c;

}

int GodownTransferDatabaseHelper::updateGodownTransfer(GeneralVoucherDataObject *obj, QString oldID)
{
    this->startTransaction();

    QMap<QString,QVariant> map;
    map.insert(Godown_Transfer_Main_Voucher_No,obj->voucherNumber);
    map.insert(Godown_Transfer_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Godown_Transfer_Main_Date_Created,obj->VoucherDate);
    map.insert(Godown_Transfer_Main_Voucher_Date,obj->VoucherDate);
    map.insert(Godown_Transfer_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    map.insert(Godown_Transfer_Main_Billing_Name,obj->BillingName);
    map.insert(Godown_Transfer_Main_Total,obj->grandTotal);
    map.insert(Godown_Transfer_Main_Narration,obj->narration);
    map.insert(Godown_Transfer_Main_Added_By,obj->AddedBy);
    map.insert(Godown_Transfer_Main_Delivery_Date,obj->DeliveryDate);
    map.insert(Godown_Transfer_Main_Completion_Probability,obj->CompletionProbability);
    map.insert(Godown_Transfer_Main_Credit_Period,obj->ledgerObject.Credit_Period);
    map.insert(Godown_Transfer_Main_POC_Name,obj->ledgerObject.ContactPersonName);
    map.insert(Godown_Transfer_Main_POC_Phone,obj->ledgerObject.ContactPersonNumber);
    map.insert(Godown_Transfer_Main_POC_Email,obj->ledgerObject.ContactPersonEmail);
    map.insert(Godown_Transfer_Main_Project_ID,obj->ProjectId);
    map.insert(Godown_Transfer_Main_Revision_Number,obj->RevisionNo);
    map.insert(Godown_Transfer_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Godown_Transfer_Main_TimeStamp,QDateTime::currentDateTime());
    map.insert(Godown_Transfer_Main_Last_Updated,QDateTime::currentDateTime());
    map.insert(Godown_Transfer_Main_FromLocation, obj->fromGodownID);
    map.insert(Godown_Transfer_Main_ToLocation, obj->toGodownID);
    map.insert(Godown_Transfer_Main_VoucherStatus, obj->status);

    QMap<QString,QVariant> map3;
    map3.insert(Godown_Transfer_Main_Voucher_No, oldID);
    map3.insert(Godown_Transfer_Main_Voucher_Prefix, obj->VoucherPrefix);
    db->updateRecordsPlus(map,map3,Godown_Transfer_Main_TableName);

    QMap<QString,QVariant> maptxndel;
    maptxndel.insert(Transaction_Helper_VoucherNo,oldID);
    maptxndel.insert(Transaction_Helper_VoucherType,VoucherType);

    QMap<QString,QVariant> map6;
    map6.insert(Godown_Transfer_Details_Voucher_No,oldID);
    map6.insert(Godown_Transfer_Details_Voucher_Prefix,obj->VoucherPrefix);
    db->deleteRecordsPlus(map6,Godown_Transfer_Details_TableName);
    itemTransaction->deleteItemTransaction(obj);


    for(CompoundItemDataObject compitem:obj->InventoryItems){
        QMap<QString,QVariant> map1;
        inventoryItemDataModel item = compitem.BaseItem;
        map1.insert(Godown_Transfer_Details_Voucher_No,obj->voucherNumber);
        if(item.ItemReqUuid == "X"){
            item.ItemReqUuid = db->getUUID();
        }
        map1.insert(Godown_Transfer_Details_Voucher_Prefix,obj->VoucherPrefix);
        map1.insert(Godown_Transfer_Details_Voucher_Date,obj->VoucherDate);
        map1.insert(Godown_Transfer_Details_Inventory_Item_ID,item.ItemID);
        map1.insert(Godown_Transfer_Details_Quantity,item.quantity);
        map1.insert(Godown_Transfer_Details_Price,item.price);
        map1.insert(Godown_Transfer_Details_PriceLevel,item.PriceLevel);
        map1.insert(Godown_Transfer_Details_isCompoundItem,item.IsCompoundItem);
        map1.insert(Godown_Transfer_Details_subItemOff_list_ID,0);
        map1.insert(Godown_Transfer_Details_Discount_Percent,item.discountPercentage);
        map1.insert(Godown_Transfer_Details_Discount_Amount,item.discountinAmount);
        map1.insert(Godown_Transfer_Details_Subtotal,item.subTotal);
        map1.insert(Godown_Transfer_Details_Project_ID,obj->ProjectId);
        map1.insert(Godown_Transfer_Details_Item_Narration,item.narration);
        map1.insert(Godown_Transfer_Details_Item_Description,item.ItemDescription);
        map1.insert(Godown_Transfer_Details_Salesman_ID,obj->SalesmanID);
        map1.insert(Godown_Transfer_Details_Requirement_ItemID,item.ItemReqUuid);
        map1.insert(Godown_Transfer_Details_Uom, item.uomObject._id);
        map1.insert(Godown_Transfer_Details_Time, QTime::currentTime());

        ItemModel->ItemID = item.ItemID;
        ItemModel->From_Ledger = item.defaultPurchaseLedgerID;
        ItemModel->To_Ledger = obj->ledgerObject.LedgerID;
        ItemModel->VoucherID = obj->voucherNumber;
        ItemModel->VoucherPrefix = obj->VoucherPrefix;
        ItemModel->VoucherDate = obj->VoucherDate;
        ItemModel->VoucherType = VoucherType;
        ItemModel->From_Location = obj->fromGodownID;
        ItemModel->To_Location = obj->toGodownID;
        ItemModel->crQty = item.quantity;
        ItemModel->drQty = item.quantity;
        ItemModel->drAmount = item.subTotal;
        ItemModel->itemReqId = item.ItemReqUuid;
        ItemModel->Narration = obj->narration;
        ItemModel->consumed = item.consumedQty;

        itemTransaction->insertItemTransaction(ItemModel, item.godownList);

        int basid = db->insertRecordsPlus(map1,Godown_Transfer_Details_TableName);

        for(inventoryItemDataModel subitem:compitem.subItems){
            QMap<QString,QVariant> map2;
            if(subitem.ItemReqUuid == "X"){
                subitem.ItemReqUuid = db->getUUID();
            }
            map2.insert(Godown_Transfer_Details_Voucher_No,obj->voucherNumber);
            map2.insert(Godown_Transfer_Details_Voucher_Prefix,obj->VoucherPrefix);
            map2.insert(Godown_Transfer_Details_Voucher_Date,obj->VoucherDate);
            map2.insert(Godown_Transfer_Details_Inventory_Item_ID,item.ItemID);
            map2.insert(Godown_Transfer_Details_Quantity,item.quantity);
            map2.insert(Godown_Transfer_Details_Price,item.price);
            map2.insert(Godown_Transfer_Details_PriceLevel,item.PriceLevel);
            map2.insert(Godown_Transfer_Details_isCompoundItem,item.IsCompoundItem);
            map2.insert(Godown_Transfer_Details_subItemOff_list_ID,0);
            map2.insert(Godown_Transfer_Details_Discount_Percent,item.discountPercentage);
            map2.insert(Godown_Transfer_Details_Discount_Amount,item.discountinAmount);
            map2.insert(Godown_Transfer_Details_Subtotal,item.subTotal);
            map2.insert(Godown_Transfer_Details_Project_ID,obj->ProjectId);
            map2.insert(Godown_Transfer_Details_Item_Narration,item.narration);
            map2.insert(Godown_Transfer_Details_Item_Description,item.ItemDescription);
            map2.insert(Godown_Transfer_Details_Salesman_ID,obj->SalesmanID);
            map2.insert(Godown_Transfer_Details_Requirement_ItemID,item.ItemReqUuid);
            map2.insert(Godown_Transfer_Details_Uom, item.uomObject._id);
            map2.insert(Godown_Transfer_Details_Time, QTime::currentTime());

            db->insertRecordsPlus(map2,Godown_Transfer_Details_TableName);
        }

    }

    int c=commit();

//    if(obj->ledgerObject.dbName != ""){
//        GeneralVoucherDataObject v;
//        NotificationDataModel *notification = new NotificationDataModel();
//        QJsonDocument doc(v.ObjToJson(obj));
//        QString strJson(doc.toJson(QJsonDocument::Compact));

//        notification->NotificationData = strJson;
//        notification->ToDBName=obj->ledgerObject.dbName;
//        notification->TypeOfData="Voucher";
//        notification->Action=1002;
//        notification->GeneratedTransactionId=QString::number(QDateTime::currentDateTime().toTime_t()) + obj->VoucherPrefix;

//        sendChannelDatabaseHelper *sendHelper = new sendChannelDatabaseHelper();
//         sendHelper->insertNotification(notification);
//    }

    return c;

}

int GodownTransferDatabaseHelper::deleteGodownTransfer(GeneralVoucherDataObject *obj)
{
    this->startTransaction();
    QMap<QString,QVariant> map3;
    map3.insert(Godown_Transfer_Main_Voucher_No,obj->voucherNumber);
    map3.insert(Godown_Transfer_Main_Voucher_Prefix,obj->VoucherPrefix);
    db->deleteRecordsPlus(map3,Godown_Transfer_Main_TableName);

    QMap<QString,QVariant> map4;
    map4.insert(Godown_Transfer_Details_Voucher_No,obj->voucherNumber);
    map4.insert(Godown_Transfer_Details_Voucher_Prefix,obj->VoucherPrefix);
    db->deleteRecordsPlus(map4,Godown_Transfer_Details_TableName);

    itemTransaction->deleteItemTransaction(obj);
    ledTransaction->deleteTransaction(obj);

    return commit();
}

int GodownTransferDatabaseHelper::commit()
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

}

int GodownTransferDatabaseHelper::getMaxID()
{
    QString qStr = "SELECT MAX("+Godown_Transfer_Main_id+") FROM " + Godown_Transfer_Main_TableName;
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

QSqlQueryModel *GodownTransferDatabaseHelper::getVoucherListByDateModel(QDate dateFrom, QDate dateTo)
{
    LedgerMasterDatabaseHelper ledHelper;
    QString qStr ="SELECT ";
    qStr += "'V' , ";
    qStr += Godown_Transfer_Main_Voucher_No +", ";
    qStr += "'" + VoucherType + "', " ;
    qStr += Godown_Transfer_Main_Voucher_Prefix + ", ";
    qStr += Godown_Transfer_Main_Voucher_Date+" as 'Voucher Date', ";
    qStr += " CONCAT(" + Godown_Transfer_Main_Voucher_Prefix + ", '-', " + Godown_Transfer_Main_Voucher_No+ ") as 'Voucher No', ";
    qStr += "(SELECT "+ledHelper.Ledger_Master_LedgerName + " FROM "+ ledHelper.Ledger_Master_TableName;
    qStr += " WHERE "+ledHelper.Ledger_Master_LedgerID + "= som.";
    qStr += Godown_Transfer_Main_Ledger_ID + ") Ledger, ";
    qStr += Godown_Transfer_Main_Total ;

    qStr += " FROM " + Godown_Transfer_Main_TableName;
    qStr += " som WHERE ";
    qStr += Godown_Transfer_Main_Voucher_Date;
    qStr += " >= '";
    qStr += dateFrom.toString("yyyy-MM-dd");
    qStr += "' AND ";
    qStr += Godown_Transfer_Main_Voucher_Date;
    qStr += " <= '";
    qStr += dateTo.toString("yyyy-MM-dd");
    qStr += "'";

    return DatabaseHelper::getQueryModel(qStr);

}

GeneralVoucherDataObject *GodownTransferDatabaseHelper::getVoucherByVoucherNoPtr(QString vNo, QString voucherPrefix)
{
    GeneralVoucherDataObject *obj = new GeneralVoucherDataObject();
    SalesInventoryItemDatabaseHelper *itemHelper = new SalesInventoryItemDatabaseHelper();
    LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper();



    QString qStrDetail = "SELECT distinct " + Godown_Transfer_Details_Voucher_No
            + ", " + Godown_Transfer_Details_Voucher_Prefix
            + ", " + Godown_Transfer_Details_Voucher_Date
            + ", " + Godown_Transfer_Details_Inventory_Item_ID
            + ", " + Godown_Transfer_Details_Quantity
            + ", " + Godown_Transfer_Details_Price
            + ", " + Godown_Transfer_Details_Uom
            + ", subq.convalue , subq.UomName, subq.UomSymbol "
            + ", " + Godown_Transfer_Details_PriceLevel
            + ", " + Godown_Transfer_Details_Discount_Percent
            + ", " + Godown_Transfer_Details_Discount_Amount
            + ", " + Godown_Transfer_Details_Subtotal
            + ", " + Godown_Transfer_Details_Item_Narration
            + ", " + Godown_Transfer_Details_Item_Description
            + ", " + Godown_Transfer_Details_Salesman_ID
            + ", " + Godown_Transfer_Details_Requirement_ItemID
            + ", " + Godown_Transfer_Details_itemVoucherStatus
            + ", " + Godown_Transfer_Details_itemProductionStatus
            + ", " + Godown_Transfer_Details_Time
            + ", " + Godown_Transfer_Details_isCompoundItem
            + ", " + Godown_Transfer_Details_subItemOff_list_ID
            + " FROM " + Godown_Transfer_Details_TableName+ " s LEFT OUTER JOIN "
            + UomDataBaseHelper::qStrUomForItemDetail + " as subq "
            + " ON s." + Godown_Transfer_Details_Inventory_Item_ID + " = subq.itemID "
            + " AND s." + Godown_Transfer_Details_Uom + " = subq.UOM "
            + " WHERE " + Godown_Transfer_Details_Voucher_No + "= :vno"
            + " AND " +  Godown_Transfer_Details_Voucher_Prefix + "= :voucherPrefix";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStrDetail);
    query.bindValue(":vno",vNo);
    query.bindValue(":voucherPrefix",voucherPrefix);
    if(!query.exec()){
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    else{
        while(query.next()){
            int i=3;
            inventoryItemDataModel item = itemHelper->getInventoryItemByID(query.value(i++).toString());
            item.quantity = query.value(i++).toFloat();
            item.price = query.value(i++).toFloat();
            item.uomObject._id = query.value(i++).toString();
            item.uomObject.convRate = query.value(i++).toFloat();
            item.uomObject.UomName = query.value(i++).toString();
            item.uomObject.UomSymbol = query.value(i++).toString();
            item.PriceLevel = query.value(i++).toString();
            item.discountPercentage = query.value(i++).toFloat();
            item.discountinAmount = query.value(i++).toFloat();
            item.subTotal = query.value(i++).toFloat();
            item.narration = query.value(i++).toString();
            item.ItemDescription = query.value(i++).toString();
            item.ItemReqUuid = query.value(i++).toString();
            item.length = query.value(i++).toFloat();
            item.Dimension = query.value(i++).toString();
            item.itemVoucherStatus = query.value(i++).toInt();
            item.itemProductionStatus = query.value(i++).toInt();

            ItemTransactionDatabaseHelper transHelper;
            item.consumedQty = transHelper.getConsumedQtyByVoucher(item.ItemID, vNo, VoucherType, voucherPrefix, item.ItemReqUuid);

            if(query.value(i++).toBool()||(query.value(i++).toInt()==0)){
                CompoundItemDataObject cObj;
                cObj.BaseItem = item;
                obj->InventoryItems.append(cObj);
            }
            else{

                int itemSize = obj->InventoryItems.size();
                obj->InventoryItems[itemSize-1].subItems.append(item);
                obj->InventoryItems[itemSize-1].subItemsTotal +=item.subTotal;
            }
        }
    }


    QString qStrMain ="SELECT "
            + Godown_Transfer_Main_Voucher_No + ", "
            + Godown_Transfer_Main_Voucher_Prefix + ", "
            + Godown_Transfer_Main_Date_Created + ", "
            + Godown_Transfer_Main_Voucher_Date + ", "
            + Godown_Transfer_Main_TimeStamp + ", "
            + Godown_Transfer_Main_Ledger_ID + ", "
            + Godown_Transfer_Main_Billing_Name + ", "
            + Godown_Transfer_Main_Total + ", "
            + Godown_Transfer_Main_Narration + ", "
            + Godown_Transfer_Main_Added_By + ", "
            + Godown_Transfer_Main_Delivery_Date + ", "
            + Godown_Transfer_Main_Completion_Probability + ", "
            + Godown_Transfer_Main_Credit_Period + ", "
            + Godown_Transfer_Main_POC_Name + ", "
            + Godown_Transfer_Main_POC_Phone + ", "
            + Godown_Transfer_Main_POC_Email + ", "
            + Godown_Transfer_Main_Project_ID + ", "
            + Godown_Transfer_Main_Revision_Number + ", "
            + Godown_Transfer_Main_Salesman_ID + ", "
            + Godown_Transfer_Main_FromLocation + ", "
            + Godown_Transfer_Main_ToLocation + ", "
            + Godown_Transfer_Main_VoucherStatus + " "
            + " FROM " + Godown_Transfer_Main_TableName
            + " WHERE " + Godown_Transfer_Main_Voucher_No + "= :vno"
            + " AND " +  Godown_Transfer_Main_Voucher_Prefix + "= :voucherPrefix";

    query.prepare(qStrMain);
    query.bindValue(":vno",vNo);
    query.bindValue(":voucherPrefix", voucherPrefix);
    if(!query.exec()){
        qDebug()<<query.lastQuery();
        qDebug()<<query.lastError();
    }
    else{
        while(query.next()){
            int i =0;

            obj->voucherNumber = query.value(i++).toString();
            obj->VoucherPrefix = query.value(i++).toString();
            obj->voucherType = VoucherType;
            obj->DateCreated = query.value(i++).toDate();
            obj->VoucherDate = query.value(i++).toDate();
            obj->timestamp = query.value(i++).toDateTime();
            obj->ledgerObject = ledHelper->getLedgerObjectByID( query.value(i++).toString());
            obj->BillingName = query.value(i++).toString();
            obj->grandTotal = query.value(i++).toFloat();
            obj->narration = query.value(i++).toString();
            obj->AddedById = query.value(i++).toInt();
            obj->DeliveryDate = query.value(i++).toDateTime();
            obj->CompletionProbability = query.value(i++).toFloat();
            obj->CreditPeriod = query.value(i++).toInt();
            obj->POCName  = query.value(i++).toString();
            obj->POCPhone = query.value(i++).toString();
            obj->POCEmail = query.value(i++).toString();
            obj->ProjectId = query.value(i++).toString();
            obj->RevisionNo = query.value(i++).toInt();
            obj->SalesmanID = query.value(i++).toInt();
            obj->fromGodownID = query.value(i++).toString();
            obj->toGodownID = query.value(i++).toString();
            obj->status = query.value(i++).toInt();

//            obj->LPO = query.value(i++).toString();
//            obj->ModeOfService = query.value(i++).toInt();
//            obj->discountinAmount = query.value(i++).toFloat();
//            obj->discountPercent = query.value(i++).toFloat();
//            obj->taxTotalAmount = query.value(i++).toFloat();
//            obj->Contact._id = query.value(i++).toInt();
//            obj->grossTotal = query.value(i++).toFloat();
//            obj->reference = query.value(i++).toString();
//            obj->CustomerExpectingDate = query.value(i++).toDate();
//            obj->currency = query.value(i++).toString();
//            obj->numBoxes = query.value(i++).toInt();
//            obj->totalWeight = query.value(i++).toFloat();
//            obj->LRNO = query.value(i++).toString();
//            obj->_DisplayVoucherNumber = query.value(i++).toString();
//            obj->ReqVoucherList = query\.value\(i\+\+\)\.toJsonArray\(\);
//            obj->Origin = query.value(i++).toInt();
        }
    }

    qDebug()<<" to go down "<<obj->toGodownID;


    return obj;

}
