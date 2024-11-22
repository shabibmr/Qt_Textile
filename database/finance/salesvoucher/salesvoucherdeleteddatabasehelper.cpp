#include "salesVoucherDeletedDatabaseHelper.h"

QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_TableName = "Sales_Invoice_Main_Deleted";

QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_id = "_id";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_Voucher_No = "Voucher_No";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_Voucher_Prefix = "Voucher_Prefix";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_Date_Created = "Date_Created";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_Voucher_Date = "Voucher_Date";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_TimeStamp = "`TimeStamp`";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_TimeStampC = "TimeStamp";

QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_Last_Updated = "Last_Updated";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_Ledger_ID = "Ledger_ID";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_Billing_Name = "Billing_Name";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_Total = "Total";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_Narration = "Narration";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_Added_By = "Added_By";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_Delivery_Date = "Delivery_Date";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_Completion_Probability = "Completion_Probability";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_Credit_Period = "Credit_Period";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_POC_Name = "POC_Name";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_POC_Phone = "POC_Phone";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_POC_Email = "POC_Email";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_Project_ID = "Project_ID";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_Revision_Number = "Revision_Number";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_Converted_To_Sales_Order = "Converted_To_Sales_Order";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_Quotation_Dropped = "Quotation_Dropped";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_Quotation_Dropped_Reason = "Quotation_Dropped_Reason";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_Requirement_Voucher_No = "Requirement_Voucher_No";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_Salesman_ID = "Salesman_ID";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_LPORef="LPO_Ref";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_ModeOfService = "MODE_OF_SERVICE";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_Discount_In_Amount = "Discount_In_Amount";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_Discount_In_Percent = "Discount_In_Percent";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_VATAmount = "VAT_Amount";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_VoucherStatus = "VoucherStatus";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_Contact_ID = "ContactID";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_Gross_Total = "Gross_Total";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_Reference ="Reference";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_Customer_Expecting_Date = "Customer_Expected_Date";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_Currency = "Currency";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_BoxesCount = "Boxes_Count";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_TotalWeight = "Total_Weight";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_LRNo = "LR_No";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_VoucherNoDisplay = "Voucher_No_Display";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_ReqVoucherList = "Requirement_Voucher_List";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_Origin = "Origin";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_Round_Off = "Round_Off";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_GeneratedTransactionId = "TransactionId";
QString salesVoucherDeletedDatabaseHelper::Sales_Invoice_Main_PaymentSplit="PaymentSplit";

salesVoucherDeletedDatabaseHelper::salesVoucherDeletedDatabaseHelper(QString connString)
{
    db = new DatabaseHelper(connString);
}

salesVoucherDeletedDatabaseHelper::~salesVoucherDeletedDatabaseHelper()
{

}

int salesVoucherDeletedDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

QString salesVoucherDeletedDatabaseHelper::getNextVoucherNo(QString voucherPrefix)
{
    GeneralVoucherDatabaseHelper vHelper;
    QString voucherNumber = vHelper.getNextVoucherNoByType(VoucherType, voucherPrefix, Sales_Invoice_Main_TableName);
    return voucherNumber;

}

QPair<QString, QString> salesVoucherDeletedDatabaseHelper::getVoucherNoByTransactionId(QString transId)
{
    GeneralVoucherDatabaseHelper vHelper;
    return  vHelper.getVoucherNoByTransactionId(transId, Sales_Invoice_Main_TableName);

}


