#include "contravoucherdatabasehelper.h"

ContraVoucherDatabaseHelper::ContraVoucherDatabaseHelper(QString connString)
{
    db = new DatabaseHelper(connString);
    ledTransaction = new TransactionHelper();
}

ContraVoucherDatabaseHelper::~ContraVoucherDatabaseHelper()
{

}

int ContraVoucherDatabaseHelper::startTransaction()
{
    db->startTransaction();

}

QString ContraVoucherDatabaseHelper::getNextVoucherNo(QString voucherPrefix)
{
    GeneralVoucherDatabaseHelper vHelper;
    QString voucherNumber = vHelper.getNextVoucherNoByType(VoucherType, voucherPrefix, Contra_Voucher_Main_TableName);
    return voucherNumber;

}

int ContraVoucherDatabaseHelper::insertVoucher(GeneralVoucherDataObject *obj)
{
    this->startTransaction();
    QMap<QString,QVariant> map;
    obj->voucherType = VoucherType;

    GeneralVoucherDatabaseHelper vHelper;
    obj->voucherNumber = vHelper.setNextVoucherNoByType(VoucherType, obj->VoucherPrefix, Contra_Voucher_Main_TableName );

    map.insert(Contra_Voucher_Main_Voucher_No,obj->voucherNumber);
    map.insert(Contra_Voucher_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Contra_Voucher_Main_Date_Created,obj->VoucherDate);
    map.insert(Contra_Voucher_Main_Voucher_Date,obj->VoucherDate);
    map.insert(Contra_Voucher_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    map.insert(Contra_Voucher_Main_Billing_Name,obj->ledgerObject.LedgerName);
    map.insert(Contra_Voucher_Main_Total,obj->grandTotal);
    map.insert(Contra_Voucher_Main_Narration,obj->narration);
    map.insert(Contra_Voucher_Main_Project_ID,obj->ProjectId);
    map.insert(Contra_Voucher_Main_Revision_Number,obj->RevisionNo);
    map.insert(Contra_Voucher_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Contra_Voucher_Main_TimeStampC,obj->timestamp);
    map.insert(Contra_Voucher_Main_Last_Updated,QDateTime::currentDateTime());

    QJsonDocument doc(obj->ReqVoucherList);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    map.insert(Contra_Voucher_Main_ReqVoucherList, strJson);


    db->insertRecordsPlus(map,Contra_Voucher_Main_TableName);
    for(LedgerMasterDataModel item:obj->ledgersList){
        QMap<QString,QVariant> map1;

        map1.insert(Contra_Voucher_Details_Voucher_No,obj->voucherNumber);
        map1.insert(Contra_Voucher_Details_Voucher_Prefix,obj->VoucherPrefix);
        map1.insert(Contra_Voucher_Details_Voucher_Date,obj->VoucherDate);
        map1.insert(Contra_Voucher_Details_Ledger_ID,item.LedgerID);
        map1.insert(Contra_Voucher_Details_CrAmount,item.crAmount);
        map1.insert(Contra_Voucher_Details_DrAmount,item.drAmount);
        map1.insert(Contra_Voucher_Details_Project_ID,obj->ProjectId);
        map1.insert(Contra_Voucher_Details_Narration,item.narration);
        map1.insert(Contra_Voucher_Details_Salesman_ID,obj->SalesmanID);

        int basid = db->insertRecordsPlus(map1,Contra_Voucher_Details_TableName);
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

    MainLedger.crAmount = obj->grandTotal;
    MainLedger.LedgerID = obj->ledgerObject.LedgerID;
    MainLedger.isInvoiceItem = true;
    MainLedger.AgainstLedger = obj->ledgersList[0].LedgerID;
    obj->ledgersList.insert(0,MainLedger);

    for(int i =0;i<obj->ledgersList.size();i++){
        if(obj->ledgersList[i].drAmount>0)
            obj->ledgersList[i].AgainstLedger = MainLedger.LedgerID;
        else
            obj->ledgersList[i].AgainstLedger = obj->ledgersList[0].LedgerID;
    }

    ledTransaction->insertTransaction(obj);

    return commit();
}

int ContraVoucherDatabaseHelper::updateVoucher(GeneralVoucherDataObject *obj, QString oldID)
{
    this->startTransaction();
    obj->voucherType = VoucherType;
    QMap<QString,QVariant> map5;
    map5.insert(Contra_Voucher_Details_Voucher_No,oldID);
    db->deleteRecordsPlus(map5,Contra_Voucher_Details_TableName);

    QMap<QString,QVariant> maptr;
    maptr.insert(Transaction_Helper_VoucherNo,oldID);
    maptr.insert(Transaction_Helper_VoucherType,VoucherType);
    //    db->deleteRecordsPlus(maptr,Transaction_Helper_TableName);

    QMap<QString,QVariant> map;
    map.insert(Contra_Voucher_Main_Voucher_No,obj->voucherNumber);
    map.insert(Contra_Voucher_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Contra_Voucher_Main_Date_Created,obj->VoucherDate);
    map.insert(Contra_Voucher_Main_Voucher_Date,obj->VoucherDate);
    map.insert(Contra_Voucher_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    map.insert(Contra_Voucher_Main_Billing_Name,obj->ledgerObject.LedgerName);
    map.insert(Contra_Voucher_Main_Total,obj->grandTotal);
    map.insert(Contra_Voucher_Main_Narration,obj->narration);
    map.insert(Contra_Voucher_Main_Project_ID,obj->ProjectId);
    map.insert(Contra_Voucher_Main_Revision_Number,obj->RevisionNo);
    map.insert(Contra_Voucher_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Contra_Voucher_Main_Last_Updated, QDateTime::currentDateTime());

    QJsonDocument doc(obj->ReqVoucherList);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    map.insert(Contra_Voucher_Main_ReqVoucherList, strJson);

    QMap<QString,QVariant> map3;
    map3.insert(Contra_Voucher_Main_Voucher_No,oldID);
    db->updateRecordsPlus(map,map3,Contra_Voucher_Main_TableName);


    for(LedgerMasterDataModel item:obj->ledgersList){
        QMap<QString,QVariant> map1;

        map1.insert(Contra_Voucher_Details_Voucher_No,obj->voucherNumber);
        map1.insert(Contra_Voucher_Details_Voucher_Prefix,obj->VoucherPrefix);
        map1.insert(Contra_Voucher_Details_Voucher_Date,obj->VoucherDate);
        map1.insert(Contra_Voucher_Details_Ledger_ID,item.LedgerID);
        map1.insert(Contra_Voucher_Details_CrAmount,item.crAmount);
        map1.insert(Contra_Voucher_Details_DrAmount,item.drAmount);
        map1.insert(Contra_Voucher_Details_Project_ID,obj->ProjectId);
        map1.insert(Contra_Voucher_Details_Narration,item.narration);
        map1.insert(Contra_Voucher_Details_Salesman_ID,obj->SalesmanID);

        int basid = db->insertRecordsPlus(map1,Contra_Voucher_Details_TableName);
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
        else
            obj->ledgersList[i].AgainstLedger = obj->ledgersList[0].LedgerID;
    }



    ledTransaction->deleteTransaction(obj);
    ledTransaction->insertTransaction(obj);


    return commit();
}

int ContraVoucherDatabaseHelper::deleteVoucher(GeneralVoucherDataObject *obj)
{
    this->startTransaction();
    QMap<QString,QVariant> map3;
    map3.insert(Contra_Voucher_Main_Voucher_No,obj->voucherNumber);
    db->deleteRecordsPlus(map3,Contra_Voucher_Main_TableName);

    QMap<QString,QVariant> map4;
    map4.insert(Contra_Voucher_Details_Voucher_No,obj->voucherNumber);
    db->deleteRecordsPlus(map4,Contra_Voucher_Details_TableName);

    QMap<QString,QVariant> maptr;
    maptr.insert(Transaction_Helper_VoucherNo,obj->voucherNumber);
    maptr.insert(Transaction_Helper_VoucherType,VoucherType);
    //    db->deleteRecordsPlus(maptr,Transaction_Helper_TableName);

    ledTransaction->deleteTransaction(obj);
    return commit();

}

int ContraVoucherDatabaseHelper::commit()
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

int ContraVoucherDatabaseHelper::getMaxID()
{
    QString qStr = "SELECT MAX("+Contra_Voucher_Main_id+") FROM " + Contra_Voucher_Main_TableName;
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

GeneralVoucherDataObject ContraVoucherDatabaseHelper::getVoucherByVoucherNo(QString vNo)
{
    qDebug()<<"256";
    GeneralVoucherDataObject obj;
    SalesInventoryItemDatabaseHelper *itemHelper = new SalesInventoryItemDatabaseHelper();
    LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper();
    BillwiseRecordsDatabaseHelper *billHelper = new BillwiseRecordsDatabaseHelper;

    QString qStrLedgers ="SELECT ";
    qStrLedgers += Contra_Voucher_Details_Ledger_ID +", ";
    qStrLedgers += Contra_Voucher_Details_CrAmount;
    qStrLedgers += " FROM " + Contra_Voucher_Details_TableName;
    qStrLedgers += " WHERE " + Contra_Voucher_Details_Voucher_No + "=:id";


    QString qStr ="SELECT "
            + Contra_Voucher_Main_Voucher_No+ ", "
            + Contra_Voucher_Main_Voucher_Prefix+ ", "
            + Contra_Voucher_Main_Voucher_Date+", "
            + Contra_Voucher_Main_Ledger_ID+ ", "
            + Contra_Voucher_Main_Total
            + " FROM " + Contra_Voucher_Main_TableName
            + " WHERE " + Contra_Voucher_Main_Voucher_No + "=:id";


    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

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
            ledger.drAmount = query.value(1).toFloat();
            ledger.amount = query.value(1).toFloat();
            ledger.listid =cnt++;
            ledger.mapList = billHelper->getBillMappingsByLedger(ledger.LedgerID, vNo, QDate::currentDate());
            obj.ledgersList.append(ledger);
        }
        qDebug()<<"305";
    }

    query.prepare(qStr);
    query.bindValue(":id",vNo);

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

        }
    }

    return obj;
}

