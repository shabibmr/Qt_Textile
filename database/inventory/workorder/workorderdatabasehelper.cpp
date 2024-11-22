#include "workorderdatabasehelper.h"

QString WorkOrderDatabaseHelper::Work_Order_Main_TableName = "Work_Order_Main";

QString WorkOrderDatabaseHelper::Work_Order_Main_id = "_id";
QString WorkOrderDatabaseHelper::Work_Order_Main_Voucher_No = "Voucher_No";
QString WorkOrderDatabaseHelper::Work_Order_Main_Voucher_Prefix = "Voucher_Prefix";
QString WorkOrderDatabaseHelper::Work_Order_Main_Date_Created = "Date_Created";
QString WorkOrderDatabaseHelper::Work_Order_Main_Voucher_Date = "Voucher_Date";
QString WorkOrderDatabaseHelper::Work_Order_Main_TimeStamp = "`TimeStamp`";
QString WorkOrderDatabaseHelper::Work_Order_Main_TimeStampC = "TimeStamp";

QString WorkOrderDatabaseHelper::Work_Order_Main_Last_Updated="Last_Updated";
QString WorkOrderDatabaseHelper::Work_Order_Main_Ledger_ID = "Ledger_ID";
QString WorkOrderDatabaseHelper::Work_Order_Main_Billing_Name = "Billing_Name";
QString WorkOrderDatabaseHelper::Work_Order_Main_Total = "Total";
QString WorkOrderDatabaseHelper::Work_Order_Main_Narration = "Narration";
QString WorkOrderDatabaseHelper::Work_Order_Main_Added_By = "Added_By";
QString WorkOrderDatabaseHelper::Work_Order_Main_Delivery_Date = "Delivery_Date";
QString WorkOrderDatabaseHelper::Work_Order_Main_Completion_Probability = "Completion_Probability";
QString WorkOrderDatabaseHelper::Work_Order_Main_Credit_Period = "Credit_Period";
QString WorkOrderDatabaseHelper::Work_Order_Main_POC_Name = "POC_Name";
QString WorkOrderDatabaseHelper::Work_Order_Main_POC_Phone = "POC_Phone";
QString WorkOrderDatabaseHelper::Work_Order_Main_POC_Email = "POC_Email";
QString WorkOrderDatabaseHelper::Work_Order_Main_Project_ID = "Project_ID";
QString WorkOrderDatabaseHelper::Work_Order_Main_Revision_Number = "Revision_Number";
QString WorkOrderDatabaseHelper::Work_Order_Main_Converted_To_Sales_Order = "Converted_To_Sales_Order";
QString WorkOrderDatabaseHelper::Work_Order_Main_Quotation_Dropped = "Quotation_Dropped";
QString WorkOrderDatabaseHelper::Work_Order_Main_Quotation_Dropped_Reason = "Quotation_Dropped_Reason";
QString WorkOrderDatabaseHelper::Work_Order_Main_Requirement_Voucher_No = "Requirement_Voucher_No";
QString WorkOrderDatabaseHelper::Work_Order_Main_Salesman_ID = "Salesman_ID";
QString WorkOrderDatabaseHelper::Work_Order_Main_LPO = "LPO_Ref";
QString WorkOrderDatabaseHelper::Work_Order_Main_VoucherStatus = "VoucherStatus";
QString WorkOrderDatabaseHelper::Work_Order_Main_ModeOfService = "MODE_OF_SERVICE";
QString WorkOrderDatabaseHelper::Work_Order_Main_Discount_In_Amount = "Discount_In_Amount";
QString WorkOrderDatabaseHelper::Work_Order_Main_Discount_In_Percent = "Discount_In_Percent";
QString WorkOrderDatabaseHelper::Work_Order_Main_VATAmount = "VAT_Amount";
QString WorkOrderDatabaseHelper::Work_Order_Main_Contact_ID = "ContactID";
QString WorkOrderDatabaseHelper::Work_Order_Main_Gross_Total = "Gross_Total";
QString WorkOrderDatabaseHelper::Work_Order_Main_Reference = "Reference";
QString WorkOrderDatabaseHelper::Work_Order_Main_Customer_Expecting_Date = "Customer_Expected_Date";
QString WorkOrderDatabaseHelper::Work_Order_Main_Currency = "Currency";
QString WorkOrderDatabaseHelper::Work_Order_Main_BoxesCount = "Boxes_Count";
QString WorkOrderDatabaseHelper::Work_Order_Main_TotalWeight = "Total_Weight";
QString WorkOrderDatabaseHelper::Work_Order_Main_LRNo = "LR_No";
QString WorkOrderDatabaseHelper::Work_Order_Main_VoucherNoDisplay = "Voucher_No_Display";
QString WorkOrderDatabaseHelper::Work_Order_Main_ReqVoucherList = "Requirement_Voucher_List";
QString WorkOrderDatabaseHelper::Work_Order_Main_Origin = "Origin";
QString WorkOrderDatabaseHelper::Work_Order_Main_RoundOff = "Round_Off";
QString WorkOrderDatabaseHelper::Work_Order_Main_ToLocation = "Section";
QString WorkOrderDatabaseHelper::Work_Order_Main_TransactionId = "TransactionId";
QString WorkOrderDatabaseHelper::Work_Order_Main_Completed_TimeStamp="Completed_TimeStamp";