int salesVoucherDeletedDatabaseHelper::insertVoucher(GeneralVoucherDataObject *obj)
{
    GeneralVoucherDatabaseHelper *vHelper = new GeneralVoucherDatabaseHelper;
    qDebug()<<"insert deleted begin"<<obj->voucherNumber;
    this->startTransaction();
    QMap<QString,QVariant> map;

    if(obj->SalesmanID == 0){
        obj->SalesmanID = LoginValues::userID;
    }

    //    qDebug()<<"Phone : "<<obj->ledgerObject.ContactPersonNumber;

    //    obj->voucherNumber = QString::number(getMaxIDwithPrefix(obj->VoucherPrefix)+1);

    QString vNo = obj->VoucherPrefix + " - " + obj->voucherNumber;

    obj->voucherNumber = vHelper->setNextVoucherNoByType(VoucherType, obj->VoucherPrefix, Sales_Invoice_Main_TableName);
    //    if(obj->ledgerObject.dbName != "" && !obj->fromExternal && obj->sendFlag)
    if(obj->TransactionId.length()==0)
        obj->TransactionId=QString::number(QDateTime::currentDateTime().toTime_t()) + obj->VoucherPrefix + db->clientId;

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
    map.insert(Sales_Invoice_Main_Converted_To_Sales_Order,vNo);
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



    qint64 lastId= db->insertRecordsPlus(map,Sales_Invoice_Main_TableName);

    //    qDebug()<<"New Voucher ID = "<<lastId;

    //    QMap<QString,QVariant> idmap;
    //    idmap.insert(Sales_Invoice_Main_Voucher_No,QString::number(lastId));
    //    QMap<QString,QVariant> idmapClause;
    //    idmapClause.insert(Sales_Invoice_Main_id,lastId);
    //    db->updateRecordsPlus(idmap,idmapClause,Sales_Invoice_Main_TableName);
    //    qDebug()<<"New ID updated = "<<lastId;

    //    obj->voucherNumber = QString::number(lastId);

    for(CompoundItemDataObject compitem:obj->InventoryItems){
        QMap<QString,QVariant> map1;
        inventoryItemDataModel item = compitem.BaseItem;
        if(item.ItemReqUuid == "X"){
            item.ItemReqUuid = db->getUUID();
        }
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
        //        map1.insert(Sales_Invoice_Details_Requirement_ItemID,item.requirementItemIdOld);
        map1.insert(Sales_Invoice_Details_Requirement_ItemID,item.ItemReqUuid);
        map1.insert(Sales_Invoice_Details_TaxRate, item.taxRate);
        map1.insert(Sales_Invoice_Details_TaxAmount, item.taxAmount);
        map1.insert(Sales_Invoice_Details_Length,item.length);
        map1.insert(Sales_Invoice_Details_Time,QDateTime::currentDateTime());
        map1.insert(Sales_Invoice_Details_UomId, item.uomObject._id);
        qDebug()<<"Current Time ==== "<<QTime::currentTime();



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
        notification->Action=1001;
        //        notification->GeneratedTransactionId=QString::number(QDateTime::currentDateTime().toTime_t()) + obj->VoucherPrefix;
        notification->GeneratedTransactionId = obj->TransactionId;

        sendChannelDatabaseHelper *sendHelper = new sendChannelDatabaseHelper();
        sendHelper->insertNotification(notification);
    }
    else{
        qDebug()<<obj->TransactionId;
    }
    qDebug()<<"277";
    return c;

}



int salesVoucherDeletedDatabaseHelper::deleteSalesVoucher(GeneralVoucherDataObject *obj)
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


    return commit();
}

int salesVoucherDeletedDatabaseHelper::commit()
{
    qDebug()<<"Saving deleted";
    if(flag){
        if(db->commitTransaction())
            return true;
        else
            return false;
    }
    else{
        qDebug()<<"Rollback from salesVoucher";
        db->rollBackTransaction();
        flag=true;
        return 0;
    }
    return 0;
}

int salesVoucherDeletedDatabaseHelper::getMaxID()
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

