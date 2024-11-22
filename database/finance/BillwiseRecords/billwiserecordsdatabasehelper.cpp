#include "billwiserecordsdatabasehelper.h"

BillwiseRecordsDatabaseHelper::BillwiseRecordsDatabaseHelper()
{
    db = new DatabaseHelper();
}

int BillwiseRecordsDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

int BillwiseRecordsDatabaseHelper::commit()
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

int BillwiseRecordsDatabaseHelper::insertBillwiseRecords(BillwiseRecordDataModel *obj)
{
//    startTransaction();
    qDebug()<<Q_FUNC_INFO;
    QMap<QString,QVariant> map;
    qDebug()<<"Insert billwise. voucher:"<<obj->VoucherNo;
    map.insert(Billwise_Records_VoucherPrefix, obj->VoucherPrefix);
    map.insert(Billwise_Records_VoucherType, obj->VoucherType);
    map.insert(Billwise_Records_VoucherNo, obj->VoucherNo);
    map.insert(Billwise_Records_VoucherDate, obj->VoucherDate);
    map.insert(Billwise_Records_LedgerId, obj->LedgerID);
    map.insert(Billwise_Records_CreditAmount, obj->CreditAmount);
    map.insert(Billwise_Records_DebitAmount, obj->DebitAmount);
    map.insert(Billwise_Records_RefPrefix, obj->RefPrefix);
    map.insert(Billwise_Records_RefType, obj->RefType);
    map.insert(Billwise_Records_RefNo, obj->RefVoucherNo);
    map.insert(Billwise_Records_MethodOfAdj, obj->MethodOfAdjustment);
    map.insert(Billwise_Records_DueDate, obj->DueDate);

    db->insertRecordsPlus(map,Billwise_Records_TableName);

//    return commit();

}

int BillwiseRecordsDatabaseHelper::updateBillwiseRecords(BillwiseRecordDataModel *obj, QString id)
{
//    startTransaction();
    QMap<QString,QVariant> map;

    map.insert(Billwise_Records_VoucherPrefix, obj->VoucherPrefix);
    map.insert(Billwise_Records_VoucherType, obj->VoucherType);
    map.insert(Billwise_Records_VoucherNo, obj->VoucherNo);
    map.insert(Billwise_Records_VoucherDate, obj->VoucherDate);
    map.insert(Billwise_Records_LedgerId, obj->LedgerID);
    map.insert(Billwise_Records_CreditAmount, obj->CreditAmount);
    map.insert(Billwise_Records_DebitAmount, obj->DebitAmount);
    map.insert(Billwise_Records_RefPrefix, obj->RefPrefix);
    map.insert(Billwise_Records_RefType, obj->RefType);
    map.insert(Billwise_Records_RefNo, obj->RefVoucherNo);
    map.insert(Billwise_Records_MethodOfAdj, obj->MethodOfAdjustment);
    map.insert(Billwise_Records_DueDate, obj->DueDate);
    QMap<QString,QVariant> map1;
    map1.insert(Billwise_Records_id,id);


    db->updateRecordsPlus(map,map1,Billwise_Records_TableName);
//    return commit();
}

int BillwiseRecordsDatabaseHelper::deleteBillwiseRecordsByLedger(QString ledger)
{
//    startTransaction();

    QMap<QString,QVariant> map1;
    map1.insert(Billwise_Records_LedgerId,ledger);
    map1.insert(Billwise_Records_VoucherType,"OPENING");

    db->deleteRecordsPlus(map1,Billwise_Records_TableName);
//    return commit();

}

int BillwiseRecordsDatabaseHelper::deleteBillwiseRecordsByVoucher(QString voucherNo, QString voucherType, QString voucherPrefix)
{
//    startTransaction();

    qDebug()<<"Delete billwise"<<voucherNo<<voucherType<<voucherPrefix;

    QMap<QString,QVariant> map1;
    map1.insert(Billwise_Records_VoucherNo,voucherNo);
    map1.insert(Billwise_Records_VoucherType, voucherType);
    map1.insert(Billwise_Records_VoucherPrefix, voucherPrefix);

    db->deleteRecordsPlus(map1,Billwise_Records_TableName);
//    return commit();

}

