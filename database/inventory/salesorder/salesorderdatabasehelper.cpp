#include "salesorderdatabasehelper.h"

#include <database/Settings/ConfigurationSettingsDatabaseHelper.h>
#include "database/inventory/measurements/measurementsdatabasehelper.h"
QString SalesOrderDatabaseHelper::Sales_Order_Main_TableName = "Sales_Order_Main";

QString SalesOrderDatabaseHelper::Sales_Order_Main_id = "_id";
QString SalesOrderDatabaseHelper::Sales_Order_Main_Voucher_No = "Voucher_No";
QString SalesOrderDatabaseHelper::Sales_Order_Main_Voucher_Prefix = "Voucher_Prefix";
QString SalesOrderDatabaseHelper::Sales_Order_Main_Date_Created = "Date_Created";
QString SalesOrderDatabaseHelper::Sales_Order_Main_Voucher_Date = "Voucher_Date";
QString SalesOrderDatabaseHelper::Sales_Order_Main_TimeStamp = "`TimeStamp`";
QString SalesOrderDatabaseHelper::Sales_Order_Main_TimeStampC = "TimeStamp";

QString SalesOrderDatabaseHelper::Sales_Order_Main_Last_Updated = "Last_Updated";
QString SalesOrderDatabaseHelper::Sales_Order_Main_Ledger_ID = "Ledger_ID";
QString SalesOrderDatabaseHelper::Sales_Order_Main_Billing_Name = "Billing_Name";
QString SalesOrderDatabaseHelper::Sales_Order_Main_Total = "Total";
QString SalesOrderDatabaseHelper::Sales_Order_Main_Narration = "Narration";
QString SalesOrderDatabaseHelper::Sales_Order_Main_Added_By = "Added_By";
QString SalesOrderDatabaseHelper::Sales_Order_Main_Delivery_Date = "Delivery_Date";
QString SalesOrderDatabaseHelper::Sales_Order_Main_Assigned_Time = "Assigned_Time";
QString SalesOrderDatabaseHelper::Sales_Order_Main_Completion_Probability = "Completion_Probability";
QString SalesOrderDatabaseHelper::Sales_Order_Main_Credit_Period = "Credit_Period";
QString SalesOrderDatabaseHelper::Sales_Order_Main_POC_Name = "POC_Name";
QString SalesOrderDatabaseHelper::Sales_Order_Main_POC_Phone = "POC_Phone";
QString SalesOrderDatabaseHelper::Sales_Order_Main_POC_Email = "POC_Email";
QString SalesOrderDatabaseHelper::Sales_Order_Main_Project_ID = "Project_ID";
QString SalesOrderDatabaseHelper::Sales_Order_Main_Revision_Number = "Revision_Number";
QString SalesOrderDatabaseHelper::Sales_Order_Main_Converted_To_Sales_Order = "Converted_To_Sales_Order";
QString SalesOrderDatabaseHelper::Sales_Order_Main_Quotation_Dropped = "Quotation_Dropped";
QString SalesOrderDatabaseHelper::Sales_Order_Main_Quotation_Dropped_Reason = "Quotation_Dropped_Reason";
QString SalesOrderDatabaseHelper::Sales_Order_Main_Requirement_Voucher_No = "Requirement_Voucher_No";
QString SalesOrderDatabaseHelper::Sales_Order_Main_Salesman_ID = "Salesman_ID";
QString SalesOrderDatabaseHelper::Sales_Order_Main_LPO = "LPO_Ref";
QString SalesOrderDatabaseHelper::Sales_Order_Main_VoucherStatus = "VoucherStatus";
QString SalesOrderDatabaseHelper::Sales_Order_Main_ModeOfService = "MODE_OF_SERVICE";
QString SalesOrderDatabaseHelper::Sales_Order_Main_Discount_In_Amount = "Discount_In_Amount";
QString SalesOrderDatabaseHelper::Sales_Order_Main_Discount_In_Percent = "Discount_In_Percent";
QString SalesOrderDatabaseHelper::Sales_Order_Main_VATAmount = "VAT_Amount";
QString SalesOrderDatabaseHelper::Sales_Order_Main_Contact_ID = "ContactID";
QString SalesOrderDatabaseHelper::Sales_Order_Main_Gross_Total = "Gross_Total";
QString SalesOrderDatabaseHelper::Sales_Order_Main_Reference = "Reference";
QString SalesOrderDatabaseHelper::Sales_Order_Main_Customer_Expecting_Date = "Customer_Expected_Date";
QString SalesOrderDatabaseHelper::Sales_Order_Main_Currency = "Currency";
QString SalesOrderDatabaseHelper::Sales_Order_Main_BoxesCount = "Boxes_Count";
QString SalesOrderDatabaseHelper::Sales_Order_Main_TotalWeight = "Total_Weight";
QString SalesOrderDatabaseHelper::Sales_Order_Main_LRNo = "LR_No";
QString SalesOrderDatabaseHelper::Sales_Order_Main_VoucherNoDisplay = "Voucher_No_Display";
QString SalesOrderDatabaseHelper::Sales_Order_Main_ReqVoucherList = "Requirement_Voucher_List";
QString SalesOrderDatabaseHelper::Sales_Order_Main_Origin = "Origin";
QString SalesOrderDatabaseHelper::Sales_Order_Main_RoundOff = "Round_Off";
QString SalesOrderDatabaseHelper::Sales_Order_Main_Location="Location";
QString SalesOrderDatabaseHelper::Sales_Order_Main_Generated_TransactionId = "TransactionId";

QString SalesOrderDatabaseHelper::Sales_Order_Details_TableName = "Sales_Order_Details";

QString SalesOrderDatabaseHelper::Sales_Order_Details_id = "_id";
QString SalesOrderDatabaseHelper::Sales_Order_Details_Voucher_No = "Voucher_No";
QString SalesOrderDatabaseHelper::Sales_Order_Details_Voucher_Prefix = "Voucher_Prefix";
QString SalesOrderDatabaseHelper::Sales_Order_Details_Voucher_Date = "Voucher_Date";
QString SalesOrderDatabaseHelper::Sales_Order_Details_Inventory_Item_ID = "Inventory_Item_ID";
QString SalesOrderDatabaseHelper::Sales_Order_Details_Quantity = "Quantity";
QString SalesOrderDatabaseHelper::Sales_Order_Details_QuantityFull = "Quantity_Full";
QString SalesOrderDatabaseHelper::Sales_Order_Details_QuantityDisc = "Quantity_Disc";

QString SalesOrderDatabaseHelper::Sales_Order_Details_Price = "Price";
QString SalesOrderDatabaseHelper::Sales_Order_Details_UOM_ID = "Uom_ID";
QString SalesOrderDatabaseHelper::Sales_Order_Details_PriceLevel = "PriceLevel";
QString SalesOrderDatabaseHelper::Sales_Order_Details_isCompoundItem = "isCompoundItem";
QString SalesOrderDatabaseHelper::Sales_Order_Details_subItemOff_list_ID = "subItemOff_list_ID";
QString SalesOrderDatabaseHelper::Sales_Order_Details_Discount_Percent = "Discount_Percent";
QString SalesOrderDatabaseHelper::Sales_Order_Details_Discount_Amount = "Discount_Amount";
QString SalesOrderDatabaseHelper::Sales_Order_Details_Subtotal = "Subtotal";
QString SalesOrderDatabaseHelper::Sales_Order_Details_Project_ID = "Project_ID";
QString SalesOrderDatabaseHelper::Sales_Order_Details_Item_Narration = "Item_Narration";
QString SalesOrderDatabaseHelper::Sales_Order_Details_Item_Description = "Item_Description";
QString SalesOrderDatabaseHelper::Sales_Order_Details_Salesman_ID = "Salesman_ID";
QString SalesOrderDatabaseHelper::Sales_Order_Details_Requirement_ItemID ="Requirement_ItemID";
QString SalesOrderDatabaseHelper::Sales_Order_Details_Length="Length";
QString SalesOrderDatabaseHelper::Sales_Order_Details_Dimension="Dimension";
QString SalesOrderDatabaseHelper::Sales_Order_Details_itemVoucherStatus ="ItemVoucherStatus";
QString SalesOrderDatabaseHelper::Sales_Order_Details_itemProductionStatus ="itemProductionStatus";
QString SalesOrderDatabaseHelper::Sales_Order_Details_TimeDate ="TimeDate";
QString SalesOrderDatabaseHelper::Sales_Order_Details_TaxRate = "Tax_Rate";
QString SalesOrderDatabaseHelper::Sales_Order_Details_TaxAmount = "Tax_Amount";
QString SalesOrderDatabaseHelper::Sales_Order_Details_ContainerCount = "ContainerCount";

QString SalesOrderDatabaseHelper::VoucherType =   GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesOrder);//"SALES ORDER";

QString SalesOrderDatabaseHelper::WorkOrder_tableName = "work_order_detail";
QString SalesOrderDatabaseHelper::WorkOrder_id ="_id";
QString SalesOrderDatabaseHelper::WorkOrder_SalesOrder_Id = "SalesOrder_Id";
QString SalesOrderDatabaseHelper::WorkOrder_requirement_no="requirement_no";

QString SalesOrderDatabaseHelper::WorkOrder_WorkOrder_id="WorkOrder_id";
QString SalesOrderDatabaseHelper::WorkOrder_qty_No="qty_No";
QString SalesOrderDatabaseHelper::WorkOrder_itemID="itemID";
QString SalesOrderDatabaseHelper::WorkOrder_SerialNo="SerialNo";
QString SalesOrderDatabaseHelper::WorkOrder_status="status";
QString SalesOrderDatabaseHelper::WorkOrder_TechnicanID="TechnicanID";
QString SalesOrderDatabaseHelper::WorkOrder_CompletedDate="CompletedDate";
QString SalesOrderDatabaseHelper::WorkOrder_TimeStamp="TimeStamp";
QString SalesOrderDatabaseHelper::WorkOrder_DN_id = "Dn_id";
QString SalesOrderDatabaseHelper::WorkOrder_Expected_Date="Expected_Date";


SalesOrderDatabaseHelper::SalesOrderDatabaseHelper(QString connString)
{
    db = new DatabaseHelper(connString);
    ledTransaction = new TransactionHelper();

}

SalesOrderDatabaseHelper::~SalesOrderDatabaseHelper()
{

}

int SalesOrderDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

QString SalesOrderDatabaseHelper::getNextVoucherNo(QString voucherPrefix)
{
    GeneralVoucherDatabaseHelper vHelper;
    QString voucherNumber = vHelper.getNextVoucherNoByType(VoucherType, voucherPrefix, Sales_Order_Main_TableName);
    return voucherNumber;

}

QPair<QString, QString> SalesOrderDatabaseHelper::getVoucherNoByTransactionId(QString transId)
{
    GeneralVoucherDatabaseHelper vHelper;
    return vHelper.getVoucherNoByTransactionId(transId, Sales_Order_Main_TableName);

}

QMap<QString, float> SalesOrderDatabaseHelper::getOccupiedTables(QString voucherPrefix)
{
    QString qStr = "SELECT REFERENCE, SUM(TOTAL) FROM SALES_ORDER_MAIN";
    qStr += " where "+ Sales_Order_Main_VoucherStatus +" in ('110','120','130','150','160')" ;
    qStr += " AND ";
    qStr += Sales_Order_Main_Voucher_Prefix +" like '"+voucherPrefix+"'";
    //    qStr += " AND ";
    qStr += " GROUP BY REFERENCE";

    QMap<QString,float> map;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    if(query.exec(qStr)){
        while(query.next()){
            map.insert(query.value(0).toString(),query.value(1).toFloat());
        }
    }
    else{
        qDebug()<<qStr;
        qDebug()<<query.lastError();

    }
    return map;

}


