#include "salesvoucherdatabasehelper.h"
#include "database/Settings/ConfigurationSettingsDatabaseHelper.h"
#include <QFile>
#include <QSqlRecord>
#include "database/inventory/measurements/measurementsdatabasehelper.h"

QString salesVoucherDatabaseHelper::Sales_Invoice_Main_TableName = "Sales_Invoice_Main";

QString salesVoucherDatabaseHelper::Sales_Invoice_Main_id = "_id";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_Voucher_No = "Voucher_No";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_Voucher_Prefix = "Voucher_Prefix";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_Date_Created = "Date_Created";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_Voucher_Date = "Voucher_Date";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_TimeStamp = "`TimeStamp`";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_TimeStampC = "TimeStamp";

QString salesVoucherDatabaseHelper::Sales_Invoice_Main_Last_Updated = "Last_Updated";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_Ledger_ID = "Ledger_ID";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_Billing_Name = "Billing_Name";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_Total = "Total";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_Narration = "Narration";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_Added_By = "Added_By";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_Delivery_Date = "Delivery_Date";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_Completion_Probability = "Completion_Probability";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_Credit_Period = "Credit_Period";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_POC_Name = "POC_Name";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_POC_Phone = "POC_Phone";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_POC_Email = "POC_Email";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_Project_ID = "Project_ID";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_Revision_Number = "Revision_Number";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_Converted_To_Sales_Order = "Converted_To_Sales_Order";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_Quotation_Dropped = "Quotation_Dropped";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_Quotation_Dropped_Reason = "Quotation_Dropped_Reason";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_Requirement_Voucher_No = "Requirement_Voucher_No";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_Salesman_ID = "Salesman_ID";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_LPORef="LPO_Ref";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_ModeOfService = "MODE_OF_SERVICE";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_Discount_In_Amount = "Discount_In_Amount";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_Discount_In_Percent = "Discount_In_Percent";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_VATAmount = "VAT_Amount";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_VoucherStatus = "VoucherStatus";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_Contact_ID = "ContactID";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_Gross_Total = "Gross_Total";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_Reference ="Reference";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_Customer_Expecting_Date = "Customer_Expected_Date";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_Currency = "Currency";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_BoxesCount = "Boxes_Count";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_TotalWeight = "Total_Weight";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_LRNo = "LR_No";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_VoucherNoDisplay = "Voucher_No_Display";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_ReqVoucherList = "Requirement_Voucher_List";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_Origin = "Origin";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_Round_Off = "Round_Off";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_GeneratedTransactionId = "TransactionId";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_PaymentSplit="PaymentSplit";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_Godown="PaymentCompleted";
QString salesVoucherDatabaseHelper::Sales_Invoice_Main_Location="Location";

salesVoucherDatabaseHelper::salesVoucherDatabaseHelper(QString connString)
{
    db = new DatabaseHelper(connString);
    bulkDB = new BulkLoadDatabaseHelper(connString);
    itemTransaction = new ItemTransactionDatabaseHelper(db);
    ItemModel = new ItemTransactionDataModel();
    ledTransaction = new TransactionHelper(db);
    godownHelper = new GodownDatabaseHelper(db);
}

salesVoucherDatabaseHelper::~salesVoucherDatabaseHelper()
{

}

int salesVoucherDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

QString salesVoucherDatabaseHelper::getNextVoucherNo(QString voucherPrefix)
{
    GeneralVoucherDatabaseHelper vHelper;
    QString voucherNumber = vHelper.getNextVoucherNoByType(VoucherType, voucherPrefix, Sales_Invoice_Main_TableName);
    return voucherNumber;

}

QPair<QString, QString> salesVoucherDatabaseHelper::getVoucherNoByTransactionId(QString transId)
{
    GeneralVoucherDatabaseHelper vHelper;
    return  vHelper.getVoucherNoByTransactionId(transId, Sales_Invoice_Main_TableName);

}

QPair<QString, QString> salesVoucherDatabaseHelper::getVoucherNoByReqVoucherNo(QString transId)
{
    GeneralVoucherDatabaseHelper vHelper;
    return  vHelper.getVoucherNoByReqVoucherNo(transId, Sales_Invoice_Main_TableName);

}

QSqlQueryModel *salesVoucherDatabaseHelper::getVoucherListByDateModel(QDate dateFrom, QDate dateTo)
{

}


