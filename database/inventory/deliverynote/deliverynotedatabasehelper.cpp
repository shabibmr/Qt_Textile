#include "deliverynotedatabasehelper.h"

DeliveryNoteDatabaseHelper::DeliveryNoteDatabaseHelper(QString connString)
{
    db = new DatabaseHelper(connString);

    itemTransaction = new ItemTransactionDatabaseHelper(db);
    ItemModel = new ItemTransactionDataModel();
}

DeliveryNoteDatabaseHelper::~DeliveryNoteDatabaseHelper()
{
}

int DeliveryNoteDatabaseHelper::startTransaction()
{
    db->startTransaction();

}

QString DeliveryNoteDatabaseHelper::getNextVoucherNo(QString voucherPrefix)
{
    GeneralVoucherDatabaseHelper vHelper;
    QString voucherNumber = vHelper.getNextVoucherNoByType(VoucherType, voucherPrefix, Delivery_Note_Main_TableName);
    return voucherNumber;
}

QPair<QString, QString> DeliveryNoteDatabaseHelper::getVoucherNoByTransactionId(QString transId)
{
    GeneralVoucherDatabaseHelper vHelper;
    return vHelper.getVoucherNoByTransactionId(transId, Delivery_Note_Main_TableName);

}

QString DeliveryNoteDatabaseHelper::getVoucherNumberByLedID(QString LedID, QDate Date)
{

}
int DeliveryNoteDatabaseHelper::insertVoucher(GeneralVoucherDataObject *obj)
{
    this->startTransaction();
    QMap<QString,QVariant> map;

    GeneralVoucherDatabaseHelper vHelper;
    obj->voucherNumber = vHelper.setNextVoucherNoByType(VoucherType, obj->VoucherPrefix, Delivery_Note_Main_TableName );

    if(obj->TransactionId.length()==0)
        obj->TransactionId=QString::number(QDateTime::currentDateTime().toTime_t()) + obj->VoucherPrefix + db->clientId;


    map.insert(Delivery_Note_Main_Voucher_No,obj->voucherNumber);
    map.insert(Delivery_Note_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Delivery_Note_Main_Date_Created,obj->VoucherDate);
    map.insert(Delivery_Note_Main_Voucher_Date,obj->VoucherDate);
    map.insert(Delivery_Note_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    map.insert(Delivery_Note_Main_Billing_Name,obj->ledgerObject.LedgerName);
    map.insert(Delivery_Note_Main_Total,obj->grandTotal);
    map.insert(Delivery_Note_Main_Narration,obj->narration);
    map.insert(Delivery_Note_Main_Added_By,obj->AddedBy);
    map.insert(Delivery_Note_Main_Delivery_Date,obj->DeliveryDate);
    map.insert(Delivery_Note_Main_Completion_Probability,obj->CompletionProbability);
    map.insert(Delivery_Note_Main_Credit_Period,obj->ledgerObject.Credit_Period);
    map.insert(Delivery_Note_Main_POC_Name,obj->ledgerObject.ContactPersonName);
    map.insert(Delivery_Note_Main_POC_Phone,obj->ledgerObject.ContactPersonNumber);
    map.insert(Delivery_Note_Main_POC_Email,obj->ledgerObject.ContactPersonEmail);
    map.insert(Delivery_Note_Main_Project_ID,obj->ProjectId);
    map.insert(Delivery_Note_Main_Revision_Number,obj->RevisionNo);
    map.insert(Delivery_Note_Main_Converted_To_Sales_Order,obj->ConvertedToSalesOrder);
    map.insert(Delivery_Note_Main_Quotation_Dropped,obj->QuotationDropped);
    map.insert(Delivery_Note_Main_Quotation_Dropped_Reason,obj->QuotationDroppedReason);
    map.insert(Delivery_Note_Main_Requirement_Voucher_No,obj->RequirementVoucherNo);
    map.insert(Delivery_Note_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Delivery_Note_Main_LPO,obj->LPO);
    map.insert(Delivery_Note_Main_TimeStampC,obj->timestamp);
    map.insert(Delivery_Note_Main_Last_Updated,QDateTime::currentDateTime());
    map.insert(Delivery_Note_Main_Godown,obj->fromGodownID);
    map.insert(Delivery_Note_Main_Discount_In_Amount,obj->discountinAmount);
    map.insert(Delivery_Note_Main_Discount_In_Percent,obj->discountPercent);
    map.insert(Delivery_Note_Main_GeneratedTransactionId, obj->TransactionId);

    QJsonDocument doc(obj->ReqVoucherList);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    map.insert(Delivery_Note_Main_ReqVoucherList,strJson);

    db->insertRecordsPlus(map,Delivery_Note_Main_TableName);

    for(CompoundItemDataObject compitem:obj->InventoryItems){
        QMap<QString,QVariant> map1;
        inventoryItemDataModel item = compitem.BaseItem;
        if(item.ItemReqUuid == "X")
            item.ItemReqUuid = db->getUUID();
        map1.insert(Delivery_Note_Details_Voucher_No,obj->voucherNumber);
        map1.insert(Delivery_Note_Details_Voucher_Prefix,obj->VoucherPrefix);
        map1.insert(Delivery_Note_Details_Voucher_Date,obj->VoucherDate);
        map1.insert(Delivery_Note_Details_Inventory_Item_ID,item.ItemID);
        map1.insert(Delivery_Note_Details_Inventory_Item_ID,item.ItemID);
        map1.insert(Delivery_Note_Details_Quantity,item.quantity);
        map1.insert(Delivery_Note_Details_UOM_ID,item.uomObject._id);


        map1.insert(Delivery_Note_Details_Price,item.price);
        map1.insert(Delivery_Note_Details_PriceLevel,item.PriceLevel);
        map1.insert(Delivery_Note_Details_isCompoundItem,item.IsCompoundItem);
        map1.insert(Delivery_Note_Details_subItemOff_list_ID,0);
        map1.insert(Delivery_Note_Details_Discount_Percent,item.discountPercentage);
        map1.insert(Delivery_Note_Details_Discount_Amount,item.discountinAmount);
        map1.insert(Delivery_Note_Details_Subtotal,item.subTotal);
        map1.insert(Delivery_Note_Details_Project_ID,obj->ProjectId);
        map1.insert(Delivery_Note_Details_Item_Narration,item.narration);
        map1.insert(Delivery_Note_Details_Item_Description,item.ItemDescription);
        map1.insert(Delivery_Note_Details_Salesman_ID,obj->SalesmanID);
        map1.insert(Delivery_Note_Details_Requirement_ItemID,item.ItemReqUuid);
        map1.insert(Delivery_Note_Details_Length,item.length);
        map1.insert(Delivery_Note_Details_TaxRate, item.taxRate);
        map1.insert(Delivery_Note_Details_TaxAmount, item.taxAmount);
        map1.insert(Delivery_Note_Details_Time, QTime::currentTime());

        int basid = db->insertRecordsPlus(map1,Delivery_Note_Details_TableName);

        ItemModel = new ItemTransactionDataModel;
        if(!itemTransaction->hasRecordsForVoucherTypeByItemReqId(item.ItemReqUuid, GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesVoucher))){

            qDebug()<<"Insert item trasaction for DN";
            ItemModel->ItemID = item.ItemID;
            ItemModel->From_Ledger = item.defaultSalesLedgerID;
            ItemModel->To_Ledger = obj->ledgerObject.LedgerID;
            ItemModel->VoucherID = obj->voucherNumber;
            ItemModel->VoucherPrefix = obj->VoucherPrefix;
            ItemModel->VoucherDate = obj->VoucherDate;
            ItemModel->VoucherType = VoucherType;
//            ItemModel->From_Location = "Godown";
            ItemModel->From_Location = item.fromGodownID;
            ItemModel->To_Location = "OUT";
            ItemModel->crQty = item.quantity;
            ItemModel->drQty = 0;
            ItemModel->crAmount = item.subTotal;
            ItemModel->itemReqId = item.ItemReqUuid;
            ItemModel->Narration = obj->narration;
            ItemModel->Price = item.price;

            itemTransaction->insertItemTransaction(ItemModel, item.godownList);

        }


        for(inventoryItemDataModel subitem:compitem.subItems){
            QMap<QString,QVariant> map2;
            if(subitem.ItemReqUuid == "X")
                subitem.ItemReqUuid = db->getUUID();
            map2.insert(Delivery_Note_Details_Voucher_No,obj->voucherNumber);
            map2.insert(Delivery_Note_Details_Voucher_Prefix,obj->VoucherPrefix);
            map2.insert(Delivery_Note_Details_Voucher_Date,obj->VoucherDate);
            map2.insert(Delivery_Note_Details_Inventory_Item_ID,subitem.ItemID);
            map2.insert(Delivery_Note_Details_Quantity,subitem.quantity);
            map2.insert(Delivery_Note_Details_UOM_ID,subitem.uomObject._id);
            map2.insert(Delivery_Note_Details_Price,subitem.price);
            map2.insert(Delivery_Note_Details_PriceLevel,subitem.PriceLevel);
            map2.insert(Delivery_Note_Details_isCompoundItem,subitem.IsCompoundItem);
            map2.insert(Delivery_Note_Details_subItemOff_list_ID,basid);
            map2.insert(Delivery_Note_Details_Discount_Percent,subitem.discountPercentage);
            map2.insert(Delivery_Note_Details_Discount_Amount,subitem.discountinAmount);
            map2.insert(Delivery_Note_Details_Subtotal,subitem.subTotal);
            map2.insert(Delivery_Note_Details_Project_ID,obj->ProjectId);
            map2.insert(Delivery_Note_Details_Item_Narration,subitem.narration);
            map2.insert(Delivery_Note_Details_Item_Description,subitem.ItemDescription);
            map2.insert(Delivery_Note_Details_Salesman_ID,obj->SalesmanID);
            map2.insert(Delivery_Note_Details_Requirement_ItemID,subitem.ItemReqUuid);
            map2.insert(Delivery_Note_Details_Length,subitem.length);
            map2.insert(Delivery_Note_Details_TaxRate, subitem.taxRate);
            map2.insert(Delivery_Note_Details_TaxAmount, subitem.taxAmount);


            db->insertRecordsPlus(map2,Delivery_Note_Details_TableName);

        }
    }

//    return commit();

    int c=commit();

    if( c && obj->ledgerObject.dbName != "" && !obj->fromExternal && obj->sendFlag){
        GeneralVoucherDataObject v;
        NotificationDataModel *notification = new NotificationDataModel();
        QJsonDocument doc(v.ObjToJson(obj));
        QString strJson(doc.toJson(QJsonDocument::Compact));

        notification->NotificationData = strJson;
        notification->ToDBName=obj->ledgerObject.dbName;
        notification->TypeOfData="Voucher";
        notification->Action=1001;
        notification->GeneratedTransactionId=obj->TransactionId;

        sendChannelDatabaseHelper *sendHelper = new sendChannelDatabaseHelper();
        sendHelper->insertNotification(notification);
    }

    return c;

}