int SalesOrderDatabaseHelper::insertVoucher(GeneralVoucherDataObject *obj)
{
    this->startTransaction();

    //    obj->voucherNumber = QString::number(getMaxIDwithPrefix(obj->VoucherPrefix));

    GeneralVoucherDatabaseHelper vHelper;
    obj->voucherNumber = vHelper.setNextVoucherNoByType(VoucherType, obj->VoucherPrefix, Sales_Order_Main_TableName );

    if(obj->voucherNumber.length()==0)
        obj->voucherNumber = QString::number(getMaxIDwithPrefix(obj->VoucherPrefix)+1);

    if(obj->TransactionId.length()==0)
        obj->TransactionId=QString::number(QDateTime::currentDateTime().toTime_t()) + obj->VoucherPrefix + db->clientId;

    if(obj->measurement!=nullptr){
        obj->QuotationDroppedReason = obj->measurement->contactMeasureID;
    }
    QMap<QString,QVariant> map;

    map.insert(Sales_Order_Main_Voucher_No,obj->voucherNumber);
    map.insert(Sales_Order_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Sales_Order_Main_Date_Created,obj->VoucherDate);
    map.insert(Sales_Order_Main_Voucher_Date,obj->VoucherDate);
    map.insert(Sales_Order_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    map.insert(Sales_Order_Main_Billing_Name,obj->ledgerObject.LedgerName);
    map.insert(Sales_Order_Main_Total,obj->grandTotal);
    map.insert(Sales_Order_Main_Narration,obj->narration);
    map.insert(Sales_Order_Main_Added_By,obj->AddedById);
    map.insert(Sales_Order_Main_Delivery_Date,obj->DeliveryDate);
    map.insert(Sales_Order_Main_Customer_Expecting_Date,obj->CustomerExpectingDate);
    map.insert(Sales_Order_Main_Completion_Probability,obj->CompletionProbability);
    map.insert(Sales_Order_Main_Credit_Period,obj->ledgerObject.Credit_Period);
    map.insert(Sales_Order_Main_POC_Name,obj->Contact.ContactName);
    map.insert(Sales_Order_Main_POC_Phone,obj->Contact.PhoneNumber);
    map.insert(Sales_Order_Main_POC_Email,obj->Contact.email);
    map.insert(Sales_Order_Main_Project_ID,obj->ProjectId);
    map.insert(Sales_Order_Main_Revision_Number,obj->RevisionNo);
    map.insert(Sales_Order_Main_Converted_To_Sales_Order,obj->ConvertedToSalesOrder);
    map.insert(Sales_Order_Main_Quotation_Dropped,obj->QuotationDropped);
    map.insert(Sales_Order_Main_Quotation_Dropped_Reason,obj->QuotationDroppedReason);
    map.insert(Sales_Order_Main_Requirement_Voucher_No,obj->RequirementVoucherNo);
    map.insert(Sales_Order_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Sales_Order_Main_LPO,obj->LPO);
    map.insert(Sales_Order_Main_ModeOfService,obj->ModeOfService);
    map.insert(Sales_Order_Main_Contact_ID,obj->Contact._id);
    map.insert(Sales_Order_Main_VATAmount,obj->taxTotalAmount);
    map.insert(Sales_Order_Main_VoucherStatus,obj->status);
    map.insert(Sales_Order_Main_Discount_In_Amount,obj->discountinAmount);
    map.insert(Sales_Order_Main_Discount_In_Percent,obj->discountPercent);
    map.insert(Sales_Order_Main_Gross_Total,obj->grossTotal);
    map.insert(Sales_Order_Main_Reference,obj->reference);
    map.insert(Sales_Order_Main_Generated_TransactionId, obj->TransactionId);
    map.insert(Sales_Order_Main_TimeStampC, obj->timestamp);
    map.insert(Sales_Order_Main_Last_Updated,QDateTime::currentDateTime());
    map.insert(Sales_Order_Main_Location,obj->Location);
    QJsonDocument doc(obj->ReqVoucherList);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    map.insert(Sales_Order_Main_ReqVoucherList,strJson);

    qint64 lastId = db->insertRecordsPlus(map,Sales_Order_Main_TableName);

    qDebug()<<"New ID = "<<lastId;
    //    QMap<QString,QVariant> idmap;
    //    idmap.insert(Sales_Order_Main_Voucher_No,QString::number(lastId));
    //    QMap<QString,QVariant> idmapClause;
    //    idmapClause.insert(Sales_Order_Main_id,lastId);
    //    db->updateRecordsPlus(idmap,idmapClause,Sales_Order_Main_TableName);
    //    qDebug()<<"New ID updated = "<<lastId;

    //    obj->voucherNumber = QString::number(lastId);

    for(CompoundItemDataObject compitem:obj->InventoryItems){
        QMap<QString,QVariant> map1;
        inventoryItemDataModel item = compitem.BaseItem;
        if(item.ItemReqUuid == "X")
            item.ItemReqUuid = db->getUUID();

        qDebug()<<item.ItemReqUuid<<"&&&&&&&&&&&";
        item.itemProductionStatus = QuotationStatus::SalesOrderCreated;

        map1.insert(Sales_Order_Details_Voucher_No,obj->voucherNumber);
        map1.insert(Sales_Order_Details_Voucher_Prefix,obj->VoucherPrefix);
        map1.insert(Sales_Order_Details_Voucher_Date,obj->VoucherDate);
        map1.insert(Sales_Order_Details_Inventory_Item_ID,item.ItemID);
        map1.insert(Sales_Order_Details_Quantity,item.quantity);
        map1.insert(Sales_Order_Details_QuantityDisc,item.discQuantity);
        map1.insert(Sales_Order_Details_QuantityFull,item.quantityFull);
        map1.insert(Sales_Order_Details_Price,item.price);
        map1.insert(Sales_Order_Details_PriceLevel,item.PriceLevel);
        map1.insert(Sales_Order_Details_isCompoundItem,item.IsCompoundItem);
        map1.insert(Sales_Order_Details_subItemOff_list_ID,0);
        map1.insert(Sales_Order_Details_Discount_Percent,item.discountPercentage);
        map1.insert(Sales_Order_Details_Discount_Amount,item.discountinAmount);
        map1.insert(Sales_Order_Details_Subtotal,item.subTotal);
        map1.insert(Sales_Order_Details_Project_ID,item.ProjectID);
        map1.insert(Sales_Order_Details_Item_Narration,item.narration);
        map1.insert(Sales_Order_Details_Item_Description,item.ItemDescription);
        map1.insert(Sales_Order_Details_Salesman_ID,obj->SalesmanID);
        map1.insert(Sales_Order_Details_Requirement_ItemID,item.ItemReqUuid);
        map1.insert(Sales_Order_Details_Length,item.length);
        map1.insert(Sales_Order_Details_Dimension,item.Dimension);
        map1.insert(Sales_Order_Details_itemProductionStatus,item.itemProductionStatus);
        map1.insert(Sales_Order_Details_itemVoucherStatus,item.itemVoucherStatus);
        map1.insert(Sales_Order_Details_UOM_ID,item.uomObject._id);
        map1.insert(Sales_Order_Details_TaxRate, item.taxRate);
        map1.insert(Sales_Order_Details_TaxAmount, item.taxAmount);
        map1.insert(Sales_Order_Details_TimeDate, item.manufactureDate);
        map1.insert(Sales_Order_Details_ContainerCount, item.requestQty);


        int basid = db->insertRecordsPlus(map1,Sales_Order_Details_TableName);

        for(inventoryItemDataModel subitem:compitem.subItems){
            QMap<QString,QVariant> map2;
            if(subitem.ItemReqUuid == "X")
                subitem.ItemReqUuid = db->getUUID();
            item.itemProductionStatus = QuotationStatus::SalesOrderCreated;
            map2.insert(Sales_Order_Details_Voucher_No,obj->voucherNumber);
            map2.insert(Sales_Order_Details_Voucher_Prefix,obj->VoucherPrefix);
            map2.insert(Sales_Order_Details_Voucher_Date,obj->VoucherDate);
            map2.insert(Sales_Order_Details_Inventory_Item_ID,subitem.ItemID);
            map2.insert(Sales_Order_Details_Quantity,subitem.quantity);
            map2.insert(Sales_Order_Details_Price,subitem.price);
            map2.insert(Sales_Order_Details_PriceLevel,subitem.PriceLevel);
            map2.insert(Sales_Order_Details_isCompoundItem,subitem.IsCompoundItem);
            map2.insert(Sales_Order_Details_subItemOff_list_ID,basid);
            map2.insert(Sales_Order_Details_Discount_Percent,subitem.discountPercentage);
            map2.insert(Sales_Order_Details_Discount_Amount,subitem.discount);
            map2.insert(Sales_Order_Details_Subtotal,subitem.subTotal);
            map2.insert(Sales_Order_Details_Project_ID,item.ProjectID);
            map2.insert(Sales_Order_Details_Item_Narration,subitem.narration);
            map2.insert(Sales_Order_Details_Item_Description,subitem.ItemDescription);
            map2.insert(Sales_Order_Details_Salesman_ID,obj->SalesmanID);
            map2.insert(Sales_Order_Details_Requirement_ItemID,subitem.ItemReqUuid);
            map2.insert(Sales_Order_Details_Length,subitem.length);
            map2.insert(Sales_Order_Details_Dimension,item.Dimension);
            map2.insert(Sales_Order_Details_itemProductionStatus,item.itemProductionStatus);
            map2.insert(Sales_Order_Details_UOM_ID,item.uomObject._id);
            map2.insert(Sales_Order_Details_TaxRate, subitem.taxRate);
            map2.insert(Sales_Order_Details_TaxAmount, subitem.taxAmount);
            db->insertRecordsPlus(map2,Sales_Order_Details_TableName);
        }
        //        QMap <QString,QVariant> ledMap;
        //        ledMap.insert(Proxy_Transaction_Helper_VoucherType,VoucherType);
        //        ledMap.insert(Proxy_Transaction_Helper_VoucherNo,obj->voucherNumber);
        //        ledMap.insert(Proxy_Transaction_Helper_VoucherPrefix,obj->VoucherPrefix);
        //        ledMap.insert(Proxy_Transaction_Helper_VoucherDate,obj->VoucherDate);
        //        ledMap.insert(Proxy_Transaction_Helper_ProjectID,obj->ProjectId);
        //        ledMap.insert(Proxy_Transaction_Helper_SalesManID,obj->SalesmanID);
        //        ledMap.insert(Proxy_Transaction_Helper_TranactionAmount,compitem.subItemsTotal+compitem.BaseItem.subTotal);
        //        ledMap.insert(Proxy_Transaction_Helper_DebitLedgerID,compitem.BaseItem.defaultSalesLedgerID);
        //        ledMap.insert(Proxy_Transaction_Helper_CreditLedgerID,obj->ledgerObject.LedgerID);
        //        ledMap.insert(Proxy_Transaction_Helper_Narration,obj->ledgerObject.narration);
        //        db->insertRecordsPlus(ledMap,Proxy_Transaction_Helper_TableName);

    }

    qDebug()<<"%%%%%%%%%%%%%5307"<<obj->ledgersList.size();

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

    obj->ledgersList[0].AgainstLedger = "0x7x3";

    qDebug()<<"%%%%%%%%%%%%%5307"<<obj->ledgersList.size()<<"&&&&&&&&&&&&&&&";
    ledTransaction->insertProxyTransaction(obj);


    //    return commit();
    int c=commit();
    qDebug()<<obj->ObjToJson(obj);

    if( c &&obj->ledgerObject.dbName != "" && !obj->fromExternal && obj->sendFlag){
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
int SalesOrderDatabaseHelper::updateVoucher(GeneralVoucherDataObject *obj, QString oldID)
{
    if(obj->measurement!=nullptr){
        obj->QuotationDroppedReason = obj->measurement->contactMeasureID;
    }
    this->startTransaction();
    QMap<QString,QVariant> map;
    map.insert(Sales_Order_Main_Voucher_No,obj->voucherNumber);
    map.insert(Sales_Order_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Sales_Order_Main_Date_Created,obj->VoucherDate);
    map.insert(Sales_Order_Main_Voucher_Date,obj->VoucherDate);
    map.insert(Sales_Order_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    map.insert(Sales_Order_Main_Billing_Name,obj->ledgerObject.LedgerName);
    map.insert(Sales_Order_Main_Total,obj->grandTotal);
    map.insert(Sales_Order_Main_Narration,obj->narration);
    map.insert(Sales_Order_Main_Added_By,obj->AddedById);
    map.insert(Sales_Order_Main_Delivery_Date,obj->DeliveryDate);
    map.insert(Sales_Order_Main_Customer_Expecting_Date,obj->CustomerExpectingDate);
    map.insert(Sales_Order_Main_Completion_Probability,obj->CompletionProbability);
    map.insert(Sales_Order_Main_Credit_Period,obj->ledgerObject.Credit_Period);
    map.insert(Sales_Order_Main_POC_Name,obj->Contact.ContactName);
    map.insert(Sales_Order_Main_POC_Phone,obj->Contact.PhoneNumber);
    map.insert(Sales_Order_Main_POC_Email,obj->Contact.email);
    map.insert(Sales_Order_Main_Project_ID,obj->ProjectId);
    map.insert(Sales_Order_Main_Revision_Number,obj->RevisionNo);
    map.insert(Sales_Order_Main_Converted_To_Sales_Order,obj->ConvertedToSalesOrder);
    map.insert(Sales_Order_Main_Quotation_Dropped,obj->QuotationDropped);
    map.insert(Sales_Order_Main_Quotation_Dropped_Reason,obj->QuotationDroppedReason);
    map.insert(Sales_Order_Main_Requirement_Voucher_No,obj->RequirementVoucherNo);
    map.insert(Sales_Order_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Sales_Order_Main_LPO,obj->LPO);
    map.insert(Sales_Order_Main_ModeOfService,obj->ModeOfService);
    map.insert(Sales_Order_Main_Contact_ID,obj->Contact._id);
    map.insert(Sales_Order_Main_VATAmount,obj->taxTotalAmount);
    map.insert(Sales_Order_Main_VoucherStatus,obj->status);
    map.insert(Sales_Order_Main_Discount_In_Amount,obj->discountinAmount);
    map.insert(Sales_Order_Main_Discount_In_Percent,obj->discountPercent);
    map.insert(Sales_Order_Main_Gross_Total,obj->grossTotal);
    map.insert(Sales_Order_Main_Reference,obj->reference);
    map.insert(Sales_Order_Main_Generated_TransactionId, obj->TransactionId);
    map.insert(Sales_Order_Main_Last_Updated,QDateTime::currentDateTime());
    map.insert(Sales_Order_Main_Location,obj->Location);

    QJsonDocument doc(obj->ReqVoucherList);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    map.insert(Sales_Order_Main_ReqVoucherList,strJson);

    QMap<QString,QVariant> map3;
    map3.insert(Sales_Order_Main_Voucher_No,obj->voucherNumber);
    map3.insert(Sales_Order_Main_Voucher_Prefix , obj->VoucherPrefix);
    db->updateRecordsPlus(map,map3,Sales_Order_Main_TableName);

    QMap<QString,QVariant> map6;
    map6.insert(Sales_Order_Details_Voucher_No,oldID);
    map6.insert(Sales_Order_Details_Voucher_Prefix , obj->VoucherPrefix);
    db->deleteRecordsPlus(map6,Sales_Order_Details_TableName);

    for(CompoundItemDataObject compitem:obj->InventoryItems){
        QMap<QString,QVariant> map1;
        inventoryItemDataModel item = compitem.BaseItem;
        if(item.ItemReqUuid == "X")
            item.ItemReqUuid = db->getUUID();
        if(item.itemProductionStatus == 0)
            item.itemProductionStatus = QuotationStatus::SalesOrderCreated;
        map1.insert(Sales_Order_Details_Voucher_No,obj->voucherNumber);
        map1.insert(Sales_Order_Details_Voucher_Prefix,obj->VoucherPrefix);
        map1.insert(Sales_Order_Details_Voucher_Date,obj->VoucherDate);
        map1.insert(Sales_Order_Details_Inventory_Item_ID,item.ItemID);
        map1.insert(Sales_Order_Details_Quantity,item.quantity);
        map1.insert(Sales_Order_Details_QuantityDisc,item.discQuantity);
        map1.insert(Sales_Order_Details_QuantityFull,item.quantityFull);

        map1.insert(Sales_Order_Details_Price,item.price);
        map1.insert(Sales_Order_Details_PriceLevel,item.PriceLevel);
        map1.insert(Sales_Order_Details_isCompoundItem,item.IsCompoundItem);
        map1.insert(Sales_Order_Details_subItemOff_list_ID,0);
        map1.insert(Sales_Order_Details_Discount_Percent,item.discountPercentage);
        map1.insert(Sales_Order_Details_Discount_Amount,item.discountinAmount);
        map1.insert(Sales_Order_Details_Subtotal,item.subTotal);
        map1.insert(Sales_Order_Details_Project_ID,item.ProjectID);
        map1.insert(Sales_Order_Details_Item_Narration,item.narration);
        map1.insert(Sales_Order_Details_Item_Description,item.ItemDescription);
        map1.insert(Sales_Order_Details_Salesman_ID,obj->SalesmanID);
        map1.insert(Sales_Order_Details_Requirement_ItemID,item.ItemReqUuid);
        map1.insert(Sales_Order_Details_Length,item.length);
        map1.insert(Sales_Order_Details_Dimension,item.Dimension);
        map1.insert(Sales_Order_Details_itemProductionStatus,item.itemProductionStatus);
        map1.insert(Sales_Order_Details_itemVoucherStatus,item.itemVoucherStatus);
        map1.insert(Sales_Order_Details_UOM_ID,item.uomObject._id);
        map1.insert(Sales_Order_Details_TaxRate, item.taxRate);
        map1.insert(Sales_Order_Details_TaxAmount, item.taxAmount);
        map1.insert(Sales_Order_Details_TimeDate, item.manufactureDate);
        map1.insert(Sales_Order_Details_ContainerCount, item.requestQty);
        int basid = db->insertRecordsPlus(map1,Sales_Order_Details_TableName);

        for(inventoryItemDataModel subitem:compitem.subItems){
            QMap<QString,QVariant> map2;
            if(subitem.ItemReqUuid == "X")
                subitem.ItemReqUuid = db->getUUID();
            if(item.itemProductionStatus == 0)
                item.itemProductionStatus = QuotationStatus::SalesOrderCreated;
            map2.insert(Sales_Order_Details_Voucher_No,obj->voucherNumber);
            map2.insert(Sales_Order_Details_Voucher_Prefix,obj->VoucherPrefix);
            map2.insert(Sales_Order_Details_Voucher_Date,obj->VoucherDate);
            map2.insert(Sales_Order_Details_Inventory_Item_ID,subitem.ItemID);
            map2.insert(Sales_Order_Details_Quantity,subitem.quantity);
            map2.insert(Sales_Order_Details_Price,subitem.price);
            map2.insert(Sales_Order_Details_PriceLevel,subitem.PriceLevel);
            map2.insert(Sales_Order_Details_isCompoundItem,subitem.IsCompoundItem);
            map2.insert(Sales_Order_Details_subItemOff_list_ID,basid);
            map2.insert(Sales_Order_Details_Discount_Percent,subitem.discountPercentage);
            map2.insert(Sales_Order_Details_Discount_Amount,subitem.discount);
            map2.insert(Sales_Order_Details_Subtotal,subitem.subTotal);
            map2.insert(Sales_Order_Details_Project_ID,item.ProjectID);
            map2.insert(Sales_Order_Details_Item_Narration,subitem.narration);
            map2.insert(Sales_Order_Details_Item_Description,subitem.ItemDescription);
            map2.insert(Sales_Order_Details_Salesman_ID,obj->SalesmanID);
            map2.insert(Sales_Order_Details_Requirement_ItemID,subitem.ItemReqUuid);
            map2.insert(Sales_Order_Details_Length,subitem.length);
            map2.insert(Sales_Order_Details_Dimension,item.Dimension);
            map2.insert(Sales_Order_Details_itemProductionStatus,item.itemProductionStatus);
            map2.insert(Sales_Order_Details_UOM_ID,subitem.uomObject._id);
            map2.insert(Sales_Order_Details_TaxRate, subitem.taxRate);
            map2.insert(Sales_Order_Details_TaxAmount, subitem.taxAmount);
            db->insertRecordsPlus(map2,Sales_Order_Details_TableName);
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
    //        ledMap.insert(Proxy_Transaction_Helper_DebitLedgerID,ledger.LedgerID);
    //        ledMap.insert(Proxy_Transaction_Helper_CreditLedgerID,obj->ledgerObject.LedgerID);
    //        ledMap.insert(Proxy_Transaction_Helper_Narration,obj->ledgerObject.narration);
    //        ledMap.insert(Proxy_Transaction_Helper_isAdditional,true);
    //        db->insertRecordsPlus(ledMap,Proxy_Transaction_Helper_TableName);
    //    }

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

    obj->ledgersList[0].AgainstLedger = "0x7x3";
    //    qDebug()<<"253";

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

int SalesOrderDatabaseHelper::deleteVoucher(GeneralVoucherDataObject *obj)
{
    qDebug()<<"Deleting"<<obj->voucherNumber<<obj->VoucherPrefix;
    if(obj->voucherNumber.length()<1)
        return 0;
    this->startTransaction();
    QMap<QString,QVariant> map3;
    map3.insert(Sales_Order_Main_Voucher_No,obj->voucherNumber);
    map3.insert(Sales_Order_Main_Voucher_Prefix,obj->VoucherPrefix);
    db->deleteRecordsPlus(map3,Sales_Order_Main_TableName);

    QMap<QString,QVariant> map4;
    map4.insert(Sales_Order_Details_Voucher_Prefix,obj->VoucherPrefix);
    map4.insert(Sales_Order_Details_Voucher_No,obj->voucherNumber);
    db->deleteRecordsPlus(map4,Sales_Order_Details_TableName);
    ledTransaction->deleteProxyTransaction(obj);
    return commit();

}

int SalesOrderDatabaseHelper::commit()
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

int SalesOrderDatabaseHelper::getMaxID()
{
    QString qStr = "SELECT MAX("+Sales_Order_Main_id+") FROM " + Sales_Order_Main_TableName;
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

QDate SalesOrderDatabaseHelper::getCompletedDate(qint64 reqID, int qtyNumber, QString SalesOrderNo)
{
    QString qStr = "";
    qStr += "SELECT "+ WorkOrder_CompletedDate;
    qStr += " from " + WorkOrder_tableName;
    qStr += " where ";
    qStr += WorkOrder_requirement_no + "='"+QString::number(reqID)+"'";
    qStr += " And ";
    qStr += WorkOrder_qty_No + "='"+QString::number(qtyNumber)+"'";
    qStr += " And ";
    qStr += WorkOrder_SalesOrder_Id + "='"+SalesOrderNo+"'";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QDate date;
    if(query.exec(qStr)){
        while(query.next()){
            date= query.value(0).toDate();
        }
    }
    return date;
}

void SalesOrderDatabaseHelper::setCompletedDate(QDate date, qint64 reqID, int qtyNumber, QString SalesOrderNo)
{
    QMap<QString,QVariant> map;
    QMap<QString,QVariant> clause;
    map.insert(WorkOrder_CompletedDate,date);

    clause.insert(WorkOrder_requirement_no,reqID);
    clause.insert(WorkOrder_qty_No,qtyNumber);
    clause.insert(WorkOrder_SalesOrder_Id,SalesOrderNo);
    startTransaction();
    db->updateRecordsPlus(map,clause,WorkOrder_tableName);
    commit();
}

QDate SalesOrderDatabaseHelper::getExpectedDate(qint64 reqID, int qtyNumber, QString SalesOrderNo)
{
    QString qStr = "";
    qStr += "SELECT "+ WorkOrder_Expected_Date;
    qStr += " from " + WorkOrder_tableName;
    qStr += " where ";
    qStr += WorkOrder_requirement_no + "='"+QString::number(reqID)+"'";
    qStr += " And ";
    qStr += WorkOrder_qty_No + "='"+QString::number(qtyNumber)+"'";
    qStr += " And ";
    qStr += WorkOrder_SalesOrder_Id + "='"+SalesOrderNo+"'";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QDate date;
    if(query.exec(qStr)){
        while(query.next()){
            date= query.value(0).toDate();
        }
    }
    return date;
}

void SalesOrderDatabaseHelper::setExpectedDate(QDate date, qint64 reqID, int qtyNumber, QString SalesOrderNo)
{
    QMap<QString,QVariant> map;
    QMap<QString,QVariant> clause;
    map.insert(WorkOrder_Expected_Date,date);

    clause.insert(WorkOrder_requirement_no,reqID);
    clause.insert(WorkOrder_qty_No,qtyNumber);
    clause.insert(WorkOrder_SalesOrder_Id,SalesOrderNo);
    startTransaction();
    db->updateRecordsPlus(map,clause,WorkOrder_tableName);
    commit();
}

QString SalesOrderDatabaseHelper::getWorkOrderNumberbySo(QString SO)
{
    QString qStr = "";
    qStr += "SELECT "+ WorkOrder_WorkOrder_id;
    qStr += " from " + WorkOrder_tableName;
    qStr += " where ";
    qStr += WorkOrder_SalesOrder_Id + "='"+SO+"'";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString number;
    if(query.exec(qStr)){
        while(query.next()){
            number= query.value(0).toString();
        }
    }
    else{
        qDebug()<<query.lastError();
    }
    return number;
}

QStringList SalesOrderDatabaseHelper::getSerialNumbersbyRequirement(qint64 reqid)
{
    QStringList serails;

    QString qStr = "";
    qStr += "SELECT "+ WorkOrder_SerialNo;
    qStr += " from " + WorkOrder_tableName;
    qStr += " where ";
    qStr += WorkOrder_requirement_no + "='"+QString::number(reqid)+"'";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    if(query.exec(qStr)){
        while(query.next()){
            serails<<query.value(0).toString();
        }
    }
    return serails;
}

QStringList SalesOrderDatabaseHelper::getSerailNumbersLeftonCusomtItemforDN(qint64 reqid)
{
    QStringList serails;

    QString qStr = "";
    qStr += "SELECT "+ WorkOrder_SerialNo;
    qStr += " from " + WorkOrder_tableName;
    qStr += " where ";
    qStr += WorkOrder_requirement_no + "='"+QString::number(reqid)+"'";
    qStr += " and ";
    qStr += WorkOrder_DN_id + " is  null";
    qStr += " and ";
    qStr += WorkOrder_status +"=1";
    qDebug()<<qStr;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    if(query.exec(qStr)){
        while(query.next()){
            serails<<query.value(0).toString();
        }
    }
    return serails;
}

QStringList SalesOrderDatabaseHelper::getSerailNumbersLeftonCusomtItemforDNwithdelID(qint64 reqid, QString delID)
{
    QStringList serails;

    QString qStr = "";
    qStr += "SELECT "+ WorkOrder_SerialNo;
    qStr += " from " + WorkOrder_tableName;
    qStr += " where ";
    qStr += WorkOrder_requirement_no + "='"+QString::number(reqid)+"'";
    qStr += " and ";
    qStr += WorkOrder_DN_id + "  ='"+delID+"'";
    qStr += " and ";
    qStr += WorkOrder_status +"=1";

    qDebug()<<qStr;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    if(query.exec(qStr)){
        while(query.next()){
            serails<<query.value(0).toString();
        }
    }
    return serails;
}

QStringList SalesOrderDatabaseHelper::getSerailNumbersLeftonReadyItemforDNwithdelID(qint64 reqid, QString delID)
/*
 *
 *
 * This function is used to get the serial numbers left
 *
 */

{
    QStringList serails;
    QString qStr = "";
    qStr += "SELECT "+ WorkOrder_SerialNo;
    qStr += " from " + WorkOrder_tableName;
    qStr += " where ";
    qStr += WorkOrder_requirement_no + "='"+QString::number(reqid)+"'";
    qStr += " and ";
    qStr += WorkOrder_DN_id + "  ='"+delID+"'";
    qDebug()<<qStr;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    if(query.exec(qStr)){
        while(query.next()){
            serails<<query.value(0).toString();
        }
    }
    return serails;
}



//float SalesOrderDatabaseHelper::getHowManyItemsDeliverableCustomItem(qint64 reqID){

//    QString qStr = "SELECT ";
//    qStr += "count(" + WorkOrder_qty_No;
//    qStr += ") FROM "+ WorkOrder_tableName;
//    qStr += " where ";
//    qStr += WorkOrder_status +"=1";
//    qStr += " and ";
//    qStr += WorkOrder_DN_id + " is null";
//    qStr += " and ";
//    qStr += WorkOrder_requirement_no + "=:id";

//    qDebug()<<qStr<<reqID;
//    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

//    query.prepare(qStr);
//    query.bindValue(":id",reqID);


//    float val =0;
//    if(query.exec()){
//        while(query.next()){
//            val = query.value(0).toFloat();
//        }
//    }
//    else{
//        query.lastError();
//    }

//    return val;

//}

//float SalesOrderDatabaseHelper::getHowManyItemsDeliverableReadyItem(qint64 reqID)
//{
//    QString qStr = "SELECT ";
//    qStr += "count(" + WorkOrder_qty_No;
//    qStr += ") FROM "+ WorkOrder_tableName;
//    qStr += " where ";
//    qStr += WorkOrder_DN_id + " is null";
//    qStr += " and ";
//    qStr += WorkOrder_requirement_no + "=:id";

//    qDebug()<<qStr<<reqID;
//    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

//    query.prepare(qStr);
//    query.bindValue(":id",reqID);


//    float val =0;
//    if(query.exec()){
//        while(query.next()){
//            val = query.value(0).toFloat();
//        }
//    }
//    else{
//        query.lastError();
//    }

//    return val;
//}



void SalesOrderDatabaseHelper::setDNID(qint64 rqID,int qtyno, QString dnID)
{
    QMap<QString,QVariant> map;
    QMap<QString,QVariant> clause;
    map.insert(WorkOrder_DN_id,dnID);

    clause.insert(WorkOrder_requirement_no,rqID);
    clause.insert(WorkOrder_qty_No,qtyno);
    startTransaction();
    db->updateRecordsPlus(map,clause,WorkOrder_tableName);
    commit();
}

QStringList SalesOrderDatabaseHelper::getSerialNumbersbyRequirementandDeliveryNote(qint64 reqid, QString delID)
{
    QStringList serails;

    QString qStr = "";
    qStr += "SELECT "+ WorkOrder_SerialNo;
    qStr += " from " + WorkOrder_tableName;
    qStr += " where ";
    qStr += WorkOrder_requirement_no + "='"+QString::number(reqid)+"'";
    qStr += " And ";
    qStr += WorkOrder_DN_id + "='"+delID+"'";


    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    if(query.exec(qStr)){
        while(query.next()){
            serails<<query.value(0).toString();
        }
    }


    return serails;
}

QStringList SalesOrderDatabaseHelper::getSerialNumbersbyRequirementandSOID(qint64 reqid, QString SalesOrderNo)
{
    QStringList serails;

    QString qStr = "";
    qStr += "SELECT "+ WorkOrder_SerialNo;
    qStr += " from " + WorkOrder_tableName;
    qStr += " where ";
    qStr += WorkOrder_requirement_no + "='"+QString::number(reqid)+"'";
    qStr += " And ";
    qStr += WorkOrder_SalesOrder_Id + "='"+SalesOrderNo+"'";


    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    if(query.exec(qStr)){
        while(query.next()){
            serails<<query.value(0).toString();
        }
    }


    return serails;
}


int SalesOrderDatabaseHelper::getMaxIDwithPrefix(QString prefix)
{
    QString qStr = "SELECT MAX("+Sales_Order_Main_Voucher_No+") FROM " + Sales_Order_Main_TableName;
    qStr += " WHERE "+Sales_Order_Main_Voucher_Prefix + "='"+prefix+"'";
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

int SalesOrderDatabaseHelper::getNextKotNumber()
{
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    return query.exec("INSERT INTO `kotnumbertable`(`tem`) VALUES ('0')");
}

int SalesOrderDatabaseHelper::getNextTakeWayNumber()
{
    GeneralVoucherDatabaseHelper vHelper;
    QString voucherNumber = vHelper.setNextVoucherNoByType("TakeAway", LoginValues::voucherPrefix , Sales_Order_Main_TableName );

}

void SalesOrderDatabaseHelper::updateReference(QString vNo, QString ref)
{
    this->startTransaction();
    QMap<QString,QVariant> map;
    QMap<QString,QVariant> clause;
    map.insert(Sales_Order_Main_Reference,ref);
    clause.insert(Sales_Order_Main_Voucher_No,vNo);
    db->updateRecordsPlus(map,clause,Sales_Order_Main_TableName);
    commit();
}

void SalesOrderDatabaseHelper::updateLedger(QString ledID, QString ledName, QString vNo, QString vPrefix)
{
    this->startTransaction();
    QMap<QString,QVariant> map;
    QMap<QString,QVariant> clause;
    map.insert(Sales_Order_Main_Ledger_ID,ledID);
    map.insert(Sales_Order_Main_Billing_Name,ledName);
    clause.insert(Sales_Order_Main_Voucher_No,vNo);
    clause.insert(Sales_Order_Main_Voucher_Prefix,vPrefix);
    db->updateRecordsPlus(map,clause,Sales_Order_Main_TableName);
    commit();
}

void SalesOrderDatabaseHelper::updateSalesMan(int empID, QString vNo, QString vPrefix)
{
    this->startTransaction();
    QMap<QString,QVariant> map;
    QMap<QString,QVariant> clause;
    map.insert(Sales_Order_Main_Salesman_ID,empID);

    clause.insert(Sales_Order_Main_Voucher_No,vNo);
    clause.insert(Sales_Order_Main_Voucher_Prefix,vPrefix);
    db->updateRecordsPlus(map,clause,Sales_Order_Main_TableName);
    commit();
}

void SalesOrderDatabaseHelper::updateKOTPrintStatus(QString voucherNo, QString voucherPrefix, bool status)
{
    this->startTransaction();

    QMap<QString,QVariant> map;
    QMap<QString,QVariant> clause;
    map.insert(Sales_Order_Main_Quotation_Dropped,status);

    clause.insert(Sales_Order_Main_Voucher_No,voucherNo);
    clause.insert(Sales_Order_Main_Voucher_Prefix,voucherPrefix);
    db->updateRecordsPlus(map,clause,Sales_Order_Main_TableName);

    commit();
}

int SalesOrderDatabaseHelper::deleteSalesOrderItemByListID(int id)
{
    this->startTransaction();
    QMap<QString,QVariant> clause;
    clause.insert(Sales_Order_Details_id,id);
    db->deleteRecordsPlus(clause,Sales_Order_Details_TableName);
    return commit();

}

int SalesOrderDatabaseHelper::getInventoryItemsCount(QString vNo)
{
    int ser;

    QString qStr = "";
    qStr += "SELECT count("+ Sales_Order_Details_id;
    qStr += ") from " + Sales_Order_Details_TableName;
    qStr += " where ";
    qStr += Sales_Order_Details_Voucher_No + "='"+vNo;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    if(query.exec(qStr)){
        ser= query.size();

    }
    return ser;
}

GeneralVoucherDataObject SalesOrderDatabaseHelper::getVoucherByVoucherNo(QString vNo)
{
    qDebug()<<"256";
    GeneralVoucherDataObject obj;
    SalesInventoryItemDatabaseHelper *itemHelper = new SalesInventoryItemDatabaseHelper();
    LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper();
    AddressBookDatabaseHelper *addHelper = new AddressBookDatabaseHelper();


    QString qStrMain ="SELECT "
                       + Sales_Order_Main_Voucher_No+ ", "
                       + Sales_Order_Main_Voucher_Prefix+ ", "
                       + Sales_Order_Main_Voucher_Date+", "
                       + Sales_Order_Main_Ledger_ID+ ", "
                       + Sales_Order_Main_Total +", "
                       + Sales_Order_Main_VoucherStatus +", "
                       + Sales_Order_Main_TimeStamp+", "
                       + Sales_Order_Main_Added_By + ", "
                       + Sales_Order_Main_Salesman_ID + ", "
                       + Sales_Order_Main_Narration + ", "
                       + Sales_Order_Main_Discount_In_Amount + ", "
                       + Sales_Order_Main_Discount_In_Percent + ", "
                       + Sales_Order_Main_VATAmount + ", "
                       + Sales_Order_Main_Reference + ", "
                       + Sales_Order_Main_ModeOfService + " "

                       + " FROM " + Sales_Order_Main_TableName
                       + " WHERE " + Sales_Order_Main_Voucher_No + "= :vno";



    QString qStrDetail = "SELECT "
                         + Sales_Order_Details_id +", "
                         + Sales_Order_Details_Voucher_No +", "
                         + Sales_Order_Details_Voucher_Prefix +", "
                         + Sales_Order_Details_Voucher_Date +", "
                         + Sales_Order_Details_Inventory_Item_ID +", "
                         + Sales_Order_Details_Price +", "
                         + Sales_Order_Details_Quantity +", "
                         + Sales_Order_Details_Discount_Percent +", "
                         + Sales_Order_Details_Subtotal +", "
                         + Sales_Order_Details_isCompoundItem +", "
                         + Sales_Order_Details_subItemOff_list_ID +", "
                         + Sales_Order_Details_PriceLevel +", "
                         + Sales_Order_Details_Item_Description +", "
                         + Sales_Order_Details_Item_Narration +", "
                         + Sales_Order_Details_Requirement_ItemID + ", "
                         + Sales_Order_Details_Length + ", "
                         + Sales_Order_Details_Dimension + ", "
                         + Sales_Order_Details_Discount_Amount + ", "
                         + Sales_Order_Details_itemProductionStatus + " "
                         + " FROM " + Sales_Order_Details_TableName
                         + " WHERE " + Sales_Order_Details_Voucher_No + "= :vno";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));


    query.prepare(qStrDetail);
    query.bindValue(":vno",vNo);
    if(!query.exec()){
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    else{
        while(query.next()){
            qDebug()<<"317";
            inventoryItemDataModel item = itemHelper->getInventoryItemByID(query.value(4).toString());
            item.price = query.value(5).toFloat();
            item.quantity = query.value(6).toFloat();
            item.discountPercentage = query.value(7).toFloat();
            item.subTotal = query.value(8).toFloat();
            item.PriceLevel = query.value(11).toString();
            item.ItemDescription = query.value(12).toString();
            item.narration = query.value(13).toString();
            item.ItemReqUuid = query.value(14).toString();
            item.length = query.value(15).toFloat();
            item.Dimension = query.value(16).toString();
            item.discountinAmount = query.value(17).toFloat();
            item.listId = query.value(0).toInt();
            item.itemProductionStatus = query.value(18).toInt();
            if(query.value(9).toBool()||(query.value(10).toInt()==0)){
                CompoundItemDataObject cObj;
                cObj.BaseItem = item;
                obj.InventoryItems.append(cObj);
            }
            else{

                int itemSize = obj.InventoryItems.size();
                obj.InventoryItems[itemSize-1].subItems.append(item);
                obj.InventoryItems[itemSize-1].subItemsTotal +=item.subTotal;
            }
        }
    }

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
            obj.ledgerObject = ledHelper->getLedgerObjectByID(obj.ledgerObject.LedgerID );
            obj.grandTotal = query.value(i++).toFloat();
            obj.status = query.value(i++).toInt();
            obj.timestamp = query.value(i++).toDateTime();
            obj.AddedById = query.value(i++).toInt();
            obj.SalesmanID = query.value(i++).toInt();
            obj.narration = query.value(i++).toString();
            obj.discountinAmount = query.value(i++).toFloat();
            obj.discountPercent = query.value(i++).toFloat();
            obj.taxTotalAmount = query.value(i++).toFloat();
            obj.reference = query.value(i++).toString();
            obj.ModeOfService = query.value(i++).toInt();
        }
    }

    return obj;
}

GeneralVoucherDataObject *SalesOrderDatabaseHelper::getVoucherByVoucherNoPtr(QString vNo, QString voucherPrefix)
{
    GeneralVoucherDataObject *obj = new GeneralVoucherDataObject();
    SalesInventoryItemDatabaseHelper *itemHelper = new SalesInventoryItemDatabaseHelper();
    LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper();
    //    AddressBookDatabaseHelper *addHelper = new AddressBookDatabaseHelper();
    //    UomDataBaseHelper *uomHelper = new UomDataBaseHelper();

    QString qStrMain ="SELECT "
                       //            + Sales_Order_Main_id + ", "
                       + Sales_Order_Main_Voucher_No + ", "
                       + Sales_Order_Main_Voucher_Prefix + ", "
                       + Sales_Order_Main_Date_Created + ", "
                       + Sales_Order_Main_Voucher_Date + ", "
                       + Sales_Order_Main_TimeStamp + ", "
                       + Sales_Order_Main_Ledger_ID + ", "
                       + Sales_Order_Main_Billing_Name + ", "
                       + Sales_Order_Main_Total + ", "
                       + Sales_Order_Main_Narration + ", "
                       + Sales_Order_Main_Added_By + ", "
                       + Sales_Order_Main_Delivery_Date + ", "
                       + Sales_Order_Main_Completion_Probability + ", "
                       + Sales_Order_Main_Credit_Period + ", "
                       + Sales_Order_Main_POC_Name + ", "
                       + Sales_Order_Main_POC_Phone + ", "
                       + Sales_Order_Main_POC_Email + ", "
                       + Sales_Order_Main_Project_ID + ", "
                       + Sales_Order_Main_Revision_Number + ", "
                       + Sales_Order_Main_Converted_To_Sales_Order + ", "
                       + Sales_Order_Main_Quotation_Dropped + ", "
                       + Sales_Order_Main_Quotation_Dropped_Reason + ", "
                       + Sales_Order_Main_Requirement_Voucher_No + ", "
                       + Sales_Order_Main_Salesman_ID + ", "
                       + Sales_Order_Main_LPO + ", "
                       + Sales_Order_Main_VoucherStatus + ", "
                       + Sales_Order_Main_ModeOfService + ", "
                       + Sales_Order_Main_Discount_In_Amount + ", "
                       + Sales_Order_Main_Discount_In_Percent + ", "
                       + Sales_Order_Main_VATAmount + ", "
                       + Sales_Order_Main_Contact_ID + ", "
                       + Sales_Order_Main_Gross_Total + ", "
                       + Sales_Order_Main_Reference + ", "
                       + Sales_Order_Main_Customer_Expecting_Date + ", "
                       + Sales_Order_Main_Currency + ", "
                       + Sales_Order_Main_BoxesCount+ ", "
                       + Sales_Order_Main_TotalWeight + ", "
                       + Sales_Order_Main_LRNo + ", "
                       + Sales_Order_Main_VoucherNoDisplay + ", "
                       + Sales_Order_Main_ReqVoucherList + ", "
                       + Sales_Order_Main_Origin + ", "
                       + Sales_Order_Main_Generated_TransactionId+", "
                       + Sales_Order_Main_Location
                       + " FROM " + Sales_Order_Main_TableName
                       + " WHERE " + Sales_Order_Main_Voucher_No + "= :vno"
                       + " AND " +  Sales_Order_Main_Voucher_Prefix + "= :voucherPrefix";

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
                         + Sales_Order_Details_id +", "
                         + Sales_Order_Details_Voucher_No
                         + ", " + Sales_Order_Details_Voucher_Prefix
                         + ", " + Sales_Order_Details_Voucher_Date
                         + ", " + Sales_Order_Details_Inventory_Item_ID
                         + ", " + Sales_Order_Details_Quantity
                         + ", " + Sales_Order_Details_Price
                         + ", " + Sales_Order_Details_UOM_ID
                         + ", subq.convalue , subq.UomName, subq.UomSymbol "
                         + ", " + Sales_Order_Details_PriceLevel
                         + ", " + Sales_Order_Details_Discount_Percent
                         + ", " + Sales_Order_Details_Discount_Amount
                         + ", " + Sales_Order_Details_Subtotal
                         + ", " + Sales_Order_Details_Item_Narration
                         + ", " + Sales_Order_Details_Item_Description
                         + ", " + Sales_Order_Details_Salesman_ID
                         + ", " + Sales_Order_Details_Requirement_ItemID
                         + ", " + Sales_Order_Details_Length
                         + ", " + Sales_Order_Details_Dimension
                         + ", " + Sales_Order_Details_itemVoucherStatus
                         + ", " + Sales_Order_Details_itemProductionStatus
                         + ", " + Sales_Order_Details_TimeDate
                         + ", " + Sales_Order_Details_isCompoundItem
                         + ", " + Sales_Order_Details_subItemOff_list_ID
                         + ", "  + Sales_Order_Details_TaxRate
                         + ", " +  Sales_Order_Details_TaxAmount
                         + " FROM " + Sales_Order_Details_TableName+ " s LEFT OUTER JOIN "
                         + UomDataBaseHelper::qStrUomForItemDetail + " as subq "
                         + " ON s." + Sales_Order_Details_Inventory_Item_ID + " = subq.itemID "
                         + " AND s." + Sales_Order_Details_UOM_ID + " = subq.UOM "
                         + " WHERE " + Sales_Order_Details_Voucher_No + "= :vno"
                         + " AND " +  Sales_Order_Details_Voucher_Prefix + "= :voucherPrefix";

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
            qDebug()<<"at db helper Ledgers are : "<<ledger.LedgerName<<" : "<<ledger.drAmount<<ledger.crAmount<<ledger.isInvoiceItem;
            //            if(!ledger.isInvoiceItem){
            //                qDebug()<<"Adding "<<ledger.LedgerName;
            obj->ledgersList.append(ledger);
            //            }
        }
    }
    //    qDebug()<<qStrDetail;
    query.prepare(qStrDetail);
    query.bindValue(":vno",vNo);
    query.bindValue(":voucherPrefix",voucherPrefix);
    if(!query.exec()){
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    else{
        qDebug()<<query.lastQuery();
        while(query.next()){
            int i=4;
            QString item__ID = query.value(i++).toString();
            //            qDebug()<<"Item ID :"<<item__ID;
            inventoryItemDataModel item = itemHelper->getInventoryItemByID(item__ID,false);

            item.quantity = query.value(i++).toFloat();
            item.prevQty = item.quantity;
            item.price = query.value(i++).toFloat();
            item.uomObject._id = query.value(i++).toString();
            item.uomObject.convRate = query.value(i++).toFloat();
            item.uomObject.UomName = query.value(i++).toString();
            item.uomObject.UomSymbol = query.value(i++).toString();
            //            qDebug()<<"^^^^^^^^^^^^^^^^^^^^^^^^^^SO item UOM"<<item.uomObject._id;
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
            item.manufactureDate = query.value(i++).toDate();


            //            i++;
            bool isCompund = query.value(i++).toBool();
            int sublistid = query.value(i++).toInt();
            item.listId = query.value(0).toInt();
            item.taxRate = query.value(26).toFloat();
            item.taxAmount = query.value(27).toFloat();


            if(isCompund||sublistid==0){
                CompoundItemDataObject cObj;
                cObj.BaseItem = item;
                obj->InventoryItems.append(cObj);
            }
            else{
                int itemSize = obj->InventoryItems.size();
                obj->InventoryItems[itemSize-1].subItems.append(item);
                obj->InventoryItems[itemSize-1].subItemsTotal +=item.subTotal;
            }
            //            qDebug()<<"Item Name = "<<item.ItemName;


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
            obj->ReqVoucherList = query.value(i++).toJsonArray();
            obj->Origin = query.value(i++).toInt();
            obj->TransactionId = query.value(i++).toString();
            obj->Location = query.value(i++).toString();
        }
    }
    //    qDebug()<<qStrMain;
    //    qDebug()<<qStrDetail;
    //    qDebug()<<vNo;
    //    qDebug()<<"Prefix : "<<voucherPrefix;

    if(obj->POCPhone.length()>0){
        AddressBookDatabaseHelper *addrHelper = new AddressBookDatabaseHelper();
        obj->Contact = *(addrHelper->getContactPtrByPhone(obj->POCPhone));
        obj->Contact.address = obj->Location;
    }
    else{
        qDebug()<<"NO PHONE NUMBER DETECTED";
    }
    if(obj->QuotationDroppedReason.length()>0){

        MeasurementsDatabaseHelper dbhelper;
        ContactMeasurement *cm = new ContactMeasurement();
        cm->contactMeasureID = obj->QuotationDroppedReason;
        cm->contactID = obj->Contact.addressId;
        cm = dbhelper.getContactMeasurement(cm);
        obj->measurement = cm;
        qDebug()<<"read measurement";
    }

    obj->voucherType = VoucherType;
    return obj;

}

QString SalesOrderDatabaseHelper::getVoucherNumberByLedID(QString LedID, QDate Date)
{
    QString qStr;
    QString vchID = "-1";
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    qStr += "SELECT "+ Sales_Order_Main_Voucher_No ;
    qStr += " FROM ";
    qStr += Sales_Order_Main_TableName;
    qStr += " WHERE ";
    qStr += Sales_Order_Main_VoucherStatus +"=:a0";
    qStr += " AND ";
    qStr += Sales_Order_Main_Ledger_ID + "=:a1";
    qStr += " AND ";
    qStr += Sales_Order_Main_Voucher_Date + "=:a2";



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


QSqlQueryModel *SalesOrderDatabaseHelper::getVoucherListByDateQModel(QDate dateFrom, QDate dateTo, int empID, int plistID, QString route,
                                                                     QString allowdStatus, QString xmptList,QString voucherPrefix)
{
    UserProfileDatabaseHelper user;
    LedgerMasterDatabaseHelper led;
    PricelistDatabaseHelper price;
    AddressBookDatabaseHelper addr;
    QString qStr ="SELECT ";
    qStr += Sales_Order_Main_Voucher_Date+" as 'Date', ";
    qStr += Sales_Order_Main_Voucher_Prefix + " as Prf,";
    qStr += Sales_Order_Main_Voucher_No + " as 'V.No', ";
    qStr += Sales_Order_Main_Reference + ",";

    qStr += "(SELECT "+user.Employee_Details_Name+ " from "+ user.Employee_Details_TableName+ " EMPT where ";
    qStr += "EMPT."+user.Employee_Details_id + "= somd."+Sales_Order_Main_Salesman_ID;
    qStr += ") as Salesman, ";
    qStr += "(SELECT "+price.PriceListName+ " from "+ price.PriceLists_TableName+ " PRCT where ";
    qStr += "PRCT."+price.PriceListID + "= somd."+Sales_Order_Main_ModeOfService;
    qStr += ") as 'Service', ";
    qStr += "(SELECT "+led.Ledger_Master_LedgerName+ " from "+ led.Ledger_Master_TableName+ " LMT where ";
    qStr += "somd."+Sales_Order_Main_Ledger_ID + "= LMT."+led.Ledger_Master_LedgerID;
    qStr += ") as Ledger, ";
    qStr += Sales_Order_Main_POC_Name + " 'Name', ";
    qStr += Sales_Order_Main_POC_Phone + "'Phone', ";//8
    qStr += "( SELECT distinct " + addr.Address_Book_Route + " FROM " + addr.Address_Book_TableName ;
    qStr += " WHERE " + addr.Address_Book_PhoneNumber + " = " + Sales_Order_Main_POC_Phone + " limit 1 ) as Route, ";
    qStr += Sales_Order_Main_Total +" as 'Amount', ";
    qStr += "( SELECT distinct " + addr.Address_Book_address + " FROM " + addr.Address_Book_TableName ;
    qStr += " WHERE " + addr.Address_Book_PhoneNumber + " = " + Sales_Order_Main_POC_Phone + " limit 1 ) as Address, ";

    qStr += "substr(" + Sales_Order_Main_TimeStamp + ", 11,15) as OrderTime,";
    qStr += "substr(" + Sales_Order_Main_Assigned_Time + ", 11,15) as PickedTime";

    qStr += " FROM " + Sales_Order_Main_TableName;

    qStr += " as somd WHERE ";
    qStr += Sales_Order_Main_Voucher_Date;
    qStr += " >= ";
    qStr += DatabaseHelper::getDateStringStatic(dateFrom);
    qStr += " AND ";
    qStr += Sales_Order_Main_Voucher_Date;
    qStr += " <= ";
    qStr += DatabaseHelper::getDateStringStatic(dateTo);

    //    qStr += " AND ";
    //    qStr += Sales_Order_Main_Delivery_Date;
    //    qStr += " >= ";
    //    qStr += DatabaseHelper::getDateStringStatic(dateFrom);
    //    qStr += " AND ";
    //    qStr += Sales_Order_Main_Delivery_Date;
    //    qStr += " <= ";
    //    qStr += DatabaseHelper::getDateStringStatic(dateTo.addDays(1));

    if(allowdStatus.length()>0){
        qStr += " AND ";
        qStr += Sales_Order_Main_VoucherStatus;
        qStr += "  in (" + allowdStatus+")";
    }
    if(xmptList.length()>0){
        qStr += " AND ";
        qStr += Sales_Order_Main_VoucherStatus;
        qStr += " not in (" + xmptList+")";
    }

    if(empID>0){
        qStr += " AND ";
        qStr += Sales_Order_Main_Salesman_ID;
        qStr += " = " + QString::number(empID);
    }
    if(plistID>0){
        qStr += " AND ";
        qStr += Sales_Order_Main_ModeOfService;
        qStr += " = " + QString::number(plistID);
    }
    if(route != "%"  ){
        qStr += " AND ";
        qStr += "( SELECT distinct coalesce(" + addr.Customer_Details_Route + ",'') FROM " + addr.Customer_Details_TableName ;
        qStr += " WHERE " + addr.Customer_Details_Phone + " = " + Sales_Order_Main_POC_Phone + ") ";
        qStr += " like '" +route + "'";
    }
    if(voucherPrefix!="%"){
        qStr += " AND ";
        qStr  += Sales_Order_Main_Voucher_Prefix;
        qStr += " like '"+ voucherPrefix+"'";
    }

    qDebug()<<"1785"<<qStr;

    return DatabaseHelper::getQueryModelL(qStr);
}

QSqlQueryModel *SalesOrderDatabaseHelper::getVouchersForDelivery(QDate dateFrom, QDate dateTo, int empID, int plistID, QString route,
                                                                 QString allowdStatus, QString xmptList,QString time,int status)
{
    UserProfileDatabaseHelper user;
    LedgerMasterDatabaseHelper led;
    PricelistDatabaseHelper price;
    AddressBookDatabaseHelper addr;
    QString qStr ="SELECT ";
    qStr += Sales_Order_Main_Voucher_Date+" as 'Ord Date', ";       //0
    qStr += Sales_Order_Main_Voucher_No+ " as 'V.No', ";            //1
    qStr += Sales_Order_Main_Voucher_Prefix+ ",";                   //2

    qStr += "(SELECT "+user.Employee_Details_Name+ " from "+ user.Employee_Details_TableName+ " EMPT where ";
    qStr += "EMPT."+user.Employee_Details_id + "= somd."+Sales_Order_Main_Salesman_ID;
    qStr += " limit 1) as Salesman, ";  //3

    qStr += "( SELECT  " + addr.Address_Book_Route + " FROM " + addr.Address_Book_TableName ;
    qStr += " WHERE " + addr.Address_Book_PhoneNumber + " = " + Sales_Order_Main_POC_Phone + " limit 1) as Route, ";
    //4
    qStr += Sales_Order_Main_Delivery_Date + " as 'Time'";
    qStr += ","; // 5

    qStr += "(SELECT "+price.PriceListName+ " from "+ price.PriceLists_TableName+ " PRCT where ";
    qStr += "PRCT."+price.PriceListID + "= somd."+Sales_Order_Main_ModeOfService;
    qStr += " limit 1) as 'Service', "; //6



    qStr += "(SELECT "+addr.Address_Book_ContactName + " from " + addr.Address_Book_TableName + " Addr Where ";
    qStr += "Addr."+addr.Address_Book_PhoneNumber + "= somd."+Sales_Order_Main_POC_Phone;
    qStr += " limit 1) as 'Name', "; //7

    qStr += Sales_Order_Main_POC_Phone + " as 'Phone', "; //8


    qStr += "(SELECT "+led.Ledger_Master_LedgerName+ " from "+ led.Ledger_Master_TableName+ " LMT where ";
    qStr += "somd."+Sales_Order_Main_Ledger_ID + "= LMT."+led.Ledger_Master_LedgerID;
    qStr += " limit 1) as Ledger,"; //9


    qStr += Sales_Order_Main_Total +" as 'Amount', "; //10

    qStr += Sales_Order_Main_Reference + ","; //11
    qStr += Sales_Order_Main_Location+",";          //12

    qStr += Sales_Order_Main_Delivery_Date+",";      //13
    qStr += Sales_Order_Main_VoucherStatus + ", "; //14
    qStr += Sales_Order_Main_Quotation_Dropped; //15




    qStr += " FROM " + Sales_Order_Main_TableName;
    qStr += " as somd ";

    qStr += " WHERE ";

    //    qStr += Sales_Order_Main_Voucher_Date;
    //    qStr += " >= ";
    //    qStr += DatabaseHelper::getDateStringStatic(dateFrom);
    //    qStr += " AND ";
    //    qStr += Sales_Order_Main_Voucher_Date;
    //    qStr += " <= ";
    //    qStr += DatabaseHelper::getDateStringStatic(dateTo);


    //    qStr += " AND ";

    qStr += " ( ";
    qStr += Sales_Order_Main_Delivery_Date;
    qStr += " >= ";
    qStr += DatabaseHelper::getDateStringStatic(dateFrom);
    qStr += " AND ";
    qStr += Sales_Order_Main_Delivery_Date;
    qStr += " <= ";
    qStr += DatabaseHelper::getDateStringStatic(dateTo);



    if(allowdStatus.length()>0){
        qStr += " AND ";
        qStr += Sales_Order_Main_VoucherStatus;
        qStr += "  in (" + allowdStatus+")";
    }
    if(xmptList.length()>0){
        qStr += " AND ";
        qStr += Sales_Order_Main_VoucherStatus;
        qStr += " not in (" + xmptList+")";
    }

    if(empID>0){
        qStr += " AND ";
        qStr += Sales_Order_Main_Salesman_ID;
        qStr += " = " + QString::number(empID);
    }
    if(plistID>0){
        qStr += " AND ";
        qStr += Sales_Order_Main_ModeOfService;
        qStr += " = " + QString::number(plistID);
    }
    if(route != "%"  ){
        qStr += " AND ";
        qStr += "( SELECT coalesce(" + addr.Address_Book_Route + ",'') FROM " + addr.Address_Book_TableName ;
        qStr += " WHERE " + addr.Address_Book_PhoneNumber + " = " + Sales_Order_Main_POC_Phone + " limit 1 ) ";
        qStr += " like '" +route + "'";
    }
    //    $this->Sales_Order_Main_Delivery_Date >= str_to_date('$fromdate', '%Y-%m-%d %h:%i:%s') AND $this->Sales_Order_Main_Delivery_Date  <= str_to_date('$todate', '%Y-%m-%d %h:%i:%s')

    if(time != "%"){
        QDateTime df =  QDateTime(dateFrom);df.setTime(QTime::fromString(time,"hh:mm:ss"));
        QDateTime dt =  QDateTime(dateTo); dt.setTime(QTime::fromString(time,"hh:mm:ss"));
        qStr += " AND ";
        qStr += "SUBSTRING(somd."+Sales_Order_Main_Delivery_Date+", 12, 8)";
        qStr += " like '";
        qStr += time+"'";
    }
    if(status!=0){
        qStr += " AND ";
        qStr += Sales_Order_Main_VoucherStatus +" = ";
        qStr += "'"+QString::number(status)+"'";
    }

    qStr += " ) ";
    qDebug()<<qStr;

    return DatabaseHelper::getQueryModelL(qStr);
}

QSqlQueryModel *SalesOrderDatabaseHelper::getVouchersForBooking(QDate dateFrom, QDate dateTo, int empID, int plistID, QString route, QString allowdStatus, QString xmptList, QString time, int status)
{
    UserProfileDatabaseHelper user;
    LedgerMasterDatabaseHelper led;
    PricelistDatabaseHelper price;
    AddressBookDatabaseHelper addr;
    QString qStr ="SELECT ";
    qStr += Sales_Order_Main_Voucher_Date+" as 'Ord Date', ";       //0
    qStr += Sales_Order_Main_Voucher_No+ " as 'V.No', ";            //1
    qStr += Sales_Order_Main_Voucher_Prefix+ ",";                   //2

    qStr += "(SELECT "+user.Employee_Details_Name+ " from "+ user.Employee_Details_TableName+ " EMPT where ";
    qStr += "EMPT."+user.Employee_Details_id + "= somd."+Sales_Order_Main_Salesman_ID;
    qStr += " limit 1) as Salesman, ";  //3

    qStr += "( SELECT  " + addr.Address_Book_Route + " FROM " + addr.Address_Book_TableName ;
    qStr += " WHERE " + addr.Address_Book_PhoneNumber + " = " + Sales_Order_Main_POC_Phone + " limit 1) as Route, ";
    //4
    qStr += Sales_Order_Main_Delivery_Date + " as 'Booking'";
    qStr += ","; // 5

    qStr += "(SELECT "+price.PriceListName+ " from "+ price.PriceLists_TableName+ " PRCT where ";
    qStr += "PRCT."+price.PriceListID + "= somd."+Sales_Order_Main_ModeOfService;
    qStr += " limit 1) as 'Service', "; //6



    qStr += "(SELECT "+addr.Address_Book_ContactName + " from " + addr.Address_Book_TableName + " Addr Where ";
    qStr += "Addr."+addr.Address_Book_PhoneNumber + "= somd."+Sales_Order_Main_POC_Phone;
    qStr += " limit 1) as 'Name', "; //7

    qStr += Sales_Order_Main_POC_Phone + " as 'Phone', "; //8


    qStr += "(SELECT "+led.Ledger_Master_LedgerName+ " from "+ led.Ledger_Master_TableName+ " LMT where ";
    qStr += "somd."+Sales_Order_Main_Ledger_ID + "= LMT."+led.Ledger_Master_LedgerID;
    qStr += " limit 1) as Ledger,"; //9


    qStr += Sales_Order_Main_Total +" as 'Amount', "; //10

    qStr += Sales_Order_Main_Reference + ","; //11
    qStr += Sales_Order_Main_Location+",";          //12

    qStr += Sales_Order_Main_Customer_Expecting_Date+",";      //13
    qStr += Sales_Order_Main_VoucherStatus + ", "; //14
    qStr += Sales_Order_Main_Quotation_Dropped; //15




    qStr += " FROM " + Sales_Order_Main_TableName;
    qStr += " as somd ";

    qStr += " WHERE ";


    qStr += " ( ";
    qStr += Sales_Order_Main_Delivery_Date;
    qStr += " >= ";
    qStr += DatabaseHelper::getDateStringStatic(dateFrom);
    qStr += " AND ";
    qStr += Sales_Order_Main_Delivery_Date;
    qStr += " <= ";
    qStr += DatabaseHelper::getDateStringStatic(dateTo);

    qStr += ") ";

    if(empID>0){
        qStr += " AND ";
        qStr += Sales_Order_Main_Salesman_ID;
        qStr += " = " + QString::number(empID);
    }


    //    $this->Sales_Order_Main_Delivery_Date >= str_to_date('$fromdate', '%Y-%m-%d %h:%i:%s') AND $this->Sales_Order_Main_Delivery_Date  <= str_to_date('$todate', '%Y-%m-%d %h:%i:%s')


    if(status != 0){
        qStr += " AND ";
        qStr += Sales_Order_Main_VoucherStatus +" = ";
        qStr += "'"+QString::number(status)+"'";
    }

    qStr += " order by Booking";


    return DatabaseHelper::getQueryModelL(qStr);
}

QSqlQueryModel *SalesOrderDatabaseHelper::getVoucherListByDateTimeModel(QDateTime dateFrom, QDateTime dateTo)
{
    LedgerMasterDatabaseHelper ledHelper;
    QString qStr ="SELECT ";
    qStr += "'V' , ";
    qStr += Sales_Order_Main_Voucher_No +", ";
    qStr += "'" + VoucherType + "', " ;
    qStr += Sales_Order_Main_Voucher_Prefix + ", ";
    qStr += Sales_Order_Main_Voucher_Date+" as 'Voucher Date', ";
    //    qStr += " CONCAT(" + Sales_Order_Main_Voucher_Prefix + ", '-', " + Sales_Order_Main_Voucher_No+ ") as 'Voucher No', ";
    qStr += Sales_Order_Main_Voucher_No +" as 'Voucher No', ";
    qStr += "(SELECT "+ledHelper.Ledger_Master_LedgerName + " FROM "+ ledHelper.Ledger_Master_TableName;
    qStr += " WHERE "+ledHelper.Ledger_Master_LedgerID + "= som.";
    qStr += Sales_Order_Main_Ledger_ID + ") Ledger, ";
    qStr += Sales_Order_Main_Total +", " ;
    //    qStr += Sales_Order_Main_TimeStamp +" 'Date'  ";
    //    qStr += Sales_Order_Main_Added_By + ", ";
    //    qStr += Sales_Order_Main_Salesman_ID + ", ";
    //    qStr += Sales_Order_Main_Narration + ", ";
    //    qStr += Sales_Order_Main_LPO + ", ";
    //    qStr += Sales_Order_Main_ModeOfService + ", ";
    qStr += Sales_Order_Main_VoucherStatus + ", ";
    //    qStr += Sales_Order_Main_Reference + ", ";
    qStr += Sales_Order_Main_POC_Phone + " as 'Phone' "+",";
    qStr +=  Sales_Order_Main_POC_Name + " as 'Name' ";

    //    qStr += Sales_Order_Main_Requirement_Voucher_No + " ";
    qStr += " FROM " + Sales_Order_Main_TableName;
    qStr += " som WHERE ";
    qStr += Sales_Order_Main_Voucher_Date;
    qStr += " >= '";
    qStr += dateFrom.toString("yyyy-MM-dd");
    qStr += "' AND ";
    qStr += Sales_Order_Main_Voucher_Date;
    qStr += " <= '";
    qStr += dateTo.toString("yyyy-MM-dd");
    qStr += "'";

    return DatabaseHelper::getQueryModel(qStr);
}


QSqlQueryModel *SalesOrderDatabaseHelper::getVoucherListByDateGModel(QDateTime dateFrom, QDateTime dateTo)
{
    LedgerMasterDatabaseHelper ledHelper;
    QString qStr ="SELECT ";
    qStr += Sales_Order_Main_Voucher_No+ ", ";
    qStr += Sales_Order_Main_Voucher_Prefix+ ", ";
    qStr += Sales_Order_Main_Voucher_Date+", ";
    qStr += "(SELECT "+ledHelper.Ledger_Master_LedgerName + " FROM "+ ledHelper.Ledger_Master_TableName;
    qStr += " WHERE "+ledHelper.Ledger_Master_LedgerID + "= som.";
    qStr += Sales_Order_Main_Ledger_ID + ") Ledger, ";
    qStr += Sales_Order_Main_Total +", ";
    qStr += Sales_Order_Main_TimeStamp +" 'Date'  ";
    //    qStr += Sales_Order_Main_Added_By + ", ";
    //    qStr += Sales_Order_Main_Salesman_ID + ", ";
    //    qStr += Sales_Order_Main_Narration + ", ";
    //    qStr += Sales_Order_Main_LPO + ", ";
    //    qStr += Sales_Order_Main_ModeOfService + ", ";
    //    qStr += Sales_Order_Main_VoucherStatus + ", ";
    //    qStr += Sales_Order_Main_Reference + ", ";
    //    qStr += Sales_Order_Main_POC_Phone + ", ";
    //    qStr += Sales_Order_Main_Requirement_Voucher_No + " ";
    qStr += " FROM " + Sales_Order_Main_TableName;
    qStr += " som WHERE ";
    qStr += Sales_Order_Main_Voucher_Date;
    qStr += " >= '";
    qStr += dateFrom.toString("yyyy-MM-dd");
    qStr += "' AND ";
    qStr += Sales_Order_Main_Voucher_Date;
    qStr += " <= '";
    qStr += dateTo.toString("yyyy-MM-dd");
    qStr += "'";

    //    qDebug()<<qStr;

    QSqlQueryModel *model = new QSqlQueryModel;
    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}





int SalesOrderDatabaseHelper::updateSalesOrderStatus(GeneralVoucherDataObject *voucher)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    this->startTransaction();
    QMap<QString,QVariant> map;
    QMap<QString,QVariant> clause;
    map.insert(Sales_Order_Main_VoucherStatus,voucher->status);
    clause.insert(Sales_Order_Main_Voucher_No,voucher->voucherNumber);
    clause.insert(Sales_Order_Main_Voucher_Prefix,voucher->VoucherPrefix);
    db->updateRecordsPlus(map,clause,Sales_Order_Main_TableName);
    return commit();

}

void SalesOrderDatabaseHelper::setItemVoucherStatus(float qty, QString id)
{
    QString qStr ="SELECT "+Sales_Order_Details_itemVoucherStatus+ " from "+ Sales_Order_Details_TableName +
                   " WHERE "+Sales_Order_Details_Requirement_ItemID + " = " + id;
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

    map.insert(Sales_Order_Details_itemVoucherStatus,qty);
    clause.insert(Sales_Order_Details_Requirement_ItemID,id);
    db->updateRecordsPlus(map,clause,Sales_Order_Details_TableName);

}

void SalesOrderDatabaseHelper::setAllItemVoucherStatusAsQStatus(GeneralVoucherDataObject *voucher, int status)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    QMap<QString,QVariant> map;
    QMap<QString,QVariant> clause;

    map.insert(Sales_Order_Details_itemVoucherStatus,status);
    clause.insert(Sales_Order_Details_Voucher_No,voucher->voucherNumber);
    clause.insert(Sales_Order_Details_Voucher_Prefix,voucher->VoucherPrefix);
    startTransaction();
    db->updateRecordsPlus(map,clause,Sales_Order_Details_TableName);
    commit();
}

void SalesOrderDatabaseHelper::resetItemVoucherStatus(float qty,QString id)
{
    QString qStr ="SELECT "+Sales_Order_Details_itemVoucherStatus+ " from "+ Sales_Order_Details_TableName +
                   " WHERE "+Sales_Order_Details_Requirement_ItemID + " = " + id;
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

    map.insert(Sales_Order_Details_itemVoucherStatus,qty);
    clause.insert(Sales_Order_Details_Requirement_ItemID,id);
    startTransaction();
    db->updateRecordsPlus(map,clause,Sales_Order_Details_TableName);
    commit();
}

void SalesOrderDatabaseHelper::setSerialNumberOnSo(QString serial, qint64 reqID)
{
    QMap<QString,QVariant> map;
    QMap<QString,QVariant> clause;

    map.insert(Sales_Order_Details_Voucher_Prefix,serial);
    clause.insert(Sales_Order_Details_Requirement_ItemID,reqID);
    startTransaction();
    db->updateRecordsPlus(map,clause,Sales_Order_Details_TableName);
    commit();
}

void SalesOrderDatabaseHelper::setItemProductionStatus(int status, QString reqID)
{
    qDebug()<< "sales order Status Changing";
    QMap<QString,QVariant> map;
    QMap<QString,QVariant> clause;

    map.insert(Sales_Order_Details_itemProductionStatus,status);
    clause.insert(Sales_Order_Details_Requirement_ItemID,reqID);
    startTransaction();
    db->updateRecordsPlus(map,clause,Sales_Order_Details_TableName);
    commit();
    qDebug()<< "Status Changed";

}

void SalesOrderDatabaseHelper::setSalesOrderStatus(int status,QString qNo,QString voucherPrefix)
{
    qDebug()<< "sales order Status Changing"<<status<<qNo<<voucherPrefix;
    QMap<QString,QVariant> map;
    QMap<QString,QVariant> clause;
    qDebug()<<"Voucher no = "<<qNo;
    map.insert(Sales_Order_Main_VoucherStatus,status);
    clause.insert(Sales_Order_Main_Voucher_No,qNo);
    clause.insert(Sales_Order_Main_Voucher_Prefix,voucherPrefix);
    startTransaction();
    db->updateRecordsPlus(map,clause,Sales_Order_Main_TableName);
    commit();
    qDebug()<< "Status Changed";

}

void SalesOrderDatabaseHelper::switchSalesMan(int newEmpID, QString qNo,QString pref,QDateTime dTime)
{
    QMap<QString,QVariant> map;
    QMap<QString,QVariant> clause;

    map.insert(Sales_Order_Main_Salesman_ID,newEmpID);
    map.insert(Sales_Order_Main_Assigned_Time,dTime);

    clause.insert(Sales_Order_Main_Voucher_No,qNo);
    clause.insert(Sales_Order_Main_Voucher_Prefix,pref);

    startTransaction();
    db->updateRecordsPlus(map,clause,Sales_Order_Main_TableName);
    commit();
}


//float SalesOrderDatabaseHelper::getMaxAllowedQty(qint64 requirementItemId)
//{
//    QuotationDatabaseHelper *quoteHelper = new QuotationDatabaseHelper;

//    QString qStr = "SELECT "+quoteHelper->Quotation_Details_Quantity+ " FROM "+quoteHelper->Quotation_Details_TableName;
//    qStr += " WHERE "+quoteHelper->Quotation_Details_id + "=:id";

//    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
//    query.prepare(qStr);

//    query.bindValue(":id",requirementItemId);
//    float qty =0;
//    if(query.exec()){
//        while(query.next()){
//            qty = query.value(0).toFloat();
//        }
//    }

//    else{
//        qDebug()<<query.lastError();
//    }

//    qStr = "SELECT SUM("+Sales_Order_Details_Quantity+ ") FROM "+Sales_Order_Details_TableName;
//    qStr += " WHERE "+Sales_Order_Details_Requirement_ItemID + "=:id";
//    query.prepare(qStr);

//    query.bindValue(":id",requirementItemId);

//    if(query.exec()){
//        while(query.next()){
//            qty -= query.value(0).toFloat();
//        }
//    }

//    else{
//        qDebug()<<query.lastError();
//    }

//    return qty>0?qty:0;

//}



QPair<QString,QString> SalesOrderDatabaseHelper::getVoucherNumberByRef(QString ref)
{
    QString qStr = "";
    qStr += "SELECT "+ Sales_Order_Main_Voucher_No;
    qStr += ", "+Sales_Order_Main_Voucher_Prefix;
    qStr += " from " + Sales_Order_Main_TableName;
    qStr += " where ";
    qStr += Sales_Order_Main_Reference + "='"+ref+"'";
    qStr += " and " + Sales_Order_Main_VoucherStatus + " <> '170' ";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString number ="";
    QString pref ="";
    if(query.exec(qStr)){
        while(query.next()){
            number= query.value(0).toString();
            pref= query.value(1).toString();

        }
    }
    else{
        qDebug()<<query.lastError();
    }
    QPair<QString,QString> val; val.first = number; val.second = pref;
    return val;
}


void SalesOrderDatabaseHelper::truncateKotNumberTable()
{
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.exec("TRUNCATE TABLE `kotnumbertable`");

}

int SalesOrderDatabaseHelper::getNewKotNumber()
{
    int n=0;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    if(query.exec("INSERT INTO `kotnumbertable`(`tem`) VALUES ('0')")){
        n = query.lastInsertId().toInt();
        qDebug()<<"^^^^^^^^^^^^^^^^^^^^^ last ID: "<<n;
    }
    else{
        qDebug()<<query.lastError();

    }
    return n;
}

int SalesOrderDatabaseHelper::getSOPending()
{
    int n=0;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "";
    qStr = "SELECT COUNT("+Sales_Order_Main_id+") FROM "+Sales_Order_Main_TableName;
    qStr += " where ";
    qStr += Sales_Order_Main_VoucherStatus+ "in (";
    qStr += QString::number(QuotationStatus::SalesOrderCreated)+", ";
    qStr += QString::number(QuotationStatus::WorkOrderRaised)+", ";
    qStr += QString::number(QuotationStatus::WorkOrderCompleted)+") ";
    //    qDebug()<<qStr;
    if(query.exec(qStr)){
        while(query.next())
            n = query.value(0).toInt();
    }
    return n;
}

GeneralVoucherDataObject *SalesOrderDatabaseHelper::getPendingSalesOrders()
{
    WorkOrderDatabaseHelper *woHelper = new WorkOrderDatabaseHelper();
    GeneralVoucherDataObject *obj = new GeneralVoucherDataObject();
    SalesInventoryItemDatabaseHelper *sHelper = new SalesInventoryItemDatabaseHelper();

    QString qStr = " SELECT so." + Sales_Order_Details_Inventory_Item_ID + ", ";
    qStr += "so." + Sales_Order_Details_Quantity + " - coalesce((sum(wo." + woHelper->WorkOrder_qty + ")),0)";
    qStr += " FROM " + Sales_Order_Details_TableName + " SO left outer join ";
    qStr += woHelper->WorkOrder_tableName + " wo on SO." ;
    qStr += Sales_Order_Details_Inventory_Item_ID + " = wo." + woHelper->WorkOrder_itemID ;
    qStr += " and so." + Sales_Order_Details_Requirement_ItemID + " = wo." + woHelper->WorkOrder_requirement_no;
    qStr += " group by so." + Sales_Order_Details_Inventory_Item_ID + " , so." + Sales_Order_Details_Requirement_ItemID;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);

    if(query.exec()){
        while(query.next()){
            inventoryItemDataModel item;
            CompoundItemDataObject compItem;

            item = sHelper->getInventoryItemByID(query.value(0).toString());
            item.quantity = query.value(1).toInt();
            compItem.BaseItem = item;
            obj->InventoryItems.append(compItem);
        }
    }

    return obj;
}

QSqlQueryModel *SalesOrderDatabaseHelper::getPendingSalesOrdersByDeliveryDate(QDateTime fromDate, QDateTime toDate)
{

    QString qStr = " SELECT " ;
    qStr += Sales_Order_Main_Voucher_No + " as 'Voucher No', ";
    qStr += Sales_Order_Main_Voucher_Prefix + ", ";
    qStr += Sales_Order_Main_Voucher_Date + " as 'Booking Date', ";
    qStr += Sales_Order_Main_Delivery_Date + " as 'Delivery Date', ";
    qStr += Sales_Order_Main_POC_Name + " as 'Customer Name', ";
    qStr += Sales_Order_Main_Total + " as 'Total Amount' ";
    qStr += " FROM " + Sales_Order_Main_TableName;
    qStr += " WHERE ";
    qStr += Sales_Order_Main_Delivery_Date + " <= " + db->getDateTimeString(toDate);
    qStr += " AND ";
    qStr += Sales_Order_Main_Delivery_Date + " >= " + db->getDateTimeString(fromDate);
    qStr += " AND " + Sales_Order_Main_VoucherStatus + " < " + QString::number(QuotationStatus::SalesInvoiceRaised);
    qStr += " ORDER BY " + Sales_Order_Main_Delivery_Date + ", " + Sales_Order_Main_Voucher_No;

    //    qDebug()<<qStr;

    return db->getQueryModel(qStr);
}

QSqlQueryModel *SalesOrderDatabaseHelper::getItemwisePendingOrdersByDeliveryDate(QDate fromDate, QDate toDate, QString time)
{

    QString qStr = " SELECT " ;
    qStr += " (select item_name FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where item_id = " + Sales_Order_Details_Inventory_Item_ID + ") as Item, ";
    qStr += " SUM(" + Sales_Order_Details_Quantity + ") as Quantity FROM ";
    qStr += " ( SELECT " + Sales_Order_Details_Inventory_Item_ID + " , " + Sales_Order_Details_Quantity;
    qStr += " FROM " + Sales_Order_Main_TableName + " main ";
    qStr += " JOIN " + Sales_Order_Details_TableName + " det ";
    qStr += " ON main." + Sales_Order_Main_Voucher_No + " = det." + Sales_Order_Details_Voucher_No;
    qStr += " WHERE ";
    qStr += Sales_Order_Main_Delivery_Date + " < " + db->getDateString(toDate.addDays(1));
    qStr += " AND ";
    qStr += Sales_Order_Main_Delivery_Date + " >= " + db->getDateString(fromDate);
    if(time != "%"){
        QDateTime df =  QDateTime(fromDate);df.setTime(QTime::fromString(time,"hh:mm:ss"));
        QDateTime dt =  QDateTime(toDate); dt.setTime(QTime::fromString(time,"hh:mm:ss"));
        qStr += " AND ";
        qStr += "SUBSTRING(main."+Sales_Order_Main_Delivery_Date+", 12, 8)";
        qStr += " like '";
        qStr += time+"'";
    }
    qStr += " AND " + Sales_Order_Main_VoucherStatus + " < " + QString::number(QuotationStatus::SalesInvoiceRaised);
    qStr += " ) items GROUP BY " + Sales_Order_Details_Inventory_Item_ID;

    qDebug()<<qStr;

    return db->getQueryModel(qStr);
}

QSqlQueryModel *SalesOrderDatabaseHelper::getEmployeesBookedBetween(QDateTime fromDate, QDateTime toDate, QString EmployeeGroupID)
{
    QString dfrom = fromDate.toString("yyyy-MM-dd hh:mm");
    QString dto = toDate.toString("yyyy-MM-dd hh:mm");

    QString qStr = " SELECT Salesman_ID ";
    qStr += " FROM sales_order_main ";
    qStr += " WHERE ";
//    qStr += "Salesman_ID = $salesmanID	";
//    qStr += " AND ";
    qStr += " ( delivery_date >= STR_TO_DATE('"+dfrom+"', '%Y-%m-%d %H:%i:%s') AND delivery_date < STR_TO_DATE('"+dto+"', '%Y-%m-%d %H:%i:%s'))";
    qStr += " OR ( Customer_Expected_Date > STR_TO_DATE('"+dfrom+"', '%Y-%m-%d %H:%i:%s') AND Customer_Expected_Date <= STR_TO_DATE('"+dto+"', '%Y-%m-%d %H:%i:%s'))";

    QSqlQueryModel *m = db->getQueryModel(qStr);
    QString emplist;

    for(int i=0;i<m->rowCount();i++){
        emplist+= m->record(i).value(0).toString()+",";
    }
    emplist.chop(1);

    QString qStr2 = "select _id,name from employee_details where";

    if(m->rowCount()>0)
        qStr2 += " _id not in ("+emplist+") and ";

    qStr2 += " UserGroupID like '"+EmployeeGroupID+"'";

    return db->getQueryModel(qStr2);

}

QSqlQueryModel *SalesOrderDatabaseHelper::getEmployeeBookedSlotsByDate(QString EmployeeID, QDateTime fromDate, QDateTime toDate)
{
    QString qStr = " SELECT " ;

    return db->getQueryModel(qStr);
}

QSqlQueryModel *SalesOrderDatabaseHelper::getItemwisePendingOrders(QDate fromDate, QDate toDate)
{
    QString qStr = " SELECT " ;
    qStr += " (select item_name FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where item_id = " + Sales_Order_Details_Inventory_Item_ID + ") as Item, ";
    qStr += " SUM(" + Sales_Order_Details_Quantity + ") as Quantity FROM ";
    qStr += " ( SELECT " + Sales_Order_Details_Inventory_Item_ID + " , " + Sales_Order_Details_Quantity;
    qStr += " FROM " + Sales_Order_Main_TableName + " main ";
    qStr += " JOIN " + Sales_Order_Details_TableName + " det ";
    qStr += " ON main." + Sales_Order_Main_Voucher_No + " = det." + Sales_Order_Details_Voucher_No;
    qStr += " WHERE ";
    qStr += Sales_Order_Main_Voucher_Date + " < " + db->getDateString(toDate.addDays(1));
    qStr += " AND ";
    qStr += Sales_Order_Main_Voucher_Date + " >= " + db->getDateString(fromDate);
    qStr += " AND " + Sales_Order_Main_VoucherStatus + " < " + QString::number(QuotationStatus::SalesInvoiceRaised);
    qStr += " ) items GROUP BY " + Sales_Order_Details_Inventory_Item_ID;
    qStr += " order by 1";
    return db->getQueryModel(qStr);
}



QSqlQueryModel *SalesOrderDatabaseHelper::getVoucherListByDateModel(QDate dateFrom, QDate dateTo)
{

}
