#include "quotationdatabasehelper.h"

QuotationDatabaseHelper::QuotationDatabaseHelper()
{
    db = new DatabaseHelper();
}

QuotationDatabaseHelper::~QuotationDatabaseHelper()
{

}

int QuotationDatabaseHelper::startTransaction()
{
    db->startTransaction();
    return 1;
}

QString QuotationDatabaseHelper::getNextVoucherNo(QString voucherPrefix)
{
    GeneralVoucherDatabaseHelper vHelper;
    QString voucherNumber = vHelper.getNextVoucherNoByType(ledgerType, voucherPrefix, Quotation_Main_TableName);
    return voucherNumber;

}


int QuotationDatabaseHelper::insertQuotation(GeneralVoucherDataObject *obj)
{
    this->startTransaction();
    QMap<QString,QVariant> map;

    //    obj->voucherNumber = QString::number(getMaxID()+1);

    GeneralVoucherDatabaseHelper vHelper;
    obj->voucherNumber = vHelper.setNextVoucherNoByType(ledgerType, obj->VoucherPrefix, Quotation_Main_TableName );

    map.insert(Quotation_Main_Voucher_No,obj->voucherNumber);
    map.insert(Quotation_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Quotation_Main_Date_Created,obj->VoucherDate);
    map.insert(Quotation_Main_Voucher_Date,obj->VoucherDate);
    map.insert(Quotation_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    map.insert(Quotation_Main_Billing_Name,obj->ledgerObject.LedgerName);
    map.insert(Quotation_Main_Total,obj->grandTotal);
    map.insert(Quotation_Main_Discount_In_Amount,obj->discountinAmount);
    map.insert(Quotation_Main_Discount_In_Percent,obj->discountPercent);
    map.insert(Quotation_Main_Discount_Total,obj->discount);
    map.insert(Quotation_Main_Narration,obj->narration);
    map.insert(Quotation_Main_Added_By,obj->AddedById);
    map.insert(Quotation_Main_Delivery_Date,obj->DeliveryDate);
    map.insert(Quotation_Main_Completion_Probability,obj->CompletionProbability);
    map.insert(Quotation_Main_Credit_Period,obj->ledgerObject.Credit_Period);
    map.insert(Quotation_Main_POC_Name,obj->POCName);
    map.insert(Quotation_Main_POC_Phone,obj->POCPhone);
    map.insert(Quotation_Main_POC_Email,obj->POCEmail);
    map.insert(Quotation_Main_Project_ID,obj->ProjectId);
    map.insert(Quotation_Main_Revision_Number,obj->RevisionNo);
    map.insert(Quotation_Main_Converted_To_Sales_Order,obj->ConvertedToSalesOrder);
    map.insert(Quotation_Main_Quotation_Dropped,obj->QuotationDropped);
    map.insert(Quotation_Main_Quotation_Dropped_Reason,obj->QuotationDroppedReason);
    map.insert(Quotation_Main_Requirement_Voucher_No,obj->RequirementVoucherNo);
    map.insert(Quotation_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Quotation_Main_Terms_And_Conditions_ID,obj->TermsAndConditionsID);
    map.insert(Quotation_Main_Contact_ID,obj->Contact._id);
    map.insert(Quotation_Main_VoucherStatus,obj->status);
    map.insert(Quotation_Main_Location,obj->Location);
    map.insert(Quotation_Main_TimeStampC, obj->timestamp);


    db->insertRecordsPlus(map,Quotation_Main_TableName);

    for(CompoundItemDataObject compitem:obj->InventoryItems){
        QMap<QString,QVariant> map1;
        inventoryItemDataModel item = compitem.BaseItem;
//        if(item.ItemReqUuid == "X")
//            item.ItemReqUuid = db->getUUID();
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
        map1.insert(Quotation_Details_Discount_Amount,item.discountinAmount);
        map1.insert(Quotation_Details_Subtotal,item.subTotal);
        map1.insert(Quotation_Details_Project_ID,obj->ProjectId);
        map1.insert(Quotation_Details_Item_Narration,item.narration);
        map1.insert(Quotation_Details_Item_Description,item.ItemDescription);
        map1.insert(Quotation_Details_Salesman_ID,obj->SalesmanID);
        map1.insert(Quotation_Details_Requirement_ItemID,item.ItemReqUuid);
        map1.insert(Quotation_Details_Length,item.length);
        map1.insert(Quotation_Details_Dimenstion,item.Dimension);


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
            map2.insert(Quotation_Details_Discount_Amount,subitem.discountinAmount);
            map2.insert(Quotation_Details_Subtotal,subitem.subTotal);
            map2.insert(Quotation_Details_Project_ID,obj->ProjectId);
            map2.insert(Quotation_Details_Item_Narration,subitem.narration);
            map2.insert(Quotation_Details_Item_Description,subitem.ItemDescription);
            map2.insert(Quotation_Details_Salesman_ID,obj->SalesmanID);
            map2.insert(Quotation_Details_Requirement_ItemID,item.ItemReqUuid);
            map2.insert(Quotation_Details_Length,item.length);
            map2.insert(Quotation_Details_Dimenstion,item.Dimension);

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

int QuotationDatabaseHelper::updateQuotation(GeneralVoucherDataObject *obj, QString oldID)
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
    map.insert(Quotation_Main_Discount_In_Amount,obj->discountinAmount);
    map.insert(Quotation_Main_Discount_In_Percent,obj->discountPercent);
    map.insert(Quotation_Main_Discount_Total,obj->discount);
    map.insert(Quotation_Main_Narration,obj->narration);
    map.insert(Quotation_Main_Added_By,obj->AddedById);
    map.insert(Quotation_Main_Delivery_Date,obj->DeliveryDate);
    map.insert(Quotation_Main_Completion_Probability,obj->CompletionProbability);
    map.insert(Quotation_Main_Credit_Period,obj->ledgerObject.Credit_Period);
    map.insert(Quotation_Main_POC_Name,obj->POCName);
    map.insert(Quotation_Main_POC_Phone,obj->POCPhone);
    map.insert(Quotation_Main_POC_Email,obj->POCEmail);
    map.insert(Quotation_Main_Project_ID,obj->ProjectId);
    map.insert(Quotation_Main_Revision_Number,obj->RevisionNo);
    map.insert(Quotation_Main_Converted_To_Sales_Order,obj->ConvertedToSalesOrder);
    map.insert(Quotation_Main_Quotation_Dropped,obj->QuotationDropped);
    map.insert(Quotation_Main_Quotation_Dropped_Reason,obj->QuotationDroppedReason);
    map.insert(Quotation_Main_Requirement_Voucher_No,obj->RequirementVoucherNo);
    map.insert(Quotation_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Quotation_Main_Terms_And_Conditions_ID,obj->TermsAndConditionsID);
    map.insert(Quotation_Main_Contact_ID,obj->Contact._id);
    map.insert(Quotation_Main_VoucherStatus,obj->status);
    map.insert(Quotation_Main_Location,obj->Location);

    QMap<QString,QVariant> map3;
    map3.insert(Quotation_Main_Voucher_No,oldID);
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
        map1.insert(Quotation_Details_Discount_Amount,item.discountinAmount);
        map1.insert(Quotation_Details_Subtotal,item.subTotal);
        map1.insert(Quotation_Details_Project_ID,obj->ProjectId);
        map1.insert(Quotation_Details_Item_Narration,item.narration);
        map1.insert(Quotation_Details_Item_Description,item.ItemDescription);
        map1.insert(Quotation_Details_Salesman_ID,obj->SalesmanID);
        map1.insert(Quotation_Details_Requirement_ItemID,item.ItemReqUuid);
        map1.insert(Quotation_Details_Length,item.length);
        map1.insert(Quotation_Details_Dimenstion,item.Dimension);

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
            map2.insert(Quotation_Details_Discount_Amount,subitem.discountinAmount);
            map2.insert(Quotation_Details_Subtotal,subitem.subTotal);
            map2.insert(Quotation_Details_Project_ID,obj->ProjectId);
            map2.insert(Quotation_Details_Item_Narration,subitem.narration);
            map2.insert(Quotation_Details_Item_Description,subitem.ItemDescription);
            map2.insert(Quotation_Details_Salesman_ID,obj->SalesmanID);
            map2.insert(Quotation_Details_Requirement_ItemID,subitem.ItemReqUuid);
            map2.insert(Quotation_Details_Length,item.length);
            map2.insert(Quotation_Details_Dimenstion,item.Dimension);

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

int QuotationDatabaseHelper::deleteQuotation(QString oldID)
{
    this->startTransaction();
    QMap<QString,QVariant> map3;
    map3.insert(Quotation_Main_Voucher_No,oldID);
    db->deleteRecordsPlus(map3,Quotation_Main_TableName);

    QMap<QString,QVariant> map4;
    map4.insert(Quotation_Details_Voucher_No,oldID);
    db->deleteRecordsPlus(map4,Quotation_Details_TableName);
    return commit();
}

int QuotationDatabaseHelper::commit(int n)
{
    if(flag){
        db->commitTransaction();
        return n;
    }
    else{
        db->rollBackTransaction();
        return 0;
    }
}

int QuotationDatabaseHelper::getMaxID()
{
    QString qStr = "SELECT MAX("+Quotation_Main_id+") FROM " + Quotation_Main_TableName;
    //  qDebug()<<qStr;
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

GeneralVoucherDataObject QuotationDatabaseHelper::getVoucherByVoucherNo(QString vNo)
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
            + Quotation_Details_Dimenstion + ", "
            + Quotation_Details_Discount_Amount + " "
            + " FROM " + Quotation_Details_TableName
            + " WHERE " + Quotation_Details_Voucher_No + "= :vno";

    QString qStrMain ="SELECT "
            + Quotation_Main_Voucher_No+ ", "
            + Quotation_Main_Voucher_Prefix+ ", "
            + Quotation_Main_Voucher_Date+", "
            + Quotation_Main_Ledger_ID+ ", "
            + Quotation_Main_Total +", "
            + Quotation_Main_Client_Approval_Status +", "
            + Quotation_Main_Manager_Approval_Status +", "
            + Quotation_Main_VoucherStatus +", "
            + Quotation_Main_TimeStamp+", "
            + Quotation_Main_Contact_ID + ", "
            + Quotation_Main_Added_By + ", "
            + Quotation_Main_Salesman_ID + ", "
            + Quotation_Main_Narration + ", "
            + Quotation_Main_LPO_Ref + ", "
            + Quotation_Main_Discount_In_Amount + ", "
            + Quotation_Main_Discount_In_Percent + ", "
            + Quotation_Main_Discount_Total + ", "
            + Quotation_Main_POC_Name + ", "
            + Quotation_Main_POC_Phone + ", "
            + Quotation_Main_POC_Email + ", "
            + Quotation_Main_Location + " "



            + " FROM " + Quotation_Main_TableName
            + " WHERE " + Quotation_Main_Voucher_No + "= :vno";
    ;

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
            item.ItemReqUuid = query.value(0).toString();
            item.length= query.value(15).toFloat();
            item.Dimension = query.value(16).toString();
            item.discountinAmount = query.value(17).toFloat();

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
    //    qDebug()<<qStrDetail<<vNo;
    query.prepare(qStrMain);
    query.bindValue(":vno",vNo);
    if(!query.exec()){
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    else{

        while(query.next()){
            int i=0;
            obj.voucherNumber = query.value(i++).toString();
            obj.VoucherPrefix = query.value(i++).toString();
            obj.VoucherDate = query.value(i++).toDate();
            obj.ledgerObject.LedgerID = query.value(i++).toString();
            obj.grandTotal = query.value(i++).toFloat();
            obj.ClientApprovalStatus =query.value(i++).toBool();
            obj.ManagerApprovalStatus = query.value(i++).toBool();
            obj.status = query.value(i++).toInt();
            obj.timestamp = query.value(i++).toDateTime();
            obj.Contact = addHelper->getContact(query.value(i++).toInt());
            obj.AddedById = query.value(i++).toInt();
            obj.SalesmanID = query.value(i++).toInt();
            obj.narration = query.value(i++).toString();
            obj.LPO = query.value(i++).toString();
            obj.ledgerObject.LedgerName = ledHelper->getLedgerNameByID(obj.Contact.ledgerId );
            obj.discountinAmount = query.value(i++).toFloat();
            obj.discountPercent = query.value(i++).toFloat();
            obj.discount = query.value(i++).toFloat();
            obj.POCName= query.value(i++).toString();
            obj.POCPhone= query.value(i++).toString();
            obj.POCEmail= query.value(i++).toString();
            obj.Location= query.value(i++).toString();


        }
    }
    return obj;
}

GeneralVoucherDataObject *QuotationDatabaseHelper::getVoucherByVoucherNoAsPtr(QString vNo)
{
//    qDebug()<<"256";
//    GeneralVoucherDataObject obj;
//    SalesInventoryItemDatabaseHelper *itemHelper = new SalesInventoryItemDatabaseHelper();
//    LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper();
//    AddressBookDatabaseHelper *addHelper = new AddressBookDatabaseHelper();
//    QString qStrLedgers ="SELECT "
//            + Proxy_Transaction_Helper_id +", "
//            + Proxy_Transaction_Helper_VoucherNo +", "
//            + Proxy_Transaction_Helper_VoucherPrefix +", "
//            + Proxy_Transaction_Helper_ProjectID +", "
//            + Proxy_Transaction_Helper_TranactionAmount +", "
//            + Proxy_Transaction_Helper_CreditLedgerID +", "
//            + Proxy_Transaction_Helper_DebitLedgerID + ", "
//            + Proxy_Transaction_Helper_Narration +", "
//            + Proxy_Transaction_Helper_SalesManID +", "
//            + Proxy_Transaction_Helper_isAdditional
//            + " FROM " + Proxy_Transaction_Helper_TableName
//            + " WHERE " + Proxy_Transaction_Helper_VoucherNo + "=:id"
//            + " AND " + Proxy_Transaction_Helper_VoucherType + "='"+ledgerType+"'";

//    QString qStrDetail = "SELECT "
//            + Quotation_Details_id +", "
//            + Quotation_Details_Voucher_No +", "
//            + Quotation_Details_Voucher_Prefix +", "
//            + Quotation_Details_Voucher_Date +", "
//            + Quotation_Details_Inventory_Item_ID +", "
//            + Quotation_Details_Price +", "
//            + Quotation_Details_Quantity +", "
//            + Quotation_Details_Discount_Percent +", "
//            + Quotation_Details_Subtotal +", "
//            + Quotation_Details_isCompoundItem +", "
//            + Quotation_Details_subItemOff_list_ID +", "
//            + Quotation_Details_PriceLevel +", "
//            + Quotation_Details_Item_Description +", "
//            + Quotation_Details_Item_Narration +", "
//            + Quotation_Details_Requirement_ItemID +", "
//            + Quotation_Details_Length + ", "
//            + Quotation_Details_Dimenstion + ", "
//            + Quotation_Details_Discount_Amount + " "
//            + " FROM " + Quotation_Details_TableName
//            + " WHERE " + Quotation_Details_Voucher_No + "= :vno";

//    QString qStrMain ="SELECT "
//            + Quotation_Main_Voucher_No+ ", "
//            + Quotation_Main_Voucher_Prefix+ ", "
//            + Quotation_Main_Voucher_Date+", "
//            + Quotation_Main_Ledger_ID+ ", "
//            + Quotation_Main_Total +", "
//            + Quotation_Main_Client_Approval_Status +", "
//            + Quotation_Main_Manager_Approval_Status +", "
//            + Quotation_Main_VoucherStatus +", "
//            + Quotation_Main_TimeStamp+", "
//            + Quotation_Main_Contact_ID + ", "
//            + Quotation_Main_Added_By + ", "
//            + Quotation_Main_Salesman_ID + ", "
//            + Quotation_Main_Narration + ", "
//            + Quotation_Main_LPO_Ref + ", "
//            + Quotation_Main_Discount_In_Amount + ", "
//            + Quotation_Main_Discount_In_Percent + ", "
//            + Quotation_Main_Discount_Total + ", "
//            + Quotation_Main_POC_Name + ", "
//            + Quotation_Main_POC_Phone + ", "
//            + Quotation_Main_POC_Email + ", "
//            + Quotation_Main_Location + " "



//            + " FROM " + Quotation_Main_TableName
//            + " WHERE " + Quotation_Main_Voucher_No + "= :vno";
//    ;

//    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

//    query.prepare(qStrLedgers);
//    query.bindValue(":id",vNo);
//    if(!query.exec()){
//        qDebug()<<query.lastError();
//        qDebug()<<query.lastQuery();
//    }
//    else{
//        while(query.next()){
//            // AddressBookDataModel address = addHelper->getContact(query.value(6).toInt());

//            LedgerMasterDataModel ledger;
//            ledger = ledHelper->getLedgerObjectByID(query.value(6).toString());
//            ledger.amount = query.value(4).toFloat();
//            if(query.value(9).toBool())
//                obj.ledgersList.append(ledger);
//        }
//    }
//    query.prepare(qStrDetail);
//    query.bindValue(":vno",vNo);
//    if(!query.exec()){
//        qDebug()<<query.lastError();
//        qDebug()<<query.lastQuery();
//    }
//    else{
//        while(query.next()){
//            inventoryItemDataModel item = itemHelper->getInventoryItemByID(query.value(4).toString());
//            item.price = query.value(5).toFloat();
//            item.quantity = query.value(6).toFloat();
//            item.discountPercentage = query.value(7).toFloat();

//            item.subTotal = query.value(8).toFloat();
//            item.PriceLevel = query.value(11).toString();
//            item.narration = query.value(13).toString();
//            item.ItemDescription = query.value(12).toString();
//            item.ItemReqUuid = query.value(0).toString();
//            item.length= query.value(15).toFloat();
//            item.Dimension = query.value(16).toString();
//            item.discountinAmount = query.value(17).toFloat();

//            item.listId = query.value(0).toInt();
//            if(query.value(9).toBool()||(query.value(10).toInt()==0)){
//                CompoundItemDataObject cObj;
//                cObj.BaseItem = item;
//                obj.InventoryItems.append(cObj);
//            }
//            else{

//                int itemSize = obj.InventoryItems.size();
//                qDebug()<<itemSize;
//                obj.InventoryItems[itemSize-1].subItems.append(item);
//                obj.InventoryItems[itemSize-1].subItemsTotal +=item.subTotal;
//            }
//        }
//    }
//    //    qDebug()<<qStrDetail<<vNo;
//    query.prepare(qStrMain);
//    query.bindValue(":vno",vNo);
//    if(!query.exec()){
//        qDebug()<<query.lastError();
//        qDebug()<<query.lastQuery();
//    }
//    else{

//        while(query.next()){
//            int i=0;
//            obj.voucherNumber = query.value(i++).toString();
//            obj.VoucherPrefix = query.value(i++).toString();
//            obj.VoucherDate = query.value(i++).toDate();
//            obj.ledgerObject.LedgerID = query.value(i++).toString();
//            obj.grandTotal = query.value(i++).toFloat();
//            obj.ClientApprovalStatus =query.value(i++).toBool();
//            obj.ManagerApprovalStatus = query.value(i++).toBool();
//            obj.status = query.value(i++).toInt();
//            obj.timestamp = query.value(i++).toDateTime();
//            obj.Contact = addHelper->getContact(query.value(i++).toInt());
//            obj.AddedById = query.value(i++).toInt();
//            obj.SalesmanID = query.value(i++).toInt();
//            obj.narration = query.value(i++).toString();
//            obj.LPO = query.value(i++).toString();
//            obj.ledgerObject.LedgerName = ledHelper->getLedgerNameByID(obj.Contact.ledgerId );
//            obj.discountinAmount = query.value(i++).toFloat();
//            obj.discountPercent = query.value(i++).toFloat();
//            obj.discount = query.value(i++).toFloat();
//            obj.POCName= query.value(i++).toString();
//            obj.POCPhone= query.value(i++).toString();
//            obj.POCEmail= query.value(i++).toString();
//            obj.Location= query.value(i++).toString();


//        }
//    }
//    return obj;
}

QList<GeneralVoucherDataObject> QuotationDatabaseHelper::getVoucherListByDate(QDate dateFrom, QDate dateTo)
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
            + Quotation_Main_VoucherStatus +", "
            + Quotation_Main_TimeStamp+", "
            + Quotation_Main_Contact_ID + ", "
            + Quotation_Main_Added_By + ", "
            + Quotation_Main_Salesman_ID + ", "
            + Quotation_Main_Narration + ", "
            + Quotation_Main_LPO_Ref + ", "
            + Quotation_Main_Revision_Number + " "



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
            item.RevisionNo = query.value(i++).toInt();
            vouchersList.append(item);
        }
    }

    return vouchersList;
}

int QuotationDatabaseHelper::setClientApproval(QString vid, bool value, QString LPO, QDate deliveryDate, QString notes)
{
    this->startTransaction();
    QMap<QString,QVariant> map1;
    QMap<QString,QVariant> clause;

    map1.insert(Quotation_Main_Client_Approval_Status,value);
    map1.insert(Quotation_Main_LPO_Ref,LPO);
    map1.insert(Quotation_Main_Delivery_Date,deliveryDate);

    clause.insert(Quotation_Details_Voucher_No,vid);


    db->updateRecordsPlus(map1,clause,Quotation_Main_TableName);
    return commit();

}

int QuotationDatabaseHelper::setManagerApproval(QString vid, bool value)
{
    this->startTransaction();
    QMap<QString,QVariant> map1;
    QMap<QString,QVariant> clause;

    map1.insert(Quotation_Main_Client_Approval_Status,value);
    clause.insert(Quotation_Details_Voucher_No,vid);

    db->updateRecordsPlus(map1,clause,Quotation_Main_TableName);
    return commit();


}

bool QuotationDatabaseHelper::getClientApprovalStatus(QString vid)
{
    bool status = false;
    QString qStr ="SELECT "
            + Quotation_Main_Client_Approval_Status
            + " FROM "
            + Quotation_Main_TableName
            + " WHERE "
            + Quotation_Main_Voucher_No + "='"+vid +"'";
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

bool QuotationDatabaseHelper::getManagerApprovalStatus(QString vid)
{
    bool status = false;
    QString qStr ="SELECT "
            + Quotation_Main_Manager_Approval_Status
            + " FROM "
            + Quotation_Main_TableName
            + " WHERE "
            + Quotation_Main_Voucher_No + "='"+vid +"'";
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

int QuotationDatabaseHelper::updateQuotationStatus(QString quotationVoucherNumber, QString status)
{
    this->startTransaction();

    QMap<QString,QVariant> map;
    QMap<QString,QVariant> clause;

    map.insert(Quotation_Main_Converted_To_Sales_Order,status);
    clause.insert(Quotation_Main_Voucher_No,quotationVoucherNumber);

    db->updateRecordsPlus(map,clause,Quotation_Main_TableName);



    return commit();

}

void QuotationDatabaseHelper::setItemVoucherStatus(float qty,QString id)
{
    QString qStr ="SELECT "+itemVoucherStatus+ " from "+ Quotation_Details_TableName +
            " WHERE "+Quotation_Details_Requirement_ItemID + " = " + id;
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
    clause.insert(Quotation_Details_Requirement_ItemID,id);
    db->updateRecordsPlus(map,clause,Quotation_Details_TableName);
}

void QuotationDatabaseHelper::resetItemVoucherStatus(float qty,QString id)
{
    QString qStr ="SELECT "+itemVoucherStatus+ " from "+ Quotation_Details_TableName +
            " WHERE "+Quotation_Details_Requirement_ItemID + " = " + id;
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
    clause.insert(Quotation_Details_Requirement_ItemID,id);
    db->updateRecordsPlus(map,clause,Quotation_Details_TableName);
}

QMap<QString, float> QuotationDatabaseHelper::getAllUnimportedQuotes(QString ledid, QDate date)

{
    QMap<QString, float> data;
    QString qStr ="";
    qStr += " SELECT QTY,VOUCHER_NO from ";
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QMap<QString,float> vouchers;

    if(query.exec(qStr)){
        while(query.next()){

        }

    }
    else{

    }

    return data;

}

int QuotationDatabaseHelper::getFileCount(QString quoteNumber)
{
    QString qStr = "SELECT " + Quotation_Details_id + " FROM " +Quotation_Details_TableName + " WHERE " + Quotation_Details_Voucher_No
            + " = :ID";

    int cnt =0;
    QString qStr1 = "SELECT Count("+ Quotation_Documents_id+") FROM "+Quotation_Documents_TableName +" WHERE "
            +Quotation_Documents_ReqId + " IN ( "+ qStr + ") or "+Quotation_Documents_QuotationNumber  + "= :id1";
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr1);
    query.bindValue(":ID",quoteNumber);
    query.bindValue(":id1",quoteNumber);

    if(!query.exec()){

    }
    else{
        while(query.next()){
            cnt = query.value(0).toInt();
        }
    }
    return cnt;

}

QByteArray QuotationDatabaseHelper::getFilewithId(int id)
{
    QByteArray file;

    QString qStr = "Select "+Quotation_Documents_File_Data+ " from "+ Quotation_Documents_TableName
            + " where " + Quotation_Documents_id + "=:id";
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);
    query.bindValue(":id",id);
    if(query.exec()){
        while(query.next())
            file=query.value(0).toByteArray();
    }
    else{
        qDebug()<<query.lastError();
    }

    return file;
}

QList<DocumentsDataModel> QuotationDatabaseHelper::getALLFilesWithReqID(int reqID,int tagType)
{
    QList<DocumentsDataModel> list;
    QString qStr = "select "
            + Quotation_Documents_id
            + ", " + Quotation_Documents_File_Name
            + ", " + Quotation_Documents_File_Type
            + ", " + Quotation_Documents_File_Description
            + ", " + Quotation_Documents_AddedByID
            + ", " + Quotation_Documents_TimeStamp
            + ", " + Quotation_Documents_TagType
            + ", " + Quotation_Documents_SearchTag
            + ", " + Quotation_Documents_ClientApprovalStatus

            + " FROM "+  Quotation_Documents_TableName
            + " WHERE "+ Quotation_Documents_ReqId  + "=:id"
            + " AND "  + Quotation_Documents_TagType + " <= :tagType";
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);
    query.bindValue(":id",reqID);
    query.bindValue(":tagType",tagType);

    if(query.exec()){
        while(query.next()){
            DocumentsDataModel x;
            x.id = query.value(0).toInt();
            x.filename = query.value(1).toString();
            x.fileType = query.value(2).toString();
            x.fileDescr = query.value(3).toString();
            x.addedByID = query.value(4).toInt();
            x.timestamp = query.value(5).toDateTime();
            x.TagType = query.value(6).toString();
            x.Searchtag = query.value(7).toString();
            x.status = query.value(8).toInt();

            list.append(x);
        }
    }
    else{
        qDebug()<<query.lastError();
    }

    return list;
}