int salesVoucherDatabaseHelper::insertVoucher(GeneralVoucherDataObject *obj)
{
    //    if(obj->InventoryItems.size() == 0)
    //        return 0;

    GeneralVoucherDatabaseHelper *vHelper = new GeneralVoucherDatabaseHelper;

    this->startTransaction();
    QMap<QString,QVariant> map;

    //    qDebug()<<"Phone : "<<obj->ledgerObject.ContactPersonNumber;

    //    obj->voucherNumber = QString::number(getMaxIDwithPrefix(obj->VoucherPrefix)+1);

    obj->voucherNumber = vHelper->setNextVoucherNoByType(VoucherType, obj->VoucherPrefix, Sales_Invoice_Main_TableName);
    //    if(obj->ledgerObject.dbName != "" && !obj->fromExternal && obj->sendFlag)
    if(obj->TransactionId.length()==0)
        obj->TransactionId=QString::number(QDateTime::currentDateTime().toTime_t()) + obj->VoucherPrefix + db->clientId;

    if(obj->measurement!=nullptr){
        obj->QuotationDroppedReason = obj->measurement->contactMeasureID;
    }

    map.insert(Sales_Invoice_Main_Voucher_No,obj->voucherNumber);
    map.insert(Sales_Invoice_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Sales_Invoice_Main_Date_Created,obj->VoucherDate);
    map.insert(Sales_Invoice_Main_Voucher_Date,obj->VoucherDate);
    map.insert(Sales_Invoice_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    map.insert(Sales_Invoice_Main_Billing_Name,obj->ledgerObject.LedgerName);
    map.insert(Sales_Invoice_Main_Total,obj->grandTotal);
    map.insert(Sales_Invoice_Main_Narration,obj->narration);
    map.insert(Sales_Invoice_Main_Added_By,obj->AddedBy);
    map.insert(Sales_Invoice_Main_Delivery_Date,obj->DeliveryDate);
    map.insert(Sales_Invoice_Main_Completion_Probability,obj->CompletionProbability);
    map.insert(Sales_Invoice_Main_Credit_Period,obj->ledgerObject.Credit_Period);
    map.insert(Sales_Invoice_Main_POC_Name,obj->ledgerObject.ContactPersonName);
    map.insert(Sales_Invoice_Main_POC_Phone,obj->Contact.PhoneNumber);
    map.insert(Sales_Invoice_Main_POC_Email,obj->ledgerObject.ContactPersonEmail);
    map.insert(Sales_Invoice_Main_Project_ID,obj->ProjectId);
    map.insert(Sales_Invoice_Main_Revision_Number,obj->RevisionNo);
    map.insert(Sales_Invoice_Main_Converted_To_Sales_Order,obj->ConvertedToSalesOrder);
    map.insert(Sales_Invoice_Main_Quotation_Dropped,obj->QuotationDropped);
    map.insert(Sales_Invoice_Main_Quotation_Dropped_Reason,obj->QuotationDroppedReason);
    map.insert(Sales_Invoice_Main_Requirement_Voucher_No,obj->RequirementVoucherNo);
    map.insert(Sales_Invoice_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Sales_Invoice_Main_ModeOfService,obj->ModeOfService);
    map.insert(Sales_Invoice_Main_Contact_ID,obj->Contact._id);
    map.insert(Sales_Invoice_Main_VATAmount,obj->taxTotalAmount);
    map.insert(Sales_Invoice_Main_VoucherStatus,obj->status);
    map.insert(Sales_Invoice_Main_Discount_In_Amount,obj->discountinAmount);
    map.insert(Sales_Invoice_Main_Discount_In_Percent,obj->discountPercent);
    map.insert(Sales_Invoice_Main_Gross_Total,obj->grossTotal);
    map.insert(Sales_Invoice_Main_Reference,obj->reference);
    map.insert(Sales_Invoice_Main_Round_Off,obj->roundOff);
    map.insert(Sales_Invoice_Main_LPORef,obj->LPO);
    map.insert(Sales_Invoice_Main_GeneratedTransactionId, obj->TransactionId);
    map.insert(Sales_Invoice_Main_TimeStampC, obj->timestamp);
    map.insert(Sales_Invoice_Main_Last_Updated,QDateTime::currentDateTime());
    map.insert(Sales_Invoice_Main_PaymentSplit,obj->paymentSplit);
    map.insert(Sales_Invoice_Main_BoxesCount, obj->numBoxes);
    map.insert(Sales_Invoice_Main_Godown,obj->fromGodownID);
    map.insert(Sales_Invoice_Main_Location,obj->Location);
    QJsonDocument doc(obj->ReqVoucherList);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    map.insert(Sales_Invoice_Main_ReqVoucherList, strJson);



    qint64 lastId= db->insertRecordsPlus(map,Sales_Invoice_Main_TableName);

    qDebug()<<lastId<<"last id";
    for(CompoundItemDataObject compitem:obj->InventoryItems){

        QMap<QString,QVariant> map1;
        inventoryItemDataModel item = compitem.BaseItem;
        if(item.ItemReqUuid == "X"){
            item.ItemReqUuid = db->getUUID();
        }
        qDebug()<<item.ItemName;
        map1.insert(Sales_Invoice_Details_Requirement_ItemID, item.ItemReqUuid);
        map1.insert(Sales_Invoice_Details_Voucher_No,obj->voucherNumber);
        map1.insert(Sales_Invoice_Details_Voucher_Prefix,obj->VoucherPrefix);
        map1.insert(Sales_Invoice_Details_Voucher_Date,obj->VoucherDate);
        map1.insert(Sales_Invoice_Details_Inventory_Item_ID,item.ItemID);
        map1.insert(Sales_Invoice_Details_Quantity,item.quantity);
        map1.insert(Sales_Invoice_Details_Price,item.price);
        map1.insert(Sales_Invoice_Details_PriceLevel,item.PriceLevel);
        qDebug()<<"175";
        map1.insert(Sales_Invoice_Details_isCompoundItem,item.IsCompoundItem);
        map1.insert(Sales_Invoice_Details_subItemOff_list_ID,0);
        map1.insert(Sales_Invoice_Details_Discount_Percent,item.discountPercentage);
        map1.insert(Sales_Invoice_Details_Discount_Amount,item.discountinAmount);
        map1.insert(Sales_Invoice_Details_Subtotal,item.subTotal);
        map1.insert(Sales_Invoice_Details_Project_ID,obj->ProjectId);
        map1.insert(Sales_Invoice_Details_Item_Narration,item.narration);
        map1.insert(Sales_Invoice_Details_Item_Description,item.ItemDescription);
        map1.insert(Sales_Invoice_Details_Salesman_ID,obj->SalesmanID);
        qDebug()<<"185";
        //        map1.insert(Sales_Invoice_Details_Requirement_ItemID,item.requirementItemIdOld);
        map1.insert(Sales_Invoice_Details_Requirement_ItemID,item.ItemReqUuid);
        map1.insert(Sales_Invoice_Details_TaxRate, item.taxRate);
        map1.insert(Sales_Invoice_Details_TaxAmount, item.taxAmount);
        map1.insert(Sales_Invoice_Details_Length,item.length);
        map1.insert(Sales_Invoice_Details_Time,QDateTime::currentDateTime());
        map1.insert(Sales_Invoice_Details_UomId, item.uomObject._id);

        //        qDebug()<<"Current Time ==== "<<QTime::currentTime();
        qDebug()<<"195";

        ItemModel = new ItemTransactionDataModel;

        bool checkForDN = ConfigurationSettingsDatabaseHelper::getValue(checkDNexists,false).toBool();
        if(checkForDN)
        {
            bool checkDNforItem = itemTransaction->hasRecordsForVoucherTypeByItemReqId(item.ItemReqUuid, GMVoucherTypes::getVoucherString(GMVoucherTypes::DeliveryNote));
            if(checkDNforItem)
            {
                qDebug()<<"delete if DN exists";
                GeneralVoucherDataObject* dnVoucher = new GeneralVoucherDataObject();
                dnVoucher = obj;
                dnVoucher->voucherType = GMVoucherTypes::getVoucherString(GMVoucherTypes::DeliveryNote);
                itemTransaction->deleteItemTransaction(dnVoucher);
            }
        }
        qDebug()<<"205";


        ItemModel->ItemID = item.ItemID;
        ItemModel->From_Ledger = item.defaultSalesLedgerID;
        ItemModel->To_Ledger = obj->ledgerObject.LedgerID;
        ItemModel->VoucherID = obj->voucherNumber;
        ItemModel->VoucherPrefix = obj->VoucherPrefix;
        ItemModel->VoucherDate = obj->VoucherDate;
        ItemModel->VoucherType = VoucherType;
        //        ItemModel->From_Location = "Godown";
        if(item.fromGodownID.length()==0)
            item.fromGodownID = godownHelper->getDefaultGodown();
        qDebug()<<item.fromGodownID;
        ItemModel->From_Location = item.fromGodownID;
        ItemModel->To_Location = "OUT";
        ItemModel->crQty = item.quantity;
        ItemModel->drQty = 0;
        ItemModel->crAmount = item.subTotal;
        ItemModel->itemReqId = item.ItemReqUuid;
        ItemModel->Narration = obj->narration;
        ItemModel->Price = item.price;

        itemTransaction->insertItemTransaction(ItemModel, item.godownList);

        int basid = db->insertRecordsPlus(map1,Sales_Invoice_Details_TableName);
        for(inventoryItemDataModel subitem:compitem.subItems){
            QMap<QString,QVariant> map2;
            if(item.ItemReqUuid == "X"){
                item.ItemReqUuid = db->getUUID();
                map2.insert(Sales_Invoice_Details_Requirement_ItemID, item.ItemReqUuid);
            }
            map2.insert(Sales_Invoice_Details_Voucher_No,obj->voucherNumber);
            map2.insert(Sales_Invoice_Details_Voucher_Prefix,obj->VoucherPrefix);
            map2.insert(Sales_Invoice_Details_Voucher_Date,obj->VoucherDate);
            map2.insert(Sales_Invoice_Details_Inventory_Item_ID,subitem.ItemID);
            map2.insert(Sales_Invoice_Details_Quantity,subitem.quantity);
            map2.insert(Sales_Invoice_Details_Price,subitem.price);
            map2.insert(Sales_Invoice_Details_PriceLevel,subitem.PriceLevel);
            map2.insert(Sales_Invoice_Details_isCompoundItem,subitem.IsCompoundItem);
            map2.insert(Sales_Invoice_Details_subItemOff_list_ID,basid);
            map2.insert(Sales_Invoice_Details_Discount_Percent,subitem.discountPercentage);
            map2.insert(Sales_Invoice_Details_Discount_Amount,subitem.discount);
            map2.insert(Sales_Invoice_Details_Subtotal,subitem.subTotal);
            map2.insert(Sales_Invoice_Details_Project_ID,obj->ProjectId);
            map2.insert(Sales_Invoice_Details_Item_Narration,subitem.narration);
            map2.insert(Sales_Invoice_Details_Item_Description,subitem.ItemDescription);
            map2.insert(Sales_Invoice_Details_Salesman_ID,obj->SalesmanID);
            //            map2.insert(Sales_Invoice_Details_Requirement_ItemID,subitem.requirementItemIdOld);
            map2.insert(Sales_Invoice_Details_Length,item.length);
            map2.insert(Sales_Invoice_Details_Time,QTime::currentTime());
            map2.insert(Sales_Invoice_Details_UomId, item.uomObject._id);
            map2.insert(Sales_Invoice_Details_TaxRate, item.taxRate);
            map2.insert(Sales_Invoice_Details_TaxAmount, item.taxAmount);
            db->insertRecordsPlus(map2,Sales_Invoice_Details_TableName);

        }

        QMap <QString,QVariant> ledMap;
        ledMap.insert(Transaction_Helper_VoucherType,VoucherType);
        ledMap.insert(Transaction_Helper_VoucherNo,obj->voucherNumber);
        ledMap.insert(Transaction_Helper_VoucherPrefix,obj->VoucherPrefix);
        ledMap.insert(Transaction_Helper_VoucherDate,obj->VoucherDate);
        ledMap.insert(Transaction_Helper_ProjectID,obj->ProjectId);
        ledMap.insert(Transaction_Helper_SalesManID,obj->SalesmanID);
        ledMap.insert(Transaction_Helper_TranactionAmount,compitem.subItemsTotal+compitem.BaseItem.subTotal);
        ledMap.insert(Transaction_Helper_DebitLedgerID,compitem.BaseItem.defaultSalesLedgerID);
        ledMap.insert(Transaction_Helper_CreditLedgerID,obj->ledgerObject.LedgerID);
        ledMap.insert(Transaction_Helper_Narration,obj->ledgerObject.narration);

        //        db->insertRecordsPlus(ledMap,Transaction_Helper_TableName);

    }

    //    itemTransaction->updateConsumedQtyForVoucher(obj);

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

        if(obj->paymentSplit)
            obj->ledgersList[i].isInvoiceItem = true;
    }

    if(!obj->paymentSplit)
        obj->ledgersList.insert(0,MainLedger);

    obj->voucherType = VoucherType;

    obj->ledgersList[0].AgainstLedger = "0x7x5";

    ledTransaction->insertTransaction(obj);

    //    qDebug()<<"255";
    //    if(commit()){
    //        return lastId;
    //    }
    //    return 0;
    //    qDebug()<<obj->ObjToJson(obj);

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
    else{
        //        qDebug()<<obj->TransactionId;
    }
    //    qDebug()<<"277";
    //    qDebug()<<obj->ObjToJson(obj);
    return c;

}



