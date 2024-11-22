#include "debitnotevoucherdatabasehelper.h"

QString DebitNoteVoucherDatabaseHelper::Debit_Note_Main_TableName = "Debit_Note_Main";
DebitNoteVoucherDatabaseHelper::DebitNoteVoucherDatabaseHelper(QString connString)
{
    db = new DatabaseHelper(connString);
    itemTransaction = new ItemTransactionDatabaseHelper(db);
    ItemModel = new ItemTransactionDataModel();
    ledTransaction = new TransactionHelper(db);
}

DebitNoteVoucherDatabaseHelper::~DebitNoteVoucherDatabaseHelper()
{

}

int DebitNoteVoucherDatabaseHelper::startTransaction()
{
    db->startTransaction();

}

QString DebitNoteVoucherDatabaseHelper::getNextVoucherNo(QString voucherPrefix)
{
    GeneralVoucherDatabaseHelper vHelper;
    QString voucherNumber = vHelper.getNextVoucherNoByType(VoucherType, voucherPrefix, Debit_Note_Main_TableName);
    return voucherNumber;

}

QPair<QString, QString> DebitNoteVoucherDatabaseHelper::getVoucherNoByTransactionId(QString transId)
{
    GeneralVoucherDatabaseHelper vHelper;
    return vHelper.getVoucherNoByTransactionId(transId, Debit_Note_Main_TableName);


}

QString DebitNoteVoucherDatabaseHelper::getVoucherNumberByLedID(QString LedID, QDate Date)
{

}