GeneralVoucherDataObject *ContraVoucherDatabaseHelper::getVoucherByVoucherNoPtr(QString vNo, QString voucherPrefix)
{
    GeneralVoucherDataObject *obj = new GeneralVoucherDataObject;
    SalesInventoryItemDatabaseHelper *itemHelper = new SalesInventoryItemDatabaseHelper();
    LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper();
    BillwiseRecordsDatabaseHelper *billHelper = new BillwiseRecordsDatabaseHelper;

    QString qStrLedgers ="SELECT ";
    qStrLedgers += Contra_Voucher_Details_Ledger_ID +", ";
    qStrLedgers += Contra_Voucher_Details_DrAmount + ", ";
    qStrLedgers += Contra_Voucher_Details_CrAmount;
    qStrLedgers += " FROM " + Contra_Voucher_Details_TableName;
    qStrLedgers += " WHERE " + Contra_Voucher_Details_Voucher_No + "=:id";


    QString qStrMain ="SELECT "
            + Contra_Voucher_Main_Voucher_No + ", "
            + Contra_Voucher_Main_Voucher_Prefix + ", "
            + Contra_Voucher_Main_Date_Created + ", "
            + Contra_Voucher_Main_Voucher_Date + ", "
            + Contra_Voucher_Main_TimeStamp + ", "
            + Contra_Voucher_Main_Ledger_ID + ", "
            + Contra_Voucher_Main_Billing_Name + ", "
            + Contra_Voucher_Main_Total + ", "
            + Contra_Voucher_Main_Narration + ", "
            + Contra_Voucher_Main_Added_By + ", "
            + Contra_Voucher_Main_Delivery_Date + ", "
            + Contra_Voucher_Main_Completion_Probability + ", "
            + Contra_Voucher_Main_Credit_Period + ", "
            + Contra_Voucher_Main_POC_Name + ", "
            + Contra_Voucher_Main_POC_Phone + ", "
            + Contra_Voucher_Main_POC_Email + ", "
            + Contra_Voucher_Main_Project_ID + ", "
            + Contra_Voucher_Main_Revision_Number + ", "
            + Contra_Voucher_Main_Converted_To_Sales_Order + ", "
            + Contra_Voucher_Main_Quotation_Dropped + ", "
            + Contra_Voucher_Main_Quotation_Dropped_Reason + ", "
            + Contra_Voucher_Main_Requirement_Voucher_No + ", "
            + Contra_Voucher_Main_Salesman_ID + ", "
            + Contra_Voucher_Main_LPORef + ", "
            + Contra_Voucher_Main_VoucherStatus + ", "
            + Contra_Voucher_Main_ModeOfService + ", "
            + Contra_Voucher_Main_Discount_In_Amount + ", "
            + Contra_Voucher_Main_Discount_In_Percent + ", "
            + Contra_Voucher_Main_VATAmount + ", "
            + Contra_Voucher_Main_Contact_ID + ", "
            + Contra_Voucher_Main_Gross_Total + ", "
            + Contra_Voucher_Main_Reference + ", "
            + Contra_Voucher_Main_Customer_Expecting_Date + ", "
            + Contra_Voucher_Main_Currency + ", "
            + Contra_Voucher_Main_BoxesCount+ ", "
            + Contra_Voucher_Main_TotalWeight + ", "
            + Contra_Voucher_Main_LRNo + ", "
            + Contra_Voucher_Main_VoucherNoDisplay + ", "
            + Contra_Voucher_Main_ReqVoucherList + ", "
            + Contra_Voucher_Main_Origin
            + " FROM " + Contra_Voucher_Main_TableName
            + " WHERE " + Contra_Voucher_Main_Voucher_No + "= :vno"
            + " AND " +  Contra_Voucher_Main_Voucher_Prefix + "= :voucherPrefix";


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
            ledger.amount = query.value(1).toFloat();
            ledger.listid =cnt++;
            ledger.mapList = billHelper->getBillMappingsByLedger(ledger.LedgerID, vNo, QDate::currentDate());
            obj->ledgersList.append(ledger);
        }
    }

    query.prepare(qStrMain);
    query.bindValue(":vno",vNo);
    query.bindValue(":voucherPrefix", voucherPrefix);

    if(!query.exec()){
        qDebug()<<query.lastQuery();
        qDebug()<<query.lastError();
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


        }
    }

    return obj;
}