int DeliveryNoteDatabaseHelper::updateVoucher(GeneralVoucherDataObject *obj, QString oldID)
{
    this->startTransaction();
    QMap<QString,QVariant> map5;
    map5.insert(Delivery_Note_Details_Voucher_No,oldID);
    map5.insert(Delivery_Note_Details_Voucher_Prefix,obj->VoucherPrefix);
    db->deleteRecordsPlus(map5,Delivery_Note_Details_TableName);
    itemTransaction->deleteItemTransaction(obj);

    QMap<QString,QVariant> map;
    map.insert(Delivery_Note_Main_Voucher_No,obj->voucherNumber);
    map.insert(Delivery_Note_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Delivery_Note_Main_Date_Created,obj->VoucherDate);
    map.insert(Delivery_Note_Main_Voucher_Date,obj->VoucherDate);
    map.insert(Delivery_Note_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    map.insert(Delivery_Note_Main_Billing_Name,obj->ledgerObject.LedgerName);
    map.insert(Delivery_Note_Main_Total,obj->grandTotal);
    map.insert(Delivery_Note_Main_Narration,obj->narration);
    map.insert(Delivery_Note_Main_Added_By,obj->AddedBy);
    map.insert(Delivery_Note_Main_Delivery_Date,obj->DeliveryDate);
    map.insert(Delivery_Note_Main_Completion_Probability,obj->CompletionProbability);
    map.insert(Delivery_Note_Main_Credit_Period,obj->ledgerObject.Credit_Period);
    map.insert(Delivery_Note_Main_POC_Name,obj->ledgerObject.ContactPersonName);
    map.insert(Delivery_Note_Main_POC_Phone,obj->ledgerObject.ContactPersonNumber);
    map.insert(Delivery_Note_Main_POC_Email,obj->ledgerObject.ContactPersonEmail);
    map.insert(Delivery_Note_Main_Project_ID,obj->ProjectId);
    map.insert(Delivery_Note_Main_Revision_Number,obj->RevisionNo);
    map.insert(Delivery_Note_Main_Converted_To_Sales_Order,obj->ConvertedToSalesOrder);
    map.insert(Delivery_Note_Main_Quotation_Dropped,obj->QuotationDropped);
    map.insert(Delivery_Note_Main_Quotation_Dropped_Reason,obj->QuotationDroppedReason);
    map.insert(Delivery_Note_Main_Requirement_Voucher_No,obj->RequirementVoucherNo);
    map.insert(Delivery_Note_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Delivery_Note_Main_LPO,obj->LPO);
    map.insert(Delivery_Note_Main_Last_Updated,QDateTime::currentDateTime());
    map.insert(Delivery_Note_Main_Godown,obj->fromGodownID);
    map.insert(Delivery_Note_Main_Discount_In_Amount,obj->discountinAmount);
    map.insert(Delivery_Note_Main_Discount_In_Percent,obj->discountPercent);
    map.insert(Delivery_Note_Main_GeneratedTransactionId, obj->TransactionId);

    QJsonDocument doc(obj->ReqVoucherList);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    map.insert(Delivery_Note_Main_ReqVoucherList,strJson);


    QMap<QString,QVariant> map3;
    map3.insert(Delivery_Note_Main_Voucher_No,oldID);
    map3.insert(Delivery_Note_Main_Voucher_Prefix,obj->VoucherPrefix);
    db->updateRecordsPlus(map,map3,Delivery_Note_Main_TableName);

    QMap<QString,QVariant> map6;
    map6.insert(Delivery_Note_Details_Voucher_No,oldID);
    map6.insert(Delivery_Note_Details_Voucher_Prefix,obj->VoucherPrefix);
    db->deleteRecordsPlus(map6,Delivery_Note_Details_TableName);



    for(CompoundItemDataObject compitem:obj->InventoryItems){
        QMap<QString,QVariant> map1;
        inventoryItemDataModel item = compitem.BaseItem;
        if(item.ItemReqUuid == "X")
            item.ItemReqUuid = db->getUUID();

        map1.insert(Delivery_Note_Details_Voucher_No,obj->voucherNumber);
        map1.insert(Delivery_Note_Details_Voucher_Prefix,obj->VoucherPrefix);
        map1.insert(Delivery_Note_Details_Voucher_Date,obj->VoucherDate);
        map1.insert(Delivery_Note_Details_Inventory_Item_ID,item.ItemID);
        map1.insert(Delivery_Note_Details_Quantity,item.quantity);
        map1.insert(Delivery_Note_Details_UOM_ID,item.uomObject._id);
        map1.insert(Delivery_Note_Details_Price,item.price);
        map1.insert(Delivery_Note_Details_PriceLevel,item.PriceLevel);
        map1.insert(Delivery_Note_Details_isCompoundItem,item.IsCompoundItem);
        map1.insert(Delivery_Note_Details_subItemOff_list_ID,0);
        map1.insert(Delivery_Note_Details_Discount_Percent,item.discountPercentage);
        map1.insert(Delivery_Note_Details_Discount_Amount,item.discountinAmount);
        map1.insert(Delivery_Note_Details_Subtotal,item.subTotal);
        map1.insert(Delivery_Note_Details_Project_ID,obj->ProjectId);
        map1.insert(Delivery_Note_Details_Item_Narration,item.narration);
        map1.insert(Delivery_Note_Details_Item_Description,item.ItemDescription);
        map1.insert(Delivery_Note_Details_Salesman_ID,obj->SalesmanID);
        map1.insert(Delivery_Note_Details_Requirement_ItemID,item.ItemReqUuid);
        map1.insert(Delivery_Note_Details_Length,item.length);
        qDebug()<<"tax rate"<<item.taxRate;
        map1.insert(Delivery_Note_Details_TaxRate, item.taxRate);
        map1.insert(Delivery_Note_Details_TaxAmount, item.taxAmount);

        int basid = db->insertRecordsPlus(map1,Delivery_Note_Details_TableName);

        ItemModel = new ItemTransactionDataModel;
        if(!itemTransaction->hasRecordsForVoucherTypeByItemReqId(item.ItemReqUuid, GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesVoucher))){

            qDebug()<<"Insert item trasaction for DN"<<item.fromGodownID<<item.ItemName;
            ItemModel->ItemID = item.ItemID;
            ItemModel->From_Ledger = item.defaultSalesLedgerID;
            ItemModel->To_Ledger = obj->ledgerObject.LedgerID;
            ItemModel->VoucherID = obj->voucherNumber;
            ItemModel->VoucherPrefix = obj->VoucherPrefix;
            ItemModel->VoucherDate = obj->VoucherDate;
            ItemModel->VoucherType = VoucherType;

            ItemModel->From_Location = item.fromGodownID;
            ItemModel->To_Location = "OUT";
            ItemModel->crQty = item.quantity;
            ItemModel->drQty = 0;
            ItemModel->crAmount = item.subTotal;
            ItemModel->itemReqId = item.ItemReqUuid;
            ItemModel->Narration = obj->narration;
            ItemModel->Price = item.price;
            itemTransaction->insertItemTransaction(ItemModel, item.godownList);
        }

        for(inventoryItemDataModel subitem:compitem.subItems){
            QMap<QString,QVariant> map2;
            map2.insert(Delivery_Note_Details_Voucher_No,obj->voucherNumber);
            map2.insert(Delivery_Note_Details_Voucher_Prefix,obj->VoucherPrefix);
            map2.insert(Delivery_Note_Details_Voucher_Date,obj->VoucherDate);
            map2.insert(Delivery_Note_Details_Inventory_Item_ID,subitem.ItemID);
            map2.insert(Delivery_Note_Details_Quantity,subitem.quantity);
            map2.insert(Delivery_Note_Details_UOM_ID,subitem.uomObject._id);
            map2.insert(Delivery_Note_Details_Price,subitem.price);
            map2.insert(Delivery_Note_Details_PriceLevel,subitem.PriceLevel);
            map2.insert(Delivery_Note_Details_isCompoundItem,subitem.IsCompoundItem);
            map2.insert(Delivery_Note_Details_subItemOff_list_ID,basid);
            map2.insert(Delivery_Note_Details_Discount_Percent,subitem.discountPercentage);
            map2.insert(Delivery_Note_Details_Discount_Amount,subitem.discountinAmount);
            map2.insert(Delivery_Note_Details_Subtotal,subitem.subTotal);
            map2.insert(Delivery_Note_Details_Project_ID,obj->ProjectId);
            map2.insert(Delivery_Note_Details_Item_Narration,subitem.narration);
            map2.insert(Delivery_Note_Details_Item_Description,subitem.ItemDescription);
            map2.insert(Delivery_Note_Details_Salesman_ID,obj->SalesmanID);
            map2.insert(Delivery_Note_Details_Requirement_ItemID,subitem.ItemReqUuid);
            map2.insert(Delivery_Note_Details_Length,subitem.length);
            map2.insert(Delivery_Note_Details_TaxRate, subitem.taxRate);
            map2.insert(Delivery_Note_Details_TaxAmount, subitem.taxAmount);
            db->insertRecordsPlus(map2,Delivery_Note_Details_TableName);
        }
    }

    int c=commit();

    if( c && obj->ledgerObject.dbName != "" && !obj->fromExternal && obj->sendFlag){
        GeneralVoucherDataObject v;
        NotificationDataModel *notification = new NotificationDataModel();
        QJsonDocument doc(v.ObjToJson(obj));
        QString strJson(doc.toJson(QJsonDocument::Compact));
        notification->NotificationData = strJson;
        notification->ToDBName=obj->ledgerObject.dbName;
        notification->TypeOfData="Voucher";
        notification->Action=1002;
        notification->GeneratedTransactionId=obj->TransactionId;
        sendChannelDatabaseHelper *sendHelper = new sendChannelDatabaseHelper();
        sendHelper->insertNotification(notification);
    }

    return c;

}