QSqlQueryModel *DebitNoteVoucherDatabaseHelper::getVoucherListByDateTimeModel(QDateTime dateFrom, QDateTime dateTo)
{

}
int DebitNoteVoucherDatabaseHelper::insertVoucher(GeneralVoucherDataObject *obj)
{
    this->startTransaction();
    QMap<QString,QVariant> map;

    GeneralVoucherDatabaseHelper vHelper;
    obj->voucherNumber = vHelper.setNextVoucherNoByType(VoucherType, obj->VoucherPrefix, Debit_Note_Main_TableName );

    if(obj->TransactionId.length()==0)
    obj->TransactionId=QString::number(QDateTime::currentDateTime().toTime_t()) + obj->VoucherPrefix + db->clientId;

    map.insert(Debit_Note_Main_Voucher_No,obj->voucherNumber);
    map.insert(Debit_Note_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Debit_Note_Main_Date_Created,obj->VoucherDate);
    map.insert(Debit_Note_Main_Voucher_Date,obj->VoucherDate);
    map.insert(Debit_Note_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    map.insert(Debit_Note_Main_Billing_Name,obj->ledgerObject.LedgerName);
    map.insert(Debit_Note_Main_Total,obj->grandTotal);
    map.insert(Debit_Note_Main_Narration,obj->narration);
    map.insert(Debit_Note_Main_Added_By,obj->AddedBy);
    map.insert(Debit_Note_Main_Delivery_Date,obj->DeliveryDate);
    map.insert(Debit_Note_Main_Completion_Probability,obj->CompletionProbability);
    map.insert(Debit_Note_Main_Credit_Period,obj->ledgerObject.Credit_Period);
    map.insert(Debit_Note_Main_POC_Name,obj->ledgerObject.ContactPersonName);
    map.insert(Debit_Note_Main_POC_Phone,obj->ledgerObject.ContactPersonNumber);
    map.insert(Debit_Note_Main_POC_Email,obj->ledgerObject.ContactPersonEmail);
    map.insert(Debit_Note_Main_Project_ID,obj->ProjectId);
    map.insert(Debit_Note_Main_Revision_Number,obj->RevisionNo);
    map.insert(Debit_Note_Main_Converted_To_Sales_Order,obj->ConvertedToSalesOrder);
    map.insert(Debit_Note_Main_Quotation_Dropped,obj->QuotationDropped);
    map.insert(Debit_Note_Main_Quotation_Dropped_Reason,obj->QuotationDroppedReason);
    map.insert(Debit_Note_Main_Requirement_Voucher_No,obj->RequirementVoucherNo);
    map.insert(Debit_Note_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Debit_Note_Main_Generated_TransactionId, obj->TransactionId);
    map.insert(Debit_Note_Main_Godown, obj->fromGodownID);
    map.insert(Debit_Note_Main_TimeStampC,obj->timestamp);
    map.insert(Debit_Note_Main_Last_Updated,QDateTime::currentDateTime());
    map.insert(Debit_Note_Main_VoucherStatus,obj->status);
    map.insert(Debit_Note_Main_Discount_In_Amount,obj->discountinAmount);
    map.insert(Debit_Note_Main_Discount_In_Percent,obj->discountPercent);

    QJsonDocument doc(obj->ReqVoucherList);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    map.insert(Debit_Note_Main_ReqVoucherList, strJson);

    db->insertRecordsPlus(map,Debit_Note_Main_TableName);

    for(CompoundItemDataObject compitem:obj->InventoryItems){
        QMap<QString,QVariant> map1;
        inventoryItemDataModel item = compitem.BaseItem;
        if(item.ItemReqUuid == "X")
            item.ItemReqUuid = db->getUUID();
        map1.insert(Debit_Note_Details_Voucher_No,obj->voucherNumber);
        map1.insert(Debit_Note_Details_Voucher_Prefix,obj->VoucherPrefix);
        map1.insert(Debit_Note_Details_Voucher_Date,obj->VoucherDate);
        map1.insert(Debit_Note_Details_Inventory_Item_ID,item.ItemID);
        map1.insert(Debit_Note_Details_Quantity,item.quantity);
        map1.insert(Debit_Note_Details_Price,item.price);

        map1.insert(Debit_Note_Details_PriceLevel,item.PriceLevel);
        map1.insert(Debit_Note_Details_isCompoundItem,item.IsCompoundItem);
        map1.insert(Debit_Note_Details_subItemOff_list_ID,0);
        map1.insert(Debit_Note_Details_Discount_Percent,item.discountPercentage);
        map1.insert(Debit_Note_Details_Discount_Amount,item.discountinAmount);
        map1.insert(Debit_Note_Details_Subtotal,item.subTotal);
        map1.insert(Debit_Note_Details_Project_ID,obj->ProjectId);
        map1.insert(Debit_Note_Details_Item_Narration,item.narration);
        map1.insert(Debit_Note_Details_Item_Description,item.ItemDescription);
        map1.insert(Debit_Note_Details_Salesman_ID,obj->SalesmanID);
        map1.insert(Debit_Note_Details_Requirement_ItemID,item.ItemReqUuid);
        map1.insert(Debit_Note_Details_Dimension,item.Dimension);
        map1.insert(Debit_Note_Details_Length,item.length);
        map1.insert(Debit_Note_Details_TaxRate, item.taxRate);
        map1.insert(Debit_Note_Details_TaxAmount, item.taxAmount);
        map1.insert(Debit_Note_Details_UomId, item.uomObject._id);
        ItemModel->ItemID = item.ItemID;
        ItemModel->From_Ledger = item.DefaultPurchaseReturnLedgerID;
        ItemModel->To_Ledger = obj->ledgerObject.LedgerID;
        ItemModel->VoucherID = obj->voucherNumber;
        ItemModel->VoucherPrefix = obj->VoucherPrefix;
        ItemModel->VoucherDate = obj->VoucherDate;
        ItemModel->VoucherType = VoucherType;

//        ItemModel->From_Location = "Godown";
        ItemModel->From_Location = item.fromGodownID;
        ItemModel->To_Location = "OUT";
        ItemModel->crQty = item.quantity;
        ItemModel->drQty = 0;
        ItemModel->crAmount = item.subTotal;
        ItemModel->Price = item.price;
        ItemModel->itemReqId = item.ItemReqUuid;
        ItemModel->Narration = obj->narration;

        itemTransaction->insertItemTransaction(ItemModel, item.godownList);

        int basid = db->insertRecordsPlus(map1,Debit_Note_Details_TableName);

        for(inventoryItemDataModel subitem:compitem.subItems){
            QMap<QString,QVariant> map2;
            if(subitem.ItemReqUuid == "X")
                subitem.ItemReqUuid = db->getUUID();
            map2.insert(Debit_Note_Details_Voucher_No,obj->voucherNumber);
            map2.insert(Debit_Note_Details_Voucher_Prefix,obj->VoucherPrefix);
            map2.insert(Debit_Note_Details_Voucher_Date,obj->VoucherDate);
            map2.insert(Debit_Note_Details_Inventory_Item_ID,subitem.ItemID);
            map2.insert(Debit_Note_Details_Quantity,subitem.quantity);
            map2.insert(Debit_Note_Details_Price,subitem.price);
            map2.insert(Debit_Note_Details_PriceLevel,subitem.PriceLevel);
            map2.insert(Debit_Note_Details_isCompoundItem,subitem.IsCompoundItem);
            map2.insert(Debit_Note_Details_subItemOff_list_ID,basid);
            map2.insert(Debit_Note_Details_Discount_Percent,subitem.discountPercentage);
            map2.insert(Debit_Note_Details_Discount_Amount,subitem.discountinAmount);
            map2.insert(Debit_Note_Details_Subtotal,subitem.subTotal);
            map2.insert(Debit_Note_Details_Project_ID,obj->ProjectId);
            map2.insert(Debit_Note_Details_Item_Narration,subitem.narration);
            map2.insert(Debit_Note_Details_Item_Description,subitem.ItemDescription);
            map2.insert(Debit_Note_Details_Salesman_ID,obj->SalesmanID);
            map2.insert(Debit_Note_Details_Requirement_ItemID,subitem.ItemReqUuid);
            map2.insert(Debit_Note_Details_Dimension,subitem.Dimension);
            map2.insert(Debit_Note_Details_Length,subitem.length);
            map2.insert(Debit_Note_Details_TaxRate, item.taxRate);
            map2.insert(Debit_Note_Details_TaxAmount, item.taxAmount);

            db->insertRecordsPlus(map2,Debit_Note_Details_TableName);
        }
    }

    LedgerMasterDataModel MainLedger;

    MainLedger.drAmount = obj->grandTotal;
    MainLedger.LedgerID = obj->ledgerObject.LedgerID;
    MainLedger.isInvoiceItem = true;
    MainLedger.mapList = obj->mapList;


    QString agCrID ;
    QString agDrID = obj->ledgerObject.LedgerID ;


    float  amt=0;
    for(int i=0;i<obj->ledgersList.size();i++)
    {
       obj->ledgersList[i].AgainstLedger = agDrID;
       if(obj->ledgersList[i].crAmount>amt)
           agCrID = obj->ledgersList[i].LedgerID;
    }

    if(!obj->paymentSplit)
        obj->ledgersList.insert(0,MainLedger);

    obj->voucherType = VoucherType;

    obj->ledgersList[0].AgainstLedger = "0x8x14";

    ledTransaction->insertTransaction(obj);

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
//        notification->GeneratedTransactionId=QString::number(QDateTime::currentDateTime().toTime_t()) + obj->VoucherPrefix;
        notification->GeneratedTransactionId = obj->TransactionId;

        sendChannelDatabaseHelper *sendHelper = new sendChannelDatabaseHelper();
        sendHelper->insertNotification(notification);
    }

    return c;

}