int salesVoucherDatabaseHelper::updateVoucher(GeneralVoucherDataObject *obj, QString oldID)
{
    //    if(obj->InventoryItems.size() == 0)
    //        return 0;

    //    qDebug()<< "Old Id =" <<oldID<<QTime::currentTime();
    this->startTransaction();
    if(obj->measurement!=nullptr){
        obj->QuotationDroppedReason = obj->measurement->contactMeasureID;
    }


    QMap<QString,QVariant> map;
    map.insert(Sales_Invoice_Main_Voucher_No,obj->voucherNumber);
    map.insert(Sales_Invoice_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Sales_Invoice_Main_Date_Created,obj->VoucherDate);
    map.insert(Sales_Invoice_Main_Voucher_Date,obj->VoucherDate);
    map.insert(Sales_Invoice_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    map.insert(Sales_Invoice_Main_Billing_Name,obj->ledgerObject.LedgerName);
    map.insert(Sales_Invoice_Main_Total,obj->grandTotal);
    map.insert(Sales_Invoice_Main_Narration,obj->narration);
    map.insert(Sales_Invoice_Main_Added_By,obj->AddedBy);
    map.insert(Sales_Invoice_Main_Delivery_Date,obj->DeliveryDate);
    map.insert(Sales_Invoice_Main_Completion_Probability,obj->CompletionProbability);
    map.insert(Sales_Invoice_Main_Credit_Period,obj->ledgerObject.Credit_Period);
    map.insert(Sales_Invoice_Main_POC_Name,obj->ledgerObject.ContactPersonName);
    map.insert(Sales_Invoice_Main_POC_Phone,obj->Contact.PhoneNumber);
    map.insert(Sales_Invoice_Main_POC_Email,obj->ledgerObject.ContactPersonEmail);
    map.insert(Sales_Invoice_Main_Project_ID,obj->ProjectId);
    map.insert(Sales_Invoice_Main_Revision_Number,obj->RevisionNo);
    map.insert(Sales_Invoice_Main_Converted_To_Sales_Order,obj->ConvertedToSalesOrder);
    map.insert(Sales_Invoice_Main_Quotation_Dropped,obj->QuotationDropped);
    map.insert(Sales_Invoice_Main_Quotation_Dropped_Reason,obj->QuotationDroppedReason);
    map.insert(Sales_Invoice_Main_Requirement_Voucher_No,obj->RequirementVoucherNo);
    map.insert(Sales_Invoice_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Sales_Invoice_Main_ModeOfService,obj->ModeOfService);
    map.insert(Sales_Invoice_Main_Contact_ID,obj->Contact._id);
    map.insert(Sales_Invoice_Main_VATAmount,obj->taxTotalAmount);
    map.insert(Sales_Invoice_Main_VoucherStatus,obj->status);
    map.insert(Sales_Invoice_Main_Discount_In_Amount,obj->discountinAmount);
    map.insert(Sales_Invoice_Main_Discount_In_Percent,obj->discountPercent);
    map.insert(Sales_Invoice_Main_Gross_Total,obj->grossTotal);
    map.insert(Sales_Invoice_Main_Reference,obj->reference);
    map.insert(Sales_Invoice_Main_Round_Off,obj->roundOff);
    map.insert(Sales_Invoice_Main_LPORef,obj->LPO);
    map.insert(Sales_Invoice_Main_GeneratedTransactionId, obj->TransactionId);
    map.insert(Sales_Invoice_Main_Last_Updated,QDateTime::currentDateTime());
    map.insert(Sales_Invoice_Main_PaymentSplit,obj->paymentSplit);
    map.insert(Sales_Invoice_Main_BoxesCount, obj->numBoxes);
    map.insert(Sales_Invoice_Main_Godown,obj->fromGodownID);
    map.insert(Sales_Invoice_Main_Location,obj->Location);
    QJsonDocument doc(obj->ReqVoucherList);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    map.insert(Sales_Invoice_Main_ReqVoucherList, strJson);

    QMap<QString,QVariant> map3;
    map3.insert(Sales_Invoice_Main_Voucher_No,obj->voucherNumber);
    map3.insert(Sales_Invoice_Main_Voucher_Prefix,obj->VoucherPrefix);
    db->updateRecordsPlus(map,map3,Sales_Invoice_Main_TableName);

    //    qDebug()<<"main updated"<<QTime::currentTime();

    QMap<QString,QVariant> maptxndel;
    maptxndel.insert(Transaction_Helper_VoucherNo,oldID);
    maptxndel.insert(Transaction_Helper_VoucherType,VoucherType);
    //    db->deleteRecordsPlus(maptxndel,Transaction_Helper_TableName);

    QMap<QString,QVariant> map6;
    map6.insert(Sales_Invoice_Details_Voucher_No,oldID);
    map6.insert(Sales_Invoice_Details_Voucher_Prefix,obj->VoucherPrefix);
    db->deleteRecordsPlus(map6,Sales_Invoice_Details_TableName);

    //    qDebug()<<"det delete "<<QTime::currentTime();

    itemTransaction->deleteItemTransaction(obj);

    //    qDebug()<<"delete trans"<<QTime::currentTime();

    for(CompoundItemDataObject compitem:obj->InventoryItems){
        QMap<QString,QVariant> map1;
        inventoryItemDataModel item = compitem.BaseItem;
        if(item.ItemReqUuid == "X"){
            item.ItemReqUuid = db->getUUID();
        }
        map1.insert(Sales_Invoice_Details_Voucher_No,obj->voucherNumber);
        map1.insert(Sales_Invoice_Details_Voucher_Prefix,obj->VoucherPrefix);
        map1.insert(Sales_Invoice_Details_Voucher_Date,obj->VoucherDate);
        map1.insert(Sales_Invoice_Details_Inventory_Item_ID,item.ItemID);
        map1.insert(Sales_Invoice_Details_Quantity,item.quantity);
        map1.insert(Sales_Invoice_Details_Price,item.price);
        map1.insert(Sales_Invoice_Details_PriceLevel,item.PriceLevel);
        map1.insert(Sales_Invoice_Details_isCompoundItem,item.IsCompoundItem);
        map1.insert(Sales_Invoice_Details_subItemOff_list_ID,0);
        map1.insert(Sales_Invoice_Details_Discount_Percent,item.discountPercentage);
        map1.insert(Sales_Invoice_Details_Discount_Amount,item.discountinAmount);
        map1.insert(Sales_Invoice_Details_Subtotal,item.subTotal);
        map1.insert(Sales_Invoice_Details_Project_ID,obj->ProjectId);
        map1.insert(Sales_Invoice_Details_Item_Narration,item.narration);
        map1.insert(Sales_Invoice_Details_Item_Description,item.ItemDescription);
        map1.insert(Sales_Invoice_Details_Salesman_ID,obj->SalesmanID);
        map1.insert(Sales_Invoice_Details_Requirement_ItemID,item.ItemReqUuid);
        map1.insert(Sales_Invoice_Details_Length,item.length);
        map1.insert(Sales_Invoice_Details_Time,QTime::currentTime());
        map1.insert(Sales_Invoice_Details_UomId, item.uomObject._id);
        map1.insert(Sales_Invoice_Details_TaxRate, item.taxRate);
        map1.insert(Sales_Invoice_Details_TaxAmount, item.taxAmount);

        ItemModel->ItemID = item.ItemID;
        ItemModel->From_Ledger = item.defaultSalesLedgerID;
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
        ItemModel->itemReqId = item.ItemReqUuid;
        ItemModel->Narration = obj->narration;
        ItemModel->Price = item.price;

        itemTransaction->insertItemTransaction(ItemModel, item.godownList);
        //           qDebug()<<"insert trans"<<QTime::currentTime();
        qint64 basid = db->insertRecordsPlus(map1,Sales_Invoice_Details_TableName);
        //        qDebug()<<"insert det"<<QTime::currentTime();
        for(inventoryItemDataModel subitem:compitem.subItems){
            QMap<QString,QVariant> map2;
            map2.insert(Sales_Invoice_Details_Voucher_No,obj->voucherNumber);
            map2.insert(Sales_Invoice_Details_Voucher_Prefix,obj->VoucherPrefix);
            map2.insert(Sales_Invoice_Details_Voucher_Date,obj->VoucherDate);
            map2.insert(Sales_Invoice_Details_Inventory_Item_ID,subitem.ItemID);
            map2.insert(Sales_Invoice_Details_Quantity,subitem.quantity);
            map2.insert(Sales_Invoice_Details_Price,subitem.price);
            map2.insert(Sales_Invoice_Details_PriceLevel,subitem.PriceLevel);
            map2.insert(Sales_Invoice_Details_isCompoundItem,subitem.IsCompoundItem);
            map2.insert(Sales_Invoice_Details_subItemOff_list_ID,basid);
            map2.insert(Sales_Invoice_Details_Discount_Percent,subitem.discountPercentage);
            map2.insert(Sales_Invoice_Details_Discount_Amount,subitem.discount);
            map2.insert(Sales_Invoice_Details_Subtotal,subitem.subTotal);
            map2.insert(Sales_Invoice_Details_Project_ID,obj->ProjectId);
            map2.insert(Sales_Invoice_Details_Item_Narration,subitem.narration);
            map2.insert(Sales_Invoice_Details_Item_Description,subitem.ItemDescription);
            map2.insert(Sales_Invoice_Details_Salesman_ID,obj->SalesmanID);
            map2.insert(Sales_Invoice_Details_Requirement_ItemID,subitem.ItemReqUuid);
            map2.insert(Sales_Invoice_Details_Length,item.length);
            map2.insert(Sales_Invoice_Details_Time,QTime::currentTime());
            map2.insert(Sales_Invoice_Details_UomId, item.uomObject._id);

            db->insertRecordsPlus(map2,Sales_Invoice_Details_TableName);
        }


    }

    //    itemTransaction->updateConsumedQtyForVoucher(obj);

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

        if(obj->paymentSplit)
            obj->ledgersList[i].isInvoiceItem = true;
    }

    if(!obj->paymentSplit)
        obj->ledgersList.insert(0,MainLedger);

    obj->voucherType = VoucherType;

    obj->ledgersList[0].AgainstLedger = "0x7x5";
    //    qDebug()<<"253";

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

int salesVoucherDatabaseHelper::deleteVoucher(GeneralVoucherDataObject *obj)
{
    this->startTransaction();
    QMap<QString,QVariant> map3;
    map3.insert(Sales_Invoice_Main_Voucher_No,obj->voucherNumber);
    map3.insert(Sales_Invoice_Main_Voucher_Prefix, obj->VoucherPrefix);
    db->deleteRecordsPlus(map3,Sales_Invoice_Main_TableName);

    QMap<QString,QVariant> map4;
    map4.insert(Sales_Invoice_Details_Voucher_No,obj->voucherNumber);
    map4.insert(Sales_Invoice_Details_Voucher_Prefix, obj->VoucherPrefix);
    db->deleteRecordsPlus(map4,Sales_Invoice_Details_TableName);

    itemTransaction->deleteItemTransaction(obj);

    ledTransaction->deleteTransaction(obj);

    return commit();
}

int salesVoucherDatabaseHelper::commit()
{
    if(flag){
        if(db->commitTransaction())
            return true;
        else
            return false;
    }
    else{
        //        qDebug()<<"Rollback from salesVoucher";
        db->rollBackTransaction();
        flag=true;
        return 0;
    }
    return 0;
}

int salesVoucherDatabaseHelper::getMaxID()
{
    QString qStr = "SELECT MAX("+Sales_Invoice_Main_id+") FROM " + Sales_Invoice_Main_TableName;

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

int salesVoucherDatabaseHelper::getMaxIDwithPrefix(QString prefix)
{
    QString qStr = "SELECT MAX("+Sales_Invoice_Main_Voucher_No+") FROM " + Sales_Invoice_Main_TableName;
    qStr += " WHERE "+Sales_Invoice_Main_Voucher_Prefix + "='"+prefix+"'";
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

GeneralVoucherDataObject *salesVoucherDatabaseHelper::getVoucherByVoucherNoPtr(QString vNo, QString voucherPrefix)
{
    //    qDebug()<<"get voucher";
    GeneralVoucherDataObject *obj = new GeneralVoucherDataObject();
    SalesInventoryItemDatabaseHelper *itemHelper = new SalesInventoryItemDatabaseHelper();
    LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper();
    AddressBookDatabaseHelper *addHelper = new AddressBookDatabaseHelper();
    UomDataBaseHelper *uomHelper = new UomDataBaseHelper();

    QString qStrMain ="SELECT "
                       //            + Sales_Order_Main_id + ", "
                       + Sales_Invoice_Main_Voucher_No + ", "
                       + Sales_Invoice_Main_Voucher_Prefix + ", "
                       + Sales_Invoice_Main_Date_Created + ", "
                       + Sales_Invoice_Main_Voucher_Date + ", "
                       + Sales_Invoice_Main_TimeStamp + ", "
                       + Sales_Invoice_Main_Ledger_ID + ", "
                       + Sales_Invoice_Main_Billing_Name + ", "
                       + Sales_Invoice_Main_Total + ", "
                       + Sales_Invoice_Main_Narration + ", "
                       + Sales_Invoice_Main_Added_By + ", "
                       + Sales_Invoice_Main_Delivery_Date + ", "
                       + Sales_Invoice_Main_Completion_Probability + ", "
                       + Sales_Invoice_Main_Credit_Period + ", "
                       + Sales_Invoice_Main_POC_Name + ", "
                       + Sales_Invoice_Main_POC_Phone + ", "
                       + Sales_Invoice_Main_POC_Email + ", "
                       + Sales_Invoice_Main_Project_ID + ", "
                       + Sales_Invoice_Main_Revision_Number + ", "
                       + Sales_Invoice_Main_Converted_To_Sales_Order + ", "
                       + Sales_Invoice_Main_Quotation_Dropped + ", "
                       + Sales_Invoice_Main_Quotation_Dropped_Reason + ", "
                       + Sales_Invoice_Main_Requirement_Voucher_No + ", "
                       + Sales_Invoice_Main_Salesman_ID + ", "
                       + Sales_Invoice_Main_LPORef + ", "
                       + Sales_Invoice_Main_VoucherStatus + ", "
                       + Sales_Invoice_Main_ModeOfService + ", "
                       + Sales_Invoice_Main_Discount_In_Amount + ", "
                       + Sales_Invoice_Main_Discount_In_Percent + ", "
                       + Sales_Invoice_Main_VATAmount + ", "
                       + Sales_Invoice_Main_Contact_ID + ", "
                       + Sales_Invoice_Main_Gross_Total + ", "
                       + Sales_Invoice_Main_Reference + ", "
                       + Sales_Invoice_Main_Customer_Expecting_Date + ", "
                       + Sales_Invoice_Main_Currency + ", "
                       + Sales_Invoice_Main_BoxesCount+ ", "
                       + Sales_Invoice_Main_TotalWeight + ", "
                       + Sales_Invoice_Main_LRNo + ", "
                       + Sales_Invoice_Main_VoucherNoDisplay + ", "
                       + Sales_Invoice_Main_ReqVoucherList + ", "
                       + Sales_Invoice_Main_Origin + ", "
                       + Sales_Invoice_Main_GeneratedTransactionId+", "
                       + Sales_Invoice_Main_PaymentSplit+", "
                       + Sales_Invoice_Main_Godown +", "
                       + Sales_Invoice_Main_Location
                       + " FROM " + Sales_Invoice_Main_TableName
                       + " WHERE " + Sales_Invoice_Main_Voucher_No + "= :vno"
                       + " AND " +  Sales_Invoice_Main_Voucher_Prefix + "= :voucherPrefix";

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
                         + Sales_Invoice_Details_id
                         + ", " + Sales_Invoice_Details_Voucher_No
                         + ", " + Sales_Invoice_Details_Voucher_Prefix
                         + ", " + Sales_Invoice_Details_Voucher_Date
                         + ", " + Sales_Invoice_Details_Inventory_Item_ID
                         + ", " + Sales_Invoice_Details_Quantity
                         + ", " + Sales_Invoice_Details_Price
                         + ", " + Sales_Invoice_Details_UomId
                         + ", subq.convalue , subq.UomName, subq.UomSymbol "
                         + ", " + Sales_Invoice_Details_PriceLevel
                         + ", " + Sales_Invoice_Details_Discount_Percent
                         + ", " + Sales_Invoice_Details_Discount_Amount
                         + ", " + Sales_Invoice_Details_Subtotal
                         + ", " + Sales_Invoice_Details_Item_Narration
                         + ", " + Sales_Invoice_Details_Item_Description
                         + ", " + Sales_Invoice_Details_Salesman_ID
                         + ", " + Sales_Invoice_Details_Requirement_ItemID
                         + ", " + Sales_Invoice_Details_Length
                         + ", " + Sales_Invoice_Details_Dimension
                         + ", " + Sales_Invoice_Details_itemVoucherStatus
                         + ", " + Sales_Invoice_Details_itemProductionStatus
                         + ", " + Sales_Invoice_Details_Time
                         + ", " + Sales_Invoice_Details_isCompoundItem
                         + ", " + Sales_Invoice_Details_subItemOff_list_ID
                         + ", "  + Sales_Invoice_Details_TaxRate
                         + ", " +  Sales_Invoice_Details_TaxAmount
                         + " FROM " + Sales_Invoice_Details_TableName+ " s LEFT OUTER JOIN "
                         + UomDataBaseHelper::qStrUomForItemDetail + " as subq "
                         + " ON s." + Sales_Invoice_Details_Inventory_Item_ID + " = subq.itemID "
                         + " AND s." + Sales_Invoice_Details_UomId + " = subq.UOM "
                         + " WHERE " + Sales_Invoice_Details_Voucher_No + "= :vno"
                         + " AND " +  Sales_Invoice_Details_Voucher_Prefix + "= :voucherPrefix"
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
            //            qDebug()<<"at db helper Ledgers are : "<<ledger.LedgerName<<" : "<<ledger.drAmount<<ledger.crAmount<<ledger.isIndividual;
            if(!ledger.isInvoiceItem){
                //                qDebug()<<"Adding "<<ledger.LedgerName;
                obj->ledgersList.append(ledger);
            }
        }
    }

    query.prepare(qStrDetail);
    qDebug()<<Q_FUNC_INFO<<__LINE__<<qStrDetail;
    query.bindValue(":vno",vNo);
    query.bindValue(":voucherPrefix",voucherPrefix);
    if(!query.exec()){
        qDebug()<<query.lastQuery();
        qDebug()<<query.lastError()<<"detail query&&&";
    }
    else{
        if(query.size()>0) {
            while(query.next()){
                //                int i=3;
                QSqlRecord record = query.record();
                inventoryItemDataModel item = itemHelper->getInventoryItemByID(query.value(record.indexOf(Sales_Invoice_Details_Inventory_Item_ID)).toString(),false);
                item.quantity = query.value(record.indexOf(Sales_Invoice_Details_Quantity)).toFloat();
                item.prevQty = item.quantity;
                item.price = query.value(record.indexOf(Sales_Invoice_Details_Price)).toFloat();
                item.uomObject._id = query.value(record.indexOf("Uom_Id")).toString();
                item.uomObject.convRate = query.value(record.indexOf("convalue")).toFloat();
                item.uomObject.UomName = query.value(record.indexOf("UomName")).toString();
                item.uomObject.UomSymbol = query.value(record.indexOf("UomSymbol")).toString();
                item.PriceLevel = query.value(record.indexOf(Sales_Invoice_Details_PriceLevel)).toString();
                item.discountPercentage = query.value(record.indexOf(Sales_Invoice_Details_Discount_Percent)).toFloat();
                item.discountinAmount = query.value(record.indexOf(Sales_Invoice_Details_Discount_Amount)).toFloat();
                item.subTotal = query.value(record.indexOf(Sales_Invoice_Details_Subtotal)).toFloat();
                item.narration = query.value(record.indexOf(Sales_Invoice_Details_Item_Narration)).toString();
                item.ItemDescription = query.value(record.indexOf(Sales_Invoice_Details_Item_Description)).toString();
                item.salesMan_id  = query.value(record.indexOf(Sales_Invoice_Details_Salesman_ID)).toString();
                item.ItemReqUuid = query.value(record.indexOf(Sales_Invoice_Details_Requirement_ItemID)).toString();
                item.length = query.value(record.indexOf(Sales_Invoice_Details_Length)).toFloat();
                item.Dimension = query.value(record.indexOf(Sales_Invoice_Details_Dimension)).toString();
                item.itemVoucherStatus = query.value(record.indexOf(Sales_Invoice_Details_itemVoucherStatus)).toInt();
                item.itemProductionStatus = query.value(record.indexOf(Sales_Invoice_Details_itemProductionStatus)).toInt();
                item.godownList = itemTransaction->getGodownAllocationByVoucherAndItem
                                  (vNo, obj->voucherType, voucherPrefix,item.ItemReqUuid);

                if(item.godownList.length()==1){
                    item.toGodownID = item.godownList[0].toGodown;
                    item.fromGodownID = item.godownList[0].fromGodown;
                    //                    item.fifoList.clear();
                }
                if(obj->InventoryItems.size() == 0){
                    obj->fromGodownID = item.fromGodownID;
                    obj->toGodownID = item.toGodownID;
                }

                //                i++;
                item.taxRate   = query.value(record.indexOf(Sales_Invoice_Details_TaxRate)).toFloat();
                item.taxAmount = query.value(record.indexOf(Sales_Invoice_Details_TaxAmount)).toFloat();
                //            qDebug()<<"TAX : "<<item.taxRate<<item.taxAmount;
                if(query.value(record.indexOf(Sales_Invoice_Details_isCompoundItem)).toBool()||(query.value(record.indexOf(Sales_Invoice_Details_subItemOff_list_ID)).toInt()==0)){
                    CompoundItemDataObject cObj;
                    cObj.BaseItem = item;
                    obj->InventoryItems.append(cObj);
                }
                else{
                    qDebug() << "Adding SubItem";
                    int itemSize = obj->InventoryItems.size();
                    obj->InventoryItems[itemSize-1].subItems.append(item);
                    obj->InventoryItems[itemSize-1].subItemsTotal +=item.subTotal;
                }



            }
        }
    }

    qDebug()<<"inventory items : ";
    for(int i=0; i< obj->InventoryItems.size(); i++)
        qDebug()<<obj->InventoryItems[i].BaseItem.ItemName;

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
            obj->Contact.PhoneNumber = obj->POCPhone;
            obj->Contact.ContactName = obj->POCName;
            obj->Contact.email = obj->POCEmail;
            obj->ledgerObject.ContactPersonNumber = obj->POCPhone;
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
            QString reqVlist = query.value(i++).toString();
            QJsonDocument doc = QJsonDocument::fromJson(reqVlist.toUtf8());
            obj->ReqVoucherList = doc.array();
            obj->Origin = query.value(i++).toInt();
            obj->TransactionId = query.value(i++).toString();
            obj->paymentSplit = query.value(i++).toBool();
            obj->fromGodownID = query.value(i++).toString();
            //            query.value(i++).toString();
            obj->Location =  query.value(i++).toString();
            if(obj->paymentSplit)
                obj->paymentSplit = false;
            qDebug()<<Q_FUNC_INFO<<__LINE__<<obj->fromGodownID<<obj->TransactionId;
            if(obj->InventoryItems.length()>0 ){
                if(obj->InventoryItems[0].BaseItem.PriceLevel.length() > 0)
                    obj->priceListId = obj->InventoryItems[0].BaseItem.PriceLevel;
                else
                    obj->priceListId = QString::number(obj->ModeOfService);
            }
        }
    }

    if(obj->POCPhone.length()>0){
        qDebug()<<"Fetching Contact";
        obj->Contact = *(addHelper->getContactPtrByPhone(obj->POCPhone));
        //        QString qStr = "Select `name`,`address`, `route` from customer_details where phone ='"+obj->POCPhone+"'";
        //        if(query.exec(qStr))
        //            while(query.next()){
        //                obj->Contact.ContactName = query.value(0).toString();
        //                obj->Contact.address = query.value(1).toString();
        //                obj->Contact.route = query.value(2).toString();
        //                obj->Contact.PhoneNumber = obj->POCPhone;
        //            }
        //        else
        //            qDebug()<<query.lastError();
        obj->Contact.address = obj->Location;
    }

    //    for(int i=0;i<obj->ledgersList.length();i++){
    //        qDebug()<<"Ledgers are : "<<obj->ledgersList[i].LedgerName<<" : "<<obj->ledgersList[i].drAmount<<obj->ledgersList[i].crAmount;
    //    }
    if(obj->QuotationDroppedReason.length()>0){

        MeasurementsDatabaseHelper dbhelper;
        ContactMeasurement *cm = new ContactMeasurement();
        cm->contactMeasureID = obj->QuotationDroppedReason;
        cm->contactID = obj->Contact.addressId;
        cm = dbhelper.getContactMeasurement(cm);
        obj->measurement = cm;
        qDebug()<<"read measurement";
    }


    return obj;
}