QList<DocumentsDataModel> QuotationDatabaseHelper::getALLFilesWithQuoteNo(QString quoteNo , int tagType)
{
    QList<DocumentsDataModel> list;
    QString qStr = "select "
            + Quotation_Documents_id
            + ", " + Quotation_Documents_File_Name
            + ", " + Quotation_Documents_File_Type
            + ", " + Quotation_Documents_File_Description
            + ", " + Quotation_Documents_AddedByID
            + ", " + Quotation_Documents_TimeStamp
            + ", " + Quotation_Documents_TagType
            + ", " + Quotation_Documents_SearchTag
            + ", " + Quotation_Documents_ClientApprovalStatus

            + " FROM "+  Quotation_Documents_TableName
            + " WHERE "+ Quotation_Documents_QuotationNumber  + "=:id"
            + " ORDER BY "+Quotation_Documents_ReqId;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);
    query.bindValue(":id",quoteNo);
    if(query.exec()){
        while(query.next()){
            DocumentsDataModel x;
            x.id = query.value(0).toInt();
            x.filename = query.value(1).toString();
            x.fileType = query.value(2).toString();
            x.fileDescr = query.value(3).toString();
            x.addedByID = query.value(4).toInt();
            x.timestamp = query.value(5).toDateTime();
            x.TagType = query.value(6).toString();
            x.Searchtag = query.value(7).toString();
            x.status = query.value(7).toInt();

            list.append(x);
        }
    }
    else{
        qDebug()<<query.lastError();
    }

    return list;
}

