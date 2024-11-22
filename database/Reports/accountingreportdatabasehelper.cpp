#include "accountingreportdatabasehelper.h"

AccountingReportDatabaseHelper::AccountingReportDatabaseHelper()
{
    db = new DatabaseHelper();
}

QSqlQueryModel *AccountingReportDatabaseHelper::getBalanceForAllLedgers(QDate fromDate, QDate toDate)
{
    // trial balance -- not used in dit
    QSqlQueryModel *model = new QSqlQueryModel();


    QString qStr;
    qStr += " SELECT LEDGER_NAME, (CASE WHEN CLOSING < 0 THEN -1*CLOSING  ELSE NULL END) DEBIT,  ";
    qStr += " (CASE WHEN CLOSING > 0 THEN CLOSING  ELSE NULL END) CREDIT FROM ";
    qStr += " (SELECT LED.LEDGER_NAME, LED.LEDGER_ID, LED.LEDGER_TYPE, LED.GROUP_ID, ";
    qStr += " CAST( ";
    qStr += " ROUND(COALESCE(SUM(TRANS.CREDIT_AMOUNT),0),2) - ROUND(COALESCE(SUM(TRANS.DEBIT_AMOUNT),0),2) ";
    qStr += " AS DECIMAL(10,2)) AS CLOSING FROM " + led.Ledger_Master_TableName + " LED ";
    //    qStr += " INNER JOIN  ";
    //    qStr += " (SELECT  GROUP_ID, ";
    //    qStr += "         GROUP_NAME, ";
    //    qStr += "         PARENT_ID  ";
    //    qStr += " FROM    (SELECT * FROM  " + AccountGroupMasterDatabaseHelper::Accounts_Group_TableName +"  ";
    //    qStr += "          ORDER BY PARENT_ID, GROUP_ID) GROUPS_SORTED, ";
    //    qStr += "         (SELECT @PV := '" + groupID + "') INITIALISATION ";
    //    qStr += " WHERE   FIND_IN_SET(PARENT_ID, @PV) ";
    //    qStr += " AND     LENGTH(@PV := CONCAT(@PV, ',', GROUP_ID)) ";
    //    qStr += " UNION  ";
    //    qStr += " SELECT  GROUP_ID, ";
    //    qStr += "         GROUP_NAME, ";
    //    qStr += "         PARENT_ID  ";
    //    qStr += " FROM    " + AccountGroupMasterDatabaseHelper::Accounts_Group_TableName +"   ";
    //    qStr += " WHERE GROUP_ID = '" + groupID + "') AS ACC_GROUP ";
    //    qStr += " ON LED.GROUP_ID = ACC_GROUP.GROUP_ID ";
    qStr += " LEFT OUTER JOIN TRANSACTION_HELPER TRANS ";
    qStr += " ON LED.LEDGER_ID = TRANS.LEDGER_ID ";
    qStr += " AND TRANS.VOUCHER_DATE <= '" + toDate.toString("yyyy-MM-dd") + "' ";
    qStr += " GROUP BY LED.LEDGER_ID)A ";
    qStr += " WHERE CLOSING <>0";
    qStr += " ORDER BY LEDGER_TYPE,GROUP_ID ";

    //    qDebug()<<qStr;

    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}

