#include "OldQuotationDatabaseHelper.h"


OldQuotationDatabaseHelper::OldQuotationDatabaseHelper()
{
    db = new DatabaseHelper();
}

OldQuotationDatabaseHelper::~OldQuotationDatabaseHelper()
{

}

int OldQuotationDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

int OldQuotationDatabaseHelper::insertQuotation(GeneralVoucherDataObject *obj)
{
    this->startTransaction();
    QMap<QString,QVariant> map;
    map.insert(Quotation_Main_Voucher_No,obj->voucherNumber);
    map.insert(Quotation_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Quotation_Main_Date_Created,obj->VoucherDate);
    map.insert(Quotation_Main_Voucher_Date,obj->VoucherDate);
    map.insert(Quotation_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    map.insert(Quotation_Main_Billing_Name,obj->ledgerObject.LedgerName);
    map.insert(Quotation_Main_Total,obj->grandTotal);
    map.insert(Quotation_Main_Narration,obj->narration);
    map.insert(Quotation_Main_Added_By,obj->AddedById);
    map.insert(Quotation_Main_Delivery_Date,obj->DeliveryDate);
    map.insert(Quotation_Main_Completion_Probability,obj->CompletionProbability);
    map.insert(Quotation_Main_Credit_Period,obj->ledgerObject.Credit_Period);
    map.insert(Quotation_Main_POC_Name,obj->ledgerObject.ContactPersonName);
    map.insert(Quotation_Main_POC_Phone,obj->ledgerObject.ContactPersonNumber);
    map.insert(Quotation_Main_POC_Email,obj->ledgerObject.ContactPersonEmail);
    map.insert(Quotation_Main_Project_ID,obj->ProjectId);
    map.insert(Quotation_Main_Revision_Number,obj->RevisionNo);
    map.insert(Quotation_Main_Converted_To_Sales_Order,obj->ConvertedToSalesOrder);
    map.insert(Quotation_Main_Quotation_Dropped,obj->QuotationDropped);
    map.insert(Quotation_Main_Quotation_Dropped_Reason,obj->QuotationDroppedReason);
    map.insert(Quotation_Main_Requirement_Voucher_No,obj->RequirementVoucherNo);
    map.insert(Quotation_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Quotation_Main_Terms_And_Conditions_ID,obj->TermsAndConditionsID);
    map.insert(Quotation_Main_Contact_ID,obj->Contact._id);

    db->insertRecordsPlus(map,Quotation_Main_TableName);

    for(CompoundItemDataObject compitem:obj->InventoryItems){
        QMap<QString,QVariant> map1;
        inventoryItemDataModel item = compitem.BaseItem;
        map1.insert(Quotation_Details_Voucher_No,obj->voucherNumber);
        map1.insert(Quotation_Details_Voucher_Prefix,obj->VoucherPrefix);
        map1.insert(Quotation_Details_Voucher_Date,obj->VoucherDate);
        map1.insert(Quotation_Details_Inventory_Item_ID,item.ItemID);
        map1.insert(Quotation_Details_Quantity,item.quantity);
        map1.insert(Quotation_Details_Price,item.price);
        map1.insert(Quotation_Details_PriceLevel,item.PriceLevel);
        map1.insert(Quotation_Details_isCompoundItem,item.IsCompoundItem);
        map1.insert(Quotation_Details_subItemOff_list_ID,0);
        map1.insert(Quotation_Details_Discount_Percent,item.discountPercentage);
        map1.insert(Quotation_Details_Discount_Amount,item.discount);
        map1.insert(Quotation_Details_Subtotal,item.subTotal);
        map1.insert(Quotation_Details_Project_ID,obj->ProjectId);
        map1.insert(Quotation_Details_Item_Narration,item.narration);
        map1.insert(Quotation_Details_Item_Description,item.ItemDescription);
        map1.insert(Quotation_Details_Salesman_ID,obj->SalesmanID);
        map1.insert(Quotation_Details_Requirement_ItemID,item.requirementItemIdOld);
        map1.insert(Quotation_Details_Length,item.length);


        int basid = db->insertRecordsPlus(map1,Quotation_Details_TableName);

        for(inventoryItemDataModel subitem:compitem.subItems){
            QMap<QString,QVariant> map2;
            map2.insert(Quotation_Details_Voucher_No,obj->voucherNumber);
            map2.insert(Quotation_Details_Voucher_Prefix,obj->VoucherPrefix);
            map2.insert(Quotation_Details_Voucher_Date,obj->VoucherDate);
            map2.insert(Quotation_Details_Inventory_Item_ID,subitem.ItemID);
            map2.insert(Quotation_Details_Quantity,subitem.quantity);
            map2.insert(Quotation_Details_Price,subitem.price);
            map2.insert(Quotation_Details_PriceLevel,subitem.PriceLevel);
            map2.insert(Quotation_Details_isCompoundItem,subitem.IsCompoundItem);
            map2.insert(Quotation_Details_subItemOff_list_ID,basid);
            map2.insert(Quotation_Details_Discount_Percent,subitem.discountPercentage);
            map2.insert(Quotation_Details_Discount_Amount,subitem.discount);
            map2.insert(Quotation_Details_Subtotal,subitem.subTotal);
            map2.insert(Quotation_Details_Project_ID,obj->ProjectId);
            map2.insert(Quotation_Details_Item_Narration,subitem.narration);
            map2.insert(Quotation_Details_Item_Description,subitem.ItemDescription);
            map2.insert(Quotation_Details_Salesman_ID,obj->SalesmanID);
            map2.insert(Quotation_Details_Requirement_ItemID,subitem.requirementItemIdOld);
            map2.insert(Quotation_Details_Length,item.length);

            db->insertRecordsPlus(map2,Quotation_Details_TableName);
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
        qDebug()<<"Start proxy";
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
        qDebug()<<"End proxy";
    }

    return commit();
}

int OldQuotationDatabaseHelper::updateQuotation(GeneralVoucherDataObject *obj, QString oldID)
{
    this->startTransaction();
    QMap<QString,QVariant> map5;
    map5.insert(Quotation_Details_Voucher_No,oldID);
    db->deleteRecordsPlus(map5,Quotation_Details_TableName);

    QMap<QString,QVariant> map6;
    map6.insert(Proxy_Transaction_Helper_VoucherNo,oldID);
    db->deleteRecordsPlus(map6,Proxy_Transaction_Helper_TableName);


    QMap<QString,QVariant> map;
    map.insert(Quotation_Main_Voucher_No,obj->voucherNumber);
    map.insert(Quotation_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Quotation_Main_Date_Created,obj->VoucherDate);
    map.insert(Quotation_Main_Voucher_Date,obj->VoucherDate);
    map.insert(Quotation_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    map.insert(Quotation_Main_Billing_Name,obj->ledgerObject.LedgerName);
    map.insert(Quotation_Main_Total,obj->grandTotal);
    map.insert(Quotation_Main_Narration,obj->narration);
    map.insert(Quotation_Main_Added_By,obj->AddedById);
    map.insert(Quotation_Main_Delivery_Date,obj->DeliveryDate);
    map.insert(Quotation_Main_Completion_Probability,obj->CompletionProbability);
    map.insert(Quotation_Main_Credit_Period,obj->ledgerObject.Credit_Period);
    map.insert(Quotation_Main_POC_Name,obj->ledgerObject.ContactPersonName);
    map.insert(Quotation_Main_POC_Phone,obj->ledgerObject.ContactPersonNumber);
    map.insert(Quotation_Main_POC_Email,obj->ledgerObject.ContactPersonEmail);
    map.insert(Quotation_Main_Project_ID,obj->ProjectId);
    map.insert(Quotation_Main_Revision_Number,obj->RevisionNo);
    map.insert(Quotation_Main_Converted_To_Sales_Order,obj->ConvertedToSalesOrder);
    map.insert(Quotation_Main_Quotation_Dropped,obj->QuotationDropped);
    map.insert(Quotation_Main_Quotation_Dropped_Reason,obj->QuotationDroppedReason);
    map.insert(Quotation_Main_Requirement_Voucher_No,obj->RequirementVoucherNo);
    map.insert(Quotation_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Quotation_Main_Terms_And_Conditions_ID,obj->TermsAndConditionsID);
    map.insert(Quotation_Main_Contact_ID,obj->Contact._id);

    QMap<QString,QVariant> map3;
    map3.insert(Quotation_Main_id,oldID);
    db->updateRecordsPlus(map,map3,Quotation_Main_TableName);


    for(CompoundItemDataObject compitem:obj->InventoryItems){
        QMap<QString,QVariant> map1;
        inventoryItemDataModel item = compitem.BaseItem;
        map1.insert(Quotation_Details_Voucher_No,obj->voucherNumber);
        map1.insert(Quotation_Details_Voucher_Prefix,obj->VoucherPrefix);
        map1.insert(Quotation_Details_Voucher_Date,obj->VoucherDate);
        map1.insert(Quotation_Details_Inventory_Item_ID,item.ItemID);
        map1.insert(Quotation_Details_Quantity,item.quantity);
        map1.insert(Quotation_Details_Price,item.price);
        map1.insert(Quotation_Details_PriceLevel,item.PriceLevel);
        map1.insert(Quotation_Details_isCompoundItem,item.IsCompoundItem);
        map1.insert(Quotation_Details_subItemOff_list_ID,0);
        map1.insert(Quotation_Details_Discount_Percent,item.discountPercentage);
        map1.insert(Quotation_Details_Discount_Amount,item.discount);
        map1.insert(Quotation_Details_Subtotal,item.subTotal);
        map1.insert(Quotation_Details_Project_ID,obj->ProjectId);
        map1.insert(Quotation_Details_Item_Narration,item.narration);
        map1.insert(Quotation_Details_Item_Description,item.ItemDescription);
        map1.insert(Quotation_Details_Salesman_ID,obj->SalesmanID);
        map1.insert(Quotation_Details_Requirement_ItemID,item.requirementItemIdOld);
        map1.insert(Quotation_Details_Length,item.length);

        int basid = db->insertRecordsPlus(map1,Quotation_Details_TableName);

        for(inventoryItemDataModel subitem:compitem.subItems){
            QMap<QString,QVariant> map2;
            map2.insert(Quotation_Details_Voucher_No,obj->voucherNumber);
            map2.insert(Quotation_Details_Voucher_Prefix,obj->VoucherPrefix);
            map2.insert(Quotation_Details_Voucher_Date,obj->VoucherDate);
            map2.insert(Quotation_Details_Inventory_Item_ID,subitem.ItemID);
            map2.insert(Quotation_Details_Quantity,subitem.quantity);
            map2.insert(Quotation_Details_Price,subitem.price);
            map2.insert(Quotation_Details_PriceLevel,subitem.PriceLevel);
            map2.insert(Quotation_Details_isCompoundItem,subitem.IsCompoundItem);
            map2.insert(Quotation_Details_subItemOff_list_ID,basid);
            map2.insert(Quotation_Details_Discount_Percent,subitem.discountPercentage);
            map2.insert(Quotation_Details_Discount_Amount,subitem.discount);
            map2.insert(Quotation_Details_Subtotal,subitem.subTotal);
            map2.insert(Quotation_Details_Project_ID,obj->ProjectId);
            map2.insert(Quotation_Details_Item_Narration,subitem.narration);
            map2.insert(Quotation_Details_Item_Description,subitem.ItemDescription);
            map2.insert(Quotation_Details_Salesman_ID,obj->SalesmanID);
            map2.insert(Quotation_Details_Requirement_ItemID,subitem.requirementItemIdOld);
            map2.insert(Quotation_Details_Length,item.length);

            db->insertRecordsPlus(map2,Quotation_Details_TableName);
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
        ledMap.insert(Proxy_Transaction_Helper_CreditLedgerID,ledger.LedgerID);
        ledMap.insert(Proxy_Transaction_Helper_DebitLedgerID,obj->ledgerObject.LedgerID);
        ledMap.insert(Proxy_Transaction_Helper_Narration,obj->ledgerObject.narration);
        ledMap.insert(Proxy_Transaction_Helper_isAdditional,true);
        db->insertRecordsPlus(ledMap,Proxy_Transaction_Helper_TableName);
    }

    return commit();
}

int OldQuotationDatabaseHelper::deleteQuotation(QString oldID)
{
    this->startTransaction();
    QMap<QString,QVariant> map3;
    map3.insert(Quotation_Main_id,oldID);
    db->deleteRecordsPlus(map3,Quotation_Main_TableName);

    QMap<QString,QVariant> map4;
    map4.insert(Quotation_Details_id,oldID);
    db->deleteRecordsPlus(map4,Quotation_Details_TableName);
    return commit();
}

int OldQuotationDatabaseHelper::commit()
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

int OldQuotationDatabaseHelper::getMaxID()
{
    QString qStr = "SELECT MAX("+Quotation_Main_id+") FROM " + Quotation_Main_TableName;
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

GeneralVoucherDataObject OldQuotationDatabaseHelper::getVoucherByVoucherNo(QString vNo)
{
    qDebug()<<"256";
    GeneralVoucherDataObject obj;
    SalesInventoryItemDatabaseHelper *itemHelper = new SalesInventoryItemDatabaseHelper();
    LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper();
    AddressBookDatabaseHelper *addHelper = new AddressBookDatabaseHelper();
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
            + Quotation_Details_id +", "
            + Quotation_Details_Voucher_No +", "
            + Quotation_Details_Voucher_Prefix +", "
            + Quotation_Details_Voucher_Date +", "
            + Quotation_Details_Inventory_Item_ID +", "
            + Quotation_Details_Price +", "
            + Quotation_Details_Quantity +", "
            + Quotation_Details_Discount_Percent +", "
            + Quotation_Details_Subtotal +", "
            + Quotation_Details_isCompoundItem +", "
            + Quotation_Details_subItemOff_list_ID +", "
            + Quotation_Details_PriceLevel +", "
            + Quotation_Details_Item_Description +", "
            + Quotation_Details_Item_Narration +", "
            + Quotation_Details_Requirement_ItemID +", "
            + Quotation_Details_Length + ", "
            + Quotation_Details_Discount_Amount + " "
            + " FROM " + Quotation_Details_TableName
            + " WHERE " + Quotation_Details_Voucher_No + "= :vno";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStrLedgers);
    query.bindValue(":id",vNo);
    if(!query.exec()){
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    else{
        while(query.next()){
           // AddressBookDataModel address = addHelper->getContact(query.value(6).toInt());

            LedgerMasterDataModel ledger;
            ledger = ledHelper->getLedgerObjectByID(query.value(6).toString());
            ledger.amount = query.value(4).toFloat();
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
            item.price = query.value(5).toFloat();
            item.quantity = query.value(6).toFloat();
            item.discountPercentage = query.value(7).toFloat();
            item.subTotal = query.value(8).toFloat();
            item.PriceLevel = query.value(11).toString();
            item.narration = query.value(13).toString();
            item.ItemDescription = query.value(12).toString();
            item.requirementItemIdOld = query.value(0).toLongLong();
            item.length= query.value(15).toFloat();
            item.discount = query.value(16).toFloat();

            item.listId = query.value(0).toInt();
            if(query.value(9).toBool()||(query.value(10).toInt()==0)){
                CompoundItemDataObject cObj;
                cObj.BaseItem = item;
                obj.InventoryItems.append(cObj);
            }
            else{

                int itemSize = obj.InventoryItems.size();
                qDebug()<<itemSize;
                obj.InventoryItems[itemSize-1].subItems.append(item);
                obj.InventoryItems[itemSize-1].subItemsTotal +=item.subTotal;
            }
        }
    }
    return obj;
}

QList<GeneralVoucherDataObject> OldQuotationDatabaseHelper::getVoucherListByDate(QDate dateFrom, QDate dateTo)
{
    QList<GeneralVoucherDataObject> vouchersList;
    QString qStr ="SELECT "
            + Quotation_Main_Voucher_No+ ", "
            + Quotation_Main_Voucher_Prefix+ ", "
            + Quotation_Main_Voucher_Date+", "
            + Quotation_Main_Ledger_ID+ ", "
            + Quotation_Main_Total +", "
            + Quotation_Main_Client_Approval_Status +", "
            + Quotation_Main_Manager_Approval_Status +", "
            + Quotation_Main_Client_Approval_Status +", "
            + Quotation_Main_TimeStamp+", "
            + Quotation_Main_Contact_ID + ", "
            + Quotation_Main_Added_By + ", "
            + Quotation_Main_Salesman_ID + ", "
            + Quotation_Main_Narration + ", "
            + Quotation_Main_LPO_Ref + " "


            + " FROM " + Quotation_Main_TableName
            + " WHERE "
            + Quotation_Main_Voucher_Date
            + " >= '"
            + dateFrom.toString("yyyy-MM-dd")
            + "' AND "
            + Quotation_Main_Voucher_Date
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
            item.ClientApprovalStatus =query.value(i++).toBool();
            item.ManagerApprovalStatus = query.value(i++).toBool();
            item.status = query.value(i++).toInt();
            item.timestamp = query.value(i++).toDateTime();
            item.Contact = addHelper->getContact(query.value(i++).toInt());
            item.AddedById = query.value(i++).toInt();
            item.SalesmanID = query.value(i++).toInt();
            item.narration = query.value(i++).toString();
            item.LPO = query.value(i++).toString();
            vouchersList.append(item);
        }
    }

    return vouchersList;
}
