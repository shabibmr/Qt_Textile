#include "stockrequestdatabasehelper.h"

StockRequestDatabaseHelper::StockRequestDatabaseHelper( QString connString )
{
    db = new DatabaseHelper( connString);
    itemTransaction = new ItemTransactionDatabaseHelper(db);
}

StockRequestDatabaseHelper::~StockRequestDatabaseHelper()
{

}

int StockRequestDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

int StockRequestDatabaseHelper::commit()
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

QString StockRequestDatabaseHelper::getNextVoucherNo(QString voucherPrefix)
{
    GeneralVoucherDatabaseHelper vHelper;
    return vHelper.getNextVoucherNoByType(VoucherType, voucherPrefix, Stock_Request_Main_TableName);
}

QSqlQueryModel *StockRequestDatabaseHelper::getVoucherListByDateTimeModel(QDateTime dateFrom, QDateTime dateTo)
{

}

QPair<QString, QString> StockRequestDatabaseHelper::getVoucherNoByTransactionId(QString transId)
{
    GeneralVoucherDatabaseHelper vHelper;
    return  vHelper.getVoucherNoByTransactionId(transId, Stock_Request_Main_TableName);

}



QString StockRequestDatabaseHelper::getVoucherNumberByLedID(QString LedID, QDate Date)
{

}