QSqlQueryModel *salesVoucherDatabaseHelper::getVoucherListByDateTimeasModel(QDateTime dateFrom, QDateTime dateTo,
                                                                            QString vPref)
{
    QSqlQueryModel *model = new QSqlQueryModel;

    QString timestamp = "concat("+Sales_Invoice_Main_Voucher_Date+",' ',CAST("
                        +Sales_Invoice_Main_TimeStamp+" AS TIME))";
    LedgerMasterDatabaseHelper led;
    UserProfileDatabaseHelper user;
    QString qStr ="SELECT cast("
                   + Sales_Invoice_Main_Voucher_No+ " as int) 'Bill No.', "
                   + Sales_Invoice_Main_Voucher_Prefix+ "  , "
                   + Sales_Invoice_Main_Voucher_Date+" 'Date', "
                   + "(SELECT "+led.Ledger_Master_LedgerName+" from "+led.Ledger_Master_TableName;
    qStr    += " where "+led.Ledger_Master_LedgerID +" = sal." + Sales_Invoice_Main_Ledger_ID+ ") Payment, "
            + Sales_Invoice_Main_Reference+ " , "
            + Sales_Invoice_Main_Converted_To_Sales_Order+ " 'SO.#' , "
            + "(SELECT "+user.Employee_Details_Name+ " from "+user.Employee_Details_TableName;
    qStr    += " where "+ user.Employee_Details_id +"= sal."+ Sales_Invoice_Main_Salesman_ID + ") Salesman, "
            + Sales_Invoice_Main_Total +" 'Amount' "
            + " FROM " + Sales_Invoice_Main_TableName +" sal "
            + " WHERE ";
    qStr += " ( ";
    qStr += timestamp + ">=" +db->getDateTimeString(dateFrom) + "";
    qStr += " and ";
    qStr += timestamp + "<= " +  db->getDateTimeString(dateTo) + "";
    qStr += ")";
    if(vPref!="%"){
        qStr += " AND "+Sales_Invoice_Main_Voucher_Prefix +" ='"+vPref+"'";
    }
    qStr += " order by 1";
    //    qStr += " order by ("+ Sales_Invoice_Main_TimeStamp +") desc";





    qDebug()<<qStr;

    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}


