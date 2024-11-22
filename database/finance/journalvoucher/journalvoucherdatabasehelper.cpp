#include "journalvoucherdatabasehelper.h"

JournalVoucherDatabaseHelper::JournalVoucherDatabaseHelper(QString connString)
{
    db = new DatabaseHelper(connString);
    ledTransaction = new TransactionHelper(db);

}

JournalVoucherDatabaseHelper::~JournalVoucherDatabaseHelper()
{

}

int JournalVoucherDatabaseHelper::startTransaction()
{
    db->startTransaction();

}

QString JournalVoucherDatabaseHelper::getNextVoucherNo(QString voucherPrefix)
{
    GeneralVoucherDatabaseHelper vHelper;
    QString voucherNumber = vHelper.getNextVoucherNoByType(VoucherType, voucherPrefix, Journal_Voucher_Main_TableName);
    return voucherNumber;

}

QString JournalVoucherDatabaseHelper::getVoucherNumberByLedID(QString LedID, QDate Date)
{

}

QPair<QString, QString> JournalVoucherDatabaseHelper::getVoucherNoByTransactionId(QString transId)
{

}

QSqlQueryModel *JournalVoucherDatabaseHelper::getVoucherListByDateTimeModel(QDateTime dateFrom, QDateTime dateTo)
{

}