QString BillwiseRecordsDatabaseHelper::getMaxID()
{

}

QList<BillwiseRecordDataModel*> BillwiseRecordsDatabaseHelper::getAllBillwiseRecords()
{
    QList<BillwiseRecordDataModel*> data;

    QString qStr = " SELECT ";
    qStr += Billwise_Records_id + ", ";
    qStr += Billwise_Records_VoucherNo + ", ";
    qStr += Billwise_Records_VoucherType + ", ";
    qStr += Billwise_Records_VoucherPrefix + ", ";
    qStr += Billwise_Records_VoucherDate + ", ";
    qStr += Billwise_Records_LedgerId + ", ";
    qStr += Billwise_Records_RefNo + ", ";
    qStr += Billwise_Records_RefType + ", ";
    qStr += Billwise_Records_RefPrefix + ", ";
    qStr += Billwise_Records_MethodOfAdj + ", ";
    qStr += Billwise_Records_DebitAmount + ", ";
    qStr += Billwise_Records_CreditAmount + ", ";
    qStr += Billwise_Records_DueDate + " ";
    qStr += " FROM " + Billwise_Records_TableName ;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    qDebug()<<qStr;
    if(!query.exec(qStr)){
        qDebug()<<query.lastError();
    }

    while (query.next()) {
        BillwiseRecordDataModel* obj = new BillwiseRecordDataModel();
        int i=0;
        obj->_id = query.value(i++).toInt();
        obj->VoucherNo = query.value(i++).toString();
        obj->VoucherType = query.value(i++).toString();
        obj->VoucherPrefix = query.value(i++).toString();
        obj->VoucherDate = query.value(i++).toDate();
        obj->LedgerID = query.value(i++).toString();
        obj->RefVoucherNo = query.value(i++).toString();
        obj->RefType = query.value(i++).toString();
        obj->RefPrefix = query.value(i++).toString();
        obj->MethodOfAdjustment = query.value(i++).toString();
        obj->DebitAmount = query.value(i++).toFloat();
        obj->CreditAmount = query.value(i++).toFloat();
        obj->DueDate = query.value(i++).toDate();
        data.append(obj);
    }
    return data;

}

QList<BillwiseRecordDataModel*> BillwiseRecordsDatabaseHelper::getAllBillwiseRecordsByLedger(QString ledger)
{
    QList<BillwiseRecordDataModel*> data;

    QString qStr = " SELECT ";
    qStr += Billwise_Records_id + ", ";
    qStr += Billwise_Records_VoucherNo + ", ";
    qStr += Billwise_Records_VoucherType + ", ";
    qStr += Billwise_Records_VoucherPrefix + ", ";
    qStr += Billwise_Records_VoucherDate + ", ";
    qStr += Billwise_Records_LedgerId + ", ";
    qStr += Billwise_Records_RefNo + ", ";
    qStr += Billwise_Records_RefType + ", ";
    qStr += Billwise_Records_RefPrefix + ", ";
    qStr += Billwise_Records_MethodOfAdj + ", ";
    qStr += Billwise_Records_DebitAmount + ", ";
    qStr += Billwise_Records_CreditAmount + ", ";
    qStr += Billwise_Records_DueDate + " ";
    qStr += " FROM " + Billwise_Records_TableName ;
    qStr += " WHERE " + Billwise_Records_LedgerId + " = '" + ledger + "'";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    qDebug()<<qStr;
    if(!query.exec(qStr)){
        qDebug()<<query.lastError();
    }

    while (query.next()) {
        BillwiseRecordDataModel* obj = new BillwiseRecordDataModel();
        int i=0;
        obj->_id = query.value(i++).toInt();
        obj->VoucherNo = query.value(i++).toString();
        obj->VoucherType = query.value(i++).toString();
        obj->VoucherPrefix = query.value(i++).toString();
        obj->VoucherDate = query.value(i++).toDate();
        obj->LedgerID = query.value(i++).toString();
        obj->RefVoucherNo = query.value(i++).toString();
        obj->RefType = query.value(i++).toString();
        obj->RefPrefix = query.value(i++).toString();
        obj->MethodOfAdjustment = query.value(i++).toString();
        obj->DebitAmount = query.value(i++).toFloat();
        obj->CreditAmount = query.value(i++).toFloat();
        obj->DueDate = query.value(i++).toDate();
        data.append(obj);
    }
    return data;

}

