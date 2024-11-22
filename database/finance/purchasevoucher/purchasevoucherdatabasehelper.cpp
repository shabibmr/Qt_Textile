#include "purchasevoucherdatabasehelper.h"

QString PurchaseVoucherDatabaseHelper::Purchase_Invoice_Main_TableName = "Purchase_Invoice_Main";

PurchaseVoucherDatabaseHelper::PurchaseVoucherDatabaseHelper(QString connString)
{
    db = new DatabaseHelper(connString);
    itemTransaction = new ItemTransactionDatabaseHelper(db);
    ItemModel = new ItemTransactionDataModel();
    ledTransaction = new TransactionHelper(db);
}

PurchaseVoucherDatabaseHelper::~PurchaseVoucherDatabaseHelper()
{

}

int PurchaseVoucherDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

QString PurchaseVoucherDatabaseHelper::getNextVoucherNo(QString voucherPrefix)
{
    GeneralVoucherDatabaseHelper vHelper;
    QString voucherNumber = vHelper.getNextVoucherNoByType(VoucherType, voucherPrefix, Purchase_Invoice_Main_TableName);
    return voucherNumber;

}

QPair<QString, QString> PurchaseVoucherDatabaseHelper::getVoucherNoByTransactionId(QString transId)
{
    GeneralVoucherDatabaseHelper vHelper;
    return vHelper.getVoucherNoByTransactionId(transId, Purchase_Invoice_Main_TableName);
}

QString PurchaseVoucherDatabaseHelper::getVoucherNumberByLedID(QString LedID, QDate Date)
{

}

QSqlQueryModel *PurchaseVoucherDatabaseHelper::getVoucherListByDateTimeModel(QDateTime dateFrom, QDateTime dateTo)
{

}