int JournalVoucherDatabaseHelper::insertVoucher(GeneralVoucherDataObject *obj)
{
    this->startTransaction();

    QMap<QString,QVariant> map;

    GeneralVoucherDatabaseHelper vHelper;
    obj->voucherNumber = vHelper.setNextVoucherNoByType(VoucherType, obj->VoucherPrefix, Journal_Voucher_Main_TableName );

    map.insert(Journal_Voucher_Main_Voucher_No,obj->voucherNumber);
    map.insert(Journal_Voucher_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Journal_Voucher_Main_Date_Created,obj->VoucherDate);
    map.insert(Journal_Voucher_Main_Voucher_Date,obj->VoucherDate);
    map.insert(Journal_Voucher_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    map.insert(Journal_Voucher_Main_Billing_Name,obj->ledgerObject.LedgerName);
    map.insert(Journal_Voucher_Main_Total,obj->grandTotal);
    map.insert(Journal_Voucher_Main_Narration,obj->narration);
    map.insert(Journal_Voucher_Main_Project_ID,obj->ProjectId);
    map.insert(Journal_Voucher_Main_Revision_Number,obj->RevisionNo);
    map.insert(Journal_Voucher_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Journal_Voucher_Main_TimeStampC,obj->timestamp);
    map.insert(Journal_Voucher_Main_Last_Updated,QDateTime::currentDateTime());
    map.insert(Journal_Voucher_Main_Status,QString::number(obj->status));


    db->insertRecordsPlus(map,Journal_Voucher_Main_TableName);

    for(LedgerMasterDataModel item:obj->ledgersList){
        QMap<QString,QVariant> map1;

        map1.insert(Journal_Voucher_Details_Voucher_No,obj->voucherNumber);
        map1.insert(Journal_Voucher_Details_Voucher_Prefix,obj->VoucherPrefix);
        map1.insert(Journal_Voucher_Details_Voucher_Date,obj->VoucherDate);
        map1.insert(Journal_Voucher_Details_Ledger_ID,item.LedgerID);
        map1.insert(Journal_Voucher_Details_CR_Amount,item.crAmount);
        map1.insert(Journal_Voucher_Details_DR_Amount,item.drAmount);
        map1.insert(Journal_Voucher_Details_Narration,item.narration);
        map1.insert(Journal_Voucher_Main_Project_ID,obj->ProjectId);
        map1.insert(Journal_Voucher_Details_Salesman_ID,obj->SalesmanID);

        int basid = db->insertRecordsPlus(map1,Journal_Voucher_Details_TableName);
        qDebug()<<"inserted Journal details"<<basid<<item.LedgerID<<item.drAmount;
    }

    for(LedgerMasterDataModel ledger:obj->ledgersList){
        QMap <QString,QVariant> ledMap;
        ledMap.insert(Transaction_Helper_VoucherType,VoucherType);
        ledMap.insert(Transaction_Helper_VoucherNo,obj->voucherNumber);
        ledMap.insert(Transaction_Helper_VoucherPrefix,obj->VoucherPrefix);
        ledMap.insert(Transaction_Helper_VoucherDate,obj->VoucherDate);
        ledMap.insert(Transaction_Helper_ProjectID,obj->ProjectId);
        ledMap.insert(Transaction_Helper_SalesManID,obj->SalesmanID);
        if(ledger.crAmount>0){
            ledMap.insert(Transaction_Helper_TranactionAmount,ledger.crAmount);
            ledMap.insert(Transaction_Helper_CreditLedgerID,ledger.LedgerID);


        }else{
            ledMap.insert(Transaction_Helper_TranactionAmount,ledger.drAmount);
            ledMap.insert(Transaction_Helper_DebitLedgerID,ledger.LedgerID);


        }
        ledMap.insert(Transaction_Helper_Narration,obj->ledgerObject.narration);
        //        db->insertRecordsPlus(ledMap,Transaction_Helper_TableName);
    }

    QString agCrID ;
    QString agDrID ;
    obj->voucherType = VoucherType;

    for(int i =0;i<obj->ledgersList.size();i++){
        if(obj->ledgersList[i].drAmount>0)
            obj->ledgersList[i].AgainstLedger = obj->ledgersList[0].LedgerID;
        else
            obj->ledgersList[i].AgainstLedger = obj->ledgersList[0].LedgerID;
    }
    ledTransaction->insertTransaction(obj);

    return commit();
}

int JournalVoucherDatabaseHelper::updateVoucher(GeneralVoucherDataObject *obj, QString oldID)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    this->startTransaction();

    QMap<QString,QVariant> map;
    map.insert(Journal_Voucher_Main_Voucher_No,obj->voucherNumber);
    map.insert(Journal_Voucher_Main_Voucher_Prefix,obj->VoucherPrefix);
    map.insert(Journal_Voucher_Main_Date_Created,obj->VoucherDate);
    map.insert(Journal_Voucher_Main_Voucher_Date,obj->VoucherDate);
    map.insert(Journal_Voucher_Main_Ledger_ID,obj->ledgerObject.LedgerID);
    map.insert(Journal_Voucher_Main_Billing_Name,obj->ledgerObject.LedgerName);
    map.insert(Journal_Voucher_Main_Total,obj->grandTotal);
    map.insert(Journal_Voucher_Main_Narration,obj->narration);
    map.insert(Journal_Voucher_Main_Project_ID,obj->ProjectId);
    map.insert(Journal_Voucher_Main_Revision_Number,obj->RevisionNo);
    map.insert(Journal_Voucher_Main_Salesman_ID,obj->SalesmanID);
    map.insert(Journal_Voucher_Main_Last_Updated,QDateTime::currentDateTime());

    QMap<QString,QVariant> map3;
    map3.insert(Journal_Voucher_Main_Voucher_No,oldID);
    map3.insert(Journal_Voucher_Main_Voucher_Prefix, obj->VoucherPrefix);
    db->updateRecordsPlus(map,map3,Journal_Voucher_Main_TableName);

    QMap<QString,QVariant> maptxndel;
    maptxndel.insert(Transaction_Helper_VoucherNo,oldID);
    maptxndel.insert(Transaction_Helper_VoucherType,VoucherType);
    //    db->deleteRecordsPlus(maptxndel,Transaction_Helper_TableName);

    QMap<QString,QVariant> map6;
    map6.insert(Journal_Voucher_Details_Voucher_No,oldID);
    map6.insert(Journal_Voucher_Details_Voucher_Prefix,obj->VoucherPrefix);
    db->deleteRecordsPlus(map6,Journal_Voucher_Details_TableName);

    for(LedgerMasterDataModel item:obj->ledgersList){
        QMap<QString,QVariant> map1;

        map1.insert(Journal_Voucher_Details_Voucher_No,obj->voucherNumber);
        map1.insert(Journal_Voucher_Details_Voucher_Prefix,obj->VoucherPrefix);
        map1.insert(Journal_Voucher_Details_Voucher_Date,obj->VoucherDate);
        map1.insert(Journal_Voucher_Details_Ledger_ID,item.LedgerID);
        map1.insert(Journal_Voucher_Details_CR_Amount,item.crAmount);
        map1.insert(Journal_Voucher_Details_DR_Amount,item.drAmount);
        map1.insert(Journal_Voucher_Details_Narration,item.narration);
        map1.insert(Journal_Voucher_Main_Project_ID,obj->ProjectId);
        map1.insert(Journal_Voucher_Details_Salesman_ID,obj->SalesmanID);

        int basid = db->insertRecordsPlus(map1,Journal_Voucher_Details_TableName);
        // qDebug()<<"updated Journal details"<<basid<<item.LedgerID<<item.drAmount;
    }

    //    QMap<QString,QVariant> map6;
    //    map6.insert(Journal_Voucher_Details_Voucher_No,oldID);
    //    map6.insert(Journal_Voucher_Details_Voucher_Prefix,obj->VoucherPrefix);
    //    map6.insertMulti(Journal_Voucher_Details_CR_Amount,item.crAmount)
    //    db->deleteRecordsPlus(map6,Journal_Voucher_Details_TableName);


    QMap<QString,QVariant> maptr;
    maptr.insert(Transaction_Helper_VoucherNo,oldID);
    maptr.insert(Transaction_Helper_VoucherType,VoucherType);
    db->deleteRecordsPlus(maptr,Transaction_Helper_TableName);

    QString agCrID ;
    QString agDrID ;
    obj->voucherType = VoucherType;

    for(int i =0;i<obj->ledgersList.size();i++){
        if(obj->ledgersList[i].drAmount>0)
            obj->ledgersList[i].AgainstLedger = obj->ledgersList[0].LedgerID;
        else
            obj->ledgersList[i].AgainstLedger = obj->ledgersList[0].LedgerID;
    }

    ledTransaction->deleteTransaction(obj);
    ledTransaction->insertTransaction(obj);

    qDebug()<<Q_FUNC_INFO<<__LINE__;
    return commit();
}