QList<BillwiseRecordDataModel*> BillwiseRecordsDatabaseHelper::getPendingBillsByLedger(QString ledger, QDate date)
{
    QList<BillwiseRecordDataModel*> data;

//    QString qStr = "SELECT " ;

//    qStr += "Main." + Billwise_Records_id + ", ";
//    qStr += "Main." + Billwise_Records_VoucherNo + ", ";
//    qStr += "Main." + Billwise_Records_VoucherType + ", ";
//    qStr += "Main." + Billwise_Records_VoucherPrefix + ", ";
//    qStr += "Main." + Billwise_Records_VoucherDate + ", ";
//    qStr += "Main." + Billwise_Records_LedgerId + ", ";
//    qStr += "Main." + Billwise_Records_RefNo + ", ";
//    qStr += "Main." + Billwise_Records_RefType + ", ";
//    qStr += "Main." + Billwise_Records_RefPrefix + ", ";
//    qStr += "Main." + Billwise_Records_MethodOfAdj + ", ";
//    qStr += "coalesce(Main." + Billwise_Records_CreditAmount + ",0) - sum(coalesce(Ref." + Billwise_Records_DebitAmount + ",0)), ";
//    qStr += "coalesce(Main." + Billwise_Records_DebitAmount + ",0) - sum(coalesce(Ref." + Billwise_Records_CreditAmount + ",0)), ";
//    qStr += "Main." + Billwise_Records_DueDate + ", ";
//    qStr+= "Main." + Billwise_Records_CreditAmount + ", ";
//    qStr += "Main." + Billwise_Records_DebitAmount + " ";
//    qStr += "FROM " + Billwise_Records_TableName + " Main LEFT OUTER JOIN " ;
//    qStr += Billwise_Records_TableName + " REF ";
//    qStr += " ON Main." + Billwise_Records_VoucherNo + " = Ref." + Billwise_Records_RefNo;
//    qStr += " AND Main." + Billwise_Records_id + " <> Ref." + Billwise_Records_id;
//    qStr +=  " AND Main." + Billwise_Records_VoucherPrefix + " = Ref." + Billwise_Records_VoucherPrefix;
//    qStr +=  " AND Main." + Billwise_Records_VoucherType + " = Ref." + Billwise_Records_VoucherType;
//    qStr += " WHERE Main." + Billwise_Records_LedgerId + " = '" + ledger + "'";
//    qStr += " AND Main." + Billwise_Records_VoucherDate + " <= '" + date.toString("yyyy-MM-dd") + "' ";
//    qStr += " AND (Ref." + Billwise_Records_VoucherDate + " <= '" + date.toString("yyyy-MM-dd") + "' ";
//    qStr += " OR Ref." + Billwise_Records_VoucherDate + " is null) ";
//    qStr += " AND (Ref." + Billwise_Records_LedgerId + " = Main." + Billwise_Records_LedgerId;
//    qStr += " OR Ref." + Billwise_Records_LedgerId + " is null)";

//    qStr += " AND Main." + Billwise_Records_RefNo + " is null ";
//    qStr += " GROUP BY Main." + Billwise_Records_VoucherNo;
//    qStr += " HAVING (coalesce(Main." + Billwise_Records_DebitAmount + ",0) + ";
//    qStr += " coalesce(Main." +  Billwise_Records_CreditAmount + ",0)) - ";
//    qStr += " (sum(coalesce(Ref." + Billwise_Records_DebitAmount + ",0)) + ";
//    qStr += " sum(coalesce(Ref." +  Billwise_Records_CreditAmount + ",0))) >0 ";

    QString qStr = " SELECT ";
    qStr += Billwise_Records_RefNo + ", ";
    qStr += Billwise_Records_RefPrefix + ", ";
    qStr += Billwise_Records_RefType + ", ";
    qStr += " min(" + Billwise_Records_VoucherDate + ") as voucherDate, ";
    qStr += " SUM(COALESCE(" + Billwise_Records_CreditAmount + ",0)) - ";
    qStr += " SUM(COALESCE(" + Billwise_Records_DebitAmount + ",0)) ";
    qStr += " FROM " + Billwise_Records_TableName + " Ref ";
    qStr += " WHERE Ref." + Billwise_Records_LedgerId + " = '" + ledger + "'";
    qStr += " AND Ref." + Billwise_Records_VoucherDate + " <= '" + date.toString("yyyy-MM-dd") + "'";
    qStr += " AND (" + Billwise_Records_MethodOfAdj + "<> 'ON ACC' OR " + Billwise_Records_MethodOfAdj + " IS NULL)";
    qStr += " GROUP BY " + Billwise_Records_RefNo + ", " + Billwise_Records_RefType + ", " + Billwise_Records_RefPrefix;
    qStr += " HAVING SUM(COALESCE(" + Billwise_Records_CreditAmount + ",0)) - ";
    qStr += "SUM(COALESCE(" + Billwise_Records_DebitAmount + ",0)) <> 0";
    qStr += " UNION SELECT '', '', 'ON ACCOUNT',";
    qStr += " max(" + Billwise_Records_VoucherDate + ") as voucherDate, ";
    qStr += " SUM(COALESCE(" + Billwise_Records_CreditAmount + ",0)) - ";
    qStr += " SUM(COALESCE(" + Billwise_Records_DebitAmount + ",0)) ";
    qStr += " FROM " + Billwise_Records_TableName + " Ref ";
    qStr += " WHERE Ref." + Billwise_Records_LedgerId + " = '" + ledger + "'";
    qStr += " AND Ref." + Billwise_Records_VoucherDate + " <= '" + date.toString("yyyy-MM-dd") + "'";
    qStr += " AND " + Billwise_Records_MethodOfAdj + " = 'ON ACC' ";
    qStr += " GROUP BY Ref." + Billwise_Records_LedgerId;
    qStr += " HAVING SUM(COALESCE(" + Billwise_Records_CreditAmount + ",0)) - ";
    qStr += " SUM(COALESCE(" + Billwise_Records_DebitAmount + ",0)) <> 0";
    qStr += " ORDER BY 4 ";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    qDebug()<<qStr;
    if(!query.exec(qStr)){
        qDebug()<<query.lastError();
    }

    while (query.next()) {
        BillwiseRecordDataModel* obj = new BillwiseRecordDataModel;
        int i=0;
//        obj->_id = query.value(i++).toInt();
        obj->VoucherNo = query.value(i++).toString();
        obj->VoucherPrefix = query.value(i++).toString();
        obj->VoucherType = query.value(i++).toString();
        obj->VoucherDate = query.value(i++).toDate();
        obj->Amount= query.value(i++).toFloat();
//        obj->LedgerID = query.value(i++).toString();
//        obj->RefVoucherNo = query.value(i++).toString();
//        obj->RefType = query.value(i++).toString();
//        obj->RefPrefix = query.value(i++).toString();
//        obj->MethodOfAdjustment = query.value(i++).toString();
//        obj->DebitAmount = query.value(i++).toFloat();
//        obj->CreditAmount = query.value(i++).toFloat();
//        obj->DueDate = query.value(i++).toDate();

//        if(obj->CreditAmount > 0)
//            obj->Amount=obj->CreditAmount;
//        else
//            obj->Amount=-1*obj->DebitAmount;
        data.append(obj);
    }
    return data;

}

