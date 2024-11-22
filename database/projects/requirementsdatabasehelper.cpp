#include "requirementsdatabasehelper.h"

RequirementsDatabaseHelper::RequirementsDatabaseHelper()
{
    db = new DatabaseHelper();
}

RequirementsDatabaseHelper::~RequirementsDatabaseHelper()
{

}

int RequirementsDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

QString RequirementsDatabaseHelper::getNextVoucherNo(QString voucherPrefix)
{
    GeneralVoucherDatabaseHelper vHelper;
    QString voucherNumber = vHelper.getNextVoucherNoByType(ledgerType, voucherPrefix, Requirement_Main_TableName);
    return voucherNumber;

}


int RequirementsDatabaseHelper::insertRequirement(GeneralVoucherDataObject *obj)
{
    this->startTransaction();
    qDebug()<<"inserting data";

    QMap<QString,QVariant> map;

    GeneralVoucherDatabaseHelper vHelper;
    obj->voucherNumber = vHelper.setNextVoucherNoByType(ledgerType, obj->VoucherPrefix , Requirement_Main_TableName);

    map.insert(Requirement_Main_Voucher_No,obj->voucherNumber);
    map.insert(Requirement_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Requirement_Main_Date_Created,obj->VoucherDate);
    map.insert(Requirement_Main_Voucher_Date,obj->VoucherDate);
    map.insert(Requirement_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    map.insert(Requirement_Main_Billing_Name,obj->ledgerObject.LedgerName);
    map.insert(Requirement_Main_Total,obj->grandTotal);
    map.insert(Requirement_Main_Narration,obj->narration);
    map.insert(Requirement_Main_Added_By,obj->AddedById);
    map.insert(Requirement_Main_Delivery_Date,obj->DeliveryDate);
    map.insert(Requirement_Main_Completion_Probability,obj->CompletionProbability);
    map.insert(Requirement_Main_Credit_Period,obj->ledgerObject.Credit_Period);
    map.insert(Requirement_Main_POC_Name,obj->ledgerObject.ContactPersonName);
    map.insert(Requirement_Main_POC_Phone,obj->ledgerObject.ContactPersonNumber);
    map.insert(Requirement_Main_POC_Email,obj->ledgerObject.ContactPersonEmail);
    map.insert(Requirement_Main_Project_ID,obj->ProjectId);
    map.insert(Requirement_Main_Revision_Number,obj->RevisionNo);
    map.insert(Requirement_Main_Converted_To_Sales_Order,obj->ConvertedToSalesOrder);
    map.insert(Requirement_Main_Quotation_Prepared,obj->QuotationPrepared);
    map.insert(Requirement_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Requirement_Main_ContactID,obj->Contact._id);

    db->insertRecordsPlus(map,Requirement_Main_TableName);

    for(CompoundItemDataObject compitem:obj->InventoryItems){
        QMap<QString,QVariant> map1;
        inventoryItemDataModel item = compitem.BaseItem;
        map1.insert(Requirement_Details_Voucher_No,obj->voucherNumber);
        map1.insert(Requirement_Details_Voucher_Prefix,obj->VoucherPrefix);
        map1.insert(Requirement_Details_Voucher_Date,obj->VoucherDate);
        map1.insert(Requirement_Details_Inventory_Item_ID,item.ItemID);
        map1.insert(Requirement_Details_Quantity,item.quantity);
        map1.insert(Requirement_Details_Price,item.price);
        map1.insert(Requirement_Details_PriceLevel,item.PriceLevel);
        map1.insert(Requirement_Details_isCompoundItem,item.IsCompoundItem);
        map1.insert(Requirement_Details_subItemOff_list_ID,0);
        map1.insert(Requirement_Details_Discount_Percent,item.discountPercentage);
        map1.insert(Requirement_Details_Discount_Amount,item.discount);
        map1.insert(Requirement_Details_Subtotal,item.subTotal);
        map1.insert(Requirement_Details_Project_ID,obj->ProjectId);
        map1.insert(Requirement_Details_Item_Narration,item.narration);
        map1.insert(Requirement_Details_Item_Description,item.ItemDescription);
        map1.insert(Requirement_Details_Salesman_ID,obj->SalesmanID);
        map1.insert(Requirement_Details_Length,item.length);
        int basid = db->insertRecordsPlus(map1,Requirement_Details_TableName);

        for(inventoryItemDataModel subitem:compitem.subItems){
            QMap<QString,QVariant> map2;
            map2.insert(Requirement_Details_Voucher_No,obj->voucherNumber);
            map2.insert(Requirement_Details_Voucher_Prefix,obj->VoucherPrefix);
            map2.insert(Requirement_Details_Voucher_Date,obj->VoucherDate);
            map2.insert(Requirement_Details_Inventory_Item_ID,subitem.ItemID);
            map2.insert(Requirement_Details_Quantity,subitem.quantity);
            map2.insert(Requirement_Details_Price,subitem.price);
            map2.insert(Requirement_Details_PriceLevel,subitem.PriceLevel);
            map2.insert(Requirement_Details_isCompoundItem,subitem.IsCompoundItem);
            map2.insert(Requirement_Details_subItemOff_list_ID,basid);
            map2.insert(Requirement_Details_Discount_Percent,subitem.discountPercentage);
            map2.insert(Requirement_Details_Discount_Amount,subitem.discount);
            map2.insert(Requirement_Details_Subtotal,subitem.subTotal);
            map2.insert(Requirement_Details_Project_ID,obj->ProjectId);
            map2.insert(Requirement_Details_Item_Narration,subitem.narration);
            map2.insert(Requirement_Details_Item_Description,subitem.ItemDescription);
            map2.insert(Requirement_Details_Salesman_ID,obj->SalesmanID);
            map2.insert(Requirement_Details_Length,item.length);
            db->insertRecordsPlus(map2,Requirement_Details_TableName);
        }
        QMap <QString,QVariant> ledMap;
        ledMap.insert(Proxy_Transaction_Helper_VoucherType,ledgerType);
        ledMap.insert(Proxy_Transaction_Helper_VoucherNo,obj->voucherNumber);
        ledMap.insert(Proxy_Transaction_Helper_VoucherPrefix,obj->VoucherPrefix);
        ledMap.insert(Proxy_Transaction_Helper_VoucherDate,obj->VoucherDate);
        ledMap.insert(Proxy_Transaction_Helper_ProjectID,obj->ProjectId);
        ledMap.insert(Proxy_Transaction_Helper_SalesManID,obj->SalesmanID);
        ledMap.insert(Proxy_Transaction_Helper_TranactionAmount,compitem.subItemsTotal+compitem.BaseItem.subTotal);
        ledMap.insert(Proxy_Transaction_Helper_DebitLedgerID,compitem.BaseItem.defaultSalesLedgerID);
        ledMap.insert(Proxy_Transaction_Helper_CreditLedgerID,obj->ledgerObject.LedgerID);
        ledMap.insert(Proxy_Transaction_Helper_Narration,obj->ledgerObject.narration);
        db->insertRecordsPlus(ledMap,Proxy_Transaction_Helper_TableName);

    }
    qDebug()<<"Ledgers Size = "<<obj->ledgersList.size();
    for(LedgerMasterDataModel ledger:obj->ledgersList){
        QMap <QString,QVariant> ledMap;
        ledMap.insert(Proxy_Transaction_Helper_VoucherType,ledgerType);
        ledMap.insert(Proxy_Transaction_Helper_VoucherNo,obj->voucherNumber);
        ledMap.insert(Proxy_Transaction_Helper_VoucherPrefix,obj->VoucherPrefix);
        ledMap.insert(Proxy_Transaction_Helper_VoucherDate,obj->VoucherDate);
        ledMap.insert(Proxy_Transaction_Helper_ProjectID,obj->ProjectId);
        ledMap.insert(Proxy_Transaction_Helper_SalesManID,obj->SalesmanID);
        ledMap.insert(Proxy_Transaction_Helper_TranactionAmount,ledger.amount);
        ledMap.insert(Proxy_Transaction_Helper_DebitLedgerID,ledger.LedgerID);
        ledMap.insert(Proxy_Transaction_Helper_CreditLedgerID,obj->ledgerObject.LedgerID);
        ledMap.insert(Proxy_Transaction_Helper_Narration,obj->ledgerObject.narration);
        ledMap.insert(Proxy_Transaction_Helper_isAdditional,true);
        db->insertRecordsPlus(ledMap,Proxy_Transaction_Helper_TableName);
    }

    return commit();
}

int RequirementsDatabaseHelper::updateRequirement(GeneralVoucherDataObject *obj, QString oldID)
{
    this->startTransaction();
    QMap<QString,QVariant> map5;
    map5.insert(Requirement_Details_Voucher_No,oldID);
    db->deleteRecordsPlus(map5,Requirement_Details_TableName);

    QMap<QString,QVariant> ledDelMap;
    ledDelMap.insert(Proxy_Transaction_Helper_VoucherNo,obj->voucherNumber);
    db->deleteRecordsPlus(ledDelMap,Proxy_Transaction_Helper_TableName);

    QMap<QString,QVariant> map;
    map.insert(Requirement_Main_Voucher_No,obj->voucherNumber);
    map.insert(Requirement_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Requirement_Main_Date_Created,obj->VoucherDate);
    map.insert(Requirement_Main_Voucher_Date,obj->VoucherDate);
    map.insert(Requirement_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    map.insert(Requirement_Main_Billing_Name,obj->ledgerObject.LedgerName);
    map.insert(Requirement_Main_Total,obj->grandTotal);
    map.insert(Requirement_Main_Narration,obj->narration);
    map.insert(Requirement_Main_Added_By,obj->AddedById);
    map.insert(Requirement_Main_Delivery_Date,obj->DeliveryDate);
    map.insert(Requirement_Main_Completion_Probability,obj->CompletionProbability);
    map.insert(Requirement_Main_Credit_Period,obj->ledgerObject.Credit_Period);
    map.insert(Requirement_Main_POC_Name,obj->ledgerObject.ContactPersonName);
    map.insert(Requirement_Main_POC_Phone,obj->ledgerObject.ContactPersonNumber);
    map.insert(Requirement_Main_POC_Email,obj->ledgerObject.ContactPersonEmail);
    map.insert(Requirement_Main_Project_ID,obj->ProjectId);
    map.insert(Requirement_Main_Revision_Number,obj->RevisionNo);
    map.insert(Requirement_Main_Converted_To_Sales_Order,obj->ConvertedToSalesOrder);
    map.insert(Requirement_Main_Quotation_Prepared,obj->QuotationPrepared);
    map.insert(Requirement_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Requirement_Main_ContactID,obj->Contact._id);

    QMap<QString,QVariant> map3;
    map3.insert(Requirement_Main_Voucher_No,oldID);
    db->updateRecordsPlus(map,map3,Requirement_Main_TableName);


    for(CompoundItemDataObject compitem:obj->InventoryItems){
        QMap<QString,QVariant> map1;
        inventoryItemDataModel item = compitem.BaseItem;
        map1.insert(Requirement_Details_Voucher_No,obj->voucherNumber);
        map1.insert(Requirement_Details_Voucher_Prefix,obj->VoucherPrefix);
        map1.insert(Requirement_Details_Voucher_Date,obj->VoucherDate);
        map1.insert(Requirement_Details_Inventory_Item_ID,item.ItemID);
        map1.insert(Requirement_Details_Quantity,item.quantity);
        map1.insert(Requirement_Details_Price,item.price);
        map1.insert(Requirement_Details_PriceLevel,item.PriceLevel);
        map1.insert(Requirement_Details_isCompoundItem,item.IsCompoundItem);
        map1.insert(Requirement_Details_subItemOff_list_ID,0);
        map1.insert(Requirement_Details_Discount_Percent,item.discountPercentage);
        map1.insert(Requirement_Details_Discount_Amount,item.discount);
        map1.insert(Requirement_Details_Subtotal,item.subTotal);
        map1.insert(Requirement_Details_Project_ID,obj->ProjectId);
        map1.insert(Requirement_Details_Item_Narration,item.narration);
        map1.insert(Requirement_Details_Item_Description,item.ItemDescription);
        map1.insert(Requirement_Details_Salesman_ID,obj->SalesmanID);
        map1.insert(Requirement_Details_Length,item.length);

        int basid = db->insertRecordsPlus(map1,Requirement_Details_TableName);

        for(inventoryItemDataModel subitem:compitem.subItems){
            QMap<QString,QVariant> map2;
            map2.insert(Requirement_Details_Voucher_No,obj->voucherNumber);
            map2.insert(Requirement_Details_Voucher_Prefix,obj->VoucherPrefix);
            map2.insert(Requirement_Details_Voucher_Date,obj->VoucherDate);
            map2.insert(Requirement_Details_Inventory_Item_ID,subitem.ItemID);
            map2.insert(Requirement_Details_Quantity,subitem.quantity);
            map2.insert(Requirement_Details_Price,subitem.price);
            map2.insert(Requirement_Details_PriceLevel,subitem.PriceLevel);
            map2.insert(Requirement_Details_isCompoundItem,subitem.IsCompoundItem);
            map2.insert(Requirement_Details_subItemOff_list_ID,basid);
            map2.insert(Requirement_Details_Discount_Percent,subitem.discountPercentage);
            map2.insert(Requirement_Details_Discount_Amount,subitem.discount);
            map2.insert(Requirement_Details_Subtotal,subitem.subTotal);
            map2.insert(Requirement_Details_Project_ID,obj->ProjectId);
            map2.insert(Requirement_Details_Item_Narration,subitem.narration);
            map2.insert(Requirement_Details_Item_Description,subitem.ItemDescription);
            map2.insert(Requirement_Details_Salesman_ID,obj->SalesmanID);
            map2.insert(Requirement_Details_Length,item.length);
            db->insertRecordsPlus(map2,Requirement_Details_TableName);
        }

        QMap <QString,QVariant> ledMap;
        ledMap.insert(Proxy_Transaction_Helper_VoucherType,ledgerType);
        ledMap.insert(Proxy_Transaction_Helper_VoucherNo,obj->voucherNumber);
        ledMap.insert(Proxy_Transaction_Helper_VoucherPrefix,obj->VoucherPrefix);
        ledMap.insert(Proxy_Transaction_Helper_VoucherDate,obj->VoucherDate);
        ledMap.insert(Proxy_Transaction_Helper_ProjectID,obj->ProjectId);
        ledMap.insert(Proxy_Transaction_Helper_SalesManID,obj->SalesmanID);
        ledMap.insert(Proxy_Transaction_Helper_TranactionAmount,compitem.subItemsTotal+compitem.BaseItem.subTotal);
        ledMap.insert(Proxy_Transaction_Helper_DebitLedgerID,compitem.BaseItem.defaultSalesLedgerID);
        ledMap.insert(Proxy_Transaction_Helper_CreditLedgerID,obj->ledgerObject.LedgerID);
        ledMap.insert(Proxy_Transaction_Helper_Narration,obj->ledgerObject.narration);
        db->insertRecordsPlus(ledMap,Proxy_Transaction_Helper_TableName);
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
        ledMap.insert(Proxy_Transaction_Helper_DebitLedgerID,ledger.LedgerID);
        ledMap.insert(Proxy_Transaction_Helper_CreditLedgerID,obj->ledgerObject.LedgerID);
        ledMap.insert(Proxy_Transaction_Helper_Narration,obj->narration);
        ledMap.insert(Proxy_Transaction_Helper_isAdditional,true);
        db->insertRecordsPlus(ledMap,Proxy_Transaction_Helper_TableName);
    }
    
    return commit();
}

int RequirementsDatabaseHelper::deleteRequirement(QString oldID)
{
    this->startTransaction();

    QMap<QString,QVariant> map3;
    map3.insert(Requirement_Main_Voucher_No,oldID);
    db->deleteRecordsPlus(map3,Requirement_Main_TableName);

    QMap<QString,QVariant> map4;
    map4.insert(Requirement_Details_Voucher_No,oldID);
    db->deleteRecordsPlus(map4,Requirement_Details_TableName);

    QMap<QString,QVariant> ledDelMap;
    ledDelMap.insert(Proxy_Transaction_Helper_VoucherNo,oldID);
    db->deleteRecordsPlus(ledDelMap,Proxy_Transaction_Helper_TableName);

    return commit();
}

int RequirementsDatabaseHelper::commit()
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

int RequirementsDatabaseHelper::getMaxID()
{
    QString qStr = "SELECT MAX("+Requirement_Main_id+") FROM " + Requirement_Main_TableName;
//    qDebug()<<qStr;
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

GeneralVoucherDataObject RequirementsDatabaseHelper::getVoucherByVoucherNo(QString vNo)
{
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
            + Proxy_Transaction_Helper_SalesManID +", "
            + Proxy_Transaction_Helper_isAdditional
            + " FROM " + Proxy_Transaction_Helper_TableName
            + " WHERE " + Proxy_Transaction_Helper_VoucherNo + "=:id"
            + " AND " + Proxy_Transaction_Helper_VoucherType + "='"+ledgerType+"'";

    QString qStrDetail = "SELECT "
            + Requirement_Details_id +", "
            + Requirement_Details_Voucher_No +", "
            + Requirement_Details_Voucher_Prefix +", "
            + Requirement_Details_Voucher_Date +", "
            + Requirement_Details_Inventory_Item_ID +", "
            + Requirement_Details_Price +", "
            + Requirement_Details_Quantity +", "
            + Requirement_Details_Discount_Percent +", "
            + Requirement_Details_Subtotal +", "
            + Requirement_Details_isCompoundItem +", "
            + Requirement_Details_subItemOff_list_ID +", "
            + Requirement_Details_PriceLevel +", "
            + Requirement_Details_Item_Description +", "
            + Requirement_Details_Item_Narration +", "
            + Requirement_Details_Length +" "
            + " FROM " + Requirement_Details_TableName
            + " WHERE " + Requirement_Details_Voucher_No + "= :vno";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStrLedgers);
    query.bindValue(":id",vNo);
    if(!query.exec()){
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    else{
        while(query.next()){
            LedgerMasterDataModel ledger;
            ledger = ledHelper->getLedgerObjectByID(query.value(6).toString());
            ledger.amount = query.value(4).toFloat();
            qDebug()<<"ledger status "<<query.value(9).toBool();
            if(query.value(9).toBool())
                obj.ledgersList.append(ledger);
        }
    }
    query.prepare(qStrDetail);
    query.bindValue(":vno",vNo);
    if(!query.exec()){
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    else{
        while(query.next()){

            inventoryItemDataModel item = itemHelper->getInventoryItemByID(query.value(4).toString());
            item.requirementItemIdOld = query.value(0).toLongLong();
            item.price = query.value(5).toFloat();
            item.quantity = query.value(6).toFloat();
            item.discountPercentage = query.value(7).toFloat();
            item.subTotal = query.value(8).toFloat();
            item.PriceLevel = query.value(11).toString();
            item.narration = query.value(13).toString();
            item.length = query.value(14).toFloat();
            item.ItemDescription = query.value(12).toString();
            item.listId = query.value(0).toInt();
            if(query.value(9).toBool()||query.value(10).toInt()==0  ){
                CompoundItemDataObject cObj;
                cObj.BaseItem = item;
                obj.InventoryItems.append(cObj);
            }
            else{
                int itemSize = obj.InventoryItems.size();
                obj.InventoryItems[itemSize-1].subItems.append(item);
                obj.InventoryItems[itemSize-1].subItemsTotal +=item.subTotal;
            }
        }
    }
    return obj;
}

QList<GeneralVoucherDataObject> RequirementsDatabaseHelper::getVoucherListByDate(QDate dateFrom, QDate dateTo)
{
    QList<GeneralVoucherDataObject> vouchersList;
    QString qStr ="SELECT "
            + Requirement_Main_Voucher_No+ ", "
            + Requirement_Main_Voucher_Prefix+ ", "
            + Requirement_Main_Voucher_Date+", "
            + Requirement_Main_Ledger_ID+ ", "
            + Requirement_Main_Total + ", "
            + Requirement_Main_TimeStamp + ", "
            + Requirement_Main_ContactID + ", "
            + Requirement_Main_Added_By + ", "
            + Requirement_Main_Salesman_ID + ", "
            + Requirement_Main_Narration

            + " FROM " + Requirement_Main_TableName
            + " WHERE "
            + Requirement_Main_Voucher_Date
            + " >= '"
            + dateFrom.toString("yyyy-MM-dd")
            + "' AND "
            + Requirement_Main_Voucher_Date
            + " <= '"
            + dateTo.toString("yyyy-MM-dd")
            + "'"
            ;
    LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper();
    AddressBookDatabaseHelper *addHelper = new AddressBookDatabaseHelper();

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
            item.timestamp = query.value(i++).toDateTime();
            item.Contact= addHelper->getContact(query.value(i++).toInt());
            item.AddedById  = query.value(i++).toInt();
            item.SalesmanID = query.value(i++).toInt();
            item.narration = query.value(i++).toString();
            vouchersList.append(item);
        }
    }
    qDebug()<<"Query Size = "<<query.size();

    return vouchersList;
}


void RequirementsDatabaseHelper::setItemVoucherStatus(float qty,QString id)
{
    QString qStr ="SELECT "+itemVoucherStatus+ " from "+ Requirement_Details_TableName +
            " WHERE "+Requirement_Details_Requirement_ItemID + " = " + id;
    float currQty=0;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    if(query.exec()){
        while(query.next()){
            currQty = query.value(0).toFloat();
        }
    }
    else{

    }
    qty +=currQty;

    QMap<QString,QVariant> map;
    QMap<QString,QVariant> clause;

    map.insert(itemVoucherStatus,qty);
    clause.insert(Requirement_Details_Requirement_ItemID,id);
    db->updateRecordsPlus(map,clause,Requirement_Details_TableName);
}

void RequirementsDatabaseHelper::resetItemVoucherStatus(float qty,QString id)
{
    QString qStr ="SELECT "+itemVoucherStatus+ " from "+ Requirement_Details_TableName +
            " WHERE "+Requirement_Details_Requirement_ItemID + " = " + id;
    float currQty=0;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    if(query.exec()){
        while(query.next()){
            currQty = query.value(0).toFloat();
        }
    }
    else{

    }
    qty -=currQty;

    QMap<QString,QVariant> map;
    QMap<QString,QVariant> clause;

    map.insert(itemVoucherStatus,qty);
    clause.insert(Requirement_Details_Requirement_ItemID,id);
    db->updateRecordsPlus(map,clause,Requirement_Details_TableName);
}

