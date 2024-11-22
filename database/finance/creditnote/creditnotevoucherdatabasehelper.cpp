#include "creditnotevoucherdatabasehelper.h"

QString CreditNoteVoucherDatabaseHelper::Credit_Note_Main_TableName = "Credit_Note_Main";
CreditNoteVoucherDatabaseHelper::CreditNoteVoucherDatabaseHelper(QString connString)
{
    db = new DatabaseHelper(connString);
    itemTransaction = new ItemTransactionDatabaseHelper(db);
    ItemModel = new ItemTransactionDataModel();
    ledTransaction = new TransactionHelper(db);
}

CreditNoteVoucherDatabaseHelper::~CreditNoteVoucherDatabaseHelper()
{

}

int CreditNoteVoucherDatabaseHelper::startTransaction()
{
    db->startTransaction();

}

QPair<QString, QString> CreditNoteVoucherDatabaseHelper::getVoucherNoByTransactionId(QString transId)
{
    GeneralVoucherDatabaseHelper vHelper;
    return vHelper.getVoucherNoByTransactionId(transId, Credit_Note_Main_TableName);


}

QString CreditNoteVoucherDatabaseHelper::getVoucherNumberByLedID(QString LedID, QDate Date)
{

}

QSqlQueryModel *CreditNoteVoucherDatabaseHelper::getVoucherListByDateTimeModel(QDateTime dateFrom, QDateTime dateTo)
{

}