QSqlQueryModel *BillwiseRecordsDatabaseHelper::getPendingBillsModelByLedger(QString ledger, GeneralVoucherDataObject *voucher)
{
    QString qStr = " SELECT min(VoucherDate), RefNo, RefType, RefPrefix, ";
    qStr += " LedgerID, round(Sum(Amount),2), round(Sum(SelectedAmount),2),  max(Method_of_adj) as  Method_of_adj ";
    qStr += " FROM ( SELECT VoucherDate, ";
    qStr += " case when Method_of_adj = 'ON ACC' then '' else RefNo end as RefNo, ";
    qStr += " case when Method_of_adj = 'ON ACC' then 'On Account' else RefType end as RefType, ";
    qStr += " case when Method_of_adj = 'ON ACC' then '' else RefPrefix end as RefPrefix, ";
    qStr += " LedgerID, case when VoucherType = '" + voucher->voucherType + "' ";
    qStr += " and VoucherNo = '" + voucher->voucherNumber + "' ";
    qStr += " and voucherPrefix = '" + voucher->VoucherPrefix + "' ";
//    qStr += " and Method_Of_Adj <> 'NEW REF' ";
    qStr += " then 0 else CreditAmount - DebitAmount end as Amount, ";
    qStr += " case when VoucherType = '" + voucher->voucherType + "' ";
    qStr += " and VoucherNo = '" + voucher->voucherNumber + "' ";
    qStr += " and voucherPrefix = '" + voucher->VoucherPrefix + "' ";
    qStr += " then CreditAmount - DebitAmount else 0 end as SelectedAmount ,";
    qStr += " case when VoucherType = '" + voucher->voucherType + "' ";
    qStr += " and VoucherNo = '" + voucher->voucherNumber + "' ";
    qStr += " and voucherPrefix = '" + voucher->VoucherPrefix + "' ";
    qStr += " then Method_of_adj else '' end as Method_of_adj FROM `billwise_records` ";
    qStr += " where VoucherDate <= " + db->getDateString(voucher->VoucherDate);
    qStr += " and LedgerID = '" + ledger + "' ) X ";
    qStr += " GROUP BY RefNo, RefType, RefPrefix ";
    qStr += " HAVING round(Sum(Amount),2) <> 0 OR Method_Of_Adj = 'New Ref' ";
    qStr += " ORDER BY 7, 1 ";

    return db->getQueryModel(qStr);
}

