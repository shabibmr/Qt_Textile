#include "restaurantshiftdatabasehelper.h"

RestaurantShiftDatabaseHelper::RestaurantShiftDatabaseHelper()
{
    db = new DatabaseHelper();
    trans = new TransactionHelper();
}

int RestaurantShiftDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

int RestaurantShiftDatabaseHelper::insertShiftData(ShiftDataModel *shiftData)
{
    this->startTransaction();
    QMap<QString,QVariant> valuesMap;

    valuesMap.insert(Shift_StartVoucher_No,shiftData->BillsFrom);
    valuesMap.insert(Shift_Stopvoucher_No,shiftData->BillsTo);
    valuesMap.insert(Shift_ShiftTimeStamp,QDateTime::currentDateTime());

    valuesMap.insert(Shift_OpeningBalance,shiftData->OpeningCash);

    valuesMap.insert(Shift_SalesValue,shiftData->SalesTotal);
    valuesMap.insert(Shift_CashSales,shiftData->CashSales);
    valuesMap.insert(Shift_CardSales,shiftData->CardSales);
    valuesMap.insert(Shift_CreditSales,shiftData->CreditSales);
//    valuesMap.insert(Shift_ZomatoSales,shiftData->ZomatoSales);
//    valuesMap.insert(Shift_ZomatoCash,shiftData->ZomatoCash);
//    valuesMap.insert(Shift_ZomatoCredit,shiftData->ZomatoCredit);
    valuesMap.insert(Shift_PurchaseTotal,shiftData->PurchaseTotal);
    valuesMap.insert(Shift_PurchaseCash,shiftData->CashPurchase);
    valuesMap.insert(Shift_PurchaseCard,shiftData->CardPurchase);
    valuesMap.insert(Shift_PurchaseCredit,shiftData->CreditPurchase);
    valuesMap.insert(Shift_PaymentCash,shiftData->CashPayments);
    valuesMap.insert(Shift_PaymentTotal, shiftData->PaymentsTotal);
    valuesMap.insert(Shift_PaymentCard, shiftData->CardPayments);
    valuesMap.insert(Shift_ReceiptCash,shiftData->CashReceipts);
    valuesMap.insert(Shift_ReceiptTotal, shiftData->ReceiptTotal);
    valuesMap.insert(Shift_ReceiptCard, shiftData->CardReceipts);

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

    qDebug()<<"22222";
    int id = db->insertRecordsPlus(valuesMap,Shifts_TableName);
    qDebug()<<id;
    this->commit();

    return id;
}

void RestaurantShiftDatabaseHelper::updateShiftData(ShiftDataModel *shiftData, int shiftID)
{
    this->startTransaction();

    QMap<QString,QVariant> valuesMap;

    valuesMap.insert(Shift_StartVoucher_No,shiftData->BillsFrom);
    valuesMap.insert(Shift_Stopvoucher_No,shiftData->BillsTo);
    valuesMap.insert(Shift_ShiftTimeStamp,QDateTime::currentDateTime());
    valuesMap.insert(Shift_SalesValue,shiftData->SalesTotal);
    valuesMap.insert(Shift_CashSales,shiftData->CashSales);
    valuesMap.insert(Shift_CardSales,shiftData->CardSales);
    valuesMap.insert(Shift_CreditSales,shiftData->CreditSales);
    valuesMap.insert(Shift_PurchaseTotal,shiftData->PurchaseTotal);
    valuesMap.insert(Shift_PurchaseCash,shiftData->CashPurchase);
    valuesMap.insert(Shift_PurchaseCard,shiftData->CardPurchase);
    valuesMap.insert(Shift_PurchaseCredit,shiftData->CreditPurchase);
    valuesMap.insert(Shift_PaymentCash,shiftData->CashPayments);
    valuesMap.insert(Shift_PaymentTotal, shiftData->PaymentsTotal);
    valuesMap.insert(Shift_PaymentCard, shiftData->CardPayments);
    valuesMap.insert(Shift_ReceiptCash,shiftData->CashReceipts);
    valuesMap.insert(Shift_ReceiptTotal, shiftData->ReceiptTotal);
    valuesMap.insert(Shift_ReceiptCard, shiftData->CardReceipts);
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

    this->commit();
}

void RestaurantShiftDatabaseHelper::deleteShift(int shiftID)
{
    this->startTransaction();
    QMap<QString,QVariant> clauseMap;
    clauseMap.insert(Shift_ID,shiftID);
    db->deleteRecordsPlus(clauseMap,Shifts_TableName);
    this->commit();
}

QString RestaurantShiftDatabaseHelper::getMaxID()
{

}

int RestaurantShiftDatabaseHelper::commit()
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

QString RestaurantShiftDatabaseHelper::getFirstBillinShift(ShiftDataModel *shift)
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