QSqlQueryModel *ContraVoucherDatabaseHelper::getVoucherListByDateModel(QDate dateFrom, QDate dateTo)
{
    LedgerMasterDatabaseHelper ledHelper;
    QString qStr ="SELECT ";
    qStr += "'V' , ";
    qStr += Contra_Voucher_Main_Voucher_No +", ";
    qStr += "'" + VoucherType + "', " ;
    qStr += Contra_Voucher_Main_Voucher_Prefix + ", ";
    qStr += Contra_Voucher_Main_Voucher_Date+" as 'Voucher Date', ";
    qStr += " CONCAT(" + Contra_Voucher_Main_Voucher_Prefix + ", '-', " + Contra_Voucher_Main_Voucher_No+ ") as 'Voucher No', ";
    qStr += "(SELECT "+ledHelper.Ledger_Master_LedgerName + " FROM "+ ledHelper.Ledger_Master_TableName;
    qStr += " WHERE "+ledHelper.Ledger_Master_LedgerID + "= SA.";
    qStr += Contra_Voucher_Main_Ledger_ID + ") Ledger, ";
    qStr += Contra_Voucher_Main_Total ;
    qStr += " FROM " + Contra_Voucher_Main_TableName;
    qStr += " as SA WHERE ";
    qStr += Contra_Voucher_Main_Voucher_Date;
    qStr += " >= '";
    qStr += dateFrom.toString("yyyy-MM-dd");
    qStr += "' AND ";
    qStr += Contra_Voucher_Main_Voucher_Date;
    qStr += " <= '";
    qStr += dateTo.toString("yyyy-MM-dd");
    qStr += "'";

    return  DatabaseHelper::getQueryModel(qStr);

}

QString ContraVoucherDatabaseHelper::getVoucherNumberByLedID(QString LedID, QDate Date)
{

}

QPair<QString, QString> ContraVoucherDatabaseHelper::getVoucherNoByTransactionId(QString transId)
{

}

QSqlQueryModel *ContraVoucherDatabaseHelper::getVoucherListByDateTimeModel(QDateTime dateFrom, QDateTime dateTo)
{

}

QList<GeneralVoucherDataObject> ContraVoucherDatabaseHelper::getVoucherListByDate(QDate dateFrom, QDate dateTo)
{
    QList<GeneralVoucherDataObject> vouchersList;
    QString qStr ="SELECT "
            + Contra_Voucher_Main_Voucher_No+ ", "
            + Contra_Voucher_Main_Voucher_Prefix+ ", "
            + Contra_Voucher_Main_Voucher_Date+", "
            + Contra_Voucher_Main_Ledger_ID+ ", "
            + Contra_Voucher_Main_Total
            + " FROM " + Contra_Voucher_Main_TableName
            + " WHERE "
            + Contra_Voucher_Main_Voucher_Date
            + " >= '"
            + dateFrom.toString("yyyy-MM-dd")
            + "' AND "
            + Contra_Voucher_Main_Voucher_Date
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