QString WorkOrderDatabaseHelper::WorkOrder_tableName = "work_order_detail";
QString WorkOrderDatabaseHelper::WorkOrder_id ="_id";
QString WorkOrderDatabaseHelper::WorkOrder_WorkOrder_id="WorkOrder_id"; //voucher_no
QString WorkOrderDatabaseHelper::WorkOrder_VoucherPrefix="Voucher_Prefix";
QString WorkOrderDatabaseHelper::WorkOrder_itemID="itemID";
QString WorkOrderDatabaseHelper::WorkOrder_UOM_ID = "Uom_ID";
QString WorkOrderDatabaseHelper::WorkOrder_qty="Quantity";
QString WorkOrderDatabaseHelper::WorkOrder_Price="Price";
QString WorkOrderDatabaseHelper::WorkOrder_SerialNo="SerialNo";
QString WorkOrderDatabaseHelper::WorkOrder_StartTimestamp="Start_TimeStamp";
QString WorkOrderDatabaseHelper::WorkOrder_CompletedDate="CompletedDate";
QString WorkOrderDatabaseHelper::WorkOrder_TimeStamp="TimeStamp";
QString WorkOrderDatabaseHelper::WorkOrder_Section="Section";
QString WorkOrderDatabaseHelper::WorkOrder_itemProductionStatus ="itemProductionStatus";
QString WorkOrderDatabaseHelper::WorkOrder_Requirement_ItemID ="Requirement_ItemID";

QString WorkOrderDatabaseHelper::WorkOrder_SalesOrder_Id = "SalesOrder_Id";
QString WorkOrderDatabaseHelper::WorkOrder_requirement_no="requirement_no";
QString WorkOrderDatabaseHelper::WorkOrder_status="status";
QString WorkOrderDatabaseHelper::WorkOrder_TechnicanID="TechnicanID";
QString WorkOrderDatabaseHelper::WorkOrder_Expected_Date="Expected_Date";
QString WorkOrderDatabaseHelper::WorkOrder_CompletionTimestamp="Completion_Timestamp";
QString WorkOrderDatabaseHelper::WorkOrder_DN_id = "Dn_id";
QString WorkOrderDatabaseHelper::WorkOrder_itemVoucherStatus ="ItemVoucherStatus";
QString WorkOrderDatabaseHelper::WorkOrder_Project_ID = "Project_ID";
QString WorkOrderDatabaseHelper::WorkOrder_Salesman_ID = "Salesman_ID";

QString WorkOrderDatabaseHelper::VoucherType = GMVoucherTypes::getVoucherString(GMVoucherTypes::WorkOrder); //"WORK ORDER";

WorkOrderDatabaseHelper::WorkOrderDatabaseHelper(QString connString)
{
    db = new DatabaseHelper(connString);

}

WorkOrderDatabaseHelper::~WorkOrderDatabaseHelper()
{

}
QString WorkOrderDatabaseHelper::getNextVoucherNo(QString voucherPrefix)
{
    GeneralVoucherDatabaseHelper vHelper;
    QString voucherNumber = vHelper.getNextVoucherNoByType(VoucherType, voucherPrefix, Work_Order_Main_TableName);
    return voucherNumber;
}

GeneralVoucherDataObject WorkOrderDatabaseHelper::getWorkOrderBySalesOrder(QString SalesOrderID)
{
    GeneralVoucherDataObject obj;
}

QPair<QString, QString> WorkOrderDatabaseHelper::getVoucherNoByTransactionId(QString transId)
{
    GeneralVoucherDatabaseHelper vHelper;
    return  vHelper.getVoucherNoByTransactionId(transId, Work_Order_Main_TableName);

}

int WorkOrderDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