QSqlQueryModel *salesVoucherDatabaseHelper::getVoucherListByDateTimeModel(QDateTime dateFrom, QDateTime dateTo)
{
    QSqlQueryModel *model = new QSqlQueryModel;

    QString timestamp = "concat("+Sales_Invoice_Main_Voucher_Date+",' ',CAST("
                        +Sales_Invoice_Main_TimeStamp+" AS TIME))";
    LedgerMasterDatabaseHelper ledHelper;
    UserProfileDatabaseHelper user;


    QString qStr ="SELECT ";
    qStr += "'V' , ";
    qStr += Sales_Invoice_Main_Voucher_No +", ";
    qStr += "'" + VoucherType + "', " ;
    qStr += Sales_Invoice_Main_Voucher_Prefix + ", ";
    qStr += Sales_Invoice_Main_Voucher_Date+" as 'Voucher Date', ";
    //    qStr += " CONCAT(" + Sales_Invoice_Main_Voucher_Prefix + ", '-', " + Sales_Invoice_Main_Voucher_No+ ") as 'Voucher No', ";
    qStr += "CAST(";
    qStr += Sales_Invoice_Main_Voucher_No +" as UNSIGNED) as 'Voucher No', ";
    qStr += "(SELECT "+ledHelper.Ledger_Master_LedgerName + " FROM "+ ledHelper.Ledger_Master_TableName;
    qStr += " WHERE "+ledHelper.Ledger_Master_LedgerID + "= sim.";
    qStr += Sales_Invoice_Main_Ledger_ID + ") Ledger, ";
    qStr += Sales_Invoice_Main_Total ;
    qStr += " FROM " + Sales_Invoice_Main_TableName;
    qStr += " sim WHERE ";
    qStr += "str_to_date(" + timestamp+ ",'%Y-%m-%d %H:%i:%s')";
    qStr += " >= ";
    qStr += db->getDateTimeString( dateFrom);
    qStr += " AND ";
    qStr += "str_to_date(" + timestamp+ ",'%Y-%m-%d %H:%i:%s')";
    qStr += " <= ";
    qStr += db->getDateTimeString( dateTo);
    qStr += "";

    qDebug()<<qStr;

    model = DatabaseHelper::getQueryModel(qStr);
    //    model = DatabaseHelper::getQueryModelL(qStr);
    return model;
}

QString salesVoucherDatabaseHelper::getVoucherNumberByLedID(QString LedID, QDate Date)
{

}

QString salesVoucherDatabaseHelper::getVoucherListByDateTimeModelQStr(QDateTime dateFrom, QDateTime dateTo)
{
    QString timestamp = "concat("+Sales_Invoice_Main_Voucher_Date+",' ',CAST("
                        +Sales_Invoice_Main_TimeStamp+" AS TIME))";


    LedgerMasterDatabaseHelper ledHelper;
    UserProfileDatabaseHelper user;
    QString qStr ="SELECT ";
    qStr += "'V' , ";
    qStr += Sales_Invoice_Main_Voucher_No +", ";
    qStr += "'" + VoucherType + "', " ;
    qStr += Sales_Invoice_Main_Voucher_Prefix + ", ";
    qStr += Sales_Invoice_Main_Voucher_Date+" as 'Voucher Date', ";
    //    qStr += " CONCAT(" + Sales_Invoice_Main_Voucher_Prefix + ", '-', " + Sales_Invoice_Main_Voucher_No+ ") as 'Voucher No', ";
    qStr += "CAST(";
    qStr += Sales_Invoice_Main_Voucher_No +" as UNSIGNED) as 'Voucher No', ";
    qStr += "(SELECT "+ledHelper.Ledger_Master_LedgerName + " FROM "+ ledHelper.Ledger_Master_TableName;
    qStr += " WHERE "+ledHelper.Ledger_Master_LedgerID + "= sim.";
    qStr += Sales_Invoice_Main_Ledger_ID + ") Ledger, ";
    qStr += Sales_Invoice_Main_Total ;
    qStr += " FROM " + Sales_Invoice_Main_TableName;
    qStr += " sim WHERE ";
    qStr += "str_to_date(" + timestamp+ ",'%Y-%m-%d %H:%i:%s')";
    qStr += " >= ";
    qStr += db->getDateTimeString( dateFrom);
    qStr += " AND ";
    qStr += "str_to_date(" + timestamp+ ",'%Y-%m-%d %H:%i:%s')";
    qStr += " <= ";
    qStr += db->getDateTimeString( dateTo);
    qStr += "";
    return qStr;
}