QList<BillwiseRecordDataModel*> BillwiseRecordsDatabaseHelper::getOpeningBalanceMapping(QString ledger)
{
    qDebug()<<"Get opening balance for ledger : "<<ledger;
    QList<BillwiseRecordDataModel*> data;

    QString qStr = " SELECT ";
    qStr += Billwise_Records_id + ", ";
    qStr += Billwise_Records_VoucherNo + ", ";
    qStr += Billwise_Records_VoucherType + ", ";
    qStr += Billwise_Records_VoucherPrefix + ", ";
    qStr += Billwise_Records_VoucherDate + ", ";
    qStr += Billwise_Records_LedgerId + ", ";
    qStr += Billwise_Records_RefNo + ", ";
    qStr += Billwise_Records_RefType + ", ";
    qStr += Billwise_Records_RefPrefix + ", ";
    qStr += Billwise_Records_MethodOfAdj + ", ";
    qStr += Billwise_Records_DebitAmount + ", ";
    qStr += Billwise_Records_CreditAmount + ", ";
    qStr += Billwise_Records_DueDate + " ";
    qStr += " FROM " + Billwise_Records_TableName ;
    qStr += " WHERE " + Billwise_Records_LedgerId + " = '" + ledger + "'";
    qStr += " AND " + Billwise_Records_VoucherType + " = 'OPENING'";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    qDebug()<<qStr;
    if(!query.exec(qStr)){
        qDebug()<<query.lastError();
    }

    while (query.next()) {
        BillwiseRecordDataModel* obj = new BillwiseRecordDataModel;
        int i=0;
        obj->_id = query.value(i++).toInt();
        obj->VoucherNo = query.value(i++).toString();
        obj->VoucherType = query.value(i++).toString();
        obj->VoucherPrefix = query.value(i++).toString();
        obj->VoucherDate = query.value(i++).toDate();
        obj->LedgerID = query.value(i++).toString();
        obj->RefVoucherNo = query.value(i++).toString();
        obj->RefType = query.value(i++).toString();
        obj->RefPrefix = query.value(i++).toString();
        obj->MethodOfAdjustment = query.value(i++).toString();
        obj->DebitAmount = query.value(i++).toFloat();
        obj->CreditAmount = query.value(i++).toFloat();
        obj->DueDate = query.value(i++).toDate();



        if(obj->CreditAmount > 0)
            obj->Amount=obj->CreditAmount;
        else
            obj->Amount=-1*obj->DebitAmount;
        obj->selectedAmount = obj->Amount;
        qDebug()<<"Data appended for opening balance";
        data.append(obj);
    }
    qDebug()<<"DEBUG"<<data.size();
    return data;

}