int JournalVoucherDatabaseHelper::deleteVoucher(GeneralVoucherDataObject *obj)
{
    this->startTransaction();
    QMap<QString,QVariant> map3;
    map3.insert(Journal_Voucher_Main_Voucher_No,obj->voucherNumber);
    map3.insert(Journal_Voucher_Main_Voucher_Prefix, obj->VoucherPrefix);
    db->deleteRecordsPlus(map3,Journal_Voucher_Main_TableName);

    QMap<QString,QVariant> map4;
    map4.insert(Journal_Voucher_Details_Voucher_No,obj->voucherNumber);
    map4.insert(Journal_Voucher_Details_Voucher_Prefix,obj->VoucherPrefix);
    db->deleteRecordsPlus(map4,Journal_Voucher_Details_TableName);

    QMap<QString,QVariant> maptr;
    maptr.insert(Transaction_Helper_VoucherNo,obj->voucherNumber);
    maptr.insert(Transaction_Helper_VoucherType,VoucherType);
    maptr.insert(Transaction_Helper_VoucherPrefix,obj->VoucherPrefix);
    db->deleteRecordsPlus(maptr,Transaction_Helper_TableName);

    return commit();
}

int JournalVoucherDatabaseHelper::commit()
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

int JournalVoucherDatabaseHelper::getMaxID()
{
    QString qStr = "SELECT MAX("+Journal_Voucher_Main_id+") FROM " + Journal_Voucher_Main_TableName;
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



GeneralVoucherDataObject *JournalVoucherDatabaseHelper::getVoucherByVoucherNoPtr(QString vNo, QString voucherPrefix)
{
    GeneralVoucherDataObject* obj = new GeneralVoucherDataObject;
    LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper();
    TransactionHelper trans;

    QString qStrMain ="SELECT ";
    qStrMain +=  Journal_Voucher_Main_Voucher_No + ", ";
    qStrMain +=  Journal_Voucher_Main_Voucher_Prefix + ", ";
    qStrMain +=  Journal_Voucher_Main_Date_Created + ", ";
    qStrMain +=  Journal_Voucher_Main_Voucher_Date + ", ";
    qStrMain +=  Journal_Voucher_Main_TimeStamp + ", ";
    qStrMain +=  Journal_Voucher_Main_Ledger_ID + ", ";
    qStrMain +=  Journal_Voucher_Main_Billing_Name + ", ";
    qStrMain +=  Journal_Voucher_Main_Total + ", ";
    qStrMain +=  Journal_Voucher_Main_Narration + ", ";
    qStrMain +=  Journal_Voucher_Main_Added_By + ", ";
    qStrMain +=  Journal_Voucher_Main_Credit_Period + ", ";
    qStrMain +=  Journal_Voucher_Main_POC_Name + ", ";
    qStrMain +=  Journal_Voucher_Main_POC_Phone + ", ";
    qStrMain +=  Journal_Voucher_Main_POC_Email + ", ";
    qStrMain +=  Journal_Voucher_Main_Project_ID + ", ";
    qStrMain +=  Journal_Voucher_Main_Revision_Number + ", ";
    qStrMain +=  Journal_Voucher_Main_Salesman_ID + ", ";
    qStrMain +=  Journal_Voucher_Main_Currency + ", ";
    qStrMain +=  Journal_Voucher_Main_VoucherNoDisplay + ", ";
    qStrMain +=  Journal_Voucher_Main_Status + ", ";
    qStrMain +=  Journal_Voucher_Main_Origin;
    qStrMain +=  " FROM " + Journal_Voucher_Main_TableName;
    qStrMain +=  " WHERE " + Journal_Voucher_Main_Voucher_No + "= :vno";
    qStrMain +=  " AND " +  Journal_Voucher_Main_Voucher_Prefix + "= :voucherPrefix";

    QString qStrLedgers ="SELECT "
            + Transaction_Helper_id +", "
            + Transaction_Helper_VoucherNo +", "
            + Transaction_Helper_VoucherPrefix +", "
            + Transaction_Helper_ProjectID +", "
            + trans.Transaction_Helper_LedgerID +", "
            + trans.Transaction_Helper_Credit_Amount +", "
            + trans.Transaction_Helper_Debit_Amount+ ", "
            + Transaction_Helper_Narration +", "
            + Transaction_Helper_SalesManID
            + " FROM " + Transaction_Helper_TableName
            + " WHERE " + Transaction_Helper_VoucherNo + "=:id"
            + " AND " + Transaction_Helper_VoucherPrefix + "='"+voucherPrefix+"'"
            + " AND " + Transaction_Helper_VoucherType + "='"+VoucherType+"'";

    QString qStrDetail = "SELECT ";
    qStrDetail += Journal_Voucher_Details_id +", ";
    qStrDetail += Journal_Voucher_Details_Voucher_No +", ";
    qStrDetail += Journal_Voucher_Details_Voucher_Prefix +", ";
    qStrDetail += Journal_Voucher_Details_Voucher_Date +", ";
    qStrDetail += Journal_Voucher_Details_Ledger_ID +", ";
    qStrDetail += Journal_Voucher_Details_CR_Amount +", ";
    qStrDetail += Journal_Voucher_Details_DR_Amount +", ";
    qStrDetail += Journal_Voucher_Details_Narration +" ";
    qStrDetail += " FROM " + Journal_Voucher_Details_TableName;
    qStrDetail += " WHERE " + Journal_Voucher_Details_Voucher_No + " = :vno";
    qStrDetail += " AND " + Journal_Voucher_Details_Voucher_Prefix + "= :voucherPrefix" ;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStrLedgers);
    query.bindValue(":id",vNo);
    qDebug()<<"294"<<qStrLedgers;
    if(!query.exec()){
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    else{
        while(query.next()){
            LedgerMasterDataModel ledger;
            ledger = ledHelper->getLedgerObjectByID(query.value(4).toString());
            ledger.crAmount=query.value(5).toFloat();
            ledger.drAmount=query.value(6).toFloat();
            //            if(query.value(6).toString().length()>0){
            //                ledger = ledHelper->getLedgerObjectByID(query.value(6).toString());
            //                ledger.drAmount=query.value(4).toFloat();
            //            }
            //            else{
            //                ledger = ledHelper->getLedgerObjectByID(query.value(5).toString());
            //                ledger.crAmount=query.value(4).toFloat();
            //            }

            //            ledger.amount = query.value(4).toFloat();

            obj->ledgersList.append(ledger);
        }
        qDebug()<<"305";
    }



    //    query.prepare(qStrDetail);
    //    query.bindValue(":vno",vNo);
    //    query.bindValue(":voucherPrefix", voucherPrefix);
    //    if(!query.exec()){
    //        qDebug()<<query.lastError();
    //        qDebug()<<query.lastQuery();
    //    }
    //    else{
    //        while(query.next()){
    //            LedgerMasterDataModel ledger;
    //            ledger = ledHelper->getLedgerObjectByID(query.value(4).toString());
    //            ledger.crAmount=query.value(5).toFloat();
    //            ledger.drAmount=query.value(6).toFloat();
    //            obj->ledgersList.append(ledger);
    //        }
    //    }
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
            obj->CreditPeriod = query.value(i++).toInt();
            obj->POCName  = query.value(i++).toString();
            obj->POCPhone = query.value(i++).toString();
            obj->POCEmail = query.value(i++).toString();
            obj->ProjectId = query.value(i++).toString();
            obj->RevisionNo = query.value(i++).toInt();
            obj->SalesmanID = query.value(i++).toInt();
            obj->currency = query.value(i++).toString();
            obj->_DisplayVoucherNumber = query.value(i++).toString();
            obj->status = query.value(i++).toInt();
            obj->Origin = query.value(i++).toInt();

        }
    }

    obj->voucherType = VoucherType;
    return obj;
}