QSqlQueryModel *salesVoucherDatabaseHelper::getListOfItemsByDate(QDateTime dateFrom, QDateTime dateTo, QStringList excludedVouchers)
{
    QString timestamp = "concat(main."+Sales_Invoice_Main_Voucher_Date+",' ',CAST(main."
                        +Sales_Invoice_Main_TimeStamp+" AS TIME))";
    SalesInventoryItemDatabaseHelper item;

    QString excludedVouchersList ;
    if(excludedVouchers.size() > 0){
        for(QString itemID:excludedVouchers)
            excludedVouchersList += "'" + itemID + "',";
        excludedVouchersList.truncate(excludedVouchersList.lastIndexOf(QChar(',')));
    }

    QString qStr = " SELECT " + Sales_Invoice_Details_Inventory_Item_ID + ", " ;
    qStr += " (SELECT " + item.Sales_Inventory_ItemName + " FROM " + item.Sales_Inventory_Table_Name;
    qStr += " WHERE " + item.Sales_Inventory_ItemId + " = " + Sales_Invoice_Details_Inventory_Item_ID + ") as Item, ";
    qStr += " COUNT(distinct det." + Sales_Invoice_Details_Voucher_No + ") as 'Num Of Bills', ";
    qStr += " SUM(" + Sales_Invoice_Details_Quantity + ") as Quantity, ";
    qStr += " ROUND(SUM(" + Sales_Invoice_Details_Subtotal + "),2) as 'Sub Total', ";
    qStr += " ROUND(SUM(" + Sales_Invoice_Details_Subtotal + "+" + Sales_Invoice_Details_TaxAmount + "),2) as 'Total Amount'";
    qStr += " FROM " + Sales_Invoice_Details_TableName + " det JOIN " + Sales_Invoice_Main_TableName + " main ";
    qStr += " ON det." + Sales_Invoice_Details_Voucher_No + " = main." + Sales_Invoice_Main_Voucher_No;
    qStr += " AND det." + Sales_Invoice_Details_Voucher_Prefix + " = main." + Sales_Invoice_Main_Voucher_Prefix;
    qStr += " WHERE ";
    qStr += "str_to_date(" + timestamp+ ",'%Y-%m-%d %H:%i:%s') >= " + db->getDateTimeString( dateFrom);
    qStr += " AND ";
    qStr += "str_to_date(" + timestamp+ ",'%Y-%m-%d %H:%i:%s') <= " + db->getDateTimeString( dateTo);
    qStr += " AND main." + Sales_Invoice_Main_Ledger_ID + " = '0x5x23x1' ";
    if(excludedVouchersList.size() > 0)
        qStr += " AND main." + Sales_Invoice_Main_Voucher_No + " not in (" + excludedVouchersList +") ";
    qStr += " AND main." + Sales_Invoice_Main_Voucher_No + "  in ( SELECT " + Sales_Invoice_Details_Voucher_No;
    qStr += " FROM " + Sales_Invoice_Details_TableName ;
    qStr += " WHERE ";
    qStr += "str_to_date(concat("+Sales_Invoice_Details_Voucher_Date+",' ',CAST("
            +Sales_Invoice_Details_Time+" AS TIME)),'%Y-%m-%d %H:%i:%s') >= " + db->getDateTimeString( dateFrom);
    qStr += " AND ";
    qStr += "str_to_date(concat("+Sales_Invoice_Details_Voucher_Date+",' ',CAST("
            +Sales_Invoice_Details_Time+" AS TIME)),'%Y-%m-%d %H:%i:%s') <= " + db->getDateTimeString( dateTo);
    qStr += " GROUP BY " + Sales_Invoice_Details_Voucher_No;
    qStr += " HAVING count(distinct " + Sales_Invoice_Details_Inventory_Item_ID +") > 1 ) ";
    qStr += " GROUP BY " + Sales_Invoice_Details_Inventory_Item_ID;
    qStr += " ORDER BY Item ";

    return DatabaseHelper::getQueryModel(qStr);

}

float salesVoucherDatabaseHelper::getTotalSalesByDate(QDateTime dateFrom, QDateTime dateTo, QStringList excludedVouchers)
{
    QString timestamp = "concat("+Sales_Invoice_Main_Voucher_Date+",' ',CAST("
                        +Sales_Invoice_Main_TimeStamp+" AS TIME))";

    QString excludedVouchersList ;
    if(excludedVouchers.size() > 0){
        for(QString itemID:excludedVouchers)
            excludedVouchersList += "'" + itemID + "',";
        excludedVouchersList.truncate(excludedVouchersList.lastIndexOf(QChar(',')));
    }

    QString qStr = " SELECT "  ;
    qStr += "SUM(" + Sales_Invoice_Main_Total + ")" ;
    qStr += " FROM " + Sales_Invoice_Main_TableName;
    qStr += " sim WHERE ";
    qStr += "str_to_date(" + timestamp+ ",'%Y-%m-%d %H:%i:%s')";
    qStr += " >= ";
    qStr += db->getDateTimeString( dateFrom);
    qStr += " AND ";
    qStr += "str_to_date(" + timestamp+ ",'%Y-%m-%d %H:%i:%s')";
    qStr += " <= ";
    qStr += db->getDateTimeString( dateTo);
    qStr += " AND sim." + Sales_Invoice_Main_Ledger_ID + " = '0x5x23x1' ";
    if(excludedVouchersList.size() > 0)
        qStr += " AND sim." + Sales_Invoice_Main_Voucher_No + " not in (" + excludedVouchersList +") ";
    qStr += " AND sim." + Sales_Invoice_Main_Voucher_No + " in (";
    qStr += " SELECT " + Sales_Invoice_Details_Voucher_No + " from " + Sales_Invoice_Details_TableName;
    qStr += " GROUP BY " + Sales_Invoice_Details_Voucher_No;
    qStr += " HAVING COUNT(DISTINCT " + Sales_Invoice_Details_Inventory_Item_ID + ") >1 )";
    qStr += "";

    return DatabaseHelper::getSingletonQueryResult(qStr).toFloat();
}

bool salesVoucherDatabaseHelper::deleteItemsFromVouchers(QStringList itemList, QDateTime dateFrom, QDateTime dateTo, QList<QPair<QString, QString> > voucherList)
{
    QFile File("featadjlog.txt");
    File.open(QFile::WriteOnly | QFile::Text | QFile::Append);

    QString functionName = Q_FUNC_INFO;
    QString errorData = "\nFunction : " + functionName + " at " + QTime::currentTime().toString();

    errorData += "\n Items: " ;
    for(int i=0; i<itemList.size(); i++)
        errorData += itemList[i] + ", ";
    errorData += "\n Vouchers "  ;
    for(int i=0; i<voucherList.size(); i++)
        errorData += voucherList[i].first + ", ";

    File.write(errorData.toUtf8());
    File.close();


    bulkDB->startTransaction();
    QString items ;
    for(QString itemID:itemList)
        items += "'" + itemID + "',";
    items.truncate(items.lastIndexOf(QChar(',')));

    QString vouchers ;
    for(QPair<QString, QString> vPair:voucherList)
        vouchers += "'" + vPair.first + "',";
    vouchers.truncate(vouchers.lastIndexOf(QChar(',')));

    QString timestamp = "concat(main."+Sales_Invoice_Main_Voucher_Date+",' ',CAST(main."
                        +Sales_Invoice_Main_TimeStamp+" AS TIME))";

    QString qStr = " DELETE det.* FROM " + Sales_Invoice_Details_TableName + " det JOIN " + Sales_Invoice_Main_TableName + " main ";
    qStr += " ON det." + Sales_Invoice_Details_Voucher_No + " = main." + Sales_Invoice_Main_Voucher_No;
    qStr += " AND det." + Sales_Invoice_Details_Voucher_Prefix + " = main." + Sales_Invoice_Main_Voucher_Prefix;
    qStr += " WHERE str_to_date(" + timestamp+ ",'%Y-%m-%d %H:%i:%s') >= " + db->getDateTimeString( dateFrom);
    qStr += " AND str_to_date(" + timestamp+ ",'%Y-%m-%d %H:%i:%s') <= " + db->getDateTimeString( dateTo);
    qStr += " AND " + Sales_Invoice_Details_Inventory_Item_ID + " in (" + items + ")";
    qStr += " AND main." + Sales_Invoice_Main_Voucher_No + " in (" + vouchers + ")";


    return bulkDB->runDML(qStr)["SUCCESS"].toBool();


}

bool salesVoucherDatabaseHelper::checkVouchersBeforeDelete(QStringList itemList, QDateTime dateFrom, QDateTime dateTo, QList<QPair<QString, QString> > voucherList)
{
    bulkDB->startTransaction();
    QString items ;
    for(QString itemID:itemList)
        items += "'" + itemID + "',";
    items.truncate(items.lastIndexOf(QChar(',')));

    QString vouchers ;
    for(QPair<QString, QString> vPair:voucherList)
        vouchers += "'" + vPair.first + "',";
    vouchers.truncate(vouchers.lastIndexOf(QChar(',')));

    QString timestamp = "concat(main."+Sales_Invoice_Main_Voucher_Date+",' ',CAST(main."
                        +Sales_Invoice_Main_TimeStamp+" AS TIME))";

    QString qStr = " SELECT det.* FROM " + Sales_Invoice_Details_TableName + " det JOIN " + Sales_Invoice_Main_TableName + " main ";
    qStr += " ON det." + Sales_Invoice_Details_Voucher_No + " = main." + Sales_Invoice_Main_Voucher_No;
    qStr += " AND det." + Sales_Invoice_Details_Voucher_Prefix + " = main." + Sales_Invoice_Main_Voucher_Prefix;
    qStr += " WHERE str_to_date(" + timestamp+ ",'%Y-%m-%d %H:%i:%s') >= " + db->getDateTimeString( dateFrom);
    qStr += " AND str_to_date(" + timestamp+ ",'%Y-%m-%d %H:%i:%s') <= " + db->getDateTimeString( dateTo);
    qStr += " AND " + Sales_Invoice_Details_Inventory_Item_ID + " not in (" + items + ")";
    qStr += " AND main." + Sales_Invoice_Main_Voucher_No + " in (" + vouchers + ")";



    return bulkDB->runDML(qStr)["SUCCESS"].toBool();


}

bool salesVoucherDatabaseHelper::deleteVouchers(QList<QPair<QString, QString> > voucherList)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    //    bulkDB->startTransaction();
    QString vouchers ;
    for(QPair<QString, QString> vPair:voucherList)
        vouchers += "'" + vPair.first + "',";
    vouchers.truncate(vouchers.lastIndexOf(QChar(',')));

    QString qStrMain = " DELETE main.* FROM " + Sales_Invoice_Main_TableName + " main ";
    qStrMain += " WHERE " + Sales_Invoice_Main_Voucher_No + " in (" + vouchers + ")";

    QString qStrDet = " DELETE det.* FROM " + Sales_Invoice_Details_TableName + " det ";
    qStrDet += " WHERE " + Sales_Invoice_Details_Voucher_No + " in (" + vouchers + ")";

    QString qStrTrans = " DELETE trans.* FROM " + Transaction_Helper_TableName + " trans ";
    qStrTrans += " WHERE " + Transaction_Helper_VoucherNo + " in (" + vouchers + ") and voucher_Type = 'SALESVOUCHER' ";

    QString qStrItemTrans = " DELETE trans.* FROM transaction_item_helper trans ";
    qStrItemTrans += " WHERE voucherID in (" + vouchers + ") and voucherType = 'SALESVOUCHER'";


    if(bulkDB->runDML(qStrMain)["SUCCESS"].toBool()){
        if(bulkDB->runDML(qStrDet)["SUCCESS"].toBool()){
            if(bulkDB->runDML(qStrTrans)["SUCCESS"].toBool())
                return bulkDB->runDML(qStrItemTrans)["SUCCESS"].toBool();
            else
                return false;
        }
        else return false;
    }
    else return false;

}

