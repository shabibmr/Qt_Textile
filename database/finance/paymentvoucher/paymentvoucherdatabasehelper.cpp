#include "paymentvoucherdatabasehelper.h"

PaymentVoucherDatabaseHelper::PaymentVoucherDatabaseHelper(QString connString)
{
    db = new DatabaseHelper(connString);
    ledTransaction = new TransactionHelper(db);
}

PaymentVoucherDatabaseHelper::~PaymentVoucherDatabaseHelper()
{

}

int PaymentVoucherDatabaseHelper::startTransaction()
{
    db->startTransaction();

}

QString PaymentVoucherDatabaseHelper::getNextVoucherNo(QString voucherPrefix)
{
    GeneralVoucherDatabaseHelper vHelper;
    QString voucherNumber = vHelper.getNextVoucherNoByType(VoucherType, voucherPrefix, Payment_Voucher_Main_TableName);
    return voucherNumber;

}

QPair<QString, QString> PaymentVoucherDatabaseHelper::getVoucherNoByTransactionId(QString transId)
{
    GeneralVoucherDatabaseHelper vHelper;
    return vHelper.getVoucherNoByTransactionId(transId, Payment_Voucher_Main_TableName);

}

QSqlQueryModel *PaymentVoucherDatabaseHelper::getVoucherListByDateTimeModel(QDateTime dateFrom, QDateTime dateTo)
{

}

QSqlQueryModel *PaymentVoucherDatabaseHelper::getVoucherListByDateModel(QDate dateFrom, QDate dateTo)
{

}

QString PaymentVoucherDatabaseHelper::getVoucherNumberByLedID(QString LedID, QDate Date)
{

}

QSqlQueryModel *PaymentVoucherDatabaseHelper::getDetailsList(QDate dateFrom, QDate dateTo)
{
    QString qStr="";

    qStr += "SELECT main."+Payment_Voucher_Details_Voucher_Date+",";
    qStr += " main." + Payment_Voucher_Main_Voucher_No + ", ";
    qStr += "( select ledger_name from ledger_master led where led.ledger_id = det."+Payment_Voucher_Details_Ledger_ID;
    qStr += ") 'Ledger', ";
    qStr += "( select ledger_name from ledger_master led where led.ledger_id = main."+Payment_Voucher_Main_Ledger_ID;
    qStr += ") 'Mode Of Pay', "  ;
    qStr += Payment_Voucher_Details_DrAmount + " as Amount, ";
    qStr += " main." + Payment_Voucher_Main_Narration;
    qStr += " FROM "+ Payment_Voucher_Main_TableName + " main JOIN " + Payment_Voucher_Details_TableName;
    qStr += " det ON main." + Payment_Voucher_Main_Voucher_No + " = det." + Payment_Voucher_Details_Voucher_No;
    qStr += " AND main." + Payment_Voucher_Main_Voucher_Prefix + " = det." + Payment_Voucher_Details_Voucher_Prefix;
    qStr += " where ";
    qStr +=  " main." + Payment_Voucher_Main_Voucher_Date + ">=" + DatabaseHelper::getDateStringStatic(dateFrom) + " ";
    qStr += " and ";
    qStr +=  " main." + Payment_Voucher_Main_Voucher_Date + "<= " + DatabaseHelper::getDateStringStatic(dateTo) + " ";
    qStr += " order by 1";

    return DatabaseHelper::getQueryModel(qStr);
}

