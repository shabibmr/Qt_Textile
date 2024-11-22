#include "shiftdatabasehelper.h"

ShiftDatabaseHelper::ShiftDatabaseHelper()
{
    db = new DatabaseHelper();
    trans = new TransactionHelper();
}

int ShiftDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

int ShiftDatabaseHelper::insertShiftData(ShiftDataModel *shiftData)
{
    startTransaction();
    QMap<QString,QVariant> valuesMap;

    valuesMap.insert(Shift_StartVoucher_No,shiftData->BillsFrom);
    valuesMap.insert(Shift_Stopvoucher_No,shiftData->BillsTo);
    valuesMap.insert(Shift_ShiftTimeStamp,QDateTime::currentDateTime());
    valuesMap.insert(Shift_OpeningBalance,shiftData->OpeningCash);
    valuesMap.insert(Shift_SalesValue,shiftData->SalesTotal);
    valuesMap.insert(Shift_CashSales,shiftData->CashSales);
    valuesMap.insert(Shift_CardSales,shiftData->CardSales);
    valuesMap.insert(Shift_CreditSales,shiftData->CreditSales);
    valuesMap.insert(Shift_ZomatoSales,shiftData->ZomatoSales);
    valuesMap.insert(Shift_ZomatoCash,shiftData->ZomatoCash);
    valuesMap.insert(Shift_ZomatoCredit,shiftData->ZomatoCredit);
    valuesMap.insert(Shift_PurchaseTotal,shiftData->PurchaseTotal);
    valuesMap.insert(Shift_PurchaseCash,shiftData->CashPurchase);
    valuesMap.insert(Shift_PaymentCash,shiftData->CashPayments);
    valuesMap.insert(Shift_ReceiptCash,shiftData->CashReceipts);

    valuesMap.insert(Shift_cashierID,shiftData->cashierID);
    valuesMap.insert(Shift_voucherPrefix,shiftData->voucherPrefix);
    valuesMap.insert(Shift_MacId,shiftData->MaciD);
    valuesMap.insert(Shift_POSNumber,shiftData->POSNumber);
    valuesMap.insert(Shift_paxCount,shiftData->PaxCount);
    valuesMap.insert(Shift_Narration,shiftData->Narration);
    valuesMap.insert(Shift_Start_DateTime,shiftData->ShiftStartDateTime);
    valuesMap.insert(Shift_End_DateTime,shiftData->ShiftEndDateTime);

    valuesMap.insert(Shift_TotalCash,shiftData->totalCash);
    valuesMap.insert(Shift_CasinCounter,shiftData->CashInCounter);
    valuesMap.insert(Shift_CashWithDrawn,shiftData->CashWithDrawn);
    valuesMap.insert(Shift_CashBalance,shiftData->CashBalance);
    valuesMap.insert(Shift_TillDifference,shiftData->TillDifference);

    int id = db->insertRecordsPlus(valuesMap,Shifts_TableName);
    commit();
    return id;
}

void ShiftDatabaseHelper::updateShiftData(ShiftDataModel *shiftData, int shiftID)
{
    QMap<QString,QVariant> valuesMap;

    valuesMap.insert(Shift_StartVoucher_No,shiftData->BillsFrom);
    valuesMap.insert(Shift_Stopvoucher_No,shiftData->BillsTo);
    valuesMap.insert(Shift_ShiftTimeStamp,QDateTime::currentDateTime());
    valuesMap.insert(Shift_SalesValue,shiftData->SalesTotal);
    valuesMap.insert(Shift_CashSales,shiftData->CashSales);
    valuesMap.insert(Shift_CardSales,shiftData->CardSales);
    valuesMap.insert(Shift_CreditSales,shiftData->CreditSales);
    valuesMap.insert(Shift_ZomatoSales,shiftData->ZomatoSales);
    valuesMap.insert(Shift_ZomatoCash,shiftData->ZomatoCash);
    valuesMap.insert(Shift_ZomatoCredit,shiftData->ZomatoCredit);
    valuesMap.insert(Shift_PurchaseTotal,shiftData->PurchaseTotal);
    valuesMap.insert(Shift_PurchaseCash,shiftData->CashPurchase);
    valuesMap.insert(Shift_PaymentCash,shiftData->CashPayments);
    valuesMap.insert(Shift_ReceiptCash,shiftData->CashReceipts);
    valuesMap.insert(Shift_OpeningBalance,shiftData->OpeningCash);

    valuesMap.insert(Shift_cashierID,shiftData->cashierID);
    valuesMap.insert(Shift_voucherPrefix,shiftData->voucherPrefix);
    valuesMap.insert(Shift_MacId,shiftData->MaciD);
    valuesMap.insert(Shift_POSNumber,shiftData->POSNumber);
    valuesMap.insert(Shift_paxCount,shiftData->PaxCount);
    valuesMap.insert(Shift_Narration,shiftData->Narration);
    valuesMap.insert(Shift_Start_DateTime,shiftData->ShiftStartDateTime);
    valuesMap.insert(Shift_End_DateTime,shiftData->ShiftEndDateTime);

    valuesMap.insert(Shift_TotalCash,shiftData->totalCash);
    valuesMap.insert(Shift_CasinCounter,shiftData->CashInCounter);
    valuesMap.insert(Shift_CashWithDrawn,shiftData->CashWithDrawn);
    valuesMap.insert(Shift_CashBalance,shiftData->CashBalance);
    valuesMap.insert(Shift_TillDifference,shiftData->TillDifference);

    startTransaction();
    QMap<QString,QVariant> clauseMap;
    clauseMap.insert(Shift_ID,shiftID);
    db->updateRecordsPlus(valuesMap,clauseMap,Shifts_TableName);
    commit();
}

void ShiftDatabaseHelper::deleteShift(int shiftID)
{
    startTransaction();
    QMap<QString,QVariant> clauseMap;
    clauseMap.insert(Shift_ID,shiftID);
    db->deleteRecordsPlus(clauseMap,Shifts_TableName);
    commit();
}

QString ShiftDatabaseHelper::getMaxID()
{

}

int ShiftDatabaseHelper::commit()
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