QSqlQueryModel *AccountingReportDatabaseHelper::getTrialBalance(QDate date)
{
    //   not used
    QSqlQueryModel *model = new QSqlQueryModel();
    TransactionHelper *t= new TransactionHelper();
    LedgerMasterDatabaseHelper led;
    //    QString qStr="SELECT ";

    //    qStr += "Ledger,Format(IF(DEBIT<0,-DEBIT,null),2) Debit,Format(IF(CREDIT>0,CREDIT,NULL),2) Credit";
    //    qStr += " FROM( select ";

    //    qStr += "ledger_name Ledger,";

    //    qStr += led.Ledger_Master_openingBalance +"+ ( Select SUM(";
    //    qStr += t->Transaction_Helper_Credit_Amount+"-"+t->Transaction_Helper_Debit_Amount +")";
    //    qStr += " FROM ";
    //    qStr += t->Transaction_Helper_TableName +" where ";
    //    qStr += t->Transaction_Helper_LedgerID + "=led."+led.Ledger_Master_LedgerID;
    //    qStr += " and ";
    //    qStr += t->Transaction_Helper_VoucherDate +" <='"+date.toString("yyyy-MM-dd")+"'";
    //    qStr +=") as Debit ";


    //    qStr += ",";

    //    qStr += led.Ledger_Master_openingBalance +"+ ( Select SUM(";
    //    qStr += t->Transaction_Helper_Credit_Amount+"-"+t->Transaction_Helper_Debit_Amount +")";
    //    qStr += " FROM ";
    //    qStr += t->Transaction_Helper_TableName + " where ";
    //    qStr += t->Transaction_Helper_LedgerID + "=led."+led.Ledger_Master_LedgerID;
    //    qStr += " and ";
    //    qStr += t->Transaction_Helper_VoucherDate +" <='"+date.toString("yyyy-MM-dd")+"'";
    //    qStr +=") as Credit";

    //    qStr += " from "+led.Ledger_Master_TableName + " led";

    //    qStr += " Order By " + led.Ledger_Master_LedgerName+") trial ";
    //    qStr += " order by Ledger";

    QString qStr = "";
    qStr += " SELECT LEDGER_NAME, (CASE WHEN CLOSING < 0 THEN -1*CLOSING  ELSE NULL END) DEBIT,  ";
    qStr += " (CASE WHEN CLOSING > 0 THEN CLOSING  ELSE NULL END) CREDIT FROM ";
    qStr += " (SELECT LED.LEDGER_NAME, LED.LEDGER_ID,  ";
    qStr += " CAST(COALESCE(LED.OPENING_BALANCE,0)+ ";
    qStr += " ROUND(COALESCE(SUM(TRANS.CREDIT_AMOUNT),0),2) - ROUND(COALESCE(SUM(TRANS.DEBIT_AMOUNT),0),2) AS DECIMAL(10,2)) AS CLOSING FROM " + led.Ledger_Master_TableName + " LED ";
    qStr += " LEFT OUTER JOIN TRANSACTION_HELPER TRANS ";
    qStr += " ON LED.LEDGER_ID = TRANS.LEDGER_ID ";
    qStr += " AND LED.Ledger_Type in ('Assets', 'Liabilities')";
    qStr += " AND TRANS.VOUCHER_DATE <= '" + date.toString("yyyy-MM-dd") + "' ";
    qStr += " GROUP BY LED.LEDGER_ID)A ";
    qStr += " WHERE CLOSING <>0";

    //    qDebug()<<qStr;
    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}

QSqlQueryModel *AccountingReportDatabaseHelper::getLedgerBalanceByGroup(QString groupID, QDate fromDate, QDate toDate)
{
    //   not used
    QSqlQueryModel *model = new QSqlQueryModel();
    TransactionHelper *t= new TransactionHelper();


    QString qStr;
    qStr += " SELECT LEDGER_NAME, (CASE WHEN CLOSING < 0 THEN -1*CLOSING  ELSE NULL END) DEBIT,  ";
    qStr += " (CASE WHEN CLOSING > 0 THEN CLOSING  ELSE NULL END) CREDIT FROM ";
    qStr += " (SELECT LED.LEDGER_NAME, LED.LEDGER_ID,  ";
    qStr += " CAST(COALESCE(LED.OPENING_BALANCE,0)+ ";
    qStr += " ROUND(COALESCE(SUM(TRANS.CREDIT_AMOUNT),0),2) - ROUND(COALESCE(SUM(TRANS.DEBIT_AMOUNT),0),2) AS DECIMAL(10,2)) AS CLOSING FROM " + led.Ledger_Master_TableName + " LED ";
    qStr += " INNER JOIN  ";
    qStr += " (SELECT  GROUP_ID, ";
    qStr += "         GROUP_NAME, ";
    qStr += "         PARENT_ID  ";
    qStr += " FROM    (SELECT * FROM  " + AccountGroupMasterDatabaseHelper::Accounts_Group_TableName +"  ";
    qStr += "          ORDER BY PARENT_ID, GROUP_ID) GROUPS_SORTED, ";
    qStr += "         (SELECT @PV := '" + groupID + "') INITIALISATION ";
    qStr += " WHERE   FIND_IN_SET(PARENT_ID, @PV) ";
    qStr += " AND     LENGTH(@PV := CONCAT(@PV, ',', GROUP_ID)) ";
    qStr += " UNION  ";
    qStr += " SELECT  GROUP_ID, ";
    qStr += "         GROUP_NAME, ";
    qStr += "         PARENT_ID  ";
    qStr += " FROM    " + AccountGroupMasterDatabaseHelper::Accounts_Group_TableName +"   ";
    qStr += " WHERE GROUP_ID = '" + groupID + "') AS ACC_GROUP ";
    qStr += " ON LED.GROUP_ID = ACC_GROUP.GROUP_ID ";
    qStr += " LEFT OUTER JOIN TRANSACTION_HELPER TRANS ";
    qStr += " ON LED.LEDGER_ID = TRANS.LEDGER_ID ";
    qStr += " AND TRANS.VOUCHER_DATE <= '" + toDate.toString("yyyy-MM-dd") + "' ";
    qStr += " GROUP BY LED.LEDGER_ID)A ";
    //    qStr += " WHERE CLOSING <>0";



    //    qDebug()<<qStr;

    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}