QList<GeneralVoucherDataObject> JournalVoucherDatabaseHelper::getVoucherListByDate(QDate dateFrom, QDate dateTo)
{
    QList<GeneralVoucherDataObject> vouchersList;
    QString qStr ="SELECT "
            + Journal_Voucher_Main_Voucher_No+ ", "
            + Journal_Voucher_Main_Voucher_Prefix+ ", "
            + Journal_Voucher_Main_Voucher_Date+", "
            + Journal_Voucher_Main_Ledger_ID+ ", "
            + Journal_Voucher_Main_Total
            + " FROM " + Journal_Voucher_Main_TableName
            + " WHERE "
            + Journal_Voucher_Main_Voucher_Date
            + " >= '"
            + dateFrom.toString("yyyy-MM-dd")
            + "' AND "
            + Journal_Voucher_Main_Voucher_Date
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

QSqlQueryModel *JournalVoucherDatabaseHelper::getVoucherListByDateModel(QDate dateFrom, QDate dateTo)
{
    LedgerMasterDatabaseHelper ledHelper;
    QString qStr ="SELECT ";
    qStr += "'V' , ";
    qStr += " som." + Journal_Voucher_Main_Voucher_No +", ";
    qStr += "'" + VoucherType + "', " ;
    qStr += " som." + Journal_Voucher_Main_Voucher_Prefix + ", ";
    qStr += " som." + Journal_Voucher_Main_Voucher_Date+" as 'Voucher Date', ";
    qStr += " CONCAT(som." + Journal_Voucher_Main_Voucher_Prefix + ", '-', som." + Journal_Voucher_Main_Voucher_No+ ") as 'Voucher No', ";
    qStr += "(SELECT "+ledHelper.Ledger_Master_LedgerName + " FROM "+ ledHelper.Ledger_Master_TableName;
    qStr += " WHERE "+ledHelper.Ledger_Master_LedgerID + "= det.Ledger_Id) Ledger, ";
    qStr += " det.total " ;
    qStr += " FROM " + Journal_Voucher_Main_TableName + " som ";
    qStr += " JOIN (SELECT " + Transaction_Helper_VoucherNo + ", " + Transaction_Helper_VoucherPrefix + ", ";
    qStr += " Sum(credit_amount) as total, Ledger_Id FROM " + Transaction_Helper_TableName ;
    qStr += " WHERE " + Transaction_Helper_VoucherType + " = '" + VoucherType + "'";
    qStr += " GROUP BY " + Transaction_Helper_VoucherNo + ", " + Transaction_Helper_VoucherType + ", " + Transaction_Helper_VoucherPrefix ;
    qStr += "  ) det ON som." + Journal_Voucher_Main_Voucher_No + " = det." + Transaction_Helper_VoucherNo ;
    qStr += " AND som." + Journal_Voucher_Main_Voucher_Prefix + " = det." + Transaction_Helper_VoucherPrefix ;
    qStr += " WHERE som.";
    qStr += Journal_Voucher_Main_Voucher_Date;
    qStr += " >= '";
    qStr += dateFrom.toString("yyyy-MM-dd");
    qStr += "' AND som.";
    qStr += Journal_Voucher_Main_Voucher_Date;
    qStr += " <= '";
    qStr += dateTo.toString("yyyy-MM-dd");
    qStr += "'";

    return DatabaseHelper::getQueryModel(qStr);

}