QString RestaurantShiftDatabaseHelper::getFirstBillByDate(ShiftDataModel *shift)
{
    QString vNO;
    salesVoucherDatabaseHelper shelper;
    QString qStr ="SELECT MAX(CAST(`"+shelper.Sales_Invoice_Main_Voucher_No+"` AS UNSIGNED)) FROM "+shelper.Sales_Invoice_Main_TableName
            + " where ";
    qStr += shelper.Sales_Invoice_Main_TimeStamp +"<" + db->getDateTimeString(shift->ShiftStartDateTime);
    qStr += " and " + shelper.Sales_Invoice_Main_Voucher_Prefix + " like '" + shift->voucherPrefix + "'";

    qDebug()<<"FIRST BILL  "<<qStr;
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

QString RestaurantShiftDatabaseHelper::getLastBillByDate(ShiftDataModel *shift)
{
    QString vNO;
    salesVoucherDatabaseHelper shelper;
    QString qStr ="SELECT MAX(CAST(`"+shelper.Sales_Invoice_Main_Voucher_No+"` AS UNSIGNED)) FROM ";
    qStr +=shelper.Sales_Invoice_Main_TableName+ " where ";
    qStr += shelper.Sales_Invoice_Main_TimeStamp +"<'"+shift->ShiftEndDateTime.toString("yyyy-MM-dd hh:mm:ss")+"'";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);
    qDebug()<<"get last bill"<<qStr;
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


QString RestaurantShiftDatabaseHelper::getLastBill(ShiftDataModel *shift, QDateTime dateTime)
{
    QString vNO;
    salesVoucherDatabaseHelper shelper;
    QString qStr ="select max(cast("+shelper.Sales_Invoice_Main_Voucher_No+" as unsigned)) from ";
    qStr += shelper.Sales_Invoice_Main_TableName;
    qStr += " where "+shelper.Sales_Invoice_Main_Voucher_Prefix +" like '" + shift->voucherPrefix + "'";
    qStr += " and ";
    qStr += shelper.Sales_Invoice_Main_TimeStamp + "<=";
    qStr += DatabaseHelper::getDateTimeStringStatic(dateTime);

    qDebug()<<qStr;
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

void RestaurantShiftDatabaseHelper::truncateKotNumberTable()
{
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    startTransaction();
    query.exec("TRUNCATE TABLE `kotnumbertable`");
    query.exec("Truncate table kot_details");
    query.exec("Truncate table kot_details_deleted");

    commit();
    //    SalesOrderDatabaseHelper so;
    //    query.exec("TRUNCATE TABLE `"+so.Sales_Order_Main_TableName+"`");
    //    query.exec("TRUNCATE TABLE `"+so.Sales_Order_Details_TableName+"`");

}

int RestaurantShiftDatabaseHelper::getBillsCount(ShiftDataModel *shift)
{
    int vNO;

    QString qStr ="select count('"+shelper.Sales_Invoice_Main_Voucher_No+"') from "+shelper.Sales_Invoice_Main_TableName;
    qStr += " where "+shelper.Sales_Invoice_Main_Voucher_Prefix +" like '" + shift->voucherPrefix + "'";
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

float RestaurantShiftDatabaseHelper::getOpeningCash(ShiftDataModel *shift)
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

float RestaurantShiftDatabaseHelper::getSalesTotal(ShiftDataModel *shift)
{
    float vNO;

    QString qStr ="SELECT SUM("+shelper.Sales_Invoice_Main_Total+") FROM ";
    qStr += shelper.Sales_Invoice_Main_TableName + " WHERE ";
    qStr += shelper.Sales_Invoice_Main_Voucher_Prefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " and ";
    qStr += "CAST("+shelper.Sales_Invoice_Main_Voucher_No+" AS UNSIGNED)" + ">= CAST(" + shift->BillsFrom +" AS UNSIGNED)";
    qStr += " and ";
    qStr += "CAST("+shelper.Sales_Invoice_Main_Voucher_No+" AS UNSIGNED)" + "<= CAST(" + shift->BillsTo+ " AS UNSIGNED)";

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

float RestaurantShiftDatabaseHelper::getSalesTotalByVoucherDate(ShiftDataModel *shift)
{
    float vNO = 0.0;

    QString qStr ="SELECT SUM("+shelper.Sales_Invoice_Main_Total+") FROM ";
    qStr += shelper.Sales_Invoice_Main_TableName + " WHERE ";
    qStr += shelper.Sales_Invoice_Main_Voucher_Prefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " and ";
    qStr += shelper.Sales_Invoice_Main_TimeStamp+" >= " + DatabaseHelper::getDateTimeStringStatic(shift->ShiftStartDateTime);
    qStr += " and ";
    qStr += shelper.Sales_Invoice_Main_TimeStamp+" <= " + DatabaseHelper::getDateTimeStringStatic(shift->ShiftEndDateTime);

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    qDebug()<<"sales total"<<qStr;

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

float RestaurantShiftDatabaseHelper::getVatTotal(ShiftDataModel *shift)
{
    float vNO;

    QString qStr ="SELECT SUM("+shelper.Sales_Invoice_Main_VATAmount+") FROM ";
    qStr += shelper.Sales_Invoice_Main_TableName + " WHERE ";
    qStr += shelper.Sales_Invoice_Main_Voucher_Prefix + "  like '"+ shift->voucherPrefix+"'";
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

float RestaurantShiftDatabaseHelper::getInputVatTotal(ShiftDataModel *shift)
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


float RestaurantShiftDatabaseHelper::getSalesByCash(ShiftDataModel *shift)
{
    float vNO;
    QString qStr ="SELECT SUM("+shelper.Sales_Invoice_Main_Total+") FROM ";
    qStr += shelper.Sales_Invoice_Main_TableName + " WHERE ";
    qStr += shelper.Sales_Invoice_Main_Voucher_Prefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " and "+shelper.Sales_Invoice_Main_Ledger_ID+ "='"+ shift->cashID+"'";
    qStr += " and ";
    qStr += "CAST("+shelper.Sales_Invoice_Main_Voucher_No+" AS UNSIGNED)"  + ">= CAST(" + shift->BillsFrom +" AS UNSIGNED)";
    qStr += " and ";
    qStr += "CAST("+shelper.Sales_Invoice_Main_Voucher_No+" AS UNSIGNED)"  + "<= CAST(" + shift->BillsTo +" AS UNSIGNED)";

    qStr = "SELECT ";
    qStr += " SUM("+trans->Transaction_Helper_Debit_Amount+") -";
    qStr += " SUM("+trans->Transaction_Helper_Credit_Amount+")";
    qStr += " FROM " ;
    qStr += trans->Transaction_Helper_TableName;
    qStr += " WHERE " ;
    qStr += trans->Transaction_Helper_VoucherPrefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " AND ";
    qStr += trans->Transaction_Helper_VoucherType + "= 'SALESVOUCHER' ";
    qStr += " AND " + trans->Transaction_Helper_LedgerID ;
    qStr += " like '"+ shift->cashID+"' ";
    qStr += " AND ";
    qStr += "CAST("+trans->Transaction_Helper_VoucherNo+" AS UNSIGNED)"  + ">= CAST(" + shift->BillsFrom +" AS UNSIGNED)";
    qStr += " AND ";
    qStr += "CAST("+trans->Transaction_Helper_VoucherNo+" AS UNSIGNED)"  + "<= CAST(" + shift->BillsTo +" AS UNSIGNED)";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    qDebug()<<"SALES BY CASH "<<qStr;
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


float RestaurantShiftDatabaseHelper::getSalesByCashByVoucherDate(ShiftDataModel *shift)
{
    float vNO;
    QString qStr ="SELECT SUM("+shelper.Sales_Invoice_Main_Total+") FROM ";
    qStr += shelper.Sales_Invoice_Main_TableName + " WHERE ";
    qStr += shelper.Sales_Invoice_Main_Voucher_Prefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " and "+shelper.Sales_Invoice_Main_Ledger_ID+ "='"+ shift->cashID+"'";
    qStr += " and ";
    qStr += "CAST("+shelper.Sales_Invoice_Main_Voucher_No+" AS UNSIGNED)"  + ">= CAST(" + shift->BillsFrom +" AS UNSIGNED)";
    qStr += " and ";
    qStr += "CAST("+shelper.Sales_Invoice_Main_Voucher_No+" AS UNSIGNED)"  + "<= CAST(" + shift->BillsTo +" AS UNSIGNED)";

    qStr = "SELECT ";
    qStr += " SUM("+trans->Transaction_Helper_Debit_Amount+") -";
    qStr += " SUM("+trans->Transaction_Helper_Credit_Amount+")";
    qStr += " FROM " ;
    qStr += trans->Transaction_Helper_TableName;
    qStr += " WHERE " ;
    qStr += trans->Transaction_Helper_VoucherPrefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " AND ";
    qStr += trans->Transaction_Helper_VoucherType + "= 'SALESVOUCHER' ";
    qStr += " AND " + trans->Transaction_Helper_LedgerID ;
    qStr += " like '"+ shift->cashID+"' ";
    qStr += " AND ";
    qStr += trans->Transaction_Helper_TimeStamp+" >= " + DatabaseHelper::getDateTimeStringStatic(shift->ShiftStartDateTime) ;
    qStr += " AND ";
    qStr += trans->Transaction_Helper_TimeStamp+" <= " + DatabaseHelper::getDateTimeStringStatic(shift->ShiftEndDateTime) ;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    qDebug()<<"SALES BY CASH "<<qStr;
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

float RestaurantShiftDatabaseHelper::getSalesByCredit(ShiftDataModel *shift)
{
    float vNO = 0;
    QString sundryDebtorsId = "%";
    LedgerMasterDatabaseHelper ledHelper;
    AccountGroupMasterDatabaseHelper *acchelper = new AccountGroupMasterDatabaseHelper;
    sundryDebtorsId = acchelper->getGroupIDByName("Sundry Debtors");
    sundryDebtorsId=ledHelper.getAllLedgerIDsUnderGroupRecursiveAsString(sundryDebtorsId);

    QString qStr ="SELECT SUM("+shelper.Sales_Invoice_Main_Total+") FROM ";
    qStr += shelper.Sales_Invoice_Main_TableName + " WHERE ";
    qStr += shelper.Sales_Invoice_Main_Voucher_Prefix + " like '"+ shift->voucherPrefix+"'";
    qStr += " and "+shelper.Sales_Invoice_Main_Ledger_ID+ " in ("+ sundryDebtorsId+")";
    qStr += " and ";
    qStr += "CAST("+shelper.Sales_Invoice_Main_Voucher_No+" AS UNSIGNED)"  + ">= CAST(" + shift->BillsFrom +" AS UNSIGNED)";
    qStr += " and ";
    qStr += "CAST("+shelper.Sales_Invoice_Main_Voucher_No+" AS UNSIGNED)"  + "<= CAST(" + shift->BillsTo +" AS UNSIGNED)";



    qStr = "SELECT ";
    qStr += " SUM("+trans->Transaction_Helper_Debit_Amount+") -";
    qStr += " SUM("+trans->Transaction_Helper_Credit_Amount+")";
    qStr += " FROM " ;
    qStr += trans->Transaction_Helper_TableName;
    qStr += " WHERE " ;
    qStr += trans->Transaction_Helper_VoucherPrefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " AND ";
    qStr += trans->Transaction_Helper_VoucherType + "= 'SALESVOUCHER' ";
    qStr += " AND " + trans->Transaction_Helper_LedgerID ;
    qStr += " in ("+ sundryDebtorsId+") ";
    qStr += " AND ";
    qStr += "CAST("+trans->Transaction_Helper_VoucherNo+" AS UNSIGNED)"  + ">= CAST(" + shift->BillsFrom +" AS UNSIGNED)";
    qStr += " AND ";
    qStr += "CAST("+trans->Transaction_Helper_VoucherNo+" AS UNSIGNED)"  + "<= CAST(" + shift->BillsTo +" AS UNSIGNED)";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    //    qDebug()<<"Sales Credit";
    query.prepare(qStr);

    qDebug()<<"SALES BY Credit "<<qStr;
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


float RestaurantShiftDatabaseHelper::getSalesByCreditByVoucherDate(ShiftDataModel *shift)
{
    float vNO = 0;
    QString sundryDebtorsId = "%";
    LedgerMasterDatabaseHelper ledHelper;
    AccountGroupMasterDatabaseHelper *acchelper = new AccountGroupMasterDatabaseHelper;
    sundryDebtorsId = acchelper->getGroupIDByName("Sundry Debtors");
    sundryDebtorsId=ledHelper.getAllLedgerIDsUnderGroupRecursiveAsString(sundryDebtorsId);

    QString qStr ="SELECT SUM("+shelper.Sales_Invoice_Main_Total+") FROM ";
    qStr += shelper.Sales_Invoice_Main_TableName + " WHERE ";
    qStr += shelper.Sales_Invoice_Main_Voucher_Prefix + " like '"+ shift->voucherPrefix+"'";
    qStr += " and "+shelper.Sales_Invoice_Main_Ledger_ID+ " in ("+ sundryDebtorsId+")";
    qStr += " and ";
    qStr += "CAST("+shelper.Sales_Invoice_Main_TimeStamp+" AS UNSIGNED)"  + ">= CAST(" + shift->BillsFrom +" AS UNSIGNED)";
    qStr += " and ";
    qStr += "CAST("+shelper.Sales_Invoice_Main_TimeStamp+" AS UNSIGNED)"  + "<= CAST(" + shift->BillsTo +" AS UNSIGNED)";



    qStr = "SELECT ";
    qStr += " SUM("+trans->Transaction_Helper_Debit_Amount+") -";
    qStr += " SUM("+trans->Transaction_Helper_Credit_Amount+")";
    qStr += " FROM " ;
    qStr += trans->Transaction_Helper_TableName;
    qStr += " WHERE " ;
    qStr += trans->Transaction_Helper_VoucherPrefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " AND ";
    qStr += trans->Transaction_Helper_VoucherType + "= 'SALESVOUCHER' ";
    qStr += " AND " + trans->Transaction_Helper_LedgerID ;
    qStr += " in ("+ sundryDebtorsId+") ";
    qStr += " AND ";
    qStr += trans->Transaction_Helper_TimeStamp+" >= " + DatabaseHelper::getDateTimeStringStatic(shift->ShiftStartDateTime) ;
    qStr += " AND ";
    qStr += trans->Transaction_Helper_TimeStamp+" <= " + DatabaseHelper::getDateTimeStringStatic(shift->ShiftEndDateTime) ;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

       qDebug()<<"Sales Credit";
    query.prepare(qStr);

    qDebug()<<"SALES BY Credit "<<qStr;
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


float RestaurantShiftDatabaseHelper::getSalesByCard(ShiftDataModel *shift)
{
    LedgerMasterDatabaseHelper ledHelper;

    float vNO =0;
    QString BankAccountsId = "";
    AccountGroupMasterDatabaseHelper *acchelper = new AccountGroupMasterDatabaseHelper;
    BankAccountsId = acchelper->getGroupIDByName("Bank Accounts");
    BankAccountsId= ledHelper.getAllLedgerIDsUnderGroupRecursiveAsString(BankAccountsId);

    QString qStr ="SELECT SUM("+shelper.Sales_Invoice_Main_Total+") FROM ";
    qStr += shelper.Sales_Invoice_Main_TableName + " WHERE ";
    qStr += shelper.Sales_Invoice_Main_Voucher_Prefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " and "+shelper.Sales_Invoice_Main_Ledger_ID+ " in ("+BankAccountsId+")";
    qStr += " and ";
    qStr += "CAST("+shelper.Sales_Invoice_Main_Voucher_No+" AS UNSIGNED)"  + ">= CAST(" + shift->BillsFrom +" AS UNSIGNED)";
    qStr += " and ";
    qStr += "CAST("+shelper.Sales_Invoice_Main_Voucher_No+" AS UNSIGNED)"  + "<= CAST(" + shift->BillsTo +" AS UNSIGNED)";


    qStr = "SELECT ";
    qStr += " SUM("+trans->Transaction_Helper_Debit_Amount+") -";
    qStr += " SUM("+trans->Transaction_Helper_Credit_Amount+")";
    qStr += " FROM " ;
    qStr += trans->Transaction_Helper_TableName;
    qStr += " WHERE " ;
    qStr += trans->Transaction_Helper_VoucherPrefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " AND ";
    qStr += trans->Transaction_Helper_VoucherType + "= 'SALESVOUCHER' ";
    qStr += " AND " + trans->Transaction_Helper_LedgerID ;
    qStr += " in ("+ BankAccountsId+") ";
    qStr += " AND ";
    qStr += "CAST("+trans->Transaction_Helper_VoucherNo+" AS UNSIGNED)"  + ">= CAST(" + shift->BillsFrom +" AS UNSIGNED)";
    qStr += " AND ";
    qStr += "CAST("+trans->Transaction_Helper_VoucherNo+" AS UNSIGNED)"  + "<= CAST(" + shift->BillsTo +" AS UNSIGNED)";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);

    qDebug()<<"SALES BY Card "<<qStr;

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



float RestaurantShiftDatabaseHelper::getSalesByCardByVoucherDate(ShiftDataModel *shift)
{
    LedgerMasterDatabaseHelper ledHelper;

    float vNO =0;
    QString BankAccountsId = "";
    AccountGroupMasterDatabaseHelper *acchelper = new AccountGroupMasterDatabaseHelper;
    BankAccountsId = acchelper->getGroupIDByName("Bank Accounts");
    BankAccountsId= ledHelper.getAllLedgerIDsUnderGroupRecursiveAsString(BankAccountsId);

    QString qStr ="SELECT SUM("+shelper.Sales_Invoice_Main_Total+") FROM ";
    qStr += shelper.Sales_Invoice_Main_TableName + " WHERE ";
    qStr += shelper.Sales_Invoice_Main_Voucher_Prefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " and "+shelper.Sales_Invoice_Main_Ledger_ID+ " in ("+BankAccountsId+")";
    qStr += " and ";
    qStr += "CAST("+shelper.Sales_Invoice_Main_Voucher_No+" AS UNSIGNED)"  + ">= CAST(" + shift->BillsFrom +" AS UNSIGNED)";
    qStr += " and ";
    qStr += "CAST("+shelper.Sales_Invoice_Main_Voucher_No+" AS UNSIGNED)"  + "<= CAST(" + shift->BillsTo +" AS UNSIGNED)";


    qStr = "SELECT ";
    qStr += " SUM("+trans->Transaction_Helper_Debit_Amount+") -";
    qStr += " SUM("+trans->Transaction_Helper_Credit_Amount+")";
    qStr += " FROM " ;
    qStr += trans->Transaction_Helper_TableName;
    qStr += " WHERE " ;
    qStr += trans->Transaction_Helper_VoucherPrefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " AND ";
    qStr += trans->Transaction_Helper_VoucherType + "= 'SALESVOUCHER' ";
    qStr += " AND " + trans->Transaction_Helper_LedgerID ;
    qStr += " in ("+ BankAccountsId+") ";
    qStr += " AND ";
    qStr += trans->Transaction_Helper_TimeStamp+" >= " + DatabaseHelper::getDateTimeStringStatic(shift->ShiftStartDateTime) ;
    qStr += " AND ";
    qStr += trans->Transaction_Helper_TimeStamp+" <= " + DatabaseHelper::getDateTimeStringStatic(shift->ShiftEndDateTime) ;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);

    qDebug()<<"SALES BY Card "<<qStr;

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

float RestaurantShiftDatabaseHelper::getPurchaseTotal(ShiftDataModel *shift)
{
    float vNO=0;
    PurchaseVoucherDatabaseHelper pHelp;
    QString timestamp = "concat("+pHelp.Purchase_Invoice_Main_Voucher_Date+",' ',CAST("
            +pHelp.Purchase_Invoice_Main_TimeStamp+" AS TIME))";
    timestamp = pHelp.Purchase_Invoice_Main_TimeStamp;
    QString qStr ="SELECT SUM("+pHelp.Purchase_Invoice_Main_Total+") FROM ";

    qStr += pHelp.Purchase_Invoice_Main_TableName + " WHERE ";
    qStr += pHelp.Purchase_Invoice_Main_Voucher_Prefix + " like '" + shift->voucherPrefix + "'";

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

float RestaurantShiftDatabaseHelper::getPurchaseByCash(ShiftDataModel *shift)
{
    float vNO=0;
    PurchaseVoucherDatabaseHelper pHelp;

    //    QString timestamp = "concat("+pHelp.Purchase_Invoice_Main_Voucher_Date+",' ',CAST("
    //            +pHelp.Purchase_Invoice_Main_TimeStamp+" AS TIME))";
    QString timestamp = pHelp.Purchase_Invoice_Main_TimeStamp;

    QString qStr ="SELECT SUM("+pHelp.Purchase_Invoice_Main_Total+") FROM ";

    qStr += pHelp.Purchase_Invoice_Main_TableName + " WHERE ";
    qStr += pHelp.Purchase_Invoice_Main_Voucher_Prefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " and "+pHelp.Purchase_Invoice_Main_Ledger_ID+ "='"+ shift->cashID+"'";
    qStr += " and ";
    qStr += " ( ";

    qStr += " ( ";
    qStr += timestamp + ">'" + shift->ShiftStartDateTime.toString("yyyy-MM-dd hh:mm:ss") + "'";
    qStr += " and ";
    qStr += timestamp + "< '" + shift->ShiftEndDateTime.toString("yyyy-MM-dd hh:mm:ss") + "'";
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


float RestaurantShiftDatabaseHelper::getPurchaseByCard(ShiftDataModel *shift)
{
    float vNO=0;
    AccountGroupMasterDatabaseHelper *acchelper = new AccountGroupMasterDatabaseHelper;
    LedgerMasterDatabaseHelper ledHelper;
    QString BankAccountsId = acchelper->getGroupIDByName("Bank Accounts");
    BankAccountsId= ledHelper.getAllLedgerIDsUnderGroupRecursiveAsString(BankAccountsId);
    PurchaseVoucherDatabaseHelper pHelp;

    //    QString timestamp = "concat("+pHelp.Purchase_Invoice_Main_Voucher_Date+",' ',CAST("
    //            +pHelp.Purchase_Invoice_Main_TimeStamp+" AS TIME))";
    QString timestamp = pHelp.Purchase_Invoice_Main_TimeStamp;

    QString qStr ="SELECT SUM("+pHelp.Purchase_Invoice_Main_Total+") FROM ";

    qStr += pHelp.Purchase_Invoice_Main_TableName + " WHERE ";
    qStr += pHelp.Purchase_Invoice_Main_Voucher_Prefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " and "+pHelp.Purchase_Invoice_Main_Ledger_ID+ " in ("+ BankAccountsId+")";
    qStr += " and ";
    qStr += " ( ";

    qStr += " ( ";
    qStr += timestamp + ">'" + shift->ShiftStartDateTime.toString("yyyy-MM-dd hh:mm:ss") + "'";
    qStr += " and ";
    qStr += timestamp + "< '" + shift->ShiftEndDateTime.toString("yyyy-MM-dd hh:mm:ss") + "'";
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

float RestaurantShiftDatabaseHelper::getPurchaseByCredit(ShiftDataModel *shift)
{
    float vNO=0;
    QString sundryCreditorsId = "";
    LedgerMasterDatabaseHelper ledHelper;
    AccountGroupMasterDatabaseHelper *acchelper = new AccountGroupMasterDatabaseHelper;
    sundryCreditorsId = acchelper->getGroupIDByName("Sundry Creditors") ;
    sundryCreditorsId=ledHelper.getAllLedgerIDsUnderGroupRecursiveAsString(sundryCreditorsId);


    PurchaseVoucherDatabaseHelper pHelp;
    QString timestamp = "concat("+pHelp.Purchase_Invoice_Main_Voucher_Date+",' ',CAST("
            +pHelp.Purchase_Invoice_Main_TimeStamp+" AS TIME))";
    timestamp = pHelp.Purchase_Invoice_Main_TimeStamp;
    QString qStr ="SELECT SUM("+pHelp.Purchase_Invoice_Main_Total+") FROM ";

    qStr += pHelp.Purchase_Invoice_Main_TableName + " WHERE ";
    qStr += pHelp.Purchase_Invoice_Main_Voucher_Prefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " and "+pHelp.Purchase_Invoice_Main_Ledger_ID+ " in ("+sundryCreditorsId+")";
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

float RestaurantShiftDatabaseHelper::getPaymentByCash(ShiftDataModel *shift)
{
    float vNO=0;
    PaymentVoucherDatabaseHelper pHelp;

    QString timestamp = "concat("+pHelp.Payment_Voucher_Main_Voucher_Date+",' ',CAST("
            +pHelp.Payment_Voucher_Main_TimeStamp+" AS TIME))";
    timestamp = pHelp.Payment_Voucher_Main_TimeStamp;

    QString qStr ="SELECT SUM("+pHelp.Payment_Voucher_Main_Total+") FROM ";

    qStr += pHelp.Payment_Voucher_Main_TableName + " WHERE ";
    qStr += pHelp.Payment_Voucher_Main_Voucher_Prefix + "  like '"+ shift->voucherPrefix+"'";
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


float RestaurantShiftDatabaseHelper::getPaymentByCard(ShiftDataModel *shift)
{
    float vNO=0;
    AccountGroupMasterDatabaseHelper *acchelper = new AccountGroupMasterDatabaseHelper;
    LedgerMasterDatabaseHelper ledHelper;
    QString BankAccountsId = acchelper->getGroupIDByName("Bank Accounts");
    BankAccountsId= ledHelper.getAllLedgerIDsUnderGroupRecursiveAsString(BankAccountsId);
    PaymentVoucherDatabaseHelper pHelp;

    QString timestamp = "concat("+pHelp.Payment_Voucher_Main_Voucher_Date+",' ',CAST("
            +pHelp.Payment_Voucher_Main_TimeStamp+" AS TIME))";
    timestamp = pHelp.Payment_Voucher_Main_TimeStamp;

    QString qStr ="SELECT SUM("+pHelp.Payment_Voucher_Main_Total+") FROM ";

    qStr += pHelp.Payment_Voucher_Main_TableName + " WHERE ";
    qStr += pHelp.Payment_Voucher_Main_Voucher_Prefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " and "+pHelp.Payment_Voucher_Main_Ledger_ID+ " in ("+ BankAccountsId+") ";
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
    qDebug()<<Q_FUNC_INFO<<__LINE__<<qStr;

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


float RestaurantShiftDatabaseHelper::getReceiptByCard(ShiftDataModel *shift)
{
    float vNO=0;
    AccountGroupMasterDatabaseHelper *acchelper = new AccountGroupMasterDatabaseHelper;
    LedgerMasterDatabaseHelper ledHelper;
    QString BankAccountsId = acchelper->getGroupIDByName("Bank Accounts");
    BankAccountsId= ledHelper.getAllLedgerIDsUnderGroupRecursiveAsString(BankAccountsId);
    ReceiptVoucherDatabaseHelper pHelp;

    QString timestamp = "concat("+pHelp.Receipt_Voucher_Main_Voucher_Date+",' ',CAST("
            +pHelp.Receipt_Voucher_Main_TimeStamp+" AS TIME))";
    timestamp = pHelp.Receipt_Voucher_Main_TimeStamp;

    QString qStr ="SELECT SUM("+pHelp.Receipt_Voucher_Main_Total+") FROM ";

    qStr += pHelp.Receipt_Voucher_Main_TableName + " WHERE ";
    qStr += pHelp.Receipt_Voucher_Main_Voucher_Prefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " and "+pHelp.Receipt_Voucher_Main_Ledger_ID+ " in ("+ BankAccountsId+") ";
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



float RestaurantShiftDatabaseHelper::getSalesReturnByCash(ShiftDataModel *shift)
{
    float vNO=0;
    CreditNoteVoucherDatabaseHelper pHelp;

    QString timestamp = "concat("+pHelp.Credit_Note_Main_Voucher_Date+",' ',CAST("
            +pHelp.Credit_Note_Main_TimeStamp+" AS TIME))";
    timestamp = pHelp.Credit_Note_Main_TimeStamp;

    QString qStr ="SELECT SUM("+pHelp.Credit_Note_Main_Total+") FROM ";

    qStr += pHelp.Credit_Note_Main_TableName + " WHERE ";
    qStr += pHelp.Credit_Note_Main_Voucher_Prefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " and "+pHelp.Credit_Note_Main_Ledger_ID+ "='"+ shift->cashID+"'";
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


float RestaurantShiftDatabaseHelper::getPurchaseReturnByCash(ShiftDataModel *shift)
{
    float vNO=0;
    DebitNoteVoucherDatabaseHelper pHelp;

    QString timestamp = "concat("+pHelp.Debit_Note_Main_Voucher_Date+",' ',CAST("
            +pHelp.Debit_Note_Main_TimeStamp+" AS TIME))";
    timestamp = pHelp.Debit_Note_Main_TimeStamp;

    QString qStr ="SELECT SUM("+pHelp.Debit_Note_Main_Total+") FROM ";

    qStr += pHelp.Debit_Note_Main_TableName + " WHERE ";
    qStr += pHelp.Debit_Note_Main_Voucher_Prefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " and "+pHelp.Debit_Note_Main_Ledger_ID+ "='"+ shift->cashID+"'";
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


float RestaurantShiftDatabaseHelper::getTotalSalesReturn(ShiftDataModel *shift)
{
    float vNO=0;
    CreditNoteVoucherDatabaseHelper pHelp;

    QString timestamp = "concat("+pHelp.Credit_Note_Main_Voucher_Date+",' ',CAST("
            +pHelp.Credit_Note_Main_TimeStamp+" AS TIME))";
    timestamp = pHelp.Credit_Note_Main_TimeStamp;

    QString qStr ="SELECT SUM("+pHelp.Credit_Note_Main_Total+") FROM ";

    qStr += pHelp.Credit_Note_Main_TableName + " WHERE ";
    qStr += pHelp.Credit_Note_Main_Voucher_Prefix + "  like '"+ shift->voucherPrefix+"'";
//    qStr += " and "+pHelp.Credit_Note_Main_Ledger_ID+ "='"+ shift->cashID+"'";
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


float RestaurantShiftDatabaseHelper::getTotalPurchaseReturn(ShiftDataModel *shift)
{
    float vNO=0;
    DebitNoteVoucherDatabaseHelper pHelp;

    QString timestamp = "concat("+pHelp.Debit_Note_Main_Voucher_Date+",' ',CAST("
            +pHelp.Debit_Note_Main_TimeStamp+" AS TIME))";
    timestamp = pHelp.Debit_Note_Main_TimeStamp;

    QString qStr ="SELECT SUM("+pHelp.Debit_Note_Main_Total+") FROM ";

    qStr += pHelp.Debit_Note_Main_TableName + " WHERE ";
    qStr += pHelp.Debit_Note_Main_Voucher_Prefix + "  like '" + shift->voucherPrefix + "'";
//    qStr += " and "+pHelp.Debit_Note_Main_Ledger_ID+ "='"+ shift->cashID+"'";
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

QSqlQueryModel *RestaurantShiftDatabaseHelper::getShiftReportModelByVoucherPrefix(QDateTime fromDate, QDateTime toDate)
{
    QSqlQueryModel* model = new QSqlQueryModel;

    QString qStr1 = " SELECT _id, voucherPrefix FROM shifttable where ShiftEnd_DateTime > " + db->getDateTimeString(fromDate);
    qDebug()<<Q_FUNC_INFO<<__LINE__<<qStr1;
    model = DatabaseHelper::getQueryModel(qStr1);
    if(model->rowCount() == 0)
        return model;
    QMap<int, QString> idMap;



    for (int i=0; i<model->rowCount(); i++) {
        idMap.insert(model->record(i).value(0).toInt(), model->record(i).value(1).toString());

    }
    qDebug()<<Q_FUNC_INFO<<__LINE__<<idMap<<model->rowCount();


    QString qStr = "select _id, ShiftEnd_DateTime, substr(Title, 3,50) '',  ";

    for(int id:idMap.keys()){
        qStr += "coalesce(MAX(value"+ QString::number(id) +"),0) as " + idMap[id] + ", ";
    }
    qStr += " case when title like ('01%') or title like ('02%') or title like ('03%') or title like ('18%') or title like ('19%') or title like ('24%') then '' else   ";
    for(int id:idMap.keys()){
        qStr += " coalesce(MAX(value"+ QString::number(id) +"),0) + ";
    }
    qStr.chop(2);
    qStr += " end as Total ";
    qStr += " FROM (";


    qStr += " select _id, ShiftEnd_DateTime, Title, VoucherPrefix,  ";
    for(int id:idMap.keys()){
        qStr += "CASE WHEN _id = '" + QString::number(id)  + "' THEN (value) END ";
        qStr += "as value" + QString::number(id) + ", ";
    }
    qStr.chop(2);
    qStr += " FROM ( ";
    qStr += " select _id, ShiftEnd_DateTime, voucherPrefix, title, value from ( ";
    qStr += " select _id, ShiftEnd_DateTime , voucherPrefix, (SELECT name from  Employee_Details  where _id = cashierID) as value, '01Cashier' as title ";
    qStr += " from shifttable where  ( `ShiftEnd_DateTime`> " + db->getDateTimeString(fromDate) + "  and `ShiftEnd_DateTime`<=  " + db->getDateTimeString(toDate) + " ) ";
    qStr += " union all ";
    qStr += " select _id, ShiftEnd_DateTime , voucherPrefix, `StartVoucherNo` as value, '02From Bill#' as title ";
    qStr += " from shifttable where  ( `ShiftEnd_DateTime`> " + db->getDateTimeString(fromDate) + " and `ShiftEnd_DateTime`<=  " + db->getDateTimeString(toDate) + " ) ";
    qStr += " union all ";
    qStr += " select _id, ShiftEnd_DateTime , voucherPrefix, `StopVoucherNo` as value, '03To Bill#' as title ";
    qStr += " from shifttable where  ( `ShiftEnd_DateTime`> " + db->getDateTimeString(fromDate) + "  and `ShiftEnd_DateTime`<=  " + db->getDateTimeString(toDate) + " ) ";
    qStr += " union all ";
    qStr += " select _id, ShiftEnd_DateTime , voucherPrefix, `SalesValue` as value, '04Sales Value' as title ";
    qStr += " from shifttable where  ( `ShiftEnd_DateTime`> " + db->getDateTimeString(fromDate) + "  and `ShiftEnd_DateTime`<=  " + db->getDateTimeString(toDate) + " ) ";
    qStr += " union all ";
    qStr += " select _id, ShiftEnd_DateTime , voucherPrefix, `CashSales` as value, '05Cash Sales' as title ";
    qStr += " from shifttable where  ( `ShiftEnd_DateTime`> " + db->getDateTimeString(fromDate) + "  and `ShiftEnd_DateTime`<=  " + db->getDateTimeString(toDate) + " ) ";
    qStr += " union all ";
    qStr += " select _id, ShiftEnd_DateTime , voucherPrefix, `CardSales` as value, '06Card Sales' as title ";
    qStr += " from shifttable where  ( `ShiftEnd_DateTime`> " + db->getDateTimeString(fromDate) + "  and `ShiftEnd_DateTime`<=  " + db->getDateTimeString(toDate) + " ) ";
    qStr += " union all ";
    qStr += " select _id, ShiftEnd_DateTime , voucherPrefix, `CreditSales` as value, '07Credit Sales' as title ";
    qStr += " from shifttable where  ( `ShiftEnd_DateTime`> " + db->getDateTimeString(fromDate) + "  and `ShiftEnd_DateTime`<=  " + db->getDateTimeString(toDate) + " ) ";
    qStr += " union all ";
    qStr += " select _id, ShiftEnd_DateTime , voucherPrefix, `PaymentCash` as value, '08Payment Cash' as title ";
    qStr += " from shifttable where  ( `ShiftEnd_DateTime`> " + db->getDateTimeString(fromDate) + "  and `ShiftEnd_DateTime`<=  " + db->getDateTimeString(toDate) + " ) ";
    qStr += " union all ";
    qStr += " select _id, ShiftEnd_DateTime , voucherPrefix, `PaymentCard` as value, '09Payment Card' as title ";
    qStr += " from shifttable where  ( `ShiftEnd_DateTime`> " + db->getDateTimeString(fromDate) + "  and `ShiftEnd_DateTime`<=  " + db->getDateTimeString(toDate) + " ) ";
    qStr += " union all ";
    qStr += " select _id, ShiftEnd_DateTime , voucherPrefix, `PaymentTotal` as value, '10Payment Total' as title ";
    qStr += " from shifttable where  ( `ShiftEnd_DateTime`> " + db->getDateTimeString(fromDate) + "  and `ShiftEnd_DateTime`<=  " + db->getDateTimeString(toDate) + " ) ";
    qStr += " union all ";
    qStr += " select _id, ShiftEnd_DateTime , voucherPrefix, `PurchaseCash` as value, '11Purchase Cash' as title ";
    qStr += " from shifttable where  ( `ShiftEnd_DateTime`> " + db->getDateTimeString(fromDate) + "  and `ShiftEnd_DateTime`<=  " + db->getDateTimeString(toDate) + " ) ";
    qStr += " union all ";
    qStr += " select _id, ShiftEnd_DateTime , voucherPrefix, `PurchaseCard` as value, '12Purchase Card' as title ";
    qStr += " from shifttable where  ( `ShiftEnd_DateTime`> " + db->getDateTimeString(fromDate) + "  and `ShiftEnd_DateTime`<=  " + db->getDateTimeString(toDate) + " ) ";
    qStr += " union all ";
    qStr += " select _id, ShiftEnd_DateTime , voucherPrefix, `PurchaseCredit` as value, '13Purchase Credit' as title ";
    qStr += " from shifttable where  ( `ShiftEnd_DateTime`> " + db->getDateTimeString(fromDate) + "  and `ShiftEnd_DateTime`<=  " + db->getDateTimeString(toDate) + " ) ";
    qStr += " union all ";
    qStr += " select _id, ShiftEnd_DateTime , voucherPrefix, `PurchaseTotal` as value, '14Purchase Total' as title ";
    qStr += " from shifttable where  ( `ShiftEnd_DateTime`> " + db->getDateTimeString(fromDate) + "  and `ShiftEnd_DateTime`<=  " + db->getDateTimeString(toDate) + " ) ";
    qStr += " union all ";
    qStr += " select _id, ShiftEnd_DateTime , voucherPrefix, `ReceiptCash` as value, '15Receipt Cash' as title ";
    qStr += " from shifttable where  ( `ShiftEnd_DateTime`> " + db->getDateTimeString(fromDate) + "  and `ShiftEnd_DateTime`<=  " + db->getDateTimeString(toDate) + " ) ";
    qStr += " union all ";
    qStr += " select _id, ShiftEnd_DateTime , voucherPrefix, `ReceiptCard` as value, '16Receipt Card' as title ";
    qStr += " from shifttable where  ( `ShiftEnd_DateTime`> " + db->getDateTimeString(fromDate) + "  and `ShiftEnd_DateTime`<=  " + db->getDateTimeString(toDate) + " ) ";
    qStr += " union all ";
    qStr += " select _id, ShiftEnd_DateTime , voucherPrefix, `ReceiptTotal` as value, '16Receipt Total' as title ";
    qStr += " from shifttable where  ( `ShiftEnd_DateTime`> " + db->getDateTimeString(fromDate) + "  and `ShiftEnd_DateTime`<=  " + db->getDateTimeString(toDate) + " ) ";
    qStr += " union all ";
    qStr += " select _id, ShiftEnd_DateTime , voucherPrefix, `paxCount` as value, '17PAX' as title ";
    qStr += " from shifttable where  ( `ShiftEnd_DateTime`> " + db->getDateTimeString(fromDate) + "  and `ShiftEnd_DateTime`<=  " + db->getDateTimeString(toDate) + " ) ";
    qStr += " union all ";
    qStr += " select _id, ShiftEnd_DateTime , voucherPrefix, `ShiftStart_DateTime` as value, '18Start Time' as title ";
    qStr += " from shifttable where  ( `ShiftEnd_DateTime`> " + db->getDateTimeString(fromDate) + "  and `ShiftEnd_DateTime`<=  " + db->getDateTimeString(toDate) + " ) ";
    qStr += " union all ";
    qStr += " select _id, ShiftEnd_DateTime , voucherPrefix, `ShiftEnd_DateTime` as value, '19End Time' as title ";
    qStr += " from shifttable where  ( `ShiftEnd_DateTime`> " + db->getDateTimeString(fromDate) + "  and `ShiftEnd_DateTime`<=  " + db->getDateTimeString(toDate) + " ) ";
    qStr += " union all ";
    qStr += " select _id, ShiftEnd_DateTime , voucherPrefix, `OpeningBalance` as value, '20Opening Cash' as title ";
    qStr += " from shifttable where  ( `ShiftEnd_DateTime`> " + db->getDateTimeString(fromDate) + "  and `ShiftEnd_DateTime`<=  " + db->getDateTimeString(toDate) + " ) ";
    qStr += " union all ";
    qStr += " select _id, ShiftEnd_DateTime , voucherPrefix, `TotalCash` as value, '20Total Cash' as title ";
    qStr += " from shifttable where  ( `ShiftEnd_DateTime`> " + db->getDateTimeString(fromDate) + "  and `ShiftEnd_DateTime`<=  " + db->getDateTimeString(toDate) + " ) ";
    qStr += " union all ";
    qStr += " select _id, ShiftEnd_DateTime , voucherPrefix, `CasinCounter` as value, '21Cash in Counter' as title ";
    qStr += " from shifttable where  ( `ShiftEnd_DateTime`> " + db->getDateTimeString(fromDate) + "  and `ShiftEnd_DateTime`<=  " + db->getDateTimeString(toDate) + " ) ";
    qStr += " union all ";
    qStr += " select _id, ShiftEnd_DateTime , voucherPrefix, `TillDifference` as value, '22Till Difference' as title ";
    qStr += " from shifttable where  ( `ShiftEnd_DateTime`> " + db->getDateTimeString(fromDate) + "  and `ShiftEnd_DateTime`<=  " + db->getDateTimeString(toDate) + " ) ";
    qStr += " union all ";
    qStr += " select _id, ShiftEnd_DateTime , voucherPrefix, `Narration` as value, '24Narration' as title ";
    qStr += " from shifttable where  ( `ShiftEnd_DateTime`> " + db->getDateTimeString(fromDate) + "  and `ShiftEnd_DateTime`<=  " + db->getDateTimeString(toDate) + " ) ";
    qStr += " union all ";
    qStr += " select _id, ShiftEnd_DateTime , voucherPrefix, `CashWithDrawn` as value, '23Cash Withdrawn' as title ";
    qStr += " from shifttable where  ( `ShiftEnd_DateTime`> " + db->getDateTimeString(fromDate) + "  and `ShiftEnd_DateTime`<=  " + db->getDateTimeString(toDate) + " ) ";
    qStr += " ) A group by _id, title ";
    qStr += " ) B group by  _id, title  ";
    qStr += " ) as final";
    qStr += " GROUP BY  title";
    qStr += " ORDER BY Title";

//    qDebug()<<qStr;
    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}

GeneralVoucherDataObject RestaurantShiftDatabaseHelper::getPaymentDetails(ShiftDataModel *shift)
{
    GeneralVoucherDataObject obj;
    PaymentVoucherDatabaseHelper pHelp;
    LedgerMasterDatabaseHelper ledHelper;

    QString timestamp = "concat("+pHelp.Payment_Voucher_Main_Voucher_Date+",' ',CAST("
            +pHelp.Payment_Voucher_Main_TimeStamp+" AS TIME))";
    timestamp = pHelp.Payment_Voucher_Main_TimeStamp;
    QString qStr="";

    qStr += "SELECT "+pHelp.Payment_Voucher_Details_Ledger_ID;
    qStr += ", "+pHelp.Payment_Voucher_Details_DrAmount;
    qStr += " FROM "+pHelp.Payment_Voucher_Details_TableName;
    qStr += " where "+pHelp.Payment_Voucher_Details_Voucher_No+" in ";
    qStr +="(SELECT "+pHelp.Payment_Voucher_Main_Voucher_No+" FROM ";
    qStr += pHelp.Payment_Voucher_Main_TableName + " WHERE ";
    qStr += pHelp.Payment_Voucher_Main_Voucher_Prefix + "  like '"+ shift->voucherPrefix+"'";
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

GeneralVoucherDataObject RestaurantShiftDatabaseHelper::getPaymentDetailsByVDate(ShiftDataModel *shift)
{
    GeneralVoucherDataObject obj;
    PaymentVoucherDatabaseHelper pHelp;
    LedgerMasterDatabaseHelper ledHelper;

    QString timestamp = "concat("+pHelp.Payment_Voucher_Main_Voucher_Date+",' ',CAST("
            +pHelp.Payment_Voucher_Main_TimeStamp+" AS TIME))";
    timestamp = pHelp.Payment_Voucher_Main_TimeStamp;
    QString qStr="";

    qStr += "SELECT "+pHelp.Payment_Voucher_Details_Ledger_ID;
    qStr += ", "+pHelp.Payment_Voucher_Details_DrAmount;
    qStr += " FROM "+pHelp.Payment_Voucher_Details_TableName;
    qStr += " where "+pHelp.Payment_Voucher_Details_Voucher_No+" in ";
    qStr +="(SELECT "+pHelp.Payment_Voucher_Main_Voucher_No+" FROM ";
    qStr += pHelp.Payment_Voucher_Main_TableName + " WHERE ";
    qStr += pHelp.Payment_Voucher_Main_Voucher_Prefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " and "+pHelp.Payment_Voucher_Main_Ledger_ID+ "='"+ shift->cashID+"'";
    qStr += " and ";
    qStr += " ( ";
    qStr += " ( ";

    //    qStr += pHelp.Payment_Voucher_Main_Voucher_Date + ">="+ shift->ShiftStartDateTime.toString("yyyy-MM-dd");

    qStr += pHelp.Payment_Voucher_Main_Voucher_Date + ">=" + DatabaseHelper::getDateTimeStringStatic( shift->ShiftStartDateTime) + " ";
    qStr += " and ";
    qStr += pHelp.Payment_Voucher_Main_Voucher_Date + "<= " + DatabaseHelper::getDateTimeStringStatic( shift->ShiftEndDateTime) + " ";
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

GeneralVoucherDataObject RestaurantShiftDatabaseHelper::getCreditPaymentDetails(ShiftDataModel *shift)
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
    qStr += pHelp.Payment_Voucher_Main_Voucher_Prefix + "  like '"+ shift->voucherPrefix+"'";
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

GeneralVoucherDataObject RestaurantShiftDatabaseHelper::getCreditSalesDetails(ShiftDataModel *shift)
{
    GeneralVoucherDataObject obj;

    LedgerMasterDatabaseHelper ledHelper;
    QString sundryDebtorIds = "";
    AccountGroupMasterDatabaseHelper *acchelper = new AccountGroupMasterDatabaseHelper;
    sundryDebtorIds = acchelper->getGroupIDByName("Sundry Debtors");
    sundryDebtorIds=ledHelper.getAllLedgerIDsUnderGroupRecursiveAsString(sundryDebtorIds);

    salesVoucherDatabaseHelper pHelp;

    // QString timestamp = "concat("+pHelp.Sales_Invoice_Main_Voucher_Date+",' ','00:00:00')";


    QString qStr ="SELECT SUM("+pHelp.Transaction_Helper_Debit_Amount+")";
    qStr += ", "+pHelp.Transaction_Helper_LedgerID;
    qStr +=" FROM ";
    qStr += pHelp.Transaction_Helper_TableName + " WHERE ";
    qStr += pHelp.Transaction_Helper_VoucherPrefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " and "+pHelp.Transaction_Helper_LedgerID+ " in ("+sundryDebtorIds+")";
    qStr += " and " + trans->Transaction_Helper_VoucherType + " = 'SALESVOUCHER' ";
    qStr += " and ";
    qStr += " ( ";
    qStr += trans->Transaction_Helper_TimeStamp+" >= " + DatabaseHelper::getDateTimeStringStatic(shift->ShiftStartDateTime) ;
    qStr += " AND ";
    qStr += trans->Transaction_Helper_TimeStamp+" <= " + DatabaseHelper::getDateTimeStringStatic(shift->ShiftEndDateTime) ;
    qStr += ")";
    qStr += " group by "+pHelp.Transaction_Helper_LedgerID;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    qDebug()<<"Sales Details"<<qStr<<Q_FUNC_INFO;
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

GeneralVoucherDataObject RestaurantShiftDatabaseHelper::getBankAmountDetails(ShiftDataModel *shift)
{
    GeneralVoucherDataObject obj;

    LedgerMasterDatabaseHelper ledHelper;
    QString sundryDebtorIds = "";
    AccountGroupMasterDatabaseHelper *acchelper = new AccountGroupMasterDatabaseHelper;
    sundryDebtorIds = acchelper->getGroupIDByName("Bank Accounts");
    sundryDebtorIds=ledHelper.getAllLedgerIDsUnderGroupRecursiveAsString(sundryDebtorIds);

    salesVoucherDatabaseHelper pHelp;

    // QString timestamp = "concat("+pHelp.Sales_Invoice_Main_Voucher_Date+",' ','00:00:00')";


    QString qStr ="SELECT SUM("+pHelp.Transaction_Helper_Debit_Amount+")";
    qStr += ", "+pHelp.Transaction_Helper_LedgerID;
    qStr +=" FROM ";

    qStr += pHelp.Transaction_Helper_TableName + " WHERE ";
    qStr += pHelp.Transaction_Helper_VoucherPrefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " and " + pHelp.Transaction_Helper_VoucherType +  " = 'SALESVOUCHER' ";
    qStr += " and "+pHelp.Transaction_Helper_LedgerID+ " in ("+sundryDebtorIds+")";
    qStr += " and ";
    qStr += " ( ";
    qStr += trans->Transaction_Helper_TimeStamp+" >= " + DatabaseHelper::getDateTimeStringStatic(shift->ShiftStartDateTime) ;
    qStr += " AND ";
    qStr += trans->Transaction_Helper_TimeStamp+" <= " + DatabaseHelper::getDateTimeStringStatic(shift->ShiftEndDateTime) ;
    qStr += ")";
    qStr += " group by "+pHelp.Transaction_Helper_LedgerID;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    qDebug()<<"Bank Details"<<qStr;
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

GeneralVoucherDataObject RestaurantShiftDatabaseHelper::getCreditPurchaseDetails(ShiftDataModel *shift)
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


    QString qStr ="SELECT SUM("+pHelp.Transaction_Helper_Credit_Amount+")";
    qStr += ", "+pHelp.Transaction_Helper_LedgerID;
    qStr +=" FROM ";

    qStr += pHelp.Transaction_Helper_TableName + " WHERE ";
    qStr += pHelp.Transaction_Helper_VoucherPrefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " and "+pHelp.Transaction_Helper_LedgerID+ " in ("+sundryCreditorsId+")";
    qStr += " and ";
    qStr += " ( ";
    qStr += "CAST("+pHelp.Transaction_Helper_VoucherNo+" AS UNSIGNED)"  + ">= CAST(" + shift->BillsFrom +" AS UNSIGNED)";
    qStr += " and ";
    qStr += "CAST("+pHelp.Transaction_Helper_VoucherNo+" AS UNSIGNED)"  + "<= CAST(" + shift->BillsTo +" AS UNSIGNED)";
    qStr += ")";
    qStr += " group by "+pHelp.Transaction_Helper_LedgerID;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    qDebug()<<"PurchaseDetails"<<qStr;
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

GeneralVoucherDataObject RestaurantShiftDatabaseHelper::getCreditReceiptDetails(ShiftDataModel *shift)
{
    GeneralVoucherDataObject obj;
    ReceiptVoucherDatabaseHelper pHelp;
    LedgerMasterDatabaseHelper ledHelper;

    QString timestamp = "concat("+pHelp.Receipt_Voucher_Main_Voucher_Date+",' ','00:00:00')";

    //            +pHelp.Purchase_Invoice_Main_TimeStamp+" AS TIME))";
    //    timestamp = pHelp.Purchase_Invoice_Main_TimeStamp;

    QString qStr;

    qStr += "SELECT "+pHelp.Receipt_Voucher_Details_Ledger_ID;
    qStr += ", SUM("+pHelp.Receipt_Voucher_Details_DrAmount+")";
    qStr += " FROM "+pHelp.Receipt_Voucher_Details_TableName;
    qStr += " where "+pHelp.Receipt_Voucher_Details_Voucher_No+" in ";
    qStr +="(SELECT "+pHelp.Receipt_Voucher_Main_Voucher_No+" FROM ";
    qStr += pHelp.Receipt_Voucher_Main_TableName + " WHERE ";
    qStr += pHelp.Receipt_Voucher_Main_Voucher_Prefix + "  like '"+ shift->voucherPrefix+"'";
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

QSqlQueryModel* RestaurantShiftDatabaseHelper::AllLedgerSummary(ShiftDataModel *shift, bool includeCash)
{
    qDebug()<<Q_FUNC_INFO;

    LedgerMasterDatabaseHelper ledHelper;
    QString sundryDebtorIds = "";
    AccountGroupMasterDatabaseHelper *acchelper = new AccountGroupMasterDatabaseHelper;
    sundryDebtorIds = acchelper->getGroupIDByName("Bank Accounts");
    sundryDebtorIds=ledHelper.getAllLedgerIDsUnderGroupRecursiveAsString(sundryDebtorIds);

    qDebug()<<"sundry debtors id"<<sundryDebtorIds;

    salesVoucherDatabaseHelper pHelp;

    // QString timestamp = "concat("+pHelp.Sales_Invoice_Main_Voucher_Date+",' ','00:00:00')";


    QString qStr;
    //    qStr ="SELECT ";

    //    qStr += "(select ledger_name from "+ledHelper.Ledger_Master_TableName+" led where led.ledger_id=sam.";
    //    qStr += pHelp.Sales_Invoice_Main_Ledger_ID;
    //    qStr += ") Ledger";
    //    qStr += ", SUM("+pHelp.Sales_Invoice_Main_Total+") as Amount";

    //    qStr +=" FROM ";
    //    qStr += pHelp.Sales_Invoice_Main_TableName + " sam WHERE ";
    //    qStr += pHelp.Sales_Invoice_Main_Voucher_Prefix + "  like '"+ shift->voucherPrefix+"'";
    //    qStr += " and ";
    //    qStr += " ( ";
    //    qStr += "CAST("+pHelp.Sales_Invoice_Main_Voucher_No+" AS UNSIGNED)"  + ">= CAST(" + shift->BillsFrom +" AS UNSIGNED)";
    //    qStr += " and ";
    //    qStr += "CAST("+pHelp.Sales_Invoice_Main_Voucher_No+" AS UNSIGNED)"  + "<= CAST(" + shift->BillsTo +" AS UNSIGNED)";
    //    qStr += ")";
    //    qStr += " group by "+pHelp.Sales_Invoice_Main_Ledger_ID;

    qStr ="SELECT ";
    qStr += "(select ledger_name from "+ledHelper.Ledger_Master_TableName+" led where led.ledger_id=sam.";
    qStr += pHelp.Transaction_Helper_LedgerID;
    qStr += ") Ledger, ";
    qStr += " SUM("+pHelp.Transaction_Helper_Debit_Amount+") as Amount";
    qStr += ", "+pHelp.Transaction_Helper_LedgerID;
    qStr +=" FROM ";
    qStr += pHelp.Transaction_Helper_TableName + " sam WHERE ";
    qStr += pHelp.Transaction_Helper_VoucherPrefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " and "+pHelp.Transaction_Helper_Debit_Amount+ " > 0 ";
    qStr += " and ";
    qStr += " ( ";
    qStr += "CAST("+pHelp.Transaction_Helper_VoucherNo+" AS UNSIGNED)"  + ">= CAST(" + shift->BillsFrom +" AS UNSIGNED)";
    qStr += " and ";
    qStr += "CAST("+pHelp.Transaction_Helper_VoucherNo+" AS UNSIGNED)"  + "<= CAST(" + shift->BillsTo +" AS UNSIGNED)";
    qStr += ")";
    qStr += " AND " + pHelp.Transaction_Helper_VoucherType + " = 'SALESVOUCHER'";
    if(!includeCash)
        qStr += " AND " + pHelp.Transaction_Helper_LedgerID + " not like '0x5x23x1'";
    qStr += " group by "+pHelp.Transaction_Helper_LedgerID;

    qDebug()<<qStr;

    return DatabaseHelper::getQueryModel(qStr);




}


QSqlQueryModel* RestaurantShiftDatabaseHelper::AllLedgerSummaryByVoucherDate(ShiftDataModel *shift, bool includeCash)
{
    qDebug()<<Q_FUNC_INFO;

    LedgerMasterDatabaseHelper ledHelper;
    QString sundryDebtorIds = "";
    AccountGroupMasterDatabaseHelper *acchelper = new AccountGroupMasterDatabaseHelper;
    sundryDebtorIds = acchelper->getGroupIDByName("Bank Accounts");
    sundryDebtorIds=ledHelper.getAllLedgerIDsUnderGroupRecursiveAsString(sundryDebtorIds);

    qDebug()<<"sundry debtors id"<<sundryDebtorIds;

    salesVoucherDatabaseHelper pHelp;

    // QString timestamp = "concat("+pHelp.Sales_Invoice_Main_Voucher_Date+",' ','00:00:00')";


    QString qStr;

    qStr ="SELECT ";
    qStr += "(select ledger_name from "+ledHelper.Ledger_Master_TableName+" led where led.ledger_id=sam.";
    qStr += pHelp.Transaction_Helper_LedgerID;
    qStr += ") Ledger, ";
    qStr += " SUM("+pHelp.Transaction_Helper_Debit_Amount+") as Amount";
    qStr += ", "+pHelp.Transaction_Helper_LedgerID;
    qStr +=" FROM ";
    qStr += pHelp.Transaction_Helper_TableName + " sam WHERE ";
    qStr += pHelp.Transaction_Helper_VoucherPrefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " and "+pHelp.Transaction_Helper_Debit_Amount+ " > 0 ";
    qStr += " and ";
    qStr += " ( ";
    qStr += trans->Transaction_Helper_TimeStamp+" >= " + DatabaseHelper::getDateTimeStringStatic(shift->ShiftStartDateTime) ;
    qStr += " AND ";
    qStr += trans->Transaction_Helper_TimeStamp+" <= " + DatabaseHelper::getDateTimeStringStatic(shift->ShiftEndDateTime) ;
    qStr += ")";
    qStr += " AND " + pHelp.Transaction_Helper_VoucherType + " = 'SALESVOUCHER'";
    if(!includeCash)
        qStr += " AND " + pHelp.Transaction_Helper_LedgerID + " not like '0x5x23x1'";
    qStr += " group by "+pHelp.Transaction_Helper_LedgerID;

    qDebug()<<qStr;

    return DatabaseHelper::getQueryModel(qStr);

}

float RestaurantShiftDatabaseHelper::getTotalCashWithDrawn(ShiftDataModel *shift)
{
    QString qStr = " SELECT SUM(" + Shift_CashWithDrawn + ") FROM " + Shifts_TableName;
    qStr += " where " + Shift_ShiftTimeStamp + " >= " + DatabaseHelper::getDateTimeStringStatic(shift->ShiftStartDateTime);
    qStr += " and " + Shift_ShiftTimeStamp + " <= " + DatabaseHelper::getDateTimeStringStatic(shift->ShiftEndDateTime);

    return DatabaseHelper::getSingletonQueryResult(qStr).toFloat();

}

float RestaurantShiftDatabaseHelper::getTotalCashInCounter(ShiftDataModel *shift)
{
    QString qStr = " SELECT SUM(" + Shift_CasinCounter + ") FROM " + Shifts_TableName;
    qStr += " where " + Shift_ShiftTimeStamp + " >= " + DatabaseHelper::getDateTimeStringStatic(shift->ShiftStartDateTime);
    qStr += " and " + Shift_ShiftTimeStamp + " <= " + DatabaseHelper::getDateTimeStringStatic(shift->ShiftEndDateTime);
    qStr += " order by _id desc limit 1 ";

    return DatabaseHelper::getSingletonQueryResult(qStr).toFloat();

}


QSqlQueryModel* RestaurantShiftDatabaseHelper::GetExpensesByShift(ShiftDataModel *shift)
{
    qDebug()<<Q_FUNC_INFO;

    LedgerMasterDatabaseHelper ledHelper;
    salesVoucherDatabaseHelper pHelp;

    QString qStr ="SELECT ";
    qStr += "(select ledger_name from "+ledHelper.Ledger_Master_TableName+" led where led.ledger_id=sam.";
    qStr += pHelp.Transaction_Helper_LedgerID;
    qStr += ") Ledger, ";
    qStr += " SUM("+pHelp.Transaction_Helper_Debit_Amount+")";
    qStr += ", "+pHelp.Transaction_Helper_LedgerID;
    qStr +=" FROM ";
    qStr += pHelp.Transaction_Helper_TableName + " sam WHERE ";
    qStr += pHelp.Transaction_Helper_VoucherPrefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " and "+pHelp.Transaction_Helper_Debit_Amount+ " > 0 ";
    qStr += " and ";
    qStr += " ( ";
    qStr += pHelp.Transaction_Helper_VoucherDate+" >= " + db->getDateTimeString(shift->ShiftStartDateTime);
    qStr += " and ";
    qStr += pHelp.Transaction_Helper_VoucherDate+" <= " + db->getDateTimeString(shift->ShiftEndDateTime);
    qStr += ")";
    qStr += " AND " + pHelp.Transaction_Helper_VoucherType + " = 'PAYMENTVOUCHER'";
    qStr += " AND " + pHelp.Transaction_Helper_LedgerID + " = '0x5x23x1'";

    qDebug()<<qStr;

    return DatabaseHelper::getQueryModel(qStr);




}


QList<ShiftLedgersDataModel *> RestaurantShiftDatabaseHelper::getSalesByLedgersList(ShiftDataModel *shift)
{
    qDebug()<<Q_FUNC_INFO;

    QList<ShiftLedgersDataModel *> data;
    LedgerMasterDatabaseHelper ledHelper;
    salesVoucherDatabaseHelper pHelp;

    QString qStr;

    qStr ="SELECT ";
    qStr += "(select ledger_name from "+ledHelper.Ledger_Master_TableName+" led where led.ledger_id=sam.";
    qStr += pHelp.Transaction_Helper_LedgerID;
    qStr += ") Ledger, ";
    qStr += " SUM("+pHelp.Transaction_Helper_Debit_Amount+") as Amount";
    qStr += ", "+pHelp.Transaction_Helper_LedgerID;
    qStr +=" FROM ";
    qStr += pHelp.Transaction_Helper_TableName + " sam WHERE ";
    qStr += pHelp.Transaction_Helper_VoucherPrefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " and "+pHelp.Transaction_Helper_Debit_Amount+ " > 0 ";
    qStr += " and " + pHelp.Transaction_Helper_LedgerID + " not in ('0x5x23x1', '0x12x11')";
    qStr += " and ";
    qStr += " ( ";
    qStr += "CAST("+pHelp.Transaction_Helper_VoucherNo+" AS UNSIGNED)"  + ">= CAST(" + shift->BillsFrom +" AS UNSIGNED)";
    qStr += " and ";
    qStr += "CAST("+pHelp.Transaction_Helper_VoucherNo+" AS UNSIGNED)"  + "<= CAST(" + shift->BillsTo +" AS UNSIGNED)";
    qStr += ")";
    qStr += " group by "+pHelp.Transaction_Helper_LedgerID;

    qDebug()<<qStr;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);
    if(query.exec()){
        while (query.next()) {
            qDebug()<<" in loop";
            ShiftLedgersDataModel *led = new ShiftLedgersDataModel;
            led->ledgerID = query.value(2).toString();
            led->ledgerName =query.value(0).toString();
            led->systemAmount = query.value(1).toFloat();

            data.append(led);
        }
    }

    qDebug()<<"returning "<<data.size();
    return data;

}

float RestaurantShiftDatabaseHelper::getReceiptByCash(ShiftDataModel *shift)
{
    float vNO;
    ReceiptVoucherDatabaseHelper pHelp;
    QString timestamp = "concat("+pHelp.Receipt_Voucher_Main_Voucher_Date+",' ',CAST("
            +pHelp.Receipt_Voucher_Main_TimeStamp+" AS TIME))";
    timestamp = pHelp.Receipt_Voucher_Main_TimeStamp;
    QString qStr ="SELECT SUM("+pHelp.Receipt_Voucher_Main_Total+") FROM ";

    qStr += pHelp.Receipt_Voucher_Main_TableName + " WHERE ";
    qStr += pHelp.Receipt_Voucher_Main_Voucher_Prefix + "  like '"+ shift->voucherPrefix+"'";
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

float RestaurantShiftDatabaseHelper::getReceiptTotal(ShiftDataModel *shift)
{
    float vNO;
    ReceiptVoucherDatabaseHelper pHelp;
    QString timestamp = "concat("+pHelp.Receipt_Voucher_Main_Voucher_Date+",' ',CAST("
            +pHelp.Receipt_Voucher_Main_TimeStamp+" AS TIME))";
    timestamp = pHelp.Receipt_Voucher_Main_TimeStamp;
    QString qStr ="SELECT SUM("+pHelp.Receipt_Voucher_Main_Total+") FROM ";

    qStr += pHelp.Receipt_Voucher_Main_TableName + " WHERE ";
    qStr += pHelp.Receipt_Voucher_Main_Voucher_Prefix + " like '" + shift->voucherPrefix + "'";

    qStr += " and ";
    qStr += " ( ";

    qStr += " ( ";
    qStr += timestamp + ">'" + shift->ShiftStartDateTime.toString("yyyy-MM-dd hh:mm:ss") + "'";
    qStr += " and ";
    qStr += timestamp + "< '" + shift->ShiftEndDateTime.toString("yyyy-MM-dd  hh:mm:ss" ) + "'";
    qStr += ")";

    qStr += ")";

    qDebug()<<"Reciept Total : "<<qStr;
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

float RestaurantShiftDatabaseHelper::getPaymentTotal(ShiftDataModel *shift, QString ledger)
{
    float vNO;
    PaymentVoucherDatabaseHelper pHelp;
    QString timestamp = "concat("+pHelp.Payment_Voucher_Main_Voucher_Date+",' ',CAST("
            +pHelp.Payment_Voucher_Main_TimeStamp+" AS TIME))";


    timestamp = pHelp.Payment_Voucher_Main_TimeStamp;
    QString qStr ="SELECT SUM("+pHelp.Payment_Voucher_Main_Total+") FROM ";

    qStr += pHelp.Payment_Voucher_Main_TableName + " WHERE ";
    qStr += pHelp.Payment_Voucher_Main_Voucher_Prefix + " like '" + shift->voucherPrefix + "'";

    qStr += " and ";
    qStr += " ( ";

    qStr += " ( ";
    qStr += timestamp + ">'" + shift->ShiftStartDateTime.toString("yyyy-MM-dd hh:mm:ss") + "'";
    qStr += " and ";
    qStr += timestamp + "< '" + shift->ShiftEndDateTime.toString("yyyy-MM-dd  hh:mm:ss" ) + "'";
    qStr += ")";

    qStr += ")";
    if(ledger != "%")
        qStr += " and " + pHelp.Payment_Voucher_Main_Ledger_ID + " like '" + ledger + "'";
    qDebug()<<"Payments Total : "<<qStr;
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
    qDebug()<<"Total Payment query .... "<<qStr;
    return vNO;
}

float RestaurantShiftDatabaseHelper::DayBookgetPurchaseTotal(ShiftDataModel *shift)
{
    float vNO;
    PurchaseVoucherDatabaseHelper pHelp;
    QString timestamp = "concat("+pHelp.Purchase_Invoice_Main_Voucher_Date+",' ','00:00:00')";
    timestamp = pHelp.Purchase_Invoice_Main_Voucher_Date;
    //            +pHelp.Purchase_Invoice_Main_TimeStamp+" AS TIME))";
    //    timestamp = pHelp.Purchase_Invoice_Main_TimeStamp;
    QString qStr ="SELECT SUM("+pHelp.Purchase_Invoice_Main_Total+") FROM ";

    qStr += pHelp.Purchase_Invoice_Main_TableName + " WHERE ";
    qStr += pHelp.Purchase_Invoice_Main_Voucher_Prefix + " like '" + shift->voucherPrefix + "'";

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

float RestaurantShiftDatabaseHelper::DayBookgetPurchaseByCash(ShiftDataModel *shift)
{

    float vNO;
    PurchaseVoucherDatabaseHelper pHelp;

    QString timestamp = "concat("+pHelp.Purchase_Invoice_Main_Voucher_Date+",' ','00:00:00')";
    timestamp = pHelp.Purchase_Invoice_Main_Voucher_Date;

    //            +pHelp.Purchase_Invoice_Main_TimeStamp+" AS TIME))";
    //    timestamp = pHelp.Purchase_Invoice_Main_TimeStamp;

    QString qStr ="SELECT SUM("+pHelp.Purchase_Invoice_Main_Total+") FROM ";

    qStr += pHelp.Purchase_Invoice_Main_TableName + " WHERE ";
    qStr += pHelp.Purchase_Invoice_Main_Voucher_Prefix + "  like '"+ shift->voucherPrefix+"'";
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

float RestaurantShiftDatabaseHelper::DayBookgetPurchaseByCredit(ShiftDataModel *shift)
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
    qStr += pHelp.Purchase_Invoice_Main_Voucher_Prefix + "  like '"+ shift->voucherPrefix+"'";
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

float RestaurantShiftDatabaseHelper::DayBookgetPaymentByCash(ShiftDataModel *shift)
{

    float vNO;
    PaymentVoucherDatabaseHelper pHelp;

    QString timestamp = "concat("+pHelp.Payment_Voucher_Main_Voucher_Date+",' ','00:00:00')";
    timestamp = pHelp.Payment_Voucher_Main_Voucher_Date;
    //            +pHelp.Purchase_Invoice_Main_TimeStamp+" AS TIME))";
    //    timestamp = pHelp.Purchase_Invoice_Main_TimeStamp;

    QString qStr ="SELECT SUM("+pHelp.Payment_Voucher_Main_Total+") FROM ";

    qStr += pHelp.Payment_Voucher_Main_TableName + " WHERE ";
    qStr += pHelp.Payment_Voucher_Main_Voucher_Prefix + "  like '"+ shift->voucherPrefix+"'";
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

float RestaurantShiftDatabaseHelper::DayBookgetReceiptByCash(ShiftDataModel *shift)
{

    float vNO;
    ReceiptVoucherDatabaseHelper pHelp;
    QString timestamp = "concat("+pHelp.Receipt_Voucher_Main_Voucher_Date+",' ','00:00:00')";
    timestamp = pHelp.Receipt_Voucher_Main_Voucher_Date;

    //            +pHelp.Purchase_Invoice_Main_TimeStamp+" AS TIME))";
    //    timestamp = pHelp.Purchase_Invoice_Main_TimeStamp;

    QString qStr ="SELECT SUM("+pHelp.Receipt_Voucher_Main_Total+") FROM ";

    qStr += pHelp.Receipt_Voucher_Main_TableName + " WHERE ";
    qStr += pHelp.Receipt_Voucher_Main_Voucher_Prefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " and "+pHelp.Receipt_Voucher_Main_Ledger_ID+ "='"+ shift->cashID+"'";
    qStr += " and ";
    qStr += " ( ";

    qStr += " ( ";
    qStr += timestamp + ">='" + shift->ShiftStartDateTime.toString("yyyy-MM-dd") + "'";
    qStr += " and ";
    qStr += timestamp + "< '" + shift->ShiftEndDateTime.toString("yyyy-MM-dd") + "'";
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

float RestaurantShiftDatabaseHelper::DayBookgetReceiptTotal(ShiftDataModel *shift)
{

    float vNO;
    ReceiptVoucherDatabaseHelper pHelp;

    QString timestamp = "concat("+pHelp.Receipt_Voucher_Main_Voucher_Date+",' ','00:00:00')";
    timestamp = pHelp.Receipt_Voucher_Main_Voucher_Date;

    //            +pHelp.Purchase_Invoice_Main_TimeStamp+" AS TIME))";
    //    timestamp = pHelp.Purchase_Invoice_Main_TimeStamp;
    QString qStr ="SELECT SUM("+pHelp.Receipt_Voucher_Main_Total+") FROM ";

    qStr += pHelp.Receipt_Voucher_Main_TableName + " WHERE ";
    qStr += pHelp.Receipt_Voucher_Main_Voucher_Prefix + " like '" + shift->voucherPrefix + "'";

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

float RestaurantShiftDatabaseHelper::DayBookgetPaymentTotal(ShiftDataModel *shift)
{

    float vNO;
    PaymentVoucherDatabaseHelper pHelp;

    QString timestamp = "concat("+pHelp.Payment_Voucher_Main_Voucher_Date+",' ','00:00:00')";
    timestamp = pHelp.Payment_Voucher_Main_Voucher_Date;

    //            +pHelp.Purchase_Invoice_Main_TimeStamp+" AS TIME))";
    //    timestamp = pHelp.Purchase_Invoice_Main_TimeStamp;
    QString qStr ="SELECT SUM("+pHelp.Payment_Voucher_Main_Total+") FROM ";

    qStr += pHelp.Payment_Voucher_Main_TableName + " WHERE ";
    qStr += pHelp.Payment_Voucher_Main_Voucher_Prefix + " like '" + shift->voucherPrefix + "'";

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

float RestaurantShiftDatabaseHelper::getZomatoSalesTotal(ShiftDataModel *shift)
{
    float vNO;

    QString qStr ="SELECT SUM("+shelper.Sales_Invoice_Main_Total+") FROM ";
    qStr += shelper.Sales_Invoice_Main_TableName + " WHERE ";
    qStr += shelper.Sales_Invoice_Main_Voucher_Prefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " and "+shelper.Sales_Invoice_Main_Billing_Name +"='potafo'";

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

float RestaurantShiftDatabaseHelper::getZomatoSalesByCash(ShiftDataModel *shift)
{
    float vNO;
    QString qStr ="SELECT SUM("+shelper.Sales_Invoice_Main_Total+") FROM ";
    qStr += shelper.Sales_Invoice_Main_TableName + " WHERE ";
    qStr += shelper.Sales_Invoice_Main_Voucher_Prefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " and "+shelper.Sales_Invoice_Main_Billing_Name +"='potafo'";

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

float RestaurantShiftDatabaseHelper::getZomatoSalesByCredit(ShiftDataModel *shift)
{
    float vNO;

    QString zomatoId = "";
    LedgerMasterDatabaseHelper *led = new LedgerMasterDatabaseHelper;
    zomatoId = "0x5x19x17";//led->getLedgerIDByName("Zomato");

    QString qStr ="SELECT SUM("+shelper.Sales_Invoice_Main_Total+") FROM ";
    qStr += shelper.Sales_Invoice_Main_TableName + " WHERE ";
    qStr += shelper.Sales_Invoice_Main_Voucher_Prefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " and "+shelper.Sales_Invoice_Main_Billing_Name +"='potafo'";

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

float RestaurantShiftDatabaseHelper::getTalabatSalesTotal(ShiftDataModel *shift)
{
    float vNO;

    QString qStr ="SELECT SUM("+shelper.Sales_Invoice_Main_Total+") FROM ";
    qStr += shelper.Sales_Invoice_Main_TableName + " WHERE ";
    qStr += shelper.Sales_Invoice_Main_Voucher_Prefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " and "+shelper.Sales_Invoice_Main_Billing_Name +"='talabat'";

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

float RestaurantShiftDatabaseHelper::getTalabatSalesByCash(ShiftDataModel *shift)
{
    float vNO;
    QString qStr ="SELECT SUM("+shelper.Sales_Invoice_Main_Total+") FROM ";
    qStr += shelper.Sales_Invoice_Main_TableName + " WHERE ";
    qStr += shelper.Sales_Invoice_Main_Voucher_Prefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " and "+shelper.Sales_Invoice_Main_Billing_Name +"='talabat'";

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

float RestaurantShiftDatabaseHelper::getTalabatSalesByCredit(ShiftDataModel *shift)
{
    float vNO;

    QString zomatoId = "";
    LedgerMasterDatabaseHelper *led = new LedgerMasterDatabaseHelper;
    zomatoId = led->getLedgerIDByName("Talabat");//"0x5x19x66";

    QString qStr ="SELECT SUM("+shelper.Sales_Invoice_Main_Total+") FROM ";
    qStr += shelper.Sales_Invoice_Main_TableName + " WHERE ";
    qStr += shelper.Sales_Invoice_Main_Voucher_Prefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " and "+shelper.Sales_Invoice_Main_Billing_Name +"='talabat'";

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

float RestaurantShiftDatabaseHelper::getCarriageSalesTotal(ShiftDataModel *shift)
{
    float vNO;

    QString qStr ="SELECT SUM("+shelper.Sales_Invoice_Main_Total+") FROM ";
    qStr += shelper.Sales_Invoice_Main_TableName + " WHERE ";
    qStr += shelper.Sales_Invoice_Main_Voucher_Prefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " and "+shelper.Sales_Invoice_Main_Billing_Name +"='carriage'";

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

float RestaurantShiftDatabaseHelper::getDeliverooSalesTotal(ShiftDataModel *shift)
{
    float vNO;

    QString qStr ="SELECT SUM("+shelper.Sales_Invoice_Main_Total+") FROM ";
    qStr += shelper.Sales_Invoice_Main_TableName + " WHERE ";
    qStr += shelper.Sales_Invoice_Main_Voucher_Prefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " and "+shelper.Sales_Invoice_Main_Billing_Name +"='Deliveroo'";

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

QList<ShiftDataModel> RestaurantShiftDatabaseHelper::getAllShiftDetailsByDate(QDateTime dateFrom, QDateTime dateTo)
{
    QList<ShiftDataModel> shiftList;

    QString qStr  = " SELECT ";

    qStr += " "+Shift_SalesValue;
    qStr += ", "+Shift_CashSales;
    qStr += ", "+Shift_CardSales;
    qStr += ", "+Shift_CreditSales;
//    qStr += ", "+Shift_ZomatoSales;
//    qStr += ", "+Shift_ZomatoCash;
//    qStr += ", "+Shift_ZomatoCredit;
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
//            shiftData.ZomatoSales= query.value(i++).toFloat();
//            shiftData.ZomatoCash= query.value(i++).toFloat();
//            shiftData.ZomatoCredit= query.value(i++).toFloat();
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

ShiftDataModel RestaurantShiftDatabaseHelper::getlastShiftDetailByVoucherPrefix(QString prefix)
{
    ShiftDataModel shiftData;

    QString qStr  = " SELECT ";

    qStr += " "+Shift_SalesValue;
    qStr += ", "+Shift_CashSales;
    qStr += ", "+Shift_CardSales;
    qStr += ", "+Shift_CreditSales;
//    qStr += ", "+Shift_ZomatoSales;
//    qStr += ", "+Shift_ZomatoCash;
//    qStr += ", "+Shift_ZomatoCredit;
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

    //    qDebug()<<qStr;
    query.prepare(qStr);


    if(query.exec()){
        while(query.next()){
            int i =0;
            shiftData.SalesTotal    = query.value(i++).toFloat();
            shiftData.CashSales= query.value(i++).toFloat();
            shiftData.CardSales= query.value(i++).toFloat();
            shiftData.CreditSales= query.value(i++).toFloat();
//            shiftData.ZomatoSales= query.value(i++).toFloat();
//            shiftData.ZomatoCash= query.value(i++).toFloat();
//            shiftData.ZomatoCredit= query.value(i++).toFloat();
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
            qDebug()<<"last Bill from db:"<<shiftData.BillsTo ;

        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }


    return shiftData;
}

ShiftDataModel RestaurantShiftDatabaseHelper::getShiftDetailByshiftID(int id)
{
    ShiftDataModel shiftData;

    QString qStr  = " SELECT ";

    qStr += " "+Shift_SalesValue;
    qStr += ", "+Shift_CashSales;
    qStr += ", "+Shift_CardSales;
    qStr += ", "+Shift_CreditSales;
//    qStr += ", "+Shift_ZomatoSales;
//    qStr += ", "+Shift_ZomatoCash;
//    qStr += ", "+Shift_ZomatoCredit;
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
    qStr += " where ";
    qStr += Shift_ID ;
    qStr += "='"+QString::number(id)+"' ";


    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    //    qDebug()<<qStr;
    query.prepare(qStr);


    if(query.exec()){
        while(query.next()){
            int i =0;
            shiftData.SalesTotal    = query.value(i++).toFloat();
            shiftData.CashSales= query.value(i++).toFloat();
            shiftData.CardSales= query.value(i++).toFloat();
            shiftData.CreditSales= query.value(i++).toFloat();
//            shiftData.ZomatoSales= query.value(i++).toFloat();
//            shiftData.ZomatoCash= query.value(i++).toFloat();
//            shiftData.ZomatoCredit= query.value(i++).toFloat();
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
            qDebug()<<"last Bill from db:"<<shiftData.BillsTo ;

            DenominationsDataModel *deno = new DenominationsDataModel;
            shiftData.denominations = deno;
            qDebug()<<Q_FUNC_INFO<<shiftData.denominations->total;

        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }

    shiftData.billsCount = shiftData.BillsTo.toInt()-shiftData.BillsFrom.toInt();


    return shiftData;
}

ShiftDataModel RestaurantShiftDatabaseHelper::getlastClosingDetailByVoucherPrefix(QString vPref)
{
    ShiftDataModel shiftData;

    QString qStr  = " SELECT ";

    qStr += " "+Shift_SalesValue;
    qStr += ", "+Shift_CashSales;
    qStr += ", "+Shift_CardSales;
    qStr += ", "+Shift_CreditSales;
//    qStr += ", "+Shift_ZomatoSales;
//    qStr += ", "+Shift_ZomatoCash;
//    qStr += ", "+Shift_ZomatoCredit;
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
    qStr += Shift_ID +" in (select max("+Shift_ID+")";
    qStr += " from " + Shifts_TableName + " where ";
    qStr += Shift_voucherPrefix + " like '"+vPref +"')";



    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);


    if(query.exec()){
        while(query.next()){
            int i =0;
            shiftData.SalesTotal = query.value(i++).toFloat();
            shiftData.CashSales= query.value(i++).toFloat();
            shiftData.CardSales= query.value(i++).toFloat();
            shiftData.CreditSales= query.value(i++).toFloat();
//            shiftData.ZomatoSales= query.value(i++).toFloat();
//            shiftData.ZomatoCash= query.value(i++).toFloat();
//            shiftData.ZomatoCredit= query.value(i++).toFloat();
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

            DenominationsDataModel *deno = new DenominationsDataModel;
            shiftData.denominations = deno;
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }

    qDebug()<<qStr;
    qDebug()<<"Last Opening = "<<shiftData.CashBalance;
    return shiftData;
}

void RestaurantShiftDatabaseHelper::ClearSales()
{
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr;

    qStr = "TRUNCATE SALES_INVOICE_MAIN";
    query.exec(qStr);
    qStr = "TRUNCATE SALES_INVOICE_DETAILS";
    query.exec(qStr);
    qStr = "TRUNCATE TRANSACTION_HELPER";
    query.exec(qStr);
    qStr = "TRUNCATE TRANSACTION_ITEMHELPER";
    query.exec(qStr);

}

void RestaurantShiftDatabaseHelper::updateShiftWithdrawAmt(int shiftid, float withdrawal,float balance)
{
    QMap<QString,QVariant> clause;
    QMap<QString,QVariant> data;
    data.insert(Shift_CashWithDrawn,withdrawal);
    data.insert(Shift_CashBalance,balance);
    clause.insert(Shift_ID,shiftid);
    startTransaction();
    db->updateRecordsPlus(data,clause,Shifts_TableName);
    commit();
}


QDateTime RestaurantShiftDatabaseHelper::getVoucherDateTimebyBill(QString vno)
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

float RestaurantShiftDatabaseHelper::getAdvancePaymentFromSalesOrders(QDateTime fromDateTime, QDateTime toDateTime,
                                                                      QString vpref)
{
    LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper;
    QString ledGerID = ledHelper->getLedgerIDByName("Advance Payment");

    QString qStr = " SELECT sum(rv.total) from receipt_voucher_main rv ";
    qStr += " inner join sales_order_main so on rv.transactionId = so.transactionId";
    qStr += " where so.ledger_id = '" + ledGerID + "'";
    qStr += " and rv.timestamp >= " + db->getDateTimeString(fromDateTime);
    qStr += " and rv.timestamp <= " + db->getDateTimeString(toDateTime);


    qStr ="SELECT sum(rv.total) from receipt_voucher_main rv ";
    qStr +=" inner join sales_order_main so on rv.transactionId = so.transactionId";
    qStr +=" where so.ledger_id = '" + ledGerID + "'";
    qStr += " and rv.voucher_prefix= '"+vpref+"' ";
    qStr +=" and concat(rv.voucher_date, ' ', substr(rv.timestamp, 12,19)) >=  " + db->getDateTimeString(fromDateTime);
    qStr +=" and concat(rv.voucher_date, ' ', substr(rv.timestamp, 12,19)) <=  " + db->getDateTimeString(toDateTime);

    qDebug()<<"ADVAnce PAYMENT.";
    qDebug()<<qStr;

    return db->getSingletonQueryResult(qStr).toFloat();
}


QSqlQueryModel* RestaurantShiftDatabaseHelper::getVouchersByLedger(ShiftDataModel *shift, QString ledgerID)
{
    qDebug()<<Q_FUNC_INFO;

    LedgerMasterDatabaseHelper ledHelper;
    salesVoucherDatabaseHelper pHelp;

    QString qStr ="SELECT " + pHelp.Sales_Invoice_Main_Voucher_No + ", " /*+ pHelp.Sales_Invoice_Main_Voucher_Prefix + ", "*/;
    qStr += pHelp.Sales_Invoice_Main_Total;
    qStr +=" FROM ";
    qStr += pHelp.Sales_Invoice_Main_TableName + " sam WHERE ";
    qStr += pHelp.Sales_Invoice_Main_Voucher_Prefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " and ";
    qStr += "CAST("+pHelp.Sales_Invoice_Main_Voucher_No+" AS UNSIGNED)"  + ">= CAST(" + shift->BillsFrom +" AS UNSIGNED)";
    qStr += " and ";
    qStr += "CAST("+pHelp.Sales_Invoice_Main_Voucher_No+" AS UNSIGNED)"  + "<= CAST(" + shift->BillsTo +" AS UNSIGNED)";
    qStr += " and " + pHelp.Sales_Invoice_Main_Ledger_ID + " like '" + ledgerID + "' ";

    qDebug()<<qStr;

    return DatabaseHelper::getQueryModel(qStr);

}

QSqlQueryModel *RestaurantShiftDatabaseHelper::getSalesSplitModel(ShiftDataModel *shift)
{
    float vNO = 0;
    QString LedgersId = "%";
    LedgerMasterDatabaseHelper ledHelper;
    AccountGroupMasterDatabaseHelper *acchelper = new AccountGroupMasterDatabaseHelper;
    QString GroupId = acchelper->getGroupIDByName("Sales Accounts");
    LedgersId = ledHelper.getAllLedgerIDsUnderGroupRecursiveAsString(GroupId);
    GroupId = acchelper->getGroupIDByName("Duties and Taxes");
    LedgersId += ", ";
    LedgersId +=ledHelper.getAllLedgerIDsUnderGroupRecursiveAsString(GroupId);


    QString qStr ="SELECT SUM("+shelper.Sales_Invoice_Main_Total+") FROM ";
    qStr += shelper.Sales_Invoice_Main_TableName + " WHERE ";
    qStr += shelper.Sales_Invoice_Main_Voucher_Prefix + " like '"+ shift->voucherPrefix+"'";
    qStr += " and "+shelper.Sales_Invoice_Main_Ledger_ID+ " in ("+ LedgersId+")";
    qStr += " and ";
    qStr += "CAST("+shelper.Sales_Invoice_Main_Voucher_No+" AS UNSIGNED)"  + ">= CAST(" + shift->BillsFrom +" AS UNSIGNED)";
    qStr += " and ";
    qStr += "CAST("+shelper.Sales_Invoice_Main_Voucher_No+" AS UNSIGNED)"  + "<= CAST(" + shift->BillsTo +" AS UNSIGNED)";



    qStr = "SELECT (select ledger_name from ledger_master where ledger_id= t.ledger_id) as Ledger, ";
    qStr += " ROUND(ABS(SUM("+trans->Transaction_Helper_Debit_Amount+") -";
    qStr += " SUM("+trans->Transaction_Helper_Credit_Amount+")),2)  as Amount ";
    qStr += " FROM " ;
    qStr += trans->Transaction_Helper_TableName + " t ";
    qStr += " WHERE " ;
    qStr += trans->Transaction_Helper_VoucherPrefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " AND ";
    qStr += trans->Transaction_Helper_VoucherType + "= 'SALESVOUCHER' ";
    qStr += " AND " + trans->Transaction_Helper_LedgerID ;
    qStr += " in ("+ LedgersId+") ";
    qStr += " AND ";
    qStr += trans->Transaction_Helper_TimeStamp+" >= " + DatabaseHelper::getDateTimeStringStatic(shift->ShiftStartDateTime) ;
    qStr += " AND ";
    qStr += trans->Transaction_Helper_TimeStamp+" <= " + DatabaseHelper::getDateTimeStringStatic(shift->ShiftEndDateTime) ;
    qStr += " group by " + trans->Transaction_Helper_LedgerID;
    qStr += " order by 1 desc ";

    //    qStr += " UNION ALL ";
    //    qStr += "SELECT REPLACE(concat('Output CGST @ ', round(cast(substr(Ledger_id, instr(Ledger_id, 'SV')+2, 2) as int)/2,1), '% GST'),'.0', '') as Ledger, ";
    //    qStr += " ROUND(ABS(SUM("+trans->Transaction_Helper_Debit_Amount+") -";
    //    qStr += " SUM("+trans->Transaction_Helper_Credit_Amount+")),2) * round(cast(substr(Ledger_id, instr(Ledger_id, 'SV')+2, 2) as int)/2,1)/100 as Amount ";
    //    qStr += " FROM " ;
    //    qStr += trans->Transaction_Helper_TableName + " t ";
    //    qStr += " WHERE " ;
    //    qStr += trans->Transaction_Helper_VoucherPrefix + "  like '"+ shift->voucherPrefix+"'";
    //    qStr += " AND ";
    //    qStr += trans->Transaction_Helper_VoucherType + "= 'SALESVOUCHER' ";
    //    qStr += " AND ";
    //    qStr += trans->Transaction_Helper_VoucherDate+" >= " + DatabaseHelper::getDateTimeStringStatic(shift->ShiftStartDateTime) ;
    //    qStr += " AND ";
    //    qStr += trans->Transaction_Helper_VoucherDate+" <= " + DatabaseHelper::getDateTimeStringStatic(shift->ShiftEndDateTime) ;
    //    qStr += " and Ledger_Id  like '0x7xSV%'  ";
    //    qStr += " group by " + trans->Transaction_Helper_LedgerID;

    //    qStr += " UNION ALL ";
    //    qStr += "SELECT REPLACE(concat('Output SGST @ ', round(cast(substr(Ledger_id, instr(Ledger_id, 'SV')+2, 2) as int)/2,1), '% GST'),'.0', '') as Ledger, ";
    //    qStr += " ROUND(ABS(SUM("+trans->Transaction_Helper_Debit_Amount+") -";
    //    qStr += " SUM("+trans->Transaction_Helper_Credit_Amount+")),2)* round(cast(substr(Ledger_id, instr(Ledger_id, 'SV')+2, 2) as int)/2,1)/100  as Amount ";
    //    qStr += " FROM " ;
    //    qStr += trans->Transaction_Helper_TableName + " t ";
    //    qStr += " WHERE " ;
    //    qStr += trans->Transaction_Helper_VoucherPrefix + "  like '"+ shift->voucherPrefix+"'";
    //    qStr += " AND ";
    //    qStr += trans->Transaction_Helper_VoucherType + "= 'SALESVOUCHER' ";
    //    qStr += " AND ";
    //    qStr += trans->Transaction_Helper_VoucherDate+" >= " + DatabaseHelper::getDateTimeStringStatic(shift->ShiftStartDateTime) ;
    //    qStr += " AND ";
    //    qStr += trans->Transaction_Helper_VoucherDate+" <= " + DatabaseHelper::getDateTimeStringStatic(shift->ShiftEndDateTime) ;
    //    qStr += " and Ledger_Id  like '0x7xSV%'  ";
    //    qStr += " group by " + trans->Transaction_Helper_LedgerID;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    //        qDebug()<<"Sales Credit";
    query.prepare(qStr);

    qDebug()<<"Tax "<<qStr;
    if(query.exec()){
        while(query.next()){
            vNO = query.value(0).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return DatabaseHelper::getQueryModel(qStr);
}


float RestaurantShiftDatabaseHelper::getOpeningCashByShift(ShiftDataModel *shift)
{
    float vNO =0;
    QString qStr = "";
    QString cashid = shift->cashID;
    QString     timestamp = "str_to_Date(addTime(det.VoucherDate, det.Time) ,'%Y-%m-%d %H:%i:%s')" ;
    qStr += "SELECT ";
    qStr +=Shift_OpeningBalance;
    qStr += " FROM `shifttable` shift ";
    qStr += " where ";
    qStr += " ( ";
    qStr += " ShiftEnd_DateTime >=" + DatabaseHelper::getDateTimeStringStatic(shift->ShiftStartDateTime);
    qStr += " and ";
    qStr += " ShiftEnd_DateTime <= " + DatabaseHelper::getDateTimeStringStatic(shift->ShiftEndDateTime);
    qStr += ")";
    qStr += " order by ShiftEnd_DateTime ";


    qDebug()<<"Opening Cash";
    qDebug()<<qStr;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);

    if(query.exec()){
        if(query.first()){
            vNO = query.value(0).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return vNO;
}


QSqlQueryModel* RestaurantShiftDatabaseHelper::getCreditSummaryByLedger(ShiftDataModel *shift, QString voucherType, bool includeCash)
{
    qDebug()<<Q_FUNC_INFO;

    LedgerMasterDatabaseHelper ledHelper;
//    QString ledgerIds = "";
//    AccountGroupMasterDatabaseHelper *acchelper = new AccountGroupMasterDatabaseHelper;
//    ledgerIds = acchelper->getGroupIDByName("Bank Accounts");
//    ledgerIds=ledHelper.getAllLedgerIDsUnderGroupRecursiveAsString(ledgerIds);

    salesVoucherDatabaseHelper pHelp;

    QString qStr;

    qStr ="SELECT ";
    qStr += "(select ledger_name from "+ledHelper.Ledger_Master_TableName+" led where led.ledger_id=sam.";
    qStr += pHelp.Transaction_Helper_LedgerID;
    qStr += ") Ledger, ";
    qStr += " SUM("+pHelp.Transaction_Helper_Credit_Amount+") as Amount";
    qStr += ", "+pHelp.Transaction_Helper_LedgerID;
    qStr +=" FROM ";
    qStr += pHelp.Transaction_Helper_TableName + " sam WHERE ";
    qStr += pHelp.Transaction_Helper_VoucherPrefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " and "+pHelp.Transaction_Helper_Credit_Amount+ " > 0 ";
    qStr += " and ";
    qStr += " ( ";
    qStr += trans->Transaction_Helper_TimeStamp+" >= " + DatabaseHelper::getDateTimeStringStatic(shift->ShiftStartDateTime) ;
    qStr += " AND ";
    qStr += trans->Transaction_Helper_TimeStamp+" <= " + DatabaseHelper::getDateTimeStringStatic(shift->ShiftEndDateTime) ;
    qStr += ")";
    qStr += " AND " + pHelp.Transaction_Helper_VoucherType + " = '" + voucherType + "'";
//    qStr += " AND " + pHelp.Transaction_Helper_LedgerID + " in (" + ledgerIds + ") ";
//    if(!includeCash)
//        qStr += " AND " + pHelp.Transaction_Helper_LedgerID + " not like '0x5x23x1'";

    qStr += " group by "+pHelp.Transaction_Helper_LedgerID;

    qDebug()<<qStr;

    return DatabaseHelper::getQueryModel(qStr);

}


QSqlQueryModel* RestaurantShiftDatabaseHelper::getDebitSummaryByLedger(ShiftDataModel *shift, QString voucherType, bool includeCash)
{
    qDebug()<<Q_FUNC_INFO<<shift->ShiftStartDateTime;

    LedgerMasterDatabaseHelper ledHelper;
//    QString ledgerIds = "";
//    AccountGroupMasterDatabaseHelper *acchelper = new AccountGroupMasterDatabaseHelper;
//    ledgerIds = acchelper->getGroupIDByName("Bank Accounts");
//    ledgerIds=ledHelper.getAllLedgerIDsUnderGroupRecursiveAsString(ledgerIds);

    salesVoucherDatabaseHelper pHelp;

    QString qStr;

    qStr ="SELECT ";
    qStr += "(select ledger_name from "+ledHelper.Ledger_Master_TableName+" led where led.ledger_id=sam.";
    qStr += pHelp.Transaction_Helper_LedgerID;
    qStr += ") Ledger, ";
    qStr += " SUM("+pHelp.Transaction_Helper_Debit_Amount+") as Amount";
    qStr += ", "+pHelp.Transaction_Helper_LedgerID;
    qStr +=" FROM ";
    qStr += pHelp.Transaction_Helper_TableName + " sam WHERE ";
    qStr += pHelp.Transaction_Helper_VoucherPrefix + "  like '"+ shift->voucherPrefix+"'";
    qStr += " and "+pHelp.Transaction_Helper_Debit_Amount+ " > 0 ";
    qStr += " and ";
    qStr += " ( ";

    qStr += trans->Transaction_Helper_TimeStamp+" >= " + DatabaseHelper::getDateTimeStringStatic(shift->ShiftStartDateTime) ;
    qStr += " AND ";
    qStr += trans->Transaction_Helper_TimeStamp+" <= " + DatabaseHelper::getDateTimeStringStatic(shift->ShiftEndDateTime) ;
    qStr += ")";
    qStr += " AND " + pHelp.Transaction_Helper_VoucherType + " = '"+voucherType+"'";
//    qStr += " AND " + pHelp.Transaction_Helper_LedgerID + " in (" + ledgerIds + ") ";
    if(!includeCash)
        qStr += " AND " + pHelp.Transaction_Helper_LedgerID + " not like '0x5x23x1'";

    qStr += " group by "+pHelp.Transaction_Helper_LedgerID;

//    qDebug()<<qStr;

    return DatabaseHelper::getQueryModel(qStr);

}