int DeliveryNoteDatabaseHelper::deleteVoucher(GeneralVoucherDataObject *obj)
{
    this->startTransaction();
    QMap<QString,QVariant> map3;
    map3.insert(Delivery_Note_Main_Voucher_No,obj->voucherNumber);
    map3.insert(Delivery_Note_Main_Voucher_Prefix,obj->VoucherPrefix);
    db->deleteRecordsPlus(map3,Delivery_Note_Main_TableName);

    QMap<QString,QVariant> map4;
    map4.insert(Delivery_Note_Details_Voucher_No,obj->voucherNumber);
    map4.insert(Delivery_Note_Details_Voucher_Prefix,obj->VoucherPrefix);
    db->deleteRecordsPlus(map4,Delivery_Note_Details_TableName);

    itemTransaction->deleteItemTransaction(obj);
    return commit();
}

int DeliveryNoteDatabaseHelper::commit()
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

int DeliveryNoteDatabaseHelper::getMaxID()
{
    QString qStr = "SELECT MAX("+Delivery_Note_Main_id+") FROM " + Delivery_Note_Main_TableName;
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



GeneralVoucherDataObject *DeliveryNoteDatabaseHelper::getVoucherByVoucherNoPtr(QString vNo, QString voucherPrefix)
{
    GeneralVoucherDataObject* obj= new GeneralVoucherDataObject;
    SalesInventoryItemDatabaseHelper *itemHelper = new SalesInventoryItemDatabaseHelper();
    LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper();


    QString qStrMain ="SELECT distinct "
            + Delivery_Note_Main_Voucher_No + ", "
            + Delivery_Note_Main_Voucher_Prefix + ", "
            + Delivery_Note_Main_Date_Created + ", "
            + Delivery_Note_Main_Voucher_Date + ", "
            + Delivery_Note_Main_TimeStamp + ", "
            + Delivery_Note_Main_Ledger_ID + ", "
            + Delivery_Note_Main_Billing_Name + ", "
            + Delivery_Note_Main_Total + ", "
            + Delivery_Note_Main_Narration + ", "
            + Delivery_Note_Main_Added_By + ", "
            + Delivery_Note_Main_Delivery_Date + ", "
            + Delivery_Note_Main_Completion_Probability + ", "
            + Delivery_Note_Main_Credit_Period + ", "
            + Delivery_Note_Main_POC_Name + ", "
            + Delivery_Note_Main_POC_Phone + ", "
            + Delivery_Note_Main_POC_Email + ", "
            + Delivery_Note_Main_Project_ID + ", "
            + Delivery_Note_Main_Revision_Number + ", "
            + Delivery_Note_Main_Converted_To_Sales_Order + ", "
            + Delivery_Note_Main_Quotation_Dropped + ", "
            + Delivery_Note_Main_Quotation_Dropped_Reason + ", "
            + Delivery_Note_Main_Requirement_Voucher_No + ", "
            + Delivery_Note_Main_Salesman_ID + ", "
            + Delivery_Note_Main_LPO + ", "
            + Delivery_Note_Main_VoucherStatus + ", "
            + Delivery_Note_Main_ModeOfService + ", "
            + Delivery_Note_Main_Discount_In_Amount + ", "
            + Delivery_Note_Main_Discount_In_Percent + ", "
            + Delivery_Note_Main_VATAmount + ", "
            + Delivery_Note_Main_Contact_ID + ", "
            + Delivery_Note_Main_Gross_Total + ", "
            + Delivery_Note_Main_Reference + ", "
            + Delivery_Note_Main_Customer_Expecting_Date + ", "
            + Delivery_Note_Main_Currency + ", "
            + Delivery_Note_Main_BoxesCount+ ", "
            + Delivery_Note_Main_TotalWeight + ", "
            + Delivery_Note_Main_LRNo + ", "
            + Delivery_Note_Main_VoucherNoDisplay + ", "
            + Delivery_Note_Main_ReqVoucherList + ", "
            + Delivery_Note_Main_Origin +", "
            + Delivery_Note_Main_Godown + ", "
            + Delivery_Note_Main_GeneratedTransactionId
            + " FROM " + Delivery_Note_Main_TableName
            + " WHERE " + Delivery_Note_Main_Voucher_No + "= :vno"
            + " AND " +  Delivery_Note_Main_Voucher_Prefix + "= :voucherPrefix";

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
            + " WHERE " + Proxy_Transaction_Helper_VoucherNo + "=:id"
            + " AND " + Proxy_Transaction_Helper_VoucherType + "='"+VoucherType+"'";

    QString qStrDetail = "SELECT distinct " + Delivery_Note_Details_Voucher_No
            + ", " + Delivery_Note_Details_Voucher_Prefix
            + ", " + Delivery_Note_Details_Voucher_Date
            + ", " + Delivery_Note_Details_Inventory_Item_ID
            + ", " + Delivery_Note_Details_Quantity
            + ", " + Delivery_Note_Details_Price
            + ", " + Delivery_Note_Details_UOM_ID
            + ", subq.convalue , subq.UomName, subq.UomSymbol "
            + ", " + Delivery_Note_Details_PriceLevel
            + ", " + Delivery_Note_Details_Discount_Percent
            + ", " + Delivery_Note_Details_Discount_Amount
            + ", " + Delivery_Note_Details_Subtotal
            + ", " + Delivery_Note_Details_Item_Narration
            + ", " + Delivery_Note_Details_Item_Description
            + ", " + Delivery_Note_Details_Salesman_ID
            + ", " + Delivery_Note_Details_Requirement_ItemID
            + ", " + Delivery_Note_Details_Length
            + ", " + Delivery_Note_Details_Dimension
            + ", " + Delivery_Note_Details_itemVoucherStatus
            + ", " + Delivery_Note_Details_itemProductionStatus
            + ", "  + Delivery_Note_Details_TaxRate
            + ", " + Delivery_Note_Details_TaxAmount
            + ", " + Delivery_Note_Details_Time
            + ", " + Delivery_Note_Details_isCompoundItem
            + ", " + Delivery_Note_Details_subItemOff_list_ID
            + " FROM " + Delivery_Note_Details_TableName+ " s LEFT OUTER JOIN "
            + UomDataBaseHelper::qStrUomForItemDetail + " as subq "
            + " ON s." + Delivery_Note_Details_Inventory_Item_ID + " = subq.itemID "
            + " AND s." + Delivery_Note_Details_UOM_ID + " = subq.UOM "
            + " WHERE " + Delivery_Note_Details_Voucher_No + "= :vno"
            + " AND " +  Delivery_Note_Details_Voucher_Prefix + "= :voucherPrefix";

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
            item.prevQty = item.quantity;
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
            item.salesMan_id  = query.value(i++).toString();
            item.ItemReqUuid = query.value(i++).toString();
            item.length = query.value(i++).toFloat();
            item.Dimension = query.value(i++).toString();
            item.itemVoucherStatus = query.value(i++).toInt();
            item.itemProductionStatus = query.value(i++).toInt();
            item.taxRate = query.value(i++).toFloat();
            item.taxAmount = query.value(i++).toFloat();
            item.godownList = itemTransaction->getGodownAllocationByVoucherAndItem(vNo, VoucherType,
                                                                                   voucherPrefix, item.ItemReqUuid);
//            item.fifoList = itemTransaction->getFifoListByVoucherItem(vNo, obj->voucherType,
//                                                                      voucherPrefix, item.ItemID);
//            if(item.fifoList.length()==1){
//                item.toGodownID = item.fifoList[0]->To_Location;
//                item.fromGodownID = item.fifoList[0]->From_Location;
//                item.fifoList.clear();
//            }
            i++;
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

//            item.fifoList = itemTransaction->getFifoListByVoucherItem(obj->voucherNumber, obj->voucherType,
//                                                                               obj->VoucherPrefix, item.ItemID);
        }
    }
    query.prepare(qStrMain);
    query.bindValue(":vno",vNo);
    query.bindValue(":voucherPrefix", voucherPrefix);
    if(!query.exec()){
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    else{

        while(query.next()){
            int i=0;

            obj->voucherNumber = query.value(i++).toString();
            obj->VoucherPrefix = query.value(i++).toString();
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
            obj->ConvertedToSalesOrder = query.value(i++).toString();
            obj->QuotationDropped  = query.value(i++).toBool();
            obj->QuotationDroppedReason = query.value(i++).toString();
            obj->RequirementVoucherNo = query.value(i++).toString();
            obj->SalesmanID = query.value(i++).toInt();
            obj->LPO = query.value(i++).toString();
            obj->status = query.value(i++).toInt();
            obj->ModeOfService = query.value(i++).toInt();
            obj->discountinAmount = query.value(i++).toFloat();
            obj->discountPercent = query.value(i++).toFloat();
            obj->taxTotalAmount = query.value(i++).toFloat();
            obj->Contact._id = query.value(i++).toInt();
            obj->grossTotal = query.value(i++).toFloat();
            obj->reference = query.value(i++).toString();
            obj->CustomerExpectingDate = query.value(i++).toDateTime();
            obj->currency = query.value(i++).toString();
            obj->numBoxes = query.value(i++).toInt();
            obj->totalWeight = query.value(i++).toFloat();
            obj->LRNO = query.value(i++).toString();
            obj->_DisplayVoucherNumber = query.value(i++).toString();
            obj->ReqVoucherList = query.value(i++).toJsonArray();
            obj->Origin = query.value(i++).toInt();
            obj->fromGodownID = query.value(i++).toString();
            obj->TransactionId = query.value(i++).toString();

        }
    }

    obj->voucherType = VoucherType;
    return obj;
}