int StockRequestDatabaseHelper::insertVoucher(GeneralVoucherDataObject *obj)
{

    this->startTransaction();

    ItemTransactionDataModel *ItemModel = new ItemTransactionDataModel();
//    ItemTransactionDatabaseHelper *itemTransaction = new ItemTransactionDatabaseHelper();

    itemTransaction->deleteItemTransaction(obj);


    GeneralVoucherDatabaseHelper *vHelper = new GeneralVoucherDatabaseHelper;

    qDebug()<<"started insert ";
    QMap<QString,QVariant> map;
    obj->voucherNumber = vHelper->setNextVoucherNoByType(VoucherType, obj->VoucherPrefix, Stock_Request_Main_TableName);
    obj->voucherType = VoucherType;

    if(obj->TransactionId.length()==0)
        obj->TransactionId=QString::number(QDateTime::currentDateTime().toTime_t()) + obj->VoucherPrefix + db->clientId;


    map.insert(Stock_Request_Main_Voucher_No, obj->voucherNumber);
    map.insert(Stock_Request_Main_Date,obj->VoucherDate);
    map.insert(Stock_Request_Main_Voucher_Prefix, obj->VoucherPrefix);
    map.insert(Stock_Request_Main_Kitchen_Area,obj->reference);
    map.insert(Stock_Request_Main_Added_By,obj->AddedById);
    map.insert(Stock_Request_Main_Narration,obj->narration);
    map.insert(Stock_Request_Main_TimeStampC,obj->timestamp);
    map.insert(Stock_Request_Main_Last_Updated,QDateTime::currentDateTime());
    map.insert(Stock_Request_Main_FromLocation, obj->fromGodownID);
    map.insert(Stock_Request_Main_ToLocation, obj->toGodownID);
    map.insert(Stock_Request_Main_VoucherStatus, obj->status);
    map.insert(Stock_Request_Main_TransactionId, obj->TransactionId);
    map.insert(Stock_Request_Main_Salesman_ID,obj->SalesmanID);


    qint64 id = db->insertRecordsPlus(map,Stock_Request_Main_TableName);
    //    qDebug()<<"Main insered ";
    //    obj->voucherNumber=QString::number(id);
    //    QMap<QString,QVariant> idmap;
    //    idmap.insert(Stock_Request_Main_Voucher_No,obj->voucherNumber);
    //    QMap<QString,QVariant> idmapClause;
    //    idmapClause.insert(Stock_Request_Main_id,id);
    //    db->updateRecordsPlus(idmap,idmapClause,Stock_Request_Main_TableName);


    QMap<QString,QVariant> detailMap;
    qDebug()<<"Items Size = "<<obj->InventoryItems.size();
    for(int i = 0;i< obj->InventoryItems.size();i++){
        detailMap.insert(Stock_Request_Details_Voucher_No,obj->voucherNumber);
        detailMap.insert(Stock_Request_Details_Voucher_Prefix, obj->VoucherPrefix);
        detailMap.insert(Stock_Request_Details_WorkOrderId , obj->RequirementVoucherNo);
        detailMap.insert(Stock_Request_Details_Item_ID,obj->InventoryItems[i].BaseItem.ItemID);
        detailMap.insert(Stock_Request_Details_UOM_id,obj->InventoryItems[i].BaseItem.uomObject._id);
        detailMap.insert(Stock_Request_Details_Calculated_Qty,obj->InventoryItems[i].BaseItem.calculatedQty);
        detailMap.insert(Stock_Request_Details_Request_Qty,obj->InventoryItems[i].BaseItem.requestQty);
        detailMap.insert(Stock_Request_Details_Issue_Qty,obj->InventoryItems[i].BaseItem.quantity);
        detailMap.insert(Stock_Request_Details_TimeStamp, QDateTime::currentDateTime());
        if(obj->InventoryItems[i].BaseItem.ItemReqUuid == "X")
            obj->InventoryItems[i].BaseItem.ItemReqUuid = db->getUUID();
        detailMap.insert(Stock_Request_Details_ItemReqId,obj->InventoryItems[i].BaseItem.ItemReqUuid);

        db->insertRecordsPlus(detailMap,Stock_Request_Details_Table_Name);

        if(obj->InventoryItems[i].BaseItem.quantity > 0){

            ItemModel->ItemID = obj->InventoryItems[i].BaseItem.ItemID;
            ItemModel->From_Ledger = obj->InventoryItems[i].BaseItem.defaultSalesLedgerID;
            ItemModel->To_Ledger = obj->ledgerObject.LedgerID;
            ItemModel->VoucherID = obj->voucherNumber;
            ItemModel->VoucherPrefix = obj->VoucherPrefix;
            ItemModel->VoucherDate = obj->VoucherDate;
            ItemModel->VoucherType = obj->voucherType;
            ItemModel->From_Location = obj->fromGodownID;
            ItemModel->To_Location = obj->toGodownID;
            ItemModel->crQty = obj->InventoryItems[i].BaseItem.quantity;
            ItemModel->drQty = obj->InventoryItems[i].BaseItem.quantity;
            ItemModel->crAmount = obj->InventoryItems[i].BaseItem.subTotal;
            ItemModel->itemReqId = obj->InventoryItems[i].BaseItem.ItemReqUuid;
            ItemModel->Narration = obj->narration;

            qDebug()<<"insert item trans";
            itemTransaction->insertItemTransaction(ItemModel, obj->InventoryItems[i].BaseItem.godownList);

        }
//            ItemTransactionDataModel *consumption = new ItemTransactionDataModel();

//            CompoundItemDataObject item2 = obj->InventoryItems[i];

//            consumption->ItemID = item2.BaseItem.ItemID;
//            consumption->From_Ledger = item2.BaseItem.defaultSalesLedgerID;
//            consumption->To_Ledger = obj->ledgerObject.LedgerID;
//            consumption->VoucherID = obj->voucherNumber;
//            consumption->VoucherPrefix = obj->VoucherPrefix;
//            consumption->VoucherDate = obj->VoucherDate;
//            consumption->VoucherType = "CONSUMPTION";
//            consumption->From_Location = obj->toGodownName;
//            consumption->To_Location = "OUT";
//            consumption->drQty = 0;
//            consumption->crQty = item2.BaseItem.quantity;
//            consumption->crAmount = item2.BaseItem.subTotal;
//            consumption->itemReqId = item2.BaseItem.ItemReqUuid;
//            consumption->Narration = obj->narration;

//            itemTransaction->insertItemTransaction(consumption, obj->InventoryItems[i].BaseItem.fifoList);



    }
    qDebug()<<"Completd";
    return commit();
}