QList<QPair<QString, QString>> salesVoucherDatabaseHelper::getVoucherNosListByItems(QStringList itemList, QDateTime dateFrom, QDateTime dateTo, QStringList excludedVouchers)
{
    QList<QPair<QString, QString>> vList;

    QString voucherNoList;

    QString items ;
    for(QString itemID:itemList)
        items += "'" + itemID + "',";
    items.truncate(items.lastIndexOf(QChar(',')));

    QString excludedVouchersList ;
    if(excludedVouchers.size() > 0){
        for(QString itemID:excludedVouchers)
            excludedVouchersList += "'" + itemID + "',";
        excludedVouchersList.truncate(excludedVouchersList.lastIndexOf(QChar(',')));
    }

    QString timestamp = "concat(main."+Sales_Invoice_Main_Voucher_Date+",' ',CAST(main."
                        +Sales_Invoice_Main_TimeStamp+" AS TIME))";

    QString qStr = " SELECT distinct det." + Sales_Invoice_Details_Voucher_No + ", det." + Sales_Invoice_Details_Voucher_Prefix;
    qStr += " FROM " + Sales_Invoice_Details_TableName + " det JOIN " + Sales_Invoice_Main_TableName + " main ";
    qStr += " ON det." + Sales_Invoice_Details_Voucher_No + " = main." + Sales_Invoice_Main_Voucher_No;
    qStr += " AND det." + Sales_Invoice_Details_Voucher_Prefix + " = main." + Sales_Invoice_Main_Voucher_Prefix;
    qStr += " WHERE ";
    qStr += "str_to_date(" + timestamp+ ",'%Y-%m-%d %H:%i:%s') >= " + db->getDateTimeString( dateFrom);
    qStr += " AND ";
    qStr += "str_to_date(" + timestamp+ ",'%Y-%m-%d %H:%i:%s') <= " + db->getDateTimeString( dateTo);
    qStr += " AND " + Sales_Invoice_Details_Inventory_Item_ID + " in (" + items + ")";
    qStr += " AND main." + Sales_Invoice_Main_Ledger_ID + " = '0x5x23x1' ";
    if(excludedVouchersList.size() > 0)
        qStr += " AND main." + Sales_Invoice_Main_Voucher_No + " not in (" + excludedVouchersList +") ";
    qStr += " AND main." + Sales_Invoice_Main_Voucher_No + "  in ( SELECT " + Sales_Invoice_Details_Voucher_No;
    qStr += " FROM " + Sales_Invoice_Details_TableName + " GROUP BY " + Sales_Invoice_Details_Voucher_No;
    qStr += " HAVING count(distinct " + Sales_Invoice_Details_Inventory_Item_ID +") > 1 ) ";
    //    qStr += " GROUP BY " + Sales_Invoice_Details_Inventory_Item_ID;

    qDebug()<<qStr;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::dbString));

    if(query.exec(qStr)){
        while (query.next()) {
            //            QPair<QString, QString> vPair;
            //            vPair.first =  query.value(0).toString();
            //            vPair.second = query.value(1).toString();
            //            vList.append(vPair);
            voucherNoList += "'" + query.value(0).toString() + "',";
        }
    }
    else {
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }

    voucherNoList.truncate(voucherNoList.lastIndexOf(QChar(',')));

    qDebug()<<"&&& voucherNoList"<<voucherNoList;

    // check if voucher has items other than in list??
    QString qStr2 = " SELECT distinct main." + Sales_Invoice_Main_Voucher_No + " , main." + Sales_Invoice_Main_Voucher_Prefix
                    + " FROM " + Sales_Invoice_Details_TableName +
                    " det JOIN " + Sales_Invoice_Main_TableName + " main ";
    qStr2 += " ON det." + Sales_Invoice_Details_Voucher_No + " = main." + Sales_Invoice_Main_Voucher_No;
    qStr2 += " AND det." + Sales_Invoice_Details_Voucher_Prefix + " = main." + Sales_Invoice_Main_Voucher_Prefix;
    qStr2 += " WHERE str_to_date(" + timestamp+ ",'%Y-%m-%d %H:%i:%s') >= " + db->getDateTimeString( dateFrom);
    qStr2 += " AND str_to_date(" + timestamp+ ",'%Y-%m-%d %H:%i:%s') <= " + db->getDateTimeString( dateTo);
    qStr2 += " AND " + Sales_Invoice_Details_Inventory_Item_ID + " not in (" + items + ")";
    qStr2 += " AND main." + Sales_Invoice_Main_Voucher_No + " in (" + voucherNoList + ")";

    qDebug()<<qStr2;
    QSqlQuery query2(QSqlDatabase::database(DatabaseValues::dbString));
    if(query2.exec(qStr2)){
        while (query2.next()) {
            QPair<QString, QString> vPair;
            vPair.first =  query2.value(0).toString();
            vPair.second = query2.value(1).toString();
            vList.append(vPair);

        }
    }

    else {
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }

    return vList;

}

void salesVoucherDatabaseHelper::switchSalesMan(int newEmpID, QString vNO, QString vPrefix)
{
    QMap<QString,QVariant> map;
    QMap<QString,QVariant> clause;

    map.insert(Sales_Invoice_Main_Salesman_ID,newEmpID);
    clause.insert(Sales_Invoice_Main_Voucher_No,vNO);
    clause.insert(Sales_Invoice_Main_Voucher_Prefix,vPrefix);
    this->startTransaction();
    db->updateRecordsPlus(map,clause,Sales_Invoice_Main_TableName);
    this->commit();
}

QString salesVoucherDatabaseHelper::getActualVoucherByREFandDate(QString ref,QDate date)
{
    QString refno="0";

    QString qStr = "";

    qStr += "SELECT "+ Sales_Invoice_Main_Voucher_No + " from ";
    qStr += Sales_Invoice_Main_TableName + " WHERE ";
    qStr += Sales_Invoice_Main_LPORef + "='"+ ref +"' AND ";
    qStr += Sales_Invoice_Main_Voucher_Date + "='"+ date.toString("yyyy-MM-dd") +"'";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    //    qDebug()<<qStr;
    if(query.exec(qStr)){
        while(query.next()){
            refno = query.value(0).toString();
        }
    }
    else{
        qDebug()<<query.lastQuery();
        qDebug()<<query.lastError();
    }
    return refno;
}

QString salesVoucherDatabaseHelper::getActualVoucherByREFandDate(QString ref, QDateTime date)
{
    QString refno="0";

    QString qStr = "";

    qStr += "SELECT "+ Sales_Invoice_Main_Voucher_No + " from ";
    qStr += Sales_Invoice_Main_TableName + " WHERE ";
    qStr += Sales_Invoice_Main_LPORef + "='"+ ref +"' AND ";
    qStr += Sales_Invoice_Main_Voucher_Date + "='"+ date.toString("yyyy-MM-dd") +"'";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    if(query.exec(qStr)){
        while(query.next()){
            refno = query.value(0).toString();
        }
    }
    return refno;
}