QSqlQueryModel *AccountingReportDatabaseHelper::getBalanceByGroupType(QString groupType, QDate fromDate, QDate toDate)
{

    //   not used
    QSqlQueryModel *model = new QSqlQueryModel();
    int i;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QSqlQuery query2(QSqlDatabase::database(DatabaseValues::connectionString));

    QString qStr = "";
    QString qStr1 = " SELECT GROUP_ID, GROUP_NAME, '' as Value FROM  " + AccountGroupMasterDatabaseHelper::Accounts_Group_TableName +"  ";
    qStr1 += " WHERE GROUP_TYPE = '" + groupType + "'";
    qStr1 += " AND PARENT_ID = '0'";

    qDebug()<<qStr1;

    query.prepare(qStr1);

    if(query.exec()){
        i=0;
        while(query.next()){
            qStr += "SELECT '" + query.value(1).toString() + "' as " + groupType + " ,";
            qStr += " CASE WHEN  round(sum(credit_amount) - sum(debit_amount),2) <0 then -1* round(sum(credit_amount) - sum(debit_amount),2)";
            qStr += " ELSE round(sum(credit_amount) - sum(debit_amount),2) END as ' '  from transaction_helper";
            qStr += " where ledger_id in ";
            qStr += " (select ledger_id FROM " + led.Ledger_Master_TableName + " where group_id in ";
            qStr += " (select group_id from (SELECT  GROUP_ID,  GROUP_NAME, PARENT_ID  ";
            qStr += " FROM    (SELECT * FROM  " + AccountGroupMasterDatabaseHelper::Accounts_Group_TableName +"   ORDER BY PARENT_ID, GROUP_ID) GROUPS_SORTED, ";
            qStr += "         (SELECT @PV" + QString::number(i) + " := '"+ query.value(0).toString() +"') INITIALISATION ";
            qStr += " WHERE   FIND_IN_SET(PARENT_ID, @PV" + QString::number(i) + ") ";
            qStr += " AND     LENGTH(@PV" + QString::number(i) + " := CONCAT(@PV" + QString::number(i) + ", ',', GROUP_ID))";
            qStr += " UNION  ";
            qStr += " SELECT  GROUP_ID, GROUP_NAME, PARENT_ID FROM  " + AccountGroupMasterDatabaseHelper::Accounts_Group_TableName +"  ";
            qStr += " WHERE GROUP_ID = '"+ query.value(0).toString() +"')";
            qStr += "  " + AccountGroupMasterDatabaseHelper::Accounts_Group_TableName +" ))";
            //            qStr += " AND VOUCHER_DATE >= '" + fromDate.toString("yyyy-MM-dd") + "' ";
            qStr += " AND VOUCHER_DATE <= '" + toDate.toString("yyyy-MM-dd") + "' ";
            qStr += " UNION ";
            i++;
        }
        qStr += " SELECT '','' ";
        model = DatabaseHelper::getQueryModel(qStr);
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return model;
}

float AccountingReportDatabaseHelper::getStockValueByDate(QDate dateTo)
{
    //   not used
    SalesInventoryItemDatabaseHelper shelper;
    ItemTransactionDatabaseHelper itemHelper;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    float value;
    QString qStr = " SELECT coalesce((SELECT SUM(" + itemHelper.Transaction_Item_Helper_Cr_Amount ;
    qStr += ") - SUM(" + itemHelper.Transaction_Item_Helper_Dr_Amount + ") FROM transaction_item_helper ";
    qStr += " WHERE " + itemHelper.Transaction_Item_Helper_VoucherDate + "<='"+dateTo.toString("yyyy-MM-dd")+"'),0)";
    qStr += " - coalesce((SELECT sum( " + shelper.Sales_Inventory_OpeningValue + ") ";
    qStr += " from " + shelper.Sales_Inventory_Table_Name + " WHERE ";
    qStr += shelper.Sales_Inventory_OpeningBalanceDate  + "<='"+dateTo.toString("yyyy-MM-dd")+"'),0)";

    //    qDebug()<<qStr;

    query.prepare(qStr);
    if(query.exec()){
        if(query.first()) {
            value = query.value(0).toFloat();
        }
    }

    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }

    //    model->setQuery(qStr,QSqlDatabase::database(DatabaseValues::connectionString));
    return value;

}

