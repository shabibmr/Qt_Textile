#include "defectivedatabasehelper.h"
#include "database/finance/TransactionHelper/generalvoucherdatabasehelper.h"

DefectiveDatabaseHelper::DefectiveDatabaseHelper(QString connString)
{
    db = new DatabaseHelper( connString);
    itemTransaction = new ItemTransactionDatabaseHelper(db);
}

DefectiveDatabaseHelper::~DefectiveDatabaseHelper()
{

}

int DefectiveDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

QString DefectiveDatabaseHelper::getNextVoucherNo(QString voucherPrefix)
{
    GeneralVoucherDatabaseHelper vHelper;
    return vHelper.getNextVoucherNoByType(VoucherType, voucherPrefix, Wastage_Entry_Main_TableName);
}

QSqlQueryModel *DefectiveDatabaseHelper::getVoucherListByDateTimeModel(QDateTime dateFrom, QDateTime dateTo)
{

}

QPair<QString, QString> DefectiveDatabaseHelper::getVoucherNoByTransactionId(QString transId)
{

}

QString DefectiveDatabaseHelper::getVoucherNumberByLedID(QString LedID, QDate Date)
{

}


int DefectiveDatabaseHelper::insertVoucher(GeneralVoucherDataObject *obj)
{
    this->startTransaction();
    qDebug()<<"inserting data";

    //    obj->voucherNumber = QString::number(getMaxID()+1);
    GeneralVoucherDatabaseHelper *vHelper = new GeneralVoucherDatabaseHelper;
    obj->voucherNumber = vHelper->setNextVoucherNoByType(VoucherType, obj->VoucherPrefix, Wastage_Entry_Main_TableName);
    qDebug()<<Q_FUNC_INFO<<__LINE__<<obj->voucherNumber;

    QMap<QString,QVariant> map;
    map.insert(Wastage_Entry_Main_Voucher_No,obj->voucherNumber);
    map.insert(Wastage_Entry_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Wastage_Entry_Main_Date_Created,obj->VoucherDate);
    map.insert(Wastage_Entry_Main_Voucher_Date,obj->VoucherDate);
    map.insert(Wastage_Entry_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    map.insert(Wastage_Entry_Main_Billing_Name,obj->ledgerObject.LedgerName);
    map.insert(Wastage_Entry_Main_Total,obj->grandTotal);
    map.insert(Wastage_Entry_Main_Narration,obj->narration);
    map.insert(Wastage_Entry_Main_Added_By,obj->AddedBy);
    map.insert(Wastage_Entry_Main_TimeStampC,obj->timestamp);
    map.insert(Wastage_Entry_Main_Last_Updated,QDateTime::currentDateTime());
    map.insert(Wastage_Entry_Main_Quotation_Dropped_Reason, obj->QuotationDroppedReason);
    map.insert(Wastage_Entry_Main_VoucherStatus, obj->status);
    map.insert(Wastage_Entry_Main_Salesman_ID, obj->SalesmanID);



    qint64 lastId= db->insertRecordsPlus(map,Wastage_Entry_Main_TableName);

//    qDebug()<<"New ID = "<<lastId;
//    QMap<QString,QVariant> idmap;
//    idmap.insert(Wastage_Entry_Main_Voucher_No,QString::number(lastId));
//    QMap<QString,QVariant> idmapClause;
//    idmapClause.insert(Wastage_Entry_Main_id,lastId);
//    db->updateRecordsPlus(idmap,idmapClause,Wastage_Entry_Main_TableName);
//    qDebug()<<"New ID updated = "<<lastId;

//    obj->voucherNumber = QString::number(lastId);

    for(CompoundItemDataObject compitem:obj->InventoryItems){
        QMap<QString,QVariant> map1;
        inventoryItemDataModel item = compitem.BaseItem;

        if(item.ItemReqUuid == "X")
            item.ItemReqUuid = db->getUUID();

        map1.insert(Wastage_Entry_Details_Voucher_No,obj->voucherNumber);
        map1.insert(Wastage_Entry_Details_Voucher_Prefix,obj->VoucherPrefix);
        map1.insert(Wastage_Entry_Details_Voucher_Date,obj->VoucherDate);
        map1.insert(Wastage_Entry_Details_Inventory_Item_ID,item.ItemID);
        map1.insert(Wastage_Entry_Details_Dr_Quantity,item.drQty);
        map1.insert(Wastage_Entry_Details_Cr_Quantity,item.crQty);
        map1.insert(Wastage_Entry_Details_UOM_ID,item.uomObject._id);
        map1.insert(Wastage_Entry_Details_Price,item.price);
        map1.insert(Wastage_Entry_Details_Subtotal,item.subTotal);
        map1.insert(Wastage_Entry_Details_Item_Narration,obj->narration);
        map1.insert(Wastage_Entry_Details_Item_Description,item.ItemDescription);
        map1.insert(Wastage_Entry_Details_Requirement_ItemID,item.ItemReqUuid);
        map1.insert(Wastage_Entry_Details_Salesman_ID, obj->SalesmanID);

        ItemModel = new ItemTransactionDataModel();
        ItemModel->ItemID = item.ItemID;
        ItemModel->VoucherID = obj->voucherNumber;
        ItemModel->VoucherPrefix = obj->VoucherPrefix;
        ItemModel->VoucherDate = obj->VoucherDate;
        ItemModel->VoucherType = VoucherType;
        ItemModel->From_Location = item.section;
                ItemModel->To_Location = "OUT";
//        ItemModel->To_Location = item.section;
        ItemModel->crQty = item.crQty;
        ItemModel->drQty = item.drQty;
        ItemModel->itemReqId = item.ItemReqUuid;
        ItemModel->Narration = obj->narration;
        ItemModel->Price = item.price;

        itemTransaction->insertItemTransaction(ItemModel, item.godownList);
        int basid = db->insertRecordsPlus(map1,Wastage_Entry_Details_TableName);

        for(inventoryItemDataModel subitem:compitem.subItems){
            QMap<QString,QVariant> map2;
            map2.insert(Wastage_Entry_Details_Voucher_No,obj->voucherNumber);
            map2.insert(Wastage_Entry_Details_Voucher_Prefix,obj->VoucherPrefix);
            map2.insert(Wastage_Entry_Details_Voucher_Date,obj->VoucherDate);
            map2.insert(Wastage_Entry_Details_Inventory_Item_ID,subitem.ItemID);
            //            map2.insert(Wastage_Entry_Details_Quantity,subitem.quantity);
            map2.insert(Wastage_Entry_Details_Price,subitem.price);
            //            map2.insert(Wastage_Entry_Details_PriceLevel,subitem.PriceLevel);
            //            map2.insert(Wastage_Entry_Details_isCompoundItem,subitem.IsCompoundItem);
            //            map2.insert(Wastage_Entry_Details_subItemOff_list_ID,basid);
            //            map2.insert(Wastage_Entry_Details_Discount_Percent,subitem.discountPercentage);
            //            map2.insert(Wastage_Entry_Details_Discount_Amount,subitem.discount);
            map2.insert(Wastage_Entry_Details_Subtotal,subitem.subTotal);
            //            map2.insert(Wastage_Entry_Details_Project_ID,obj->ProjectId);
            map2.insert(Wastage_Entry_Details_Item_Narration,subitem.narration);
            map2.insert(Wastage_Entry_Details_Item_Description,subitem.ItemDescription);
            //            map2.insert(Wastage_Entry_Details_Salesman_ID,obj->SalesmanID);

            db->insertRecordsPlus(map2,Wastage_Entry_Details_TableName);
        }
    }
    qDebug()<<"Ledgers Size = "<<obj->ledgersList.size();
    //    for(LedgerMasterDataModel ledger:obj->ledgersList){
    //        QMap <QString,QVariant> ledMap;
    //        ledMap.insert(Proxy_Transaction_Helper_VoucherType,ledgerType);
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

int DefectiveDatabaseHelper::updateVoucher(GeneralVoucherDataObject *obj, QString oldID)
{
    this->startTransaction();

    obj->voucherType = VoucherType;
    QMap<QString,QVariant> map;
    map.insert(Wastage_Entry_Main_Voucher_No,obj->voucherNumber);
    map.insert(Wastage_Entry_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Wastage_Entry_Main_Date_Created,obj->VoucherDate);
    map.insert(Wastage_Entry_Main_Voucher_Date,obj->VoucherDate);
    map.insert(Wastage_Entry_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    map.insert(Wastage_Entry_Main_Billing_Name,obj->ledgerObject.LedgerName);
    map.insert(Wastage_Entry_Main_Total,obj->grandTotal);
    map.insert(Wastage_Entry_Main_Narration,obj->narration);
    map.insert(Wastage_Entry_Main_Added_By,obj->AddedBy);
    //    map.insert(Wastage_Entry_Main_Delivery_Date,obj->DeliveryDate);
    //    map.insert(Wastage_Entry_Main_Completion_Probability,obj->CompletionProbability);
    //    map.insert(Wastage_Entry_Main_Credit_Period,obj->ledgerObject.Credit_Period);
    //    map.insert(Wastage_Entry_Main_POC_Name,obj->ledgerObject.ContactPersonName);
    //    map.insert(Wastage_Entry_Main_POC_Phone,obj->ledgerObject.ContactPersonNumber);
    //    map.insert(Wastage_Entry_Main_POC_Email,obj->ledgerObject.ContactPersonEmail);
    //    map.insert(Wastage_Entry_Main_Project_ID,obj->ProjectId);
    //    map.insert(Wastage_Entry_Main_Revision_Number,obj->RevisionNo);
    //    map.insert(Wastage_Entry_Main_Converted_To_Sales_Order,obj->ConvertedToSalesOrder);
    //    map.insert(Wastage_Entry_Main_Quotation_Prepared,obj->QuotationPrepared);
    map.insert(Wastage_Entry_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Wastage_Entry_Main_Last_Updated,QDateTime::currentDateTime());
    map.insert(Wastage_Entry_Main_Quotation_Dropped_Reason, obj->QuotationDroppedReason);
    map.insert(Wastage_Entry_Main_VoucherStatus, obj->status);
    QMap<QString,QVariant> map3;
    map3.insert(Wastage_Entry_Main_Voucher_No,oldID);
    map3.insert(Wastage_Entry_Main_Voucher_Prefix,obj->VoucherPrefix);
    db->updateRecordsPlus(map,map3,Wastage_Entry_Main_TableName);

    QMap<QString,QVariant> map5;
    map5.insert(Wastage_Entry_Details_Voucher_No,oldID);
    map5.insert(Wastage_Entry_Details_Voucher_Prefix,obj->VoucherPrefix);
    db->deleteRecordsPlus(map5,Wastage_Entry_Details_TableName);

    itemTransaction->deleteItemTransaction(obj);

    for(CompoundItemDataObject compitem:obj->InventoryItems){
        QMap<QString,QVariant> map1;
        inventoryItemDataModel item = compitem.BaseItem;

        if(item.ItemReqUuid == "X")
            item.ItemReqUuid = db->getUUID();

        map1.insert(Wastage_Entry_Details_Voucher_No,obj->voucherNumber);
        map1.insert(Wastage_Entry_Details_Voucher_Prefix,obj->VoucherPrefix);
        map1.insert(Wastage_Entry_Details_Voucher_Date,obj->VoucherDate);
        map1.insert(Wastage_Entry_Details_Inventory_Item_ID,item.ItemID);
        map1.insert(Wastage_Entry_Details_Dr_Quantity,item.drQty);
        map1.insert(Wastage_Entry_Details_Cr_Quantity,item.crQty);
        map1.insert(Wastage_Entry_Details_UOM_ID,item.uomObject._id);

        map1.insert(Wastage_Entry_Details_Price,item.price);
        //        map1.insert(Wastage_Entry_Details_PriceLevel,item.PriceLevel);
        //        map1.insert(Wastage_Entry_Details_isCompoundItem,item.IsCompoundItem);
        //        map1.insert(Wastage_Entry_Details_subItemOff_list_ID,0);
        //        map1.insert(Wastage_Entry_Details_Discount_Percent,item.discountPercentage);
        //        map1.insert(Wastage_Entry_Details_Discount_Amount,item.discount);
        map1.insert(Wastage_Entry_Details_Subtotal,item.subTotal);
        //        map1.insert(Wastage_Entry_Details_Project_ID,obj->ProjectId);
        map1.insert(Wastage_Entry_Details_Item_Narration,obj->narration);
        map1.insert(Wastage_Entry_Details_Item_Description,item.ItemDescription);
        //        map1.insert(Wastage_Entry_Details_Salesman_ID,obj->SalesmanID);
        map1.insert(Wastage_Entry_Details_Requirement_ItemID,item.ItemReqUuid);
        map1.insert(Wastage_Entry_Details_Salesman_ID, obj->SalesmanID);

        ItemModel = new ItemTransactionDataModel();
        ItemModel->ItemID = item.ItemID;
        ItemModel->VoucherID = obj->voucherNumber;
        ItemModel->VoucherPrefix = obj->VoucherPrefix;
        ItemModel->VoucherDate = obj->VoucherDate;
        ItemModel->VoucherType = VoucherType;
        ItemModel->From_Location = item.section;
        //        ItemModel->To_Location = "Godown";
        ItemModel->To_Location = "OUT";
        ItemModel->crQty = item.crQty;
        ItemModel->drQty = item.drQty;
        ItemModel->drAmount = item.subTotal;
        ItemModel->itemReqId = item.ItemReqUuid;
        ItemModel->Narration = obj->narration;
        ItemModel->Price = item.price;

        itemTransaction->insertItemTransaction(ItemModel, item.godownList);
        int basid = db->insertRecordsPlus(map1,Wastage_Entry_Details_TableName);

        for(inventoryItemDataModel subitem:compitem.subItems){
            QMap<QString,QVariant> map2;
            map2.insert(Wastage_Entry_Details_Voucher_No,obj->voucherNumber);
            map2.insert(Wastage_Entry_Details_Voucher_Prefix,obj->VoucherPrefix);
            map2.insert(Wastage_Entry_Details_Voucher_Date,obj->VoucherDate);
            map2.insert(Wastage_Entry_Details_Inventory_Item_ID,subitem.ItemID);
            //            map2.insert(Wastage_Entry_Details_Quantity,subitem.quantity);
            map2.insert(Wastage_Entry_Details_Price,subitem.price);
            //            map2.insert(Wastage_Entry_Details_PriceLevel,subitem.PriceLevel);
            //            map2.insert(Wastage_Entry_Details_isCompoundItem,subitem.IsCompoundItem);
            //            map2.insert(Wastage_Entry_Details_subItemOff_list_ID,basid);
            //            map2.insert(Wastage_Entry_Details_Discount_Percent,subitem.discountPercentage);
            //            map2.insert(Wastage_Entry_Details_Discount_Amount,subitem.discount);
            map2.insert(Wastage_Entry_Details_Subtotal,subitem.subTotal);

            //            map2.insert(Wastage_Entry_Details_Project_ID,obj->ProjectId);
            map2.insert(Wastage_Entry_Details_Item_Narration,subitem.narration);
            map2.insert(Wastage_Entry_Details_Item_Description,subitem.ItemDescription);
            //            map2.insert(Wastage_Entry_Details_Salesman_ID,obj->SalesmanID);

            db->insertRecordsPlus(map2,Wastage_Entry_Details_TableName);
        }

    }
    //    QMap<QString,QVariant> ledDelMap;
    //    ledDelMap.insert(Proxy_Transaction_Helper_VoucherNo,obj->voucherNumber);
    //    db->deleteRecordsPlus(ledDelMap,Proxy_Transaction_Helper_TableName);

    //    for(LedgerMasterDataModel ledger:obj->ledgersList){
    //        QMap <QString,QVariant> ledMap;
    //        ledMap.insert(Proxy_Transaction_Helper_VoucherType,ledgerType);
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

int DefectiveDatabaseHelper::deleteVoucher(GeneralVoucherDataObject *obj)
{
    this->startTransaction();

    QMap<QString,QVariant> map3;
    map3.insert(Wastage_Entry_Main_id,obj->voucherNumber);
    map3.insert(Wastage_Entry_Main_Voucher_Prefix,obj->VoucherPrefix);
    db->deleteRecordsPlus(map3,Wastage_Entry_Main_TableName);

    QMap<QString,QVariant> map4;
    map4.insert(Wastage_Entry_Details_Voucher_No,obj->voucherNumber);
    map4.insert(Wastage_Entry_Details_Voucher_Prefix,obj->VoucherPrefix);
    db->deleteRecordsPlus(map4,Wastage_Entry_Details_TableName);

    //    QMap<QString,QVariant> ledDelMap;
    //    ledDelMap.insert(Proxy_Transaction_Helper_VoucherNo,oldID);
    //    db->deleteRecordsPlus(ledDelMap,Proxy_Transaction_Helper_TableName);

    itemTransaction->deleteItemTransaction(obj);

    return commit();
}

int DefectiveDatabaseHelper::commit()
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

int DefectiveDatabaseHelper::getMaxID()
{
    QString qStr = "SELECT MAX("+Wastage_Entry_Main_id+") FROM " + Wastage_Entry_Main_TableName;
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

GeneralVoucherDataObject DefectiveDatabaseHelper::getVoucherByVoucherNo(QString vNo)
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
            + Wastage_Entry_Details_id +", "
            + Wastage_Entry_Details_Voucher_No +", "
            + Wastage_Entry_Details_Voucher_Prefix +", "
            + Wastage_Entry_Details_Voucher_Date +", "
            + Wastage_Entry_Details_Inventory_Item_ID +", "
            + Wastage_Entry_Details_Quantity +", "

            //            + Wastage_Entry_Details_Discount_Percent +", "
            //            + Wastage_Entry_Details_Subtotal +", "
            //            + Wastage_Entry_Details_isCompoundItem +", "
            //            + Wastage_Entry_Details_subItemOff_list_ID +", "
            //           + Wastage_Entry_Details_PriceLevel +", "
            //            + Wastage_Entry_Details_Item_Description +", "
            + Wastage_Entry_Details_Item_Narration +" "
            + " FROM " + Wastage_Entry_Details_TableName
            + " WHERE " + Wastage_Entry_Details_Voucher_No + "= :vno";

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
            item.drQty = query.value(5).toFloat();
            item.crQty = query.value(6).toFloat();
            item.narration = query.value(7).toString();
            CompoundItemDataObject comp;comp.BaseItem = item;
            obj.InventoryItems.append(comp);
        }
    }
    return obj;
}

GeneralVoucherDataObject* DefectiveDatabaseHelper::getVoucherByVoucherNoPtr(QString vNo, QString voucherPrefix)
{
    GeneralVoucherDataObject *obj = new GeneralVoucherDataObject;
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

    QString qStrDetail = "SELECT distinct " + Wastage_Entry_Details_Voucher_No
            + ", " + Wastage_Entry_Details_Voucher_Prefix
            + ", " + Wastage_Entry_Details_Voucher_Date
            + ", " + Wastage_Entry_Details_Inventory_Item_ID
            + ", " + Wastage_Entry_Details_Dr_Quantity
            + ", " + Wastage_Entry_Details_Cr_Quantity
            + ", " + Wastage_Entry_Details_Price
            + ", " + Wastage_Entry_Details_UOM_ID
            + ", subq.convalue , subq.UomName, subq.UomSymbol "
            + ", " + Wastage_Entry_Details_PriceLevel
            + ", " + Wastage_Entry_Details_Discount_Percent
            + ", " + Wastage_Entry_Details_Discount_Amount
            + ", " + Wastage_Entry_Details_Subtotal
            + ", " + Wastage_Entry_Details_Item_Narration
            + ", " + Wastage_Entry_Details_Item_Description
            //            + ", " + Wastage_Entry_Details_Salesman_ID
            + ", " + Wastage_Entry_Details_Requirement_ItemID
            //            + ", " + Wastage_Entry_Details_Length
            //            + ", " + Wastage_Entry_Details_Dimension
            //            + ", " + Wastage_Entry_Details_itemVoucherStatus
            //            + ", " + Wastage_Entry_Details_itemProductionStatus
            //            + ", " + Wastage_Entry_Details_Time
            + ", " + Wastage_Entry_Details_isCompoundItem
            + ", " + Wastage_Entry_Details_subItemOff_list_ID
            + " FROM " + Wastage_Entry_Details_TableName+ " s LEFT OUTER JOIN "
            + UomDataBaseHelper::qStrUomForItemDetail + " as subq "
            + " ON s." + Wastage_Entry_Details_Inventory_Item_ID + " = subq.itemID "
            + " AND s." + Wastage_Entry_Details_UOM_ID + " = subq.UOM "
            + " WHERE " + Wastage_Entry_Details_Voucher_No + "= :vno"
            + " AND " +  Wastage_Entry_Details_Voucher_Prefix + "= :voucherPrefix";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStrDetail);
    query.bindValue(":vno",vNo);
    query.bindValue(":voucherPrefix", voucherPrefix);
    if(!query.exec()){
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    else{
        while(query.next()){
            int i=3;
            inventoryItemDataModel item = itemHelper->getInventoryItemByID(query.value(i++).toString());
            //            item.quantity = query.value(i++).toFloat();
            item.drQty = query.value(i++).toFloat();
            item.crQty = query.value(i++).toFloat();
            item.quantity = item.crQty;
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
            //            item.length = query.value(i++).toFloat();
            //            item.Dimension = query.value(i++).toString();
            //            item.itemVoucherStatus = query.value(i++).toInt();
            //            item.itemProductionStatus = query.value(i++).toInt();
            //            item.fifoList = itemTransaction->getFifoListByVoucherItem(obj->voucherNumber, obj->voucherType,
            //                                                                               obj->VoucherPrefix, item.ItemID);
            item.godownList = itemTransaction->getGodownAllocationByVoucherAndItem
                    (vNo, VoucherType, voucherPrefix,item.ItemReqUuid);
            if(item.godownList.length()==1){
                item.toGodownID = item.godownList[0].toGodown;
                item.fromGodownID = item.godownList[0].fromGodown;
            }
            if(obj->InventoryItems.size() == 0){
                obj->fromGodownID = item.fromGodownID;
                obj->toGodownID = item.toGodownID;
            }
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
            //            + Sales_Order_Main_id + ", "
            + Wastage_Entry_Main_Voucher_No + ", "
            + Wastage_Entry_Main_Voucher_Prefix + ", "
            + Wastage_Entry_Main_Date_Created + ", "
            + Wastage_Entry_Main_Voucher_Date + ", "
            + Wastage_Entry_Main_TimeStamp + ", "
            + Wastage_Entry_Main_Ledger_ID + ", "
            + Wastage_Entry_Main_Billing_Name + ", "
            + Wastage_Entry_Main_Total + ", "
            + Wastage_Entry_Main_Narration + ", "
            + Wastage_Entry_Main_Added_By + ", "
            + Wastage_Entry_Main_Delivery_Date + ", "
            + Wastage_Entry_Main_Completion_Probability + ", "
            + Wastage_Entry_Main_Credit_Period + ", "
            + Wastage_Entry_Main_POC_Name + ", "
            + Wastage_Entry_Main_POC_Phone + ", "
            + Wastage_Entry_Main_POC_Email + ", "
            + Wastage_Entry_Main_Project_ID + ", "
            + Wastage_Entry_Main_Revision_Number + ", "
            + Wastage_Entry_Main_Converted_To_Sales_Order + ", "
            + Wastage_Entry_Main_Quotation_Dropped + ", "
            + Wastage_Entry_Main_Quotation_Dropped_Reason + ", "
            + Wastage_Entry_Main_Requirement_Voucher_No + ", "
            + Wastage_Entry_Main_Salesman_ID + ", "
            //            + Wastage_Entry_Main_LPORef + ", "
            + Wastage_Entry_Main_VoucherStatus
            //            + Wastage_Entry_Main_ModeOfService + ", "
            //            + Wastage_Entry_Main_Discount_In_Amount + ", "
            //            + Wastage_Entry_Main_Discount_In_Percent + ", "
            //            + Wastage_Entry_Main_VATAmount + ", "
            //            + Wastage_Entry_Main_Contact_ID + ", "
            //            + Wastage_Entry_Main_Gross_Total + ", "
            //            + Wastage_Entry_Main_Reference + ", "
            //            + Wastage_Entry_Main_Customer_Expecting_Date + ", "
            //            + Wastage_Entry_Main_Currency + ", "
            //            + Wastage_Entry_Main_BoxesCount+ ", "
            //            + Wastage_Entry_Main_TotalWeight + ", "
            //            + Wastage_Entry_Main_LRNo + ", "
            //            + Wastage_Entry_Main_VoucherNoDisplay + ", "
            //            + Wastage_Entry_Main_ReqVoucherList + ", "
            //            + Wastage_Entry_Main_Origin
            + " FROM " + Wastage_Entry_Main_TableName
            + " WHERE " + Wastage_Entry_Main_Voucher_No + "= :vno"
            + " AND " +  Wastage_Entry_Main_Voucher_Prefix + "= :voucherPrefix";
    qDebug()<<qStrMain;
    query.prepare(qStrMain);
    query.bindValue(":vno",vNo);
    query.bindValue(":voucherPrefix", voucherPrefix);
    if(!query.exec()){
        qDebug()<<"vno = "<<vNo;
        qDebug()<<"pref "<<voucherPrefix;

        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    else{
        while(query.next()){
            int i=0;

            obj->voucherNumber = query.value(i++).toString(); //0
            obj->voucherType = VoucherType;
            obj->VoucherPrefix = query.value(i++).toString();
            obj->DateCreated = query.value(i++).toDate();
            obj->VoucherDate = query.value(i++).toDate();
            obj->timestamp = query.value(i++).toDateTime();
            obj->ledgerObject = ledHelper->getLedgerObjectByID( query.value(i++).toString());
            obj->BillingName = query.value(i++).toString();
            obj->grandTotal = query.value(i++).toFloat();
            obj->narration = query.value(i++).toString();
            obj->AddedById = query.value(i++).toInt();
            obj->DeliveryDate = query.value(i++).toDateTime(); //10
            obj->CompletionProbability = query.value(i++).toFloat();
            obj->CreditPeriod = query.value(i++).toInt();
            obj->POCName  = query.value(i++).toString();
            obj->POCPhone = query.value(i++).toString();
            obj->POCEmail = query.value(i++).toString();
            obj->ProjectId = query.value(i++).toString();
            obj->RevisionNo = query.value(i++).toInt();
            obj->ConvertedToSalesOrder = query.value(i++).toString();
            obj->QuotationDropped  = query.value(i++).toBool();
            obj->QuotationDroppedReason = query.value(i++).toString();
            obj->RequirementVoucherNo = query.value(i++).toString();
            obj->SalesmanID = query.value(i++).toInt();
            //            obj->LPO = query.value(i++).toString();
            obj->status = query.value(i++).toInt();//23
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
    //    qDebug()<<qStrDetail;
    return obj;
}

QList<GeneralVoucherDataObject> DefectiveDatabaseHelper::getVoucherListByDate(QDate dateFrom, QDate dateTo)
{
    QList<GeneralVoucherDataObject> vouchersList;
    QString qStr ="SELECT "
            + Wastage_Entry_Main_Voucher_No+ ", "
            + Wastage_Entry_Main_Voucher_Prefix+ ", "
            + Wastage_Entry_Main_Voucher_Date+", "
            + Wastage_Entry_Main_Ledger_ID+ ", "
            + Wastage_Entry_Main_Total
            + " FROM " + Wastage_Entry_Main_TableName
            + " WHERE "
            + Wastage_Entry_Main_Voucher_Date
            + " >= '"
            + dateFrom.toString("yyyy-MM-dd")
            + "' AND "
            + Wastage_Entry_Main_Voucher_Date
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

QSqlQueryModel *DefectiveDatabaseHelper::getVoucherListByDateModel(QDate dateFrom, QDate dateTo)
{
    LedgerMasterDatabaseHelper ledHelper;
    QString qStr ="SELECT ";
    qStr += "'V' , ";
    qStr += Wastage_Entry_Main_Voucher_No +", ";
    qStr += "'" + VoucherType + "', " ;
    qStr += Wastage_Entry_Main_Voucher_Prefix + ", ";
    qStr += Wastage_Entry_Main_Voucher_Date+" as 'Voucher Date', ";
    qStr += " CONCAT(" + Wastage_Entry_Main_Voucher_Prefix + ", '-', " + Wastage_Entry_Main_Voucher_No+ ") as 'Voucher No', ";
    qStr += "(SELECT "+ledHelper.Ledger_Master_LedgerName + " FROM "+ ledHelper.Ledger_Master_TableName;
    qStr += " WHERE "+ledHelper.Ledger_Master_LedgerID + "= som.";
    qStr += Wastage_Entry_Main_Ledger_ID + ") Ledger, ";
    qStr += Wastage_Entry_Main_Total ;
    //    qStr += Sales_Order_Main_TimeStamp +" 'Date'  ";
    //    qStr += Sales_Order_Main_Added_By + ", ";
    //    qStr += Sales_Order_Main_Salesman_ID + ", ";
    //    qStr += Sales_Order_Main_Narration + ", ";
    //    qStr += Sales_Order_Main_LPO + ", ";
    //    qStr += Sales_Order_Main_ModeOfService + ", ";
    //    qStr += Sales_Order_Main_VoucherStatus + ", ";
    //    qStr += Sales_Order_Main_Reference + ", ";
    //    qStr += Sales_Order_Main_POC_Phone + ", ";
    //    qStr += Sales_Order_Main_Requirement_Voucher_No + " ";
    qStr += " FROM " + Wastage_Entry_Main_TableName;
    qStr += " som WHERE ";
    qStr += Wastage_Entry_Main_Voucher_Date;
    qStr += " >= '";
    qStr += dateFrom.toString("yyyy-MM-dd");
    qStr += "' AND ";
    qStr += Wastage_Entry_Main_Voucher_Date;
    qStr += " <= '";
    qStr += dateTo.toString("yyyy-MM-dd");
    qStr += "'";


    return DatabaseHelper::getQueryModel(qStr);

}