int StockRequestDatabaseHelper::updateVoucher(GeneralVoucherDataObject *obj, QString id)
{
    this->startTransaction();
    obj->voucherType = VoucherType;

    ItemTransactionDataModel *ItemModel = new ItemTransactionDataModel();
//    ItemTransactionDatabaseHelper *itemTransaction = new ItemTransactionDatabaseHelper();

    qDebug()<<"call delete item trans &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&7";
    itemTransaction->deleteItemTransaction(obj);

    QMap<QString,QVariant> delmap;
    QMap<QString,QVariant> deldetailmap;
    delmap.insert(Stock_Request_Main_Voucher_No,id);
    deldetailmap.insert(Stock_Request_Main_Voucher_No,id);

    db->deleteRecordsPlus(deldetailmap,Stock_Request_Details_Table_Name);

    QMap<QString,QVariant> map;
    map.insert(Stock_Request_Main_Date,obj->VoucherDate);
    map.insert(Stock_Request_Main_Kitchen_Area,obj->reference);
    map.insert(Stock_Request_Main_Added_By,obj->AddedById);
    map.insert(Stock_Request_Main_Narration,obj->narration);
    map.insert(Stock_Request_Main_Last_Updated,QDateTime::currentDateTime());
    map.insert(Stock_Request_Main_FromLocation, obj->fromGodownID);
    map.insert(Stock_Request_Main_ToLocation, obj->toGodownID);
    map.insert(Stock_Request_Main_VoucherStatus, obj->status);
    map.insert(Stock_Request_Main_TransactionId, obj->TransactionId);
    map.insert(Stock_Request_Main_Salesman_ID,obj->SalesmanID);

    db->updateRecordsPlus(map,delmap,Stock_Request_Main_TableName);


    QMap<QString,QVariant> detailMap;

    for(int i = 0;i< obj->InventoryItems.size();i++){
        detailMap.insert(Stock_Request_Details_Voucher_No,obj->voucherNumber);
        detailMap.insert(Stock_Request_Details_Voucher_Prefix, obj->VoucherPrefix);
        detailMap.insert(Stock_Request_Details_WorkOrderId , obj->RequirementVoucherNo);
        detailMap.insert(Stock_Request_Details_Item_ID,obj->InventoryItems[i].BaseItem.ItemID);
        detailMap.insert(Stock_Request_Details_UOM_id,obj->InventoryItems[i].BaseItem.uomObject._id);
        detailMap.insert(Stock_Request_Details_Calculated_Qty,obj->InventoryItems[i].BaseItem.calculatedQty);
        detailMap.insert(Stock_Request_Details_Request_Qty,obj->InventoryItems[i].BaseItem.requestQty);
        detailMap.insert(Stock_Request_Details_Issue_Qty,obj->InventoryItems[i].BaseItem.quantity);
        detailMap.insert(Stock_Request_Details_TimeStamp, QDateTime::currentDateTime());
        if(obj->InventoryItems[i].BaseItem.ItemReqUuid == "X")
            obj->InventoryItems[i].BaseItem.ItemReqUuid = db->getUUID();
        detailMap.insert(Stock_Request_Details_ItemReqId,obj->InventoryItems[i].BaseItem.ItemReqUuid);

        db->insertRecordsPlus(detailMap,Stock_Request_Details_Table_Name);

        if(obj->InventoryItems[i].BaseItem.quantity > 0){

            ItemModel->ItemID = obj->InventoryItems[i].BaseItem.ItemID;
            ItemModel->From_Ledger = obj->InventoryItems[i].BaseItem.defaultSalesLedgerID;
            ItemModel->To_Ledger = obj->ledgerObject.LedgerID;
            ItemModel->VoucherID = obj->voucherNumber;
            ItemModel->VoucherPrefix = obj->VoucherPrefix;
            ItemModel->VoucherDate = obj->VoucherDate;
            ItemModel->VoucherType = obj->voucherType;
            ItemModel->From_Location = obj->fromGodownID;
            ItemModel->To_Location = obj->toGodownID;
            ItemModel->crQty = obj->InventoryItems[i].BaseItem.quantity;
            ItemModel->drQty = obj->InventoryItems[i].BaseItem.quantity;
            ItemModel->crAmount = obj->InventoryItems[i].BaseItem.subTotal;
            ItemModel->itemReqId = obj->InventoryItems[i].BaseItem.ItemReqUuid;
            ItemModel->Narration = obj->narration;

            qDebug()<<"insert item trans";
            itemTransaction->insertItemTransaction(ItemModel, obj->InventoryItems[i].BaseItem.godownList);

        }

    }
    return commit();
}

int StockRequestDatabaseHelper::deleteVoucher(GeneralVoucherDataObject *obj)
{
    this->startTransaction();
    QMap<QString,QVariant> delmap;
    QMap<QString,QVariant> deldetailmap;
    delmap.insert(Stock_Request_Main_Voucher_No,obj->voucherNumber);
    delmap.insert(Stock_Request_Main_Voucher_Prefix,obj->VoucherPrefix);
    deldetailmap.insert(Stock_Request_Main_Voucher_No,obj->voucherNumber);
    deldetailmap.insert(Stock_Request_Main_Voucher_Prefix,obj->VoucherPrefix);
    db->deleteRecordsPlus(delmap,Stock_Request_Main_TableName);
    db->deleteRecordsPlus(deldetailmap,Stock_Request_Details_Table_Name);

    return commit();
}