float AccountingReportDatabaseHelper::getBalanceByGroup(QString groupID, QDate fromDate, QDate toDate)
{
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    float balance =0;

    QString qStr = "SELECT SUM(CLOSING) FROM ";
    qStr += " (SELECT LED.LEDGER_NAME, LED.LEDGER_ID,  ";
    qStr += " CAST( ";
    qStr += " ROUND(COALESCE(SUM(TRANS.CREDIT_AMOUNT),0),2) - ROUND(COALESCE(SUM(TRANS.DEBIT_AMOUNT),0),2) AS DECIMAL(10,2)) AS CLOSING FROM " + led.Ledger_Master_TableName + " LED ";
    qStr += " INNER JOIN  ";
    qStr += " (SELECT  GROUP_ID, ";
    qStr += "         GROUP_NAME, ";
    qStr += "         PARENT_ID  ";
    qStr += " FROM    (SELECT * FROM  " + AccountGroupMasterDatabaseHelper::Accounts_Group_TableName +"  ";
    qStr += "          ORDER BY PARENT_ID, GROUP_ID) GROUPS_SORTED, ";
    qStr += "         (SELECT @PV := '" + groupID + "') INITIALISATION ";
    qStr += " WHERE   FIND_IN_SET(PARENT_ID, @PV) ";
    qStr += " AND     LENGTH(@PV := CONCAT(@PV, ',', GROUP_ID)) ";
    qStr += " UNION  ";
    qStr += " SELECT  GROUP_ID, ";
    qStr += "         GROUP_NAME, ";
    qStr += "         PARENT_ID  ";
    qStr += " FROM    " + AccountGroupMasterDatabaseHelper::Accounts_Group_TableName +"   ";
    qStr += " WHERE GROUP_ID = '" + groupID + "') AS ACC_GROUP ";
    qStr += " ON LED.GROUP_ID = ACC_GROUP.GROUP_ID ";
    qStr += " LEFT OUTER JOIN TRANSACTION_HELPER TRANS ";
    qStr += " ON LED.LEDGER_ID = TRANS.LEDGER_ID ";
    qStr += " AND TRANS.VOUCHER_DATE >= '" + fromDate.toString("yyyy-MM-dd") + "' ";
    qStr += " AND TRANS.VOUCHER_DATE <= '" + toDate.toString("yyyy-MM-dd") + "' ";
    qStr += " GROUP BY LED.LEDGER_ID)A ";

        qDebug()<<qStr;
    query.prepare(qStr);

    if(query.exec())    {
        if(query.first()){
            balance = query.value(0).toFloat();
        }
    }

    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }

    if(groupID == "0x5"){
//        qDebug()<<"Balance"<<balance;
        balance += -1 * getRestaurantStockValueByDate(toDate);
//        qDebug()<<balance;
    }
    return balance;
}


