#include "purchaseorderdatabasehelper.h"
QString PurchaseOrderDatabaseHelper::Purchase_Order_Main_TableName = "Purchase_Order_Main";
QString PurchaseOrderDatabaseHelper::Purchase_Order_Details_TableName = "Purchase_Order_Details";

PurchaseOrderDatabaseHelper::PurchaseOrderDatabaseHelper(QString connString)
{
    db = new DatabaseHelper(connString);
    ledTransaction = new TransactionHelper();
}

PurchaseOrderDatabaseHelper::~PurchaseOrderDatabaseHelper()
{

}

int PurchaseOrderDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

QString PurchaseOrderDatabaseHelper::getNextVoucherNo(QString voucherPrefix)
{
    GeneralVoucherDatabaseHelper vHelper;
    QString voucherNumber = vHelper.getNextVoucherNoByType(VoucherType, voucherPrefix, Purchase_Order_Main_TableName);
    return voucherNumber;

}

QPair<QString, QString> PurchaseOrderDatabaseHelper::getVoucherNoByTransactionId(QString transId)
{
    GeneralVoucherDatabaseHelper vHelper;
    return vHelper.getVoucherNoByTransactionId(transId, Purchase_Order_Main_TableName);

}

int PurchaseOrderDatabaseHelper::insertVoucher(GeneralVoucherDataObject *obj)
{
    this->startTransaction();
    QMap<QString,QVariant> map;

    GeneralVoucherDatabaseHelper vHelper;
    obj->voucherNumber = vHelper.setNextVoucherNoByType(VoucherType, obj->VoucherPrefix, Purchase_Order_Main_TableName );

    if(obj->TransactionId.length()==0)
    obj->TransactionId=QString::number(QDateTime::currentDateTime().toTime_t()) + obj->VoucherPrefix + db->clientId;








    map.insert(Purchase_Order_Main_Voucher_No,obj->voucherNumber);
    map.insert(Purchase_Order_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Purchase_Order_Main_Date_Created,obj->VoucherDate);
    map.insert(Purchase_Order_Main_Voucher_Date,obj->VoucherDate);
    map.insert(Purchase_Order_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    map.insert(Purchase_Order_Main_Billing_Name,obj->ledgerObject.LedgerName);
    map.insert(Purchase_Order_Main_Total,obj->grandTotal);
    map.insert(Purchase_Order_Main_Narration,obj->narration);
    map.insert(Purchase_Order_Main_Added_By,obj->AddedBy);
    map.insert(Purchase_Order_Main_Delivery_Date,obj->DeliveryDate);
    map.insert(Purchase_Order_Main_Completion_Probability,obj->CompletionProbability);
    map.insert(Purchase_Order_Main_Credit_Period,obj->ledgerObject.Credit_Period);
    map.insert(Purchase_Order_Main_POC_Name,obj->ledgerObject.ContactPersonName);
    map.insert(Purchase_Order_Main_POC_Phone,obj->ledgerObject.ContactPersonNumber);
    map.insert(Purchase_Order_Main_POC_Email,obj->ledgerObject.ContactPersonEmail);
    map.insert(Purchase_Order_Main_Project_ID,obj->ProjectId);
    map.insert(Purchase_Order_Main_Revision_Number,obj->RevisionNo);
    map.insert(Purchase_Order_Main_Converted_To_Sales_Order,obj->ConvertedToSalesOrder);
    map.insert(Purchase_Order_Main_Quotation_Dropped,obj->QuotationDropped);
    map.insert(Purchase_Order_Main_Quotation_Dropped_Reason,obj->QuotationDroppedReason);
    map.insert(Purchase_Order_Main_Requirement_Voucher_No,obj->RequirementVoucherNo);
    map.insert(Purchase_Order_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Purchase_Order_Main_LPO,obj->LPO);
    map.insert(Purchase_Order_Main_ModeOfService,obj->ModeOfService);
    map.insert(Purchase_Order_Main_Contact_ID,obj->Contact._id);
    map.insert(Purchase_Order_Main_VATAmount,obj->taxTotalAmount);
    map.insert(Purchase_Order_Main_VoucherStatus,obj->status);
    map.insert(Purchase_Order_Main_Discount_In_Amount,obj->discountinAmount);
    map.insert(Purchase_Order_Main_Discount_In_Percent,obj->discountPercent);
    map.insert(Purchase_Order_Main_Gross_Total,obj->grossTotal);
    map.insert(Purchase_Order_Main_Reference,obj->reference);
    map.insert(Purchase_Order_Main_TransactionId, obj->TransactionId);
    map.insert(Purchase_Order_Main_TimeStampC,obj->timestamp);
    map.insert(Purchase_Order_Main_Last_Updated,QDateTime::currentDateTime());

    QJsonDocument doc(obj->ReqVoucherList);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    map.insert(Purchase_Order_Main_ReqVoucherList,strJson);

    db->insertRecordsPlus(map,Purchase_Order_Main_TableName);

    for(CompoundItemDataObject compitem:obj->InventoryItems){
        QMap<QString,QVariant> map1;
        inventoryItemDataModel item = compitem.BaseItem;
        if(item.ItemReqUuid == "X")
            item.ItemReqUuid = db->getUUID();

        map1.insert(Purchase_Order_Details_Voucher_No,obj->voucherNumber);
        map1.insert(Purchase_Order_Details_Voucher_Prefix,obj->VoucherPrefix);
        map1.insert(Purchase_Order_Details_Voucher_Date,obj->VoucherDate);
        map1.insert(Purchase_Order_Details_Inventory_Item_ID,item.ItemID);
        map1.insert(Purchase_Order_Details_Quantity,item.quantity);
        map1.insert(Purchase_Order_Details_QuantityDisc,item.discQuantity);
        map1.insert(Purchase_Order_Details_QuantityFull,item.quantityFull);
        map1.insert(Purchase_Order_Details_Price,item.price);
        map1.insert(Purchase_Order_Details_PriceLevel,item.PriceLevel);
        map1.insert(Purchase_Order_Details_isCompoundItem,item.IsCompoundItem);
        map1.insert(Purchase_Order_Details_subItemOff_list_ID,0);
        map1.insert(Purchase_Order_Details_Discount_Percent,item.discountPercentage);
        map1.insert(Purchase_Order_Details_Discount_Amount,item.discountinAmount);
        map1.insert(Purchase_Order_Details_Subtotal,item.subTotal);
        map1.insert(Purchase_Order_Details_Project_ID,item.ProjectID);
        map1.insert(Purchase_Order_Details_Item_Narration,item.narration);
        map1.insert(Purchase_Order_Details_Item_Description,item.ItemDescription);
        map1.insert(Purchase_Order_Details_Salesman_ID,obj->SalesmanID);
        map1.insert(Purchase_Order_Details_Requirement_ItemID,item.ItemReqUuid);
        map1.insert(Purchase_Order_Details_Length,item.length);
        map1.insert(Purchase_Order_Details_Dimension,item.Dimension);
        map1.insert(Purchase_Order_Details_itemProductionStatus,item.itemProductionStatus);
        map1.insert(Purchase_Order_Details_itemVoucherStatus,item.itemVoucherStatus);
        map1.insert(Purchase_Order_Details_UOM_ID,item.uomObject._id);
        map1.insert(Purchase_Order_Details_TaxRate, item.taxRate);
        map1.insert(Purchase_Order_Details_TaxAmount, item.taxAmount);
        map1.insert(Purchase_Order_Details_TimeDate, item.manufactureDate);
        map1.insert(Purchase_Order_Details_ContainerCount, item.requestQty);

        int basid = db->insertRecordsPlus(map1,Purchase_Order_Details_TableName);

        for(inventoryItemDataModel subitem:compitem.subItems){
            QMap<QString,QVariant> map2;
            if(subitem.ItemReqUuid == "X")
                subitem.ItemReqUuid = db->getUUID();
            map2.insert(Purchase_Order_Details_Voucher_No,obj->voucherNumber);
            map2.insert(Purchase_Order_Details_Voucher_Prefix,obj->VoucherPrefix);
            map2.insert(Purchase_Order_Details_Voucher_Date,obj->VoucherDate);
            map2.insert(Purchase_Order_Details_Inventory_Item_ID,subitem.ItemID);
            map2.insert(Purchase_Order_Details_Quantity,subitem.quantity);
            map2.insert(Purchase_Order_Details_UOM_ID,subitem.uomObject._id);
            map2.insert(Purchase_Order_Details_Price,subitem.price);
            map2.insert(Purchase_Order_Details_PriceLevel,subitem.PriceLevel);
            map2.insert(Purchase_Order_Details_isCompoundItem,subitem.IsCompoundItem);
            map2.insert(Purchase_Order_Details_subItemOff_list_ID,basid);
            map2.insert(Purchase_Order_Details_Discount_Percent,subitem.discountPercentage);
            map2.insert(Purchase_Order_Details_Discount_Amount,subitem.discount);
            map2.insert(Purchase_Order_Details_Subtotal,subitem.subTotal);
            map2.insert(Purchase_Order_Details_Project_ID,obj->ProjectId);
            map2.insert(Purchase_Order_Details_Item_Narration,subitem.narration);
            map2.insert(Purchase_Order_Details_Item_Description,subitem.ItemDescription);
            map2.insert(Purchase_Order_Details_Salesman_ID,obj->SalesmanID);
            map2.insert(Purchase_Order_Details_Requirement_ItemID,subitem.ItemReqUuid);
            map2.insert(Purchase_Order_Details_TaxRate, subitem.taxRate);
            map2.insert(Purchase_Order_Details_TaxAmount, subitem.taxAmount);

            db->insertRecordsPlus(map2,Purchase_Order_Details_TableName);
        }
    }
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

    qDebug()<<"%%%%%%%%%%%%%5307"<<obj->ledgersList.size();

    LedgerMasterDataModel MainLedger;

    MainLedger.crAmount = obj->grandTotal;
    MainLedger.LedgerID = obj->ledgerObject.LedgerID;
    MainLedger.isInvoiceItem = true;
    MainLedger.mapList = obj->mapList;

    QString agDrID ;
    QString agCrID = obj->ledgerObject.LedgerID ;


    float  amt=0;
    for(int i=0;i<obj->ledgersList.size();i++)
    {
        obj->ledgersList[i].AgainstLedger = agCrID;
        if(obj->ledgersList[i].drAmount>amt)
            agDrID = obj->ledgersList[i].LedgerID;

        if(obj->paymentSplit)
            obj->ledgersList[i].isInvoiceItem = true;
    }

    if(!obj->paymentSplit)
        obj->ledgersList.insert(0,MainLedger);

    obj->voucherType = VoucherType;

    obj->ledgersList[0].AgainstLedger = "0x8x4";

    qDebug()<<"%%%%%%%%%%%%%5307"<<obj->ledgersList.size()<<"&&&&&&&&&&&&&&&";
    ledTransaction->insertProxyTransaction(obj);


    //    return commit();

    bool c=commit();

    if(c && obj->ledgerObject.dbName != "" && !obj->fromExternal && obj->sendFlag){
        GeneralVoucherDataObject v;
        NotificationDataModel *notification = new NotificationDataModel();
        QJsonDocument doc(v.ObjToJson(obj));
        QString strJson(doc.toJson(QJsonDocument::Compact));

        notification->NotificationData = strJson;
        notification->ToDBName=obj->ledgerObject.dbName;
        notification->TypeOfData="Voucher";
        notification->Action=1001;
        //      notification->GeneratedTransactionId=QString::number(QDateTime::currentDateTime().toTime_t()) + obj->VoucherPrefix;
        notification->GeneratedTransactionId = obj->TransactionId;

        sendChannelDatabaseHelper *sendHelper = new sendChannelDatabaseHelper();
        sendHelper->insertNotification(notification);
    }

    return c;

}

int PurchaseOrderDatabaseHelper::updateVoucher(GeneralVoucherDataObject *obj, QString oldID)
{
    this->startTransaction();

    QMap<QString,QVariant> map5;
    map5.insert(Purchase_Order_Details_Voucher_No,oldID);
    map5.insert(Purchase_Order_Details_Voucher_Prefix,obj->VoucherPrefix);
    db->deleteRecordsPlus(map5,Purchase_Order_Details_TableName);

    QMap<QString,QVariant> map;

    map.insert(Purchase_Order_Main_Voucher_No,obj->voucherNumber);
    map.insert(Purchase_Order_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Purchase_Order_Main_Date_Created,obj->VoucherDate);
    map.insert(Purchase_Order_Main_Voucher_Date,obj->VoucherDate);
    map.insert(Purchase_Order_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    map.insert(Purchase_Order_Main_Billing_Name,obj->ledgerObject.LedgerName);
    map.insert(Purchase_Order_Main_Total,obj->grandTotal);
    map.insert(Purchase_Order_Main_Narration,obj->narration);
    map.insert(Purchase_Order_Main_Added_By,obj->AddedById);
    map.insert(Purchase_Order_Main_Delivery_Date,obj->DeliveryDate);
    map.insert(Purchase_Order_Main_Completion_Probability,obj->CompletionProbability);
    map.insert(Purchase_Order_Main_Credit_Period,obj->ledgerObject.Credit_Period);
    map.insert(Purchase_Order_Main_POC_Name,obj->Contact.ContactName);
    map.insert(Purchase_Order_Main_POC_Phone,obj->Contact.PhoneNumber);
    map.insert(Purchase_Order_Main_POC_Email,obj->Contact.email);
    map.insert(Purchase_Order_Main_Project_ID,obj->ProjectId);
    map.insert(Purchase_Order_Main_Revision_Number,obj->RevisionNo);
    map.insert(Purchase_Order_Main_Converted_To_Sales_Order,obj->ConvertedToSalesOrder);
    map.insert(Purchase_Order_Main_Quotation_Dropped,obj->QuotationDropped);
    map.insert(Purchase_Order_Main_Quotation_Dropped_Reason,obj->QuotationDroppedReason);
    map.insert(Purchase_Order_Main_Requirement_Voucher_No,obj->RequirementVoucherNo);
    map.insert(Purchase_Order_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Purchase_Order_Main_LPO,obj->LPO);
    map.insert(Purchase_Order_Main_ModeOfService,obj->ModeOfService);
    map.insert(Purchase_Order_Main_Contact_ID,obj->Contact._id);
    map.insert(Purchase_Order_Main_VATAmount,obj->taxTotalAmount);
    map.insert(Purchase_Order_Main_VoucherStatus,obj->status);
    map.insert(Purchase_Order_Main_Discount_In_Amount,obj->discountinAmount);
    map.insert(Purchase_Order_Main_Discount_In_Percent,obj->discountPercent);
    map.insert(Purchase_Order_Main_Gross_Total,obj->grossTotal);
    map.insert(Purchase_Order_Main_Reference,obj->reference);
    map.insert(Purchase_Order_Main_TransactionId, obj->TransactionId);
    map.insert(Purchase_Order_Main_Last_Updated,QDateTime::currentDateTime());


    QJsonDocument doc(obj->ReqVoucherList);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    map.insert(Purchase_Order_Main_ReqVoucherList,strJson);

    QMap<QString,QVariant> map3;
    map3.insert(Purchase_Order_Main_Voucher_No,oldID);
    map3.insert(Purchase_Order_Main_Voucher_Prefix,obj->VoucherPrefix);

    db->updateRecordsPlus(map,map3,Purchase_Order_Main_TableName);

    for(CompoundItemDataObject compitem:obj->InventoryItems){
        QMap<QString,QVariant> map1;
        inventoryItemDataModel item = compitem.BaseItem;


        map1.insert(Purchase_Order_Details_Voucher_No,obj->voucherNumber);
        map1.insert(Purchase_Order_Details_Voucher_Prefix,obj->VoucherPrefix);
        map1.insert(Purchase_Order_Details_Voucher_Date,obj->VoucherDate);
        map1.insert(Purchase_Order_Details_Inventory_Item_ID,item.ItemID);
        map1.insert(Purchase_Order_Details_Quantity,item.quantity);
        map1.insert(Purchase_Order_Details_QuantityDisc,item.discQuantity);
        map1.insert(Purchase_Order_Details_QuantityFull,item.quantityFull);
        map1.insert(Purchase_Order_Details_Price,item.price);
        map1.insert(Purchase_Order_Details_PriceLevel,item.PriceLevel);
        map1.insert(Purchase_Order_Details_isCompoundItem,item.IsCompoundItem);
        map1.insert(Purchase_Order_Details_subItemOff_list_ID,0);
        map1.insert(Purchase_Order_Details_Discount_Percent,item.discountPercentage);
        map1.insert(Purchase_Order_Details_Discount_Amount,item.discountinAmount);
        map1.insert(Purchase_Order_Details_Subtotal,item.subTotal);
        map1.insert(Purchase_Order_Details_Project_ID,item.ProjectID);
        map1.insert(Purchase_Order_Details_Item_Narration,item.narration);
        map1.insert(Purchase_Order_Details_Item_Description,item.ItemDescription);
        map1.insert(Purchase_Order_Details_Salesman_ID,obj->SalesmanID);
        map1.insert(Purchase_Order_Details_Requirement_ItemID,item.ItemReqUuid);
        map1.insert(Purchase_Order_Details_Length,item.length);
        map1.insert(Purchase_Order_Details_Dimension,item.Dimension);
        map1.insert(Purchase_Order_Details_itemProductionStatus,item.itemProductionStatus);
        map1.insert(Purchase_Order_Details_itemVoucherStatus,item.itemVoucherStatus);
        map1.insert(Purchase_Order_Details_UOM_ID,item.uomObject._id);
        map1.insert(Purchase_Order_Details_TaxRate, item.taxRate);
        map1.insert(Purchase_Order_Details_TaxAmount, item.taxAmount);
        map1.insert(Purchase_Order_Details_TimeDate, item.manufactureDate);
        map1.insert(Purchase_Order_Details_ContainerCount, item.requestQty);

        int basid = db->insertRecordsPlus(map1,Purchase_Order_Details_TableName);

        for(inventoryItemDataModel subitem:compitem.subItems){
            QMap<QString,QVariant> map2;
            map2.insert(Purchase_Order_Details_Voucher_No,obj->voucherNumber);
            map2.insert(Purchase_Order_Details_Voucher_Prefix,obj->VoucherPrefix);
            map2.insert(Purchase_Order_Details_Voucher_Date,obj->VoucherDate);
            map2.insert(Purchase_Order_Details_Inventory_Item_ID,subitem.ItemID);
            map2.insert(Purchase_Order_Details_Quantity,subitem.quantity);
            map2.insert(Purchase_Order_Details_UOM_ID,subitem.uomObject._id);
            map2.insert(Purchase_Order_Details_Price,subitem.price);
            map2.insert(Purchase_Order_Details_PriceLevel,subitem.PriceLevel);
            map2.insert(Purchase_Order_Details_isCompoundItem,subitem.IsCompoundItem);
            map2.insert(Purchase_Order_Details_subItemOff_list_ID,basid);
            map2.insert(Purchase_Order_Details_Discount_Percent,subitem.discountPercentage);
            map2.insert(Purchase_Order_Details_Discount_Amount,subitem.discount);
            map2.insert(Purchase_Order_Details_Subtotal,subitem.subTotal);
            map2.insert(Purchase_Order_Details_Project_ID,obj->ProjectId);
            map2.insert(Purchase_Order_Details_Item_Narration,subitem.narration);
            map2.insert(Purchase_Order_Details_Item_Description,subitem.ItemDescription);
            map2.insert(Purchase_Order_Details_Salesman_ID,obj->SalesmanID);
            map2.insert(Purchase_Order_Details_Requirement_ItemID,subitem.ItemReqUuid);
            map2.insert(Purchase_Order_Details_TaxRate, subitem.taxRate);
            map2.insert(Purchase_Order_Details_TaxAmount, subitem.taxAmount);

            db->insertRecordsPlus(map2,Purchase_Order_Details_TableName);
        }
    }

//    QMap<QString,QVariant> leddel;
//    leddel.insert(Proxy_Transaction_Helper_VoucherNo,oldID);
//    leddel.insert(Proxy_Transaction_Helper_VoucherType,VoucherType);
//    db->deleteRecordsPlus(leddel,Proxy_Transaction_Helper_TableName);

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

    qDebug()<<"%%%%%%%%%%%%%5307"<<obj->ledgersList.size();

    LedgerMasterDataModel MainLedger;

    MainLedger.crAmount = obj->grandTotal;
    MainLedger.LedgerID = obj->ledgerObject.LedgerID;
    MainLedger.isInvoiceItem = true;
    MainLedger.mapList = obj->mapList;

    QString agDrID ;
    QString agCrID = obj->ledgerObject.LedgerID ;


    float  amt=0;
    for(int i=0;i<obj->ledgersList.size();i++)
    {
        obj->ledgersList[i].AgainstLedger = agCrID;
        if(obj->ledgersList[i].drAmount>amt)
            agDrID = obj->ledgersList[i].LedgerID;

        if(obj->paymentSplit)
            obj->ledgersList[i].isInvoiceItem = true;
    }

    if(!obj->paymentSplit)
        obj->ledgersList.insert(0,MainLedger);

    obj->voucherType = VoucherType;

    obj->ledgersList[0].AgainstLedger = "0x8x4";

    qDebug()<<"%%%%%%%%%%%%%5307"<<obj->ledgersList.size()<<"&&&&&&&&&&&&&&&";
    ledTransaction->deleteProxyTransaction(obj);
    ledTransaction->insertProxyTransaction(obj);


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

int PurchaseOrderDatabaseHelper::deleteVoucher(GeneralVoucherDataObject *obj)
{
    this->startTransaction();
    QMap<QString,QVariant> map3;
    map3.insert(Purchase_Order_Main_Voucher_No,obj->voucherNumber);
    map3.insert(Purchase_Order_Main_Voucher_Prefix, obj->VoucherPrefix);
    db->deleteRecordsPlus(map3,Purchase_Order_Main_TableName);

    map3.insert(Purchase_Order_Details_Voucher_Prefix,obj->VoucherPrefix);
    map3.insert(Purchase_Order_Details_Voucher_Prefix, obj->VoucherPrefix);
    db->deleteRecordsPlus(map3,Purchase_Order_Details_TableName);

    ledTransaction->deleteProxyTransaction(obj);


    return commit();

}

int PurchaseOrderDatabaseHelper::commit()
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

QSqlQueryModel *PurchaseOrderDatabaseHelper::getItemwisePendingOrders(QDate fromDate, QDate toDate)
{
    QString qStr = " SELECT " ;
    qStr += " (select item_name FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where item_id = " + Purchase_Order_Details_Inventory_Item_ID + ") as Item, ";
    qStr += " SUM(" + Purchase_Order_Details_Quantity + ") as Quantity FROM ";
    qStr += " ( SELECT " + Purchase_Order_Details_Inventory_Item_ID + " , " + Purchase_Order_Details_Quantity;
    qStr += " FROM " + Purchase_Order_Main_TableName + " main ";
    qStr += " JOIN " + Purchase_Order_Details_TableName + " det ";
    qStr += " ON main." + Purchase_Order_Main_Voucher_No + " = det." + Purchase_Order_Details_Voucher_No;
    qStr += " WHERE main.";
    qStr += Purchase_Order_Main_Voucher_Date + " <= " + db->getDateString(toDate);
    qStr += " AND main.";
    qStr += Purchase_Order_Main_Voucher_Date + " >= " + db->getDateString(fromDate);
    qStr += " ) items GROUP BY " + Purchase_Order_Details_Inventory_Item_ID;
    qStr += " order by 1";
    return db->getQueryModel(qStr);
}


GeneralVoucherDataObject *PurchaseOrderDatabaseHelper::getVoucherByVoucherNoPtr(QString vNo, QString voucherPrefix)
{
    GeneralVoucherDataObject *obj = new GeneralVoucherDataObject;
    SalesInventoryItemDatabaseHelper *itemHelper = new SalesInventoryItemDatabaseHelper();
    LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper();

    QString qStrMain ="SELECT "
            + Purchase_Order_Main_Voucher_No + ", "
            + Purchase_Order_Main_Voucher_Prefix + ", "
            + Purchase_Order_Main_Date_Created + ", "
            + Purchase_Order_Main_Voucher_Date + ", "
            + Purchase_Order_Main_TimeStamp + ", "
            + Purchase_Order_Main_Ledger_ID + ", "
            + Purchase_Order_Main_Billing_Name + ", "
            + Purchase_Order_Main_Total + ", "
            + Purchase_Order_Main_Narration + ", "
            + Purchase_Order_Main_Added_By + ", "
            + Purchase_Order_Main_Delivery_Date + ", "
            + Purchase_Order_Main_Completion_Probability + ", "
            + Purchase_Order_Main_Credit_Period + ", "
            + Purchase_Order_Main_POC_Name + ", "
            + Purchase_Order_Main_POC_Phone + ", "
            + Purchase_Order_Main_POC_Email + ", "
            + Purchase_Order_Main_Project_ID + ", "
            + Purchase_Order_Main_Revision_Number + ", "
            + Purchase_Order_Main_Converted_To_Sales_Order + ", "
            + Purchase_Order_Main_Quotation_Dropped + ", "
            + Purchase_Order_Main_Quotation_Dropped_Reason + ", "
            + Purchase_Order_Main_Requirement_Voucher_No + ", "
            + Purchase_Order_Main_Salesman_ID + ", "
            + Purchase_Order_Main_LPO + ", "
            + Purchase_Order_Main_VoucherStatus + ", "
            + Purchase_Order_Main_ModeOfService + ", "
            + Purchase_Order_Main_Discount_In_Amount + ", "
            + Purchase_Order_Main_Discount_In_Percent + ", "
            + Purchase_Order_Main_VATAmount + ", "
            + Purchase_Order_Main_Contact_ID + ", "
            + Purchase_Order_Main_Gross_Total + ", "
            + Purchase_Order_Main_Reference + ", "
            + Purchase_Order_Main_Customer_Expecting_Date + ", "
            + Purchase_Order_Main_Currency + ", "
            + Purchase_Order_Main_BoxesCount+ ", "
            + Purchase_Order_Main_TotalWeight + ", "
            + Purchase_Order_Main_LRNo + ", "
            + Purchase_Order_Main_VoucherNoDisplay + ", "
            + Purchase_Order_Main_ReqVoucherList + ", "
            + Purchase_Order_Main_Origin + ", "
            + Purchase_Order_Main_TransactionId
            + " FROM " + Purchase_Order_Main_TableName
            + " WHERE " + Purchase_Order_Main_Voucher_No + "= :vno"
            + " AND " +  Purchase_Order_Main_Voucher_Prefix + "= :voucherPrefix";

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

    QString qStrDetail = "SELECT distinct "




            + Purchase_Order_Details_Voucher_No
            + ", " + Purchase_Order_Details_Voucher_Prefix
            + ", " + Purchase_Order_Details_Voucher_Date
            + ", " + Purchase_Order_Details_Inventory_Item_ID
            + ", " + Purchase_Order_Details_Quantity
            + ", " + Purchase_Order_Details_Price
            + ", " + Purchase_Order_Details_UOM_ID
            + ", subq.convalue , subq.UomName, subq.UomSymbol "
            + ", " + Purchase_Order_Details_PriceLevel
            + ", " + Purchase_Order_Details_Discount_Percent
            + ", " + Purchase_Order_Details_Discount_Amount
            + ", " + Purchase_Order_Details_Subtotal
            + ", " + Purchase_Order_Details_Item_Narration
            + ", " + Purchase_Order_Details_Item_Description
            + ", " + Purchase_Order_Details_Salesman_ID
            + ", " + Purchase_Order_Details_Requirement_ItemID
            + ", " + Purchase_Order_Details_Length
            + ", " + Purchase_Order_Details_Dimension
            + ", " + Purchase_Order_Details_itemVoucherStatus
            + ", " + Purchase_Order_Details_itemProductionStatus
            + ", " + Purchase_Order_Details_TimeDate
            + ", " + Purchase_Order_Details_isCompoundItem
            + ", " + Purchase_Order_Details_subItemOff_list_ID
            + ", "  + Purchase_Order_Details_TaxRate
            + ", " + Purchase_Order_Details_TaxAmount
            + " FROM " + Purchase_Order_Details_TableName+ " s LEFT OUTER JOIN "
            + UomDataBaseHelper::qStrUomForItemDetail + " as subq "
            + " ON s." + Purchase_Order_Details_Inventory_Item_ID + " = subq.itemID "
            + " AND s." + Purchase_Order_Details_UOM_ID + " = subq.UOM "
            + " WHERE " + Purchase_Order_Details_Voucher_No + "= :vno"
            + " AND " +  Purchase_Order_Details_Voucher_Prefix + "= :voucherPrefix";

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
            obj->ledgersList.append(ledger);
        }
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

            i++;
            item.taxRate = query.value(25).toFloat();
            item.taxAmount = query.value(26).toFloat();
            if(query.value(i++).toBool()||(query.value(i++).toInt()==0)){

                CompoundItemDataObject cObj;
                cObj.BaseItem = item;
                obj->InventoryItems.append(cObj);
//                qDebug()<<"332";
            }
            else{

                int itemSize = obj->InventoryItems.size();
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

        }
    }
    obj->voucherType = VoucherType;
    return obj;
}

QSqlQueryModel *PurchaseOrderDatabaseHelper::getVoucherListByDateModel(QDate dateFrom, QDate dateTo)
{
    LedgerMasterDatabaseHelper ledHelper;
    QString qStr ="SELECT ";
    qStr += "'V' , ";
    qStr += Purchase_Order_Main_Voucher_No +", ";
    qStr += "'" + VoucherType + "', " ;
    qStr += Purchase_Order_Main_Voucher_Prefix + ", ";
    qStr += Purchase_Order_Main_Voucher_Date+" as 'Voucher Date', ";
    //    qStr += " CONCAT(" + Purchase_Order_Main_Voucher_Prefix + ", '-', " + Purchase_Order_Main_Voucher_No+ ") as 'Voucher No', ";
    qStr += Purchase_Order_Main_Voucher_No +" as 'Voucher No', ";
    qStr += "(SELECT "+ledHelper.Ledger_Master_LedgerName + " FROM "+ ledHelper.Ledger_Master_TableName;
    qStr += " WHERE "+ledHelper.Ledger_Master_LedgerID + "= som.";
    qStr += Purchase_Order_Main_Ledger_ID + ") Ledger, ";
    qStr += Purchase_Order_Main_Total ;
    qStr += " FROM " + Purchase_Order_Main_TableName;
    qStr += " som WHERE ";
    qStr += Purchase_Order_Main_Voucher_Date;
    qStr += " >= '";
    qStr += dateFrom.toString("yyyy-MM-dd");
    qStr += "' AND ";
    qStr += Purchase_Order_Main_Voucher_Date;
    qStr += " <= '";
    qStr += dateTo.toString("yyyy-MM-dd");
    qStr += "'";

    qDebug()<<qStr;

    return DatabaseHelper::getQueryModel(qStr);

}

int PurchaseOrderDatabaseHelper::updateVoucherStatus(GeneralVoucherDataObject *voucher)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    this->startTransaction();
    QMap<QString,QVariant> map;
    QMap<QString,QVariant> clause;
    map.insert(Purchase_Order_Main_VoucherStatus,voucher->status);
    clause.insert(Purchase_Order_Main_Voucher_No,voucher->voucherNumber);
    clause.insert(Purchase_Order_Main_Voucher_Prefix,voucher->VoucherPrefix);
    db->updateRecordsPlus(map,clause,Purchase_Order_Main_TableName);
    return commit();

}