int PaymentVoucherDatabaseHelper::insertVoucher(GeneralVoucherDataObject *obj)
{
    this->startTransaction();
    QMap<QString,QVariant> map;
    obj->voucherType = VoucherType;

    GeneralVoucherDatabaseHelper vHelper;
    obj->voucherNumber = vHelper.setNextVoucherNoByType(VoucherType, obj->VoucherPrefix, Payment_Voucher_Main_TableName );

    if(obj->TransactionId.length()==0)
    obj->TransactionId=QString::number(QDateTime::currentDateTime().toTime_t()) + obj->VoucherPrefix + db->clientId;

    map.insert(Payment_Voucher_Main_Voucher_No,obj->voucherNumber);
    map.insert(Payment_Voucher_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Payment_Voucher_Main_Date_Created,obj->VoucherDate);
    map.insert(Payment_Voucher_Main_Voucher_Date,obj->VoucherDate);
    map.insert(Payment_Voucher_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    map.insert(Payment_Voucher_Main_Billing_Name,obj->ledgerObject.LedgerName);
    map.insert(Payment_Voucher_Main_Total,obj->grandTotal);
    map.insert(Payment_Voucher_Main_Narration,obj->narration);
    map.insert(Payment_Voucher_Main_Project_ID,obj->ProjectId);
    map.insert(Payment_Voucher_Main_Revision_Number,obj->RevisionNo);
    map.insert(Payment_Voucher_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Payment_Voucher_Main_GeneratedTransactionId, obj->TransactionId);
    map.insert(Payment_Voucher_Main_VoucherStatus,obj->status);    
    map.insert(Payment_Voucher_Main_TimeStampC,obj->timestamp);
    map.insert(Payment_Voucher_Main_Last_Updated,QDateTime::currentDateTime());
    map.insert(Payment_Voucher_Main_POC_Name,obj->Contact.ContactName);
    map.insert(Payment_Voucher_Main_POC_Phone,obj->Contact.PhoneNumber);
    map.insert(Payment_Voucher_Main_POC_Email,obj->Contact.email);
    map.insert(Payment_Voucher_Main_VoucherStatus,obj->status);


    QJsonDocument doc(obj->ReqVoucherList);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    map.insert(Payment_Voucher_Main_ReqVoucherList, strJson);

    qDebug()<<"&&&&&&&&&&&&&&&&&&&&&payment list size"<<obj->ledgersList.size();
    db->insertRecordsPlus(map,Payment_Voucher_Main_TableName);
    for(LedgerMasterDataModel item:obj->ledgersList){
        QMap<QString,QVariant> map1;

        map1.insert(Payment_Voucher_Details_Voucher_No,obj->voucherNumber);
        map1.insert(Payment_Voucher_Details_Voucher_Prefix,obj->VoucherPrefix);
        map1.insert(Payment_Voucher_Details_Voucher_Date,obj->VoucherDate);
        map1.insert(Payment_Voucher_Details_Ledger_ID,item.LedgerID);
        qDebug()<<"payment details amt"<<item.amount<<item.drAmount;
        map1.insert(Payment_Voucher_Details_DrAmount,item.drAmount);
        map1.insert(Payment_Voucher_Details_Narration,item.narration);
        map1.insert(Payment_Voucher_Details_Salesman_ID,obj->SalesmanID);

        int basid = db->insertRecordsPlus(map1,Payment_Voucher_Details_TableName);
        qDebug()<<"inserted payment details"<<basid<<item.LedgerID<<item.drAmount;
    }

//    for(LedgerMasterDataModel ledger:obj->ledgersList){
//        QMap <QString,QVariant> ledMap;
//        ledMap.insert(Transaction_Helper_VoucherType,VoucherType);
//        ledMap.insert(Transaction_Helper_VoucherNo,obj->voucherNumber);
//        ledMap.insert(Transaction_Helper_VoucherPrefix,obj->VoucherPrefix);
//        ledMap.insert(Transaction_Helper_VoucherDate,obj->VoucherDate);
//        ledMap.insert(Transaction_Helper_ProjectID,obj->ProjectId);
//        ledMap.insert(Transaction_Helper_SalesManID,obj->SalesmanID);
//        ledMap.insert(Transaction_Helper_TranactionAmount,ledger.amount);
//        ledMap.insert(Transaction_Helper_CreditLedgerID,ledger.LedgerID);
//        ledMap.insert(Transaction_Helper_DebitLedgerID,obj->ledgerObject.LedgerID);
//        ledMap.insert(Transaction_Helper_Narration,obj->ledgerObject.narration);
//        //        db->insertRecordsPlus(ledMap,Transaction_Helper_TableName);
//    }

    LedgerMasterDataModel MainLedger;

    MainLedger.crAmount = obj->grandTotal;
    MainLedger.LedgerID = obj->ledgerObject.LedgerID;
    MainLedger.isInvoiceItem = true;
    MainLedger.AgainstLedger = obj->ledgersList[0].LedgerID;
    obj->ledgersList.insert(0,MainLedger);

    for(int i =0;i<obj->ledgersList.size();i++){
        if(obj->ledgersList[i].drAmount>0)
            obj->ledgersList[i].AgainstLedger = MainLedger.LedgerID;
//        else
//            obj->ledgersList[i].AgainstLedger = obj->ledgersList[0].LedgerID;
    }

    ledTransaction->insertTransaction(obj);

//    return commit();
    int c=commit();

    for(LedgerMasterDataModel led:obj->ledgersList){

        if(led.dbName != "" && !obj->fromExternal && obj->sendFlag){
            GeneralVoucherDataObject v;
            NotificationDataModel *notification = new NotificationDataModel();
            QJsonDocument doc(v.ObjToJson(obj));
            QString strJson(doc.toJson(QJsonDocument::Compact));

            notification->NotificationData = strJson;
            notification->ToDBName=led.dbName;
            notification->TypeOfData="Voucher";
            notification->Action=1001;
            //        notification->GeneratedTransactionId=QString::number(QDateTime::currentDateTime().toTime_t()) + obj->VoucherPrefix;
            notification->GeneratedTransactionId = obj->TransactionId;

            sendChannelDatabaseHelper *sendHelper = new sendChannelDatabaseHelper();
            sendHelper->insertNotification(notification);
        }
//        else{
//            qDebug()<<"No new notifications";
//            qDebug()<<obj->ledgerObject.dbName<<obj->fromExternal<<obj->sendFlag;
//        }
    }

    return c;

}

int PaymentVoucherDatabaseHelper::updateVoucher(GeneralVoucherDataObject *obj, QString oldID)
{
    this->startTransaction();
    obj->voucherType = VoucherType;
    QMap<QString,QVariant> map5;
    map5.insert(Payment_Voucher_Details_Voucher_No,oldID);
    map5.insert(Payment_Voucher_Details_Voucher_Prefix,obj->VoucherPrefix);
    qDebug()<<"&&&&&&$$$$$$$$$$$$***********"<<"deleting payment details"<<oldID<<obj->VoucherPrefix;
    db->deleteRecordsPlus(map5,Payment_Voucher_Details_TableName);

    QMap<QString,QVariant> maptr;
    maptr.insert(Transaction_Helper_VoucherNo,oldID);
    maptr.insert(Transaction_Helper_VoucherType,VoucherType);
    //    db->deleteRecordsPlus(maptr,Transaction_Helper_TableName);

    QMap<QString,QVariant> map;
    map.insert(Payment_Voucher_Main_Voucher_No,obj->voucherNumber);
    map.insert(Payment_Voucher_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Payment_Voucher_Main_Date_Created,obj->VoucherDate);
    map.insert(Payment_Voucher_Main_Voucher_Date,obj->VoucherDate);
    map.insert(Payment_Voucher_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    map.insert(Payment_Voucher_Main_Billing_Name,obj->ledgerObject.LedgerName);
    map.insert(Payment_Voucher_Main_Total,obj->grandTotal);
    map.insert(Payment_Voucher_Main_Narration,obj->narration);
    map.insert(Payment_Voucher_Main_Project_ID,obj->ProjectId);
    map.insert(Payment_Voucher_Main_Revision_Number,obj->RevisionNo);
    map.insert(Payment_Voucher_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Payment_Voucher_Main_GeneratedTransactionId, obj->TransactionId);
    map.insert(Payment_Voucher_Main_VoucherStatus,obj->status);
    map.insert(Payment_Voucher_Main_Last_Updated,QDateTime::currentDateTime());
    map.insert(Payment_Voucher_Main_POC_Name,obj->Contact.ContactName);
    map.insert(Payment_Voucher_Main_POC_Phone,obj->Contact.PhoneNumber);
    map.insert(Payment_Voucher_Main_POC_Email,obj->Contact.email);
    map.insert(Payment_Voucher_Main_VoucherStatus,obj->status);


    QJsonDocument doc(obj->ReqVoucherList);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    map.insert(Payment_Voucher_Main_ReqVoucherList, strJson);

    QMap<QString,QVariant> map3;
    map3.insert(Payment_Voucher_Main_Voucher_No,oldID);
    map3.insert(Payment_Voucher_Main_Voucher_Prefix, obj->VoucherPrefix);
    db->updateRecordsPlus(map,map3,Payment_Voucher_Main_TableName);


    for(LedgerMasterDataModel item:obj->ledgersList){
        QMap<QString,QVariant> map1;

        map1.insert(Payment_Voucher_Details_Voucher_No,obj->voucherNumber);
        map1.insert(Payment_Voucher_Details_Voucher_Prefix,obj->VoucherPrefix);
        map1.insert(Payment_Voucher_Details_Voucher_Date,obj->VoucherDate);
        map1.insert(Payment_Voucher_Details_Ledger_ID,item.LedgerID);
        map1.insert(Payment_Voucher_Details_DrAmount,item.drAmount);
        map1.insert(Payment_Voucher_Details_Narration,item.narration);
        map1.insert(Payment_Voucher_Details_Salesman_ID,obj->SalesmanID);

        int basid = db->insertRecordsPlus(map1,Payment_Voucher_Details_TableName);
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
        // db->insertRecordsPlus(ledMap,Transaction_Helper_TableName);
    }
    LedgerMasterDataModel MainLedger;

    MainLedger.crAmount = obj->grandTotal;
    MainLedger.LedgerID = obj->ledgerObject.LedgerID;
    MainLedger.isInvoiceItem = true;
    MainLedger.AgainstLedger = obj->ledgersList[0].LedgerID;
    obj->ledgersList.insert(0,MainLedger);

    for(int i =0;i<obj->ledgersList.size();i++){
        if(obj->ledgersList[i].drAmount>0)
            obj->ledgersList[i].AgainstLedger = MainLedger.LedgerID;
//        else
//            obj->ledgersList[i].AgainstLedger = obj->ledgersList[0].LedgerID;
    }



    ledTransaction->deleteTransaction(obj);
    ledTransaction->insertTransaction(obj);


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

int PaymentVoucherDatabaseHelper::deleteVoucher(GeneralVoucherDataObject *obj)
{
    this->startTransaction();
    QMap<QString,QVariant> map3;
    map3.insert(Payment_Voucher_Main_Voucher_No,obj->voucherNumber);
    map3.insert(Payment_Voucher_Main_Voucher_Prefix,obj->VoucherPrefix);
    db->deleteRecordsPlus(map3,Payment_Voucher_Main_TableName);

    QMap<QString,QVariant> map4;
    qDebug()<<Q_FUNC_INFO<<"payment voucher detais";
    map4.insert(Payment_Voucher_Details_Voucher_No,obj->voucherNumber);
    map4.insert(Payment_Voucher_Details_Voucher_Prefix,obj->VoucherPrefix);
    db->deleteRecordsPlus(map4,Payment_Voucher_Details_TableName);

    QMap<QString,QVariant> maptr;
    maptr.insert(Transaction_Helper_VoucherNo,obj->voucherNumber);
    maptr.insert(Transaction_Helper_VoucherType,VoucherType);
    //    db->deleteRecordsPlus(maptr,Transaction_Helper_TableName);

    ledTransaction->deleteTransaction(obj);

    this->commit();


}

int PaymentVoucherDatabaseHelper::commit()
{
    qDebug()<<Q_FUNC_INFO;
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

int PaymentVoucherDatabaseHelper::getMaxID()
{
    QString qStr = "SELECT MAX("+Payment_Voucher_Main_id+") FROM " + Payment_Voucher_Main_TableName;
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



GeneralVoucherDataObject *PaymentVoucherDatabaseHelper::getVoucherByVoucherNoPtr(QString vNo, QString voucherPrefix)
{
    qDebug()<<"256";
    GeneralVoucherDataObject *obj = new GeneralVoucherDataObject;
    SalesInventoryItemDatabaseHelper *itemHelper = new SalesInventoryItemDatabaseHelper();
    LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper();
    BillwiseRecordsDatabaseHelper *billHelper = new BillwiseRecordsDatabaseHelper;

    QString qStrLedgers ="SELECT ";
    qStrLedgers += Payment_Voucher_Details_Ledger_ID +", ";
    qStrLedgers += Payment_Voucher_Details_DrAmount + ", ";
    qStrLedgers += Payment_Voucher_Details_CrAmount;
    qStrLedgers += " FROM " + Payment_Voucher_Details_TableName;
    qStrLedgers += " WHERE " + Payment_Voucher_Details_Voucher_No + "= '" + vNo + "'";
    qStrLedgers += " AND " +  Payment_Voucher_Details_Voucher_Prefix + "= '" + voucherPrefix + "'";

    QString qStrMain ="SELECT "
            + Payment_Voucher_Main_Voucher_No + ", "
            + Payment_Voucher_Main_Voucher_Prefix + ", "
            + Payment_Voucher_Main_Date_Created + ", "
            + Payment_Voucher_Main_Voucher_Date + ", "
            + Payment_Voucher_Main_TimeStamp + ", "
            + Payment_Voucher_Main_Ledger_ID + ", "
            + Payment_Voucher_Main_Billing_Name + ", "
            + Payment_Voucher_Main_Total + ", "
            + Payment_Voucher_Main_Narration + ", "
            + Payment_Voucher_Main_Added_By + ", "
            + Payment_Voucher_Main_Delivery_Date + ", "
            + Payment_Voucher_Main_Completion_Probability + ", "
            + Payment_Voucher_Main_Credit_Period + ", "
            + Payment_Voucher_Main_POC_Name + ", "
            + Payment_Voucher_Main_POC_Phone + ", "
            + Payment_Voucher_Main_POC_Email + ", "
            + Payment_Voucher_Main_Project_ID + ", "
            + Payment_Voucher_Main_Revision_Number + ", "
            + Payment_Voucher_Main_Converted_To_Sales_Order + ", "
            + Payment_Voucher_Main_Quotation_Dropped + ", "
            + Payment_Voucher_Main_Quotation_Dropped_Reason + ", "
            + Payment_Voucher_Main_Requirement_Voucher_No + ", "
            + Payment_Voucher_Main_Salesman_ID + ", "
            + Payment_Voucher_Main_LPO + ", "
            + Payment_Voucher_Main_VoucherStatus + ", "
            + Payment_Voucher_Main_ModeOfService + ", "
            + Payment_Voucher_Main_Discount_In_Amount + ", "
            + Payment_Voucher_Main_Discount_In_Percent + ", "
            + Payment_Voucher_Main_VATAmount + ", "
            + Payment_Voucher_Main_Contact_ID + ", "
            + Payment_Voucher_Main_Gross_Total + ", "
            + Payment_Voucher_Main_Reference + ", "
            + Payment_Voucher_Main_Customer_Expecting_Date + ", "
            + Payment_Voucher_Main_Currency + ", "
            + Payment_Voucher_Main_BoxesCount+ ", "
            + Payment_Voucher_Main_TotalWeight + ", "
            + Payment_Voucher_Main_LRNo + ", "
            + Payment_Voucher_Main_VoucherNoDisplay + ", "
            + Payment_Voucher_Main_ReqVoucherList + ", "
            + Payment_Voucher_Main_Origin + ", "
            + Payment_Voucher_Main_GeneratedTransactionId
            + " FROM " + Payment_Voucher_Main_TableName
            + " WHERE " + Payment_Voucher_Main_Voucher_No + "= :vno"
            + " AND " +  Payment_Voucher_Main_Voucher_Prefix + "= '" + voucherPrefix + "'";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStrLedgers);
    query.bindValue(":id",vNo);
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
            ledger.amount = -1 * query.value(1).toFloat();
            ledger.crAmount = query.value(2).toFloat();
            ledger.listid =cnt++;
//            ledger.mapList = billHelper->getBillMappingsByLedger(ledger.LedgerID, vNo, QDate::currentDate());
            obj->ledgersList.append(ledger);
            qDebug()<<ledger.LedgerName<<ledger.amount;
        }
        qDebug()<<"305";
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

QSqlQueryModel *PaymentVoucherDatabaseHelper::getVoucherListByDateModel(QDateTime dateFrom, QDateTime dateTo)
{

    LedgerMasterDatabaseHelper ledHelper;
    QString qStr ="SELECT ";
    qStr += "'V' , ";
    qStr += Payment_Voucher_Main_Voucher_No +", ";
    qStr += "'" + VoucherType + "', " ;
    qStr += Payment_Voucher_Main_Voucher_Prefix + ", ";
    qStr += Payment_Voucher_Main_Voucher_Date+" as 'Voucher Date', ";
    qStr += " CONCAT(" + Payment_Voucher_Main_Voucher_Prefix + ", '-', " + Payment_Voucher_Main_Voucher_No+ ") as 'Voucher No', ";
    qStr += "(SELECT "+ledHelper.Ledger_Master_LedgerName + " FROM "+ ledHelper.Ledger_Master_TableName;
    qStr += " WHERE "+ledHelper.Ledger_Master_LedgerID + "= SA.";
    qStr += Payment_Voucher_Main_Ledger_ID + ") Ledger, ";
    qStr += Payment_Voucher_Main_Total;
    qStr += " FROM " + Payment_Voucher_Main_TableName;
    qStr += " as SA WHERE ";
    qStr += Payment_Voucher_Main_Voucher_Date;
    qStr += " >= '";
    qStr += dateFrom.toString("yyyy-MM-dd");
    qStr += "' AND ";
    qStr += Payment_Voucher_Main_Voucher_Date;
    qStr += " <= '";
    qStr += dateTo.toString("yyyy-MM-dd");
    qStr += "'";

//    qDebug()<<qStr;

    QSqlQueryModel *model = new QSqlQueryModel();
//    model->setQuery(qStr,QSqlDatabase::database(DatabaseValues::connectionString));
    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}

QList<GeneralVoucherDataObject> PaymentVoucherDatabaseHelper::getVoucherListByDate(QDate dateFrom, QDate dateTo)
{
    QList<GeneralVoucherDataObject> vouchersList;
    QString qStr ="SELECT "
            + Payment_Voucher_Main_Voucher_No+ ", "
            + Payment_Voucher_Main_Voucher_Prefix+ ", "
            + Payment_Voucher_Main_Voucher_Date+", "
            + Payment_Voucher_Main_Ledger_ID+ ", "
            + Payment_Voucher_Main_Total
            + " FROM " + Payment_Voucher_Main_TableName
            + " WHERE "
            + Payment_Voucher_Main_Voucher_Date
            + " >= '"
            + dateFrom.toString("yyyy-MM-dd")
            + "' AND "
            + Payment_Voucher_Main_Voucher_Date
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