int salesVoucherDeletedDatabaseHelper::getMaxIDwithPrefix(QString prefix)
{
    QString qStr = "SELECT MAX("+Sales_Invoice_Main_Voucher_No+") FROM " + Sales_Invoice_Main_TableName;
    qStr += " WHERE "+Sales_Invoice_Main_Voucher_Prefix + "='"+prefix+"'";
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



GeneralVoucherDataObject *salesVoucherDeletedDatabaseHelper::getVoucherByVoucherNoPtr(QString vNo, QString voucherPrefix)
{
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
            + Sales_Invoice_Main_PaymentSplit
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
            + Transaction_Helper_isAdditional
            + " FROM " + Transaction_Helper_TableName
            + " WHERE " + Transaction_Helper_VoucherNo + "=:id"
            + " AND " + Transaction_Helper_VoucherPrefix + "='"+voucherPrefix+"'"
            + " AND " + Transaction_Helper_VoucherType + "='"+VoucherType+"'";

    QString qStrDetail = "SELECT distinct " + Sales_Invoice_Details_Voucher_No
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
            + ", " + Sales_Invoice_Details_TaxRate
            + ", " +  Sales_Invoice_Details_TaxAmount
            + " FROM " + Sales_Invoice_Details_TableName+ " s LEFT OUTER JOIN "
            + UomDataBaseHelper::qStrUomForItemDetail + " as subq "
            + " ON s." + Sales_Invoice_Details_Inventory_Item_ID + " = subq.itemID "
            + " AND s." + Sales_Invoice_Details_UomId + " = subq.UOM "
            + " WHERE " + Sales_Invoice_Details_Voucher_No + "= :vno"
            + " AND " +  Sales_Invoice_Details_Voucher_Prefix + "= :voucherPrefix"
            + " order by _id asc";


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
            obj->voucherType = query.value(9).toString();
            ledger.isInvoiceItem = query.value(10).toBool();
            if(query.value(10).toBool())
                obj->ledgersList.append(ledger);
        }
    }
    query.prepare(qStrDetail);
    query.bindValue(":vno",vNo);
    query.bindValue(":voucherPrefix",voucherPrefix);
    if(!query.exec()){
        qDebug()<<query.lastQuery();
        qDebug()<<query.lastError()<<"detail query&&&";
    }
    else{
        while(query.next()){
            int i=3;
            inventoryItemDataModel item = itemHelper->getInventoryItemByID(query.value(i++).toString(),false);
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
//            item.fifoList = itemTransaction->getFifoListByVoucherItem(obj->voucherNumber, obj->voucherType,
//                                                                      obj->VoucherPrefix, item.ItemID);
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

            item.taxRate   = query.value(i++).toFloat();
            item.taxAmount = query.value(i++).toFloat();
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
            QString reqVlist = query.value(i++).toString();
            QJsonDocument doc = QJsonDocument::fromJson(reqVlist.toUtf8());

            obj->ReqVoucherList = doc.array();
            obj->Origin = query.value(i++).toInt();
            obj->TransactionId = query.value(i++).toString();
            obj->paymentSplit = query.value(i++).toBool();

        }
    }

    if(obj->POCPhone.length()>0){
        QString qStr = "Select `name`,`address` from customer_details where phone ='"+obj->POCPhone+"'";
        if(query.exec(qStr))

            while(query.next()){
                obj->Contact.ContactName = query.value(0).toString();
                obj->Contact.address = query.value(1).toString();
                obj->Contact.PhoneNumber = obj->POCPhone;
            }
        else
            qDebug()<<query.lastError();
    }

    return obj;
}


QSqlQueryModel *salesVoucherDeletedDatabaseHelper::getVoucherListByDateTimeasModel(QDateTime dateFrom, QDateTime dateTo)
{
    QSqlQueryModel *model = new QSqlQueryModel;

    QString timestamp = "concat("+Sales_Invoice_Main_Voucher_Date+",' ',CAST("
            +Sales_Invoice_Main_TimeStamp+" AS TIME))";
    LedgerMasterDatabaseHelper led;
    UserProfileDatabaseHelper user;
    QString qStr ="SELECT "
            + Sales_Invoice_Main_Voucher_No+ " 'Bill No.', "
            + Sales_Invoice_Main_Voucher_Prefix+ "  , "
            + Sales_Invoice_Main_Voucher_Date+" 'Date', "
            + "(SELECT "+led.Ledger_Master_LedgerName+" from "+led.Ledger_Master_TableName;
    qStr    += " where "+led.Ledger_Master_LedgerID +" = sal." + Sales_Invoice_Main_Ledger_ID+ ") Payment, "
            + Sales_Invoice_Main_Reference+ " , "
            + "(SELECT "+user.Employee_Details_Name+ " from "+user.Employee_Details_TableName;
    qStr    += " where "+ user.Employee_Details_id +"= sal."+ Sales_Invoice_Main_Salesman_ID + ") Salesman, "
            + Sales_Invoice_Main_Total +" 'Amount' "
            + " FROM " + Sales_Invoice_Main_TableName +" sal "
            + " WHERE ";

    qStr += " ( ";
    qStr += timestamp + ">='" + dateFrom.toString("yyyy-MM-dd hh:mm:ss") + "'";
    qStr += " and ";
    qStr += timestamp + "<= '" + dateTo.toString("yyyy-MM-dd hh:mm:ss") + "'";
    qStr += ")";

    qStr += " order by "+Sales_Invoice_Main_TimeStamp +" desc";


    //    qDebug()<<qStr;

    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}

