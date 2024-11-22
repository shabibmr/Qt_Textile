#include "stockjournaldatabasehelper.h"

StockJournalDatabaseHelper::StockJournalDatabaseHelper(QString connString)
{
    db = new DatabaseHelper(connString);
    itemTransaction = new ItemTransactionDatabaseHelper(db);

}

StockJournalDatabaseHelper::~StockJournalDatabaseHelper()
{

}

int StockJournalDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

QString StockJournalDatabaseHelper::getNextVoucherNo(QString voucherPrefix)
{
    GeneralVoucherDatabaseHelper vHelper;
    QString voucherNumber = vHelper.getNextVoucherNoByType(VoucherType, voucherPrefix, Stock_Journal_Main_TableName);
    return voucherNumber;

}

QSqlQueryModel *StockJournalDatabaseHelper::getVoucherListByDateTimeModel(QDateTime dateFrom, QDateTime dateTo)
{

}

QPair<QString, QString> StockJournalDatabaseHelper::getVoucherNoByTransactionId(QString transId)
{

}

QString StockJournalDatabaseHelper::getVoucherNumberByLedID(QString LedID, QDate Date)
{

}

int StockJournalDatabaseHelper::insertVoucher(GeneralVoucherDataObject *obj)
{
    this->startTransaction();
    qDebug()<<"inserting data";

    //    obj->voucherNumber = QString::number(getMaxID()+1);
    QMap<QString,QVariant> map;
    GeneralVoucherDatabaseHelper vHelper;
    obj->voucherNumber = vHelper.setNextVoucherNoByType(VoucherType, obj->VoucherPrefix, Stock_Journal_Main_TableName );

    map.insert(Stock_Journal_Main_Voucher_No,obj->voucherNumber);
    map.insert(Stock_Journal_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Stock_Journal_Main_Date_Created,obj->VoucherDate);
    map.insert(Stock_Journal_Main_Voucher_Date,obj->VoucherDate);
    //    map.insert(Stock_Journal_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    //    map.insert(Stock_Journal_Main_Billing_Name,obj->ledgerObject.LedgerName);
    map.insert(Stock_Journal_Main_Total,obj->grandTotal);
    map.insert(Stock_Journal_Main_Narration,obj->narration);
    map.insert(Stock_Journal_Main_Added_By,obj->AddedBy);
    map.insert(Stock_Journal_Main_VoucherStatus,obj->status);

    //    map.insert(Stock_Journal_Main_Delivery_Date,obj->DeliveryDate);
    //    map.insert(Stock_Journal_Main_Completion_Probability,obj->CompletionProbability);
    //    map.insert(Stock_Journal_Main_Credit_Period,obj->ledgerObject.Credit_Period);
    //    map.insert(Stock_Journal_Main_POC_Name,obj->ledgerObject.ContactPersonName);
    //    map.insert(Stock_Journal_Main_POC_Phone,obj->ledgerObject.ContactPersonNumber);
    //    map.insert(Stock_Journal_Main_POC_Email,obj->ledgerObject.ContactPersonEmail);
    //    map.insert(Stock_Journal_Main_Project_ID,obj->ProjectId);
    //    map.insert(Stock_Journal_Main_Revision_Number,obj->RevisionNo);
    //    map.insert(Stock_Journal_Main_Converted_To_Sales_Order,obj->ConvertedToSalesOrder);
    //    map.insert(Stock_Journal_Main_Quotation_Prepared,obj->QuotationPrepared);
    map.insert(Stock_Journal_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Stock_Journal_Main_TimeStampC,obj->timestamp);
    map.insert(Stock_Journal_Main_Last_Updated,QDateTime::currentDateTime());

    qint64 lastId= db->insertRecordsPlus(map,Stock_Journal_Main_TableName);

    //    qDebug()<<"New ID = "<<lastId;
    //    QMap<QString,QVariant> idmap;
    //    idmap.insert(Stock_Journal_Main_Voucher_No,QString::number(lastId));
    //    QMap<QString,QVariant> idmapClause;
    //    idmapClause.insert(Stock_Journal_Main_id,lastId);
    //    db->updateRecordsPlus(idmap,idmapClause,Stock_Journal_Main_TableName);
    //    qDebug()<<"New ID updated = "<<lastId;

    //    obj->voucherNumber = QString::number(lastId);

    for(CompoundItemDataObject compitem:obj->InventoryItems){
        QMap<QString,QVariant> map1;
        inventoryItemDataModel item = compitem.BaseItem;
        if(item.ItemReqUuid == "X"){
            item.ItemReqUuid = db->getUUID();
        }
        map1.insert(Stock_Journal_Details_Voucher_No,obj->voucherNumber);
        map1.insert(Stock_Journal_Details_Voucher_Prefix,obj->VoucherPrefix);
        map1.insert(Stock_Journal_Details_Voucher_Date,obj->VoucherDate);
        map1.insert(Stock_Journal_Details_Inventory_Item_ID,item.ItemID);
        map1.insert(Stock_Journal_Details_Dr_Quantity,item.drQty);
        map1.insert(Stock_Journal_Details_Cr_Quantity,item.crQty);
        map1.insert(Stock_Journal_Details_Price,item.price);
        map1.insert(Stock_Journal_Details_Subtotal,item.subTotal);
        //        map1.insert(Stock_Journal_Details_Project_ID,obj->ProjectId);
        map1.insert(Stock_Journal_Details_Item_Narration,obj->narration);
        map1.insert(Stock_Journal_Details_Item_Description,item.ItemDescription);
        map1.insert(Stock_Journal_Details_Salesman_ID,obj->SalesmanID);
        if(item.ItemReqUuid == "X"){
            item.ItemReqUuid = db->getUUID();
        }
        map1.insert(Stock_Journal_Details_Requirement_ItemID,item.ItemReqUuid);

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

        int basid = db->insertRecordsPlus(map1,Stock_Journal_Details_TableName);

        for(inventoryItemDataModel subitem:compitem.subItems){
            QMap<QString,QVariant> map2;
            if(subitem.ItemReqUuid == "X"){
                subitem.ItemReqUuid = db->getUUID();
            }
            map2.insert(Stock_Journal_Details_Voucher_No,obj->voucherNumber);
            map2.insert(Stock_Journal_Details_Voucher_Prefix,obj->VoucherPrefix);
            map2.insert(Stock_Journal_Details_Voucher_Date,obj->VoucherDate);
            map2.insert(Stock_Journal_Details_Inventory_Item_ID,subitem.ItemID);
            //            map2.insert(Stock_Journal_Details_Quantity,subitem.quantity);
            map2.insert(Stock_Journal_Details_Price,subitem.price);
            //            map2.insert(Stock_Journal_Details_PriceLevel,subitem.PriceLevel);
            //            map2.insert(Stock_Journal_Details_isCompoundItem,subitem.IsCompoundItem);
            //            map2.insert(Stock_Journal_Details_subItemOff_list_ID,basid);
            //            map2.insert(Stock_Journal_Details_Discount_Percent,subitem.discountPercentage);
            //            map2.insert(Stock_Journal_Details_Discount_Amount,subitem.discount);
            map2.insert(Stock_Journal_Details_Subtotal,subitem.subTotal);
            //            map2.insert(Stock_Journal_Details_Project_ID,obj->ProjectId);
            map2.insert(Stock_Journal_Details_Item_Narration,subitem.narration);
            map2.insert(Stock_Journal_Details_Item_Description,subitem.ItemDescription);
            //            map2.insert(Stock_Journal_Details_Salesman_ID,obj->SalesmanID);

            db->insertRecordsPlus(map2,Stock_Journal_Details_TableName);
        }
    }
    qDebug()<<"Ledgers Size = "<<obj->ledgersList.size();


    return commit();
}