GeneralVoucherDataObject *StockRequestDatabaseHelper::getVoucherByVoucherNoPtr(QString vNo, QString voucherPrefix)
{
    GeneralVoucherDataObject *obj = new GeneralVoucherDataObject;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStrMain;
    QString qStrDetail;
    SalesInventoryItemDatabaseHelper sales;

    qStrMain += "SELECT ";
    qStrMain += Stock_Request_Main_Voucher_No +", ";
    qStrMain += Stock_Request_Main_Date +", ";
    qStrMain += Stock_Request_Main_Kitchen_Area +", ";
    qStrMain += Stock_Request_Main_Narration +", ";
    qStrMain += Stock_Request_Main_Added_By + ", " ;
    qStrMain += Stock_Request_Main_FromLocation +", ";
    qStrMain += Stock_Request_Main_ToLocation + ", " ;
    qStrMain += Stock_Request_Main_VoucherStatus + ", ";
    qStrMain += Stock_Request_Main_TransactionId + ", ";
    qStrMain += Stock_Request_Main_Salesman_ID;

    qStrMain += " FROM "+ Stock_Request_Main_TableName;
    qStrMain += " WHERE "+ Stock_Request_Main_Voucher_No + "='"+vNo+"'";
    qStrMain += " AND " + Stock_Request_Main_Voucher_Prefix + " = '"+voucherPrefix + "'";

    qStrDetail += "SELECT ";
    qStrDetail += Stock_Request_Details_WorkOrderId + ", ";
    qStrDetail += Stock_Request_Details_Item_ID +", ";
    qStrDetail += Stock_Request_Details_ItemReqId + ", ";
    qStrDetail += Stock_Request_Details_UOM_id +", ";
    qStrDetail += Stock_Request_Details_Request_Qty +", ";
    qStrDetail += Stock_Request_Details_Issue_Qty + ", ";
    qStrDetail += Stock_Request_Details_Calculated_Qty ;
    qStrDetail += " FROM "+ Stock_Request_Details_Table_Name;
    qStrDetail += " WHERE "+ Stock_Request_Details_Voucher_No + "='"+vNo+"'";
    qStrDetail += " AND " + Stock_Request_Details_Voucher_Prefix + " = '"+voucherPrefix + "'";

    if(query.exec(qStrMain)){ 
        int i=0;
        while(query.next()){
            obj->voucherNumber = query.value(i++).toString();
            obj->VoucherDate   = query.value(i++).toDate();
            obj->voucherType = VoucherType;
            obj->reference     = query.value(i++).toString();
            obj->narration     = query.value(i++).toString();
            obj->AddedById     = query.value(i++).toInt();
            obj->fromGodownID = query.value(i++).toString();
            obj->toGodownID = query.value(i++).toString();
            obj->status = query.value(i++).toInt();
            obj->TransactionId = query.value(i++).toString();
            obj->SalesmanID =  query.value(i++).toInt();
        }
    }
    else{
        qDebug()<<query.lastQuery();
        qDebug()<<query.lastError();
    }


    if(query.exec(qStrDetail)){
        int i =0;
        while(query.next()){
            int i =0;

            CompoundItemDataObject item;
            obj->RequirementVoucherNo = query.value(i++).toString();
            item.BaseItem = sales.getInventoryItemByID(query.value(i++).toString());
            item.BaseItem.ItemReqUuid = query.value(i++).toString();
            item.BaseItem.uomObject._id = query.value(i++).toString();
            item.BaseItem.requestQty = query.value(i++).toFloat();
            item.BaseItem.quantity = query.value(i++).toFloat();
            item.BaseItem.calculatedQty = query.value(i++).toFloat();
            item.BaseItem.prevQty = item.BaseItem.quantity;
            item.BaseItem.consumedQty = itemTransaction->getConsumedQtyByVoucher(item.BaseItem.ItemID, vNo,
                                                                            VoucherType, voucherPrefix, item.BaseItem.ItemReqUuid);

//            item.BaseItem.fifoList = itemTransaction->getFifoListByVoucherItem(obj->voucherNumber, obj->voucherType,
//                                                                               obj->VoucherPrefix, item.BaseItem.ItemID);

//            qDebug()<<"Qty"<<item.BaseItem.quantity<<item.BaseItem.prevQty;
            obj->InventoryItems.append(item);
        }
    }
    else{
        qDebug()<<query.lastQuery();
        qDebug()<<query.lastError();
    }


    return obj;
}