int WorkOrderDatabaseHelper::insertVoucher(GeneralVoucherDataObject *obj)
{

    QString newid;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    this->startTransaction();

    GeneralVoucherDatabaseHelper vHelper;
    obj->voucherNumber = vHelper.setNextVoucherNoByType(VoucherType, obj->VoucherPrefix, Work_Order_Main_TableName );

    if(obj->TransactionId.length()==0)
    {
        qDebug()<<"Created New Work Order";
        obj->TransactionId = db->getUUID();
    }

    QMap<QString,QVariant> map1;
    map1.insert(Work_Order_Main_Voucher_No,obj->voucherNumber);
    map1.insert(Work_Order_Main_Voucher_Prefix,obj->VoucherPrefix);
    map1.insert(Work_Order_Main_Date_Created,obj->VoucherDate);
    map1.insert(Work_Order_Main_Voucher_Date,obj->VoucherDate);
    map1.insert(Work_Order_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    map1.insert(Work_Order_Main_Billing_Name,obj->ledgerObject.LedgerName);
    map1.insert(Work_Order_Main_Total,obj->grandTotal);
    map1.insert(Work_Order_Main_Narration,obj->narration);
    map1.insert(Work_Order_Main_Added_By,obj->AddedById);
    map1.insert(Work_Order_Main_Delivery_Date,obj->DeliveryDate);
    map1.insert(Work_Order_Main_Completion_Probability,obj->CompletionProbability);
    map1.insert(Work_Order_Main_Credit_Period,obj->ledgerObject.Credit_Period);
    map1.insert(Work_Order_Main_POC_Name,obj->ledgerObject.ContactPersonName);
    map1.insert(Work_Order_Main_POC_Phone,obj->Contact.PhoneNumber);
    map1.insert(Work_Order_Main_POC_Email,obj->ledgerObject.ContactPersonEmail);
    map1.insert(Work_Order_Main_Project_ID,obj->ProjectId);
    map1.insert(Work_Order_Main_Revision_Number,obj->RevisionNo);
    map1.insert(Work_Order_Main_Converted_To_Sales_Order,obj->ConvertedToSalesOrder);
    map1.insert(Work_Order_Main_Quotation_Dropped,obj->QuotationDropped);
    map1.insert(Work_Order_Main_Quotation_Dropped_Reason,obj->QuotationDroppedReason);
    map1.insert(Work_Order_Main_Requirement_Voucher_No,obj->RequirementVoucherNo);
    map1.insert(Work_Order_Main_Salesman_ID,obj->SalesmanID);
    map1.insert(Work_Order_Main_LPO,obj->LPO);
    map1.insert(Work_Order_Main_ModeOfService,obj->ModeOfService);
    map1.insert(Work_Order_Main_Contact_ID,obj->Contact._id);
    map1.insert(Work_Order_Main_VATAmount,obj->taxTotalAmount);
    map1.insert(Work_Order_Main_VoucherStatus,obj->status);
    map1.insert(Work_Order_Main_Discount_In_Amount,obj->discountinAmount);
    map1.insert(Work_Order_Main_Discount_In_Percent,obj->discountPercent);
    map1.insert(Work_Order_Main_Gross_Total,obj->grossTotal);
    map1.insert(Work_Order_Main_Reference,obj->reference);
    map1.insert(Work_Order_Main_TimeStampC,obj->timestamp);
    map1.insert(Work_Order_Main_Last_Updated,QDateTime::currentDateTime());
    map1.insert(Work_Order_Main_ToLocation, obj->toGodownID);
    map1.insert(Work_Order_Main_TransactionId, obj->TransactionId);
    map1.insert(Work_Order_Main_Completed_TimeStamp,obj->completedTimeStamp);

    QJsonDocument doc(obj->ReqVoucherList);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    map1.insert(Work_Order_Main_ReqVoucherList,strJson);

    db->insertRecordsPlus(map1,Work_Order_Main_TableName);

    for(CompoundItemDataObject item:obj->InventoryItems){
        QMap<QString,QVariant> map;
        map.insert(WorkOrder_WorkOrder_id,obj->voucherNumber);
        map.insert(WorkOrder_VoucherPrefix, obj->VoucherPrefix);
        map.insert(WorkOrder_qty,item.BaseItem.quantity);
        map.insert(WorkOrder_itemID,item.BaseItem.ItemID);
        map.insert(WorkOrder_Price, item.BaseItem.price);
        map.insert(WorkOrder_UOM_ID, item.BaseItem.uomObject._id);
        map.insert(WorkOrder_SerialNo,item.BaseItem.SerailNumber);
        map.insert(WorkOrder_itemProductionStatus,item.BaseItem.itemProductionStatus);
        map.insert(WorkOrder_Salesman_ID, obj->SalesmanID);
        map.insert(WorkOrder_Section, item.BaseItem.section);
        if(item.BaseItem.ItemReqUuid == "X"){
            item.BaseItem.ItemReqUuid = db->getUUID();
        }
        map.insert(WorkOrder_Requirement_ItemID, item.BaseItem.ItemReqUuid);

        map.insert(WorkOrder_StartTimestamp , QDateTime::currentDateTime());
        if(item.BaseItem.itemProductionStatus == QuotationStatus::WorkOrderCompleted)
            map.insert(WorkOrder_CompletedDate, item.BaseItem.orderCompletedDate);

        db->insertRecordsPlus(map,WorkOrder_tableName);

        // Insert into transaction_Item_helper

        if(item.BaseItem.itemProductionStatus == QuotationStatus::WorkOrderCompleted ||
                item.BaseItem.drQty > 0){

            //        if(item.BaseItem.fifoList.size() > 0){

            ItemTransactionDataModel *ItemModel = new ItemTransactionDataModel();
            ItemTransactionDatabaseHelper *itemTransaction = new ItemTransactionDatabaseHelper(db);
            ItemModel->ItemID = item.BaseItem.ItemID;
            ItemModel->From_Ledger = item.BaseItem.defaultSalesLedgerID;
            ItemModel->To_Ledger = obj->ledgerObject.LedgerID;
            ItemModel->VoucherID = obj->voucherNumber;
            ItemModel->VoucherPrefix = obj->VoucherPrefix;
            ItemModel->VoucherDate = obj->VoucherDate;
            ItemModel->VoucherType = VoucherType;
            ItemModel->From_Location = "OUT"; //item.BaseItem.fromGodownID;
            ItemModel->To_Location = item.BaseItem.toGodownID;
            ItemModel->drQty = item.BaseItem.drQty;
            ItemModel->crQty = 0;
            ItemModel->drAmount = item.BaseItem.subTotal;
            ItemModel->itemReqId = item.BaseItem.ItemReqUuid;
            ItemModel->Narration = obj->narration;

            itemTransaction->insertItemTransaction(ItemModel, item.BaseItem.godownList);

        }

        // Update salesOrderStatus

        float pendingQty = vHelper.getPendingQtyToExportByItem(GMVoucherTypes::SalesOrder, GMVoucherTypes::WorkOrder, item.BaseItem.ItemReqUuid);
        if(pendingQty == 0){
            updateSalesOrderStatus(item.BaseItem.ItemReqUuid, item.BaseItem.itemProductionStatus);
        }

    }
    qDebug()<<"207";
    return commit();
}

int WorkOrderDatabaseHelper::updateVoucher(GeneralVoucherDataObject *obj, QString oldID)
{
    this->startTransaction();

    bool WorkOrderCompleted = false;
    QMap<QString,QVariant> map;
    QMap<QString,QVariant> map2;

    obj->voucherType = VoucherType;

    map2.insert(Work_Order_Main_Voucher_No,obj->voucherNumber);
    map2.insert(Work_Order_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Work_Order_Main_Date_Created,obj->VoucherDate);
    map.insert(Work_Order_Main_Voucher_Date,obj->VoucherDate);
    map.insert(Work_Order_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    map.insert(Work_Order_Main_Billing_Name,obj->ledgerObject.LedgerName);
    map.insert(Work_Order_Main_Total,obj->grandTotal);
    map.insert(Work_Order_Main_Narration,obj->narration);
    map.insert(Work_Order_Main_Added_By,obj->AddedById);
    map.insert(Work_Order_Main_Delivery_Date,obj->DeliveryDate);
    map.insert(Work_Order_Main_Completion_Probability,obj->CompletionProbability);
    map.insert(Work_Order_Main_Credit_Period,obj->ledgerObject.Credit_Period);
    map.insert(Work_Order_Main_POC_Name,obj->ledgerObject.ContactPersonName);
    map.insert(Work_Order_Main_POC_Phone,obj->Contact.PhoneNumber);
    map.insert(Work_Order_Main_POC_Email,obj->ledgerObject.ContactPersonEmail);
    map.insert(Work_Order_Main_Project_ID,obj->ProjectId);
    map.insert(Work_Order_Main_Revision_Number,obj->RevisionNo);
    map.insert(Work_Order_Main_Converted_To_Sales_Order,obj->ConvertedToSalesOrder);
    map.insert(Work_Order_Main_Quotation_Dropped,obj->QuotationDropped);
    map.insert(Work_Order_Main_Quotation_Dropped_Reason,obj->QuotationDroppedReason);
    map.insert(Work_Order_Main_Requirement_Voucher_No,obj->RequirementVoucherNo);
    map.insert(Work_Order_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Work_Order_Main_LPO,obj->LPO);
    map.insert(Work_Order_Main_ModeOfService,obj->ModeOfService);
    map.insert(Work_Order_Main_Contact_ID,obj->Contact._id);
    map.insert(Work_Order_Main_VATAmount,obj->taxTotalAmount);
    map.insert(Work_Order_Main_VoucherStatus,obj->status);
    map.insert(Work_Order_Main_Discount_In_Amount,obj->discountinAmount);
    map.insert(Work_Order_Main_Discount_In_Percent,obj->discountPercent);
    map.insert(Work_Order_Main_Gross_Total,obj->grossTotal);
    map.insert(Work_Order_Main_Reference,obj->reference);
    map.insert(Work_Order_Main_Last_Updated,QDateTime::currentDateTime());
    map.insert(Work_Order_Main_TransactionId, obj->TransactionId);
    map.insert(Work_Order_Main_Completed_TimeStamp,obj->completedTimeStamp);


    QJsonDocument doc(obj->ReqVoucherList);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    map.insert(Work_Order_Main_ReqVoucherList,strJson);
    qDebug()<<"section"<<obj->toGodownID;
    map.insert(Work_Order_Main_ToLocation, obj->toGodownID);

    db->updateRecordsPlus(map,map2,Work_Order_Main_TableName);

    QMap<QString,QVariant> map3;
    map3.insert(WorkOrder_WorkOrder_id,obj->voucherNumber);
    map3.insert(WorkOrder_VoucherPrefix,obj->VoucherPrefix);

    db->deleteRecordsPlus(map3,WorkOrder_tableName);
    ItemTransactionDataModel *ItemModel = new ItemTransactionDataModel();
    ItemTransactionDatabaseHelper *itemTransaction = new ItemTransactionDatabaseHelper(db);

    itemTransaction->deleteItemTransaction(obj);

    for(CompoundItemDataObject item:obj->InventoryItems){
        QMap<QString,QVariant> map4;
        map4.insert(WorkOrder_WorkOrder_id,obj->voucherNumber);
        map4.insert(WorkOrder_VoucherPrefix, obj->VoucherPrefix);
        map4.insert(WorkOrder_Requirement_ItemID,item.BaseItem.ItemReqUuid);
        map4.insert(WorkOrder_qty,item.BaseItem.quantity);
        map4.insert(WorkOrder_itemID,item.BaseItem.ItemID);
        map4.insert(WorkOrder_Price, item.BaseItem.price);
        map4.insert(WorkOrder_UOM_ID, item.BaseItem.uomObject._id);
        map4.insert(WorkOrder_SerialNo,item.BaseItem.SerailNumber);
        map4.insert(WorkOrder_itemProductionStatus,item.BaseItem.itemProductionStatus);
        map4.insert(WorkOrder_Section, item.BaseItem.section);
        map4.insert(WorkOrder_StartTimestamp , QDateTime::currentDateTime());
        map.insert(WorkOrder_Salesman_ID, obj->SalesmanID);
        if(item.BaseItem.ItemReqUuid == "X"){
            item.BaseItem.ItemReqUuid = db->getUUID();
        }
        map4.insert(WorkOrder_Requirement_ItemID, item.BaseItem.ItemReqUuid);

        if(item.BaseItem.itemProductionStatus == QuotationStatus::WorkOrderCompleted)
            map4.insert(WorkOrder_CompletedDate, item.BaseItem.orderCompletedDate);

        db->insertRecordsPlus(map4,WorkOrder_tableName);



        if(item.BaseItem.itemProductionStatus == QuotationStatus::WorkOrderCompleted
                || item.BaseItem.drQty > 0 ){
            //        if(item.BaseItem.fifoList.size() > 0) {

            qDebug()<<"entry in wo fifo"<<item.BaseItem.godownList.size()<<item.BaseItem.drQty;
            ItemModel->ItemID = item.BaseItem.ItemID;
            ItemModel->From_Ledger = item.BaseItem.defaultSalesLedgerID;
            ItemModel->To_Ledger = obj->ledgerObject.LedgerID;
            ItemModel->VoucherID = obj->voucherNumber;
            ItemModel->VoucherPrefix = obj->VoucherPrefix;
            ItemModel->VoucherDate = obj->VoucherDate;
            ItemModel->VoucherType = VoucherType;
            //            ItemModel->From_Location = obj->fromGodownName;
            ItemModel->From_Location = "OUT";
            ItemModel->To_Location = item.BaseItem.toGodownID;
            ItemModel->drQty = item.BaseItem.drQty;
            ItemModel->crQty = 0;
            ItemModel->drAmount = item.BaseItem.subTotal;
            ItemModel->itemReqId = item.BaseItem.ItemReqUuid;
            ItemModel->Narration = obj->narration;


            itemTransaction->insertItemTransaction(ItemModel, item.BaseItem.godownList);
        }

        GeneralVoucherDatabaseHelper vHelper;
        float pendingQty = vHelper.getPendingQtyToExportByItem(GMVoucherTypes::SalesOrder, GMVoucherTypes::WorkOrder, item.BaseItem.ItemReqUuid);
        if(pendingQty == 0){
            updateSalesOrderStatus(item.BaseItem.ItemReqUuid, item.BaseItem.itemProductionStatus);
        }
        if(item.BaseItem.quantity == item.BaseItem.drQty)
            WorkOrderCompleted = true;
    }

    if(WorkOrderCompleted){

        StockRequestDatabaseHelper* stockHelper = new StockRequestDatabaseHelper();
        GeneralVoucherDataObject* issueRequest;// = new GeneralVoucherDataObject();
        issueRequest = stockHelper->getVoucherByWorkOrderId(obj->voucherNumber);
        if(issueRequest->voucherNumber != ""){
            issueRequest->voucherNumber = obj->voucherNumber;
            issueRequest->voucherType = "CONSUMPTION";
        }
        ItemTransactionDatabaseHelper *itemTransaction = new ItemTransactionDatabaseHelper(db);

        itemTransaction->deleteItemTransaction(issueRequest);



        //            itemTransaction->deleteItemTransaction(issueRequest);
        for(int i=0; i<issueRequest->InventoryItems.size();i++){

            CompoundItemDataObject item2 = issueRequest->InventoryItems[i];
            ItemTransactionDataModel *consumption = new ItemTransactionDataModel();

            consumption->ItemID = item2.BaseItem.ItemID;
            consumption->From_Ledger = item2.BaseItem.defaultSalesLedgerID;
            consumption->To_Ledger = issueRequest->ledgerObject.LedgerID;
            consumption->VoucherID = obj->voucherNumber;
            consumption->VoucherPrefix = issueRequest->VoucherPrefix;
            consumption->VoucherDate = issueRequest->VoucherDate;
            consumption->VoucherType = issueRequest->voucherType;
            consumption->From_Location = issueRequest->toGodownID;
            consumption->To_Location = "OUT";
            consumption->drQty = 0;
            consumption->crQty = item2.BaseItem.crQty;
            consumption->crAmount = item2.BaseItem.subTotal;
            consumption->itemReqId = item2.BaseItem.ItemReqUuid;
            consumption->Narration = issueRequest->narration;

            FifoCalculator *fifoCalc = new FifoCalculator();
            //                fifoCalc->setItemQtyByFifo(issueRequest,i, 0, false);
            //                qDebug()<<issueRequest->InventoryItems[i].BaseItem.godownList;

            itemTransaction->insertItemTransaction(consumption, issueRequest->InventoryItems[i].BaseItem.godownList);

        }
    }




    return commit();

}

int WorkOrderDatabaseHelper::deleteVoucher(GeneralVoucherDataObject *obj)
{
    this->startTransaction();
    QMap<QString,QVariant> mapClause;

    mapClause.insert(WorkOrder_WorkOrder_id,obj->voucherNumber);
    mapClause.insert(WorkOrder_VoucherPrefix,obj->VoucherPrefix);

    db->deleteRecordsPlus(mapClause,WorkOrder_tableName);

    return commit();

}

void WorkOrderDatabaseHelper::updateSalesOrderStatus(QString itemReqId, int status)
{
    QMap<QString, QVariant> clause;
    QMap<QString, QVariant> statMap;

    statMap.insert(SalesOrderDatabaseHelper::Sales_Order_Details_itemProductionStatus, status);
    clause.insert(SalesOrderDatabaseHelper::Sales_Order_Details_Requirement_ItemID, itemReqId);

    db->updateRecordsPlus(statMap, clause, SalesOrderDatabaseHelper::Sales_Order_Details_TableName);

    // Check sales order main

}

int WorkOrderDatabaseHelper::updateWOStatusByReqVoucherNo(QString reqVoucherNo, int status)
{

    startTransaction();
    QMap<QString, QVariant> clause;
    QMap<QString, QVariant> data;

    clause.insert(Work_Order_Main_Requirement_Voucher_No, reqVoucherNo);
    data.insert(Work_Order_Main_VoucherStatus, status);

    db->updateRecordsPlus(data, clause, Work_Order_Main_TableName);

    return commit();

}



int WorkOrderDatabaseHelper::commit()
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

int WorkOrderDatabaseHelper::getMaxID()
{
    QString qStr = "SELECT MAX("+WorkOrder_id+") FROM " + WorkOrder_tableName;
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

GeneralVoucherDataObject *WorkOrderDatabaseHelper::getVoucherByVoucherNoPtr(QString vNo, QString voucherPrefix)
{
    GeneralVoucherDataObject *obj = new GeneralVoucherDataObject();
    SalesInventoryItemDatabaseHelper *itemHelper = new SalesInventoryItemDatabaseHelper();
    LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper();
    AddressBookDatabaseHelper *addHelper = new AddressBookDatabaseHelper();
    UomDataBaseHelper *uomHelper = new UomDataBaseHelper();

    QString qStrMain ="SELECT "
            + Work_Order_Main_id + ", "
            + Work_Order_Main_Voucher_No + ", "
            + Work_Order_Main_Voucher_Prefix + ", "
            + Work_Order_Main_Date_Created + ", "
            + Work_Order_Main_Voucher_Date + ", "
            + Work_Order_Main_TimeStamp + ", "
            + Work_Order_Main_Ledger_ID + ", "
            + Work_Order_Main_Billing_Name + ", "
            + Work_Order_Main_Total + ", "
            + Work_Order_Main_Narration + ", "
            + Work_Order_Main_Added_By + ", "
            + Work_Order_Main_Delivery_Date + ", "
            + Work_Order_Main_Completion_Probability + ", "
            + Work_Order_Main_Credit_Period + ", "
            + Work_Order_Main_POC_Name + ", "
            + Work_Order_Main_POC_Phone + ", "
            + Work_Order_Main_POC_Email + ", "
            + Work_Order_Main_Project_ID + ", "
            + Work_Order_Main_Revision_Number + ", "
            + Work_Order_Main_Converted_To_Sales_Order + ", "
            + Work_Order_Main_Quotation_Dropped + ", "
            + Work_Order_Main_Quotation_Dropped_Reason + ", "
            + Work_Order_Main_Requirement_Voucher_No + ", "
            + Work_Order_Main_Salesman_ID + ", "
            + Work_Order_Main_LPO + ", "
            + Work_Order_Main_VoucherStatus + ", "
            + Work_Order_Main_ModeOfService + ", "
            + Work_Order_Main_Discount_In_Amount + ", "
            + Work_Order_Main_Discount_In_Percent + ", "
            + Work_Order_Main_VATAmount + ", "
            + Work_Order_Main_Contact_ID + ", "
            + Work_Order_Main_Gross_Total + ", "
            + Work_Order_Main_Reference + ", "
            + Work_Order_Main_Customer_Expecting_Date + ", "
            + Work_Order_Main_Currency + ", "
            + Work_Order_Main_BoxesCount+ ", "
            + Work_Order_Main_TotalWeight + ", "
            + Work_Order_Main_LRNo + ", "
            + Work_Order_Main_VoucherNoDisplay + ", "
            + Work_Order_Main_ReqVoucherList + ", "
            + Work_Order_Main_Origin + ", "
            + Work_Order_Main_ToLocation + ", "
            + Work_Order_Main_TransactionId + ", "
            + Work_Order_Main_Completed_TimeStamp
            + " FROM " + Work_Order_Main_TableName
            + " WHERE " + Work_Order_Main_Voucher_No + "= '" + vNo + "'"
            + " AND " +  Work_Order_Main_Voucher_Prefix + "= '" + voucherPrefix + "'";



    QString qStrDetail = " SELECT distinct " ;
    qStrDetail += "s." + WorkOrder_itemID+ ", ";
    qStrDetail += WorkOrder_qty+ ", ";
    qStrDetail += WorkOrder_UOM_ID + ", ";
    qStrDetail += " subq.convalue , subq.UomName, subq.UomSymbol,  subq.barCode , ";
    qStrDetail += WorkOrder_SerialNo+ ", ";
    qStrDetail += WorkOrder_itemProductionStatus+ ", ";
    qStrDetail += WorkOrder_Requirement_ItemID + ", ";
    qStrDetail += WorkOrder_CompletedDate;
    qStrDetail += " FROM " + WorkOrder_tableName  + " s LEFT OUTER JOIN " + UomDataBaseHelper::qStrUomForItemDetail + " as subq ";
    qStrDetail += " ON s." + WorkOrder_itemID + " = subq.itemID ";
    qStrDetail += " AND s." + WorkOrder_UOM_ID + " = subq.UOM ";
    qStrDetail += " WHERE " + WorkOrder_WorkOrder_id + "=:id";
    qStrDetail += " AND " +  WorkOrder_VoucherPrefix + "= '" + voucherPrefix + "'";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));


    query.prepare(qStrDetail);
    query.bindValue(":id",vNo);
    if(!query.exec()){
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    else{
        while(query.next()){
            int i=3;

            inventoryItemDataModel item = itemHelper->getInventoryItemByID(query.value(0).toString());

            item.quantity = query.value(1).toFloat();

            item.uomObject._id = query.value(2).toString();
            item.uomObject.convRate = query.value(3).toFloat();
            item.uomObject.UomName = query.value(4).toString();
            item.uomObject.UomSymbol = query.value(5).toString();
            item.uomObject.barCode = query.value(6).toString();

            item.SerailNumber = query.value(7).toString();
            item.itemProductionStatus = query.value(8).toInt();
            item.ItemReqUuid = query.value(9).toString();
            item.orderCompletedDate = query.value(10).toDate();

            ItemTransactionDatabaseHelper *itemTransaction = new ItemTransactionDatabaseHelper(db);
            //            item.fifoList = itemTransaction->getFifoListByVoucherItem(vNo, VoucherType,
            //                                                                               voucherPrefix, item.ItemID);

            //            for(ItemFifoDataModel *row:item.fifoList)
            //                item.drQty += row->drQty;
            item.godownList = itemTransaction->getGodownAllocationByVoucherAndItem
                    (vNo, VoucherType, voucherPrefix,item.ItemReqUuid);
            for(GodownTransactionDataModel godownEntry: item.godownList)
                item.drQty += godownEntry.drQty;

            qDebug()<<"dr qty  ***********"<<item.drQty<<item.godownList.size();
            item.prevQty = item.drQty;

            CompoundItemDataObject cObj;
            cObj.BaseItem = item;
            obj->InventoryItems.append(cObj);

            qDebug()<<"query size"<<query.record().count()<<"Details";
            //            }
            //            else{


            //                int itemSize = obj->InventoryItems.size();
            //                obj->InventoryItems[itemSize-1].subItems.append(item);
            //                obj->InventoryItems[itemSize-1].subItemsTotal +=item.subTotal;
            //            }
        }
    }

    //    qDebug()<<qStrDetail;


    query.prepare(qStrMain);
    //    qDebug()<<qStrMain;
    //    query.bindValue(":vno",vNo);
    //    query.bindValue(":voucherPrefix", voucherPrefix);
    if(!query.exec()){
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    else{

        while(query.next()){
            int i=1;



            obj->voucherNumber = query.value(i++).toString();
            obj->voucherType = VoucherType;
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
            obj->toGodownID = query.value(i++).toString();
            obj->TransactionId = query.value(i++).toString();
            obj->completedTimeStamp = query.value(i++).toDateTime();
            qDebug()<<"query size"<<query.record().count()<<i<<"Main";
        }
    }

    return obj;

}

QSqlQueryModel *WorkOrderDatabaseHelper::getVoucherListByDateModel(QDate dateFrom, QDate dateTo)
{
    LedgerMasterDatabaseHelper ledHelper;

    QString qStr ="SELECT ";
    qStr += "'V' , ";
    qStr += Work_Order_Main_Voucher_No +", ";
    qStr += "'" + VoucherType + "', " ;
    qStr += Work_Order_Main_Voucher_Prefix + ", ";
    qStr += Work_Order_Main_Voucher_Date+" as 'Voucher Date', ";
    qStr += " CONCAT(" + WorkOrder_VoucherPrefix + ", '-', " + Work_Order_Main_Voucher_No+ ") as 'WorkOrder No' ,";
    qStr += Work_Order_Main_VoucherStatus + " as 'Status'";
    //        qStr += "(SELECT "+ledHelper.Ledger_Master_LedgerName + " FROM "+ ledHelper.Ledger_Master_TableName;
    //        qStr += " WHERE "+ledHelper.Ledger_Master_LedgerID + "= som.";
    //        qStr += Sales_Order_Main_Ledger_ID + ") Ledger, ";
    //    qStr += Sales_Order_Main_Total ;
    qStr += " FROM " + Work_Order_Main_TableName;
    qStr += " som WHERE ";
    qStr += Work_Order_Main_Voucher_Date;
    qStr += " >= '";
    qStr += dateFrom.toString("yyyy-MM-dd");
    qStr += "' AND ";
    qStr += Work_Order_Main_Voucher_Date;
    qStr += " <= '";
    qStr += dateTo.toString("yyyy-MM-dd");
    qStr += "'";


    //    qDebug()<<qStr;

    return DatabaseHelper::getQueryModel(qStr);

}

QSqlQueryModel *WorkOrderDatabaseHelper::getVoucherListByDateTimeModel(QDateTime dateFrom, QDateTime dateTo)
{

}


QString WorkOrderDatabaseHelper::getVoucherNumberByLedID(QString LedID, QDate Date)
{

}

QSqlQueryModel *WorkOrderDatabaseHelper::getWorkOrdersByReqVoucherNo()
{
    QString qStr = "SELECT " + Work_Order_Main_Requirement_Voucher_No + ", ";
    qStr += Work_Order_Main_Voucher_Date + " Date, ";
    qStr += "( select name from employee_Details where _id = " + Work_Order_Main_Salesman_ID + ") Salesman, ";
    qStr += Work_Order_Main_Reference + " Reference, ";
    qStr += " min("+Work_Order_Main_VoucherStatus +") as Status, count(*) as '' ";
    qStr += " FROM " + Work_Order_Main_TableName + " where "+Work_Order_Main_VoucherStatus +" in (110, 120, 130) ";
    qStr += " group by " + Work_Order_Main_Requirement_Voucher_No ;

    return DatabaseHelper::getQueryModel(qStr);

}