float AccountingReportDatabaseHelper::getProfitAndLossValue(QDate fromDate, QDate toDate)
{
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    AccountGroupMasterDatabaseHelper* grpHelper = new AccountGroupMasterDatabaseHelper();
    float balance =0;
    float incomeTotal = 0;
    float expenseTotal = 0;

    QList<AccountGroupDataModel> incomeGrp = grpHelper->getPrimaryAccountsByGroupType("Income");
    QList<AccountGroupDataModel> expenseGrp = grpHelper->getPrimaryAccountsByGroupType("Expense");

    for(AccountGroupDataModel acc:incomeGrp)
    {
        float val = getBalanceByGroup(acc.groupID, fromDate, toDate);
        incomeTotal +=  val;
        qDebug()<<"income loop"<<QString::number(val, 'f',2)<<acc.groupID;
    }

    qDebug()<<"Income"<<incomeTotal;
//    qDebug()<<"closing stock"<<getRestaurantStockValueByDate(toDate);
    float clsStkVal = getRestaurantStockValueByDate(toDate);
    incomeTotal += clsStkVal; //Closing stock
    qDebug()<<"closing "<<QString::number(clsStkVal, 'f',2);
    qDebug()<<incomeTotal;
    for(AccountGroupDataModel acc:expenseGrp)
    {
        float val = getBalanceByGroup(acc.groupID, fromDate, toDate);
        expenseTotal += -1 * val;
        qDebug()<<"exp loop"<<QString::number(val, 'f',2)<<acc.groupID;
    }
    qDebug()<<"Expense"<<expenseTotal;
    float openStkVal = getRestaurantStockValueByDate(fromDate.addDays(-1));
    expenseTotal += openStkVal; //Opening stock
    qDebug()<<"open "<<QString::number(openStkVal, 'f',2);
    balance = incomeTotal - expenseTotal;

    return balance;
}

float AccountingReportDatabaseHelper::getOpeningBalanceDifference(QDate fromDate, QDate toDate)
{
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    float diff =0;
    QString qStr = " SELECT (SELECT sum(Opening_Balance) FROM " + led.Ledger_Master_TableName + ") - ";
    qStr += " ( SELECT sum(case when opening_value >0 then opening_value else (opening_stock * std_cost) end ) FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" )";
    //    qDebug()<<qStr;
    query.prepare(qStr);

    if(query.exec()){
        if(query.first()){
            diff = query.value(0).toFloat();
        }
    }

    return diff;
}


float AccountingReportDatabaseHelper::getRestaurantStockValueByDate(QDate dateTo)
{
    // usng lpp
    SalesInventoryItemDatabaseHelper shelper;
    ItemTransactionDatabaseHelper itemHelper;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    float value =0;

    QString qStr = " select round(sum(stockValue),2) from ";
    qStr += "  (select stockQty * ";
    qStr += " (select price from purchase_invoice_details where inventory_item_id = itemID ";
    qStr += " and voucher_Date <= " + db->getDateString(dateTo);
    qStr += " order by voucher_Date desc limit 1) as stockValue from ";
    qStr += " ( select itemID, sum((trans.Dr_Qty - trans.Cr_Qty) ) as stockQty ";
    qStr += " from Transaction_Item_Helper trans where ";
    qStr += " VoucherDate <= " + db->getDateString(dateTo);
    qStr += " group by itemID UNION ALL ";
    qStr += " select itemID, sum((trans.Dr_Qty - trans.Cr_Qty) ) as stockQty ";
    qStr += " from Transaction_Bom_Helper trans ";
    qStr += " where VoucherDate <= " + db->getDateString(dateTo) + " group  by itemID)A)B ";



//    qStr += "( select sum((trans.Dr_Qty - trans.Cr_Qty) * ";
//    qStr += " (select price from purchase_invoice_details ";
//    qStr += " where inventory_item_id = itemID and voucher_Date <= " + db->getDateString(dateTo);
//    qStr += " order by voucher_Date desc limit 1)";
//    qStr += " ) as stockvalue ";
////    qStr += " (select std_cost FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where item_id = itemID ) ) as stockValue ";
//    qStr += " from Transaction_Item_Helper trans where VoucherDate <= " + db->getDateString(dateTo);
//    qStr += " UNION ALL ";
//    qStr += " select sum((trans.Dr_Qty - trans.Cr_Qty) * ";
//    qStr += " (select price from purchase_invoice_details ";
//    qStr += " where inventory_item_id = itemID and voucher_Date <= " + db->getDateString(dateTo);
//    qStr += " order by voucher_Date desc limit 1)";
//    qStr += " ) as stockvalue ";
////    qStr += " (select std_cost FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where item_id = itemID ) ) as stockValue ";
//    qStr += " from Transaction_Bom_Helper trans where VoucherDate <= " + db->getDateString(dateTo);
//    qStr += " ) A ";

        qDebug()<<qStr<<Q_FUNC_INFO;

    query.prepare(qStr);
    if(query.exec()){
        if(query.first()) {
            value = query.value(0).toFloat();
        }
    }

    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }

    //    model->setQuery(qStr,QSqlDatabase::database(DatabaseValues::connectionString));
    return value;

}
