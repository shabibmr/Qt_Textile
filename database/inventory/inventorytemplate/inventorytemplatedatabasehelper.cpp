#include "inventorytemplatedatabasehelper.h"


InventoryTemplateDatabaseHelper::InventoryTemplateDatabaseHelper()
{
    db = new DatabaseHelper();
}

InventoryTemplateDatabaseHelper::~InventoryTemplateDatabaseHelper()
{

}

int InventoryTemplateDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

int InventoryTemplateDatabaseHelper::insertInventoryTemplate(GeneralVoucherDataObject *obj)
{
    this->startTransaction();
    QMap<QString,QVariant> map;

    GeneralVoucherDatabaseHelper vHelper;
    obj->voucherNumber = vHelper.setNextVoucherNoByType(VoucherType, obj->VoucherPrefix, Inventory_Template_Main_TableName );

    if(obj->TransactionId.length()==0)
    obj->TransactionId=QString::number(QDateTime::currentDateTime().toTime_t()) + obj->VoucherPrefix + db->clientId;

    map.insert(Inventory_Template_Main_Voucher_No,obj->voucherNumber);
    map.insert(Inventory_Template_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Inventory_Template_Main_Date_Created,obj->VoucherDate);
    map.insert(Inventory_Template_Main_Voucher_Date,obj->VoucherDate);
    map.insert(Inventory_Template_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    map.insert(Inventory_Template_Main_Billing_Name,obj->ledgerObject.LedgerName);
    map.insert(Inventory_Template_Main_Total,obj->grandTotal);
    map.insert(Inventory_Template_Main_Narration,obj->narration);
    map.insert(Inventory_Template_Main_Added_By,obj->AddedBy);
    map.insert(Inventory_Template_Main_Delivery_Date,obj->DeliveryDate);
    map.insert(Inventory_Template_Main_Completion_Probability,obj->CompletionProbability);
    map.insert(Inventory_Template_Main_Credit_Period,obj->ledgerObject.Credit_Period);
    map.insert(Inventory_Template_Main_POC_Name,obj->ledgerObject.ContactPersonName);
    map.insert(Inventory_Template_Main_POC_Phone,obj->ledgerObject.ContactPersonNumber);
    map.insert(Inventory_Template_Main_POC_Email,obj->ledgerObject.ContactPersonEmail);
    map.insert(Inventory_Template_Main_Project_ID,obj->ProjectId);
    map.insert(Inventory_Template_Main_Revision_Number,obj->RevisionNo);
    map.insert(Inventory_Template_Main_Converted_To_Sales_Order,obj->ConvertedToSalesOrder);
    map.insert(Inventory_Template_Main_Quotation_Dropped,obj->QuotationDropped);
    map.insert(Inventory_Template_Main_Quotation_Dropped_Reason,obj->QuotationDroppedReason);
    map.insert(Inventory_Template_Main_Requirement_Voucher_No,obj->RequirementVoucherNo);
    map.insert(Inventory_Template_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Inventory_Template_Main_TransactionId, obj->TransactionId);
    map.insert(Inventory_Template_Main_TimeStampC,QDateTime::currentDateTime());
    map.insert(Inventory_Template_Main_Last_Updated,QDateTime::currentDateTime());

    db->insertRecordsPlus(map,Inventory_Template_Main_TableName);

    for(CompoundItemDataObject compitem:obj->InventoryItems){
        QMap<QString,QVariant> map1;
        inventoryItemDataModel item = compitem.BaseItem;
        if(item.ItemReqUuid == "X")
            item.ItemReqUuid = db->getUUID();

        map1.insert(Inventory_Template_Details_Voucher_No,obj->voucherNumber);
        map1.insert(Inventory_Template_Details_Voucher_Prefix,obj->VoucherPrefix);
        map1.insert(Inventory_Template_Details_Voucher_Date,obj->VoucherDate);
        map1.insert(Inventory_Template_Details_Inventory_Item_ID,item.ItemID);
        map1.insert(Inventory_Template_Details_Quantity,item.quantity);
        map1.insert(Inventory_Template_Details_UOM_ID,item.uomObject._id);

        map1.insert(Inventory_Template_Details_Price,item.price);
        map1.insert(Inventory_Template_Details_PriceLevel,item.PriceLevel);
        map1.insert(Inventory_Template_Details_isCompoundItem,item.IsCompoundItem);
        map1.insert(Inventory_Template_Details_subItemOff_list_ID,0);
        map1.insert(Inventory_Template_Details_Discount_Percent,item.discountPercentage);
        map1.insert(Inventory_Template_Details_Discount_Amount,item.discount);
        map1.insert(Inventory_Template_Details_Subtotal,item.subTotal);
        map1.insert(Inventory_Template_Details_Project_ID,obj->ProjectId);
        map1.insert(Inventory_Template_Details_Item_Narration,item.narration);
        map1.insert(Inventory_Template_Details_Item_Description,item.ItemDescription);
        map1.insert(Inventory_Template_Details_Salesman_ID,obj->SalesmanID);
        map1.insert(Inventory_Template_Details_Requirement_ItemID,item.ItemReqUuid);
        map1.insert(Inventory_Template_Details_TaxRate, item.taxRate);
        map1.insert(Inventory_Template_Details_TaxAmount, item.taxAmount);

        int basid = db->insertRecordsPlus(map1,Inventory_Template_Details_TableName);

        for(inventoryItemDataModel subitem:compitem.subItems){
            QMap<QString,QVariant> map2;
            if(subitem.ItemReqUuid == "X")
                subitem.ItemReqUuid = db->getUUID();
            map2.insert(Inventory_Template_Details_Voucher_No,obj->voucherNumber);
            map2.insert(Inventory_Template_Details_Voucher_Prefix,obj->VoucherPrefix);
            map2.insert(Inventory_Template_Details_Voucher_Date,obj->VoucherDate);
            map2.insert(Inventory_Template_Details_Inventory_Item_ID,subitem.ItemID);
            map2.insert(Inventory_Template_Details_Quantity,subitem.quantity);
            map2.insert(Inventory_Template_Details_UOM_ID,subitem.uomObject._id);
            map2.insert(Inventory_Template_Details_Price,subitem.price);
            map2.insert(Inventory_Template_Details_PriceLevel,subitem.PriceLevel);
            map2.insert(Inventory_Template_Details_isCompoundItem,subitem.IsCompoundItem);
            map2.insert(Inventory_Template_Details_subItemOff_list_ID,basid);
            map2.insert(Inventory_Template_Details_Discount_Percent,subitem.discountPercentage);
            map2.insert(Inventory_Template_Details_Discount_Amount,subitem.discount);
            map2.insert(Inventory_Template_Details_Subtotal,subitem.subTotal);
            map2.insert(Inventory_Template_Details_Project_ID,obj->ProjectId);
            map2.insert(Inventory_Template_Details_Item_Narration,subitem.narration);
            map2.insert(Inventory_Template_Details_Item_Description,subitem.ItemDescription);
            map2.insert(Inventory_Template_Details_Salesman_ID,obj->SalesmanID);
            map2.insert(Inventory_Template_Details_Requirement_ItemID,subitem.ItemReqUuid);
            map2.insert(Inventory_Template_Details_TaxRate, subitem.taxRate);
            map2.insert(Inventory_Template_Details_TaxAmount, subitem.taxAmount);

            db->insertRecordsPlus(map2,Inventory_Template_Details_TableName);
        }
    }
    for(LedgerMasterDataModel ledger:obj->ledgersList){
        QMap <QString,QVariant> ledMap;
        ledMap.insert(Proxy_Transaction_Helper_VoucherType,VoucherType);
        ledMap.insert(Proxy_Transaction_Helper_VoucherNo,obj->voucherNumber);
        ledMap.insert(Proxy_Transaction_Helper_VoucherPrefix,obj->VoucherPrefix);
        ledMap.insert(Proxy_Transaction_Helper_VoucherDate,obj->VoucherDate);
        ledMap.insert(Proxy_Transaction_Helper_ProjectID,obj->ProjectId);
        ledMap.insert(Proxy_Transaction_Helper_SalesManID,obj->SalesmanID);
        ledMap.insert(Proxy_Transaction_Helper_TranactionAmount,ledger.amount);
        ledMap.insert(Proxy_Transaction_Helper_CreditLedgerID,ledger.LedgerID);
        ledMap.insert(Proxy_Transaction_Helper_DebitLedgerID,obj->ledgerObject.LedgerID);
        ledMap.insert(Proxy_Transaction_Helper_Narration,obj->ledgerObject.narration);
        db->insertRecordsPlus(ledMap,Proxy_Transaction_Helper_TableName);
    }

    //    return commit();

    bool c=commit();

    return c;
}

int InventoryTemplateDatabaseHelper::updateInventoryTemplate(GeneralVoucherDataObject *obj, QString oldID)
{
    this->startTransaction();

    QMap<QString,QVariant> map5;
    map5.insert(Inventory_Template_Details_Voucher_No,oldID);
    map5.insert(Inventory_Template_Details_Voucher_Prefix,obj->VoucherPrefix);
    db->deleteRecordsPlus(map5,Inventory_Template_Details_TableName);

    QMap<QString,QVariant> map;
    map.insert(Inventory_Template_Main_Voucher_No,obj->voucherNumber);
    map.insert(Inventory_Template_Main_Date_Created,obj->VoucherDate);
    map.insert(Inventory_Template_Main_Voucher_Date,obj->VoucherDate);
    map.insert(Inventory_Template_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    map.insert(Inventory_Template_Main_Billing_Name,obj->ledgerObject.LedgerName);
    map.insert(Inventory_Template_Main_Total,obj->grandTotal);
    map.insert(Inventory_Template_Main_Narration,obj->narration);
    map.insert(Inventory_Template_Main_Added_By,obj->AddedBy);
    map.insert(Inventory_Template_Main_Delivery_Date,obj->DeliveryDate);
    map.insert(Inventory_Template_Main_Completion_Probability,obj->CompletionProbability);
    map.insert(Inventory_Template_Main_Credit_Period,obj->ledgerObject.Credit_Period);
    map.insert(Inventory_Template_Main_POC_Name,obj->ledgerObject.ContactPersonName);
    map.insert(Inventory_Template_Main_POC_Phone,obj->ledgerObject.ContactPersonNumber);
    map.insert(Inventory_Template_Main_POC_Email,obj->ledgerObject.ContactPersonEmail);
    map.insert(Inventory_Template_Main_Project_ID,obj->ProjectId);
    map.insert(Inventory_Template_Main_Revision_Number,obj->RevisionNo);
    map.insert(Inventory_Template_Main_Converted_To_Sales_Order,obj->ConvertedToSalesOrder);
    map.insert(Inventory_Template_Main_Quotation_Dropped,obj->QuotationDropped);
    map.insert(Inventory_Template_Main_Quotation_Dropped_Reason,obj->QuotationDroppedReason);
    map.insert(Inventory_Template_Main_Requirement_Voucher_No,obj->RequirementVoucherNo);
    map.insert(Inventory_Template_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Inventory_Template_Main_TransactionId, obj->TransactionId);
    map.insert(Inventory_Template_Main_Last_Updated,QDateTime::currentDateTime());

    QMap<QString,QVariant> map3;
    map3.insert(Inventory_Template_Main_Voucher_No,oldID);
    map3.insert(Inventory_Template_Main_Voucher_Prefix,obj->VoucherPrefix);

    db->updateRecordsPlus(map,map3,Inventory_Template_Main_TableName);

    for(CompoundItemDataObject compitem:obj->InventoryItems){
        QMap<QString,QVariant> map1;
        inventoryItemDataModel item = compitem.BaseItem;
        map1.insert(Inventory_Template_Details_Voucher_No,obj->voucherNumber);
        map1.insert(Inventory_Template_Details_Voucher_Prefix,obj->VoucherPrefix);
        map1.insert(Inventory_Template_Details_Voucher_Date,obj->VoucherDate);
        map1.insert(Inventory_Template_Details_Inventory_Item_ID,item.ItemID);
        map1.insert(Inventory_Template_Details_Quantity,item.quantity);
        map1.insert(Inventory_Template_Details_UOM_ID,item.uomObject._id);
        map1.insert(Inventory_Template_Details_Price,item.price);
        map1.insert(Inventory_Template_Details_PriceLevel,item.PriceLevel);
        map1.insert(Inventory_Template_Details_isCompoundItem,item.IsCompoundItem);
        map1.insert(Inventory_Template_Details_subItemOff_list_ID,0);
        map1.insert(Inventory_Template_Details_Discount_Percent,item.discountPercentage);
        map1.insert(Inventory_Template_Details_Discount_Amount,item.discount);
        map1.insert(Inventory_Template_Details_Subtotal,item.subTotal);
        map1.insert(Inventory_Template_Details_Project_ID,obj->ProjectId);
        map1.insert(Inventory_Template_Details_Item_Narration,item.narration);
        map1.insert(Inventory_Template_Details_Item_Description,item.ItemDescription);
        map1.insert(Inventory_Template_Details_Salesman_ID,obj->SalesmanID);
        map1.insert(Inventory_Template_Details_Requirement_ItemID,item.ItemReqUuid);
        map1.insert(Inventory_Template_Details_TaxRate, item.taxRate);
        map1.insert(Inventory_Template_Details_TaxAmount, item.taxAmount);

        int basid = db->insertRecordsPlus(map1,Inventory_Template_Details_TableName);

        for(inventoryItemDataModel subitem:compitem.subItems){
            QMap<QString,QVariant> map2;
            map2.insert(Inventory_Template_Details_Voucher_No,obj->voucherNumber);
            map2.insert(Inventory_Template_Details_Voucher_Prefix,obj->VoucherPrefix);
            map2.insert(Inventory_Template_Details_Voucher_Date,obj->VoucherDate);
            map2.insert(Inventory_Template_Details_Inventory_Item_ID,subitem.ItemID);
            map2.insert(Inventory_Template_Details_Quantity,subitem.quantity);
            map2.insert(Inventory_Template_Details_UOM_ID,subitem.uomObject._id);
            map2.insert(Inventory_Template_Details_Price,subitem.price);
            map2.insert(Inventory_Template_Details_PriceLevel,subitem.PriceLevel);
            map2.insert(Inventory_Template_Details_isCompoundItem,subitem.IsCompoundItem);
            map2.insert(Inventory_Template_Details_subItemOff_list_ID,basid);
            map2.insert(Inventory_Template_Details_Discount_Percent,subitem.discountPercentage);
            map2.insert(Inventory_Template_Details_Discount_Amount,subitem.discount);
            map2.insert(Inventory_Template_Details_Subtotal,subitem.subTotal);
            map2.insert(Inventory_Template_Details_Project_ID,obj->ProjectId);
            map2.insert(Inventory_Template_Details_Item_Narration,subitem.narration);
            map2.insert(Inventory_Template_Details_Item_Description,subitem.ItemDescription);
            map2.insert(Inventory_Template_Details_Salesman_ID,obj->SalesmanID);
            map2.insert(Inventory_Template_Details_Requirement_ItemID,subitem.ItemReqUuid);
            map2.insert(Inventory_Template_Details_TaxRate, subitem.taxRate);
            map2.insert(Inventory_Template_Details_TaxAmount, subitem.taxAmount);

            db->insertRecordsPlus(map2,Inventory_Template_Details_TableName);
        }
    }

    QMap<QString,QVariant> leddel;
    leddel.insert(Proxy_Transaction_Helper_VoucherNo,oldID);
    leddel.insert(Proxy_Transaction_Helper_VoucherType,VoucherType);
    db->deleteRecordsPlus(leddel,Proxy_Transaction_Helper_TableName);

    for(LedgerMasterDataModel ledger:obj->ledgersList){
        QMap <QString,QVariant> ledMap;
        ledMap.insert(Proxy_Transaction_Helper_VoucherType,VoucherType);
        ledMap.insert(Proxy_Transaction_Helper_VoucherNo,obj->voucherNumber);
        ledMap.insert(Proxy_Transaction_Helper_VoucherPrefix,obj->VoucherPrefix);
        ledMap.insert(Proxy_Transaction_Helper_VoucherDate,obj->VoucherDate);
        ledMap.insert(Proxy_Transaction_Helper_ProjectID,obj->ProjectId);
        ledMap.insert(Proxy_Transaction_Helper_SalesManID,obj->SalesmanID);
        ledMap.insert(Proxy_Transaction_Helper_TranactionAmount,ledger.amount);
        ledMap.insert(Proxy_Transaction_Helper_CreditLedgerID,ledger.LedgerID);
        ledMap.insert(Proxy_Transaction_Helper_DebitLedgerID,obj->ledgerObject.LedgerID);
        ledMap.insert(Proxy_Transaction_Helper_Narration,obj->ledgerObject.narration);
        db->insertRecordsPlus(ledMap,Proxy_Transaction_Helper_TableName);
    }

    //    return commit();

    int c=commit();

    return c;
}

int InventoryTemplateDatabaseHelper::deleteInventoryTemplate(QString oldID)
{
    this->startTransaction();
    QMap<QString,QVariant> map3;
    map3.insert(Inventory_Template_Main_Voucher_No,oldID);
    db->deleteRecordsPlus(map3,Inventory_Template_Main_TableName);

    QMap<QString,QVariant> map4;
    map4.insert(Inventory_Template_Details_Voucher_No,oldID);
    db->deleteRecordsPlus(map4,Inventory_Template_Details_TableName);

    QMap<QString,QVariant> leddel;
    leddel.insert(Proxy_Transaction_Helper_VoucherNo,oldID);
    leddel.insert(Proxy_Transaction_Helper_VoucherType,VoucherType);
    db->deleteRecordsPlus(leddel,Proxy_Transaction_Helper_TableName);


    return commit();
}

int InventoryTemplateDatabaseHelper::commit()
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

int InventoryTemplateDatabaseHelper::getMaxID()
{
    QString qStr = "SELECT MAX("+Inventory_Template_Main_id+") FROM " + Inventory_Template_Main_TableName;
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

QSqlQueryModel *InventoryTemplateDatabaseHelper::getTemplatesAsModel()
{
    LedgerMasterDatabaseHelper ledHelper;

    QString qStr ="SELECT ";
    qStr += "'V' , ";
    qStr += Inventory_Template_Main_Voucher_No +", ";
    qStr += "'" + VoucherType + "', " ;
    qStr += Inventory_Template_Main_Voucher_Prefix + ", ";
    qStr += Inventory_Template_Main_Voucher_Date+" as 'Voucher Date', ";
    qStr += " CONCAT(" + Inventory_Template_Main_Voucher_Prefix + ", '-', " + Inventory_Template_Main_Voucher_No+ ") as 'Voucher No', ";

    qStr += "(SELECT "+ledHelper.Ledger_Master_LedgerName + " FROM "+ ledHelper.Ledger_Master_TableName;
    qStr += " WHERE "+ledHelper.Ledger_Master_LedgerID + "= som.";
    qStr += Inventory_Template_Main_Ledger_ID + ") Ledger, ";

    qStr += Inventory_Template_Main_Total ;
    qStr += " FROM " + Inventory_Template_Main_TableName;
    qStr += " som ";

    return DatabaseHelper::getQueryModel(qStr);
}

GeneralVoucherDataObject *InventoryTemplateDatabaseHelper::getVoucherByVoucherNoAsPtr(QString vNo)
{

}