QString PurchaseOrderDatabaseHelper::getVoucherNumberByLedID(QString LedID, QDate Date)
{
    QString qStr;
    QString vchID = "-1";
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    qStr += "SELECT "+ Purchase_Order_Main_Voucher_No ;
    qStr += " FROM ";
    qStr += Purchase_Order_Main_TableName;
    qStr += " WHERE ";
    qStr += Purchase_Order_Main_VoucherStatus +"=:a0";
    qStr += " AND ";
    qStr += Purchase_Order_Main_Ledger_ID + "=:a1";
    qStr += " AND ";
    qStr += Purchase_Order_Main_Voucher_Date + "=:a2";

    //    qDebug()<<qStr;

    query.prepare(qStr);

    query.bindValue(":a0",QuotationStatus::SalesOrderCreated);
    query.bindValue(":a1",LedID);
    query.bindValue(":a2",Date);


    if(query.exec()){
        qDebug()<<"qexec";
        while(query.next()){
            vchID = query.value(0).toString();
        }

    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return vchID;
}

QSqlQueryModel *PurchaseOrderDatabaseHelper::getVoucherListByDateTimeModel(QDateTime dateFrom, QDateTime dateTo)
{

}


QSqlQueryModel *PurchaseOrderDatabaseHelper::getPendingPurchaseOrdersByDeliveryDate(QDateTime fromDate, QDateTime toDate)
{

    QString qStr = " SELECT " ;
    qStr += Purchase_Order_Main_Voucher_No + " as 'Voucher No', ";
    qStr += Purchase_Order_Main_Voucher_Prefix + ", ";
    qStr += Purchase_Order_Main_Voucher_Date + " as 'Booking Date', ";
    qStr += Purchase_Order_Main_Delivery_Date + " as 'Delivery Date', ";
    qStr += Purchase_Order_Main_POC_Name + " as 'Vendor Name', ";
    qStr += Purchase_Order_Main_Total + " as 'Total Amount' ";
    qStr += " FROM " + Purchase_Order_Main_TableName;
    qStr += " WHERE ";
    qStr += Purchase_Order_Main_Delivery_Date + " <= " + db->getDateTimeString(toDate);
    qStr += " AND ";
    qStr += Purchase_Order_Main_Delivery_Date + " >= " + db->getDateTimeString(fromDate);
    qStr += " AND " + Purchase_Order_Main_VoucherStatus + " < " + QString::number(QuotationStatus::PurchaseInvoiceRaised);
    qStr += " ORDER BY " + Purchase_Order_Main_Delivery_Date + ", " + Purchase_Order_Main_Voucher_No;

//    qDebug()<<qStr;

    return db->getQueryModel(qStr);
}