int salesVoucherDatabaseHelper::prepareSalesVoucherInsert(GeneralVoucherDataObject *obj)
{
    this->startTransaction();
    QMap<QString,QVariant> map;

    QFile File("featadjlog.txt");
    File.open(QFile::WriteOnly | QFile::Text | QFile::Append);

    QString functionName = Q_FUNC_INFO;
    QString errorData = "\n Function : " + functionName + ". Voucher: " + obj->voucherNumber + ". Total is " + QString::number(obj->grandTotal);
    File.write(errorData.toUtf8());
    File.close();

    map.insert(Sales_Invoice_Main_Voucher_No,obj->voucherNumber);
    map.insert(Sales_Invoice_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Sales_Invoice_Main_Date_Created,obj->VoucherDate);
    map.insert(Sales_Invoice_Main_Voucher_Date,obj->VoucherDate);
    map.insert(Sales_Invoice_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    map.insert(Sales_Invoice_Main_Billing_Name,obj->ledgerObject.LedgerName);
    map.insert(Sales_Invoice_Main_Total,obj->grandTotal);
    map.insert(Sales_Invoice_Main_Narration,obj->narration);
    map.insert(Sales_Invoice_Main_Added_By,obj->AddedBy);
    map.insert(Sales_Invoice_Main_Delivery_Date,obj->DeliveryDate);
    map.insert(Sales_Invoice_Main_Completion_Probability,obj->CompletionProbability);
    map.insert(Sales_Invoice_Main_Credit_Period,obj->ledgerObject.Credit_Period);
    map.insert(Sales_Invoice_Main_POC_Name,obj->ledgerObject.ContactPersonName);
    map.insert(Sales_Invoice_Main_POC_Phone,obj->Contact.PhoneNumber);
    map.insert(Sales_Invoice_Main_POC_Email,obj->ledgerObject.ContactPersonEmail);
    map.insert(Sales_Invoice_Main_Project_ID,obj->ProjectId);
    map.insert(Sales_Invoice_Main_Revision_Number,obj->RevisionNo);
    map.insert(Sales_Invoice_Main_Converted_To_Sales_Order,obj->ConvertedToSalesOrder);
    map.insert(Sales_Invoice_Main_Quotation_Dropped,obj->QuotationDropped);
    map.insert(Sales_Invoice_Main_Quotation_Dropped_Reason,obj->QuotationDroppedReason);
    map.insert(Sales_Invoice_Main_Requirement_Voucher_No,obj->RequirementVoucherNo);
    map.insert(Sales_Invoice_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Sales_Invoice_Main_ModeOfService,obj->ModeOfService);
    map.insert(Sales_Invoice_Main_Contact_ID,obj->Contact._id);
    map.insert(Sales_Invoice_Main_VATAmount,obj->taxTotalAmount);
    map.insert(Sales_Invoice_Main_VoucherStatus,obj->status);
    map.insert(Sales_Invoice_Main_Discount_In_Amount,obj->discountinAmount);
    map.insert(Sales_Invoice_Main_Discount_In_Percent,obj->discountPercent);
    map.insert(Sales_Invoice_Main_Gross_Total,obj->grossTotal);
    map.insert(Sales_Invoice_Main_Reference,obj->reference);
    map.insert(Sales_Invoice_Main_Round_Off,obj->roundOff);
    map.insert(Sales_Invoice_Main_LPORef,obj->LPO);
    map.insert(Sales_Invoice_Main_GeneratedTransactionId, obj->TransactionId);
    map.insert(Sales_Invoice_Main_TimeStampC,obj->timestamp);
    map.insert(Sales_Invoice_Main_Last_Updated,QDateTime::currentDateTime());
    map.insert(Sales_Invoice_Main_PaymentSplit,obj->paymentSplit);

    QJsonDocument doc(obj->ReqVoucherList);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    map.insert(Sales_Invoice_Main_ReqVoucherList, strJson);

    qint64 lastId= bulkDB->createInsertQuery(map,Sales_Invoice_Main_TableName);

    for(CompoundItemDataObject compitem:obj->InventoryItems){
        QMap<QString,QVariant> map1;
        inventoryItemDataModel item = compitem.BaseItem;

        map1.insert(Sales_Invoice_Details_Requirement_ItemID, item.ItemReqUuid);
        map1.insert(Sales_Invoice_Details_Voucher_No,obj->voucherNumber);
        map1.insert(Sales_Invoice_Details_Voucher_Prefix,obj->VoucherPrefix);
        map1.insert(Sales_Invoice_Details_Voucher_Date,obj->VoucherDate);
        map1.insert(Sales_Invoice_Details_Inventory_Item_ID,item.ItemID);
        map1.insert(Sales_Invoice_Details_Quantity,item.quantity);
        map1.insert(Sales_Invoice_Details_Price,item.price);
        map1.insert(Sales_Invoice_Details_PriceLevel,item.PriceLevel);
        map1.insert(Sales_Invoice_Details_isCompoundItem,item.IsCompoundItem);
        map1.insert(Sales_Invoice_Details_subItemOff_list_ID,0);
        map1.insert(Sales_Invoice_Details_Discount_Percent,item.discountPercentage);
        map1.insert(Sales_Invoice_Details_Discount_Amount,item.discountinAmount);
        map1.insert(Sales_Invoice_Details_Subtotal,item.subTotal);
        map1.insert(Sales_Invoice_Details_Project_ID,obj->ProjectId);
        map1.insert(Sales_Invoice_Details_Item_Narration,item.narration);
        map1.insert(Sales_Invoice_Details_Item_Description,item.ItemDescription);
        map1.insert(Sales_Invoice_Details_Salesman_ID,obj->SalesmanID);
        map1.insert(Sales_Invoice_Details_Requirement_ItemID,item.ItemReqUuid);
        map1.insert(Sales_Invoice_Details_TaxRate, item.taxRate);
        map1.insert(Sales_Invoice_Details_TaxAmount, item.taxAmount);
        map1.insert(Sales_Invoice_Details_Length,item.length);
        map1.insert(Sales_Invoice_Details_Time,QDateTime::currentDateTime());
        map1.insert(Sales_Invoice_Details_UomId, item.uomObject._id);

        ItemModel = new ItemTransactionDataModel;
        if(itemTransaction->hasRecordsForVoucherTypeByItemReqId(item.ItemReqUuid, GMVoucherTypes::getVoucherString(GMVoucherTypes::DeliveryNote)))
        {
            GeneralVoucherDataObject* dnVoucher = new GeneralVoucherDataObject();
            dnVoucher = obj;
            dnVoucher->voucherType = GMVoucherTypes::getVoucherString(GMVoucherTypes::DeliveryNote);
            itemTransaction->deleteItemTransaction(dnVoucher);
        }

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

        itemTransaction->insertItemTransactionRecalc(ItemModel, item.godownList, bulkDB);


        int basid = bulkDB->createInsertQuery(map1,Sales_Invoice_Details_TableName);
        for(inventoryItemDataModel subitem:compitem.subItems){
            QMap<QString,QVariant> map2;
            if(item.ItemReqUuid == "X"){
                //                item.ItemReqUuid = db->getUUID();
                map2.insert(Sales_Invoice_Details_Requirement_ItemID, item.ItemReqUuid);
            }
            map2.insert(Sales_Invoice_Details_Voucher_No,obj->voucherNumber);
            map2.insert(Sales_Invoice_Details_Voucher_Prefix,obj->VoucherPrefix);
            map2.insert(Sales_Invoice_Details_Voucher_Date,obj->VoucherDate);
            map2.insert(Sales_Invoice_Details_Inventory_Item_ID,subitem.ItemID);
            map2.insert(Sales_Invoice_Details_Quantity,subitem.quantity);
            map2.insert(Sales_Invoice_Details_Price,subitem.price);
            map2.insert(Sales_Invoice_Details_PriceLevel,subitem.PriceLevel);
            map2.insert(Sales_Invoice_Details_isCompoundItem,subitem.IsCompoundItem);
            map2.insert(Sales_Invoice_Details_subItemOff_list_ID,basid);
            map2.insert(Sales_Invoice_Details_Discount_Percent,subitem.discountPercentage);
            map2.insert(Sales_Invoice_Details_Discount_Amount,subitem.discount);
            map2.insert(Sales_Invoice_Details_Subtotal,subitem.subTotal);
            map2.insert(Sales_Invoice_Details_Project_ID,obj->ProjectId);
            map2.insert(Sales_Invoice_Details_Item_Narration,subitem.narration);
            map2.insert(Sales_Invoice_Details_Item_Description,subitem.ItemDescription);
            map2.insert(Sales_Invoice_Details_Salesman_ID,obj->SalesmanID);
            //            map2.insert(Sales_Invoice_Details_Requirement_ItemID,subitem.requirementItemIdOld);
            map2.insert(Sales_Invoice_Details_Length,item.length);
            map2.insert(Sales_Invoice_Details_Time,QTime::currentTime());
            map2.insert(Sales_Invoice_Details_UomId, item.uomObject._id);
            map2.insert(Sales_Invoice_Details_TaxRate, item.taxRate);
            map2.insert(Sales_Invoice_Details_TaxAmount, item.taxAmount);
            bulkDB->createInsertQuery(map2,Sales_Invoice_Details_TableName);

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

    obj->ledgersList.insert(0,MainLedger);

    obj->voucherType = VoucherType;

    obj->ledgersList[0].AgainstLedger = agCrID;
    //    qDebug()<<"253";


    for(LedgerMasterDataModel ledger:obj->ledgersList){
        QMap<QString,QVariant> transmap;

        transmap.insert(Transaction_Helper_VoucherType,obj->voucherType);
        transmap.insert(Transaction_Helper_VoucherPrefix,obj->VoucherPrefix);
        transmap.insert(Transaction_Helper_VoucherNo,obj->voucherNumber);
        transmap.insert(Transaction_Helper_VoucherDate,obj->VoucherDate);
        transmap.insert(Transaction_Helper_ProjectID,obj->ProjectId);
        transmap.insert(Transaction_Helper_Narration,obj->narration);
        transmap.insert(Transaction_Helper_SalesManID,obj->SalesmanID);
        transmap.insert(Transaction_Helper_isAdditional,!(obj->ledgerObject.isInvoiceItem));
        transmap.insert(Transaction_Helper_TranactionAmount,ledger.amount);
        transmap.insert(Transaction_Helper_CreditLedgerID,ledger.LedgerID);
        transmap.insert(Transaction_Helper_DebitLedgerID,ledger.AgainstLedger);
        transmap.insert(Transaction_Helper_Debit_Amount,ledger.drAmount);
        transmap.insert(Transaction_Helper_Credit_Amount,ledger.crAmount);
        transmap.insert(Transaction_Helper_LedgerID,ledger.LedgerID);
        transmap.insert(Transaction_Helper_AgainstLedger,ledger.AgainstLedger);
        bulkDB->createInsertQuery(transmap,Transaction_Helper_TableName);
    }

    int c=commit();
    return c;

}

void salesVoucherDatabaseHelper::insertSalesVoucherRecalc(QList<QPair<QString, QString> > voucherList)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    //    bulkDB->writeQueryFiles();
    //    if(deleteVouchers(voucherList))
    //        bulkDB->readQueryFiles();
    if(deleteVouchers(voucherList))
        bulkDB->runStoredQueries();
}

QSqlQueryModel *salesVoucherDatabaseHelper::getSalesByItem(QString ItemID, QDate fromDate,QDate toDate)
{
    QString qStr;
    LedgerMasterDatabaseHelper led;
    // filtered by ledger id

    qStr = " SELECT date(main.VOUCHER_DATE) as 'Voucher Date' , cast(main.VOUCHER_NO as INteger) as 'Voucher No', "; //  VOUCHERTYPE as 'Voucher Type',   VOUCHERPREFIX as 'Voucher Prefix', ";
    qStr += " (SELECT Ledger_Name FROM " + led.Ledger_Master_TableName + " led where led.ledger_id = main.ledger_id)  as 'Ledger', ";
    qStr += " CAST(SUM(det.QUANTITY) AS DECIMAL(10,3)) as 'Quantity' ";
    qStr += " FROM sales_invoice_details det join sales_invoice_main main";
    qStr += " on det.voucher_no = main.voucher_no and det.voucher_prefix = main.voucher_prefix ";
    qStr += " WHERE det.inventory_item_id = '" + ItemID+ "'";
    qStr += " AND main.VOUCHER_DATE >= str_to_date('" + fromDate.toString("yyyy-MM-dd") + "', '%Y-%m-%d')";
    qStr += " AND main.VOUCHER_DATE <= str_to_date('" + toDate.toString("yyyy-MM-dd") + "', '%Y-%m-%d')";
    qStr += " GROUP BY main.voucher_no, main.voucher_prefix" ;
    qStr += " ORDER BY 1,2";

    qDebug()<<qStr;

    //    QSqlQueryModel* model = new QSqlQueryModel;
    //    model->setQuery(qStr,QSqlDatabase::database(DatabaseValues::connectionString));
    return DatabaseHelper::getQueryModel(qStr);


}