int CreditNoteVoucherDatabaseHelper::insertVoucher(GeneralVoucherDataObject *obj)
{
    this->startTransaction();
    QMap<QString,QVariant> map;
    GeneralVoucherDatabaseHelper vHelper;
    obj->voucherNumber = vHelper.setNextVoucherNoByType(VoucherType, obj->VoucherPrefix, Credit_Note_Main_TableName );

    if(obj->TransactionId.length()==0)
        obj->TransactionId=QString::number(QDateTime::currentDateTime().toTime_t()) + obj->VoucherPrefix + db->clientId;

    map.insert(Credit_Note_Main_Voucher_No,obj->voucherNumber);
    map.insert(Credit_Note_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Credit_Note_Main_Date_Created,obj->VoucherDate);
    map.insert(Credit_Note_Main_Voucher_Date,obj->VoucherDate);
    map.insert(Credit_Note_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    map.insert(Credit_Note_Main_Billing_Name,obj->ledgerObject.LedgerName);
    map.insert(Credit_Note_Main_Total,obj->grandTotal);
    map.insert(Credit_Note_Main_Narration,obj->narration);
    map.insert(Credit_Note_Main_Added_By,obj->AddedBy);
    map.insert(Credit_Note_Main_Delivery_Date,obj->DeliveryDate);
    map.insert(Credit_Note_Main_Completion_Probability,obj->CompletionProbability);
    map.insert(Credit_Note_Main_Credit_Period,obj->ledgerObject.Credit_Period);
    map.insert(Credit_Note_Main_POC_Name,obj->ledgerObject.ContactPersonName);
    map.insert(Credit_Note_Main_POC_Phone,obj->ledgerObject.ContactPersonNumber);
    map.insert(Credit_Note_Main_POC_Email,obj->ledgerObject.ContactPersonEmail);
    map.insert(Credit_Note_Main_Project_ID,obj->ProjectId);
    map.insert(Credit_Note_Main_Revision_Number,obj->RevisionNo);
    map.insert(Credit_Note_Main_Converted_To_Sales_Order,obj->ConvertedToSalesOrder);
    map.insert(Credit_Note_Main_Quotation_Dropped,obj->QuotationDropped);
    map.insert(Credit_Note_Main_Quotation_Dropped_Reason,obj->QuotationDroppedReason);
    map.insert(Credit_Note_Main_Requirement_Voucher_No,obj->RequirementVoucherNo);
    map.insert(Credit_Note_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Credit_Note_Main_VoucherStatus,obj->status);
    map.insert(Credit_Note_Main_Generated_TransactionId, obj->TransactionId);
    map.insert(Credit_Note_Main_TimeStampC, obj->timestamp);
    map.insert(Credit_Note_Main_Last_Updated, QDateTime::currentDateTime());
    map.insert(Credit_Note_Main_Godown, obj->toGodownID);
    map.insert(Credit_Note_Main_Discount_In_Amount,obj->discountinAmount);
    map.insert(Credit_Note_Main_Discount_In_Percent,obj->discountPercent);
    QJsonDocument doc(obj->ReqVoucherList);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    map.insert(Credit_Note_Main_ReqVoucherList, strJson);


    db->insertRecordsPlus(map,Credit_Note_Main_TableName);

    for(CompoundItemDataObject compitem:obj->InventoryItems){
        QMap<QString,QVariant> map1;
        inventoryItemDataModel item = compitem.BaseItem;
        if(item.ItemReqUuid == "X")
            item.ItemReqUuid = db->getUUID();
        map1.insert(Credit_Note_Details_Voucher_No,obj->voucherNumber);
        map1.insert(Credit_Note_Details_Voucher_Prefix,obj->VoucherPrefix);
        map1.insert(Credit_Note_Details_Voucher_Date,obj->VoucherDate);
        map1.insert(Credit_Note_Details_Inventory_Item_ID,item.ItemID);
        map1.insert(Credit_Note_Details_Quantity,item.quantity);
        map1.insert(Credit_Note_Details_Price,item.price);
        map1.insert(Credit_Note_Details_PriceLevel,item.PriceLevel);
        map1.insert(Credit_Note_Details_isCompoundItem,item.IsCompoundItem);
        map1.insert(Credit_Note_Details_subItemOff_list_ID,0);
        map1.insert(Credit_Note_Details_Discount_Percent,item.discountPercentage);
        map1.insert(Credit_Note_Details_Discount_Amount,item.discountinAmount);
        map1.insert(Credit_Note_Details_Subtotal,item.subTotal);
        map1.insert(Credit_Note_Details_Project_ID,obj->ProjectId);
        map1.insert(Credit_Note_Details_Item_Narration,item.narration);
        map1.insert(Credit_Note_Details_Item_Description,item.ItemDescription);
        map1.insert(Credit_Note_Details_Salesman_ID,obj->SalesmanID);
        map1.insert(Credit_Note_Details_Requirement_ItemID,item.ItemReqUuid);
        map1.insert(Credit_Note_Details_Dimension,item.Dimension);
        map1.insert(Credit_Note_Details_Length,item.length);
        map1.insert(Credit_Note_Details_UomId, item.uomObject._id);
        map1.insert(Credit_Note_Details_TaxRate, item.taxRate);
        map1.insert(Credit_Note_Details_TaxAmount, item.taxAmount);

        ItemModel->ItemID = item.ItemID;
        ItemModel->From_Ledger = item.DefaultSalesReturnLedgerID;
        ItemModel->To_Ledger = obj->ledgerObject.LedgerID;
        ItemModel->VoucherID = obj->voucherNumber;
        ItemModel->VoucherPrefix = obj->VoucherPrefix;
        ItemModel->VoucherDate = obj->VoucherDate;
        ItemModel->VoucherType = VoucherType;
        ItemModel->From_Location = "OUT";
        //        ItemModel->To_Location = "Godown";
        ItemModel->To_Location = item.toGodownID;
        ItemModel->crQty = 0;
        ItemModel->drQty = item.quantity;
        ItemModel->drAmount = item.subTotal;
        ItemModel->Price = item.price;
        ItemModel->itemReqId = item.ItemReqUuid;
        ItemModel->Narration = obj->narration;
        ItemModel->consumed = item.consumedQty;

        itemTransaction->insertItemTransaction(ItemModel, item.godownList);

        int basid = db->insertRecordsPlus(map1,Credit_Note_Details_TableName);

        for(inventoryItemDataModel subitem:compitem.subItems){
            QMap<QString,QVariant> map2;
            if(subitem.ItemReqUuid == "X")
                subitem.ItemReqUuid = db->getUUID();
            map2.insert(Credit_Note_Details_Voucher_No,obj->voucherNumber);
            map2.insert(Credit_Note_Details_Voucher_Prefix,obj->VoucherPrefix);
            map2.insert(Credit_Note_Details_Voucher_Date,obj->VoucherDate);
            map2.insert(Credit_Note_Details_Inventory_Item_ID,subitem.ItemID);
            map2.insert(Credit_Note_Details_Quantity,subitem.quantity);
            map2.insert(Credit_Note_Details_Price,subitem.price);
            map2.insert(Credit_Note_Details_PriceLevel,subitem.PriceLevel);
            map2.insert(Credit_Note_Details_isCompoundItem,subitem.IsCompoundItem);
            map2.insert(Credit_Note_Details_subItemOff_list_ID,basid);
            map2.insert(Credit_Note_Details_Discount_Percent,subitem.discountPercentage);
            map2.insert(Credit_Note_Details_Discount_Amount,subitem.discountinAmount);
            map2.insert(Credit_Note_Details_Subtotal,subitem.subTotal);
            map2.insert(Credit_Note_Details_Project_ID,obj->ProjectId);
            map2.insert(Credit_Note_Details_Item_Narration,subitem.narration);
            map2.insert(Credit_Note_Details_Item_Description,subitem.ItemDescription);
            map2.insert(Credit_Note_Details_Salesman_ID,obj->SalesmanID);
            map2.insert(Credit_Note_Details_Requirement_ItemID,subitem.ItemReqUuid);
            map2.insert(Credit_Note_Details_Dimension,subitem.Dimension);
            map2.insert(Credit_Note_Details_Length,subitem.length);
            map2.insert(Credit_Note_Details_UomId, item.uomObject._id);
            map2.insert(Credit_Note_Details_TaxRate, item.taxRate);
            map2.insert(Credit_Note_Details_TaxAmount, item.taxAmount);
            db->insertRecordsPlus(map2,Credit_Note_Details_TableName);
        }
    }

    LedgerMasterDataModel MainLedger;

    MainLedger.crAmount = obj->grandTotal;
    MainLedger.LedgerID = obj->ledgerObject.LedgerID;
    MainLedger.isInvoiceItem = true;
    MainLedger.mapList = obj->mapList;
    QString agDrID ,agCrID = obj->ledgerObject.LedgerID ;
    float  amt=0;
    for(int i=0;i<obj->ledgersList.size();i++)
    {
        obj->ledgersList[i].AgainstLedger = agCrID;
        if(obj->ledgersList[i].drAmount>amt)
            agDrID = obj->ledgersList[i].LedgerID;
        qDebug()<<"********************** agDrID"<<agDrID;
    }


    obj->ledgersList.insert(0,MainLedger);

    obj->voucherType = VoucherType;

    obj->ledgersList[0].AgainstLedger = "0x7x13";


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

int CreditNoteVoucherDatabaseHelper::updateVoucher(GeneralVoucherDataObject *obj, QString oldID)
{
    this->startTransaction();

    QMap<QString,QVariant> map;
    map.insert(Credit_Note_Main_Voucher_No,obj->voucherNumber);
    map.insert(Credit_Note_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Credit_Note_Main_Date_Created,obj->VoucherDate);
    map.insert(Credit_Note_Main_Voucher_Date,obj->VoucherDate);
    map.insert(Credit_Note_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    map.insert(Credit_Note_Main_Billing_Name,obj->ledgerObject.LedgerName);
    map.insert(Credit_Note_Main_Total,obj->grandTotal);
    map.insert(Credit_Note_Main_Narration,obj->narration);
    map.insert(Credit_Note_Main_Added_By,obj->AddedBy);
    map.insert(Credit_Note_Main_Delivery_Date,obj->DeliveryDate);
    map.insert(Credit_Note_Main_Completion_Probability,obj->CompletionProbability);
    map.insert(Credit_Note_Main_Credit_Period,obj->ledgerObject.Credit_Period);
    map.insert(Credit_Note_Main_POC_Name,obj->ledgerObject.ContactPersonName);
    map.insert(Credit_Note_Main_POC_Phone,obj->ledgerObject.ContactPersonNumber);
    map.insert(Credit_Note_Main_POC_Email,obj->ledgerObject.ContactPersonEmail);
    map.insert(Credit_Note_Main_Project_ID,obj->ProjectId);
    map.insert(Credit_Note_Main_Revision_Number,obj->RevisionNo);
    map.insert(Credit_Note_Main_Converted_To_Sales_Order,obj->ConvertedToSalesOrder);
    map.insert(Credit_Note_Main_Quotation_Dropped,obj->QuotationDropped);
    map.insert(Credit_Note_Main_Quotation_Dropped_Reason,obj->QuotationDroppedReason);
    map.insert(Credit_Note_Main_Requirement_Voucher_No,obj->RequirementVoucherNo);
    map.insert(Credit_Note_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Credit_Note_Main_Generated_TransactionId, obj->TransactionId);
    map.insert(Credit_Note_Main_TimeStamp,QDateTime::currentDateTime());
    map.insert(Credit_Note_Main_Last_Updated,QDateTime::currentDateTime());
    map.insert(Credit_Note_Main_Godown, obj->toGodownID);
    map.insert(Credit_Note_Main_VoucherStatus,obj->status);
    map.insert(Credit_Note_Main_Discount_In_Amount,obj->discountinAmount);
    map.insert(Credit_Note_Main_Discount_In_Percent,obj->discountPercent);

    QJsonDocument doc(obj->ReqVoucherList);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    map.insert(Credit_Note_Main_ReqVoucherList, strJson);

    QMap<QString,QVariant> map3;
    map3.insert(Credit_Note_Main_Voucher_No,oldID);
    map3.insert(Credit_Note_Main_Voucher_Prefix, obj->VoucherPrefix);
    db->updateRecordsPlus(map,map3,Credit_Note_Main_TableName);

    QMap<QString,QVariant> maptxndel;
    maptxndel.insert(Transaction_Helper_VoucherNo,oldID);
    maptxndel.insert(Transaction_Helper_VoucherType,VoucherType);
    //    db->deleteRecordsPlus(maptxndel,Transaction_Helper_TableName);

    QMap<QString,QVariant> map6;
    map6.insert(Credit_Note_Details_Voucher_No,oldID);
    map6.insert(Credit_Note_Details_Voucher_Prefix, obj->VoucherPrefix);

    db->deleteRecordsPlus(map6,Credit_Note_Details_TableName);

    itemTransaction->deleteItemTransaction(obj);

    for(CompoundItemDataObject compitem:obj->InventoryItems){
        QMap<QString,QVariant> map1;
        inventoryItemDataModel item = compitem.BaseItem;
        if(item.ItemReqUuid == "X")
            item.ItemReqUuid = db->getUUID();
        map1.insert(Credit_Note_Details_Voucher_No,obj->voucherNumber);
        map1.insert(Credit_Note_Details_Voucher_Prefix,obj->VoucherPrefix);
        map1.insert(Credit_Note_Details_Voucher_Date,obj->VoucherDate);
        map1.insert(Credit_Note_Details_Inventory_Item_ID,item.ItemID);
        map1.insert(Credit_Note_Details_Quantity,item.quantity);
        map1.insert(Credit_Note_Details_Price,item.price);
        map1.insert(Credit_Note_Details_PriceLevel,item.PriceLevel);
        map1.insert(Credit_Note_Details_isCompoundItem,item.IsCompoundItem);
        map1.insert(Credit_Note_Details_subItemOff_list_ID,0);
        map1.insert(Credit_Note_Details_Discount_Percent,item.discountPercentage);
        map1.insert(Credit_Note_Details_Discount_Amount,item.discountinAmount);
        map1.insert(Credit_Note_Details_Subtotal,item.subTotal);
        map1.insert(Credit_Note_Details_Project_ID,obj->ProjectId);
        map1.insert(Credit_Note_Details_Item_Narration,item.narration);
        map1.insert(Credit_Note_Details_Item_Description,item.ItemDescription);
        map1.insert(Credit_Note_Details_Salesman_ID,obj->SalesmanID);
        map1.insert(Credit_Note_Details_Requirement_ItemID,item.ItemReqUuid);
        map1.insert(Credit_Note_Details_Dimension,item.Dimension);
        map1.insert(Credit_Note_Details_Length,item.length);
        map1.insert(Credit_Note_Details_UomId, item.uomObject._id);
        map1.insert(Credit_Note_Details_TaxRate, item.taxRate);
        map1.insert(Credit_Note_Details_TaxAmount, item.taxAmount);

        ItemModel->ItemID = item.ItemID;
        ItemModel->From_Ledger = item.DefaultSalesReturnLedgerID;
        ItemModel->To_Ledger = obj->ledgerObject.LedgerID;
        ItemModel->VoucherID = obj->voucherNumber;
        ItemModel->VoucherPrefix = obj->VoucherPrefix;
        ItemModel->VoucherDate = obj->VoucherDate;
        ItemModel->VoucherType = VoucherType;
        ItemModel->From_Location = "OUT";
        ItemModel->To_Location = item.toGodownID;
        ItemModel->crQty = 0;
        ItemModel->drQty = item.quantity;
        ItemModel->drAmount = item.subTotal;
        ItemModel->Price = item.price;
        ItemModel->itemReqId = item.ItemReqUuid;
        ItemModel->Narration = obj->narration;
        ItemModel->consumed = item.consumedQty;
        itemTransaction->insertItemTransaction(ItemModel, item.godownList);
        int basid = db->insertRecordsPlus(map1,Credit_Note_Details_TableName);
        for(inventoryItemDataModel subitem:compitem.subItems){
            QMap<QString,QVariant> map2;
            map2.insert(Credit_Note_Details_Voucher_No,obj->voucherNumber);
            map2.insert(Credit_Note_Details_Voucher_Prefix,obj->VoucherPrefix);
            map2.insert(Credit_Note_Details_Voucher_Date,obj->VoucherDate);
            map2.insert(Credit_Note_Details_Inventory_Item_ID,subitem.ItemID);
            map2.insert(Credit_Note_Details_Quantity,subitem.quantity);
            map2.insert(Credit_Note_Details_Price,subitem.price);
            map2.insert(Credit_Note_Details_PriceLevel,subitem.PriceLevel);
            map2.insert(Credit_Note_Details_isCompoundItem,subitem.IsCompoundItem);
            map2.insert(Credit_Note_Details_subItemOff_list_ID,basid);
            map2.insert(Credit_Note_Details_Discount_Percent,subitem.discountPercentage);
            map2.insert(Credit_Note_Details_Discount_Amount,subitem.discountinAmount);
            map2.insert(Credit_Note_Details_Subtotal,subitem.subTotal);
            map2.insert(Credit_Note_Details_Project_ID,obj->ProjectId);
            map2.insert(Credit_Note_Details_Item_Narration,subitem.narration);
            map2.insert(Credit_Note_Details_Item_Description,subitem.ItemDescription);
            map2.insert(Credit_Note_Details_Salesman_ID,obj->SalesmanID);
            map2.insert(Credit_Note_Details_Requirement_ItemID,subitem.ItemReqUuid);
            map2.insert(Credit_Note_Details_Dimension,subitem.Dimension);
            map2.insert(Credit_Note_Details_Length,subitem.length);
            map2.insert(Credit_Note_Details_UomId, item.uomObject._id);
            map2.insert(Credit_Note_Details_TaxRate, item.taxRate);
            map2.insert(Credit_Note_Details_TaxAmount, item.taxAmount);

            db->insertRecordsPlus(map2,Credit_Note_Details_TableName);
        }
    }

    LedgerMasterDataModel MainLedger;
    MainLedger.crAmount = obj->grandTotal;
    MainLedger.LedgerID = obj->ledgerObject.LedgerID;
    MainLedger.isInvoiceItem = true;
    MainLedger.mapList = obj->mapList;
    QString agDrID ,agCrID = obj->ledgerObject.LedgerID ;
    float  amt=0;
    for(int i=0;i<obj->ledgersList.size();i++)
    {
        obj->ledgersList[i].AgainstLedger = agCrID;
        if(obj->ledgersList[i].drAmount>amt)
            agDrID = obj->ledgersList[i].LedgerID;
    }

    obj->ledgersList.insert(0,MainLedger);

    obj->voucherType = VoucherType;

    obj->ledgersList[0].AgainstLedger = "0x7x13";


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
        //        notification->GeneratedTransactionId=QString::number(QDateTime::currentDateTime().toTime_t()) + obj->VoucherPrefix;
        notification->GeneratedTransactionId = obj->TransactionId;

        sendChannelDatabaseHelper *sendHelper = new sendChannelDatabaseHelper();
        sendHelper->insertNotification(notification);
    }

    return c;

}

int CreditNoteVoucherDatabaseHelper::deleteVoucher(GeneralVoucherDataObject *obj)
{
    this->startTransaction();
    QMap<QString,QVariant> map3;
    map3.insert(Credit_Note_Main_Voucher_No,obj->voucherNumber);
    map3.insert(Credit_Note_Main_Voucher_Prefix, obj->VoucherPrefix);
    db->deleteRecordsPlus(map3,Credit_Note_Main_TableName);

    QMap<QString,QVariant> map4;
    map4.insert(Credit_Note_Details_Voucher_No,obj->voucherNumber);
    map4.insert(Credit_Note_Details_Voucher_Prefix, obj->VoucherPrefix);
    db->deleteRecordsPlus(map4,Credit_Note_Details_TableName);

    itemTransaction->deleteItemTransaction(obj);
    ledTransaction->deleteTransaction(obj);

    return commit();
}

int CreditNoteVoucherDatabaseHelper::commit()
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

int CreditNoteVoucherDatabaseHelper::getMaxID()
{
    QString qStr = "SELECT MAX("+Credit_Note_Main_id+") FROM " + Credit_Note_Main_TableName;
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

QString CreditNoteVoucherDatabaseHelper::getNextVoucherNo(QString voucherPrefix)
{
    GeneralVoucherDatabaseHelper vHelper;
    QString voucherNumber = vHelper.getNextVoucherNoByType(VoucherType, voucherPrefix, Credit_Note_Main_TableName);
    return voucherNumber;

}



GeneralVoucherDataObject* CreditNoteVoucherDatabaseHelper::getVoucherByVoucherNoPtr(QString vNo, QString voucherPrefix)
{
    GeneralVoucherDataObject* obj = new GeneralVoucherDataObject;
    SalesInventoryItemDatabaseHelper *itemHelper = new SalesInventoryItemDatabaseHelper();
    LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper();

    QString qStrMain ="SELECT "
            + Credit_Note_Main_Voucher_No + ", "
            + Credit_Note_Main_Voucher_Prefix + ", "
            + Credit_Note_Main_Date_Created + ", "
            + Credit_Note_Main_Voucher_Date + ", "
            + Credit_Note_Main_TimeStamp + ", "
            + Credit_Note_Main_Ledger_ID + ", "
            + Credit_Note_Main_Billing_Name + ", "
            + Credit_Note_Main_Total + ", "
            + Credit_Note_Main_Narration + ", "
            + Credit_Note_Main_Added_By + ", "
            + Credit_Note_Main_Delivery_Date + ", "
            + Credit_Note_Main_Completion_Probability + ", "
            + Credit_Note_Main_Credit_Period + ", "
            + Credit_Note_Main_POC_Name + ", "
            + Credit_Note_Main_POC_Phone + ", "
            + Credit_Note_Main_POC_Email + ", "
            + Credit_Note_Main_Project_ID + ", "
            + Credit_Note_Main_Revision_Number + ", "
            + Credit_Note_Main_Converted_To_Sales_Order + ", "
            + Credit_Note_Main_Quotation_Dropped + ", "
            + Credit_Note_Main_Quotation_Dropped_Reason + ", "
            + Credit_Note_Main_Requirement_Voucher_No + ", "
            + Credit_Note_Main_Salesman_ID + ", "
            + Credit_Note_Main_LPORef + ", "
            + Credit_Note_Main_VoucherStatus + ", "
            + Credit_Note_Main_ModeOfService + ", "
            + Credit_Note_Main_Discount_In_Amount + ", "
            + Credit_Note_Main_Discount_In_Percent + ", "
            + Credit_Note_Main_VATAmount + ", "
            + Credit_Note_Main_Contact_ID + ", "
            + Credit_Note_Main_Gross_Total + ", "
            + Credit_Note_Main_Reference + ", "
            + Credit_Note_Main_Customer_Expecting_Date + ", "
            + Credit_Note_Main_Currency + ", "
            + Credit_Note_Main_BoxesCount+ ", "
            + Credit_Note_Main_TotalWeight + ", "
            + Credit_Note_Main_LRNo + ", "
            + Credit_Note_Main_VoucherNoDisplay + ", "
            + Credit_Note_Main_ReqVoucherList + ", "
            + Credit_Note_Main_Origin + ", "
            + Credit_Note_Main_Generated_TransactionId+", "
            + Credit_Note_Main_Godown +" "
            + " FROM " + Credit_Note_Main_TableName
            + " WHERE " + Credit_Note_Main_Voucher_No + "= :vno"
            + " AND " +  Credit_Note_Main_Voucher_Prefix + "= :voucherPrefix";

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

    QString qStrDetail = "SELECT distinct " + Credit_Note_Details_Voucher_No
            + ", " + Credit_Note_Details_Voucher_Prefix
            + ", " + Credit_Note_Details_Voucher_Date
            + ", " + Credit_Note_Details_Inventory_Item_ID
            + ", " + Credit_Note_Details_Quantity
            + ", " + Credit_Note_Details_Price
            + ", " + Credit_Note_Details_UomId
            + ", subq.convalue , subq.UomName, subq.UomSymbol, subq.barCode "
            + ", " + Credit_Note_Details_PriceLevel
            + ", " + Credit_Note_Details_Discount_Percent
            + ", " + Credit_Note_Details_Discount_Amount
            + ", " + Credit_Note_Details_Subtotal
            + ", " + Credit_Note_Details_Item_Narration
            + ", " + Credit_Note_Details_Item_Description
            + ", " + Credit_Note_Details_Salesman_ID
            + ", " + Credit_Note_Details_Requirement_ItemID
            + ", " + Credit_Note_Details_Length
            + ", " + Credit_Note_Details_Dimension
            + ", " + Credit_Note_Details_itemVoucherStatus
            + ", " + Credit_Note_Details_itemProductionStatus
            + ", "  + Credit_Note_Details_TaxRate
            + ", " + Credit_Note_Details_TaxAmount
            + ", " + Credit_Note_Details_Time
            + ", " + Credit_Note_Details_isCompoundItem
            + ", " + Credit_Note_Details_subItemOff_list_ID
            + " FROM " + Credit_Note_Details_TableName+ " s LEFT OUTER JOIN "
            + UomDataBaseHelper::qStrUomForItemDetail + " as subq "
            + " ON s." + Credit_Note_Details_Inventory_Item_ID + " = subq.itemID "
            + " AND s." + Credit_Note_Details_UomId + " = subq.UOM "
            + " WHERE " + Credit_Note_Details_Voucher_No + "= :vno"
            + " AND " +  Credit_Note_Details_Voucher_Prefix + "= :voucherPrefix"
            + " ORDER BY _id asc ";

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
            ledger = ledHelper->getLedgerObjectByID(query.value(13).toString());
            ledger.amount = query.value(4).toFloat();
            obj->voucherType = query.value(9).toString();
            ledger.isInvoiceItem = !query.value(10).toBool();
            ledger.crAmount = query.value(11).toFloat();
            ledger.drAmount = query.value(12).toFloat();
            if(!ledger.isInvoiceItem)
                obj->ledgersList.append(ledger);
        }
    }
    qDebug()<<Q_FUNC_INFO<<obj->ledgersList.size();
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

            ItemTransactionDatabaseHelper transHelper;
            item.consumedQty = transHelper.getConsumedQtyByVoucher(item.ItemID, vNo, VoucherType, voucherPrefix, item.ItemReqUuid);

            i++;
            if(query.value(i++).toBool()||(query.value(i++).toInt()==0)){

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
            }

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
            obj->toGodownID = query.value(i++).toString();

        }
    }
    obj->voucherType = VoucherType;

    qDebug()<<"end of func"<<obj->ledgersList.size();
    return obj;
}


