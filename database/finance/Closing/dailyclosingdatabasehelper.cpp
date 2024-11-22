#include "dailyclosingdatabasehelper.h"

DailyClosingDatabaseHelper::DailyClosingDatabaseHelper()
{
    db = new DatabaseHelper();
    trans = new TransactionHelper();
}


int DailyClosingDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

int DailyClosingDatabaseHelper::insertShiftData(ShiftDataModel *shiftData)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    startTransaction();
    QMap<QString,QVariant> valuesMap;


    valuesMap.insert(Shift_ShiftTimeStamp,QDateTime::currentDateTime());
    valuesMap.insert(Shift_Start_DateTime,shiftData->ShiftStartDateTime);
    valuesMap.insert(Shift_End_DateTime,shiftData->ShiftEndDateTime);


    int id = db->insertRecordsPlus(valuesMap,Shifts_TableName);
    commit();
    return id;
}

void DailyClosingDatabaseHelper::updateShiftData(ShiftDataModel *shiftData, int shiftID)
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

    QMap<QString,QVariant> clauseMap;
    clauseMap.insert(Shift_ID,shiftID);
    db->updateRecordsPlus(valuesMap,clauseMap,Shifts_TableName);
}

void DailyClosingDatabaseHelper::deleteShift(int shiftID)
{
    QMap<QString,QVariant> clauseMap;
    clauseMap.insert(Shift_ID,shiftID);
    db->deleteRecordsPlus(clauseMap,Shifts_TableName);
}

QString DailyClosingDatabaseHelper::getMaxID()
{

}

int DailyClosingDatabaseHelper::commit()
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