int PurchaseVoucherDatabaseHelper::insertVoucher(GeneralVoucherDataObject *obj)
{
    this->startTransaction();

    QMap<QString,QVariant> map;
    GeneralVoucherDatabaseHelper vHelper;
    obj->voucherNumber = vHelper.setNextVoucherNoByType(VoucherType, obj->VoucherPrefix, Purchase_Invoice_Main_TableName );
    //    if(obj->ledgerObject.dbName != "" && !obj->fromExternal && obj->sendFlag)
    if(obj->TransactionId.length()==0)
        obj->TransactionId=QString::number(QDateTime::currentDateTime().toTime_t()) + obj->VoucherPrefix + db->clientId;

    map.insert(Purchase_Invoice_Main_Voucher_No,obj->voucherNumber);
    map.insert(Purchase_Invoice_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Purchase_Invoice_Main_Date_Created,obj->VoucherDate);
    map.insert(Purchase_Invoice_Main_Voucher_Date,obj->VoucherDate);
    map.insert(Purchase_Invoice_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    map.insert(Purchase_Invoice_Main_Billing_Name,obj->BillingName);
    map.insert(Purchase_Invoice_Main_Total,obj->grandTotal);
    map.insert(Purchase_Invoice_Main_Narration,obj->narration);
    map.insert(Purchase_Invoice_Main_Added_By,obj->AddedBy);
    map.insert(Purchase_Invoice_Main_Delivery_Date,obj->DeliveryDate);
    map.insert(Purchase_Invoice_Main_Completion_Probability,obj->CompletionProbability);
    map.insert(Purchase_Invoice_Main_Credit_Period,obj->ledgerObject.Credit_Period);
    map.insert(Purchase_Invoice_Main_POC_Name,obj->ledgerObject.ContactPersonName);
    map.insert(Purchase_Invoice_Main_POC_Phone,obj->ledgerObject.ContactPersonNumber);
    map.insert(Purchase_Invoice_Main_POC_Email,obj->ledgerObject.ContactPersonEmail);
    map.insert(Purchase_Invoice_Main_Project_ID,obj->ProjectId);
    map.insert(Purchase_Invoice_Main_Revision_Number,obj->RevisionNo);
    map.insert(Purchase_Invoice_Main_Converted_To_Sales_Order,obj->ConvertedToSalesOrder);
    map.insert(Purchase_Invoice_Main_Quotation_Dropped,obj->QuotationDropped);
    map.insert(Purchase_Invoice_Main_Quotation_Dropped_Reason,obj->QuotationDroppedReason);
    map.insert(Purchase_Invoice_Main_Requirement_Voucher_No,obj->RequirementVoucherNo);
    map.insert(Purchase_Invoice_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Purchase_Invoice_Main_Discount_In_Amount,obj->discountinAmount);
    map.insert(Purchase_Invoice_Main_Discount_In_Percent,obj->discountPercent);
    map.insert(Purchase_Invoice_Main_Gross_Total,obj->grossTotal);
    map.insert(Purchase_Invoice_Main_VATAmount,obj->taxTotalAmount);
    map.insert(Purchase_Invoice_Main_GeneratedTransactionId, obj->TransactionId);
    map.insert(Purchase_Invoice_Main_InvoiceNo, obj->invoiceNumber);
    qDebug()<<Q_FUNC_INFO<<__LINE__<<obj->invoiceNumber;
    map.insert(Purchase_Invoice_Main_InvoiceDate, obj->invoiceDate);
    map.insert(Purchase_Invoice_Main_TimeStampC,obj->timestamp);
    map.insert(Purchase_Invoice_Main_Last_Updated,QDateTime::currentDateTime());
    map.insert(Purchase_Invoice_Main_Godown,obj->toGodownID);
    QJsonDocument doc(obj->ReqVoucherList);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    map.insert(Purchase_Invoice_Main_ReqVoucherList, strJson);
    qint64 lastId= db->insertRecordsPlus(map,Purchase_Invoice_Main_TableName);

    //    QMap<QString,QVariant> idmap;
    //    idmap.insert(Purchase_Invoice_Main_Voucher_No,QString::number(lastId));
    //    QMap<QString,QVariant> idmapClause;
    //    idmapClause.insert(Purchase_Invoice_Main_id,lastId);
    //    db->updateRecordsPlus(idmap,idmapClause,Purchase_Invoice_Main_TableName);
    //    obj->voucherNumber = QString::number(lastId);

    for(CompoundItemDataObject compitem:obj->InventoryItems){
        QMap<QString,QVariant> map1;
        inventoryItemDataModel item = compitem.BaseItem;
        if(item.ItemReqUuid == "X"){
            item.ItemReqUuid = db->getUUID();
        }
        map1.insert(Purchase_Invoice_Details_Requirement_ItemID, item.ItemReqUuid);
        map1.insert(Purchase_Invoice_Details_Voucher_No,obj->voucherNumber);
        map1.insert(Purchase_Invoice_Details_Voucher_Prefix,obj->VoucherPrefix);
        map1.insert(Purchase_Invoice_Details_Voucher_Date,obj->VoucherDate.toString("yyyy-MM-dd"));
        map1.insert(Purchase_Invoice_Details_Inventory_Item_ID,item.ItemID);
        map1.insert(Purchase_Invoice_Details_Quantity,item.quantity);
        map1.insert(Purchase_Invoice_Details_Price,item.price);
        map1.insert(Purchase_Invoice_Details_PriceLevel,item.PriceLevel);
        map1.insert(Purchase_Invoice_Details_isCompoundItem,item.IsCompoundItem);
        map1.insert(Purchase_Invoice_Details_subItemOff_list_ID,0);
        map1.insert(Purchase_Invoice_Details_Discount_Percent,item.discountPercentage);
        map1.insert(Purchase_Invoice_Details_Discount_Amount,item.discountinAmount);
        map1.insert(Purchase_Invoice_Details_Subtotal,item.subTotal);
        map1.insert(Purchase_Invoice_Details_Project_ID,obj->ProjectId);
        map1.insert(Purchase_Invoice_Details_Item_Narration,item.narration);
        map1.insert(Purchase_Invoice_Details_Item_Description,item.ItemDescription);
        map1.insert(Purchase_Invoice_Details_Salesman_ID,obj->SalesmanID);
        //        map1.insert(Purchase_Invoice_Details_Requirement_ItemID,item.requirementItemIdOld);
        map1.insert(Purchase_Invoice_Details_Requirement_ItemID, item.ItemReqUuid);
        map1.insert(Purchase_Invoice_Details_Dimension,item.Dimension);
        map1.insert(Purchase_Invoice_Details_Length,item.length);
        map1.insert(Purchase_Invoice_Details_TaxRate, item.taxRate);
        map1.insert(Purchase_Invoice_Details_TaxAmount, item.taxAmount);
        map1.insert(Purchase_Invoice_Details_UomID, item.uomObject._id);
        map1.insert(Purchase_Invoice_Details_Time, QTime::currentTime());


        ItemModel->ItemID = item.ItemID;
        ItemModel->From_Ledger = obj->ledgerObject.LedgerID;
        ItemModel->To_Ledger = "";
        ItemModel->VoucherID = obj->voucherNumber;
        ItemModel->VoucherPrefix = obj->VoucherPrefix;
        ItemModel->VoucherDate = obj->VoucherDate;
        ItemModel->VoucherType = VoucherType;
        ItemModel->From_Location = "OUT";
        //        ItemModel->To_Location = "Godown";
        ItemModel->To_Location = item.toGodownID;
        ItemModel->crQty = 0;
        ItemModel->drQty = item.quantity;
        ItemModel->Price = item.price;
        ItemModel->drAmount = item.subTotal;
        ItemModel->itemReqId = item.ItemReqUuid;
        ItemModel->Narration = obj->narration;
        ItemModel->consumed = item.consumedQty;


        itemTransaction->insertItemTransaction(ItemModel, item.godownList);

        int basid = db->insertRecordsPlus(map1,Purchase_Invoice_Details_TableName);

        for(inventoryItemDataModel subitem:compitem.subItems){
            QMap<QString,QVariant> map2;
            map2.insert(Purchase_Invoice_Details_Voucher_No,obj->voucherNumber);
            map2.insert(Purchase_Invoice_Details_Voucher_Prefix,obj->VoucherPrefix);
            map2.insert(Purchase_Invoice_Details_Voucher_Date,obj->VoucherDate.toString("yyyy-MM-dd"));
            map2.insert(Purchase_Invoice_Details_Inventory_Item_ID,subitem.ItemID);
            map2.insert(Purchase_Invoice_Details_Quantity,subitem.quantity*subitem.uomObject.convRate);
            map2.insert(Purchase_Invoice_Details_Price,subitem.price);
            map2.insert(Purchase_Invoice_Details_PriceLevel,subitem.PriceLevel);
            map2.insert(Purchase_Invoice_Details_isCompoundItem,subitem.IsCompoundItem);
            map2.insert(Purchase_Invoice_Details_subItemOff_list_ID,basid);
            map2.insert(Purchase_Invoice_Details_Discount_Percent,subitem.discountPercentage);
            map2.insert(Purchase_Invoice_Details_Discount_Amount,subitem.discountinAmount);
            map2.insert(Purchase_Invoice_Details_Subtotal,subitem.subTotal);
            map2.insert(Purchase_Invoice_Details_Project_ID,obj->ProjectId);
            map2.insert(Purchase_Invoice_Details_Item_Narration,subitem.narration);
            map2.insert(Purchase_Invoice_Details_Item_Description,subitem.ItemDescription);
            map2.insert(Purchase_Invoice_Details_Salesman_ID,obj->SalesmanID);
            //            map2.insert(Purchase_Invoice_Details_Requirement_ItemID,subitem.requirementItemIdOld);
            map2.insert(Purchase_Invoice_Details_Requirement_ItemID, subitem.ItemReqUuid);
            map2.insert(Purchase_Invoice_Details_Dimension,item.Dimension);
            map2.insert(Purchase_Invoice_Details_Length,item.length);
            map2.insert(Purchase_Invoice_Details_UomID, item.uomObject._id);
            map2.insert(Purchase_Invoice_Details_TaxRate, item.taxRate);
            map2.insert(Purchase_Invoice_Details_TaxAmount, item.taxAmount);
            db->insertRecordsPlus(map2,Purchase_Invoice_Details_TableName);
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

    if(!obj->paymentSplit)
        obj->ledgersList.insert(0,MainLedger);

    obj->voucherType = VoucherType;

    obj->ledgersList[0].AgainstLedger = "0x8x8";

    ledTransaction->insertTransaction(obj);
    int c=commit();

    if( c && obj->ledgerObject.dbName != "" && !obj->fromExternal && obj->sendFlag){
        qDebug()<<"SEND*************";
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

int PurchaseVoucherDatabaseHelper::updateVoucher(GeneralVoucherDataObject *obj, QString oldID)
{
    this->startTransaction();

    QMap<QString,QVariant> map;
    //    map.insert(Purchase_Invoice_Main_Voucher_No,obj->voucherNumber);
    map.insert(Purchase_Invoice_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Purchase_Invoice_Main_Date_Created,obj->VoucherDate.toString("yyyy-MM-dd"));
    map.insert(Purchase_Invoice_Main_Voucher_Date,obj->VoucherDate.toString("yyyy-MM-dd"));
    map.insert(Purchase_Invoice_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    map.insert(Purchase_Invoice_Main_Billing_Name,obj->BillingName);
    map.insert(Purchase_Invoice_Main_Total,obj->grandTotal);
    map.insert(Purchase_Invoice_Main_Narration,obj->narration);
    map.insert(Purchase_Invoice_Main_Added_By,obj->AddedBy);
    map.insert(Purchase_Invoice_Main_Delivery_Date,obj->DeliveryDate);
    map.insert(Purchase_Invoice_Main_Completion_Probability,obj->CompletionProbability);
    map.insert(Purchase_Invoice_Main_Credit_Period,obj->ledgerObject.Credit_Period);
    map.insert(Purchase_Invoice_Main_POC_Name,obj->ledgerObject.ContactPersonName);
    map.insert(Purchase_Invoice_Main_POC_Phone,obj->ledgerObject.ContactPersonNumber);
    map.insert(Purchase_Invoice_Main_POC_Email,obj->ledgerObject.ContactPersonEmail);
    map.insert(Purchase_Invoice_Main_Project_ID,obj->ProjectId);
    map.insert(Purchase_Invoice_Main_Revision_Number,obj->RevisionNo);
    map.insert(Purchase_Invoice_Main_Converted_To_Sales_Order,obj->ConvertedToSalesOrder);
    map.insert(Purchase_Invoice_Main_Quotation_Dropped,obj->QuotationDropped);
    map.insert(Purchase_Invoice_Main_Quotation_Dropped_Reason,obj->QuotationDroppedReason);
    map.insert(Purchase_Invoice_Main_Requirement_Voucher_No,obj->RequirementVoucherNo);
    map.insert(Purchase_Invoice_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Purchase_Invoice_Main_Discount_In_Amount,obj->discountinAmount);
    map.insert(Purchase_Invoice_Main_Discount_In_Percent,obj->discountPercent);
    map.insert(Purchase_Invoice_Main_Gross_Total,obj->grossTotal);
    map.insert(Purchase_Invoice_Main_VATAmount,obj->taxTotalAmount);
    map.insert(Purchase_Invoice_Main_GeneratedTransactionId, obj->TransactionId);
    map.insert(Purchase_Invoice_Main_InvoiceNo, obj->invoiceNumber);
    map.insert(Purchase_Invoice_Main_InvoiceDate, obj->invoiceDate);
    map.insert(Purchase_Invoice_Main_Last_Updated,QDateTime::currentDateTime());
    map.insert(Purchase_Invoice_Main_Godown,obj->toGodownID);
    QJsonDocument doc(obj->ReqVoucherList);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    map.insert(Purchase_Invoice_Main_ReqVoucherList, strJson);

    QMap<QString,QVariant> map3;
    //    map3.insert(Purchase_Invoice_Main_id,oldID);
    map3.insert(Purchase_Invoice_Main_Voucher_No, oldID);
    map3.insert(Purchase_Invoice_Main_Voucher_Prefix, obj->VoucherPrefix);
    db->updateRecordsPlus(map,map3,Purchase_Invoice_Main_TableName);

    QMap<QString,QVariant> maptxndel;
    maptxndel.insert(Transaction_Helper_VoucherNo,oldID);
    maptxndel.insert(Transaction_Helper_VoucherType,VoucherType);
    //    db->deleteRecordsPlus(maptxndel,Transaction_Helper_TableName);

    QMap<QString,QVariant> map6;
    map6.insert(Purchase_Invoice_Details_Voucher_No,oldID);
    map6.insert(Purchase_Invoice_Details_Voucher_Prefix, obj->VoucherPrefix);
    db->deleteRecordsPlus(map6,Purchase_Invoice_Details_TableName);

    itemTransaction->deleteItemTransaction(obj);

    for(CompoundItemDataObject compitem:obj->InventoryItems){
        QMap<QString,QVariant> map1;
        inventoryItemDataModel item = compitem.BaseItem;
        //        item.ItemReqUuid = db->getUUID();
        if(item.ItemReqUuid == "X"){
            item.ItemReqUuid = db->getUUID();
        }
        map1.insert(Purchase_Invoice_Details_Voucher_No,obj->voucherNumber);
        map1.insert(Purchase_Invoice_Details_Voucher_Prefix,obj->VoucherPrefix);
        map1.insert(Purchase_Invoice_Details_Voucher_Date,obj->VoucherDate.toString("yyyy-MM-dd"));
        map1.insert(Purchase_Invoice_Details_Inventory_Item_ID,item.ItemID);
        map1.insert(Purchase_Invoice_Details_Quantity,item.quantity/**item.uomObject.convRate*/);
        map1.insert(Purchase_Invoice_Details_Price,item.price);
        map1.insert(Purchase_Invoice_Details_PriceLevel,item.PriceLevel);
        map1.insert(Purchase_Invoice_Details_isCompoundItem,item.IsCompoundItem);
        map1.insert(Purchase_Invoice_Details_subItemOff_list_ID,0);
        map1.insert(Purchase_Invoice_Details_Discount_Percent,item.discountPercentage);
        map1.insert(Purchase_Invoice_Details_Discount_Amount,item.discountinAmount);
        map1.insert(Purchase_Invoice_Details_Subtotal,item.subTotal);
        map1.insert(Purchase_Invoice_Details_Project_ID,obj->ProjectId);
        map1.insert(Purchase_Invoice_Details_Item_Narration,item.narration);
        map1.insert(Purchase_Invoice_Details_Item_Description,item.ItemDescription);
        map1.insert(Purchase_Invoice_Details_Salesman_ID,obj->SalesmanID);
        //        map1.insert(Purchase_Invoice_Details_Requirement_ItemID,item.requirementItemIdOld);

        map1.insert(Purchase_Invoice_Details_Requirement_ItemID,item.ItemReqUuid);
        map1.insert(Purchase_Invoice_Details_Dimension,item.Dimension);
        map1.insert(Purchase_Invoice_Details_Length,item.length);
        map1.insert(Purchase_Invoice_Details_UomID, item.uomObject._id);
        map1.insert(Purchase_Invoice_Details_TaxRate, item.taxRate);
        map1.insert(Purchase_Invoice_Details_TaxAmount, item.taxAmount);

        ItemModel->ItemID = item.ItemID;
        ItemModel->From_Ledger = item.defaultPurchaseLedgerID;
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

        int basid = db->insertRecordsPlus(map1,Purchase_Invoice_Details_TableName);

        for(inventoryItemDataModel subitem:compitem.subItems){
            QMap<QString,QVariant> map2;
            map2.insert(Purchase_Invoice_Details_Voucher_No,obj->voucherNumber);
            map2.insert(Purchase_Invoice_Details_Voucher_Prefix,obj->VoucherPrefix);
            map2.insert(Purchase_Invoice_Details_Voucher_Date,obj->VoucherDate);
            map2.insert(Purchase_Invoice_Details_Inventory_Item_ID,subitem.ItemID);
            map2.insert(Purchase_Invoice_Details_Quantity,subitem.quantity*subitem.uomObject.convRate);
            map2.insert(Purchase_Invoice_Details_Price,subitem.price);
            map2.insert(Purchase_Invoice_Details_PriceLevel,subitem.PriceLevel);
            map2.insert(Purchase_Invoice_Details_isCompoundItem,subitem.IsCompoundItem);
            map2.insert(Purchase_Invoice_Details_subItemOff_list_ID,basid);
            map2.insert(Purchase_Invoice_Details_Discount_Percent,subitem.discountPercentage);
            map2.insert(Purchase_Invoice_Details_Discount_Amount,subitem.discountinAmount);
            map2.insert(Purchase_Invoice_Details_Subtotal,subitem.subTotal);
            map2.insert(Purchase_Invoice_Details_Project_ID,obj->ProjectId);
            map2.insert(Purchase_Invoice_Details_Item_Narration,subitem.narration);
            map2.insert(Purchase_Invoice_Details_Item_Description,subitem.ItemDescription);
            map2.insert(Purchase_Invoice_Details_Salesman_ID,obj->SalesmanID);
            map2.insert(Purchase_Invoice_Details_Requirement_ItemID,subitem.ItemReqUuid);
            map2.insert(Purchase_Invoice_Details_Dimension,item.Dimension);
            map2.insert(Purchase_Invoice_Details_Length,item.length);
            map2.insert(Purchase_Invoice_Details_UomID, item.uomObject._id);
            map2.insert(Purchase_Invoice_Details_TaxRate, subitem.taxRate);
            map2.insert(Purchase_Invoice_Details_TaxAmount, subitem.taxAmount);

            db->insertRecordsPlus(map2,Purchase_Invoice_Details_TableName);
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

    if(!obj->paymentSplit)
        obj->ledgersList.insert(0,MainLedger);

    obj->voucherType = VoucherType;

    obj->ledgersList[0].AgainstLedger = "0x8x8";


    ledTransaction->deleteTransaction(obj);
    ledTransaction->insertTransaction(obj);

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

int PurchaseVoucherDatabaseHelper::deleteVoucher(GeneralVoucherDataObject *obj)
{
    this->startTransaction();
    QMap<QString,QVariant> map3;
    map3.insert(Purchase_Invoice_Main_Voucher_No,obj->voucherNumber);
    map3.insert(Purchase_Invoice_Main_Voucher_Prefix,obj->VoucherPrefix);
    db->deleteRecordsPlus(map3,Purchase_Invoice_Main_TableName);

    QMap<QString,QVariant> map4;
    map4.insert(Purchase_Invoice_Details_Voucher_No,obj->voucherNumber);
    map3.insert(Purchase_Invoice_Details_Voucher_Prefix,obj->VoucherPrefix);
    db->deleteRecordsPlus(map4,Purchase_Invoice_Details_TableName);

    itemTransaction->deleteItemTransaction(obj);
    ledTransaction->deleteTransaction(obj);

    return commit();

}

int PurchaseVoucherDatabaseHelper:: commit()
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

int PurchaseVoucherDatabaseHelper::getMaxID()
{
    QString qStr = "SELECT MAX("+Purchase_Invoice_Main_id+") FROM " + Purchase_Invoice_Main_TableName;
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



GeneralVoucherDataObject *PurchaseVoucherDatabaseHelper::getVoucherByVoucherNoPtr(QString vNo, QString voucherPrefix)
{
    qDebug()<<"256";
    GeneralVoucherDataObject *obj = new GeneralVoucherDataObject();
    SalesInventoryItemDatabaseHelper *itemHelper = new SalesInventoryItemDatabaseHelper();
    LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper();
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


    QString qStrDetail = "SELECT distinct "

            + Purchase_Invoice_Details_Voucher_No
            + ", " + Purchase_Invoice_Details_Voucher_Prefix
            + ", " + Purchase_Invoice_Details_Voucher_Date
            + ", " + Purchase_Invoice_Details_Inventory_Item_ID
            + ", " + Purchase_Invoice_Details_Quantity
            + ", " + Purchase_Invoice_Details_Price
            + ", " + Purchase_Invoice_Details_UomID
            + ", subq.convalue , subq.UomName, subq.UomSymbol "
            + ", " + Purchase_Invoice_Details_PriceLevel
            + ", " + Purchase_Invoice_Details_Discount_Percent
            + ", " + Purchase_Invoice_Details_Discount_Amount
            + ", " + Purchase_Invoice_Details_Subtotal
            + ", " + Purchase_Invoice_Details_Item_Narration
            + ", " + Purchase_Invoice_Details_Item_Description
            + ", " + Purchase_Invoice_Details_Salesman_ID
            + ", " + Purchase_Invoice_Details_Requirement_ItemID
            + ", " + Purchase_Invoice_Details_Length
            + ", " + Purchase_Invoice_Details_Dimension
            + ", " + Purchase_Invoice_Details_itemVoucherStatus
            + ", " + Purchase_Invoice_Details_itemProductionStatus
            + ", " + Purchase_Invoice_Details_Time
            + ", " + Purchase_Invoice_Details_isCompoundItem
            + ", " + Purchase_Invoice_Details_subItemOff_list_ID
            + ", "  + Purchase_Invoice_Details_TaxRate
            + ", " + Purchase_Invoice_Details_TaxAmount
            + " FROM " + Purchase_Invoice_Details_TableName+ " s LEFT OUTER JOIN "
            + UomDataBaseHelper::qStrUomForItemDetail + " as subq "
            + " ON s." + Purchase_Invoice_Details_Inventory_Item_ID + " = subq.itemID "
            + " AND s." + Purchase_Invoice_Details_UomID + " = subq.UOM "
            + " WHERE " + Purchase_Invoice_Details_Voucher_No + "= :vno"
            + " AND " +  Purchase_Invoice_Details_Voucher_Prefix + "= :voucherPrefix"
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
            //         qDebug()<<"at db helper Ledgers are : "<<ledger.LedgerName<<" : "<<ledger.drAmount<<ledger.crAmount<<ledger.isInvoiceItem;
            if(!ledger.isInvoiceItem){
                //                                qDebug()<<"Adding "<<ledger.LedgerName;
                obj->ledgersList.append(ledger);
            }

        }
    }
    qDebug()<<qStrDetail<<vNo;
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
            item.godownList = itemTransaction->getGodownAllocationByVoucherAndItem(vNo, obj->voucherType,
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
            item.taxRate = query.value(25).toFloat();
            item.taxAmount = query.value(26).toFloat();
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
            + Purchase_Invoice_Main_Voucher_No + ", "
            + Purchase_Invoice_Main_Voucher_Prefix + ", "
            + Purchase_Invoice_Main_Date_Created + ", "
            + Purchase_Invoice_Main_Voucher_Date + ", "
            + Purchase_Invoice_Main_TimeStamp + ", "
            + Purchase_Invoice_Main_Ledger_ID + ", "
            + Purchase_Invoice_Main_Billing_Name + ", "
            + Purchase_Invoice_Main_Total + ", "
            + Purchase_Invoice_Main_Narration + ", "
            + Purchase_Invoice_Main_Added_By + ", "
            + Purchase_Invoice_Main_Delivery_Date + ", "
            + Purchase_Invoice_Main_Completion_Probability + ", "
            + Purchase_Invoice_Main_Credit_Period + ", "
            + Purchase_Invoice_Main_POC_Name + ", "
            + Purchase_Invoice_Main_POC_Phone + ", "
            + Purchase_Invoice_Main_POC_Email + ", "
            + Purchase_Invoice_Main_Project_ID + ", "
            + Purchase_Invoice_Main_Revision_Number + ", "
            + Purchase_Invoice_Main_Converted_To_Sales_Order + ", "
            + Purchase_Invoice_Main_Quotation_Dropped + ", "
            + Purchase_Invoice_Main_Quotation_Dropped_Reason + ", "
            + Purchase_Invoice_Main_Requirement_Voucher_No + ", "
            + Purchase_Invoice_Main_Salesman_ID + ", "
            + Purchase_Invoice_Main_LPORef + ", "
            + Purchase_Invoice_Main_VoucherStatus + ", "
            + Purchase_Invoice_Main_ModeOfService + ", "
            + Purchase_Invoice_Main_Discount_In_Amount + ", "
            + Purchase_Invoice_Main_Discount_In_Percent + ", "
            + Purchase_Invoice_Main_VATAmount + ", "
            + Purchase_Invoice_Main_Contact_ID + ", "
            + Purchase_Invoice_Main_Gross_Total + ", "
            + Purchase_Invoice_Main_Reference + ", "
            + Purchase_Invoice_Main_Customer_Expecting_Date + ", "
            + Purchase_Invoice_Main_Currency + ", "
            + Purchase_Invoice_Main_BoxesCount+ ", "
            + Purchase_Invoice_Main_TotalWeight + ", "
            + Purchase_Invoice_Main_LRNo + ", "
            + Purchase_Invoice_Main_VoucherNoDisplay + ", "
            + Purchase_Invoice_Main_ReqVoucherList + ", "
            + Purchase_Invoice_Main_Origin + ", "
            + Purchase_Invoice_Main_GeneratedTransactionId+ ", "
            + Purchase_Invoice_Main_InvoiceNo +", "
            + Purchase_Invoice_Main_InvoiceDate + ", "
            + Purchase_Invoice_Main_Godown +" "
            + " FROM " + Purchase_Invoice_Main_TableName
            + " WHERE " + Purchase_Invoice_Main_Voucher_No + "= :vno"
            + " AND " +  Purchase_Invoice_Main_Voucher_Prefix + "= :voucherPrefix";

    query.prepare(qStrMain);
    query.bindValue(":vno",vNo);
    query.bindValue(":voucherPrefix", voucherPrefix);
    //    query.bindValue(":dateFrom",dateFrom.toString("yyyy-MM-dd"));
    //    query.bindValue("dateTo",dateTo.toString("yyyy-MM-dd"));
    if(!query.exec()){
        qDebug()<<query.lastQuery();
        qDebug()<<query.lastError();
    }
    else{
        while(query.next()){
            int i =0;

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
            obj->invoiceNumber= query.value(i++).toString();
            obj->invoiceDate = query.value(i++).toDate();
            obj->toGodownID = query.value(i++).toString();
        }
    }

    // Test receive channel
    //    obj = new GeneralVoucherDataObject();
    //    GeneralVoucherDataObject v;
    //    QString jsonData;
    //    QString qStrJson = " SELECT data from RECEIVECHANNEL WHERE generated_transactionid = " + vNo + "+1" ;
    //    qDebug()<<qStrJson;
    //    query.prepare(qStrJson);
    //    if(query.exec()){
    //        if(query.first()){
    //            jsonData = query.value(0).toString();
    //        }
    //    }
    //    else{
    //        qDebug()<<"NO DATA IN RECEIVE";
    //    }
    //    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
    //    QJsonObject jsonObj = doc.object();
    //    obj = v.JsonToObj(jsonObj);
    //    qDebug()<<obj->voucherType<<obj->voucherNumber<<obj->VoucherDate<<obj->InventoryItems.size();


    // EOL

    return obj;

}

float PurchaseVoucherDatabaseHelper::getLastPurchPrice(QString itemID)
{
    float val = 0;
    QString qStr = "SELECT "+Purchase_Invoice_Details_Price+ " from "+Purchase_Invoice_Details_TableName;
    qStr += " where "+Purchase_Invoice_Details_Inventory_Item_ID + "='"+itemID+"' order by ";

    qStr += Purchase_Invoice_Details_Voucher_Date ;
    qStr += " desc limit 1";
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    if(query.exec(qStr)){
        while(query.next()){
            val = query.value(0).toFloat();
        }
    }
    return val;
}

QSqlQueryModel *PurchaseVoucherDatabaseHelper::getVoucherListByDateModel(QDate dateFrom, QDate dateTo)
{
    LedgerMasterDatabaseHelper ledHelper;
    QString qStr ="SELECT ";
    qStr += "'V' , ";
    qStr += Purchase_Invoice_Main_Voucher_No +", ";
    qStr += "'" + VoucherType + "', " ;
    qStr += Purchase_Invoice_Main_Voucher_Prefix + ", ";
    qStr += Purchase_Invoice_Main_Voucher_Date+" as 'Voucher Date', ";
    //    qStr += " CONCAT(" + Purchase_Invoice_Main_Voucher_Prefix + ", '-', " + Purchase_Invoice_Main_Voucher_No+ ") as 'Voucher No', ";
    qStr += Purchase_Invoice_Main_Voucher_No +" as 'Voucher No', ";
    qStr += "(SELECT "+ledHelper.Ledger_Master_LedgerName + " FROM "+ ledHelper.Ledger_Master_TableName;
    qStr += " WHERE "+ledHelper.Ledger_Master_LedgerID + "= som.";
    qStr += Purchase_Invoice_Main_Ledger_ID + ") Ledger, ";
    qStr += Purchase_Invoice_Main_Total ;

    qStr += " FROM " + Purchase_Invoice_Main_TableName;
    qStr += " som WHERE ";
    qStr += Purchase_Invoice_Main_Voucher_Date;
    qStr += " >= '";
    qStr += dateFrom.toString("yyyy-MM-dd");
    qStr += "' AND ";
    qStr += Purchase_Invoice_Main_Voucher_Date;
    qStr += " <= '";
    qStr += dateTo.toString("yyyy-MM-dd");
    qStr += "'";

    return DatabaseHelper::getQueryModel(qStr);

}