QString ShiftDatabaseHelper::getFirstBillinShift(ShiftDataModel *shift)
{
    QString vNO;
    QString qStr ="SELECT MAX(`"+Shift_Stopvoucher_No+"`) FROM "+Shifts_TableName
            + " where "+Shift_voucherPrefix +"=:pref" ;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);

    query.bindValue(":pref",shift->voucherPrefix);
    if(query.exec()){
        while(query.next()){
            vNO = QString::number(query.value(0).toInt()+1);
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return vNO;
}

QDateTime ShiftDatabaseHelper::getVoucherDateTimebyBill(QString vno)
{
    QDateTime vNO;
    salesVoucherDatabaseHelper shelper;
    QString qStr ="SELECT "+shelper.Sales_Invoice_Main_TimeStamp+" FROM "+shelper.Sales_Invoice_Main_TableName
            + " where ";
    qStr += shelper.Sales_Invoice_Main_Voucher_No+ "=:Vno";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);

    query.bindValue(":Vno",vno);
    if(query.exec()){
        while(query.next()){
            vNO = query.value(0).toDateTime();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return vNO;
}

QString ShiftDatabaseHelper::getFirstBillByDate(ShiftDataModel *shift)
{
    QString vNO;
    salesVoucherDatabaseHelper shelper;
    QString timestamp = "str_to_Date(concat(stable." + salesVoucherDatabaseHelper::Sales_Invoice_Main_Voucher_Date ;
    timestamp += ", substr(" + salesVoucherDatabaseHelper::Sales_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;

    QString qStr ="SELECT MAX(CAST(`"+shelper.Sales_Invoice_Main_Voucher_No+"` AS UNSIGNED)) FROM "+shelper.Sales_Invoice_Main_TableName
            + " stable where ";
    qStr += timestamp +"<"+ db->getDateTimeString(shift->ShiftStartDateTime);

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);
    qDebug()<<qStr;

    query.bindValue(":pref",shift->voucherPrefix);
    if(query.exec()){
        while(query.next()){
            vNO = QString::number(query.value(0).toInt()+1);
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return vNO;
}

QString ShiftDatabaseHelper::getLastBillByDate(ShiftDataModel *shift)
{
    QString vNO;
    salesVoucherDatabaseHelper shelper;
    QString timestamp = "str_to_Date(concat(stable." + salesVoucherDatabaseHelper::Sales_Invoice_Main_Voucher_Date ;
    timestamp += ", substr(" + salesVoucherDatabaseHelper::Sales_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;

    QString qStr ="SELECT MAX(CAST(`"+shelper.Sales_Invoice_Main_Voucher_No+"` AS UNSIGNED)) FROM "+shelper.Sales_Invoice_Main_TableName
            + " stable where ";
//    qStr += shelper.Sales_Invoice_Main_TimeStamp +"<'"+shift->ShiftEndDateTime.toString("yyyy-MM-dd hh:mm:ss")+"'";
    qStr += timestamp +"<"+ db->getDateTimeString(shift->ShiftEndDateTime);

    qDebug()<<qStr;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);
//    qDebug()<<"get last bill"<<qStr;
    query.bindValue(":pref",shift->voucherPrefix);
    if(query.exec()){
        while(query.next()){
            vNO = QString::number(query.value(0).toInt());
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return vNO;
}


QString ShiftDatabaseHelper::getLastBill(ShiftDataModel *shift)
{
    QString vNO;
    salesVoucherDatabaseHelper shelper;
    QString qStr ="select max(cast("+shelper.Sales_Invoice_Main_Voucher_No+" as unsigned)) from "+shelper.Sales_Invoice_Main_TableName;
    qStr += " where "+shelper.Sales_Invoice_Main_Voucher_Prefix +"=:pref";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);

    query.bindValue(":pref",shift->voucherPrefix);

    if(query.exec()){
        while(query.next()){
            vNO = query.value(0).toString();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return vNO;
}

void ShiftDatabaseHelper::truncateKotNumberTable()
{
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.exec("TRUNCATE TABLE `kotnumbertable`");

}

int ShiftDatabaseHelper::getBillsCount(ShiftDataModel *shift)
{
    int vNO;

    QString qStr ="select count('"+shelper.Sales_Invoice_Main_Voucher_No+"') from "+shelper.Sales_Invoice_Main_TableName;
    qStr += " where "+shelper.Sales_Invoice_Main_Voucher_Prefix +"=:pref";
    qStr += " and CAST("+shelper.Sales_Invoice_Main_Voucher_No +" AS UNSIGNED) >=:bfrom";
    qStr += " and CAST("+shelper.Sales_Invoice_Main_Voucher_No +" AS UNSIGNED) <=:bto";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));


    query.prepare(qStr);
    query.bindValue(":pref",shift->voucherPrefix);
    query.bindValue(":bfrom",shift->BillsFrom);
    query.bindValue(":bto",shift->BillsTo);


    if(query.exec()){
        while(query.next()){
            vNO = query.value(0).toInt();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return vNO;
}

float ShiftDatabaseHelper::getOpeningCash(ShiftDataModel *shift)
{
    float vNO =0;
    QString qStr = "";
    QString cashid = shift->cashID;
    LedgerMasterDatabaseHelper led;
    TransactionHelper trans;
    qStr += "SELECT ";

    qStr += led.Ledger_Master_openingBalance +"-";
    qStr += "(SELECT COALESCE(SUM("+trans.Transaction_Helper_Credit_Amount +"-";
    qStr += trans.Transaction_Helper_Debit_Amount +"),0)";
    qStr += " FROM "+ trans.Transaction_Helper_TableName + " WHERE ";
    qStr += trans.Transaction_Helper_LedgerID + "='"+cashid+"'";
    //    qStr += " AND ";
    //    qStr += trans.Transaction_Helper_VoucherDate + ">='"+shift->ShiftStartDateTime.toString("yyyy-MM-dd")+"'";
    qStr += " AND ";
    qStr += trans.Transaction_Helper_VoucherDate + "<'"+shift->ShiftStartDateTime.toString("yyyy-MM-dd")+"'";
    //    qStr += " AND ";
    //    qStr += "DATE_FORMAT("+trans.Transaction_Helper_TimeStamp+",'%H:%i:%s') "+ ">='"+shift->ShiftStartDateTime.toString("hh:mm:ss")+"'";
    qStr += " OR (";
    qStr += trans.Transaction_Helper_VoucherDate + "='"+shift->ShiftStartDateTime.toString("yyyy-MM-dd")+"'";
    qStr += " AND ";
    qStr += "DATE_FORMAT("+trans.Transaction_Helper_TimeStamp+",'%H:%i:%s')" + "<='"+shift->ShiftStartDateTime.toString("hh:mm:ss")+"'";
    qStr += ")";

    qStr += ")";
    qStr += " from ";
    qStr += led.Ledger_Master_TableName;
    qStr +=  " where ";
    qStr += led.Ledger_Master_LedgerID + "='"+cashid+"'";

    qDebug()<<"Opening Cash";
    qDebug()<<qStr;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);

    if(query.exec()){
        while(query.next()){
            vNO = query.value(0).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return -vNO;
}

float ShiftDatabaseHelper::getSalesTotal(ShiftDataModel *shift)
{
    float vNO;

    QString qStr ="SELECT SUM("+shelper.Sales_Invoice_Main_Total+") FROM ";
    qStr += shelper.Sales_Invoice_Main_TableName + " WHERE ";
    qStr += shelper.Sales_Invoice_Main_Voucher_Prefix + "='"+ shift->voucherPrefix+"'";
    qStr += " and ";
    qStr += "CAST("+shelper.Sales_Invoice_Main_Voucher_No+" AS UNSIGNED)" + ">= CAST(" + shift->BillsFrom +" AS UNSIGNED)";
    qStr += " and ";
    qStr += "CAST("+shelper.Sales_Invoice_Main_Voucher_No+" AS UNSIGNED)" + "<= CAST(" + shift->BillsTo+ " AS UNSIGNED)";
//    qStr += " and "+shelper.Sales_Invoice_Main_Voucher_Prefix +"='"+shift->voucherPrefix+"'";


    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    qDebug()<<qStr;

    query.prepare(qStr);

    if(query.exec()){
        while(query.next()){
            vNO = query.value(0).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return vNO;
}

float ShiftDatabaseHelper::getVatTotal(ShiftDataModel *shift)
{
    float vNO;

    QString qStr ="SELECT SUM("+shelper.Sales_Invoice_Main_VATAmount+") FROM ";
    qStr += shelper.Sales_Invoice_Main_TableName + " WHERE ";
    qStr += shelper.Sales_Invoice_Main_Voucher_Prefix + "='"+ shift->voucherPrefix+"'";
    qStr += " and ";
    qStr += "CAST("+shelper.Sales_Invoice_Main_Voucher_No+" AS UNSIGNED)"  + ">= CAST(" + shift->BillsFrom +" AS UNSIGNED)";
    qStr += " and ";
    qStr += "CAST("+shelper.Sales_Invoice_Main_Voucher_No+" AS UNSIGNED)" + "<= CAST(" + shift->BillsTo +" AS UNSIGNED)";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    //    qDebug()<<qStr;

    query.prepare(qStr);

    if(query.exec()){
        while(query.next()){
            vNO = query.value(0).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return vNO;
}

float ShiftDatabaseHelper::getInputVatTotal(ShiftDataModel *shift)
{
    float vNO;

    PurchaseVoucherDatabaseHelper shelper;
    QString timestamp = shelper.Purchase_Invoice_Main_Voucher_Date;
    QString qStr ="SELECT SUM("+shelper.Purchase_Invoice_Main_VATAmount+") FROM ";
    qStr += shelper.Purchase_Invoice_Main_TableName + " WHERE ";
    qStr += " ( ";
    qStr += timestamp + ">='" + shift->ShiftStartDateTime.toString("yyyy-MM-dd") + "'";
    qStr += " and ";
    qStr += timestamp + "<='" + shift->ShiftEndDateTime.toString("yyyy-MM-dd" ) + "'";
    qStr += ")";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    //    qDebug()<<qStr;

    query.prepare(qStr);

    if(query.exec()){
        while(query.next()){
            vNO = query.value(0).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return vNO;
}

float ShiftDatabaseHelper::getSalesByCash(ShiftDataModel *shift)
{
    float vNO;
    QString qStr ="SELECT SUM("+shelper.Sales_Invoice_Main_Total+") FROM ";
    qStr += shelper.Sales_Invoice_Main_TableName + " WHERE ";
    qStr += shelper.Sales_Invoice_Main_Voucher_Prefix + "='"+ shift->voucherPrefix+"'";
    qStr += " and "+shelper.Sales_Invoice_Main_Ledger_ID+ "='"+ shift->cashID+"'";
    qStr += " and ";
    qStr += "CAST("+shelper.Sales_Invoice_Main_Voucher_No+" AS UNSIGNED)"  + ">= CAST(" + shift->BillsFrom +" AS UNSIGNED)";
    qStr += " and ";
    qStr += "CAST("+shelper.Sales_Invoice_Main_Voucher_No+" AS UNSIGNED)"  + "<= CAST(" + shift->BillsTo +" AS UNSIGNED)";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);

    if(query.exec()){
        while(query.next()){
            vNO = query.value(0).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return vNO;
}

float ShiftDatabaseHelper::getSalesByCredit(ShiftDataModel *shift)
{
    float vNO=0;
    QString sundryDebtorsId = "%";
    AccountGroupMasterDatabaseHelper *acchelper = new AccountGroupMasterDatabaseHelper;
    sundryDebtorsId = acchelper->getGroupIDByName("Sundry Debtors") +"x%";

    QString qStr ="SELECT SUM("+shelper.Sales_Invoice_Main_Total+") FROM ";
    qStr += shelper.Sales_Invoice_Main_TableName + " WHERE ";
    qStr += shelper.Sales_Invoice_Main_Voucher_Prefix + " like '"+ shift->voucherPrefix+"'";
    qStr += " and "+shelper.Sales_Invoice_Main_Ledger_ID+ " like '"+ sundryDebtorsId+"'";
    qStr += " and ";
    qStr += "CAST("+shelper.Sales_Invoice_Main_Voucher_No+" AS UNSIGNED)"  + ">= CAST(" + shift->BillsFrom +" AS UNSIGNED)";
    qStr += " and ";
    qStr += "CAST("+shelper.Sales_Invoice_Main_Voucher_No+" AS UNSIGNED)"  + "<= CAST(" + shift->BillsTo +" AS UNSIGNED)";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    //    qDebug()<<"Sales Credit";
    query.prepare(qStr);

    if(query.exec()){
        while(query.next()){
            vNO = query.value(0).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return vNO;
}

float ShiftDatabaseHelper::getSalesByCard(ShiftDataModel *shift)
{
    float vNO;
    QString BankAccountsId = "";
    AccountGroupMasterDatabaseHelper *acchelper = new AccountGroupMasterDatabaseHelper;
    BankAccountsId = acchelper->getGroupIDByName("Bank Accounts") +"x%";
    QString qStr ="SELECT SUM("+shelper.Sales_Invoice_Main_Total+") FROM ";
    qStr += shelper.Sales_Invoice_Main_TableName + " WHERE ";
    qStr += shelper.Sales_Invoice_Main_Voucher_Prefix + "='"+ shift->voucherPrefix+"'";
    qStr += " and "+shelper.Sales_Invoice_Main_Ledger_ID+ " like '"+ BankAccountsId+"'";
    qStr += " and ";
    qStr += "CAST("+shelper.Sales_Invoice_Main_Voucher_No+" AS UNSIGNED)"  + ">= CAST(" + shift->BillsFrom +" AS UNSIGNED)";
    qStr += " and ";
    qStr += "CAST("+shelper.Sales_Invoice_Main_Voucher_No+" AS UNSIGNED)"  + "<= CAST(" + shift->BillsTo +" AS UNSIGNED)";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);

    if(query.exec()){
        while(query.next()){
            vNO = query.value(0).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }

    qDebug()<<"Sales By card";

    return vNO;
}

float ShiftDatabaseHelper::getPurchaseTotal(ShiftDataModel *shift)
{
    float vNO;
    PurchaseVoucherDatabaseHelper pHelp;
    QString timestamp = "concat("+pHelp.Purchase_Invoice_Main_Voucher_Date+",' ',CAST("
            +pHelp.Purchase_Invoice_Main_TimeStamp+" AS TIME))";
    timestamp = pHelp.Purchase_Invoice_Main_TimeStamp;
    QString qStr ="SELECT SUM("+pHelp.Purchase_Invoice_Main_Total+") FROM ";

    qStr += pHelp.Purchase_Invoice_Main_TableName + " WHERE ";
    qStr += pHelp.Purchase_Invoice_Main_Voucher_Prefix + "= '"+ shift->voucherPrefix+"'";
    qStr += " and ";
    qStr += " ( ";
    qStr += " ( ";
    qStr += pHelp.Purchase_Invoice_Main_Voucher_Date + ">'" + shift->ShiftStartDateTime.toString("yyyy-MM-dd hh:mm:ss") + "'";
    qStr += " and ";
    qStr += pHelp.Purchase_Invoice_Main_Voucher_Date + "< '" + shift->ShiftEndDateTime.toString("yyyy-MM-dd  hh:mm:ss" ) + "'";
    qStr += ")";

    qStr += ")";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);

    if(query.exec()){
        while(query.next()){
            vNO = query.value(0).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return vNO;

}

float ShiftDatabaseHelper::getPurchaseByCash(ShiftDataModel *shift)
{
    float vNO;
    PurchaseVoucherDatabaseHelper pHelp;

    //    QString timestamp = "concat("+pHelp.Purchase_Invoice_Main_Voucher_Date+",' ',CAST("
    //            +pHelp.Purchase_Invoice_Main_TimeStamp+" AS TIME))";
    QString timestamp = pHelp.Purchase_Invoice_Main_TimeStamp;

    QString qStr ="SELECT SUM("+pHelp.Purchase_Invoice_Main_Total+") FROM ";

    qStr += pHelp.Purchase_Invoice_Main_TableName + " WHERE ";
    qStr += pHelp.Purchase_Invoice_Main_Voucher_Prefix + "='"+ shift->voucherPrefix+"'";
    qStr += " and "+pHelp.Purchase_Invoice_Main_Ledger_ID+ "='"+ shift->cashID+"'";
    qStr += " and ";
    qStr += " ( ";

    qStr += " ( ";
    qStr += pHelp.Purchase_Invoice_Main_Voucher_Date + ">'" + shift->ShiftStartDateTime.toString("yyyy-MM-dd hh:mm:ss") + "'";
    qStr += " and ";
    qStr += pHelp.Purchase_Invoice_Main_Voucher_Date + "< '" + shift->ShiftEndDateTime.toString("yyyy-MM-dd hh:mm:ss") + "'";
    qStr += ")";

    qStr += ")";

    //    qDebug()<<"Purchase Total";
    //    qDebug()<<qStr;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);

    if(query.exec()){
        while(query.next()){
            vNO = query.value(0).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return vNO;
}

float ShiftDatabaseHelper::getPurchaseByCredit(ShiftDataModel *shift)
{
    float vNO;
    QString sundryCreditorsId = "";
    AccountGroupMasterDatabaseHelper *acchelper = new AccountGroupMasterDatabaseHelper;
    sundryCreditorsId = acchelper->getGroupIDByName("Sundry Creditors") +"x%";

    PurchaseVoucherDatabaseHelper pHelp;
    QString timestamp = "concat("+pHelp.Purchase_Invoice_Main_Voucher_Date+",' ',CAST("
            +pHelp.Purchase_Invoice_Main_TimeStamp+" AS TIME))";
    timestamp = pHelp.Purchase_Invoice_Main_TimeStamp;
    QString qStr ="SELECT SUM("+pHelp.Purchase_Invoice_Main_Total+") FROM ";

    qStr += pHelp.Purchase_Invoice_Main_TableName + " WHERE ";
    qStr += pHelp.Purchase_Invoice_Main_Voucher_Prefix + "='"+ shift->voucherPrefix+"'";
    qStr += " and "+pHelp.Purchase_Invoice_Main_Ledger_ID+ " like '"+sundryCreditorsId+"'";
    qStr += " and ";
    qStr += " ( ";

    qStr += " ( ";
    qStr += timestamp + ">'" + shift->ShiftStartDateTime.toString("yyyy-MM-dd hh:mm:ss") + "'";
    qStr += " and ";
    qStr += timestamp + "< '" + shift->ShiftEndDateTime.toString("yyyy-MM-dd hh:mm:ss") + "'";
    qStr += ")";

    qStr += ")";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);

    if(query.exec()){
        while(query.next()){
            vNO = query.value(0).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return vNO;
}

float ShiftDatabaseHelper::getPaymentByCash(ShiftDataModel *shift)
{
    float vNO;
    PaymentVoucherDatabaseHelper pHelp;

    QString timestamp = "concat("+pHelp.Payment_Voucher_Main_Voucher_Date+",' ',CAST("
            +pHelp.Payment_Voucher_Main_TimeStamp+" AS TIME))";
    timestamp = pHelp.Payment_Voucher_Main_TimeStamp;

    QString qStr ="SELECT SUM("+pHelp.Payment_Voucher_Main_Total+") FROM ";

    qStr += pHelp.Payment_Voucher_Main_TableName + " WHERE ";
    qStr += pHelp.Payment_Voucher_Main_Voucher_Prefix + "='"+ shift->voucherPrefix+"'";
    qStr += " and "+pHelp.Payment_Voucher_Main_Ledger_ID+ "='"+ shift->cashID+"'";
    qStr += " and ";
    qStr += " ( ";

    qStr += " ( ";

    //    qStr += pHelp.Payment_Voucher_Main_Voucher_Date + ">="+ shift->ShiftStartDateTime.toString("yyyy-MM-dd");

    qStr += timestamp + ">'" + shift->ShiftStartDateTime.toString("yyyy-MM-dd hh:mm:ss") + "'";
    qStr += " and ";
    qStr += timestamp + "< '" + shift->ShiftEndDateTime.toString("yyyy-MM-dd hh:mm:ss") + "'";
    qStr += ")";
    qStr += ")";


    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);

    if(query.exec()){
        while(query.next()){
            vNO = query.value(0).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return vNO;
}

GeneralVoucherDataObject ShiftDatabaseHelper::getPaymentDetails(ShiftDataModel *shift)
{
    GeneralVoucherDataObject obj;
    PaymentVoucherDatabaseHelper pHelp;
    LedgerMasterDatabaseHelper ledHelper;

    QString timestamp = "concat("+pHelp.Payment_Voucher_Main_Voucher_Date+",' ',CAST("
            +pHelp.Payment_Voucher_Main_TimeStamp+" AS TIME))";
    timestamp = pHelp.Payment_Voucher_Main_TimeStamp;
    QString qStr;

    qStr += "SELECT "+pHelp.Payment_Voucher_Details_Ledger_ID;
    qStr += ", "+pHelp.Payment_Voucher_Details_CrAmount;
    qStr += " FROM "+pHelp.Payment_Voucher_Details_TableName;
    qStr += " where "+pHelp.Payment_Voucher_Details_Voucher_No+" in ";
    qStr +="(SELECT "+pHelp.Payment_Voucher_Main_Voucher_No+" FROM ";
    qStr += pHelp.Payment_Voucher_Main_TableName + " WHERE ";
    qStr += pHelp.Payment_Voucher_Main_Voucher_Prefix + "='"+ shift->voucherPrefix+"'";
    qStr += " and "+pHelp.Payment_Voucher_Main_Ledger_ID+ "='"+ shift->cashID+"'";
    qStr += " and ";
    qStr += " ( ";
    qStr += " ( ";

    //    qStr += pHelp.Payment_Voucher_Main_Voucher_Date + ">="+ shift->ShiftStartDateTime.toString("yyyy-MM-dd");

    qStr += timestamp + ">'" + shift->ShiftStartDateTime.toString("yyyy-MM-dd hh:mm:ss") + "'";
    qStr += " and ";
    qStr += timestamp + "< '" + shift->ShiftEndDateTime.toString("yyyy-MM-dd hh:mm:ss") + "'";
    qStr += ")";
    qStr += "))";


    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    qDebug()<<"Payment Details"<<qStr;
    query.prepare(qStr);

    if(query.exec()){
        while(query.next()){
            LedgerMasterDataModel led;
            led = ledHelper.getLedgerObjectByID(query.value(0).toString());
            led.crAmount = query.value(1).toFloat();
            obj.ledgersList.append(led);
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return obj;
}

GeneralVoucherDataObject ShiftDatabaseHelper::getCreditPaymentDetails(ShiftDataModel *shift)
{
    GeneralVoucherDataObject obj;
    PaymentVoucherDatabaseHelper pHelp;
    LedgerMasterDatabaseHelper ledHelper;


    QString timestamp = "concat("+pHelp.Payment_Voucher_Main_Voucher_Date+",' ','00:00:00')";
    timestamp = pHelp.Payment_Voucher_Main_Voucher_Date;
    //            +pHelp.Purchase_Invoice_Main_TimeStamp+" AS TIME))";
    //    timestamp = pHelp.Purchase_Invoice_Main_TimeStamp;

    QString qStr;
    qStr += "SELECT "+pHelp.Payment_Voucher_Details_Ledger_ID;
    qStr += ", SUM("+pHelp.Payment_Voucher_Details_CrAmount+")";
    qStr += " FROM "+pHelp.Payment_Voucher_Details_TableName;
    qStr += " where "+pHelp.Payment_Voucher_Details_Voucher_No+" in ";
    qStr +="(SELECT "+pHelp.Payment_Voucher_Main_Voucher_No+" FROM ";
    qStr += pHelp.Payment_Voucher_Main_TableName + " WHERE ";
    qStr += pHelp.Payment_Voucher_Main_Voucher_Prefix + "='"+ shift->voucherPrefix+"'";
    qStr += " and "+pHelp.Payment_Voucher_Main_Ledger_ID+ "='"+ shift->cashID+"'";
    qStr += " and ";
    qStr += " ( ";
    qStr += " ( ";

    //    qStr += pHelp.Payment_Voucher_Main_Voucher_Date + ">="+ shift->ShiftStartDateTime.toString("yyyy-MM-dd");

    qStr += timestamp + ">='" + shift->ShiftStartDateTime.toString("yyyy-MM-dd") + "'";
    qStr += " and ";
    qStr += timestamp + "< '" + shift->ShiftEndDateTime.toString("yyyy-MM-dd") + "'";
    qStr += ")";
    qStr += "))";
    qStr += " group by "+pHelp.Payment_Voucher_Main_Ledger_ID;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    //    qDebug()<<"PaymentDetails"<<qStr;
    query.prepare(qStr);

    if(query.exec()){
        while(query.next()){
            LedgerMasterDataModel led;
            led = ledHelper.getLedgerObjectByID(query.value(0).toString());
            led.crAmount = query.value(1).toFloat();
            obj.ledgersList.append(led);
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return obj;
}

GeneralVoucherDataObject ShiftDatabaseHelper::getCreditSalesDetails(ShiftDataModel *shift)
{
    GeneralVoucherDataObject obj;

    LedgerMasterDatabaseHelper ledHelper;
    QString sundryCreditorsId = "";
    AccountGroupMasterDatabaseHelper *acchelper = new AccountGroupMasterDatabaseHelper;
    sundryCreditorsId = acchelper->getGroupIDByName("Sundry Debtors") +"x%";

    salesVoucherDatabaseHelper pHelp;
    QString timestamp = "concat("+pHelp.Sales_Invoice_Main_Voucher_Date+",' ','00:00:00')";

    //            +pHelp.Purchase_Invoice_Main_TimeStamp+" AS TIME))";
    //    timestamp = pHelp.Purchase_Invoice_Main_TimeStamp;


    QString qStr ="SELECT SUM("+pHelp.Sales_Invoice_Main_Total+")";
    qStr += ", "+pHelp.Sales_Invoice_Main_Ledger_ID;
    qStr +=" FROM ";

    qStr += pHelp.Sales_Invoice_Main_TableName + " WHERE ";
    qStr += pHelp.Sales_Invoice_Main_Voucher_Prefix + "='"+ shift->voucherPrefix+"'";
    qStr += " and "+pHelp.Sales_Invoice_Main_Ledger_ID+ " like '"+sundryCreditorsId+"'";
    qStr += " and ";
    qStr += " ( ";
    qStr += "CAST("+shelper.Sales_Invoice_Main_Voucher_No+" AS UNSIGNED)"  + ">= CAST(" + shift->BillsFrom +" AS UNSIGNED)";
    qStr += " and ";
    qStr += "CAST("+shelper.Sales_Invoice_Main_Voucher_No+" AS UNSIGNED)"  + "<= CAST(" + shift->BillsTo +" AS UNSIGNED)";

    qStr += ")";
    qStr += " group by "+pHelp.Sales_Invoice_Main_Ledger_ID;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    //    qDebug()<<"Sales Details"<<qStr;
    query.prepare(qStr);

    if(query.exec()){
        while(query.next()){
            LedgerMasterDataModel led;
            led = ledHelper.getLedgerObjectByID(query.value(1).toString());
            led.drAmount = query.value(0).toFloat();
            obj.ledgersList.append(led);
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return obj;
}

GeneralVoucherDataObject ShiftDatabaseHelper::getCreditPurchaseDetails(ShiftDataModel *shift)
{
    GeneralVoucherDataObject obj;

    LedgerMasterDatabaseHelper ledHelper;
    QString sundryCreditorsId = "";
    AccountGroupMasterDatabaseHelper *acchelper = new AccountGroupMasterDatabaseHelper;
    sundryCreditorsId = acchelper->getGroupIDByName("Sundry Creditors") +"x%";

    PurchaseVoucherDatabaseHelper pHelp;
    QString timestamp = "concat("+pHelp.Purchase_Invoice_Main_Voucher_Date+",' ','00:00:00')";
    timestamp = pHelp.Purchase_Invoice_Main_Voucher_Date;
    //            +pHelp.Purchase_Invoice_Main_TimeStamp+" AS TIME))";
    //    timestamp = pHelp.Purchase_Invoice_Main_TimeStamp;


    QString qStr ="SELECT SUM("+pHelp.Purchase_Invoice_Main_Total+")";
    qStr += ", "+pHelp.Purchase_Invoice_Main_Ledger_ID;
    qStr +=" FROM ";

    qStr += pHelp.Purchase_Invoice_Main_TableName + " WHERE ";
    qStr += pHelp.Purchase_Invoice_Main_Voucher_Prefix + "='"+ shift->voucherPrefix+"'";
    qStr += " and "+pHelp.Purchase_Invoice_Main_Ledger_ID+ " like '"+sundryCreditorsId+"'";
    qStr += " and ";
    qStr += " ( ";

    qStr += " ( ";
    qStr += timestamp + ">='" + shift->ShiftStartDateTime.toString("yyyy-MM-dd") + "'";
    qStr += " and ";
    qStr += timestamp + "< '" + shift->ShiftEndDateTime.toString("yyyy-MM-dd") + "'";
    qStr += ")";

    qStr += ")";
    qStr += " group by "+pHelp.Purchase_Invoice_Main_Ledger_ID;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    //    qDebug()<<"PaymentDetails"<<qStr;
    query.prepare(qStr);

    if(query.exec()){
        while(query.next()){
            LedgerMasterDataModel led;
            led = ledHelper.getLedgerObjectByID(query.value(1).toString());
            led.crAmount = query.value(0).toFloat();
            obj.ledgersList.append(led);
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return obj;
}

GeneralVoucherDataObject ShiftDatabaseHelper::getCreditReceiptDetails(ShiftDataModel *shift)
{
    GeneralVoucherDataObject obj;
    ReceiptVoucherDatabaseHelper pHelp;
    LedgerMasterDatabaseHelper ledHelper;

    QString timestamp = "concat("+pHelp.Receipt_Voucher_Main_Voucher_Date+",' ','00:00:00')";

    //            +pHelp.Purchase_Invoice_Main_TimeStamp+" AS TIME))";
    //    timestamp = pHelp.Purchase_Invoice_Main_TimeStamp;

    QString qStr;

    qStr += "SELECT "+pHelp.Receipt_Voucher_Details_Ledger_ID;
    qStr += ", SUM("+pHelp.Receipt_Voucher_Details_CrAmount+")";
    qStr += " FROM "+pHelp.Receipt_Voucher_Details_TableName;
    qStr += " where "+pHelp.Receipt_Voucher_Details_Voucher_No+" in ";
    qStr +="(SELECT "+pHelp.Receipt_Voucher_Main_Voucher_No+" FROM ";
    qStr += pHelp.Receipt_Voucher_Main_TableName + " WHERE ";
    qStr += pHelp.Receipt_Voucher_Main_Voucher_Prefix + "='"+ shift->voucherPrefix+"'";
    qStr += " and "+pHelp.Receipt_Voucher_Main_Ledger_ID+ "='"+ shift->cashID+"'";
    qStr += " and ";
    qStr += " ( ";
    qStr += " ( ";

    //    qStr += pHelp.Payment_Voucher_Main_Voucher_Date + ">="+ shift->ShiftStartDateTime.toString("yyyy-MM-dd");

    qStr += timestamp + ">='" + shift->ShiftStartDateTime.toString("yyyy-MM-dd") + "'";
    qStr += " and ";
    qStr += timestamp + "< '" + shift->ShiftEndDateTime.toString("yyyy-MM-dd") + "'";
    qStr += ")";
    qStr += "))";
    qStr += " group by "+pHelp.Receipt_Voucher_Main_Ledger_ID;


    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    //    qDebug()<<"PaymentDetails"<<qStr;
    query.prepare(qStr);

    if(query.exec()){
        while(query.next()){
            LedgerMasterDataModel led;
            led = ledHelper.getLedgerObjectByID(query.value(0).toString());
            led.drAmount = query.value(1).toFloat();
            obj.ledgersList.append(led);
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return obj;
}

float ShiftDatabaseHelper::getReceiptByCash(ShiftDataModel *shift)
{
    float vNO;
    ReceiptVoucherDatabaseHelper pHelp;
    QString timestamp = "concat("+pHelp.Receipt_Voucher_Main_Voucher_Date+",' ',CAST("
            +pHelp.Receipt_Voucher_Main_TimeStamp+" AS TIME))";
    timestamp = pHelp.Receipt_Voucher_Main_TimeStamp;
    QString qStr ="SELECT SUM("+pHelp.Receipt_Voucher_Main_Total+") FROM ";

    qStr += pHelp.Receipt_Voucher_Main_TableName + " WHERE ";
    qStr += pHelp.Receipt_Voucher_Main_Voucher_Prefix + "='"+ shift->voucherPrefix+"'";
    qStr += " and "+pHelp.Receipt_Voucher_Main_Ledger_ID+ "='"+ shift->cashID+"'";
    qStr += " and ";
    qStr += " ( ";

    qStr += " ( ";
    qStr += timestamp + ">'" + shift->ShiftStartDateTime.toString("yyyy-MM-dd hh:mm:ss") + "'";
    qStr += " and ";
    qStr += timestamp + "< '" + shift->ShiftEndDateTime.toString("yyyy-MM-dd hh:mm:ss") + "'";
    qStr += ")";

    qStr += ")";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);

    if(query.exec()){
        while(query.next()){
            vNO = query.value(0).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }

    return vNO;
}

float ShiftDatabaseHelper::getReceiptTotal(ShiftDataModel *shift)
{
    float vNO;
    ReceiptVoucherDatabaseHelper pHelp;
    QString timestamp = "concat("+pHelp.Receipt_Voucher_Main_Voucher_Date+",' ',CAST("
            +pHelp.Receipt_Voucher_Main_TimeStamp+" AS TIME))";
    timestamp = pHelp.Receipt_Voucher_Main_TimeStamp;
    QString qStr ="SELECT SUM("+pHelp.Receipt_Voucher_Main_Total+") FROM ";

    qStr += pHelp.Receipt_Voucher_Main_TableName + " WHERE ";
    qStr += pHelp.Receipt_Voucher_Main_Voucher_Prefix + "= '"+ shift->voucherPrefix+"'";

    qStr += " and ";
    qStr += " ( ";

    qStr += " ( ";
    qStr += timestamp + ">'" + shift->ShiftStartDateTime.toString("yyyy-MM-dd hh:mm:ss") + "'";
    qStr += " and ";
    qStr += timestamp + "< '" + shift->ShiftEndDateTime.toString("yyyy-MM-dd  hh:mm:ss" ) + "'";
    qStr += ")";

    qStr += ")";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);

    if(query.exec()){
        while(query.next()){
            vNO = query.value(0).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return vNO;
}

float ShiftDatabaseHelper::getPaymentTotal(ShiftDataModel *shift)
{
    float vNO;
    PaymentVoucherDatabaseHelper pHelp;
    QString timestamp = "concat("+pHelp.Payment_Voucher_Main_Voucher_Date+",' ',CAST("
            +pHelp.Payment_Voucher_Main_TimeStamp+" AS TIME))";
    timestamp = pHelp.Payment_Voucher_Main_TimeStamp;
    QString qStr ="SELECT SUM("+pHelp.Payment_Voucher_Main_Total+") FROM ";

    qStr += pHelp.Payment_Voucher_Main_TableName + " WHERE ";
    qStr += pHelp.Payment_Voucher_Main_Voucher_Prefix + "= '"+ shift->voucherPrefix+"'";

    qStr += " and ";
    qStr += " ( ";

    qStr += " ( ";
    qStr += timestamp + ">'" + shift->ShiftStartDateTime.toString("yyyy-MM-dd hh:mm:ss") + "'";
    qStr += " and ";
    qStr += timestamp + "< '" + shift->ShiftEndDateTime.toString("yyyy-MM-dd  hh:mm:ss" ) + "'";
    qStr += ")";

    qStr += ")";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);

    if(query.exec()){
        while(query.next()){
            vNO = query.value(0).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return vNO;
}

float ShiftDatabaseHelper::DayBookgetPurchaseTotal(ShiftDataModel *shift)
{
    float vNO;
    PurchaseVoucherDatabaseHelper pHelp;
    QString timestamp = "concat("+pHelp.Purchase_Invoice_Main_Voucher_Date+",' ','00:00:00')";
    timestamp = pHelp.Purchase_Invoice_Main_Voucher_Date;
    //            +pHelp.Purchase_Invoice_Main_TimeStamp+" AS TIME))";
    //    timestamp = pHelp.Purchase_Invoice_Main_TimeStamp;
    QString qStr ="SELECT SUM("+pHelp.Purchase_Invoice_Main_Total+") FROM ";

    qStr += pHelp.Purchase_Invoice_Main_TableName + " WHERE ";
    qStr += pHelp.Purchase_Invoice_Main_Voucher_Prefix + "= '"+ shift->voucherPrefix+"'";

    qStr += " and ";
    qStr += " ( ";

    qStr += " ( ";
    qStr += timestamp + ">='" + shift->ShiftStartDateTime.toString("yyyy-MM-dd") + "'";
    qStr += " and ";
    qStr += timestamp + "< '" + shift->ShiftEndDateTime.toString("yyyy-MM-dd" ) + "'";
    qStr += ")";

    qStr += ")";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);

    if(query.exec()){
        while(query.next()){
            vNO = query.value(0).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return vNO;
}

float ShiftDatabaseHelper::DayBookgetPurchaseByCash(ShiftDataModel *shift)
{

    float vNO;
    PurchaseVoucherDatabaseHelper pHelp;

    QString timestamp = "concat("+pHelp.Purchase_Invoice_Main_Voucher_Date+",' ','00:00:00')";
    timestamp = pHelp.Purchase_Invoice_Main_Voucher_Date;

    //            +pHelp.Purchase_Invoice_Main_TimeStamp+" AS TIME))";
    //    timestamp = pHelp.Purchase_Invoice_Main_TimeStamp;

    QString qStr ="SELECT SUM("+pHelp.Purchase_Invoice_Main_Total+") FROM ";

    qStr += pHelp.Purchase_Invoice_Main_TableName + " WHERE ";
    qStr += pHelp.Purchase_Invoice_Main_Voucher_Prefix + "='"+ shift->voucherPrefix+"'";
    qStr += " and "+pHelp.Purchase_Invoice_Main_Ledger_ID+ "='"+ shift->cashID+"'";
    qStr += " and ";
    qStr += " ( ";

    qStr += " ( ";
    qStr += timestamp + ">='" + shift->ShiftStartDateTime.toString("yyyy-MM-dd") + "'";
    qStr += " and ";
    qStr += timestamp + "< '" + shift->ShiftEndDateTime.toString("yyyy-MM-dd") + "'";
    qStr += ")";

    qStr += ")";

    //    qDebug()<<"Purchase Total";
    //    qDebug()<<qStr;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);

    if(query.exec()){
        while(query.next()){
            vNO = query.value(0).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return vNO;
}

float ShiftDatabaseHelper::DayBookgetPurchaseByCredit(ShiftDataModel *shift)
{

    float vNO;
    QString sundryCreditorsId = "";
    AccountGroupMasterDatabaseHelper *acchelper = new AccountGroupMasterDatabaseHelper;
    sundryCreditorsId = acchelper->getGroupIDByName("Sundry Creditors") +"x%";

    PurchaseVoucherDatabaseHelper pHelp;
    QString timestamp = "concat("+pHelp.Purchase_Invoice_Main_Voucher_Date+",' ','00:00:00')";
    timestamp = pHelp.Purchase_Invoice_Main_Voucher_Date;

    //            +pHelp.Purchase_Invoice_Main_TimeStamp+" AS TIME))";
    //    timestamp = pHelp.Purchase_Invoice_Main_TimeStamp;

    QString qStr ="SELECT SUM("+pHelp.Purchase_Invoice_Main_Total+") FROM ";

    qStr += pHelp.Purchase_Invoice_Main_TableName + " WHERE ";
    qStr += pHelp.Purchase_Invoice_Main_Voucher_Prefix + "='"+ shift->voucherPrefix+"'";
    qStr += " and "+pHelp.Purchase_Invoice_Main_Ledger_ID+ " like '"+sundryCreditorsId+"'";
    qStr += " and ";
    qStr += " ( ";

    qStr += " ( ";
    qStr += timestamp + ">='" + shift->ShiftStartDateTime.toString("yyyy-MM-dd") + "'";
    qStr += " and ";
    qStr += timestamp + "< '" + shift->ShiftEndDateTime.toString("yyyy-MM-dd") + "'";
    qStr += ")";

    qStr += ")";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);

    if(query.exec()){
        while(query.next()){
            vNO = query.value(0).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return vNO;
}

float ShiftDatabaseHelper::DayBookgetPaymentByCash(ShiftDataModel *shift)
{

    float vNO;
    PaymentVoucherDatabaseHelper pHelp;

    QString timestamp = "concat("+pHelp.Payment_Voucher_Main_Voucher_Date+",' ','00:00:00')";
    timestamp = pHelp.Payment_Voucher_Main_Voucher_Date;
    //            +pHelp.Purchase_Invoice_Main_TimeStamp+" AS TIME))";
    //    timestamp = pHelp.Purchase_Invoice_Main_TimeStamp;

    QString qStr ="SELECT SUM("+pHelp.Payment_Voucher_Main_Total+") FROM ";

    qStr += pHelp.Payment_Voucher_Main_TableName + " WHERE ";
    qStr += pHelp.Payment_Voucher_Main_Voucher_Prefix + "='"+ shift->voucherPrefix+"'";
    qStr += " and "+pHelp.Payment_Voucher_Main_Ledger_ID+ "='"+ shift->cashID+"'";
    qStr += " and ";
    qStr += " ( ";

    qStr += " ( ";

    //    qStr += pHelp.Payment_Voucher_Main_Voucher_Date + ">="+ shift->ShiftStartDateTime.toString("yyyy-MM-dd");

    qStr += timestamp + ">='" + shift->ShiftStartDateTime.toString("yyyy-MM-dd") + "'";
    qStr += " and ";
    qStr += timestamp + "<'" + shift->ShiftEndDateTime.toString("yyyy-MM-dd") + "'";
    qStr += ")";
    qStr += ")";

    qDebug()<<"Pay Cash"<<qStr;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);

    if(query.exec()){
        while(query.next()){
            vNO = query.value(0).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return vNO;
}

float ShiftDatabaseHelper::DayBookgetReceiptByCash(ShiftDataModel *shift)
{

    float vNO;
    ReceiptVoucherDatabaseHelper pHelp;
    QString timestamp = "concat("+pHelp.Receipt_Voucher_Main_Voucher_Date+",' ','00:00:00')";
    timestamp = pHelp.Receipt_Voucher_Main_Voucher_Date;

    //            +pHelp.Purchase_Invoice_Main_TimeStamp+" AS TIME))";
    //    timestamp = pHelp.Purchase_Invoice_Main_TimeStamp;

    QString qStr ="SELECT SUM("+pHelp.Receipt_Voucher_Main_Total+") FROM ";

    qStr += pHelp.Receipt_Voucher_Main_TableName + " WHERE ";
    qStr += pHelp.Receipt_Voucher_Main_Voucher_Prefix + "='"+ shift->voucherPrefix+"'";
    qStr += " and "+pHelp.Receipt_Voucher_Main_Ledger_ID+ "='"+ shift->cashID+"'";
    qStr += " and ";
    qStr += " ( ";

    qStr += " ( ";
    qStr += timestamp + ">='" + shift->ShiftStartDateTime.toString("yyyy-MM-dd") + "'";
    qStr += " and ";
    qStr += timestamp + "<= '" + shift->ShiftEndDateTime.toString("yyyy-MM-dd") + "'";
    qStr += ")";

    qStr += ")";

    //    qDebug()<<"Rec Cash"<<qStr;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);

    if(query.exec()){
        while(query.next()){
            vNO = query.value(0).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }

    return vNO;
}

float ShiftDatabaseHelper::DayBookgetReceiptTotal(ShiftDataModel *shift)
{

    float vNO;
    ReceiptVoucherDatabaseHelper pHelp;

    QString timestamp = "concat("+pHelp.Receipt_Voucher_Main_Voucher_Date+",' ','00:00:00')";
    timestamp = pHelp.Receipt_Voucher_Main_Voucher_Date;

    //            +pHelp.Purchase_Invoice_Main_TimeStamp+" AS TIME))";
    //    timestamp = pHelp.Purchase_Invoice_Main_TimeStamp;
    QString qStr ="SELECT SUM("+pHelp.Receipt_Voucher_Main_Total+") FROM ";

    qStr += pHelp.Receipt_Voucher_Main_TableName + " WHERE ";
    qStr += pHelp.Receipt_Voucher_Main_Voucher_Prefix + "= '"+ shift->voucherPrefix+"'";

    qStr += " and ";
    qStr += " ( ";

    qStr += " ( ";
    qStr += timestamp + ">='" + shift->ShiftStartDateTime.toString("yyyy-MM-dd") + "'";
    qStr += " and ";
    qStr += timestamp + "< '" + shift->ShiftEndDateTime.toString("yyyy-MM-dd" ) + "'";
    qStr += ")";

    qStr += ")";

    //    qDebug()<<"Rec Total"<<qStr;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);

    if(query.exec()){
        while(query.next()){
            vNO = query.value(0).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return vNO;

}

float ShiftDatabaseHelper::DayBookgetPaymentTotal(ShiftDataModel *shift)
{

    float vNO;
    PaymentVoucherDatabaseHelper pHelp;

    QString timestamp = "concat("+pHelp.Payment_Voucher_Main_Voucher_Date+",' ','00:00:00')";
    timestamp = pHelp.Payment_Voucher_Main_Voucher_Date;

    //            +pHelp.Purchase_Invoice_Main_TimeStamp+" AS TIME))";
    //    timestamp = pHelp.Purchase_Invoice_Main_TimeStamp;
    QString qStr ="SELECT SUM("+pHelp.Payment_Voucher_Main_Total+") FROM ";

    qStr += pHelp.Payment_Voucher_Main_TableName + " WHERE ";
    qStr += pHelp.Payment_Voucher_Main_Voucher_Prefix + "= '"+ shift->voucherPrefix+"'";

    qStr += " and ";
    qStr += " ( ";

    qStr += " ( ";
    qStr += timestamp + ">='" + shift->ShiftStartDateTime.toString("yyyy-MM-dd") + "'";
    qStr += " and ";
    qStr += timestamp + "< '" + shift->ShiftEndDateTime.toString("yyyy-MM-dd") + "'";
    qStr += ")";

    qStr += ")";

    qDebug()<<"Pay Total"<<qStr;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);

    if(query.exec()){
        while(query.next()){
            vNO = query.value(0).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return vNO;
}

float ShiftDatabaseHelper::getZomatoSalesTotal(ShiftDataModel *shift)
{
    float vNO;

    QString qStr ="SELECT SUM("+shelper.Sales_Invoice_Main_Total+") FROM ";
    qStr += shelper.Sales_Invoice_Main_TableName + " WHERE ";
    qStr += shelper.Sales_Invoice_Main_Voucher_Prefix + "='"+ shift->voucherPrefix+"'";
    qStr += " and "+shelper.Sales_Invoice_Main_Billing_Name +"='zomato'";

    //    qStr += " and "+shelper.Sales_Invoice_Main_Ledger_ID+ "='"+ shift->cashID+"'";
    qStr += " and ";
    qStr += "CAST("+shelper.Sales_Invoice_Main_Voucher_No+" AS UNSIGNED)"  + ">= " + shift->BillsFrom;
    qStr += " and ";
    qStr += "CAST("+shelper.Sales_Invoice_Main_Voucher_No+" AS UNSIGNED)"  + "<= " + shift->BillsTo;



    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);

    if(query.exec()){
        while(query.next()){
            vNO = query.value(0).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return vNO;
}

float ShiftDatabaseHelper::getZomatoSalesByCash(ShiftDataModel *shift)
{
    float vNO;
    QString qStr ="SELECT SUM("+shelper.Sales_Invoice_Main_Total+") FROM ";
    qStr += shelper.Sales_Invoice_Main_TableName + " WHERE ";
    qStr += shelper.Sales_Invoice_Main_Voucher_Prefix + "='"+ shift->voucherPrefix+"'";
    qStr += " and "+shelper.Sales_Invoice_Main_Billing_Name +"='zomato'";

    qStr += " and "+shelper.Sales_Invoice_Main_Ledger_ID+ "='"+ shift->cashID+"'";

    qStr += " and ";
    qStr += "CAST("+shelper.Sales_Invoice_Main_Voucher_No+" AS UNSIGNED)"  + ">=" + shift->BillsFrom;
    qStr += " and ";
    qStr += "CAST("+shelper.Sales_Invoice_Main_Voucher_No+" AS UNSIGNED)"  + "<=" + shift->BillsTo;


    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);

    if(query.exec()){
        while(query.next()){
            vNO = query.value(0).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return vNO;
}

float ShiftDatabaseHelper::getZomatoSalesByCredit(ShiftDataModel *shift)
{
    float vNO;

    QString zomatoId = "";
    LedgerMasterDatabaseHelper *led = new LedgerMasterDatabaseHelper;
    zomatoId = "0x5x19x17";//led->getLedgerIDByName("Zomato");

    QString qStr ="SELECT SUM("+shelper.Sales_Invoice_Main_Total+") FROM ";
    qStr += shelper.Sales_Invoice_Main_TableName + " WHERE ";
    qStr += shelper.Sales_Invoice_Main_Voucher_Prefix + "='"+ shift->voucherPrefix+"'";
    qStr += " and "+shelper.Sales_Invoice_Main_Billing_Name +"='zomato'";

    qStr += " and "+shelper.Sales_Invoice_Main_Ledger_ID+ "='"+ zomatoId+"'";
    qStr += " and ";
    qStr += "CAST("+shelper.Sales_Invoice_Main_Voucher_No+" AS UNSIGNED)"  + ">= " + shift->BillsFrom ;
    qStr += " and ";
    qStr += "CAST("+shelper.Sales_Invoice_Main_Voucher_No+" AS UNSIGNED)"  + "<= " + shift->BillsTo;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    //    qDebug()<<"Zomato Credit "<<qStr;

    query.prepare(qStr);

    if(query.exec()){
        while(query.next()){
            vNO = query.value(0).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }

    return vNO;

}

QList<ShiftDataModel> ShiftDatabaseHelper::getAllShiftDetailsByDate(QDateTime dateFrom, QDateTime dateTo)
{
    QList<ShiftDataModel> shiftList;

    QString qStr  = " SELECT ";

    qStr += " "+Shift_SalesValue;
    qStr += ", "+Shift_CashSales;
    qStr += ", "+Shift_CardSales;
    qStr += ", "+Shift_CreditSales;
    qStr += ", "+Shift_ZomatoSales;
    qStr += ", "+Shift_ZomatoCash;
    qStr += ", "+Shift_ZomatoCredit;
    qStr += ", "+Shift_PurchaseTotal;
    qStr += ", "+Shift_PurchaseCash;
    qStr += ", "+Shift_PaymentCash;
    qStr += ", "+Shift_ReceiptCash;

    qStr += ", "+Shift_cashierID;
    qStr += ", "+Shift_voucherPrefix;
    qStr += ", "+Shift_MacId;
    qStr += ", "+Shift_POSNumber;
    qStr += ", "+Shift_paxCount;
    qStr += ", "+Shift_Narration;
    qStr += ", "+Shift_Start_DateTime;
    qStr += ", "+Shift_End_DateTime;

    qStr += ", "+Shift_TotalCash;
    qStr += ", "+Shift_CasinCounter;
    qStr += ", "+Shift_CashWithDrawn;
    qStr += ", "+Shift_CashBalance;
    qStr += ", "+Shift_TillDifference;
    qStr += ", "+Shift_OpeningBalance;

    qStr += ", "+Shift_ID;


    qStr += " from "+ Shifts_TableName;
    qStr += " where "+Shift_Start_DateTime +">= '" + dateFrom.toString("yyyy-MM-dd hh:mm:ss") + "'";
    qStr += " and "  +Shift_End_DateTime +"<= '"+ dateTo.toString("yyyy-MM-dd hh:mm:ss") + "'";

    qDebug()<<"No. Shifts";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);

    //    query.bindValue(":datefrom",dateFrom);
    //    query.bindValue(":dateto",dateTo);
    if(query.exec()){
        int i;
        while(query.next()){
            ShiftDataModel shiftData;
            i =0;
            shiftData.SalesTotal    = query.value(i++).toFloat();
            shiftData.CashSales= query.value(i++).toFloat();
            shiftData.CardSales= query.value(i++).toFloat();
            shiftData.CreditSales= query.value(i++).toFloat();
            shiftData.ZomatoSales= query.value(i++).toFloat();
            shiftData.ZomatoCash= query.value(i++).toFloat();
            shiftData.ZomatoCredit= query.value(i++).toFloat();
            shiftData.PurchaseTotal= query.value(i++).toFloat();
            shiftData.CashPurchase= query.value(i++).toFloat();
            shiftData.CashPayments= query.value(i++).toFloat();
            shiftData.CashReceipts= query.value(i++).toFloat();

            shiftData.cashierID= query.value(i++).toInt();
            shiftData.voucherPrefix= query.value(i++).toString();
            shiftData.MaciD= query.value(i++).toString();
            shiftData.POSNumber= query.value(i++).toString();
            shiftData.PaxCount= query.value(i++).toInt();
            shiftData.Narration= query.value(i++).toString();
            shiftData.ShiftStartDateTime= query.value(i++).toDateTime();
            shiftData.ShiftEndDateTime= query.value(i++).toDateTime();

            shiftData.totalCash= query.value(i++).toFloat();
            shiftData.CashInCounter= query.value(i++).toFloat();
            shiftData.CashWithDrawn= query.value(i++).toFloat();
            shiftData.CashBalance= query.value(i++).toFloat();
            shiftData.TillDifference= query.value(i++).toFloat();
            shiftData.OpeningCash= query.value(i++).toFloat();
            shiftData.shiftID = query.value(i++).toInt();

            shiftList.append(shiftData);
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    qDebug()<<"Number Of shifts = "<<shiftList.size();
    return shiftList;
}

ShiftDataModel ShiftDatabaseHelper::getlastShiftDetailByVoucherPrefix(QString prefix)
{
    ShiftDataModel shiftData;

    QString qStr  = " SELECT ";

    qStr += " "+Shift_SalesValue;
    qStr += ", "+Shift_CashSales;
    qStr += ", "+Shift_CardSales;
    qStr += ", "+Shift_CreditSales;
    qStr += ", "+Shift_PurchaseTotal;
    qStr += ", "+Shift_PurchaseCash;
    qStr += ", "+Shift_PaymentCash;
    qStr += ", "+Shift_ReceiptCash;

    qStr += ", "+Shift_cashierID;
    qStr += ", "+Shift_voucherPrefix;
    qStr += ", "+Shift_MacId;
    qStr += ", "+Shift_POSNumber;
    qStr += ", "+Shift_paxCount;
    qStr += ", "+Shift_Narration;
    qStr += ", "+Shift_Start_DateTime;
    qStr += ", "+Shift_End_DateTime;

    qStr += ", "+Shift_TotalCash;
    qStr += ", "+Shift_CasinCounter;
    qStr += ", "+Shift_CashWithDrawn;
    qStr += ", "+Shift_CashBalance;
    qStr += ", "+Shift_TillDifference;
    qStr += ", "+Shift_OpeningBalance;
    qStr += ", "+Shift_ID;
    qStr += ", "+Shift_StartVoucher_No;
    qStr += ", "+Shift_Stopvoucher_No;

    qStr += " from "+ Shifts_TableName;
    qStr += " where "+Shift_voucherPrefix +" like '"+prefix+"'";
    qStr += " and "+Shift_ID +" in (select max("+Shift_ID+") from "+Shifts_TableName+ " where ";
    qStr += Shift_voucherPrefix +" like '"+prefix+"')";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));


    query.prepare(qStr);


    if(query.exec()){
        while(query.next()){
            int i =0;
            shiftData.SalesTotal    = query.value(i++).toFloat();
            shiftData.CashSales= query.value(i++).toFloat();
            shiftData.CardSales= query.value(i++).toFloat();
            shiftData.CreditSales= query.value(i++).toFloat();
            shiftData.PurchaseTotal= query.value(i++).toFloat();
            shiftData.CashPurchase= query.value(i++).toFloat();
            shiftData.CashPayments= query.value(i++).toFloat();
            shiftData.CashReceipts= query.value(i++).toFloat();

            shiftData.cashierID= query.value(i++).toInt();
            shiftData.voucherPrefix= query.value(i++).toString();
            shiftData.MaciD= query.value(i++).toString();
            shiftData.POSNumber= query.value(i++).toString();
            shiftData.PaxCount= query.value(i++).toInt();
            shiftData.Narration= query.value(i++).toString();
            shiftData.ShiftStartDateTime= query.value(i++).toDateTime();
            shiftData.ShiftEndDateTime= query.value(i++).toDateTime();

            shiftData.totalCash= query.value(i++).toFloat();
            shiftData.CashInCounter= query.value(i++).toFloat();
            shiftData.CashWithDrawn= query.value(i++).toFloat();
            shiftData.CashBalance= query.value(i++).toFloat();
            shiftData.TillDifference= query.value(i++).toFloat();
            shiftData.OpeningCash= query.value(i++).toFloat();

            shiftData.shiftID = query.value(i++).toInt();
            shiftData.BillsFrom = query.value(i++).toString();
            shiftData.BillsTo = query.value(i++).toString();
            qDebug()<<"last Bill from db:"<<shiftData.BillsTo;

        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }


    return shiftData;
}

ShiftDataModel ShiftDatabaseHelper::getlastClosingDetailByVoucherPrefix()
{
    ShiftDataModel shiftData;

    QString qStr  = " SELECT ";

    qStr += " "+Shift_SalesValue;
    qStr += ", "+Shift_CashSales;
    qStr += ", "+Shift_CardSales;
    qStr += ", "+Shift_CreditSales;
    qStr += ", "+Shift_ZomatoSales;
    qStr += ", "+Shift_ZomatoCash;
    qStr += ", "+Shift_ZomatoCredit;
    qStr += ", "+Shift_PurchaseTotal;
    qStr += ", "+Shift_PurchaseCash;
    qStr += ", "+Shift_PaymentCash;
    qStr += ", "+Shift_ReceiptCash;

    qStr += ", "+Shift_cashierID;
    qStr += ", "+Shift_voucherPrefix;
    qStr += ", "+Shift_MacId;
    qStr += ", "+Shift_POSNumber;
    qStr += ", "+Shift_paxCount;
    qStr += ", "+Shift_Narration;
    qStr += ", "+Shift_Start_DateTime;
    qStr += ", "+Shift_End_DateTime;

    qStr += ", "+Shift_TotalCash;
    qStr += ", "+Shift_CasinCounter;
    qStr += ", "+Shift_CashWithDrawn;
    qStr += ", "+Shift_CashBalance;
    qStr += ", "+Shift_TillDifference;
    qStr += ", "+Shift_OpeningBalance;
    qStr += ", "+Shift_ID;

    qStr += " from "+ Shifts_TableName;
    qStr += " where ";
    qStr += Shift_ID +" in (select max("+Shift_ID+") from "+Shifts_TableName;


    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);


    if(query.exec()){
        while(query.next()){
            int i =0;
            shiftData.SalesTotal    = query.value(i++).toFloat();
            shiftData.CashSales= query.value(i++).toFloat();
            shiftData.CardSales= query.value(i++).toFloat();
            shiftData.CreditSales= query.value(i++).toFloat();
            shiftData.ZomatoSales= query.value(i++).toFloat();
            shiftData.ZomatoCash= query.value(i++).toFloat();
            shiftData.ZomatoCredit= query.value(i++).toFloat();
            shiftData.PurchaseTotal= query.value(i++).toFloat();
            shiftData.CashPurchase= query.value(i++).toFloat();
            shiftData.CashPayments= query.value(i++).toFloat();
            shiftData.CashReceipts= query.value(i++).toFloat();

            shiftData.cashierID= query.value(i++).toInt();
            shiftData.voucherPrefix= query.value(i++).toString();
            shiftData.MaciD= query.value(i++).toString();
            shiftData.POSNumber= query.value(i++).toString();
            shiftData.PaxCount= query.value(i++).toInt();
            shiftData.Narration= query.value(i++).toString();
            shiftData.ShiftStartDateTime= query.value(i++).toDateTime();
            shiftData.ShiftEndDateTime= query.value(i++).toDateTime();

            shiftData.totalCash= query.value(i++).toFloat();
            shiftData.CashInCounter= query.value(i++).toFloat();
            shiftData.CashWithDrawn= query.value(i++).toFloat();
            shiftData.CashBalance= query.value(i++).toFloat();
            shiftData.TillDifference= query.value(i++).toFloat();
            shiftData.OpeningCash= query.value(i++).toFloat();

            shiftData.shiftID = query.value(i++).toInt();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }

    return shiftData;
}

QString ShiftDatabaseHelper::getFirstPurchaseBillByDate(ShiftDataModel *shift)
{
    QString vNO;
    salesVoucherDatabaseHelper shelper;
    QString qStr ="SELECT MAX(CAST(`"+shelper.Sales_Invoice_Main_Voucher_No+"` AS UNSIGNED)) FROM Purchase_Invoice_Main"
            + " where ";
    qStr += shelper.Sales_Invoice_Main_TimeStamp +"<'"+shift->ShiftStartDateTime.toString("yyyy-MM-dd hh:mm:ss")+ "'";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);

    query.bindValue(":pref",shift->voucherPrefix);
    if(query.exec()){
        while(query.next()){
            vNO = QString::number(query.value(0).toInt()+1);
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return vNO;
}

QString ShiftDatabaseHelper::getLastPurchaseBillByDate(ShiftDataModel *shift)
{
    QString vNO;
    salesVoucherDatabaseHelper shelper;
    QString qStr ="SELECT MAX(CAST(`"+shelper.Sales_Invoice_Main_Voucher_No+"` AS UNSIGNED)) FROM Purchase_Invoice_Main"
            + " where ";
    qStr += shelper.Sales_Invoice_Main_TimeStamp +"<'"+shift->ShiftEndDateTime.toString("yyyy-MM-dd hh:mm:ss")+"'";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);
//    qDebug()<<"get last bill"<<qStr;
    query.bindValue(":pref",shift->voucherPrefix);
    if(query.exec()){
        while(query.next()){
            vNO = QString::number(query.value(0).toInt());
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return vNO;
}