int QuotationDatabaseHelper::insertQImage(DocumentsDataModel item)
{

    QMap<QString,QVariant> map;

    map.insert(Quotation_Documents_File_Name,item.filename);
    map.insert(Quotation_Documents_File_Description,item.fileDescr);
    map.insert(Quotation_Documents_File_Data,item.fileObj);
    map.insert(Quotation_Documents_File_Type,item.fileType);
    map.insert(Quotation_Documents_QuotationNumber,item.quoteNo);

    map.insert(Quotation_Documents_ReqId,item.ReqID);
    map.insert(Quotation_Documents_ProjectId,item.projectID);
    map.insert(Quotation_Documents_AddedByID,item.addedByID);
    map.insert(Quotation_Documents_TagType,item.TagType);
    map.insert(Quotation_Documents_SearchTag,item.Searchtag);

    map.insert(Quotation_Documents_ClientApprovalStatus,item.status);
    startTransaction();
    qint64 n = db->insertRecordsPlus(map,Quotation_Documents_TableName);
    commit();
    return n;
}

int QuotationDatabaseHelper::updateQImage(DocumentsDataModel item, int id)
{
    startTransaction();
    QMap<QString,QVariant> map;

    map.insert(Quotation_Documents_File_Name,item.filename);
    map.insert(Quotation_Documents_File_Description,item.fileDescr);
    map.insert(Quotation_Documents_File_Data,item.fileObj);
    map.insert(Quotation_Documents_File_Type,item.fileType);
    map.insert(Quotation_Documents_QuotationNumber,item.quoteNo);
    map.insert(Quotation_Documents_ReqId,item.ReqID);
    map.insert(Quotation_Documents_ProjectId,item.projectID);
    map.insert(Quotation_Documents_AddedByID,item.addedByID);
    map.insert(Quotation_Documents_TagType,item.TagType);
    map.insert(Quotation_Documents_SearchTag,item.Searchtag);
    map.insert(Quotation_Documents_ClientApprovalStatus,item.status);


    QMap<QString,QVariant> clause;

    clause.insert(Quotation_Documents_id,id);

    db->updateRecordsPlus(map,clause,Quotation_Documents_TableName);


    return commit();
}

