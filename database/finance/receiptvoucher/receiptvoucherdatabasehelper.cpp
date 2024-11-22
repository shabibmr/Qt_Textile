#include "receiptvoucherdatabasehelper.h"

ReceiptVoucherDatabaseHelper::ReceiptVoucherDatabaseHelper()
{
    db = new DatabaseHelper();
    ledTransaction = new TransactionHelper();
}

ReceiptVoucherDatabaseHelper::~ReceiptVoucherDatabaseHelper()
{

}

int ReceiptVoucherDatabaseHelper::startTransaction()
{
    db->startTransaction();

}

QString ReceiptVoucherDatabaseHelper::getNextVoucherNo(QString voucherPrefix)
{
    GeneralVoucherDatabaseHelper vHelper;
    QString voucherNumber = vHelper.getNextVoucherNoByType(VoucherType, voucherPrefix, Receipt_Voucher_Main_TableName);
    return voucherNumber;

}

QPair<QString, QString> ReceiptVoucherDatabaseHelper::getVoucherNoByTransactionId(QString transId)
{
    GeneralVoucherDatabaseHelper vHelper;
    return vHelper.getVoucherNoByTransactionId(transId, Receipt_Voucher_Main_TableName);


}

QString ReceiptVoucherDatabaseHelper::getVoucherNumberByLedID(QString LedID, QDate Date)
{

}

QSqlQueryModel *ReceiptVoucherDatabaseHelper::getVoucherListByDateModel(QDate dateFrom, QDate dateTo)
{

}

QSqlQueryModel *ReceiptVoucherDatabaseHelper::getVoucherListByDateTimeModel(QDateTime dateFrom, QDateTime dateTo)
{

    LedgerMasterDatabaseHelper ledHelper;
    QString qStr ="SELECT ";
    qStr += "'V' , ";
    qStr += Receipt_Voucher_Main_Voucher_No +", ";
    qStr += "'" + VoucherType + "', " ;
    qStr += Receipt_Voucher_Main_Voucher_Prefix + ", ";
    qStr += Receipt_Voucher_Main_Voucher_Date+" as 'Voucher Date', ";
    qStr += " CONCAT(" + Receipt_Voucher_Main_Voucher_Prefix + ", '-', " + Receipt_Voucher_Main_Voucher_No+ ") as 'Voucher No', ";
    qStr += "(SELECT "+ledHelper.Ledger_Master_LedgerName + " FROM "+ ledHelper.Ledger_Master_TableName;
    qStr += " WHERE "+ledHelper.Ledger_Master_LedgerID + "= SA.";
    qStr += Receipt_Voucher_Main_Ledger_ID + ") Ledger, ";
    qStr += Receipt_Voucher_Main_Total;
    qStr += " FROM " + Receipt_Voucher_Main_TableName;
    qStr += " as SA WHERE ";
    qStr += Receipt_Voucher_Main_Voucher_Date;
    qStr += " >= '";
    qStr += dateFrom.toString("yyyy-MM-dd");
    qStr += "' AND ";
    qStr += Receipt_Voucher_Main_Voucher_Date;
    qStr += " <= '";
    qStr += dateTo.toString("yyyy-MM-dd");
    qStr += "'";

    qDebug()<<qStr;

    return DatabaseHelper::getQueryModel(qStr);

}