int DebitNoteVoucherDatabaseHelper::updateVoucher(GeneralVoucherDataObject *obj, QString oldID)
{
    this->startTransaction();
    qDebug()<<"Old ID for update Debit note:"<<oldID;
    QMap<QString,QVariant> map;
    map.insert(Debit_Note_Main_Voucher_No,obj->voucherNumber);
    map.insert(Debit_Note_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Debit_Note_Main_Date_Created,obj->VoucherDate);
    map.insert(Debit_Note_Main_Voucher_Date,obj->VoucherDate);
    map.insert(Debit_Note_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    map.insert(Debit_Note_Main_Billing_Name,obj->ledgerObject.LedgerName);
    map.insert(Debit_Note_Main_Total,obj->grandTotal);
    map.insert(Debit_Note_Main_Narration,obj->narration);
    map.insert(Debit_Note_Main_Added_By,obj->AddedBy);
    map.insert(Debit_Note_Main_Delivery_Date,obj->DeliveryDate);
    map.insert(Debit_Note_Main_Completion_Probability,obj->CompletionProbability);
    map.insert(Debit_Note_Main_Credit_Period,obj->ledgerObject.Credit_Period);
    map.insert(Debit_Note_Main_POC_Name,obj->ledgerObject.ContactPersonName);
    map.insert(Debit_Note_Main_POC_Phone,obj->ledgerObject.ContactPersonNumber);
    map.insert(Debit_Note_Main_POC_Email,obj->ledgerObject.ContactPersonEmail);
    map.insert(Debit_Note_Main_Project_ID,obj->ProjectId);
    map.insert(Debit_Note_Main_Revision_Number,obj->RevisionNo);
    map.insert(Debit_Note_Main_Converted_To_Sales_Order,obj->ConvertedToSalesOrder);
    map.insert(Debit_Note_Main_Quotation_Dropped,obj->QuotationDropped);
    map.insert(Debit_Note_Main_Quotation_Dropped_Reason,obj->QuotationDroppedReason);
    map.insert(Debit_Note_Main_Requirement_Voucher_No,obj->RequirementVoucherNo);
    map.insert(Debit_Note_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Debit_Note_Main_Generated_TransactionId, obj->TransactionId);
    map.insert(Debit_Note_Main_Last_Updated,QDateTime::currentDateTime());
    map.insert(Debit_Note_Main_VoucherStatus,obj->status);
    map.insert(Debit_Note_Main_Godown, obj->fromGodownID);
    map.insert(Debit_Note_Main_Discount_In_Amount,obj->discountinAmount);
    map.insert(Debit_Note_Main_Discount_In_Percent,obj->discountPercent);

    QJsonDocument doc(obj->ReqVoucherList);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    map.insert(Debit_Note_Main_ReqVoucherList, strJson);

    QMap<QString,QVariant> map3;
    map3.insert(Debit_Note_Main_Voucher_No,oldID);
    map3.insert(Debit_Note_Main_Voucher_Prefix, obj->VoucherPrefix);
    db->updateRecordsPlus(map,map3,Debit_Note_Main_TableName);

    QMap<QString,QVariant> maptxndel;
    maptxndel.insert(Transaction_Helper_VoucherNo,oldID);
    maptxndel.insert(Transaction_Helper_VoucherType,VoucherType);
    //    db->deleteRecordsPlus(maptxndel,Transaction_Helper_TableName);

    QMap<QString,QVariant> map6;
    map6.insert(Debit_Note_Details_Voucher_No,oldID);
    map6.insert(Debit_Note_Details_Voucher_Prefix, obj->VoucherPrefix);
    qDebug()<<"Delete debit note:"<<oldID;
    db->deleteRecordsPlus(map6,Debit_Note_Details_TableName);

    itemTransaction->deleteItemTransaction(obj);

    for(CompoundItemDataObject compitem:obj->InventoryItems){
        QMap<QString,QVariant> map1;
        inventoryItemDataModel item = compitem.BaseItem;
        if(item.ItemReqUuid == "X")
            item.ItemReqUuid = db->getUUID();

        map1.insert(Debit_Note_Details_Voucher_No,obj->voucherNumber);
        map1.insert(Debit_Note_Details_Voucher_Prefix,obj->VoucherPrefix);
        map1.insert(Debit_Note_Details_Voucher_Date,obj->VoucherDate);
        map1.insert(Debit_Note_Details_Inventory_Item_ID,item.ItemID);
        map1.insert(Debit_Note_Details_Quantity,item.quantity);
        map1.insert(Debit_Note_Details_Price,item.price);
        map1.insert(Debit_Note_Details_PriceLevel,item.PriceLevel);
        map1.insert(Debit_Note_Details_isCompoundItem,item.IsCompoundItem);
        map1.insert(Debit_Note_Details_subItemOff_list_ID,0);
        map1.insert(Debit_Note_Details_Discount_Percent,item.discountPercentage);
        map1.insert(Debit_Note_Details_Discount_Amount,item.discountinAmount);
        map1.insert(Debit_Note_Details_Subtotal,item.subTotal);
        map1.insert(Debit_Note_Details_Project_ID,obj->ProjectId);
        map1.insert(Debit_Note_Details_Item_Narration,item.narration);
        map1.insert(Debit_Note_Details_Item_Description,item.ItemDescription);
        map1.insert(Debit_Note_Details_Salesman_ID,obj->SalesmanID);
        map1.insert(Debit_Note_Details_Requirement_ItemID,item.ItemReqUuid);
        map1.insert(Debit_Note_Details_Dimension,item.Dimension);
        map1.insert(Debit_Note_Details_Length,item.length);
        map1.insert(Debit_Note_Details_UomId, item.uomObject._id);
        map1.insert(Debit_Note_Details_TaxRate, item.taxRate);
        map1.insert(Debit_Note_Details_TaxAmount, item.taxAmount);

        ItemModel->ItemID = item.ItemID;
        ItemModel->From_Ledger = item.DefaultPurchaseReturnLedgerID;
        ItemModel->To_Ledger = obj->ledgerObject.LedgerID;
        ItemModel->VoucherID = obj->voucherNumber;
        ItemModel->VoucherPrefix = obj->VoucherPrefix;
        ItemModel->VoucherDate = obj->VoucherDate;
        ItemModel->VoucherType = VoucherType;
//        ItemModel->From_Location = "Godown";
        ItemModel->From_Location = item.fromGodownID;
        ItemModel->To_Location = "OUT";
        ItemModel->crQty = item.quantity;
        ItemModel->drQty = 0;
        ItemModel->crAmount = item.subTotal;
        ItemModel->Price = item.price;
        ItemModel->itemReqId = item.ItemReqUuid;
        ItemModel->Narration = obj->narration;

        itemTransaction->insertItemTransaction(ItemModel, item.godownList);

        int basid = db->insertRecordsPlus(map1,Debit_Note_Details_TableName);

        for(inventoryItemDataModel subitem:compitem.subItems){
            QMap<QString,QVariant> map2;


            map2.insert(Debit_Note_Details_Voucher_No,obj->voucherNumber);
            map2.insert(Debit_Note_Details_Voucher_Prefix,obj->VoucherPrefix);
            map2.insert(Debit_Note_Details_Voucher_Date,obj->VoucherDate);
            map2.insert(Debit_Note_Details_Inventory_Item_ID,subitem.ItemID);
            map2.insert(Debit_Note_Details_Quantity,subitem.quantity);
            map2.insert(Debit_Note_Details_Price,subitem.price);
            map2.insert(Debit_Note_Details_PriceLevel,subitem.PriceLevel);
            map2.insert(Debit_Note_Details_isCompoundItem,subitem.IsCompoundItem);
            map2.insert(Debit_Note_Details_subItemOff_list_ID,basid);
            map2.insert(Debit_Note_Details_Discount_Percent,subitem.discountPercentage);
            map2.insert(Debit_Note_Details_Discount_Amount,subitem.discountinAmount);
            map2.insert(Debit_Note_Details_Subtotal,subitem.subTotal);
            map2.insert(Debit_Note_Details_Project_ID,obj->ProjectId);
            map2.insert(Debit_Note_Details_Item_Narration,subitem.narration);
            map2.insert(Debit_Note_Details_Item_Description,subitem.ItemDescription);
            map2.insert(Debit_Note_Details_Salesman_ID,obj->SalesmanID);
            map2.insert(Debit_Note_Details_Requirement_ItemID,subitem.ItemReqUuid);
            map2.insert(Debit_Note_Details_Dimension,subitem.Dimension);
            map2.insert(Debit_Note_Details_Length,subitem.length);
            map2.insert(Debit_Note_Details_UomId, subitem.uomObject._id);
            map2.insert(Debit_Note_Details_TaxRate, subitem.taxRate);
            map2.insert(Debit_Note_Details_TaxAmount, subitem.taxAmount);

            db->insertRecordsPlus(map2,Debit_Note_Details_TableName);
        }
    }

    qDebug()<<Q_FUNC_INFO<<"ledgers list"<<obj->ledgersList.size();

    LedgerMasterDataModel MainLedger;

    MainLedger.drAmount = obj->grandTotal;
    MainLedger.LedgerID = obj->ledgerObject.LedgerID;
    MainLedger.isInvoiceItem = true;
    MainLedger.mapList = obj->mapList;


    QString agCrID ;
    QString agDrID = obj->ledgerObject.LedgerID ;


    float  amt=0;
    for(int i=0;i<obj->ledgersList.size();i++)
    {
       obj->ledgersList[i].AgainstLedger = agDrID;
       if(obj->ledgersList[i].crAmount>amt)
           agCrID = obj->ledgersList[i].LedgerID;
    }

    if(!obj->paymentSplit)
        obj->ledgersList.insert(0,MainLedger);

    obj->voucherType = VoucherType;

    obj->ledgersList[0].AgainstLedger = "0x8x14";

    ledTransaction->deleteTransaction(obj);
    ledTransaction->insertTransaction(obj);

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
        notification->Action=1002;
        notification->GeneratedTransactionId=QString::number(QDateTime::currentDateTime().toTime_t()) + obj->VoucherPrefix;
        notification->GeneratedTransactionId = obj->TransactionId;

        sendChannelDatabaseHelper *sendHelper = new sendChannelDatabaseHelper();
        sendHelper->insertNotification(notification);
    }

    return c;

}