GeneralVoucherDataObject *StockRequestDatabaseHelper::getVoucherByWorkOrderId(QString WoNo)
{
    GeneralVoucherDataObject *obj = new GeneralVoucherDataObject;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStrMain;
    QString qStrDetail;
    SalesInventoryItemDatabaseHelper sales;

    qStrDetail += "SELECT ";
    qStrDetail += Stock_Request_Details_Voucher_No + ", ";
    qStrDetail += Stock_Request_Details_Voucher_Prefix + ", ";
    qStrDetail += Stock_Request_Details_Item_ID +", ";
    qStrDetail += Stock_Request_Details_ItemReqId + ", ";
    qStrDetail += Stock_Request_Details_UOM_id +", ";
    qStrDetail += Stock_Request_Details_Issue_Qty;
    qStrDetail += " FROM "+ Stock_Request_Details_Table_Name;
    qStrDetail += " WHERE "+ Stock_Request_Details_WorkOrderId + "='"+WoNo+"'";

    query.prepare(qStrDetail);
    if(query.exec(qStrDetail)){
        int i =0;
        while(query.next()){
            int i =0;

            CompoundItemDataObject item;
            obj->voucherNumber = query.value(i++).toString();
            obj->VoucherPrefix = query.value(i++).toString();
            obj->voucherType = VoucherType;
            item.BaseItem = sales.getInventoryItemByID(query.value(i++).toString());
            item.BaseItem.ItemReqUuid = query.value(i++).toString();
            item.BaseItem.uomObject._id = query.value(i++).toString();
            item.BaseItem.quantity = query.value(i++).toFloat();
            item.BaseItem.crQty = item.BaseItem.quantity;
            obj->InventoryItems.append(item);
//            qDebug()<<"item"<<item.BaseItem.ItemID<<item.BaseItem.ItemName;
        }
    }
    else{
        qDebug()<<query.lastQuery();
        qDebug()<<query.lastError();
    }


    qStrMain += "SELECT ";
    qStrMain += Stock_Request_Main_Voucher_No +", ";
    qStrMain += Stock_Request_Main_Date +", ";
    qStrMain += Stock_Request_Main_Kitchen_Area +", ";
    qStrMain += Stock_Request_Main_Narration +", ";
    qStrMain += Stock_Request_Main_Added_By + ", " ;
    qStrMain += Stock_Request_Main_FromLocation +", ";
    qStrMain += Stock_Request_Main_ToLocation  + ", ";
    qStrMain += Stock_Request_Main_TransactionId;

    qStrMain += " FROM "+ Stock_Request_Main_TableName;
    qStrMain += " WHERE "+ Stock_Request_Main_Voucher_No + " = '" + obj->voucherNumber + "'";

    query.prepare(qStrMain);
    //    query.bindValue(":vNo", obj->voucherNumber);


    if(query.exec(qStrMain)){
        int i=0;
        while(query.next()){
            obj->voucherNumber = query.value(i++).toString();
            obj->VoucherDate   = query.value(i++).toDate();
            obj->reference     = query.value(i++).toString();
            obj->narration     = query.value(i++).toString();
            obj->AddedById     = query.value(i++).toInt();
            obj->fromGodownID = query.value(i++).toString();
            obj->toGodownID = query.value(i++).toString();
            obj->TransactionId = query.value(i++).toString();
        }
    }
    else{
        qDebug()<<query.lastQuery();
        qDebug()<<query.lastError();
        qDebug()<<query.boundValues();
    }


    return obj;
}

QSqlQueryModel *StockRequestDatabaseHelper::getVoucherListByDateModel(QDate fromDate, QDate toDate)
{
    QString qStr;

    qStr += "SELECT " + Stock_Request_Main_Date +", ";
    qStr += Stock_Request_Main_Voucher_No+" Voucher"+", ";
    qStr += Stock_Request_Main_Voucher_Prefix +", ";
    qStr += " (select godown_name from " +GodownDatabaseHelper::Godown_Tablename +" where godown_id = ";

    qStr += Stock_Request_Main_ToLocation+" ) Section"+", ";

    qStr += Stock_Request_Main_Narration+" from ";
    qStr += Stock_Request_Main_TableName+ " where ";
    qStr += Stock_Request_Main_Date+ ">='"+fromDate.toString("yyyy-MM-dd")+"'";
    qStr += " and ";
    qStr += Stock_Request_Main_Date+ "<='"+toDate.toString("yyyy-MM-dd")+"'";

    return DatabaseHelper::getQueryModel(qStr);

}

int StockRequestDatabaseHelper::getMaxID()
{
    QString qStr = "SELECT MAX("+Stock_Request_Main_id+") FROM " + Stock_Request_Main_TableName;

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