int ReceiptVoucherDatabaseHelper::insertVoucher(GeneralVoucherDataObject *obj)
{
    this->startTransaction();
    QMap<QString,QVariant> map;
    obj->voucherType = VoucherType;

    GeneralVoucherDatabaseHelper vHelper;
    obj->voucherNumber = vHelper.setNextVoucherNoByType(VoucherType, obj->VoucherPrefix, Receipt_Voucher_Main_TableName );

    if(obj->TransactionId.length()==0)
        obj->TransactionId=QString::number(QDateTime::currentDateTime().toTime_t()) + obj->VoucherPrefix + db->clientId;

    map.insert(Receipt_Voucher_Main_Voucher_No,obj->voucherNumber);
    map.insert(Receipt_Voucher_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Receipt_Voucher_Main_Date_Created,obj->VoucherDate);
    map.insert(Receipt_Voucher_Main_Voucher_Date,obj->VoucherDate);
    map.insert(Receipt_Voucher_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    map.insert(Receipt_Voucher_Main_Billing_Name,obj->ledgerObject.LedgerName);
    map.insert(Receipt_Voucher_Main_Total,obj->grandTotal);
    map.insert(Receipt_Voucher_Main_Narration,obj->narration);
    map.insert(Receipt_Voucher_Main_Project_ID,obj->ProjectId);
    map.insert(Receipt_Voucher_Main_Revision_Number,obj->RevisionNo);
    map.insert(Receipt_Voucher_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Receipt_Voucher_Main_GeneratedTransactionId, obj->TransactionId);
    map.insert(Receipt_Voucher_Main_TimeStampC,obj->timestamp);
    map.insert(Receipt_Voucher_Main_Last_Updated,QDateTime::currentDateTime());
    map.insert(Receipt_Voucher_Main_POC_Name,obj->Contact.ContactName);
    map.insert(Receipt_Voucher_Main_POC_Phone,obj->Contact.PhoneNumber);
    map.insert(Receipt_Voucher_Main_POC_Email,obj->Contact.email);

    QJsonDocument doc(obj->ReqVoucherList);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    map.insert(Receipt_Voucher_Main_ReqVoucherList, strJson);

    qint64 lastId= db->insertRecordsPlus(map,Receipt_Voucher_Main_TableName);

    qDebug()<<"New ID = "<<lastId;

    //   QMap<QString,QVariant> idmap;
    //   idmap.insert(Receipt_Voucher_Main_Voucher_No,QString::number(lastId));
    //   QMap<QString,QVariant> idmapClause;
    //   idmapClause.insert(Receipt_Voucher_Main_id,lastId);
    //   db->updateRecordsPlus(idmap,idmapClause,Receipt_Voucher_Main_TableName);
    //   qDebug()<<"New ID updated = "<<lastId;
    //   obj->voucherNumber = QString::number(lastId);



    for(LedgerMasterDataModel item:obj->ledgersList){
        QMap<QString,QVariant> map1;

        map1.insert(Receipt_Voucher_Details_Voucher_No,obj->voucherNumber);
        map1.insert(Receipt_Voucher_Details_Voucher_Prefix,obj->VoucherPrefix);
        map1.insert(Receipt_Voucher_Details_Voucher_Date,obj->VoucherDate);
        map1.insert(Receipt_Voucher_Details_Ledger_ID,item.LedgerID);
        map1.insert(Receipt_Voucher_Details_CrAmount,item.amount);
        map1.insert(Receipt_Voucher_Details_Narration,item.narration);
        map1.insert(Receipt_Voucher_Details_Salesman_ID,obj->SalesmanID);

        int basid = db->insertRecordsPlus(map1,Receipt_Voucher_Details_TableName);
    }

    for(LedgerMasterDataModel ledger:obj->ledgersList){
        QMap <QString,QVariant> ledMap;
        ledMap.insert(Transaction_Helper_VoucherType,VoucherType);
        ledMap.insert(Transaction_Helper_VoucherNo,obj->voucherNumber);
        ledMap.insert(Transaction_Helper_VoucherPrefix,obj->VoucherPrefix);
        ledMap.insert(Transaction_Helper_VoucherDate,obj->VoucherDate);
        ledMap.insert(Transaction_Helper_ProjectID,obj->ProjectId);
        ledMap.insert(Transaction_Helper_SalesManID,obj->SalesmanID);
        ledMap.insert(Transaction_Helper_TranactionAmount,ledger.amount);
        ledMap.insert(Transaction_Helper_DebitLedgerID,ledger.LedgerID);
        ledMap.insert(Transaction_Helper_CreditLedgerID,obj->ledgerObject.LedgerID);
        ledMap.insert(Transaction_Helper_Narration,obj->ledgerObject.narration);
        //        db->insertRecordsPlus(ledMap,Transaction_Helper_TableName);
    }
    LedgerMasterDataModel MainLedger;

    MainLedger.drAmount = obj->grandTotal;
    MainLedger.LedgerID = obj->ledgerObject.LedgerID;
    MainLedger.isInvoiceItem = true;
    MainLedger.AgainstLedger = obj->ledgersList[0].LedgerID;
    obj->ledgersList.insert(0,MainLedger);

    for(int i =0;i<obj->ledgersList.size();i++){
        if(obj->ledgersList[i].crAmount>0)
            obj->ledgersList[i].AgainstLedger = MainLedger.LedgerID;
//        else
//            obj->ledgersList[i].AgainstLedger = obj->ledgersList[0].LedgerID;
    }

    ledTransaction->insertTransaction(obj);

//    qDebug()<<obj->ObjToJson(obj);

    //    return commit();
    int c=commit();

    if(obj->ledgerObject.dbName != "" && !obj->fromExternal && obj->sendFlag){
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

int ReceiptVoucherDatabaseHelper::updateVoucher(GeneralVoucherDataObject *obj, QString oldID)
{
    this->startTransaction();

    obj->voucherType = VoucherType;

    QMap<QString,QVariant> map5;
    map5.insert(Receipt_Voucher_Details_Voucher_No,oldID);
    map5.insert(Receipt_Voucher_Details_Voucher_Prefix,obj->VoucherPrefix);
    db->deleteRecordsPlus(map5,Receipt_Voucher_Details_TableName);

    //    QMap<QString,QVariant> maptr;
    //    maptr.insert(Transaction_Helper_VoucherNo,oldID);
    //    maptr.insert(Transaction_Helper_VoucherType,VoucherType);
    //    db->deleteRecordsPlus(maptr,Transaction_Helper_TableName);

    QMap<QString,QVariant> map;
    map.insert(Receipt_Voucher_Main_Voucher_No,obj->voucherNumber);
    map.insert(Receipt_Voucher_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Receipt_Voucher_Main_Date_Created,obj->VoucherDate);
    map.insert(Receipt_Voucher_Main_Voucher_Date,obj->VoucherDate);
    qDebug()<<"insert"<<obj->ledgerObject.LedgerID;
    map.insert(Receipt_Voucher_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    map.insert(Receipt_Voucher_Main_Billing_Name,obj->ledgerObject.LedgerName);
    map.insert(Receipt_Voucher_Main_Total,obj->grandTotal);
    map.insert(Receipt_Voucher_Main_Narration,obj->narration);
    map.insert(Receipt_Voucher_Main_Project_ID,obj->ProjectId);
    map.insert(Receipt_Voucher_Main_Revision_Number,obj->RevisionNo);
    map.insert(Receipt_Voucher_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Receipt_Voucher_Main_GeneratedTransactionId, obj->TransactionId);
    map.insert(Receipt_Voucher_Main_Last_Updated,QDateTime::currentDateTime());
    map.insert(Receipt_Voucher_Main_POC_Name,obj->Contact.ContactName);
    map.insert(Receipt_Voucher_Main_POC_Phone,obj->Contact.PhoneNumber);
    map.insert(Receipt_Voucher_Main_POC_Email,obj->Contact.email);

    QJsonDocument doc(obj->ReqVoucherList);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    map.insert(Receipt_Voucher_Main_ReqVoucherList, strJson);


    QMap<QString,QVariant> map3;
    map3.insert(Receipt_Voucher_Main_Voucher_No,oldID);
    map3.insert(Receipt_Voucher_Main_Voucher_Prefix,obj->VoucherPrefix);
    db->updateRecordsPlus(map,map3,Receipt_Voucher_Main_TableName);

    for(LedgerMasterDataModel item:obj->ledgersList){
        QMap<QString,QVariant> map1;

        map1.insert(Receipt_Voucher_Details_Voucher_No,obj->voucherNumber);
        map1.insert(Receipt_Voucher_Details_Voucher_Prefix,obj->VoucherPrefix);
        map1.insert(Receipt_Voucher_Details_Voucher_Date,obj->VoucherDate);
        map1.insert(Receipt_Voucher_Details_Ledger_ID,item.LedgerID);
        map1.insert(Receipt_Voucher_Details_CrAmount,item.amount);
        map1.insert(Receipt_Voucher_Details_Narration,item.narration);
        map1.insert(Receipt_Voucher_Details_Salesman_ID,obj->SalesmanID);

        int basid = db->insertRecordsPlus(map1,Receipt_Voucher_Details_TableName);
    }

    for(LedgerMasterDataModel ledger:obj->ledgersList){
        QMap <QString,QVariant> ledMap;
        ledMap.insert(Transaction_Helper_VoucherType,VoucherType);
        ledMap.insert(Transaction_Helper_VoucherNo,obj->voucherNumber);
        ledMap.insert(Transaction_Helper_VoucherPrefix,obj->VoucherPrefix);
        ledMap.insert(Transaction_Helper_VoucherDate,obj->VoucherDate);
        ledMap.insert(Transaction_Helper_ProjectID,obj->ProjectId);
        ledMap.insert(Transaction_Helper_SalesManID,obj->SalesmanID);
        ledMap.insert(Transaction_Helper_TranactionAmount,ledger.amount);
        ledMap.insert(Transaction_Helper_CreditLedgerID,ledger.LedgerID);
        ledMap.insert(Transaction_Helper_DebitLedgerID,obj->ledgerObject.LedgerID);
        ledMap.insert(Transaction_Helper_Narration,obj->ledgerObject.narration);
        //        db->insertRecordsPlus(ledMap,Transaction_Helper_TableName);
    }
    LedgerMasterDataModel MainLedger;

    MainLedger.drAmount = obj->grandTotal;
    MainLedger.LedgerID = obj->ledgerObject.LedgerID;
    MainLedger.isInvoiceItem = true;
    MainLedger.AgainstLedger = obj->ledgersList[0].LedgerID;
    qDebug()<<"main ledger is "<<MainLedger.LedgerID<<MainLedger.AgainstLedger<<obj->ledgersList[0].LedgerID;
    obj->ledgersList.insert(0,MainLedger);


    for(int i =0;i<obj->ledgersList.size();i++){
        if(obj->ledgersList[i].crAmount>0)
            obj->ledgersList[i].AgainstLedger = MainLedger.LedgerID;
//        else
//            obj->ledgersList[i].AgainstLedger = obj->ledgersList[0].LedgerID;
    }

    ledTransaction->deleteTransaction(obj);
    ledTransaction->insertTransaction(obj);

    qDebug()<<"insert"<<obj->ledgerObject.LedgerID;
    //    return commit();
    int c=commit();

    if(obj->ledgerObject.dbName != "" && !obj->fromExternal && obj->sendFlag){
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

int ReceiptVoucherDatabaseHelper::deleteVoucher(GeneralVoucherDataObject *obj)
{
    this->startTransaction();
    QMap<QString,QVariant> map3;
    map3.insert(Receipt_Voucher_Main_Voucher_No,obj->voucherNumber);
    map3.insert(Receipt_Voucher_Main_Voucher_Prefix,obj->VoucherPrefix);
    db->deleteRecordsPlus(map3,Receipt_Voucher_Main_TableName);

    QMap<QString,QVariant> map4;
    map4.insert(Receipt_Voucher_Details_Voucher_No,obj->voucherNumber);
    map4.insert(Receipt_Voucher_Details_Voucher_Prefix,obj->VoucherPrefix);
    db->deleteRecordsPlus(map4,Receipt_Voucher_Details_TableName);

    QMap<QString,QVariant> maptr;
    maptr.insert(Transaction_Helper_VoucherNo,obj->voucherNumber);
    maptr.insert(Transaction_Helper_VoucherType,VoucherType);
    maptr.insert(Transaction_Helper_VoucherPrefix,obj->VoucherPrefix);
    db->deleteRecordsPlus(maptr,Transaction_Helper_TableName);

    return commit();
}

int ReceiptVoucherDatabaseHelper::commit()
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

int ReceiptVoucherDatabaseHelper::getMaxID()
{
    QString qStr = "SELECT MAX("+Receipt_Voucher_Main_id+") FROM " + Receipt_Voucher_Main_TableName;
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

GeneralVoucherDataObject ReceiptVoucherDatabaseHelper::getVoucherByVoucherNo(QString vNo)
{

    GeneralVoucherDataObject obj;
    SalesInventoryItemDatabaseHelper *itemHelper = new SalesInventoryItemDatabaseHelper();
    LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper();
    BillwiseRecordsDatabaseHelper *billHelper = new BillwiseRecordsDatabaseHelper;

    QString qStrLedgers ="SELECT ";
    qStrLedgers += Receipt_Voucher_Details_Ledger_ID +", ";
    qStrLedgers += Receipt_Voucher_Details_CrAmount;
    qStrLedgers += " FROM " + Receipt_Voucher_Details_TableName;
    qStrLedgers += " WHERE " + Receipt_Voucher_Details_Voucher_No + "=:id";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    QString qStr ="SELECT "
            + Receipt_Voucher_Main_Voucher_No+ ", "
            + Receipt_Voucher_Main_Voucher_Prefix+ ", "
            + Receipt_Voucher_Main_Voucher_Date+", "
            + Receipt_Voucher_Main_Ledger_ID
            + " FROM " + Receipt_Voucher_Main_TableName
            + " WHERE " + Receipt_Voucher_Main_Voucher_No + "= :vno";

    query.prepare(qStr);
    query.bindValue(":vno",vNo);

    qDebug()<<qStr<<vNo;


    if(!query.exec()){
        qDebug()<<query.lastQuery();
        qDebug()<<query.lastError();
    }
    else{
        while(query.next()){
            int i=0;
            obj.voucherNumber = query.value(i++).toString();
            obj.VoucherPrefix = query.value(i++).toString();
            obj.VoucherDate = query.value(i++).toDate();
            obj.ledgerObject.LedgerID = query.value(i++).toString();
            obj.ledgerObject.LedgerName = ledHelper->getLedgerNameByID(obj.ledgerObject.LedgerID );
            obj.grandTotal = query.value(i++).toFloat();

            qDebug()<<"led"<<obj.ledgerObject.LedgerID<<obj.ledgerObject.LedgerName;

        }
    }
    query.prepare(qStrLedgers);
    query.bindValue(":id",vNo);
    qDebug()<<"294";
    if(!query.exec()){
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    else{
        int cnt =1;

        while(query.next()){
            LedgerMasterDataModel ledger;
            ledger = ledHelper->getLedgerObjectByID(query.value(0).toString());
            ledger.crAmount = query.value(1).toFloat();
            ledger.amount = query.value(1).toFloat();
            ledger.listid = cnt;
            ledger.mapList = billHelper->getBillMappingsByLedger(ledger.LedgerID, vNo, QDate::currentDate());
            obj.ledgersList.append(ledger);

        }
    }


    return obj;
}

GeneralVoucherDataObject *ReceiptVoucherDatabaseHelper::getVoucherByVoucherNoPtr(QString vNo, QString voucherPrefix)
{
    GeneralVoucherDataObject* obj = new GeneralVoucherDataObject;

    LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper();
    BillwiseRecordsDatabaseHelper *billHelper = new BillwiseRecordsDatabaseHelper;

    QString qStrLedgers ="SELECT ";
    qStrLedgers += Receipt_Voucher_Details_Ledger_ID +", ";
    qStrLedgers += Receipt_Voucher_Details_DrAmount + ", ";
    qStrLedgers += Receipt_Voucher_Details_CrAmount;
    qStrLedgers += " FROM " + Receipt_Voucher_Details_TableName;
    qStrLedgers += " WHERE " + Receipt_Voucher_Details_Voucher_No + "=:id";
    qStrLedgers += " AND " + Receipt_Voucher_Details_Voucher_Prefix + "=:voucherPrefix";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    QString qStrMain ="SELECT "
            + Receipt_Voucher_Main_Voucher_No + ", "
            + Receipt_Voucher_Main_Voucher_Prefix + ", "
            + Receipt_Voucher_Main_Date_Created + ", "
            + Receipt_Voucher_Main_Voucher_Date + ", "
            + Receipt_Voucher_Main_TimeStamp + ", "
            + Receipt_Voucher_Main_Ledger_ID + ", "
            + Receipt_Voucher_Main_Billing_Name + ", "
            + Receipt_Voucher_Main_Total + ", "
            + Receipt_Voucher_Main_Narration + ", "
            + Receipt_Voucher_Main_Added_By + ", "
            + Receipt_Voucher_Main_Delivery_Date + ", "
            + Receipt_Voucher_Main_Completion_Probability + ", "
            + Receipt_Voucher_Main_Credit_Period + ", "
            + Receipt_Voucher_Main_POC_Name + ", "
            + Receipt_Voucher_Main_POC_Phone + ", "
            + Receipt_Voucher_Main_POC_Email + ", "
            + Receipt_Voucher_Main_Project_ID + ", "
            + Receipt_Voucher_Main_Revision_Number + ", "
            + Receipt_Voucher_Main_Converted_To_Sales_Order + ", "
            + Receipt_Voucher_Main_Quotation_Dropped + ", "
            + Receipt_Voucher_Main_Quotation_Dropped_Reason + ", "
            + Receipt_Voucher_Main_Requirement_Voucher_No + ", "
            + Receipt_Voucher_Main_Salesman_ID + ", "
            + Receipt_Voucher_Main_LPO + ", "
            + Receipt_Voucher_Main_VoucherStatus + ", "
            + Receipt_Voucher_Main_ModeOfService + ", "
            + Receipt_Voucher_Main_Discount_In_Amount + ", "
            + Receipt_Voucher_Main_Discount_In_Percent + ", "
            + Receipt_Voucher_Main_VATAmount + ", "
            + Receipt_Voucher_Main_Contact_ID + ", "
            + Receipt_Voucher_Main_Gross_Total + ", "
            + Receipt_Voucher_Main_Reference + ", "
            + Receipt_Voucher_Main_Customer_Expecting_Date + ", "
            + Receipt_Voucher_Main_Currency + ", "
            + Receipt_Voucher_Main_BoxesCount+ ", "
            + Receipt_Voucher_Main_TotalWeight + ", "
            + Receipt_Voucher_Main_LRNo + ", "
            + Receipt_Voucher_Main_VoucherNoDisplay + ", "
            + Receipt_Voucher_Main_ReqVoucherList + ", "
            + Receipt_Voucher_Main_Origin + ", "
            + Receipt_Voucher_Main_GeneratedTransactionId
            + " FROM " + Receipt_Voucher_Main_TableName
            + " WHERE " + Receipt_Voucher_Main_Voucher_No + "= :vno"
            + " AND " +  Receipt_Voucher_Main_Voucher_Prefix + "= :voucherPrefix";

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
            qDebug()<<"ledg"<<obj->ledgerObject.LedgerID<<obj->ledgerObject.LedgerName;
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

        }
    }


    query.prepare(qStrLedgers);
    query.bindValue(":id",vNo);
    query.bindValue(":voucherPrefix", voucherPrefix);

    if(!query.exec()){
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    else{
        int cnt =1;

        while(query.next()){
            LedgerMasterDataModel ledger;
            ledger = ledHelper->getLedgerObjectByID(query.value(0).toString());
            ledger.drAmount = query.value(1).toFloat();
            ledger.crAmount = query.value(2).toFloat();
            ledger.amount = query.value(2).toFloat();
            ledger.listid = cnt;
//            ledger.mapList = billHelper->getBillMappingsByLedger(ledger.LedgerID, vNo, QDate::currentDate());
            obj->ledgersList.append(ledger);
        }
    }


    obj->voucherType = VoucherType;
    return obj;
}

QList<GeneralVoucherDataObject> ReceiptVoucherDatabaseHelper::getVoucherListByDate(QDate dateFrom, QDate dateTo)
{
    QList<GeneralVoucherDataObject> vouchersList;
    QString qStr ="SELECT "
            + Receipt_Voucher_Main_Voucher_No+ ", "
            + Receipt_Voucher_Main_Voucher_Prefix+ ", "
            + Receipt_Voucher_Main_Voucher_Date+", "
            + Receipt_Voucher_Main_Ledger_ID+ ", "
            + Receipt_Voucher_Main_Total
            + " FROM " + Receipt_Voucher_Main_TableName
            + " WHERE "
            + Receipt_Voucher_Main_Voucher_Date
            + " >= '"
            + dateFrom.toString("yyyy-MM-dd")
            + "' AND "
            + Receipt_Voucher_Main_Voucher_Date
            + " <= '"
            + dateTo.toString("yyyy-MM-dd")
            + "'"
            ;
    LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper();
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
            vouchersList.append(item);
        }
    }
    qDebug()<<"Query Size = "<<query.size();

    return vouchersList;
}

QSqlQueryModel *ReceiptVoucherDatabaseHelper::getDetailsList(QDate dateFrom, QDate dateTo)
{
    QString qStr="";

    qStr += "SELECT main."+Receipt_Voucher_Details_Voucher_Date+",";
    qStr += " main." + Receipt_Voucher_Main_Voucher_No + ", ";
    qStr += "( select ledger_name from ledger_master led where led.ledger_id = det."+Receipt_Voucher_Details_Ledger_ID;
    qStr += ") 'Ledger', ";
    qStr += "( select ledger_name from ledger_master led where led.ledger_id = main."+Receipt_Voucher_Main_Ledger_ID;
    qStr += ") 'Mode Of Pay', "  ;
    qStr += Receipt_Voucher_Details_CrAmount + " as Amount, ";
    qStr += " main." + Receipt_Voucher_Main_Narration;
    qStr += " FROM "+ Receipt_Voucher_Main_TableName + " main JOIN " + Receipt_Voucher_Details_TableName;
    qStr += " det ON main." + Receipt_Voucher_Main_Voucher_No + " = det." + Receipt_Voucher_Details_Voucher_No;
    qStr += " AND main." + Receipt_Voucher_Main_Voucher_Prefix + " = det." + Receipt_Voucher_Details_Voucher_Prefix;
    qStr += " where ";
    qStr +=  " main." + Receipt_Voucher_Main_Voucher_Date + ">=" + DatabaseHelper::getDateStringStatic(dateFrom) + " ";
    qStr += " and ";
    qStr +=  " main." + Receipt_Voucher_Main_Voucher_Date + "<= " + DatabaseHelper::getDateStringStatic(dateTo) + " ";
    qStr += " order by 1";

    return DatabaseHelper::getQueryModel(qStr);
}