int DebitNoteVoucherDatabaseHelper::deleteVoucher(GeneralVoucherDataObject *obj)
{
    this->startTransaction();
    QMap<QString,QVariant> map3;
    map3.insert(Debit_Note_Main_Voucher_No,obj->voucherNumber);
    map3.insert(Debit_Note_Main_Voucher_Prefix, obj->VoucherPrefix);
    db->deleteRecordsPlus(map3,Debit_Note_Main_TableName);

    QMap<QString,QVariant> map4;
    map4.insert(Debit_Note_Details_Voucher_No,obj->voucherNumber);
    map4.insert(Debit_Note_Details_Voucher_Prefix, obj->VoucherPrefix);
    db->deleteRecordsPlus(map4,Debit_Note_Details_TableName);

    itemTransaction->deleteItemTransaction(obj);
    ledTransaction->deleteTransaction(obj);

    return commit();
}

int DebitNoteVoucherDatabaseHelper::commit()
{
    if(flag){
        if(db->commitTransaction())
            return true;
        else
            return false;
    }
    else{
        db->rollBackTransaction();
        flag=true;
        return 0;
    }
}

int DebitNoteVoucherDatabaseHelper::getMaxID()
{
    QString qStr = "SELECT MAX("+Debit_Note_Main_id+") FROM " + Debit_Note_Main_TableName;
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



GeneralVoucherDataObject *DebitNoteVoucherDatabaseHelper::getVoucherByVoucherNoPtr(QString vNo, QString voucherPrefix)
{
    qDebug()<<"256";
    GeneralVoucherDataObject *obj = new GeneralVoucherDataObject;
    SalesInventoryItemDatabaseHelper *itemHelper = new SalesInventoryItemDatabaseHelper();
    LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper();
    QString qStrMain ="SELECT "
            + Debit_Note_Main_Voucher_No + ", "
            + Debit_Note_Main_Voucher_Prefix + ", "
            + Debit_Note_Main_Date_Created + ", "
            + Debit_Note_Main_Voucher_Date + ", "
            + Debit_Note_Main_TimeStamp + ", "
            + Debit_Note_Main_Ledger_ID + ", "
            + Debit_Note_Main_Billing_Name + ", "
            + Debit_Note_Main_Total + ", "
            + Debit_Note_Main_Narration + ", "
            + Debit_Note_Main_Added_By + ", "
            + Debit_Note_Main_Delivery_Date + ", "
            + Debit_Note_Main_Completion_Probability + ", "
            + Debit_Note_Main_Credit_Period + ", "
            + Debit_Note_Main_POC_Name + ", "
            + Debit_Note_Main_POC_Phone + ", "
            + Debit_Note_Main_POC_Email + ", "
            + Debit_Note_Main_Project_ID + ", "
            + Debit_Note_Main_Revision_Number + ", "
            + Debit_Note_Main_Converted_To_Sales_Order + ", "
            + Debit_Note_Main_Quotation_Dropped + ", "
            + Debit_Note_Main_Quotation_Dropped_Reason + ", "
            + Debit_Note_Main_Requirement_Voucher_No + ", "
            + Debit_Note_Main_Salesman_ID + ", "
            + Debit_Note_Main_LPORef + ", "
            + Debit_Note_Main_VoucherStatus + ", "
            + Debit_Note_Main_ModeOfService + ", "
            + Debit_Note_Main_Discount_In_Amount + ", "
            + Debit_Note_Main_Discount_In_Percent + ", "
            + Debit_Note_Main_VATAmount + ", "
            + Debit_Note_Main_Contact_ID + ", "
            + Debit_Note_Main_Gross_Total + ", "
            + Debit_Note_Main_Reference + ", "
            + Debit_Note_Main_Customer_Expecting_Date + ", "
            + Debit_Note_Main_Currency + ", "
            + Debit_Note_Main_BoxesCount+ ", "
            + Debit_Note_Main_TotalWeight + ", "
            + Debit_Note_Main_LRNo + ", "
            + Debit_Note_Main_VoucherNoDisplay + ", "
            + Debit_Note_Main_ReqVoucherList + ", "
            + Debit_Note_Main_Origin + ", "
            + Debit_Note_Main_Generated_TransactionId +", "
            + Debit_Note_Main_Godown
            + " FROM " + Debit_Note_Main_TableName
            + " WHERE " + Debit_Note_Main_Voucher_No + "= :vno"
            + " AND " +  Debit_Note_Main_Voucher_Prefix + "= :voucherPrefix";

    QString qStrLedgers ="SELECT "
            + Transaction_Helper_id +", "
            + Transaction_Helper_VoucherNo +", "
            + Transaction_Helper_VoucherPrefix +", "
            + Transaction_Helper_ProjectID +", "
            + Transaction_Helper_TranactionAmount +", "
            + Transaction_Helper_CreditLedgerID +", "
            + Transaction_Helper_DebitLedgerID + ", "
            + Transaction_Helper_Narration +", "
            + Transaction_Helper_SalesManID +", "
            + Transaction_Helper_VoucherType + ", "
            + Transaction_Helper_isAdditional+ ", "
            + Transaction_Helper_Credit_Amount + ", "
            + Transaction_Helper_Debit_Amount + ", "
            + Transaction_Helper_LedgerID + " "

            + " FROM " + Transaction_Helper_TableName
            + " WHERE " + Transaction_Helper_VoucherNo + "=:id"
             + " AND " + Transaction_Helper_VoucherPrefix + "='"+voucherPrefix+"'"
            + " AND " + Transaction_Helper_VoucherType + "='"+VoucherType+"'";

    QString qStrDetail = "SELECT distinct " + Debit_Note_Details_Voucher_No
            + ", " + Debit_Note_Details_Voucher_Prefix
            + ", " + Debit_Note_Details_Voucher_Date
            + ", " + Debit_Note_Details_Inventory_Item_ID
            + ", " + Debit_Note_Details_Quantity
            + ", " + Debit_Note_Details_Price
            + ", " + Debit_Note_Details_UomId
            + ", subq.convalue , subq.UomName, subq.UomSymbol, subq.barCode "
            + ", " + Debit_Note_Details_PriceLevel
            + ", " + Debit_Note_Details_Discount_Percent
            + ", " + Debit_Note_Details_Discount_Amount
            + ", " + Debit_Note_Details_Subtotal
            + ", " + Debit_Note_Details_Item_Narration
            + ", " + Debit_Note_Details_Item_Description
            + ", " + Debit_Note_Details_Salesman_ID
            + ", " + Debit_Note_Details_Requirement_ItemID
            + ", " + Debit_Note_Details_Length
            + ", " + Debit_Note_Details_Dimension
            + ", " + Debit_Note_Details_itemVoucherStatus
            + ", " + Debit_Note_Details_itemProductionStatus
            + ", "  + Debit_Note_Details_TaxRate
            + ", " +  Debit_Note_Details_TaxAmount
            + ", " + Debit_Note_Details_Time
            + ", " + Debit_Note_Details_isCompoundItem
            + ", " + Debit_Note_Details_subItemOff_list_ID
            + " FROM " + Debit_Note_Details_TableName+ " s LEFT OUTER JOIN "
            + UomDataBaseHelper::qStrUomForItemDetail + " as subq "
            + " ON s." + Debit_Note_Details_Inventory_Item_ID + " = subq.itemID "
            + " AND s." + Debit_Note_Details_UomId + " = subq.UOM "
            + " WHERE " + Debit_Note_Details_Voucher_No + "= :vno"
            + " AND " +  Debit_Note_Details_Voucher_Prefix + "= :voucherPrefix"
            + " ORDER BY _id asc ";

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
            obj->voucherType = query.value(9).toString();
            ledger.isInvoiceItem = !query.value(10).toBool();
            ledger.crAmount = query.value(11).toFloat();
            ledger.drAmount = query.value(12).toFloat();
            //            qDebug()<<"at db helper Ledgers are : "<<ledger.LedgerName<<" : "<<ledger.drAmount<<ledger.crAmount<<ledger.isIndividual;
            if(!ledger.isInvoiceItem){
                //                qDebug()<<"Adding "<<ledger.LedgerName;
                obj->ledgersList.append(ledger);
            }
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
            item.uomObject.barCode = query.value(i++).toString();
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
                qDebug()<<itemSize;
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
            obj->TransactionId = query.value(i++).toString();
            obj->fromGodownID = query.value(i++).toString();


        }
    }
    obj->voucherType = VoucherType;
    return obj;
}