int StockJournalDatabaseHelper::updateVoucher(GeneralVoucherDataObject *obj, QString oldID)
{
    this->startTransaction();
    
    obj->voucherType = VoucherType;
    QMap<QString,QVariant> map;
    map.insert(Stock_Journal_Main_Voucher_No,obj->voucherNumber);
    map.insert(Stock_Journal_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Stock_Journal_Main_Date_Created,obj->VoucherDate);
    map.insert(Stock_Journal_Main_Voucher_Date,obj->VoucherDate);
    //    map.insert(Stock_Journal_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    //    map.insert(Stock_Journal_Main_Billing_Name,obj->ledgerObject.LedgerName);
    //    map.insert(Stock_Journal_Main_Total,obj->grandTotal);
    map.insert(Stock_Journal_Main_Narration,obj->narration);
    map.insert(Stock_Journal_Main_Added_By,obj->AddedBy);
    //    map.insert(Stock_Journal_Main_Delivery_Date,obj->DeliveryDate);
    //    map.insert(Stock_Journal_Main_Completion_Probability,obj->CompletionProbability);
    //    map.insert(Stock_Journal_Main_Credit_Period,obj->ledgerObject.Credit_Period);
    //    map.insert(Stock_Journal_Main_POC_Name,obj->ledgerObject.ContactPersonName);
    //    map.insert(Stock_Journal_Main_POC_Phone,obj->ledgerObject.ContactPersonNumber);
    //    map.insert(Stock_Journal_Main_POC_Email,obj->ledgerObject.ContactPersonEmail);
    //    map.insert(Stock_Journal_Main_Project_ID,obj->ProjectId);
    //    map.insert(Stock_Journal_Main_Revision_Number,obj->RevisionNo);
    //    map.insert(Stock_Journal_Main_Converted_To_Sales_Order,obj->ConvertedToSalesOrder);
    //    map.insert(Stock_Journal_Main_Quotation_Prepared,obj->QuotationPrepared);
    map.insert(Stock_Journal_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Stock_Journal_Main_Last_Updated,QDateTime::currentDateTime());
    map.insert(Stock_Journal_Main_VoucherStatus,obj->status);

    QMap<QString,QVariant> map3;
    map3.insert(Stock_Journal_Main_Voucher_No,oldID);
    map3.insert(Stock_Journal_Main_Voucher_Prefix,obj->VoucherPrefix);

    db->updateRecordsPlus(map,map3,Stock_Journal_Main_TableName);

    QMap<QString,QVariant> map5;
    map5.insert(Stock_Journal_Details_Voucher_No,oldID);
    db->deleteRecordsPlus(map5,Stock_Journal_Details_TableName);

    itemTransaction->deleteItemTransaction(obj);

    for(CompoundItemDataObject compitem:obj->InventoryItems){
        QMap<QString,QVariant> map1;
        inventoryItemDataModel item = compitem.BaseItem;
        if(item.ItemReqUuid == "X"){
            item.ItemReqUuid = db->getUUID();
        }
        map1.insert(Stock_Journal_Details_Voucher_No,obj->voucherNumber);
        map1.insert(Stock_Journal_Details_Voucher_Prefix,obj->VoucherPrefix);
        map1.insert(Stock_Journal_Details_Voucher_Date,obj->VoucherDate);
        map1.insert(Stock_Journal_Details_Inventory_Item_ID,item.ItemID);
        map1.insert(Stock_Journal_Details_Dr_Quantity,item.drQty);
        map1.insert(Stock_Journal_Details_Cr_Quantity,item.crQty);
        map1.insert(Stock_Journal_Details_Price,item.price);
        //        map1.insert(Stock_Journal_Details_PriceLevel,item.PriceLevel);
        //        map1.insert(Stock_Journal_Details_isCompoundItem,item.IsCompoundItem);
        //        map1.insert(Stock_Journal_Details_subItemOff_list_ID,0);
        //        map1.insert(Stock_Journal_Details_Discount_Percent,item.discountPercentage);
        //        map1.insert(Stock_Journal_Details_Discount_Amount,item.discount);
        map1.insert(Stock_Journal_Details_Subtotal,item.subTotal);
        //        map1.insert(Stock_Journal_Details_Project_ID,obj->ProjectId);
        map1.insert(Stock_Journal_Details_Item_Narration,obj->narration);
        map1.insert(Stock_Journal_Details_Item_Description,item.ItemDescription);
        map1.insert(Stock_Journal_Details_Salesman_ID,obj->SalesmanID);
        if(item.ItemReqUuid == "X"){
            item.ItemReqUuid = db->getUUID();
        }
        map1.insert(Stock_Journal_Details_Requirement_ItemID,item.ItemReqUuid);


        ItemModel = new ItemTransactionDataModel();
        ItemModel->ItemID = item.ItemID;
        ItemModel->VoucherID = obj->voucherNumber;
        ItemModel->VoucherPrefix = obj->VoucherPrefix;
        ItemModel->VoucherDate = obj->VoucherDate;
        ItemModel->VoucherType = VoucherType;
        ItemModel->From_Location = "OUT";
        //        ItemModel->To_Location = "Godown";

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
        ItemModel->drAmount = item.subTotal;
        ItemModel->itemReqId = item.ItemReqUuid;
        ItemModel->Narration = obj->narration;
        ItemModel->consumed = item.consumedQty;

        itemTransaction->insertItemTransaction(ItemModel, item.godownList);
        int basid = db->insertRecordsPlus(map1,Stock_Journal_Details_TableName);

        for(inventoryItemDataModel subitem:compitem.subItems){
            QMap<QString,QVariant> map2;
            if(subitem.ItemReqUuid == "X"){
                subitem.ItemReqUuid = db->getUUID();
            }
            map2.insert(Stock_Journal_Details_Voucher_No,obj->voucherNumber);
            map2.insert(Stock_Journal_Details_Voucher_Prefix,obj->VoucherPrefix);
            map2.insert(Stock_Journal_Details_Voucher_Date,obj->VoucherDate);
            map2.insert(Stock_Journal_Details_Inventory_Item_ID,subitem.ItemID);
            //            map2.insert(Stock_Journal_Details_Quantity,subitem.quantity);
            map2.insert(Stock_Journal_Details_Price,subitem.price);
            //            map2.insert(Stock_Journal_Details_PriceLevel,subitem.PriceLevel);
            //            map2.insert(Stock_Journal_Details_isCompoundItem,subitem.IsCompoundItem);
            //            map2.insert(Stock_Journal_Details_subItemOff_list_ID,basid);
            //            map2.insert(Stock_Journal_Details_Discount_Percent,subitem.discountPercentage);
            //            map2.insert(Stock_Journal_Details_Discount_Amount,subitem.discount);
            map2.insert(Stock_Journal_Details_Subtotal,subitem.subTotal);
            //            map2.insert(Stock_Journal_Details_Project_ID,obj->ProjectId);
            map2.insert(Stock_Journal_Details_Item_Narration,subitem.narration);
            map2.insert(Stock_Journal_Details_Item_Description,subitem.ItemDescription);
            //            map2.insert(Stock_Journal_Details_Salesman_ID,obj->SalesmanID);

            db->insertRecordsPlus(map2,Stock_Journal_Details_TableName);
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

int StockJournalDatabaseHelper::deleteVoucher(GeneralVoucherDataObject *obj)
{
    this->startTransaction();

    QMap<QString,QVariant> map3;
    map3.insert(Stock_Journal_Main_Voucher_No,obj->voucherNumber);
    db->deleteRecordsPlus(map3,Stock_Journal_Main_TableName);

    QMap<QString,QVariant> map4;

    map4.insert(Stock_Journal_Details_Voucher_No,obj->voucherNumber);
    map4.insert(Stock_Journal_Details_Voucher_Prefix,obj->VoucherPrefix);
    db->deleteRecordsPlus(map4,Stock_Journal_Details_TableName);

    //    QMap<QString,QVariant> ledDelMap;
    //    ledDelMap.insert(Proxy_Transaction_Helper_VoucherNo,oldID);
    //    db->deleteRecordsPlus(ledDelMap,Proxy_Transaction_Helper_TableName);

    itemTransaction->deleteItemTransaction(obj);

    return commit();
}

int StockJournalDatabaseHelper::commit()
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

int StockJournalDatabaseHelper::getMaxID()
{
    QString qStr = "SELECT MAX("+Stock_Journal_Main_id+") FROM " + Stock_Journal_Main_TableName;
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


GeneralVoucherDataObject* StockJournalDatabaseHelper::getVoucherByVoucherNoPtr(QString vNo, QString voucherPrefix)
{
    GeneralVoucherDataObject *obj = new GeneralVoucherDataObject;
    SalesInventoryItemDatabaseHelper *itemHelper = new SalesInventoryItemDatabaseHelper();

    QString qStrMain;

    qStrMain += "Select ";
    qStrMain += Stock_Journal_Main_Voucher_No +", ";
    qStrMain += Stock_Journal_Main_Voucher_Prefix +", ";
    qStrMain += Stock_Journal_Main_Voucher_Date +", ";
    qStrMain += Stock_Journal_Main_VoucherStatus +", ";
    qStrMain += Stock_Journal_Main_Narration +", ";
    qStrMain += Stock_Journal_Main_Added_By +", ";
    qStrMain += Stock_Journal_Details_Salesman_ID;
    qStrMain += " From ";
    qStrMain += Stock_Journal_Main_TableName;
    qStrMain += " WHERE ";
    qStrMain += Stock_Journal_Main_Voucher_No + "= :vno";
    qStrMain += " AND ";
    qStrMain += Stock_Journal_Main_Voucher_Prefix + "= :vpref";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));


    QString qStrDetail = "SELECT "
            + Stock_Journal_Details_id +", "
            + Stock_Journal_Details_Voucher_No +", "
            + Stock_Journal_Details_Voucher_Prefix +", "
            + Stock_Journal_Details_Voucher_Date +", "
            + Stock_Journal_Details_Inventory_Item_ID +", "
            + Stock_Journal_Details_Dr_Quantity +", "
            + Stock_Journal_Details_Cr_Quantity +", "
            + Stock_Journal_Details_Item_Narration +", "
            + Stock_Journal_Details_Requirement_ItemID
            + " FROM " + Stock_Journal_Details_TableName
            + " WHERE " + Stock_Journal_Details_Voucher_No + "= :vno"
            + " AND " + Stock_Journal_Details_Voucher_Prefix + " = '" + voucherPrefix + "'";

    query.prepare(qStrMain);
    query.bindValue(":vno",vNo);
    query.bindValue(":vpref",voucherPrefix);


    if(!query.exec()){
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    else{
        while(query.next()){
            int i=0;
            obj->voucherType = VoucherType;
            obj->voucherNumber = query.value(i++).toString();
            obj->VoucherPrefix = query.value(i++).toString();
            obj->VoucherDate = query.value(i++).toDate();
            obj->status = query.value(i++).toInt();
            obj->narration = query.value(i++).toString();
            obj->AddedById = query.value(i++).toInt();
            obj->SalesmanID = query.value(i++).toInt();

        }
    }


    qDebug()<<qStrDetail;
    query.prepare(qStrDetail);
    query.bindValue(":vno",vNo);
    if(!query.exec()){
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    else{
        while(query.next()){

            inventoryItemDataModel item = itemHelper->getInventoryItemByID(query.value(4).toString());

            item.drQty= query.value(5).toFloat();
            item.crQty= query.value(6).toFloat();
            item.quantity = qFabs(item.drQty - item.crQty);
            item.narration = query.value(7).toString();
            item.ItemReqUuid = query.value(8).toString();

            qDebug()<<Q_FUNC_INFO<<__LINE__<<item.ItemReqUuid;


            ItemTransactionDatabaseHelper transHelper;
            item.consumedQty = transHelper.getConsumedQtyByVoucher(item.ItemID, vNo, VoucherType,
                                                                   voucherPrefix, item.ItemReqUuid);
            item.godownList = itemTransaction->getGodownAllocationByVoucherAndItem(vNo, VoucherType,
                                                                                   voucherPrefix, item.ItemReqUuid);

            qDebug()<<Q_FUNC_INFO<<__LINE__<<item.godownList.size();
            //            item.fifoList = transHelper.getFifoListByVoucherItem(obj->voucherNumber, obj->voucherType,
            //                                                                 obj->VoucherPrefix, item.ItemID);

            CompoundItemDataObject comp; comp.BaseItem = item;
            obj->InventoryItems.append(comp);
            //                obj->InventoryItems[itemSize-1].subItemsTotal +=item.subTotal;
        }
    }
    return obj;
}



QSqlQueryModel *StockJournalDatabaseHelper::getVoucherListByDateModel(QDate dateFrom, QDate dateTo)
{
    LedgerMasterDatabaseHelper ledHelper;
    QString qStr ="SELECT ";
    qStr += "'V' , ";
    qStr += Stock_Journal_Main_Voucher_No +", ";
    qStr += "'" + VoucherType + "', " ;
    qStr += Stock_Journal_Main_Voucher_Prefix + ", ";
    qStr += Stock_Journal_Main_Voucher_Date+" as 'Voucher Date', ";
    qStr += " CONCAT(" + Stock_Journal_Main_Voucher_Prefix + ", '-', " + Stock_Journal_Main_Voucher_No+ ") as 'Voucher No', ";
    qStr += "(SELECT "+ledHelper.Ledger_Master_LedgerName + " FROM "+ ledHelper.Ledger_Master_TableName;
    qStr += " WHERE "+ledHelper.Ledger_Master_LedgerID + "= som.";
    qStr += Stock_Journal_Main_Ledger_ID + ") Ledger, ";
    qStr += Stock_Journal_Main_Total ;
    qStr += " FROM " + Stock_Journal_Main_TableName;
    qStr += " som WHERE ";
    qStr += Stock_Journal_Main_Voucher_Date;
    qStr += " >= ";
    qStr += db->getDateString(dateFrom);
    qStr += " AND ";
    qStr += Stock_Journal_Main_Voucher_Date;
    qStr += " <= ";
    qStr += db->getDateString(dateTo);
    qStr += "";
    return DatabaseHelper::getQueryModel(qStr);
}