QString DailyClosingDatabaseHelper::getFirstBillinShift(ShiftDataModel *shift)
{
    QString vNO;
    QString qStr ="SELECT MAX(`"+Shift_Stopvoucher_No+"`) FROM "+Shifts_TableName + " where "+Shift_voucherPrefix +" like :pref" ;

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

QString DailyClosingDatabaseHelper::getLastBill(ShiftDataModel *shift)
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

void DailyClosingDatabaseHelper::truncateKotNumberTable()
{
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.exec("TRUNCATE TABLE `kotnumbertable`");

}

int DailyClosingDatabaseHelper::getBillsCount(ShiftDataModel *shift)
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

float DailyClosingDatabaseHelper::getOpeningCash(ShiftDataModel *shift)
{
    float vNO =0;
    QString qStr = "";
    QString cashid = shift->cashID;
    LedgerMasterDatabaseHelper led;
    TransactionHelper trans;

    qStr += "SELECT ";
    qStr += led.Ledger_Master_openingBalance +"-";
    qStr += "(SELECT SUM("+trans.Transaction_Helper_Credit_Amount +"-";
    qStr += trans.Transaction_Helper_Debit_Amount +")";
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
    qStr += led.Ledger_Master_LedgerID + " like '"+cashid+"'";

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

float DailyClosingDatabaseHelper::getClosingCash(ShiftDataModel *shift)
{
    float vNO =0;
    QString qStr = "";
    QString cashid = shift->cashID;
    LedgerMasterDatabaseHelper led;
    TransactionHelper trans;
    qStr += "SELECT ";

    qStr += led.Ledger_Master_openingBalance +"-";
    qStr += "(SELECT SUM("+trans.Transaction_Helper_Credit_Amount +"-";
    qStr += trans.Transaction_Helper_Debit_Amount +")";
    qStr += " FROM "+ trans.Transaction_Helper_TableName + " WHERE ";
    qStr += trans.Transaction_Helper_LedgerID + "='"+cashid+"'";
    //    qStr += " AND ";
    //    qStr += trans.Transaction_Helper_VoucherDate + ">='"+shift->ShiftStartDateTime.toString("yyyy-MM-dd")+"'";
    qStr += " AND ";
    qStr += trans.Transaction_Helper_VoucherDate + "<'"+shift->ShiftEndDateTime.toString("yyyy-MM-dd")+"'";
    //    qStr += " AND ";
    //    qStr += "DATE_FORMAT("+trans.Transaction_Helper_TimeStamp+",'%H:%i:%s') "+ ">='"+shift->ShiftStartDateTime.toString("hh:mm:ss")+"'";
    qStr += " OR (";
    qStr += trans.Transaction_Helper_VoucherDate + " like '"+shift->ShiftEndDateTime.toString("yyyy-MM-dd")+"'";
    qStr += " AND ";
    qStr += "DATE_FORMAT("+trans.Transaction_Helper_TimeStamp+",'%H:%i:%s')" + "<='"+shift->ShiftEndDateTime.toString("hh:mm:ss")+"'";
    qStr += ")";

    qStr += ")";
    qStr += " from ";
    qStr += led.Ledger_Master_TableName;
    qStr +=  " where ";
    qStr += led.Ledger_Master_LedgerID + "='"+cashid+"'";
}

float DailyClosingDatabaseHelper::getSalesTotal(ShiftDataModel *shift)
{
    float vNO;

    QString qStr ="SELECT SUM("+shelper.Sales_Invoice_Main_Total+") FROM ";
    qStr += shelper.Sales_Invoice_Main_TableName + " WHERE ";
    qStr += shelper.Sales_Invoice_Main_Voucher_Prefix + " like '"+ shift->voucherPrefix+"'";
    qStr += " and ";
    qStr += shelper.Sales_Invoice_Main_Voucher_No + ">= '" + shift->BillsFrom +"'";
    qStr += " and ";
    qStr += shelper.Sales_Invoice_Main_Voucher_No + "<= '" + shift->BillsTo +"'";

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

float DailyClosingDatabaseHelper::getSalesByCash(ShiftDataModel *shift)
{
    float vNO;
    QString qStr ="SELECT SUM("+shelper.Sales_Invoice_Main_Total+") FROM ";
    qStr += shelper.Sales_Invoice_Main_TableName + " WHERE ";
    qStr += shelper.Sales_Invoice_Main_Voucher_Prefix + " like '"+ shift->voucherPrefix+"'";
    qStr += " and "+shelper.Sales_Invoice_Main_Ledger_ID+ "='"+ shift->cashID+"'";
    qStr += " and ";
    qStr += shelper.Sales_Invoice_Main_Voucher_No + ">= '" + shift->BillsFrom +"'";
    qStr += " and ";
    qStr += shelper.Sales_Invoice_Main_Voucher_No + "<= '" + shift->BillsTo+"'";

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


float DailyClosingDatabaseHelper::getSalesByCredit(ShiftDataModel *shift)
{
    float vNO;
    QString sundryDebtorsId = "%";
    AccountGroupMasterDatabaseHelper *acchelper = new AccountGroupMasterDatabaseHelper;
    sundryDebtorsId = acchelper->getGroupIDByName("Sundry Debtors") +"x%";

    QString qStr ="SELECT SUM("+shelper.Sales_Invoice_Main_Total+") FROM ";
    qStr += shelper.Sales_Invoice_Main_TableName + " WHERE ";
    qStr += shelper.Sales_Invoice_Main_Voucher_Prefix + " like '"+ shift->voucherPrefix+"'";
    qStr += " and "+shelper.Sales_Invoice_Main_Ledger_ID+ "='"+ sundryDebtorsId+"'";
    qStr += " and ";
    qStr += shelper.Sales_Invoice_Main_Voucher_No + ">= '" + shift->BillsFrom+"'";
    qStr += " and ";
    qStr += shelper.Sales_Invoice_Main_Voucher_No + "<= '" + shift->BillsTo+"'";

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

float DailyClosingDatabaseHelper::getSalesByCard(ShiftDataModel *shift)
{
    float vNO;
    QString BankAccountsId = "";
    AccountGroupMasterDatabaseHelper *acchelper = new AccountGroupMasterDatabaseHelper;
    BankAccountsId = acchelper->getGroupIDByName("Bank Accounts") +"x%";
    QString qStr ="SELECT SUM("+shelper.Sales_Invoice_Main_Total+") FROM ";
    qStr += shelper.Sales_Invoice_Main_TableName + " WHERE ";
    qStr += shelper.Sales_Invoice_Main_Voucher_Prefix + " like '"+ shift->voucherPrefix+"'";
    qStr += " and "+shelper.Sales_Invoice_Main_Ledger_ID+ "='"+ BankAccountsId+"'";
    qStr += " and ";
    qStr += shelper.Sales_Invoice_Main_Voucher_No + ">= '" + shift->BillsFrom+"'";
    qStr += " and ";
    qStr += shelper.Sales_Invoice_Main_Voucher_No + "<= '" + shift->BillsTo+"'";

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

float DailyClosingDatabaseHelper::getPurchaseTotal(ShiftDataModel *shift)
{
    float vNO;
    PurchaseVoucherDatabaseHelper pHelp;
    QString timestamp = "concat("+pHelp.Purchase_Invoice_Main_Voucher_Date+",' ',CAST("
            +pHelp.Purchase_Invoice_Main_TimeStamp+" AS TIME))";

    QString qStr ="SELECT SUM("+pHelp.Purchase_Invoice_Main_Total+") FROM ";

    qStr += pHelp.Purchase_Invoice_Main_TableName + " WHERE ";
    qStr += pHelp.Purchase_Invoice_Main_Voucher_Prefix + "='"+ shift->voucherPrefix+"'";

    qStr += " and ";
    qStr += " ( ";

    qStr += " ( ";
    qStr += timestamp + ">'" + shift->ShiftStartDateTime.toString("yyyy-MM-dd hh:mm:ss") + "'";
    qStr += " and ";
    qStr += timestamp + "< '" + shift->ShiftEndDateTime.toString("yyyy-MM-dd  hh:mm:ss" ) + "'";
    qStr += ")";

    qStr += ")";
    qDebug()<<"Purchase Total amount";
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
    return vNO;
}

float DailyClosingDatabaseHelper::getPurchaseByCash(ShiftDataModel *shift)
{
    float vNO;
    PurchaseVoucherDatabaseHelper pHelp;

    QString timestamp = "concat("+pHelp.Purchase_Invoice_Main_Voucher_Date+",' ',CAST("
            +pHelp.Purchase_Invoice_Main_TimeStamp+" AS TIME))";

    QString qStr ="SELECT SUM("+pHelp.Purchase_Invoice_Main_Total+") FROM ";

    qStr += pHelp.Purchase_Invoice_Main_TableName + " WHERE ";
    qStr += pHelp.Purchase_Invoice_Main_Voucher_Prefix + "='"+ shift->voucherPrefix+"'";
    qStr += " and "+pHelp.Purchase_Invoice_Main_Ledger_ID+ "='"+ shift->cashID+"'";
    qStr += " and ";
    qStr += " ( ";

    qStr += " ( ";
    qStr += timestamp + ">'" + shift->ShiftStartDateTime.toString("yyyy-MM-dd hh:mm:ss") + "'";
    qStr += " and ";
    qStr += timestamp + "< '" + shift->ShiftEndDateTime.toString("yyyy-MM-dd hh:mm:ss") + "'";
    qStr += ")";

    qStr += ")";

    qDebug()<<"Purchase Total";
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
    return vNO;
}

float DailyClosingDatabaseHelper::getPurchaseByCredit(ShiftDataModel *shift)
{
    float vNO;
    QString sundryCreditorsId = "";
    AccountGroupMasterDatabaseHelper *acchelper = new AccountGroupMasterDatabaseHelper;
    sundryCreditorsId = acchelper->getGroupIDByName("Sundry Creditors") +"x%";

    PurchaseVoucherDatabaseHelper pHelp;
    QString timestamp = "concat("+pHelp.Purchase_Invoice_Main_Voucher_Date+",' ',CAST("
            +pHelp.Purchase_Invoice_Main_TimeStamp+" AS TIME))";

    QString qStr ="SELECT SUM("+pHelp.Purchase_Invoice_Main_Total+") FROM ";

    qStr += pHelp.Purchase_Invoice_Main_TableName + " WHERE ";
    qStr += pHelp.Purchase_Invoice_Main_Voucher_Prefix + "='"+ shift->voucherPrefix+"'";
    qStr += " and "+pHelp.Purchase_Invoice_Main_Ledger_ID+ "='"+sundryCreditorsId+"'";
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

float DailyClosingDatabaseHelper::getPaymentByCash(ShiftDataModel *shift)
{
    float vNO;
    PaymentVoucherDatabaseHelper pHelp;

    QString timestamp = "concat("+pHelp.Payment_Voucher_Main_Voucher_Date+",' ',CAST("
            +pHelp.Payment_Voucher_Main_TimeStamp+" AS TIME))";

    QString qStr ="SELECT SUM("+pHelp.Payment_Voucher_Main_Total+") FROM ";

    qStr += pHelp.Payment_Voucher_Main_TableName + " WHERE ";
    qStr += pHelp.Payment_Voucher_Main_Voucher_Prefix + "='"+ shift->voucherPrefix+"'";
    qStr += " and "+pHelp.Payment_Voucher_Main_Ledger_ID+ "='"+ shift->cashID+"'";
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

float DailyClosingDatabaseHelper::getReceiptByCash(ShiftDataModel *shift)
{
    float vNO;
    ReceiptVoucherDatabaseHelper pHelp;
    QString timestamp = "concat("+pHelp.Receipt_Voucher_Main_Voucher_Date+",' ',CAST("
            +pHelp.Receipt_Voucher_Main_TimeStamp+" AS TIME))";

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

float DailyClosingDatabaseHelper::getZomatoSalesTotal(ShiftDataModel *shift)
{
    float vNO;

    QString qStr ="SELECT SUM("+shelper.Sales_Invoice_Main_Total+") FROM ";
    qStr += shelper.Sales_Invoice_Main_TableName + " WHERE ";
    qStr += shelper.Sales_Invoice_Main_Voucher_Prefix + "='"+ shift->voucherPrefix+"'";
    qStr += " and "+shelper.Sales_Invoice_Main_Billing_Name +"='zomato'";

    //    qStr += " and "+shelper.Sales_Invoice_Main_Ledger_ID+ "='"+ shift->cashID+"'";
    qStr += " and ";
    qStr += shelper.Sales_Invoice_Main_Voucher_No + ">= '" + shift->BillsFrom +"'";
    qStr += " and ";
    qStr += shelper.Sales_Invoice_Main_Voucher_No + "<= '" + shift->BillsTo+"'";



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

float DailyClosingDatabaseHelper::getZomatoSalesByCash(ShiftDataModel *shift)
{
    float vNO;
    QString qStr ="SELECT SUM("+shelper.Sales_Invoice_Main_Total+") FROM ";
    qStr += shelper.Sales_Invoice_Main_TableName + " WHERE ";
    qStr += shelper.Sales_Invoice_Main_Voucher_Prefix + "='"+ shift->voucherPrefix+"'";
    qStr += " and "+shelper.Sales_Invoice_Main_Billing_Name +"='zomato'";

    qStr += " and "+shelper.Sales_Invoice_Main_Ledger_ID+ "='"+ shift->cashID+"'";

    qStr += " and ";
    qStr += shelper.Sales_Invoice_Main_Voucher_No + ">= '" + shift->BillsFrom +"'";
    qStr += " and ";
    qStr += shelper.Sales_Invoice_Main_Voucher_No + "<= '" + shift->BillsTo+"'";


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

float DailyClosingDatabaseHelper::getZomatoSalesByCredit(ShiftDataModel *shift)
{
    float vNO;

    QString zomatoId = "";
    LedgerMasterDatabaseHelper *led = new LedgerMasterDatabaseHelper;
    zomatoId = led->getLedgerIDByName("Zomato");

    QString qStr ="SELECT SUM("+shelper.Sales_Invoice_Main_Total+") FROM ";
    qStr += shelper.Sales_Invoice_Main_TableName + " WHERE ";
    qStr += shelper.Sales_Invoice_Main_Voucher_Prefix + "='"+ shift->voucherPrefix+"'";
    qStr += " and "+shelper.Sales_Invoice_Main_Billing_Name +"='zomato'";

    qStr += " and "+shelper.Sales_Invoice_Main_Ledger_ID+ "='"+ zomatoId+"'";
    qStr += " and ";
    qStr += shelper.Sales_Invoice_Main_Voucher_No + ">= '" + shift->BillsFrom +"'";
    qStr += " and ";
    qStr += shelper.Sales_Invoice_Main_Voucher_No + "<= '" + shift->BillsTo+"'";

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

QList<ShiftDataModel> DailyClosingDatabaseHelper::getAllShiftDetailsByDate(QDate dateFrom, QDate dateTo)
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
    qStr += " where "+Shift_Start_DateTime +"<= '" + dateFrom.toString("yyyy-MM-dd hh:mm:ss") + "'";
    qStr += " and "  +Shift_End_DateTime +">= '"+ dateTo.toString("yyyy-MM-dd hh:mm:ss") + "'";


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


            shiftList.append(shiftList);
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }

    return shiftList;
}

ShiftDataModel DailyClosingDatabaseHelper::getlastShiftDetailByVoucherPrefix(QString prefix)
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
    qStr += " where "+Shift_voucherPrefix +"= '"+prefix+"'";
    qStr += " and "+Shift_ID +" in (select max("+Shift_ID+") from "+Shifts_TableName+ " where ";
    qStr += Shift_voucherPrefix +"= '"+prefix+"')";

    qDebug()<<qStr;

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

    qDebug()<<qStr;

    return shiftData;
}

ShiftDataModel DailyClosingDatabaseHelper::getlastClosingDetailByVoucherPrefix()
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

    qDebug()<<qStr;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);

    if(query.exec()){
        while(query.next()){
            int i =0;
            shiftData.SalesTotal    = query.value(i++).toFloat();
            shiftData.CashSales     = query.value(i++).toFloat();
            shiftData.CardSales     = query.value(i++).toFloat();
            shiftData.CreditSales   = query.value(i++).toFloat();
            shiftData.ZomatoSales   = query.value(i++).toFloat();
            shiftData.ZomatoCash    = query.value(i++).toFloat();
            shiftData.ZomatoCredit  = query.value(i++).toFloat();
            shiftData.PurchaseTotal = query.value(i++).toFloat();
            shiftData.CashPurchase  = query.value(i++).toFloat();
            shiftData.CashPayments  = query.value(i++).toFloat();
            shiftData.CashReceipts  = query.value(i++).toFloat();

            shiftData.cashierID     = query.value(i++).toInt();
            shiftData.voucherPrefix = query.value(i++).toString();
            shiftData.MaciD         = query.value(i++).toString();
            shiftData.POSNumber     = query.value(i++).toString();
            shiftData.PaxCount      = query.value(i++).toInt();
            shiftData.Narration     = query.value(i++).toString();
            shiftData.ShiftStartDateTime= query.value(i++).toDateTime();
            shiftData.ShiftEndDateTime= query.value(i++).toDateTime();

            shiftData.totalCash     = query.value(i++).toFloat();
            shiftData.CashInCounter = query.value(i++).toFloat();
            shiftData.CashWithDrawn = query.value(i++).toFloat();
            shiftData.CashBalance   = query.value(i++).toFloat();
            shiftData.TillDifference= query.value(i++).toFloat();
            shiftData.OpeningCash   = query.value(i++).toFloat();

            shiftData.shiftID       = query.value(i++).toInt();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }

    qDebug()<<qStr;

    return shiftData;
}

ShiftDataModel *DailyClosingDatabaseHelper::getLastClosingDetails()
{

    QString qStr = "SELECT coalesce(max(" + Shift_End_DateTime + "), '2017-01-01 00:00:00') FROM " + Shifts_TableName;
    ShiftDataModel *shift = new ShiftDataModel;
    shift->ShiftStartDateTime = DatabaseHelper::getSingletonQueryResult(qStr).toDateTime();

    qStr = "SELECT max(ShiftEnd_DateTime) FROM shifttable ";
    shift->ShiftEndDateTime = DatabaseHelper::getSingletonQueryResult(qStr).toDateTime();
    qDebug()<<Q_FUNC_INFO<<shift->ShiftStartDateTime;
    return shift;
}
