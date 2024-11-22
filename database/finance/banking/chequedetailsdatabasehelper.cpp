#include "chequedetailsdatabasehelper.h"

ChequeDetailsDatabaseHelper::ChequeDetailsDatabaseHelper()
{
    db = new DatabaseHelper();
}

int ChequeDetailsDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

int ChequeDetailsDatabaseHelper::commit()
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

int ChequeDetailsDatabaseHelper::insertChequeDetails(ChequeDetail *obj)
{

    qDebug()<<Q_FUNC_INFO;

    QMap<QString,QVariant> map;
    map.insert(Cheque_Details_LedgerId,obj->LedgerID);
    map.insert(Cheque_Details_VoucherNo,obj->VoucherID);
    map.insert(Cheque_Details_VoucherType,obj->VoucherType);
    map.insert(Cheque_Details_VoucherPrefix, obj->VoucherPrefix);
    map.insert(Cheque_Details_IssuedOn, obj->IssuedOn);
    map.insert(Cheque_Details_DebitAmount, obj->drAmount);
    map.insert(Cheque_Details_CreditAmount, obj->crAmount);
    map.insert(Cheque_Details_ChequeNo, obj->ChequeNo);
    map.insert(Cheque_Details_ChequeDate, obj->InstrumentDate);
    map.insert(Cheque_Details_IsPresented, obj->isPresented);
    map.insert(Cheque_Details_PresentedOn, obj->presentedOn);
    map.insert(Cheque_Details_IsCleared, obj->isCleared);
    map.insert(Cheque_Details_ClearedOn, obj->clearedOn);
    map.insert(Cheque_Details_IsReturned, obj->isRejected);
    map.insert(Cheque_Details_ReconciledOn, obj->reconciledDate);
    map.insert(Cheque_Details_Narration,obj->Narration);
    map.insert(Cheque_Details_BankName, obj->BankName);
    map.insert(Cheque_Details_BranchName, obj->Branch);

    db->insertRecordsPlus(map,Cheque_Details_TableName);


}

int ChequeDetailsDatabaseHelper::updateChequeDetails(ChequeDetail *obj, QString id)
{
    QMap<QString,QVariant> map;
    map.insert(Cheque_Details_LedgerId,obj->LedgerID);
    map.insert(Cheque_Details_VoucherNo,obj->VoucherID);
    map.insert(Cheque_Details_VoucherType,obj->VoucherType);
    map.insert(Cheque_Details_VoucherPrefix, obj->VoucherPrefix);
    map.insert(Cheque_Details_IssuedOn, obj->IssuedOn);
    map.insert(Cheque_Details_Amount, obj->Amount);
    map.insert(Cheque_Details_ChequeNo, obj->ChequeNo);
    map.insert(Cheque_Details_ChequeDate, obj->InstrumentDate);
    map.insert(Cheque_Details_IsPresented, obj->isPresented);
    map.insert(Cheque_Details_PresentedOn, obj->presentedOn);
    map.insert(Cheque_Details_IsCleared, obj->isCleared);
    map.insert(Cheque_Details_ClearedOn, obj->clearedOn);
    map.insert(Cheque_Details_IsReturned, obj->reconciled);
    map.insert(Cheque_Details_ReconciledOn, obj->reconciledDate);
    map.insert(Cheque_Details_Narration,obj->Narration);

    QMap<QString,QVariant> map1;
    map1.insert(Cheque_Details_id,id);
    db->updateRecordsPlus(map,map1,Cheque_Details_TableName);


}

int ChequeDetailsDatabaseHelper::deleteChequeDetails(QString id)
{

    startTransaction();

    QMap<QString,QVariant> map1;
    map1.insert(Cheque_Details_id,id);

    db->deleteRecordsPlus(map1,Cheque_Details_TableName);
    return commit();
}

int ChequeDetailsDatabaseHelper::deleteChequeDetailsByVoucher(ChequeDetail *obj)
{
    qDebug()<<Q_FUNC_INFO;

    QMap<QString,QVariant> map1;
    map1.insert(Cheque_Details_VoucherNo, obj->VoucherID);
    map1.insert(Cheque_Details_VoucherPrefix, obj->VoucherPrefix );
    map1.insert(Cheque_Details_VoucherType, obj->VoucherType);

    db->deleteRecordsPlus(map1,Cheque_Details_TableName);
}

QList<ChequeDetail> ChequeDetailsDatabaseHelper::getAllChequeDetails()
{

}