int QuotationDatabaseHelper::deleteQImage(int id)
{
    startTransaction();
    QMap<QString,QVariant> map;

    map.insert(Quotation_Documents_id,id);

    db->deleteRecordsPlus(map,Quotation_Documents_TableName);
    return commit();
}

QString QuotationDatabaseHelper::getRevisionNumber(QString vNO)
{
    QString rev = "";
    QString qStr ="SELECT "
            + Quotation_Main_Revision_Number
            + " FROM "
            + Quotation_Main_TableName
            + " WHERE "
            + Quotation_Main_Voucher_No + "='"+vNO +"'";
    ;
    qDebug()<<qStr;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);

    if(query.exec()){
        if(query.first()){
            rev = query.value(0).toString();
        }
    }
    else{
        qDebug()<<qStr;
        qDebug()<<query.lastError();
    }
    return rev;
}

QString QuotationDatabaseHelper::setQuotationStatus(int status,QString qNo)
{
    QMap<QString,QVariant> map;
    QMap<QString,QVariant> clause;

    map.insert(Quotation_Main_VoucherStatus,status);
    clause.insert(Quotation_Main_Voucher_No,qNo);
    db->updateRecordsPlus(map,clause,Quotation_Main_TableName);
    return qNo;
}

int QuotationDatabaseHelper::setImageApprovalStatusByID(int id,int status)
{
    QMap<QString,QVariant> map;
    QMap<QString,QVariant> clause;

    map.insert(Quotation_Documents_ClientApprovalStatus,status);

    clause.insert(Quotation_Documents_id,id);
    db->updateRecordsPlus(map,clause,Quotation_Documents_TableName);
}

int QuotationDatabaseHelper::getQuotationStatus(QString qNo)
{
    int rev = 0;
    QString qStr ="SELECT "
            + Quotation_Main_VoucherStatus
            + " FROM "
            + Quotation_Main_TableName
            + " WHERE "
            + Quotation_Main_Voucher_No + "='"+qNo +"'";
    ;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);

    if(query.exec()){
        if(query.first()){
            rev = query.value(0).toInt();
        }
    }
    else{
        qDebug()<<qStr;
        qDebug()<<query.lastError();
    }
    return rev;
}