QList<BillwiseRecordDataModel*> BillwiseRecordsDatabaseHelper::getBillMappingsByLedger(QString ledgerId, QString voucherNo, QDate date1)
{
    qDebug()<<Q_FUNC_INFO;
    QList<BillwiseRecordDataModel*> data;

    QString qStr = " SELECT ";
    qStr += Billwise_Records_id + ", ";
    qStr += Billwise_Records_VoucherNo + ", ";
    qStr += Billwise_Records_VoucherType + ", ";
    qStr += Billwise_Records_VoucherPrefix + ", ";
    qStr += Billwise_Records_VoucherDate + ", ";
    qStr += Billwise_Records_LedgerId + ", ";
    qStr += Billwise_Records_RefNo + ", ";
    qStr += Billwise_Records_RefType + ", ";
    qStr += Billwise_Records_RefPrefix + ", ";
    qStr += Billwise_Records_MethodOfAdj + ", ";
    qStr += "coalesce(" +Billwise_Records_DebitAmount + ",0), ";
    qStr += "coalesce(" + Billwise_Records_CreditAmount + ",0), ";
    qStr += Billwise_Records_DueDate + " ";
    qStr += " FROM " + Billwise_Records_TableName ;
    qStr += " WHERE " + Billwise_Records_LedgerId + " = '" + ledgerId + "'";
    qStr += " AND " + Billwise_Records_VoucherDate + " <= '" + date1.toString("yyyy-MM-dd") + "' ";
    qStr += " AND ((" + Billwise_Records_VoucherNo + " = '" + voucherNo + "'";
    qStr += " AND " + Billwise_Records_RefPrefix + " is NOT NULL ";
    qStr += " AND " + Billwise_Records_RefType + " is NOT NULL) ";
    qStr += " )";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    qDebug()<<qStr;
    if(!query.exec(qStr)){
        qDebug()<<query.lastError();
    }

    while (query.next()) {
        BillwiseRecordDataModel *obj = new BillwiseRecordDataModel;
        int i=0;
        obj->_id = query.value(i++).toInt();
        obj->VoucherNo = query.value(i++).toString();
        obj->VoucherType = query.value(i++).toString();
        obj->VoucherPrefix = query.value(i++).toString();
        obj->VoucherDate = query.value(i++).toDate();
        obj->LedgerID = query.value(i++).toString();
        obj->RefVoucherNo = query.value(i++).toString();
        obj->RefType = query.value(i++).toString();
        obj->RefPrefix = query.value(i++).toString();
        obj->MethodOfAdjustment = query.value(i++).toString();
        obj->DebitAmount = query.value(i++).toFloat();
        obj->CreditAmount = query.value(i++).toFloat();
        obj->DueDate = query.value(i++).toDate();
        if(obj->DebitAmount>0)
            obj->Amount=-1*obj->DebitAmount;
        else
            obj->Amount=obj->CreditAmount;
        qDebug()<<"Query amount"<<obj->Amount;

        data.append(obj);
    }
    return data;

}

QSqlQueryModel *BillwiseRecordsDatabaseHelper::getBillwiseMappingByVoucher(QString voucherNo, QString voucherType, QString voucherPrefix)
{
    QString qStr = " SELECT VoucherDate, VoucherNo, VoucherType, voucherPrefix, ";
    qStr += " (select ledger_name from ledger_master where ledger_id in ";
    qStr += " (SELECT Ledger_id from transaction_helper where voucher_Id = voucherNo and ";
    qStr += " voucher_Prefix = voucherPrefix and voucher_type = VoucherType)  limit 1) as ModeOfPay, ";
    qStr += " round(CreditAmount- DebitAmount,2), 0 As selectedAmount, Method_of_adj   from billwise_records ";
    qStr += " where RefNo = " + voucherNo + " and RefType = '" + voucherType + "' and RefPrefix = '" + voucherPrefix +"'";
    qStr += " and not (RefNo = VoucherNo and RefType = VoucherType and RefPrefix = voucherPrefix)";

    return DatabaseHelper::getQueryModel(qStr);

}