QSqlQueryModel *CreditNoteVoucherDatabaseHelper::getVoucherListByDateModel(QDate dateFrom, QDate dateTo)
{
    LedgerMasterDatabaseHelper ledHelper;
    QString qStr ="SELECT ";
    qStr += "'V' , ";
    qStr += Credit_Note_Main_Voucher_No +", ";
    qStr += "'" + VoucherType + "', " ;
    qStr += Credit_Note_Main_Voucher_Prefix + ", ";
    qStr += Credit_Note_Main_Voucher_Date+" as 'Voucher Date', ";
    qStr += " CONCAT(" + Credit_Note_Main_Voucher_Prefix + ", '-', " + Credit_Note_Main_Voucher_No+ ") as 'Voucher No', ";
    qStr += "(SELECT "+ledHelper.Ledger_Master_LedgerName + " FROM "+ ledHelper.Ledger_Master_TableName;
    qStr += " WHERE "+ledHelper.Ledger_Master_LedgerID + "= som.";
    qStr += Credit_Note_Main_Ledger_ID + ") Ledger, ";
    qStr += Credit_Note_Main_Total ;
    qStr += " FROM " + Credit_Note_Main_TableName;
    qStr += " som WHERE ";
    qStr += Credit_Note_Main_Voucher_Date;
    qStr += " >= '";
    qStr += dateFrom.toString("yyyy-MM-dd");
    qStr += "' AND ";
    qStr += Credit_Note_Main_Voucher_Date;
    qStr += " <= '";
    qStr += dateTo.toString("yyyy-MM-dd");
    qStr += "'";

    return DatabaseHelper::getQueryModel(qStr);
}