QSqlQueryModel *DeliveryNoteDatabaseHelper::getVoucherListByDateModel(QDate dateFrom, QDate dateTo)
{
    LedgerMasterDatabaseHelper ledHelper;
    QString qStr ="SELECT ";
    qStr += "'V' , ";
    qStr += Delivery_Note_Main_Voucher_No +", ";
    qStr += "'" + VoucherType + "', " ;
    qStr += Delivery_Note_Main_Voucher_Prefix + ", ";
    qStr += Delivery_Note_Main_Voucher_Date+" as 'Voucher Date', ";
    qStr += " CONCAT(" + Delivery_Note_Main_Voucher_Prefix + ", '-', " + Delivery_Note_Main_Voucher_No+ ") as 'Voucher No', ";
    qStr += "(SELECT "+ledHelper.Ledger_Master_LedgerName + " FROM "+ ledHelper.Ledger_Master_TableName;
    qStr += " WHERE "+ledHelper.Ledger_Master_LedgerID + "= som.";
    qStr += Delivery_Note_Main_Ledger_ID + ") Ledger, ";
    qStr += Delivery_Note_Main_Total ;
    qStr += " FROM " + Delivery_Note_Main_TableName;
    qStr += " som WHERE ";
    qStr += Delivery_Note_Main_Voucher_Date;
    qStr += " >= '";
    qStr += dateFrom.toString("yyyy-MM-dd");
    qStr += "' AND ";
    qStr += Delivery_Note_Main_Voucher_Date;
    qStr += " <= '";
    qStr += dateTo.toString("yyyy-MM-dd");
    qStr += "'";

    return DatabaseHelper::getQueryModel(qStr);
}