QSqlQueryModel *salesVoucherDeletedDatabaseHelper::getDeletedItemsListByDateTime(QDateTime dateFrom ,
                                                                                 QDateTime dateTo)
{
    QString qStr ;
    SalesInventoryItemDatabaseHelper itemHelper;

    QString timestamp = "concat("+Sales_Invoice_Details_Voucher_Date+",' ',CAST("
            +Sales_Invoice_Details_Time+" AS TIME))";

    qStr = "Select ";
    qStr += timestamp +", ";

    qStr += "(select "+itemHelper.Sales_Inventory_ItemName+" from ";
    qStr += itemHelper.Sales_Inventory_Table_Name + " where ";
    qStr += itemHelper.Sales_Inventory_ItemId + "= ta."+Sales_Invoice_Details_Inventory_Item_ID+") as Item,";

    qStr += Sales_Invoice_Details_Quantity + ", ";
    qStr += "(Select " + Sales_Invoice_Main_Narration + " FROM " + Sales_Invoice_Main_TableName + " main ";
    qStr += " WHERE main." + Sales_Invoice_Main_Voucher_No + " = ta." + Sales_Invoice_Details_Voucher_No + ") as Narration ";

//    qStr += "(select "+itemHelper.Sales_Inventory_ItemName+" from ";
//    qStr += itemHelper.Sales_Inventory_Table_Name + " where ";
//    qStr += itemHelper.Sales_Inventory_ItemId + "= ta."+Sales_Invoice_Details_Inventory_Item_ID+") as Item,";



    qStr += " FROM "+ Sales_Invoice_Details_TableName ;
    qStr += " ta where ";
    qStr += " ( ";
    qStr += timestamp + ">='" + dateFrom.toString("yyyy-MM-dd hh:mm:ss") + "'";
    qStr += " and ";
    qStr += timestamp + "<= '" +  dateTo.toString("yyyy-MM-dd hh:mm:ss") + "'";
    qStr += ")";

    QSqlQueryModel *model = DatabaseHelper::getQueryModel(qStr);
    return model;
}

QSqlQueryModel *salesVoucherDeletedDatabaseHelper::getVoucherListByDateTimeModel(QDateTime dateFrom, QDateTime dateTo)
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
    qStr += Sales_Invoice_Main_Voucher_No +" as 'Voucher No', ";
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

    //    qDebug()<<qStr;

    //    model = DatabaseHelper::getQueryModel(qStr);
    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}

void salesVoucherDeletedDatabaseHelper::switchSalesMan(int newEmpID, QString vNO)
{
    QMap<QString,QVariant> map;
    QMap<QString,QVariant> clause;

    map.insert(Sales_Invoice_Main_Salesman_ID,newEmpID);
    clause.insert(Sales_Invoice_Main_Voucher_No,vNO);
    db->updateRecordsPlus(map,clause,Sales_Invoice_Main_TableName);
}

QString salesVoucherDeletedDatabaseHelper::getActualVoucherByREFandDate(QString ref,QDate date)
{
    QString refno="0";

    QString qStr = "";

    qStr += "SELECT "+ Sales_Invoice_Main_Voucher_No + " from ";
    qStr += Sales_Invoice_Main_TableName + " WHERE ";
    qStr += Sales_Invoice_Main_LPORef + "='"+ ref +"' AND ";
    qStr += Sales_Invoice_Main_Voucher_Date + "='"+ date.toString("yyyy-MM-dd") +"'";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    qDebug()<<qStr;
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

QList<inventoryItemDataModel> salesVoucherDeletedDatabaseHelper::getItemsListForMail(QDateTime fromTime, QDateTime toTime)
{
    QString qStr = "";

    QList<inventoryItemDataModel> items;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    if(query.exec(qStr)){
        while(query.next()){
            inventoryItemDataModel item;
            item.ItemName = query.value(0).toString();
            item.ItemID = query.value(1).toString();
            item.quantity = query.value(2).toFloat();
            item.toTime = query.value(3).toTime();
            item.dateCreated = query.value(4).toDate();
            items.append(item);
        }
    }
    return items;

}

int salesVoucherDeletedDatabaseHelper::updateVoucher(GeneralVoucherDataObject *obj, QString oldID)
{

}

int salesVoucherDeletedDatabaseHelper::deleteVoucher(GeneralVoucherDataObject *obj)
{

}

QString salesVoucherDeletedDatabaseHelper::getVoucherNumberByLedID(QString LedID, QDate Date)
{

}

QSqlQueryModel *salesVoucherDeletedDatabaseHelper::getVoucherListByDateModel(QDate dateFrom, QDate dateTo)
{

}

QString salesVoucherDeletedDatabaseHelper::getActualVoucherByREFandDate(QString ref, QDateTime date)
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