ChequeDetail *ChequeDetailsDatabaseHelper::getChequeById(QString id)
{
    ChequeDetail* obj = new ChequeDetail();
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    QString qStr = " SELECT "   ;
    qStr += Cheque_Details_id + ", ";
    qStr += Cheque_Details_LedgerId + ", ";
    qStr += Cheque_Details_VoucherNo + ", ";
    qStr += Cheque_Details_VoucherPrefix + ", ";
    qStr += Cheque_Details_VoucherType + ", ";
    qStr += Cheque_Details_ChequeNo + ", ";
    qStr += Cheque_Details_ChequeDate + ", ";
    qStr += Cheque_Details_BankName + ", ";
    qStr += Cheque_Details_BranchName + ", ";
    qStr += Cheque_Details_Amount + ", ";
    qStr += Cheque_Details_IssuedOn + ", ";
    qStr += Cheque_Details_IsPresented + ", ";
    qStr += Cheque_Details_PresentedOn + ", ";
    qStr += Cheque_Details_IsCleared + ", ";
    qStr += Cheque_Details_ClearedOn + ", ";
    qStr += Cheque_Details_IsReturned + ", ";
    qStr += Cheque_Details_ReconciledOn + ", ";
    qStr += Cheque_Details_Narration + " ";
    qStr += " FROM " + Cheque_Details_TableName + " WHERE " + Cheque_Details_id + " = '" + id + "'";


    if(!query.exec(qStr)){
        qDebug()<<query.lastError();
        qDebug()<<qStr;
    }

    while (query.next()) {
        int i=0;
        obj->id = query.value(i++).toString();
        obj->LedgerID = query.value(i++).toString();
        obj->VoucherID = query.value(i++).toString();
        obj->VoucherPrefix = query.value(i++).toString();
        obj->VoucherType = query.value(i++).toString();
        obj->ChequeNo = query.value(i++).toString();
        obj->InstrumentDate = query.value(i++).toDate();
        obj->BankID = query.value(i++).toString();
        obj->Branch = query.value(i++).toString();
        obj->Amount = query.value(i++).toFloat();
        obj->IssuedOn = query.value(i++).toDate();
        obj->isPresented = query.value(i++).toBool();
        obj->presentedOn = query.value(i++).toDate();
        obj->isCleared = query.value(i++).toBool();
        obj->clearedOn = query.value(i++).toDate();
        obj->reconciled = query.value(i++).toBool();
        obj->reconciledDate = query.value(i++).toDate();
        obj->Narration = query.value(i++).toString();
    }

    return obj;

}

QList<ChequeDetail> ChequeDetailsDatabaseHelper::getAllChequesByDate(QDate date)
{

}

QList<ChequeDetail> ChequeDetailsDatabaseHelper::getPendingChequesByLedgerId(QString ledgerID, QDate date)
{

}

QSqlQueryModel *ChequeDetailsDatabaseHelper::getReconciliationReport( QDate date)
{
    LedgerMasterDatabaseHelper led;
    QString qStr = " SELECT " + Cheque_Details_id + ", ";
    qStr += "( SELECT Ledger_Name FROM " + led.Ledger_Master_TableName + " WHERE Ledger_Id = chq."
            + Cheque_Details_LedgerId + ") as Ledger, ";
    qStr += "( SELECT Ledger_Name FROM " + led.Ledger_Master_TableName + " WHERE Ledger_Id = chq."
            + Cheque_Details_OurBank + ") as 'Our Bank', ";
    qStr += Cheque_Details_DebitAmount + " 'Debit Amount', ";
    qStr += Cheque_Details_CreditAmount + " 'Credit Amount', ";
    qStr += " CASE WHEN " + Cheque_Details_IsReturned + " = 1 THEN 'Rejected' ";
    qStr += " WHEN " + Cheque_Details_IsPresented + " = 1 THEN 'Presented' END AS 'Current Status', ";
    qStr += Cheque_Details_IssuedOn + " 'Issued On', ";
    qStr += Cheque_Details_PresentedOn + " 'Presented On',  ";
    qStr += " '' as 'Update Status', '' as 'Status Date' ";
    qStr += " FROM " + Cheque_Details_TableName + " chq ";
    qStr += " WHERE " + Cheque_Details_IsCleared + " <> 1";
    qStr += " AND " + Cheque_Details_IssuedOn + " <= " + db->getDateString(date);

    return DatabaseHelper::getQueryModel(qStr);


}

QString ChequeDetailsDatabaseHelper::getMaxID()
{

}