QSqlQueryModel *DebitNoteVoucherDatabaseHelper::getVoucherListByDateModel(QDate dateFrom, QDate dateTo)
{
    LedgerMasterDatabaseHelper ledHelper;
    QString qStr ="SELECT ";
    qStr += "'V' , ";
    qStr += Debit_Note_Main_Voucher_No +", ";
    qStr += "'" + VoucherType + "', " ;
    qStr += Debit_Note_Main_Voucher_Prefix + ", ";
    qStr += Debit_Note_Main_Voucher_Date+" as 'Voucher Date', ";
//    qStr += " CONCAT(" + Debit_Note_Main_Voucher_Prefix + ", '-', " + Debit_Note_Main_Voucher_No+ ") as 'Voucher No', ";
    qStr += Debit_Note_Main_Voucher_No +" as 'Voucher No', ";
    qStr += "(SELECT "+ledHelper.Ledger_Master_LedgerName + " FROM "+ ledHelper.Ledger_Master_TableName;
    qStr += " WHERE "+ledHelper.Ledger_Master_LedgerID + "= som.";
    qStr += Debit_Note_Main_Ledger_ID + ") Ledger, ";
    qStr += Debit_Note_Main_Total ;
    qStr += " FROM " + Debit_Note_Main_TableName;
    qStr += " som WHERE ";
    qStr += Debit_Note_Main_Voucher_Date;
    qStr += " >= '";
    qStr += dateFrom.toString("yyyy-MM-dd");
    qStr += "' AND ";
    qStr += Debit_Note_Main_Voucher_Date;
    qStr += " <= '";
    qStr += dateTo.toString("yyyy-MM-dd");
    qStr += "'";

    return DatabaseHelper::getQueryModel(qStr);
}