void DeliveryNoteDatabaseHelper::updateSignedStatus(bool status, QString dnNO)
{
    QMap<QString,QVariant> map;
    QMap<QString,QVariant> clause;

    map.insert(Delivery_Note_Main_DN_Signed,status);

    clause.insert(Delivery_Note_Main_Voucher_No,dnNO);
    int n = db->updateRecordsPlus(map,clause,Delivery_Note_Main_TableName);
    qDebug()<<"Updtaed recors"<<n<<status<<dnNO;

}

bool DeliveryNoteDatabaseHelper::getSignatureStatus(QString dnNO)
{
    bool status = false;
    QString qStr ="SELECT "
            + Delivery_Note_Main_DN_Signed
            + " FROM "
            + Delivery_Note_Main_TableName
            + " WHERE "
            + Delivery_Note_Main_Voucher_No + "='"+dnNO +"'";
    ;
    qDebug()<<qStr;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);

    if(query.exec()){
        if(query.first()){
            status = query.value(0).toBool();
        }
    }
    else{
        qDebug()<<qStr;
        qDebug()<<query.lastError();
    }
    return status;
}



void DeliveryNoteDatabaseHelper::setItemVoucherStatus(float qty, QString id)
{
    QString qStr ="SELECT "+Delivery_Note_Details_itemVoucherStatus+ " from "+ Delivery_Note_Details_TableName +
            " WHERE "+Delivery_Note_Details_Requirement_ItemID + " = " + id;
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

    map.insert(Delivery_Note_Details_itemVoucherStatus,qty);
    clause.insert(Delivery_Note_Details_Requirement_ItemID,id);
    db->updateRecordsPlus(map,clause,Delivery_Note_Details_TableName);
}

void DeliveryNoteDatabaseHelper::resetItemVoucherStatus(float qty, QString id)
{
    QString qStr ="SELECT "+Delivery_Note_Details_itemVoucherStatus+ " from "+ Delivery_Note_Details_TableName +
            " WHERE "+Delivery_Note_Details_Requirement_ItemID + " = " + id;
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

    map.insert(Delivery_Note_Details_itemVoucherStatus,qty);
    clause.insert(Delivery_Note_Details_Requirement_ItemID,id);
    db->updateRecordsPlus(map,clause,Delivery_Note_Details_TableName);
}

QSqlQueryModel *DeliveryNoteDatabaseHelper::getVoucherListByDateTimeModel(QDateTime dateFrom, QDateTime dateTo)
{

}

