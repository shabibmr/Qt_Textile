#include "ledgerhelper.h"


LedgerHelper::LedgerHelper()
{
    db = new DatabaseHelper();
    ledHelper = new LedgerMasterDatabaseHelper;
    accHelper = new AccountGroupMasterDatabaseHelper;
}

float LedgerHelper::getClosingBalanceOfLedger(QString ledGerID, QDate date)
{
    // not used as of 7/3/21
    float balance = 0;
    TransactionHelper *t = new TransactionHelper();
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "SELECT ";

    qStr += "Cast((SELECT "+ledHelper->Ledger_Master_openingBalance + " FROM "+ ledHelper->Ledger_Master_TableName;
    qStr += " WHERE "+ ledHelper->Ledger_Master_LedgerID+ "=:id) as DECIMAL (10,2))";
    qStr += ",";
    qStr += "Cast((SELECT SUM("+t->Transaction_Helper_Credit_Amount+"-"+t->Transaction_Helper_Debit_Amount+") FROM "
            + Transaction_Helper_TableName;
    qStr += " WHERE "+t->Transaction_Helper_LedgerID + "= '"+ledGerID + "'";
    qStr += " AND "+t->Transaction_Helper_VoucherDate +"<=:date ) as DECIMAL (10,2))";
    qStr += ")";

    query.prepare(qStr);
    query.bindValue(":id",ledGerID);
    query.bindValue(":date",date);
    if(query.exec()){
        if(query.first()){
            balance = /*query.value(0).toFloat()+*/query.value(1).toFloat() ;
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return balance;
}

float LedgerHelper::getOpeningBalanceOfLedger(QString ledGerID, QDate date)
{
    float balance = 0;

    TransactionHelper *t = new TransactionHelper();
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    QString qStr = "SELECT ";

    qStr += "Cast((SELECT "+ledHelper->Ledger_Master_openingBalance + " FROM "+ ledHelper->Ledger_Master_TableName;
    qStr += " WHERE "+ ledHelper->Ledger_Master_LedgerID+ "='"+ledGerID+"') as DECIMAL (10,2))";
    qStr += ",";

    qStr += "Cast((SELECT SUM("+t->Transaction_Helper_Credit_Amount+"-"+t->Transaction_Helper_Debit_Amount+") FROM "
            + Transaction_Helper_TableName;
    qStr += " WHERE "+t->Transaction_Helper_LedgerID + "= '"+ledGerID + "'";
    qStr += " AND "+t->Transaction_Helper_VoucherDate +"<:date ) as DECIMAL (10,2))";
    //    qStr += ")";

    query.prepare(qStr);
    query.bindValue(":id",ledGerID);
    query.bindValue(":date",date);

    qDebug()<<qStr;

    if(query.exec()){
        if(query.first()){
            balance = query.value(1).toFloat() ;
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    qDebug()<<query.lastQuery();

    return balance;
}

float LedgerHelper::getOpeningBalanceOfContact(QString phone, QDate date)
{
    float balance = 0;

    TransactionHelper *t = new TransactionHelper();
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));


    QString ledGerID = ledHelper->getLedgerIDByName("Advance Payment");

    QString qStr = "SELECT round(SUM("+t->Transaction_Helper_Credit_Amount+"-"+t->Transaction_Helper_Debit_Amount+"),2) FROM "
            + Transaction_Helper_TableName;
    qStr += " WHERE "+t->Transaction_Helper_LedgerID + "= '"+ledGerID + "'";
    qStr += " AND "+t->Transaction_Helper_VoucherDate +"< " + db->getDateString(date);
    qStr += " AND (";
    qStr += " case when " + t->Transaction_Helper_VoucherType + " = '" + GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesVoucher) + "'" ;
    qStr += " then " + t->Transaction_Helper_VoucherNo + " in ";
    qStr += " ( SELECT Voucher_No from Sales_Invoice_Main Where POC_Phone = '" + phone + "')";
    qStr += "  when " + t->Transaction_Helper_VoucherType + " = '" + GMVoucherTypes::getVoucherString(GMVoucherTypes::PurchaseVoucher) + "'" ;
    qStr += " then " + t->Transaction_Helper_VoucherNo + " in ";
    qStr += " ( SELECT Voucher_No from Purchase_Invoice_Main Where POC_Phone = '" + phone + "')";
    qStr += "  when " + t->Transaction_Helper_VoucherType + " = '" + GMVoucherTypes::getVoucherString(GMVoucherTypes::PaymentVoucher) + "'" ;
    qStr += " then " + t->Transaction_Helper_VoucherNo + " in ";
    qStr += " ( SELECT Voucher_No from Payment_Voucher_Main Where POC_Phone = '" + phone + "')";
    qStr += "  when " + t->Transaction_Helper_VoucherType + " = '" + GMVoucherTypes::getVoucherString(GMVoucherTypes::ReceiptVoucher) + "'" ;
    qStr += " then " + t->Transaction_Helper_VoucherNo + " in ";
    qStr += " ( SELECT Voucher_No from Receipt_Voucher_Main Where POC_Phone = '" + phone + "')";
    qStr += " end )";
    //    qStr += ")";

    qDebug()<<qStr;
    query.prepare(qStr);
    if(query.exec()){
        if(query.first()){
            balance = query.value(0).toFloat() ;
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }

    return balance;
}

QSqlQueryModel *LedgerHelper::getVisitorsList(QDateTime fromDate, QDateTime toDate)
{
    QString qStr = "SELECT  substr(`DATEVAL`,1,10) as Date, substr(`DATEVAL`, 12,8) as Time, `NAME` as Name, `PHONENUMBER` as Phone , `ADDRESS` as Address , `TEMPERATURE` as Temperature";
    qStr += " FROM `visitorsregister` WHERE TIME_STAMP >= "+db->getDateTimeString(fromDate);
    qStr += " AND ";
    qStr += " TIME_STAMP <= "+db->getDateTimeString(toDate);

    qDebug()<<qStr;


    return DatabaseHelper::getQueryModel(qStr);

}

QSqlQueryModel *LedgerHelper::getDatewiseSales(QDateTime fromDate, QDateTime toDate)
{
    QSqlQueryModel* model = new QSqlQueryModel;
    salesVoucherDatabaseHelper shelper;

    QString qStr = "";

    qStr += " SELECT ";

    qStr += " Date_format("+shelper.Sales_Invoice_Main_Voucher_Date+",'%d-%m-%Y') as 'Date',";
    qStr += " cast(COALESCE(sum("+shelper.Sales_Invoice_Main_Gross_Total+"),0.00) as Decimal(10,2)) As 'Taxable Amount',";
    qStr += " cast(COALESCE(SUM("+shelper.Sales_Invoice_Main_VATAmount+"),0) as decimal(10,2)) as 'Tax Amount',";
    qStr += " Cast(COALESCE(SUM("+shelper.Sales_Invoice_Main_Total+"),0) as Decimal(10,2)) as 'Total Amount' ";

    qStr += " FROM "+shelper.Sales_Invoice_Main_TableName;
    qStr += " WHERE "+shelper.Sales_Invoice_Main_TimeStamp+" >='"+fromDate.toString("yyyy-MM-dd hh:mm:ss")+"'";
    qStr += " AND "+shelper.Sales_Invoice_Main_TimeStamp+"<='"+toDate.toString("yyyy-MM-dd hh:mm:ss")+"' ";
    qStr += " GROUP By("+shelper.Sales_Invoice_Main_Voucher_Date+")";

    qDebug()<<qStr;
    model = DatabaseHelper::getQueryModel(qStr);

    return model;
}


QSqlQueryModel *LedgerHelper::getDatewisePurchase(QDateTime fromDate, QDateTime toDate)
{
    QSqlQueryModel* model = new QSqlQueryModel;
    PurchaseVoucherDatabaseHelper shelper;

    QString qStr = "";

    qStr += " SELECT ";

    qStr += " Date_format("+shelper.Purchase_Invoice_Main_Voucher_Date+",'%d-%m-%Y') as 'Date',";
    qStr += " cast(COALESCE(sum("+shelper.Purchase_Invoice_Main_Gross_Total+"),0.00) as Decimal(10,2)) As 'Taxable Amount',";
    qStr += " cast(COALESCE(SUM("+shelper.Purchase_Invoice_Main_VATAmount+"),0) as decimal(10,2)) as 'Tax Amount',";
    qStr += " Cast(COALESCE(SUM("+shelper.Purchase_Invoice_Main_Total+"),0) as Decimal(10,2)) as 'Total Amount' ";

    qStr += " FROM "+shelper.Purchase_Invoice_Main_TableName;
    qStr += " WHERE "+shelper.Purchase_Invoice_Main_TimeStamp+" >='"+fromDate.toString("yyyy-MM-dd hh:mm:ss")+"'";
    qStr += " AND "+shelper.Purchase_Invoice_Main_TimeStamp+"<='"+toDate.toString("yyyy-MM-dd hh:mm:ss")+"' ";
    qStr += " GROUP By("+shelper.Purchase_Invoice_Main_Voucher_Date+")";

    qDebug()<<qStr;
    model = DatabaseHelper::getQueryModel(qStr);

    return model;
}

QSqlQueryModel *LedgerHelper::getLedgerReportModel(QString ledgerID, QDate fromDate, QDate toDate, int limit)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    TransactionHelper *t= new TransactionHelper();
    QString qStr = " SELECT Ledger, A.Voucher_Date as 'Date', A.Voucher_ID as 'Voucher NO', A.Voucher_Type as 'Voucher Type', ";
    qStr += "DrAmount as 'Dr Amount', CrAmount as 'Cr Amount', Voucher_Prefix, ";
    qStr += " case when running_bal<0 then concat(-1 *running_bal, ' Cr') else concat(running_bal, ' Dr') end as 'Running Balance' FROM (";
    qStr +=" with subq as (SELECT Against_Ledger, sum(Credit_Amount) as cred,sum(Debit_Amount) as deb, ";
    qStr += " Voucher_Date, Voucher_ID, Voucher_Type, Voucher_Prefix ";
    qStr += " From Transaction_Helper Where Ledger_ID='" + ledgerID + "' ";

    qStr += " GROUP BY Voucher_ID, Voucher_Type, voucher_prefix ";
    qStr += " ORDER BY Voucher_Date, cast(voucher_id as int), Voucher_Type, _id )";
    qStr += " SELECT (SELECT ledger_name FROM "+ LedgerMasterDatabaseHelper::Ledger_Master_TableName
            ;
    qStr += " where ledger_id = subq.Against_Ledger) AS LEDGER, ";
    qStr += " subq.Voucher_Date , subq.Voucher_ID , subq.Voucher_Type , ";
    qStr += " case when subq.deb  > 0 then subq.deb else 0 end as 'DrAmount', ";
    qStr += " case when subq.cred  > 0 then subq.cred else 0 end as 'CrAmount', ";
    qStr += " subq.Voucher_Prefix , sum(subq.deb - subq.cred) ";
    qStr += " OVER (ORDER BY voucher_date, cast(voucher_id as int), voucher_type) as running_bal ";
    qStr += " from subq ";
    qStr += " group by subq.voucher_type, subq.voucher_id, subq.voucher_prefix ) A ";
    qStr += " WHERE Voucher_Date >="+DatabaseHelper::getDateStringStatic(fromDate);
    qStr += " AND Voucher_Date <="+DatabaseHelper::getDateStringStatic(toDate);
    if(limit > 0)
    {
        qStr += " ORDER BY 2 desc LIMIT " + QString::number(limit);
    }
    else
        qStr += " order by Voucher_Date, cast(voucher_id as int), voucher_type ";

    qDebug()<<qStr;
    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}

QSqlQueryModel *LedgerHelper::getLedgerTotalsModel(QString ledgerID, QDate fromDate, QDate toDate)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    TransactionHelper *t= new TransactionHelper();
    QString qStr="SELECT ";

    qStr += " sum(subq.deb) as Debit";
    qStr += ", sum(subq.cred)  as Credit ";
    qStr += " FROM ";

    qStr += "( SELECT ";
    qStr += t->Transaction_Helper_AgainstLedger ;
    qStr += ", sum("+t->Transaction_Helper_Credit_Amount +") as cred, sum("+t->Transaction_Helper_Debit_Amount+") as deb";
    qStr += ", "+t->Transaction_Helper_VoucherDate;
    qStr += ", "+t->Transaction_Helper_VoucherNo;
    qStr += ", "+t->Transaction_Helper_VoucherType;
    qStr += ", "+t->Transaction_Helper_VoucherPrefix;
    qStr += " From "+t->Transaction_Helper_TableName;
    qStr += " Where ";
    qStr += t->Transaction_Helper_LedgerID +  "='"+ledgerID +"'";
    qStr += " AND ";
    qStr += t->Transaction_Helper_VoucherDate +" >='"+fromDate.toString("yyyy-MM-dd") +"'";
    qStr += " AND ";
    qStr += t->Transaction_Helper_VoucherDate +" <='"+toDate.toString("yyyy-MM-dd") +"'";
    qStr += " GROUP BY ";
    qStr += t->Transaction_Helper_VoucherNo +", "+t->Transaction_Helper_VoucherType;
    qStr += " ORDER BY ";
    qStr += t->Transaction_Helper_VoucherDate +", ";
    qStr += t->Transaction_Helper_VoucherNo +", "+t->Transaction_Helper_VoucherType;
    qStr += " ) subq";

    qDebug()<<qStr;
    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}

QSqlQueryModel *LedgerHelper::getContactwiseLedgerReportModel(QString phone, QDate fromDate, QDate toDate)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    TransactionHelper *t= new TransactionHelper();



    QString ledgerID = ledHelper->getLedgerIDByName("Advance Payment");

    QString qStr="SELECT * from ( SELECT ";

    //    qStr += "(SELECT ledger_name FROM " + ledHelper->Ledger_Master_TableName + " where ledger_id = subq."+t->Transaction_Helper_AgainstLedger+") AS LEDGER ";
    qStr += " subq."+t->Transaction_Helper_VoucherDate;
    qStr += ", subq."+t->Transaction_Helper_VoucherNo;
    qStr += ", subq."+t->Transaction_Helper_VoucherType;
    qStr += ", case when subq.deb>0 then subq.deb else '' end as Debit";
    qStr += ", case when subq.cred>0 then subq.cred else '' end as Credit ";
    qStr += ", subq."+t->Transaction_Helper_VoucherPrefix + " FROM ";

    qStr += "( SELECT ";
    qStr += t->Transaction_Helper_AgainstLedger ;
    qStr += ", SUM("+t->Transaction_Helper_Credit_Amount +"-"+t->Transaction_Helper_Debit_Amount+") as cred";
    qStr += ", SUM("+t->Transaction_Helper_Debit_Amount +"-"+t->Transaction_Helper_Credit_Amount+") as deb";

    qStr += ", "+t->Transaction_Helper_VoucherDate;
    qStr += ", "+t->Transaction_Helper_VoucherNo;
    qStr += ", "+t->Transaction_Helper_VoucherType;
    qStr += ", "+t->Transaction_Helper_VoucherPrefix;
    qStr += " From "+t->Transaction_Helper_TableName;
    qStr += " Where ";
    qStr += t->Transaction_Helper_LedgerID +  "='" + ledgerID + "'";
    qStr += " AND ";
    qStr += t->Transaction_Helper_VoucherDate +" >='"+fromDate.toString("yyyy-MM-dd") +"'";
    qStr += " AND ";
    qStr += t->Transaction_Helper_VoucherDate +" <='"+toDate.toString("yyyy-MM-dd") +"'";
    qStr += " AND (";
    qStr += " case when " + t->Transaction_Helper_VoucherType + " = '" + GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesVoucher) + "'" ;
    qStr += " then " + t->Transaction_Helper_VoucherNo + " in ";
    qStr += " ( SELECT Voucher_No from Sales_Invoice_Main Where POC_Phone = '" + phone + "')";
    qStr += "  when " + t->Transaction_Helper_VoucherType + " = '" + GMVoucherTypes::getVoucherString(GMVoucherTypes::PurchaseVoucher) + "'" ;
    qStr += " then " + t->Transaction_Helper_VoucherNo + " in ";
    qStr += " ( SELECT Voucher_No from Purchase_Invoice_Main Where POC_Phone = '" + phone + "')";
    qStr += "  when " + t->Transaction_Helper_VoucherType + " = '" + GMVoucherTypes::getVoucherString(GMVoucherTypes::PaymentVoucher) + "'" ;
    qStr += " then " + t->Transaction_Helper_VoucherNo + " in ";
    qStr += " ( SELECT Voucher_No from Payment_Voucher_Main Where POC_Phone = '" + phone + "')";
    qStr += "  when " + t->Transaction_Helper_VoucherType + " = '" + GMVoucherTypes::getVoucherString(GMVoucherTypes::ReceiptVoucher) + "'" ;
    qStr += " then " + t->Transaction_Helper_VoucherNo + " in ";
    qStr += " ( SELECT Voucher_No from Receipt_Voucher_Main Where POC_Phone = '" + phone + "')";
    qStr += " end )";

    qStr += " GROUP BY ";
    qStr += t->Transaction_Helper_VoucherNo +", "+t->Transaction_Helper_VoucherType;
    qStr += " ORDER BY ";
    qStr += t->Transaction_Helper_VoucherDate +", ";
    qStr += t->Transaction_Helper_VoucherNo +", "+t->Transaction_Helper_VoucherType;
    qStr += " ) subq";

    qStr += " UNION SELECT ";
    qStr += "  so.Voucher_Date, so.Voucher_No, 'SALES ORDER', cast(so.Total as decimal(10,2)) as Debit, 0 as Credit ";
    qStr += ", so.Voucher_Prefix  FROM Sales_Order_Main so ";
    qStr += " Where POC_Phone = '" + phone + "'";
    qStr += " AND ";
    qStr += "so.Voucher_Date  >='"+fromDate.toString("yyyy-MM-dd") + "'" ;
    qStr += " AND ";
    qStr += "so.Voucher_Date  <='"+toDate.toString("yyyy-MM-dd") + "'" ;
    qStr += " AND so.VoucherStatus  < " + QString::number(QuotationStatus::SalesInvoiceRaised);

    qStr += ") A order by 1";
  qDebug()<<qStr;
    if(phone.length()>0){
        model = DatabaseHelper::getQueryModel(qStr);
    }


    return model;
}

QSqlQueryModel *LedgerHelper::getB2BSalesReportWithTaxModelByDate(QDateTime fromDate, QDateTime toDate)
{

    salesVoucherDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper itemHelper;

    QSqlQueryModel* model = new QSqlQueryModel;

    QString timestamp = "concat("+salesVoucherDatabaseHelper::Sales_Invoice_Main_Voucher_Date+",' ',CAST("
            +salesVoucherDatabaseHelper::Sales_Invoice_Main_TimeStamp+" AS TIME))";

    timestamp = "str_to_Date(concat(stable." + salesVoucherDatabaseHelper::Sales_Invoice_Main_Voucher_Date ;
    timestamp += ", substr(" + salesVoucherDatabaseHelper::Sales_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;


    qDebug()<<salesVoucherDatabaseHelper::Sales_Invoice_Main_Voucher_Date;
    qDebug()<<ledHelper->Ledger_Master_TableName;

    QString qStr;
    qStr += " SELECT trans.Voucher_Date as 'Voucher Date', trans.Voucher_Id as 'Voucher No', trans.Voucher_Prefix as 'Voucher Prefix', ";
    qStr += " led.Ledger_name as Ledger, TRN as GSTIN, ";
    qStr += " sales.gross_total as 'Taxable Amount', max(Sales0GST) as 'Sales @ 0% GST',  ";
    qStr += " max(Sales5GST) as 'Sales @ 5% GST', max(OPC25) as 'Output CGST @ 2.5%', max(OPS25) as 'Output SGST @ 2.5%',";
    qStr += " max(Sales12GST) as 'Sales @ 12% GST', max(OPC6) as 'Output CGST @ 6%', max(OPS6) as 'Output SGST @ 6%',";
    qStr += " max(Sales18GST) as 'Sales @ 18% GST', max(OPC9) as 'Output CGST @ 9%', max(OPS9) as 'Output SGST @ 9%',";
    qStr += " max(Sales28GST) as 'Sales @ 28% GST', max(OPC14) as 'Output CGST @ 14%', max(OPS14) as 'Output SGST @ 14%',";
    qStr += " max(Cess) as 'Output Cess @ 1%', ";
    qStr += " sales.Vat_Amount as 'Total Tax', ";
    qStr += " sales.Total as Total";
    qStr += " FROM(";
    qStr += " SELECT Voucher_Id, Voucher_Prefix, Voucher_Date, ";
    qStr += " (case when Ledger_ID = '0x7x3' then Credit_Amount else 0 end) as 'Sales0GST',";
    qStr += " ( case when Ledger_ID = '0x7xSV5' then Credit_Amount else 0 end) as 'Sales5GST',";
    qStr += " ( case when Ledger_ID = '0x2x14xOCG25' then Credit_Amount else 0 end) as 'OPC25',";
    qStr += " ( case when Ledger_ID = '0x2x14xOSG25' then Credit_Amount else 0 end) as 'OPS25',";
    qStr += " ( case when Ledger_ID = '0x7xSV12' then Credit_Amount else 0 end) as 'Sales12GST',";
    qStr += " ( case when Ledger_ID = '0x2x14xOCG6' then Credit_Amount else 0 end) as 'OPC6',";
    qStr += " ( case when Ledger_ID = '0x2x14xOSG6' then Credit_Amount else 0 end) as 'OPS6',";
    qStr += " ( case when Ledger_ID = '0x7xSV18' then Credit_Amount else 0 end) as 'Sales18GST',";
    qStr += " ( case when Ledger_ID = '0x2x14xOCG9' then Credit_Amount else 0 end) as 'OPC9',";
    qStr += " ( case when Ledger_ID = '0x2x14xOSG9' then Credit_Amount else 0 end) as 'OPS9',";
    qStr += " ( case when Ledger_ID = '0x7xSV28' then Credit_Amount else 0 end) as 'Sales28GST',";
    qStr += " ( case when Ledger_ID = '0x2x14xOCG14' then Credit_Amount else 0 end) as 'OPC14',";
    qStr += " ( case when Ledger_ID = '0x2x14xOSG14' then Credit_Amount else 0 end) as 'OPS14',";
    qStr += " ( case when Ledger_ID = '0x2x14xCess' then Credit_Amount else 0 end) as 'Cess'";
    qStr += " FROM `transaction_helper` where voucher_type = 'SALESVOUCHER' ";
    qStr += " AND voucher_date >=" + db->getDateTimeString(fromDate);
    qStr += " AND voucher_date <=" + db->getDateTimeString(toDate);
    qStr += " ) trans ";
    qStr += " JOIN "+salesVoucherDatabaseHelper::Sales_Invoice_Main_TableName + " sales  ";
    qStr += " ON trans.voucher_id = sales.voucher_no";
    qStr += " and trans.voucher_prefix = sales.voucher_prefix";
    qStr += " JOIN " + LedgerMasterDatabaseHelper::Ledger_Master_TableName + " led ";
    qStr += " ON sales.ledger_id = led.ledger_id ";
    qStr += " AND (led.TRN is not null AND length(TRN) >1) ";
    qStr += " group by trans.voucher_id, trans.voucher_prefix";
    qStr += " order by trans.Voucher_Date, cast(trans.voucher_id as int)";


    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}



QSqlQueryModel *LedgerHelper::getB2CSalesReportWithTaxModelByDate(QDateTime fromDate, QDateTime toDate)
{
    QString qStr;
    qStr += " SELECT trans.Voucher_Date as 'Voucher Date', trans.Voucher_Id as 'Voucher No', trans.Voucher_Prefix as 'Voucher Prefix', ";
    qStr += " led.Ledger_name as Ledger,";
    qStr += " sales.gross_total as 'Taxable Amount', max(Sales0GST) as 'Sales @ 0% GST',  ";
    qStr += " max(Sales5GST) as 'Sales @ 5% GST', max(OPC25) as 'Output CGST @ 2.5%', max(OPS25) as 'Output SGST @ 2.5%',";
    qStr += " max(Sales12GST) as 'Sales @ 12% GST', max(OPC6) as 'Output CGST @ 6%', max(OPS6) as 'Output SGST @ 6%',";
    qStr += " max(Sales18GST) as 'Sales @ 18% GST', max(OPC9) as 'Output CGST @ 9%', max(OPS9) as 'Output SGST @ 9%',";
    qStr += " max(Sales28GST) as 'Sales @ 28% GST', max(OPC14) as 'Output CGST @ 14%', max(OPS14) as 'Output SGST @ 14%',";
    qStr += " max(Cess) as 'Output Cess @ 1%', ";
    qStr += " sales.Vat_Amount as 'Total Tax', ";
    qStr += " sales.Total as Total";
    qStr += " FROM(";
    qStr += " SELECT Voucher_Id, Voucher_Prefix, Voucher_Date, ";
    qStr += " (case when Ledger_ID = '0x7x3' then Credit_Amount else 0 end) as 'Sales0GST',";
    qStr += " ( case when Ledger_ID = '0x7xSV5' then Credit_Amount else 0 end) as 'Sales5GST',";
    qStr += " ( case when Ledger_ID = '0x2x14xOCG25' then Credit_Amount else 0 end) as 'OPC25',";
    qStr += " ( case when Ledger_ID = '0x2x14xOSG25' then Credit_Amount else 0 end) as 'OPS25',";
    qStr += " ( case when Ledger_ID = '0x7xSV12' then Credit_Amount else 0 end) as 'Sales12GST',";
    qStr += " ( case when Ledger_ID = '0x2x14xOCG6' then Credit_Amount else 0 end) as 'OPC6',";
    qStr += " ( case when Ledger_ID = '0x2x14xOSG6' then Credit_Amount else 0 end) as 'OPS6',";
    qStr += " ( case when Ledger_ID = '0x7xSV18' then Credit_Amount else 0 end) as 'Sales18GST',";
    qStr += " ( case when Ledger_ID = '0x2x14xOCG9' then Credit_Amount else 0 end) as 'OPC9',";
    qStr += " ( case when Ledger_ID = '0x2x14xOSG9' then Credit_Amount else 0 end) as 'OPS9',";
    qStr += " ( case when Ledger_ID = '0x7xSV28' then Credit_Amount else 0 end) as 'Sales28GST',";
    qStr += " ( case when Ledger_ID = '0x2x14xOCG14' then Credit_Amount else 0 end) as 'OPC14',";
    qStr += " ( case when Ledger_ID = '0x2x14xOSG14' then Credit_Amount else 0 end) as 'OPS14',";
    qStr += " ( case when Ledger_ID = '0x2x14xCess' then Credit_Amount else 0 end) as 'Cess'";
    qStr += " FROM `transaction_helper` where voucher_type = 'SALESVOUCHER' ";
    qStr += " AND voucher_date >=" + db->getDateTimeString(fromDate);
    qStr += " AND voucher_date <=" + db->getDateTimeString(toDate);
    qStr += " ) trans ";
    qStr += " JOIN "+salesVoucherDatabaseHelper::Sales_Invoice_Main_TableName + " sales  ";
    qStr += " ON trans.voucher_id = sales.voucher_no";
    qStr += " and trans.voucher_prefix = sales.voucher_prefix";
    qStr += " JOIN " + LedgerMasterDatabaseHelper::Ledger_Master_TableName + " led ";
    qStr += " ON sales.ledger_id = led.ledger_id ";
    qStr += " AND length(TRN) <1";
    qStr += " group by trans.voucher_id, trans.voucher_prefix";
    qStr += " order by trans.Voucher_Date, cast(trans.voucher_id as int)";


    return DatabaseHelper::getQueryModel(qStr);
}

QSqlQueryModel *LedgerHelper::getAccGroupSummaryModel(QString groupID, QDateTime fromDate, QDateTime toDate)
{

    QString qStr;
    qStr += " SELECT LEDGER_NAME as ledger, DEBIT, CREDIT FROM ";
    qStr += " (SELECT LED.LEDGER_NAME, LED.LEDGER_ID, COALESCE(SUM(TRANS.CREDIT_AMOUNT),0) as CREDIT , ";
    qStr += " COALESCE(SUM(TRANS.DEBIT_AMOUNT),0) as DEBIT,   ";
    qStr += " CAST(COALESCE(SUM(TRANS.CREDIT_AMOUNT),0) - COALESCE(SUM(TRANS.DEBIT_AMOUNT),0) AS DECIMAL(10,2)) AS CLOSING ";
    qStr += " FROM " + ledHelper->Ledger_Master_TableName + " LED ";
    qStr += " INNER JOIN  ";
    qStr += " (SELECT  GROUP_ID, ";
    qStr += "         GROUP_NAME, ";
    qStr += "         PARENT_ID  ";
    qStr += " FROM    (SELECT * FROM " + accHelper->Accounts_Group_TableName + " ";
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

    return DatabaseHelper::getQueryModel(qStr);

}

QSqlQueryModel *LedgerHelper::getLedgerwiseOutstandingModel(QString ledgerID,  QDate toDate)
{
    QString qStr = " SELECT 'V' as Ind, RefNo, RefPrefix, RefType, min(VoucherDate) as 'Voucher Date', ";
    qStr += " RefNo as 'Voucher No', RefType as 'Voucher Type', (select sum(DebitAmount - creditAmount) ";
    qStr += " from billwise_records ";
    qStr += " where voucherNo = Ref.RefNo AND voucherType = Ref.RefType AND voucherPrefix = Ref.RefPrefix) as 'Bill Amount', ";
    qStr += " SUM(COALESCE(DebitAmount,0)) -  SUM(COALESCE(CreditAmount,0)) as 'Balance Amount' ";
    qStr += " FROM Billwise_Records Ref ";
    qStr += " WHERE Ref.LedgerId = '" + ledgerID + "' ";
    qStr += " AND Ref.VoucherDate <= " + DatabaseHelper::getDateStringStatic(toDate);
             qStr += " AND (Method_Of_Adj<> 'ON ACC' OR Method_Of_Adj IS NULL) ";
    qStr += " GROUP BY RefNo, RefType, RefPrefix ";
    qStr += " HAVING SUM(COALESCE(CreditAmount,0)) - SUM(COALESCE(DebitAmount,0)) <> 0 ";
    qStr += " ORDER BY 5";


    return DatabaseHelper::getQueryModel(qStr);

}

QSqlQueryModel *LedgerHelper::getSalesReportWithTaxModelByDate(QDateTime fromDate, QDateTime toDate)
{

    qDebug()<<Q_FUNC_INFO;

    salesVoucherDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper itemHelper;

    QSqlQueryModel* model = new QSqlQueryModel;

    QString qStr ;

    qStr += " SELECT trans.Voucher_Date, trans.Voucher_Id, trans.Voucher_Prefix, ";
    qStr += " (select Ledger_name from " + LedgerMasterDatabaseHelper::Ledger_Master_TableName ;
    qStr += " where Ledger_ID = sales.Ledger_ID) as Ledger,";
    qStr += " sales.gross_total as 'Taxable Amount', max(Sales0GST) as 'Sales @ 0% GST', ";
    qStr += " max(Sales5GST) as 'Sales @ 5% GST', max(OPC25) as 'Output CGST @ 2.5%', max(OPS25) as 'Output SGST @ 2.5%',";
    qStr += " max(Sales12GST) as 'Sales @ 12% GST', max(OPC6) as 'Output CGST @ 6%', max(OPS6) as 'Output SGST @ 6%',";
    qStr += " max(Sales18GST) as 'Sales @ 18% GST', max(OPC9) as 'Output CGST @ 9%', max(OPS9) as 'Output SGST @ 9%',";
    qStr += " max(Sales28GST) as 'Sales @ 28% GST', max(OPC14) as 'Output CGST @ 14%', max(OPS14) as 'Output SGST @ 14%',";
    qStr += " max(Cess) as 'Output Cess @ 1%', ";
    qStr += " sales.Vat_Amount as 'Total Tax', ";
    qStr += " sales.Total as Total";
    qStr += " FROM(";
    qStr += " SELECT Voucher_Id, Voucher_Prefix, Voucher_Date, ";
    qStr += " (case when Ledger_ID = '0x7x3' then Credit_Amount else 0 end) as 'Sales0GST',";
    qStr += " ( case when Ledger_ID = '0x7xSV5' then Credit_Amount else 0 end) as 'Sales5GST',";
    qStr += " ( case when Ledger_ID = '0x2x14xOCG25' then Credit_Amount else 0 end) as 'OPC25',";
    qStr += " ( case when Ledger_ID = '0x2x14xOSG25' then Credit_Amount else 0 end) as 'OPS25',";
    qStr += " ( case when Ledger_ID = '0x7xSV12' then Credit_Amount else 0 end) as 'Sales12GST',";
    qStr += " ( case when Ledger_ID = '0x2x14xOCG6' then Credit_Amount else 0 end) as 'OPC6',";
    qStr += " ( case when Ledger_ID = '0x2x14xOSG6' then Credit_Amount else 0 end) as 'OPS6',";
    qStr += " ( case when Ledger_ID = '0x7xSV18' then Credit_Amount else 0 end) as 'Sales18GST',";
    qStr += " ( case when Ledger_ID = '0x2x14xOCG9' then Credit_Amount else 0 end) as 'OPC9',";
    qStr += " ( case when Ledger_ID = '0x2x14xOSG9' then Credit_Amount else 0 end) as 'OPS9',";
    qStr += " ( case when Ledger_ID = '0x7xSV28' then Credit_Amount else 0 end) as 'Sales28GST',";
    qStr += " ( case when Ledger_ID = '0x2x14xOCG14' then Credit_Amount else 0 end) as 'OPC14',";
    qStr += " ( case when Ledger_ID = '0x2x14xOSG14' then Credit_Amount else 0 end) as 'OPS14',";
    qStr += " ( case when Ledger_ID = '0x2x14xCess' then Credit_Amount else 0 end) as 'Cess'";
    qStr += " FROM `transaction_helper` where voucher_type = 'SALESVOUCHER' ";
    qStr += " AND voucher_date >=" + db->getDateTimeString(fromDate);
    qStr += " AND voucher_date <=" + db->getDateTimeString(toDate);
    qStr += " ) trans ";
    qStr += " JOIN "+salesVoucherDatabaseHelper::Sales_Invoice_Main_TableName + " sales  ";
    qStr += " ON trans.voucher_id = sales.voucher_no";
    qStr += " and trans.voucher_prefix = sales.voucher_prefix";
    qStr += " group by trans.voucher_id, trans.voucher_prefix";
    qStr += " order by trans.Voucher_Date, cast(trans.voucher_id as int)";


    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}


QSqlQueryModel *LedgerHelper::getSalesReportModelByDate(QDateTime fromDate, QDateTime toDate)
{

    salesVoucherDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper itemHelper;
    QSqlQueryModel* model = new QSqlQueryModel;
    QString timestamp = "concat("+shelper.Sales_Invoice_Main_Voucher_Date+",' ',CAST("
            +shelper.Sales_Invoice_Main_TimeStamp+" AS TIME))";
    //    timestamp = shelper.Sales_Invoice_Main_TimeStamp;
    timestamp = "str_to_Date(concat(stable." + shelper.Sales_Invoice_Main_Voucher_Date ;
    timestamp += ", substr(" + shelper.Sales_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "";
    qStr +="SELECT ";
    qStr += "";
    qStr += shelper.Sales_Invoice_Main_Voucher_Date +" as Date, ";
    qStr += "cast("+shelper.Sales_Invoice_Main_Voucher_No +" as unsigned) as Voucher, ";
    qStr += "(SELECT "+ledHelper->Ledger_Master_LedgerName+" from "+ ledHelper->Ledger_Master_TableName;
    qStr += " where "+ledHelper->Ledger_Master_LedgerID + "=stable."+shelper.Sales_Invoice_Main_Ledger_ID;
    qStr += ") as Ledger,";
    qStr += shelper.Sales_Invoice_Main_Total+" as 'Total Amount'" +", ";
    qStr += shelper.Sales_Invoice_Main_Voucher_Prefix;
    qStr += " From ";
    qStr += shelper.Sales_Invoice_Main_TableName + " stable";
    qStr += " WHERE ";
    qStr += " ( ";
    qStr += timestamp + "<= " + db->getDateTimeString(toDate) ;
    qStr += " and ";
    qStr += timestamp+ " >= "+ db->getDateTimeString(fromDate);
    qStr += ")";
    qStr += " order BY cast(";
    qStr += shelper.Sales_Invoice_Main_Voucher_No +" as unsigned) desc ";

    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}



QSqlQueryModel *LedgerHelper::getDeletedSalesReportModelByDate(QDateTime fromDate, QDateTime toDate)
{

    salesVoucherDeletedDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper itemHelper;
    QSqlQueryModel* model = new QSqlQueryModel;
    QString timestamp = "concat("+shelper.Sales_Invoice_Main_Voucher_Date+",' ',CAST("
            +shelper.Sales_Invoice_Main_TimeStamp+" AS TIME))";
    //    timestamp = shelper.Sales_Invoice_Main_TimeStamp;
    timestamp = "str_to_Date(concat(stable." + shelper.Sales_Invoice_Main_Voucher_Date ;
    timestamp += ", substr(" + shelper.Sales_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "";
    qStr +="SELECT ";
    qStr += "";
    qStr += shelper.Sales_Invoice_Main_Voucher_Date +" as Date, "; //0
    qStr += "cast("+shelper.Sales_Invoice_Main_Voucher_No +" as unsigned) as Voucher, "; //1
    qStr += "(SELECT "+ledHelper->Ledger_Master_LedgerName+" from "+ ledHelper->Ledger_Master_TableName;
    qStr += " where "+ledHelper->Ledger_Master_LedgerID + "=stable."+shelper.Sales_Invoice_Main_Ledger_ID;
    qStr += ") as Ledger,"; //2
    qStr += shelper.Sales_Invoice_Main_Total+" as 'Total Amount'" +", "; //3
    qStr += shelper.Sales_Invoice_Main_Converted_To_Sales_Order + " as Reference, "; //4
    qStr += shelper.Sales_Invoice_Main_Voucher_Prefix; //5
    qStr += " From ";
    qStr += shelper.Sales_Invoice_Main_TableName + " stable";
    qStr += " WHERE ";
    qStr += " ( ";
    qStr += timestamp + "<= " + db->getDateTimeString(toDate) ;
    qStr += " and ";
    qStr += timestamp+ " >= "+ db->getDateTimeString(fromDate);
    qStr += ")";
    qStr += " order BY cast(";
    qStr += shelper.Sales_Invoice_Main_Voucher_No +" as unsigned) desc ";

    qDebug()<<qStr;

    //    query.prepare(qStr);

    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}

QSqlQueryModel *LedgerHelper::getPurchaseReportModelByDate(QDateTime fromDate, QDateTime toDate)
{
    PurchaseVoucherDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper itemHelper;

    QSqlQueryModel* model = new QSqlQueryModel;
    QString timestamp = "concat("+shelper.Purchase_Invoice_Main_Voucher_Date+",' ',CAST("
            +shelper.Purchase_Invoice_Main_TimeStamp+" AS TIME))";

    timestamp = shelper.Purchase_Invoice_Main_Voucher_Date;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "";
    qStr +="SELECT ";
    qStr += "";
    qStr += shelper.Purchase_Invoice_Main_Voucher_Date +" as Date, ";
    qStr += "cast("+shelper.Purchase_Invoice_Main_Voucher_No +" as unsigned) as Voucher, ";
    qStr += "(SELECT "+ledHelper->Ledger_Master_LedgerName+" from "+ ledHelper->Ledger_Master_TableName;
    qStr += " where "+ledHelper->Ledger_Master_LedgerID + "=stable."+shelper.Purchase_Invoice_Main_Ledger_ID;
    qStr += ") as Ledger, ";
    qStr += shelper.Purchase_Invoice_Main_Total+" as 'Total Amount'" +", ";
    qStr += " From ";
    qStr += shelper.Purchase_Invoice_Main_TableName + " stable";
    qStr += " WHERE ";
    qStr += " ( ";
    qStr += timestamp + ">='" + fromDate.toString("yyyy-MM-dd") + "'";
    qStr += " and ";
    qStr += timestamp + "<= '" + toDate.toString("yyyy-MM-dd") + "'";
    qStr += ")";

    qStr += " order BY ";
    qStr += shelper.Purchase_Invoice_Main_Voucher_No +" desc ";

    qDebug()<<qStr;
    //    query.prepare(qStr);
    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}

QSqlQueryModel *LedgerHelper::getPurchaseReportWithTaxModelByDate(QDateTime fromDate, QDateTime toDate)
{
    PurchaseVoucherDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper itemHelper;

    QSqlQueryModel* model = new QSqlQueryModel;
    QString timestamp = "concat("+shelper.Purchase_Invoice_Main_Voucher_Date+",' ',CAST("
            +shelper.Purchase_Invoice_Main_TimeStamp+" AS TIME))";

    timestamp = shelper.Purchase_Invoice_Main_Voucher_Date;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "";
    //    qStr +="SELECT ";
    //    qStr += "";
    //    qStr += shelper.Purchase_Invoice_Main_Voucher_Date +" as Date, ";
    //    qStr += "cast("+shelper.Purchase_Invoice_Main_Voucher_No +" as unsigned) as Voucher, ";
    //    qStr += "(SELECT "+ledHelper->Ledger_Master_LedgerName+" from "+ ledHelper->Ledger_Master_TableName;
    //    qStr += " where "+ledHelper->Ledger_Master_LedgerID + "=stable."+shelper.Purchase_Invoice_Main_Ledger_ID;
    //    qStr += ") as Ledger, ";
    //    qStr += shelper.Purchase_Invoice_Main_Gross_Total + " as 'Taxable Amount', ";
    //    qStr += shelper.Purchase_Invoice_Main_VATAmount + " as 'Tax Amount', ";
    //    qStr += shelper.Purchase_Invoice_Main_Total+" as 'Total Amount'" +" ";
    //    qStr += " From ";
    //    qStr += shelper.Purchase_Invoice_Main_TableName + " stable";
    //    qStr += " WHERE ";
    //    qStr += " ( ";
    //    qStr += timestamp + ">='" + fromDate.toString("yyyy-MM-dd") + "'";
    //    qStr += " and ";
    //    qStr += timestamp + "<= '" + toDate.toString("yyyy-MM-dd") + "'";
    //    qStr += ")";

    //    qStr += " order BY ";
    //    qStr += shelper.Purchase_Invoice_Main_Voucher_No +" desc ";

    qStr += " SELECT trans.Voucher_Date as 'Voucher Date', trans.Voucher_Id as 'Voucher No', trans.Voucher_Prefix as 'Voucher Prefix', ";
    qStr += " (select Ledger_name from " + LedgerMasterDatabaseHelper::Ledger_Master_TableName ;
    qStr += " where Ledger_ID = sales.Ledger_ID) as Ledger,";
    qStr += " (select TRN from " + LedgerMasterDatabaseHelper::Ledger_Master_TableName ;
    qStr += " where Ledger_ID = sales.Ledger_ID) as GSTIN,";
    qStr += " sales.gross_total as 'Taxable Amount', max(Sales0GST) as 'Purchase @ 0% GST',  ";
    qStr += " max(Sales5GST) as 'Purchase @ 5% GST', max(OPC25) as 'Input CGST @ 2.5%', max(OPS25) as 'Input SGST @ 2.5%',";
    qStr += " max(Sales12GST) as 'Purchase @ 12% GST', max(OPC6) as 'Input CGST @ 6%', max(OPS6) as 'Input SGST @ 6%',";
    qStr += " max(Sales18GST) as 'Purchase @ 18% GST', max(OPC9) as 'Input CGST @ 9%', max(OPS9) as 'Input SGST @ 9%',";
    qStr += " max(Sales28GST) as 'Purchase @ 28% GST', max(OPC14) as 'Input CGST @ 14%', max(OPS14) as 'Input SGST @ 14%',";
    //    qStr += " max(Cess) as 'Output Cess @ 1%', ";
    qStr += " sales.Vat_Amount as 'Total Tax', ";
    qStr += " sales.Total as Total";
    qStr += " FROM(";
    qStr += " SELECT Voucher_Id, Voucher_Prefix, Voucher_Date, ";
    qStr += " (case when Ledger_ID = '0x8x4' then Debit_Amount else 0 end) as 'Sales0GST',";
    qStr += " ( case when Ledger_ID = '0x8xPV5' then Debit_Amount else 0 end) as 'Sales5GST',";
    qStr += " ( case when Ledger_ID = '0x2x14xICG25' then Debit_Amount else 0 end) as 'OPC25',";
    qStr += " ( case when Ledger_ID = '0x2x14xISG25' then Debit_Amount else 0 end) as 'OPS25',";
    qStr += " ( case when Ledger_ID = '0x8xPV12' then Debit_Amount else 0 end) as 'Sales12GST',";
    qStr += " ( case when Ledger_ID = '0x2x14xICG6'  then Debit_Amount else 0 end) as 'OPC6',";
    qStr += " ( case when Ledger_ID = '0x2x14xISG6' then Debit_Amount else 0 end) as 'OPS6',";
    qStr += " ( case when Ledger_ID = '0x8xPV18' then Debit_Amount else 0 end) as 'Sales18GST',";
    qStr += " ( case when Ledger_ID = '0x2x14xICG9' then Debit_Amount else 0 end) as 'OPC9',";
    qStr += " ( case when Ledger_ID = '0x2x14xISG9' then Debit_Amount else 0 end) as 'OPS9',";
    qStr += " ( case when Ledger_ID = '0x8xPV28' then Debit_Amount else 0 end) as 'Sales28GST',";
    qStr += " ( case when Ledger_ID = '0x2x14xICG14' then Debit_Amount else 0 end) as 'OPC14',";
    qStr += " ( case when Ledger_ID = '0x2x14xISG14' then Debit_Amount else 0 end) as 'OPS14',";
    qStr += " ( case when Ledger_ID = '0x2x14xCess' then Debit_Amount else 0 end) as 'Cess'";
    qStr += " FROM `transaction_helper` where voucher_type = 'PURCHASEVOUCHER' ";
    qStr += " AND voucher_date >=" + db->getDateTimeString(fromDate);
    qStr += " AND voucher_date <=" + db->getDateTimeString(toDate);
    qStr += " ) trans ";
    qStr += " JOIN "+PurchaseVoucherDatabaseHelper::Purchase_Invoice_Main_TableName + " sales  ";
    qStr += " ON trans.voucher_id = sales.voucher_no";
    qStr += " and trans.voucher_prefix = sales.voucher_prefix";
    qStr += " group by trans.voucher_id, trans.voucher_prefix";
    qStr += " order by trans.Voucher_Date, cast(trans.voucher_id as int)";

    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}

QSqlQueryModel *LedgerHelper::getDebitNoteReportModelByDate(QDateTime fromDate, QDateTime toDate)
{
    DebitNoteVoucherDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper itemHelper;

    QSqlQueryModel* model = new QSqlQueryModel;
    QString timestamp = "concat("+shelper.Debit_Note_Main_Voucher_Date+",' ',CAST("
            +shelper.Debit_Note_Main_TimeStamp+" AS TIME))";

    timestamp = shelper.Debit_Note_Main_Voucher_Date;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "";
    qStr += " SELECT trans.Voucher_Date as 'Voucher Date', trans.Voucher_Id as 'Voucher No', trans.Voucher_Prefix as 'Voucher Prefix', ";
    qStr += " (select Ledger_name from " + LedgerMasterDatabaseHelper::Ledger_Master_TableName ;
    qStr += " where Ledger_ID = sales.Ledger_ID) as Ledger,";
    qStr += " (select TRN from " + LedgerMasterDatabaseHelper::Ledger_Master_TableName ;
    qStr += " where Ledger_ID = sales.Ledger_ID) as GSTIN,";
    qStr += " sales.gross_total as 'Taxable Amount', max(Sales0GST) as 'Purchase @ 0% GST',  ";
    qStr += " max(Sales5GST) as 'Purchase @ 5% GST', max(OPC25) as 'Input CGST @ 2.5%', max(OPS25) as 'Input SGST @ 2.5%',";
    qStr += " max(Sales12GST) as 'Purchase @ 12% GST', max(OPC6) as 'Input CGST @ 6%', max(OPS6) as 'Input SGST @ 6%',";
    qStr += " max(Sales18GST) as 'Purchase @ 18% GST', max(OPC9) as 'Input CGST @ 9%', max(OPS9) as 'Input SGST @ 9%',";
    qStr += " max(Sales28GST) as 'Purchase @ 28% GST', max(OPC14) as 'Input CGST @ 14%', max(OPS14) as 'Input SGST @ 14%',";
    //    qStr += " max(Cess) as 'Output Cess @ 1%', ";
    qStr += " sales.Vat_Amount as 'Total Tax', ";
    qStr += " sales.Total as Total";
    qStr += " FROM(";
    qStr += " SELECT Voucher_Id, Voucher_Prefix, Voucher_Date, ";
    qStr += " (case when Ledger_ID = '0x8x4' then Debit_Amount else 0 end) as 'Sales0GST',";
    qStr += " ( case when Ledger_ID = '0x8xPV5' then Debit_Amount else 0 end) as 'Sales5GST',";
    qStr += " ( case when Ledger_ID = '0x2x14xICG25' then Debit_Amount else 0 end) as 'OPC25',";
    qStr += " ( case when Ledger_ID = '0x2x14xISG25' then Debit_Amount else 0 end) as 'OPS25',";
    qStr += " ( case when Ledger_ID = '0x8xPV12' then Debit_Amount else 0 end) as 'Sales12GST',";
    qStr += " ( case when Ledger_ID = '0x2x14xICG6'  then Debit_Amount else 0 end) as 'OPC6',";
    qStr += " ( case when Ledger_ID = '0x2x14xISG6' then Debit_Amount else 0 end) as 'OPS6',";
    qStr += " ( case when Ledger_ID = '0x8xPV18' then Debit_Amount else 0 end) as 'Sales18GST',";
    qStr += " ( case when Ledger_ID = '0x2x14xICG9' then Debit_Amount else 0 end) as 'OPC9',";
    qStr += " ( case when Ledger_ID = '0x2x14xISG9' then Debit_Amount else 0 end) as 'OPS9',";
    qStr += " ( case when Ledger_ID = '0x8xPV28' then Debit_Amount else 0 end) as 'Sales28GST',";
    qStr += " ( case when Ledger_ID = '0x2x14xICG14' then Debit_Amount else 0 end) as 'OPC14',";
    qStr += " ( case when Ledger_ID = '0x2x14xISG14' then Debit_Amount else 0 end) as 'OPS14',";
    qStr += " ( case when Ledger_ID = '0x2x14xCess' then Debit_Amount else 0 end) as 'Cess'";
    qStr += " FROM `transaction_helper` where voucher_type = 'DEBITNOTE' ";
    qStr += " AND voucher_date >=" + db->getDateTimeString(fromDate);
    qStr += " AND voucher_date <=" + db->getDateTimeString(toDate);
    qStr += " ) trans ";
    qStr += " JOIN "+DebitNoteVoucherDatabaseHelper::Debit_Note_Main_TableName + " sales  ";
    qStr += " ON trans.voucher_id = sales.voucher_no";
    qStr += " and trans.voucher_prefix = sales.voucher_prefix";
    qStr += " group by trans.voucher_id, trans.voucher_prefix";
    qStr += " order by trans.Voucher_Date, cast(trans.voucher_id as int)";
    qDebug()<<qStr;
    //    query.prepare(qStr);
    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}

QSqlQueryModel *LedgerHelper::getCreditNoteReportModelByDate(QDateTime fromDate, QDateTime toDate)
{
    CreditNoteVoucherDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper itemHelper;

    QSqlQueryModel* model = new QSqlQueryModel;
    QString timestamp = "concat("+shelper.Credit_Note_Main_Voucher_Date+",' ',CAST("
            +shelper.Credit_Note_Main_TimeStamp+" AS TIME))";

    timestamp = shelper.Credit_Note_Main_Voucher_Date;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "";
    qStr += " SELECT trans.Voucher_Date as'Voucher Date', trans.Voucher_Id as 'Voucher No', trans.Voucher_Prefix as 'Voucher Prefix', ";
    qStr += " led.Ledger_name as Ledger, TRN as GSTIN, ";
    qStr += " sales.gross_total as 'Taxable Amount', max(Sales0GST) as 'Sales @ 0% GST',  ";
    qStr += " max(Sales5GST) as 'Sales @ 5% GST', max(OPC25) as 'Output CGST @ 2.5%', max(OPS25) as 'Output SGST @ 2.5%',";
    qStr += " max(Sales12GST) as 'Sales @ 12% GST', max(OPC6) as 'Output CGST @ 6%', max(OPS6) as 'Output SGST @ 6%',";
    qStr += " max(Sales18GST) as 'Sales @ 18% GST', max(OPC9) as 'Output CGST @ 9%', max(OPS9) as 'Output SGST @ 9%',";
    qStr += " max(Sales28GST) as 'Sales @ 28% GST', max(OPC14) as 'Output CGST @ 14%', max(OPS14) as 'Output SGST @ 14%',";
    qStr += " max(Cess) as 'Output Cess @ 1%', ";
    qStr += " sales.Vat_Amount as 'Total Tax', ";
    qStr += " sales.Total as Total";
    qStr += " FROM(";
    qStr += " SELECT Voucher_Id, Voucher_Prefix, Voucher_Date, ";
    qStr += " (case when Ledger_ID = '0x7x3' then Credit_Amount else 0 end) as 'Sales0GST',";
    qStr += " ( case when Ledger_ID = '0x7xSV5' then Credit_Amount else 0 end) as 'Sales5GST',";
    qStr += " ( case when Ledger_ID = '0x2x14xOCG25' then Credit_Amount else 0 end) as 'OPC25',";
    qStr += " ( case when Ledger_ID = '0x2x14xOSG25' then Credit_Amount else 0 end) as 'OPS25',";
    qStr += " ( case when Ledger_ID = '0x7xSV12' then Credit_Amount else 0 end) as 'Sales12GST',";
    qStr += " ( case when Ledger_ID = '0x2x14xOCG6' then Credit_Amount else 0 end) as 'OPC6',";
    qStr += " ( case when Ledger_ID = '0x2x14xOSG6' then Credit_Amount else 0 end) as 'OPS6',";
    qStr += " ( case when Ledger_ID = '0x7xSV18' then Credit_Amount else 0 end) as 'Sales18GST',";
    qStr += " ( case when Ledger_ID = '0x2x14xOCG9' then Credit_Amount else 0 end) as 'OPC9',";
    qStr += " ( case when Ledger_ID = '0x2x14xOSG9' then Credit_Amount else 0 end) as 'OPS9',";
    qStr += " ( case when Ledger_ID = '0x7xSV28' then Credit_Amount else 0 end) as 'Sales28GST',";
    qStr += " ( case when Ledger_ID = '0x2x14xOCG14' then Credit_Amount else 0 end) as 'OPC14',";
    qStr += " ( case when Ledger_ID = '0x2x14xOSG14' then Credit_Amount else 0 end) as 'OPS14',";
    qStr += " ( case when Ledger_ID = '0x2x14xCess' then Credit_Amount else 0 end) as 'Cess'";
    qStr += " FROM `transaction_helper` where voucher_type = 'SALESVOUCHER' ";
    qStr += " AND voucher_date >=" + db->getDateTimeString(fromDate);
    qStr += " AND voucher_date <=" + db->getDateTimeString(toDate);
    qStr += " ) trans ";
    qStr += " JOIN "+CreditNoteVoucherDatabaseHelper::Credit_Note_Main_TableName + " sales  ";
    qStr += " ON trans.voucher_id = sales.voucher_no";
    qStr += " and trans.voucher_prefix = sales.voucher_prefix";
    qStr += " JOIN " + LedgerMasterDatabaseHelper::Ledger_Master_TableName + " led ";
    qStr += " ON sales.ledger_id = led.ledger_id ";
    qStr += " group by trans.voucher_id, trans.voucher_prefix";
    qStr += " order by trans.Voucher_Date, cast(trans.voucher_id as int)";

    qDebug()<<qStr;
    //    query.prepare(qStr);
    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}

QSqlQueryModel *LedgerHelper::getDayBookReportModel(QDate fromDate, QDate toDate)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    TransactionHelper *t= new TransactionHelper();
    QString qStr="SELECT ";

    qStr += "(SELECT ledger_name FROM " + ledHelper->Ledger_Master_TableName + " where ledger_id = subq."+t->Transaction_Helper_LedgerID+") AS LEDGER ";
    qStr += ", subq."+t->Transaction_Helper_VoucherDate;
    qStr += ", subq."+t->Transaction_Helper_VoucherNo;
    qStr += ", subq."+t->Transaction_Helper_VoucherType;
    qStr += ", (case when subq.deb > 0 then subq.deb else 0 end) as Debit";
    qStr += ", (case when subq.cred > 0 then subq.cred else 0 end) as Credit FROM ";
    qStr += "( SELECT ";

    qStr += t->Transaction_Helper_LedgerID ;
    qStr += ", "+ t->Transaction_Helper_Credit_Amount +"-"+t->Transaction_Helper_Debit_Amount+" as cred";
    qStr += ", "+ t->Transaction_Helper_Debit_Amount +"-"+t->Transaction_Helper_Credit_Amount+" as deb";
    qStr += ", "+t->Transaction_Helper_VoucherDate;
    qStr += ", "+t->Transaction_Helper_VoucherNo;
    qStr += ", "+t->Transaction_Helper_VoucherType;
    qStr += ", MIN("+t->Transaction_Helper_id+")";
    qStr += " From "+t->Transaction_Helper_TableName;
    qStr += " Where ";
    //    qStr += t->Transaction_Helper_LedgerID +  "='"+ledgerID +"'";
    //    qStr += " AND ";
    qStr += t->Transaction_Helper_VoucherDate +" >='"+fromDate.toString("yyyy-MM-dd") +"'";
    qStr += " AND ";
    qStr += t->Transaction_Helper_VoucherDate +" <='"+toDate.toString("yyyy-MM-dd") +"'";
    qStr += " GROUP BY ";
    qStr += t->Transaction_Helper_VoucherNo +", "+t->Transaction_Helper_VoucherType;
    qStr += " ORDER BY ";
    qStr += t->Transaction_Helper_VoucherDate;
    qStr += ", ";
    qStr += "Cast("+t->Transaction_Helper_VoucherNo+" as unsigned)" +", "+t->Transaction_Helper_VoucherType ;
    qStr += " ) subq";


    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}

QSqlQueryModel *LedgerHelper::getComplimentarySales(QDateTime fromDate, QDateTime toDate)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    TransactionHelper *t= new TransactionHelper();
    salesVoucherDatabaseHelper shelper;
    QString timestamp = "concat("+shelper.Sales_Invoice_Main_Voucher_Date+",' ',CAST("
            +shelper.Sales_Invoice_Main_TimeStamp+" AS TIME))";
    timestamp = shelper.Sales_Invoice_Main_TimeStamp;

    timestamp = "str_to_Date(concat(main." + shelper.Sales_Invoice_Main_Voucher_Date ;
    timestamp += ", substr(" + shelper.Sales_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;

    qDebug()<<"time***********"<<fromDate<<toDate;
    QString qStr="SELECT ";

    qStr += "sa." + shelper.Sales_Invoice_Details_Voucher_No +" as Voucher, ";
    qStr += "sa." + shelper.Sales_Invoice_Details_Voucher_Date +" as Date, ";
    qStr += "Cast(SUM(sa."+shelper.Sales_Invoice_Details_Quantity+"*sa."+shelper.Sales_Invoice_Details_Price + ") as DECIMAL(10,2)) as Amount";
    qStr += " ,  Reference FROM ";
    qStr += shelper.Sales_Invoice_Details_TableName;
    qStr += " sa JOIN " + shelper.Sales_Invoice_Main_TableName + " main " ;
    qStr += " on sa.voucher_no = main.voucher_no and sa.voucher_prefix = main.voucher_prefix ";
    qStr += " where main."+ shelper.Sales_Invoice_Main_Total +" =0 ";
    qStr += " and ";
    qStr += " ( ";
    qStr += timestamp + ">=" + db->getDateTimeString(fromDate);
    qStr += " and ";
    qStr += timestamp + "<= " + db->getDateTimeString(toDate);
    qStr += ")";
    qStr += " GROUP BY sa."+shelper.Sales_Invoice_Details_Voucher_No + ", sa." + shelper.Sales_Invoice_Details_Voucher_Prefix;
    qStr += " Order BY sa."+shelper.Sales_Invoice_Details_Voucher_No + ", sa." + shelper.Sales_Invoice_Details_Voucher_Prefix;


    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}

QSqlQueryModel *LedgerHelper::getSalesManwiseReport(QDateTime dateFrom, QDateTime dateTo)
{

    QSqlQueryModel *model = new QSqlQueryModel();
    salesVoucherDatabaseHelper shelper;
    UserProfileDatabaseHelper uhelper;

    QString timestamp = "concat("+shelper.Sales_Invoice_Main_Voucher_Date+",' ',CAST("
            +shelper.Sales_Invoice_Main_TimeStamp+" AS TIME))";
    timestamp = shelper.Sales_Invoice_Main_TimeStamp;

    timestamp = "str_to_Date(concat(sal." + shelper.Sales_Invoice_Main_Voucher_Date ;
    timestamp += ", substr(" + shelper.Sales_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;


    QString qStr=  "SELECT (";

    qStr += "SELECT "+uhelper.Employee_Details_Name+ " FROM "+ uhelper.Employee_Details_TableName;
    qStr += " WHERE "+ uhelper.Employee_Details_id +  "= sal."+shelper.Sales_Invoice_Main_Salesman_ID;
    qStr += ") Name,";

    qStr += "COUNT("+shelper.Sales_Invoice_Main_Voucher_No+") Count,";
    qStr += "SUM("+shelper.Sales_Invoice_Main_Total+") Amount ";

    qStr += " FROM "+ shelper.Sales_Invoice_Main_TableName +" sal ";

    qStr += " WHERE ";

    qStr += " ( ";
    qStr += timestamp + ">=" + db->getDateTimeString(dateFrom);
    qStr += " and ";
    qStr += timestamp + "<= " + db->getDateTimeString(dateTo);
    qStr += ")";
    qStr += " group by "+shelper.Sales_Invoice_Main_Salesman_ID;
    qStr += " order by 1";

    qDebug()<<qStr;

    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}

QSqlQueryModel *LedgerHelper::getSalesManwiseReportByService(QDateTime dateFrom, QDateTime dateTo,
                                                             int service)
{

    QSqlQueryModel *model = new QSqlQueryModel();
    salesVoucherDatabaseHelper shelper;
    UserProfileDatabaseHelper uhelper;


    QString timestamp = "concat("+shelper.Sales_Invoice_Main_Voucher_Date+",' ',CAST("
            +shelper.Sales_Invoice_Main_TimeStamp+" AS TIME))";
    timestamp = shelper.Sales_Invoice_Main_TimeStamp;

    timestamp = "str_to_Date(concat(sal." + shelper.Sales_Invoice_Main_Voucher_Date ;
    timestamp += ", substr(" + shelper.Sales_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;


    QString qStr=  "SELECT (";

    qStr += "SELECT "+uhelper.Employee_Details_Name+ " FROM "+ uhelper.Employee_Details_TableName;
    qStr += " WHERE "+ uhelper.Employee_Details_id +  "= sal."+shelper.Sales_Invoice_Main_Salesman_ID;
    qStr += ") Name,";

    qStr += "COUNT("+shelper.Sales_Invoice_Main_Voucher_No+") Count,";
    qStr += "SUM("+shelper.Sales_Invoice_Main_Total+") Amount ";

    qStr += " FROM "+ shelper.Sales_Invoice_Main_TableName +" sal ";


    qStr += " WHERE ";
    qStr += " ( ";
    qStr += timestamp + ">=" + db->getDateTimeString(dateFrom);
    qStr += " and ";
    qStr += timestamp + "<= "  + db->getDateTimeString(dateTo);
    qStr += " and ";
    qStr += shelper.Sales_Invoice_Main_ModeOfService + " = "+ QString::number(service);
    qStr += ")";
    qStr += " group by "+shelper.Sales_Invoice_Main_Salesman_ID;

    //    qDebug()<<qStr;

    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}

QSqlQueryModel *LedgerHelper::getSalesManwiseServicewiseReport(QDateTime dateFrom, QDateTime dateTo)
{

    QSqlQueryModel *model = new QSqlQueryModel();
    salesVoucherDatabaseHelper shelper;
    UserProfileDatabaseHelper uhelper;
    PricelistDatabaseHelper priceHelper;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    QList<PriceListDataObject> prices = priceHelper.getAllPriceLists();


    QString timestamp = "concat("+shelper.Sales_Invoice_Main_Voucher_Date+",' ',CAST("
            +shelper.Sales_Invoice_Main_TimeStamp+" AS TIME))";
    timestamp = shelper.Sales_Invoice_Main_TimeStamp;

    timestamp = "str_to_Date(concat(sal." + shelper.Sales_Invoice_Main_Voucher_Date ;
    timestamp += ", substr(" + shelper.Sales_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;

    QString qStr = "";

    qStr += "  select ";
    qStr += " Name, ";
    int j=0;
    QString total ;
    for(PriceListDataObject price:prices)
    {
        qStr += "max(BillCount_" + QString::number(j) + ") as ' " + price.priceListName + " ', ";
//        qStr += "max(Amount_" + QString::number(j) + ") as '" + price.priceListName + " ', ";

        total +=  " coalesce(max(Amount_" + QString::number(j) + "),0) +";
        j++;
    }

//    qStr.truncate(qStr.lastIndexOf(","));
    total.truncate(total.lastIndexOf("+"));
    qStr += total + " as 'Total Amount' ";
    qStr += " from (";

    qStr +=  "SELECT (";

    qStr += "SELECT "+uhelper.Employee_Details_Name+ " FROM "+ uhelper.Employee_Details_TableName;
    qStr += " WHERE "+ uhelper.Employee_Details_id +  "= sal."+shelper.Sales_Invoice_Main_Salesman_ID;
    qStr += ") Name,";
    int i=0;
    for(PriceListDataObject price:prices)
    {
        qStr += "(case when sal.Mode_Of_Service = '" + QString::number(price.priceListID) ;
        qStr += "' then COUNT(*) end) as 'BillCount_" + QString::number(i) + "',";
        qStr += "(case when sal.Mode_Of_Service = '" + QString::number(price.priceListID) ;
        qStr += "' then SUM(coalesce("+shelper.Sales_Invoice_Main_Total+",0)) end) as 'Amount_" + QString::number(i) + "',";
        i++;
    }

    qStr.truncate(qStr.lastIndexOf(","));

    qStr += " FROM "+ shelper.Sales_Invoice_Main_TableName +" sal ";


    qStr += " WHERE ";
    qStr += " ( ";
    qStr += timestamp + ">=" + db->getDateTimeString(dateFrom);
    qStr += " and ";
    qStr += timestamp + "<= "  + db->getDateTimeString(dateTo);

    qStr += ")";
    qStr += " group by "+shelper.Sales_Invoice_Main_Salesman_ID + " , " + shelper.Sales_Invoice_Main_ModeOfService;
    qStr += " ) X group by Name ";

    //    qDebug()<<qStr;


    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}



QSqlQueryModel *LedgerHelper::getDiscountedSales(QDateTime fromDate, QDateTime toDate)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    TransactionHelper *t= new TransactionHelper();
    salesVoucherDatabaseHelper shelper;

    QString timestamp = "concat("+shelper.Sales_Invoice_Main_Voucher_Date+",' ',CAST("
            +shelper.Sales_Invoice_Main_TimeStamp+" AS TIME))";
    timestamp = shelper.Sales_Invoice_Main_TimeStamp;

    timestamp = "str_to_Date(concat(stable." + shelper.Sales_Invoice_Main_Voucher_Date ;
    timestamp += ", substr(" + shelper.Sales_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;

    QString qStr="SELECT ";

    qStr += shelper.Sales_Invoice_Main_Voucher_No +" as Voucher, ";
    qStr += shelper.Sales_Invoice_Main_Voucher_Date +" as Date, ";
    qStr += shelper.Sales_Invoice_Main_Reference +" as Reference, ";
    qStr  += "cast(SUM("+shelper.Sales_Invoice_Main_Discount_In_Amount + ") as Decimal(10,2)) as Discount";
    qStr += " FROM ";
    qStr += shelper.Sales_Invoice_Main_TableName + " stable ";
    qStr += " where "+ shelper.Sales_Invoice_Main_Discount_In_Amount +" >0" ;
    qStr += " and ";
    qStr += " ( ";

    qStr += timestamp + "<= " + db->getDateTimeString(toDate);
    qStr += " and ";
    qStr += timestamp + ">= "+db->getDateTimeString(fromDate);
    qStr += ")";
    qStr += "" ;
    qStr += " GROUP BY "+shelper.Sales_Invoice_Details_Voucher_No;
    qStr += " Order BY "+shelper.Sales_Invoice_Details_Voucher_No;



    qDebug()<<qStr;

    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}

QSqlQueryModel *LedgerHelper::getItemwiseMovementByLedger(QString ledgerID, QDate fromDate, QDate toDate)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    TransactionHelper *t= new TransactionHelper();
    salesVoucherDatabaseHelper shelper;

    SalesInventoryItemDatabaseHelper itemHelper;

    QString qStr ="SELECT (SELECT ITEM_NAME FROM " + itemHelper.Sales_Inventory_Table_Name + " WHERE ITEM_ID = TRANS.ITEMID) AS ITEM,";
    qStr += " COALESCE(SUM(CR_QTY),0) AS CREDIT, COALESCE(SUM(DR_QTY),0) AS DEBIT ";
    qStr += " FROM TRANSACTION_ITEM_HELPER TRANS";
    qStr += " WHERE FROM_LEDGER = '"+ ledgerID + "' OR TO_LEDGER = '" + ledgerID + "'";
    qStr += " AND TRANS.VOUCHERDATE >= '" + fromDate.toString("yyyy-MM-dd")+ "'";
    qStr += " AND TRANS.VOUCHERDATE <= '" + toDate.toString("yyyy-MM-dd")+ "'";
    qStr += " GROUP BY ITEMID";

    qDebug()<<qStr;

    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}

QSqlQueryModel *LedgerHelper::getItemGroupMovementByLedger(QString ledgerID, QDate fromDate, QDate toDate)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    TransactionHelper *t= new TransactionHelper();
    salesVoucherDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper itemHelper;

    QString qStr ="SELECT (SELECT GROUP_NAME FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +"  WHERE GROUP_ID = INV.GROUP_ID) AS GROUP, ";
    qStr += " COALESCE(SUM(CR_QTY),0) AS CREDIT, COALESCE(SUM(DR_QTY),0) AS DEBIT ";
    qStr += " FROM TRANSACTION_ITEM_HELPER TRANS";
    qStr += " INNER JOIN " + itemHelper.Sales_Inventory_Table_Name + " INV ON TRANS.ItemID = INV.Item_ID";
    qStr += " WHERE (FROM_LEDGER = '"+ ledgerID + "' OR TO_LEDGER = '" + ledgerID + "')";
    qStr += " AND TRANS.VOUCHERDATE >= '" + fromDate.toString("yyyy-MM-dd")+ "'";
    qStr += " AND TRANS.VOUCHERDATE <= '" + toDate.toString("yyyy-MM-dd")+ "'";
    qStr += " GROUP BY INV.GROUP_ID";

    qDebug()<<qStr;

    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}

QSqlQueryModel *LedgerHelper::getTimeBasedSales(QDateTime fromDate, QDateTime toDate)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    TransactionHelper *t= new TransactionHelper();
    salesVoucherDatabaseHelper shelper;

    QString    timestamp = "str_to_Date(concat(stable." + shelper.Sales_Invoice_Main_Voucher_Date ;
    timestamp += ", substr(" + shelper.Sales_Invoice_Main_TimeStamp + ", 11,19)) ,'%Y-%m-%d %H:%i:%s')" ;


    QString qStr="SELECT ";
    qStr  += "CONCAT('Hour : ',HOUR("+shelper.Sales_Invoice_Main_TimeStamp+")) as Hour, ";
    qStr  += "SUM("+shelper.Sales_Invoice_Main_Total+") as Amount, ";
    qStr  += "Count("+shelper.Sales_Invoice_Main_Voucher_No+") as Count ";

    qStr += " FROM ";
    qStr += shelper.Sales_Invoice_Main_TableName;
    qStr += " Where ";
    qStr += shelper.Sales_Invoice_Details_Voucher_No +" in (" ;
    qStr += "SELECT "+shelper.Sales_Invoice_Main_Voucher_No ;
    qStr += " from ";
    qStr += shelper.Sales_Invoice_Main_TableName + " stable ";


    qStr += " WHERE ";
    qStr += timestamp + " >=" + db->getDateTimeString(fromDate);
    qStr += " and ";
    qStr += timestamp + " <="+ db->getDateTimeString(toDate);
    qStr += ")";

    qStr += " GROUP BY HOUR("+shelper.Sales_Invoice_Main_TimeStamp + ")";
    qStr += " ORDER BY HOUR("+shelper.Sales_Invoice_Main_TimeStamp + ")";

    qDebug()<<qStr;

    model = DatabaseHelper::getQueryModel(qStr);

    return model;
}

QSqlQueryModel *LedgerHelper::getServiceBasedSalesModel(QDateTime dateFrom, QDateTime dateTo)
{
    QString qStr;

    salesVoucherDatabaseHelper shelper;
    PricelistDatabaseHelper pr;
    QSqlQueryModel *model = new QSqlQueryModel();
    QString timestamp = "concat("+shelper.Sales_Invoice_Main_Voucher_Date+",' ',CAST("
            +shelper.Sales_Invoice_Main_TimeStamp+" AS TIME))";
    timestamp = shelper.Sales_Invoice_Main_TimeStamp;
    qStr = "SELECT ";
    qStr += "(SELECT "+pr.PriceListName + " from "+pr.PriceLists_TableName+" where "+pr.PriceListID+"=sim."+shelper.Sales_Invoice_Main_ModeOfService +")";
    qStr +=  "as 'Mode', ";
    qStr += " SUM("+shelper.Sales_Invoice_Main_Total+") as 'Total' from ";
    qStr += shelper.Sales_Invoice_Main_TableName +" sim";
    qStr += " where ";
    qStr += " ( ";
    qStr += timestamp + ">='" + dateFrom.toString("yyyy-MM-dd hh:mm:ss") + "'";
    qStr += " and ";
    qStr += timestamp + "<='" +   dateTo.toString("yyyy-MM-dd hh:mm:ss") + "'";
    qStr += ")";
    qStr += " group by "+shelper.Sales_Invoice_Main_ModeOfService;
    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}

QSqlQueryModel *LedgerHelper::getPendingVouchersByVoucherType(int Reftype, QDate fromDate, QDate toDate, int limit)
{
    qDebug()<<Q_FUNC_INFO;
    // same can be used for payment & receipt pending
    QString tableName = GMVoucherTypes::getVoucherMainTablename(Reftype);

    QString qStr = "";
    qStr += " SELECT RefNo VoucherNo, RefPrefix as VoucherPrefix, ";
    qStr += " RefType as VoucherType, min(VoucherDate) as VoucherDate ,";
    qStr += "  SUM(DebitAmount) -  SUM(CreditAmount) as 'Total Value' FROM Billwise_Records Ref ";
    qStr += " WHERE RefType = 'PurchaseVoucher' AND VoucherDate <= str_to_date('2021-02-13','%Y-%m-%d') ";
    qStr += " AND Ref.VoucherDate >= " + db->getDateString(fromDate);
    qStr += " AND Ref.VoucherDate <= " + db->getDateString(toDate) ;
    qStr += " GROUP BY RefNo, RefType, RefPrefix  HAVING SUM(CreditAmount) - SUM(DebitAmount) <>0 ";

    qStr += "  LIMIT " + QString::number(limit);
    return DatabaseHelper::getQueryModel(qStr);

}


QSqlQueryModel *LedgerHelper::getShiftModel(QDateTime dateFrom, QDateTime dateTo)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString timestamp = "`ShiftDate`";
    QString qStr = "SELECT `_id`,`ShiftDate` 'Date', `StartVoucherNo` 'From' , `StopVoucherNo` 'To',";
    qStr += " voucherPrefix as 'Prefix', ";
    qStr += " `SalesValue` Sales,  `CashSales` as 'Cash Sales', `CardSales` Card, `CreditSales` Credit,";
    qStr += "`PaymentCash`,";
    qStr += " (SELECT name from  " +UserProfileDatabaseHelper::Employee_Details_TableName +"  where _id = shift.cashierID) as Cashier,";
    qStr += "`paxCount` PAX , `ShiftStart_DateTime` as  Start , `ShiftEnd_DateTime` as END,";
    qStr += "`TotalCash`, `CasinCounter`, `TillDifference`, ";
    qStr += "`Narration`,CashWithDrawn , CashBalance as 'Closing Cash' ";
    qStr += " FROM `shifttable` shift ";
    qStr += " where ";
    qStr += " ( ";
    qStr += timestamp + ">='" + dateFrom.toString("yyyy-MM-dd hh:mm:ss") + "'";
    qStr += " and ";
    qStr += timestamp + "<= '" + dateTo.toString("yyyy-MM-dd hh:mm:ss") + "'";
    qStr += ")";

    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}

QSqlQueryModel *LedgerHelper::getItemwiseMovementByAccGroup(QString groupID, QDate fromDate, QDate toDate)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    TransactionHelper *t= new TransactionHelper();
    salesVoucherDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper itemHelper;

    QString qStr = "SELECT (SELECT ITEM_NAME FROM " + itemHelper.Sales_Inventory_Table_Name + " WHERE ITEM_ID = TRANS.ITEMID) AS ITEM, ";
    qStr += "COALESCE(SUM(TRANS.CR_QTY),0) , COALESCE(SUM(TRANS.DR_QTY),0) " ;
    qStr += " FROM " + ledHelper->Ledger_Master_TableName + " LED INNER JOIN " ;
    qStr += " (SELECT  GROUP_ID, GROUP_NAME, PARENT_ID " ;
    qStr += " FROM    (SELECT * FROM " + accHelper->Accounts_Group_TableName + " ORDER BY PARENT_ID, GROUP_ID) GROUPS_SORTED, ";
    qStr += " (SELECT @PV := '" + groupID + "') INITIALISATION WHERE FIND_IN_SET(PARENT_ID, @PV) ";
    qStr += " AND     LENGTH(@PV := CONCAT(@PV, ',', GROUP_ID))) AS ACC_GROUP ";
    qStr += " ON LED.GROUP_ID = ACC_GROUP.GROUP_ID LEFT OUTER JOIN TRANSACTION_ITEM_HELPER TRANS";
    qStr += " ON LED.LEDGER_ID = TRANS.TO_LEDGER OR LED.LEDGER_ID = TRANS.FROM_LEDGER ";
    qStr += " WHERE TRANS.VOUCHERDATE >= '" + fromDate.toString("yyyy-MM-dd") + "' AND TRANS.VOUCHERDATE <= '" + toDate.toString("yyyy-MM-dd") + "'";
    qStr += " GROUP BY TRANS.ITEMID ";


    qDebug()<<qStr;

    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}

QSqlQueryModel *LedgerHelper::getItemGroupMovementByAccountGroup(QString groupID, QDate fromDate, QDate toDate)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    TransactionHelper *t= new TransactionHelper();
    salesVoucherDatabaseHelper shelper;
    SalesInventoryItemDatabaseHelper itemHelper;

    QString qStr = "SELECT (SELECT GROUP_NAME FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +"  WHERE GROUP_ID = INV.GROUP_ID) AS GROUP, ";
    qStr += "COALESCE(SUM(TRANS.CR_QTY),0) , COALESCE(SUM(TRANS.DR_QTY),0) " ;
    qStr += " FROM " + ledHelper->Ledger_Master_TableName + " LED INNER JOIN " ;
    qStr += " (SELECT  GROUP_ID, GROUP_NAME, PARENT_ID " ;
    qStr += " FROM    (SELECT * FROM " + accHelper->Accounts_Group_TableName + " ORDER BY PARENT_ID, GROUP_ID) GROUPS_SORTED, ";
    qStr += " (SELECT @PV := '" + groupID + "') INITIALISATION WHERE FIND_IN_SET(PARENT_ID, @PV) ";
    qStr += " AND     LENGTH(@PV := CONCAT(@PV, ',', GROUP_ID))) AS ACC_GROUP ";
    qStr += " ON LED.GROUP_ID = ACC_GROUP.GROUP_ID LEFT OUTER JOIN TRANSACTION_ITEM_HELPER TRANS";
    qStr += " ON LED.LEDGER_ID = TRANS.TO_LEDGER OR LED.LEDGER_ID = TRANS.FROM_LEDGER ";
    qStr += " INNER JOIN " + itemHelper.Sales_Inventory_Table_Name + " INV ON TRANS.ItemID = INV.Item_ID";
    qStr += " WHERE TRANS.VOUCHERDATE >= '" + fromDate.toString("yyyy-MM-dd") + "' AND TRANS.VOUCHERDATE <= '" + toDate.toString("yyyy-MM-dd") + "'";
    qStr += " GROUP BY INV.GROUP_ID ";


    qDebug()<<qStr;

    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}

QSqlQueryModel *LedgerHelper::getOutstandingBalanceByGroup(QString groupID, QDate fromDate, QDate toDate)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    TransactionHelper *t= new TransactionHelper();
    salesVoucherDatabaseHelper shelper;

    //Ledgers within group

    QString qStr;
    qStr += " SELECT LEDGER_NAME as Ledger, (CASE WHEN CLOSING < 0 THEN -1*CLOSING  ELSE NULL END) DEBIT,  ";
    qStr += " (CASE WHEN CLOSING > 0 THEN CLOSING  ELSE NULL END) CREDIT FROM ";
    qStr += " (SELECT LED.LEDGER_NAME, LED.LEDGER_ID,  ";
//    qStr += " CAST(COALESCE(LED.OPENING_BALANCE,0)+ "; do not uncommnt
    qStr += " CAST(COALESCE(SUM(TRANS.CREDIT_AMOUNT),0) - COALESCE(SUM(TRANS.DEBIT_AMOUNT),0) AS DECIMAL(10,2)) AS CLOSING ";
    qStr += " FROM " + ledHelper->Ledger_Master_TableName + " LED ";
    qStr += " INNER JOIN  ";
    qStr += " (SELECT  GROUP_ID, ";
    qStr += "         GROUP_NAME, ";
    qStr += "         PARENT_ID  ";
    qStr += " FROM    (SELECT * FROM " + accHelper->Accounts_Group_TableName + " ";
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
    qStr += " WHERE CLOSING <>0";





    qDebug()<<qStr;

    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}

QSqlQueryModel *LedgerHelper::getDiscountedSalesForMail(QDateTime fromDate, QDateTime toDate)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    TransactionHelper *t= new TransactionHelper();
    salesVoucherDatabaseHelper shelper;

    QString timestamp = "concat("+shelper.Sales_Invoice_Main_Voucher_Date+",' ',CAST("
            +shelper.Sales_Invoice_Main_TimeStamp+" AS TIME))";
    timestamp = shelper.Sales_Invoice_Main_TimeStamp;

    QString qStr="SELECT ";

    qStr += shelper.Sales_Invoice_Main_Voucher_No +" as Voucher, ";
    qStr += shelper.Sales_Invoice_Main_Voucher_Date +" as VDate, ";
    qStr += shelper.Sales_Invoice_Main_Narration +" as Reference, ";
    qStr  += "cast(SUM("+shelper.Sales_Invoice_Main_Discount_In_Amount + ") as Decimal(10,2)) as Discount";
    qStr += " FROM ";
    qStr += shelper.Sales_Invoice_Main_TableName;
    qStr += " where "+ shelper.Sales_Invoice_Main_Discount_In_Amount +">0" ;
    qStr += " and ";
    qStr += " ( ";
    qStr += timestamp + ">='" + fromDate.toString("yyyy-MM-dd hh:mm:ss") + "'";
    qStr += " and ";
    qStr += timestamp + "<= '" + toDate.toString("yyyy-MM-dd hh:mm:ss") + "'";
    qStr += ")";
    qStr += "" ;
    qStr += " GROUP BY "+shelper.Sales_Invoice_Details_Voucher_No;
    qStr += " Order BY "+shelper.Sales_Invoice_Details_Voucher_No;



    qDebug()<<qStr;

    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}

QSqlQueryModel *LedgerHelper::getLedgerwiseTransactionSummaryByVoucherType(QDateTime fromDate, QDateTime toDate, int voucherType, int limit)
{
    qDebug()<<Q_FUNC_INFO;
    QString tableName;
    if(voucherType == GMVoucherTypes::PurchaseOrder || voucherType == GMVoucherTypes::SalesOrder)
        tableName = "Proxy_Transaction_Helper";
    else
        tableName = Transaction_Helper_TableName;

    QString qStr = "SELECT ";
    qStr += " 'L', trans." + Transaction_Helper_LedgerID + ", ";
    qStr += " led." + ledHelper->Ledger_Master_LedgerName + " as 'Ledger',";
    qStr += " SUM(" + Transaction_Helper_CrAmount + ") as 'Cr Amount', ";
    qStr += " SUM(" + Transaction_Helper_DrAmount + ") as 'Dr Amount'";
    qStr += " FROM " + tableName + " trans JOIN " + LedgerMasterDatabaseHelper::Ledger_Master_TableName + " led ";
    qStr += " ON trans.ledger_id = led.ledger_id " ;
    qStr += " WHERE " + Transaction_Helper_VoucherDate + " >='"+fromDate.toString("yyyy-MM-dd hh:mm:ss")+"'";
    qStr += " AND " + Transaction_Helper_VoucherDate + " <='"+toDate.toString("yyyy-MM-dd hh:mm:ss")+"'";
    qStr += " AND " + Transaction_Helper_VoucherType + " = '" + GMVoucherTypes::getVoucherString(voucherType) + "'";
    qStr += " GROUP BY trans." + Transaction_Helper_LedgerID ;
    if(limit == 999999)
        qStr += " ORDER BY Ledger";
    else {
        qStr += " ORDER BY " + Transaction_Helper_VoucherDate + " desc LIMIT " + QString::number(limit);

    }

    return DatabaseHelper::getQueryModel(qStr);

}

QSqlQueryModel *LedgerHelper::getTopCustomers(QDate fromDate, QDate toDate, int limit)
{
    qDebug()<<Q_FUNC_INFO;
    AccountGroupMasterDatabaseHelper accHelper;
    QString groupID = accHelper.getGroupIDByName("Sundry Debtors");

    QString qStr = " SELECT Ledger_Name as Customer , SUM(Debit_Amount) as 'Sales Value', COUNT(Voucher_Id) as 'Bill count'";
    qStr +=  " FROM " + ledHelper->Ledger_Master_TableName + " INNER JOIN " ;
    qStr += " (SELECT  GROUP_ID, GROUP_NAME, PARENT_ID " ;
    qStr += " FROM    (SELECT * FROM " + accHelper.Accounts_Group_TableName + " ORDER BY PARENT_ID, GROUP_ID) GROUPS_SORTED, ";
    qStr += " (SELECT @PV := '" + groupID + "') INITIALISATION WHERE FIND_IN_SET(PARENT_ID, @PV) ";
    qStr += " AND     LENGTH(@PV := CONCAT(@PV, ',', GROUP_ID))";
    qStr += " UNION SELECT  GROUP_ID, GROUP_NAME, PARENT_ID ";
    qStr += " FROM " + accHelper.Accounts_Group_TableName + " where Group_Id = '" + groupID + "') AS ACC_GROUP ";
    qStr += " ON Ledger_Master.GROUP_ID = ACC_GROUP.GROUP_ID";
    qStr += " LEFT OUTER JOIN Transaction_Helper ";
    qStr += " ON Ledger_Master.Ledger_Id = Transaction_Helper.Ledger_Id ";
    qStr += " WHERE Voucher_Date >= " + db->getDateString(fromDate);
    qStr += " AND Voucher_Date <= " + db->getDateString(toDate);
    qStr += " AND Voucher_Type = 'SALESVOUCHER' ";
    qStr += " GROUP BY Transaction_Helper.Ledger_ID ";
    qStr += " ORDER BY SUM(Debit_Amount) desc ";
    qStr += " LIMIT " + QString::number(limit);


    return DatabaseHelper::getQueryModel(qStr);

}

QSqlQueryModel *LedgerHelper::getRankBySalesValue(QString ledgerId ,QDate fromDate, QDate toDate)
{
    AccountGroupMasterDatabaseHelper accHelper;
    QString groupID = accHelper.getGroupIDByName("Sundry Debtors");

    QString qStr = "Select serial_Number, totalSales, billCount FROM (";

    qStr += " SELECT  @a:=@a+1 serial_number, Ledger_Id, totalSales, billCount FROM ";
    qStr += " (SELECT Ledger_Master.Ledger_Id , SUM(Debit_Amount) as totalSales, COUNT(Voucher_Id) as billCount";
    qStr +=  " FROM " + ledHelper->Ledger_Master_TableName + " INNER JOIN " ;
    qStr += " (SELECT  GROUP_ID, GROUP_NAME, PARENT_ID " ;
    qStr += " FROM    (SELECT * FROM " + accHelper.Accounts_Group_TableName + " ORDER BY PARENT_ID, GROUP_ID) GROUPS_SORTED, ";
    qStr += " (SELECT @PV := '" + groupID + "') INITIALISATION WHERE FIND_IN_SET(PARENT_ID, @PV) ";
    qStr += " AND     LENGTH(@PV := CONCAT(@PV, ',', GROUP_ID))";
    qStr += " UNION SELECT  GROUP_ID, GROUP_NAME, PARENT_ID ";
    qStr += " FROM " + accHelper.Accounts_Group_TableName + " where Group_Id = '" + groupID + "') AS ACC_GROUP ";
    qStr += " ON Ledger_Master.GROUP_ID = ACC_GROUP.GROUP_ID";
    qStr += " LEFT OUTER JOIN Transaction_Helper ";
    qStr += " ON Ledger_Master.Ledger_Id = Transaction_Helper.Ledger_Id ";
    qStr += " WHERE Voucher_Date >= " + db->getDateString(fromDate);
    qStr += " AND Voucher_Date <= " + db->getDateString(toDate);
    qStr += " GROUP BY Transaction_Helper.Ledger_ID ";
    qStr += " ORDER BY SUM(Debit_Amount) desc )A, ";
    qStr += " (SELECT @a:= 0) AS slNo )B ";
    qStr += " WHERE Ledger_Id like '" + ledgerId + "'";


    return DatabaseHelper::getQueryModel(qStr);

}

QSqlQueryModel *LedgerHelper::getTopVendors(QDate fromDate, QDate toDate, int limit)
{
    qDebug()<<Q_FUNC_INFO;
    AccountGroupMasterDatabaseHelper accHelper;
    QString groupID = accHelper.getGroupIDByName("Sundry Creditors");

    QString qStr = " SELECT Ledger_Name , SUM(Credit_Amount), COUNT(Voucher_Id) ";
    qStr +=  " FROM " + ledHelper->Ledger_Master_TableName + " INNER JOIN " ;
    qStr += " (SELECT  GROUP_ID, GROUP_NAME, PARENT_ID " ;
    qStr += " FROM    (SELECT * FROM " + accHelper.Accounts_Group_TableName + " ORDER BY PARENT_ID, GROUP_ID) GROUPS_SORTED, ";
    qStr += " (SELECT @PV := '" + groupID + "') INITIALISATION WHERE FIND_IN_SET(PARENT_ID, @PV) ";
    qStr += " AND     LENGTH(@PV := CONCAT(@PV, ',', GROUP_ID))";
    qStr += " UNION SELECT  GROUP_ID, GROUP_NAME, PARENT_ID ";
    qStr += " FROM " + accHelper.Accounts_Group_TableName + " where Group_Id = '" + groupID + "') AS ACC_GROUP ";
    qStr += " ON Ledger_Master.GROUP_ID = ACC_GROUP.GROUP_ID";
    qStr += " LEFT OUTER JOIN Transaction_Helper ";
    qStr += " ON Ledger_Master.Ledger_Id = Transaction_Helper.Ledger_Id ";
    qStr += " WHERE Voucher_Date >= " + db->getDateString(fromDate);
    qStr += " AND Voucher_Date <= " + db->getDateString(toDate);
    // qStr += " AND Transaction_Helper.Ledger_ID in (";
    qStr += " GROUP BY Transaction_Helper.Ledger_ID ";
    qStr += " AND Voucher_Type = 'PURCHASEVOUCHER' ";
    qStr += " ORDER BY SUM(Credit_Amount) desc ";
    qStr += " LIMIT " + QString::number(limit);

    return DatabaseHelper::getQueryModel(qStr);

}

QSqlQueryModel *LedgerHelper::getAverageCreditPeriodByCustomer(QString LedgerId, QDate fromDate, QDate toDate)
{
    QString qStr = "";
    qStr += "SELECT cast(SUM(numDays) / Count(*) as int)  as avgNumDays, ";
    qStr += " cast(SUM(billCount)/Count(*) as int) as avgBillCount ";
    qStr += " FROM ";
    qStr += " (SELECT RefNo, RefPrefix, RefType, max(VoucherDate) as voucherDate, ";
    qStr += " DateDiff(max(VoucherDate), (select voucherDate from billwise_records ";
    qStr += " where voucherNo = Ref.RefNo and VoucherType = Ref.RefType ";
    qStr += " and voucherPrefix = ref.RefPrefix)) as numDays, ";
    qStr += " Count(VoucherNo) as billCount ";
    qStr += " FROM Billwise_Records Ref ";
    qStr += " WHERE Ref.LedgerId = '" + LedgerId + "' ";
    qStr += " AND Ref.VoucherDate <= " + db->getDateString(toDate);
    qStr += " AND (select voucherDate from billwise_records ";
    qStr += " where voucherNo = Ref.RefNo and VoucherType = Ref.RefType ";
    qStr += " and voucherPrefix = ref.RefPrefix) >= " + db->getDateString(fromDate);
    qStr += " GROUP BY RefNo, RefType, RefPrefix ";
    qStr += " HAVING SUM(COALESCE(CreditAmount,0)) -  SUM(COALESCE(DebitAmount,0)) = 0 " ;
    qStr += ") A ";

    return DatabaseHelper::getQueryModel(qStr);

}

QSqlQueryModel *LedgerHelper::getOverDueBillsByLedger(QString LedgerId, QDate fromDate, QDate toDate, int limit)

{
    QString qStr = "SELECT RefNo as 'Voucher No', RefPrefix as 'Voucher Prefix', RefType as 'Voucher Type', ";
    qStr += "min(VoucherDate) as 'Voucher Date',  ";
    qStr += " (select DebitAmount - creditAmount from billwise_records ";
    qStr += " where voucherNo = Ref.RefNo AND voucherType = Ref.RefType AND voucherPrefix = Ref.RefPrefix) ";
    qStr += " as 'Bill Amount', ";
    qStr += " SUM(COALESCE(DebitAmount,0)) -  SUM(COALESCE(CreditAmount,0)) as 'Balance Amount'  ";
    qStr += " FROM Billwise_Records Ref  WHERE Ref.LedgerId = '" + LedgerId + "' ";
    //    qStr += " AND Ref.VoucherDate >= " + db->getDateString(fromDate);
    qStr += " AND Ref.VoucherDate <= " + db->getDateString(toDate) ;
    qStr += " AND (Method_Of_Adj<> 'ON ACC' OR Method_Of_Adj IS NULL) ";
    qStr += " GROUP BY RefNo, RefType, RefPrefix ";
    qStr += " HAVING SUM(COALESCE(CreditAmount,0)) - SUM(COALESCE(DebitAmount,0)) <> 0 ";
    qStr += " ORDER BY 4";
    qStr += "  LIMIT " + QString::number(limit);

    return DatabaseHelper::getQueryModel(qStr);

}

QSqlQueryModel *LedgerHelper::getOverDueBillsByGroup(QString groupID, QDate fromDate, QDate toDate)
{
    QString qStr = "SELECT RefNo as 'Voucher No', RefPrefix as 'Voucher Prefix', RefType as 'Voucher Type', ";
    qStr += " min(VoucherDate) as 'Voucher Date,  ";
    qStr += " (select DebitAmount - creditAmount from billwise_records ";
    qStr += " where voucherNo = Ref.RefNo AND voucherType = Ref.RefType AND voucherPrefix = Ref.RefPrefix) ";
    qStr += " as 'Bill Amount', ";
    qStr += " SUM(COALESCE(DebitAmount,0)) -  SUM(COALESCE(CreditAmount,0)) as 'Balance Amount'  ";
    qStr += " FROM " + ledHelper->Ledger_Master_TableName + " INNER JOIN " ;
    qStr += " (SELECT  GROUP_ID, GROUP_NAME, PARENT_ID " ;
    qStr += " FROM    (SELECT * FROM " + accHelper->Accounts_Group_TableName + " ORDER BY PARENT_ID, GROUP_ID) GROUPS_SORTED, ";
    qStr += " (SELECT @PV := '" + groupID + "') INITIALISATION WHERE FIND_IN_SET(PARENT_ID, @PV) ";
    qStr += " AND     LENGTH(@PV := CONCAT(@PV, ',', GROUP_ID))";
    qStr += " UNION SELECT  GROUP_ID, GROUP_NAME, PARENT_ID ";
    qStr += " FROM " + accHelper->Accounts_Group_TableName + " where Group_Id = '" + groupID + "') AS ACC_GROUP ";
    qStr += " ON Ledger_Master.GROUP_ID = ACC_GROUP.GROUP_ID";
    qStr += " LEFT OUTER JOIN Billwise_Records Ref  ";
    qStr += " ON Ledger_Master.Ledger_ID = Ref.LedgerID ";
    qStr += " WHERE  ";
    qStr += " Ref.VoucherDate <= " + db->getDateString(toDate) ;
    qStr += " AND (Method_Of_Adj<> 'ON ACC' OR Method_Of_Adj IS NULL) ";
    qStr += " GROUP BY RefNo, RefType, RefPrefix ";
    qStr += " HAVING SUM(COALESCE(CreditAmount,0)) - SUM(COALESCE(DebitAmount,0)) <> 0 ";
    qStr += " ORDER BY 4";

    return DatabaseHelper::getQueryModel(qStr);

}

QSqlQueryModel *LedgerHelper::getCreditInfoByLedger(QString LedgerId, QDate fromDate, QDate toDate)
{
    QString qStr = " SELECT ";
    qStr += " bal as 'Outstanding balance', ODate as 'Earliest Outstanding Date', ";
    qStr += " Credit_Period as 'Credit Period', CreditAllowed as 'Credit Limit'";
    qStr += "  FROM " + ledHelper->Ledger_Master_TableName + " JOIN ";
    qStr += " (SELECT LedgerId, min(voucherDate) 'ODate', SUM(Balance) as bal FROM (";
    qStr += " SELECT LedgerId, RefNo as 'Voucher No', RefPrefix as 'Voucher Prefix', RefType as 'Voucher Type', ";
    qStr += " min(VoucherDate) as voucherDate, ";
    qStr += " SUM(COALESCE(DebitAmount,0)) -  SUM(COALESCE(CreditAmount,0)) as Balance  ";
    qStr += " FROM Billwise_Records Ref  WHERE Ref.LedgerId = '" + LedgerId + "' ";
    qStr += " AND Ref.VoucherDate <= " + db->getDateString(toDate) ;
    qStr += " AND (Method_Of_Adj<> 'ON ACC' OR Method_Of_Adj IS NULL) ";
    qStr += " GROUP BY RefNo, RefType, RefPrefix ";
    qStr += " HAVING SUM(COALESCE(DebitAmount,0)) -  SUM(COALESCE(CreditAmount,0)) > 0 ) ";
    qStr += " A) as outst ON Ledger_Id = LedgerId ";
    qStr += " WHERE Ledger_Id = '" + LedgerId + "'";
    qStr += " ";

    return DatabaseHelper::getQueryModel(qStr);

}

QSqlQueryModel *LedgerHelper::getLedgerTransValueByFrequency(int voucherType, QDate fromDate, QDate toDate, QString LedgerId)
{
    qDebug()<<Q_FUNC_INFO;
    QString tableName = GMVoucherTypes::getVoucherMainTablename(voucherType);
    QString qStr = " SELECT 'Daily' as Period, ";
    qStr += " (SELECT round(COALESCE(SUM(Total),0),2) as 'Todays Sales' FROM " + tableName ;
    qStr += " WHERE Ledger_ID like '" + LedgerId + "' AND Voucher_Date = CURRENT_DATE) as 'Current Total', ";
    qStr += " (SELECT COALESCE(round(SUM(Total)/Count(*),2),0) as 'Daily Average Sales' ";
    qStr += " FROM ( SELECT COALESCE(SUM(Total),0) as Total FROM " + tableName ;
    qStr += " where Ledger_ID like '" + LedgerId + "' ";
    qStr += " AND Voucher_Date >= " + db->getDateString(fromDate);
    qStr += " AND Voucher_Date <= " + db->getDateString(toDate);
    qStr += " group by Voucher_Date )A) as 'Average'";
    qStr += " UNION ";
    qStr += " SELECT 'Week' , (SELECT round(COALESCE(SUM(Total),0),2) as WeeklySum ";
    qStr += " FROM " + tableName + " where  Ledger_ID like '" + LedgerId + "' ";
    qStr += " AND Voucher_Date >= subDate(CURRENT_DATE, DayOfWeek(CURRENT_DATE)) ";
    qStr += " AND Voucher_Date <= CURRENT_DATE) as 'CurrentTotal', ";
    qStr += " (SELECT COALESCE(round(SUM(WeeklySum)/COUNT(*),2),0) as WeeklyAvg ";
    qStr += " FROM (SELECT SUM(Total) as WeeklySum, week(Voucher_Date) ";
    qStr += " FROM " + tableName + " where Ledger_ID like '" + LedgerId + "' ";
    qStr += " AND Voucher_Date >= " + db->getDateString(fromDate);
    qStr += " AND Voucher_Date <= " + db->getDateString(toDate);
    qStr += " group by YEARWEEK(Voucher_Date) ) A ) as 'Average'";
    qStr += " UNION ";
    qStr += " SELECT 'Month', (SELECT round(COALESCE(SUM(Total),0),2) as MonthSum ";
    qStr += " FROM " + tableName + " where  Ledger_ID like '" + LedgerId + "' ";
    qStr += " AND Voucher_Date >= DATE_FORMAT(NOW() ,'%Y-%m-01') ";
    qStr += " AND Voucher_Date <= CURRENT_DATE) as 'MonthTotal', ";
    qStr += " (SELECT COALESCE(round(SUM(MonthSum)/COUNT(*),2),0) as WeeklyAvg ";
    qStr += " FROM (SELECT SUM(Total) as MonthSum FROM " + tableName ;
    qStr += " where Ledger_ID like '" + LedgerId + "' ";
    qStr += " AND Voucher_Date >= " + db->getDateString(fromDate);
    qStr += " AND Voucher_Date <= " + db->getDateString(toDate);
    qStr += " group by MONTH(Voucher_Date) ) A ) as 'Average' ";
    qStr += " UNION ";
    qStr += " SELECT 'Quarter', (SELECT round(COALESCE(SUM(Total),0),2) as QuarterSum ";
    qStr += " FROM " + tableName + " where  Ledger_ID like '" + LedgerId + "' ";
    qStr += " AND Voucher_Date >= MAKEDATE(YEAR(CURDATE()), 1) + INTERVAL QUARTER(CURDATE()) QUARTER";
    qStr += " - INTERVAL 1 QUARTER AND Voucher_Date <= CURRENT_DATE) as 'QuarterTotal', ";
    qStr += " (SELECT COALESCE(round(SUM(QuarterSum)/COUNT(*),2),0) as WeeklyAvg ";
    qStr += " FROM (SELECT SUM(Total) as QuarterSum FROM " + tableName ;
    qStr += " where Ledger_ID like '" + LedgerId + "' ";
    qStr += " AND Voucher_Date >= " + db->getDateString(fromDate);
    qStr += " AND Voucher_Date <= " + db->getDateString(toDate);
    qStr += " group by QUARTER(Voucher_Date) ) A ) as 'Average' ";
    qStr += " UNION ";
    qStr += " SELECT 'Year', (SELECT round(COALESCE(SUM(Total),0),2) as AnnualSum ";
    qStr += " FROM " + tableName + " where  Ledger_ID like '" + LedgerId + "' ";
    qStr += " AND Voucher_Date >= DATE_FORMAT(NOW() ,'%Y-01-01') ";
    qStr += " AND Voucher_Date <= CURRENT_DATE) as 'AnnualTotal', ";
    qStr += " (SELECT COALESCE(round(SUM(AnnualSum)/COUNT(*),2),0) as AnnualAvg ";
    qStr += " FROM (SELECT SUM(Total) as AnnualSum FROM " + tableName ;
    qStr += " where Ledger_ID like '" + LedgerId + "' ";
    qStr += " AND Voucher_Date >= " + db->getDateString(fromDate);
    qStr += " AND Voucher_Date <= " + db->getDateString(toDate);
    qStr += " group by YEAR(Voucher_Date) ) A ) as 'Average' ";

    return DatabaseHelper::getQueryModel(qStr);
}

QSqlQueryModel *LedgerHelper::getTopItemsByCustomer(QString ledgerID, QDate fromDate, QDate toDate, int limit)
{
    SalesInventoryItemDatabaseHelper itemHelper;

    QString qStr = "SELECT  ";
    qStr += " (select item_name from " + itemHelper.Sales_Inventory_Table_Name + " where item_id = inventory_item_id) as Item, ";
    qStr += " round(COALESCE(SUM(SubTotal),0),2) as Value ";
    qStr += " FROM sales_invoice_details det ";
    qStr += " WHERE (select Ledger_ID from sales_invoice_main ";
    qStr += " WHERE  voucher_no = det.Voucher_No and voucher_prefix = det.Voucher_Prefix) = '" + ledgerID +"'";
    qStr += " AND Voucher_Date >= " + db->getDateString(fromDate);
    qStr += " AND Voucher_Date <= " + db->getDateString(toDate);
    qStr += " GROUP BY Inventory_Item_Id ";
    qStr += " ORDER BY SUM(SubTotal) desc ";
    qStr += " LIMIT " + QString::number(limit) ;

    return DatabaseHelper::getQueryModel(qStr);
}

QSqlQueryModel *LedgerHelper::getTopItemsByVendor(QString ledgerID, QDate fromDate, QDate toDate, int limit)
{
    SalesInventoryItemDatabaseHelper itemHelper;

    QString qStr = "SELECT  ";
    qStr += " (select " + itemHelper.Sales_Inventory_ItemName + " from " + itemHelper.Sales_Inventory_Table_Name + " where item_id = inventory_item_id) as Item, ";
    qStr += " round(COALESCE(SUM(SubTotal),0),2) as Value ";
    qStr += " FROM sales_invoice_details det ";
    qStr += " WHERE (select Ledger_ID from sales_invoice_main ";
    qStr += " WHERE  voucher_no = det.Voucher_No and voucher_prefix = det.Voucher_Prefix) = '" + ledgerID +"'";
    qStr += " AND Voucher_Date >= " + db->getDateString(fromDate);
    qStr += " AND Voucher_Date <= " + db->getDateString(toDate);
    qStr += " GROUP BY Inventory_Item_Id ";
    qStr += " ORDER BY SUM(SubTotal) desc ";
    qStr += " LIMIT 5" + QString::number(limit) ;

    return DatabaseHelper::getQueryModel(qStr);
}

int LedgerHelper::getCustomerCount(QDate fromDate, QDate toDate)
{
    AccountGroupMasterDatabaseHelper accHelper;
    QString groupID = accHelper.getGroupIDByName("Sundry Debtors");

    QString qStr = "";
    qStr += " SELECT Count(distinct Ledger_Master.Ledger_Id)";
    qStr +=  " FROM " + ledHelper->Ledger_Master_TableName + " INNER JOIN " ;
    qStr += " (SELECT  GROUP_ID, GROUP_NAME, PARENT_ID " ;
    qStr += " FROM    (SELECT * FROM " + accHelper.Accounts_Group_TableName + " ORDER BY PARENT_ID, GROUP_ID) GROUPS_SORTED, ";
    qStr += " (SELECT @PV := '" + groupID + "') INITIALISATION WHERE FIND_IN_SET(PARENT_ID, @PV) ";
    qStr += " AND     LENGTH(@PV := CONCAT(@PV, ',', GROUP_ID))";
    qStr += " UNION SELECT  GROUP_ID, GROUP_NAME, PARENT_ID ";
    qStr += " FROM " + accHelper.Accounts_Group_TableName + " where Group_Id = '" + groupID + "') AS ACC_GROUP ";
    qStr += " ON Ledger_Master.GROUP_ID = ACC_GROUP.GROUP_ID";
    qStr += " LEFT OUTER JOIN Transaction_Helper ";
    qStr += " ON Ledger_Master.Ledger_Id = Transaction_Helper.Ledger_Id ";
    qStr += " WHERE Voucher_Date >= " + db->getDateString(fromDate);
    qStr += " AND Voucher_Date <= " + db->getDateString(toDate);

    qDebug()<<qStr;

    return DatabaseHelper::getSingletonQueryResult(qStr).toInt();

}

QString LedgerHelper::getTotalOutstandingBalance(QDate fromDate, QDate toDate)
{
    QString qStr;

    AccountGroupMasterDatabaseHelper acc;
    QString groupID = acc.getGroupIDByName("Sundry Debtors");

    qStr += " SELECT ROUND(COALESCE(SUM(CLOSING),0),2) FROM ";
    qStr += " (SELECT LED.LEDGER_NAME, LED.LEDGER_ID,  ";
    qStr += " round(COALESCE(SUM(TRANS.DEBIT_AMOUNT),0) - COALESCE(SUM(TRANS.CREDIT_AMOUNT),0),2)  AS CLOSING ";
    qStr += " FROM " + ledHelper->Ledger_Master_TableName + " LED ";
    qStr += " INNER JOIN  ";
    qStr += " (SELECT  GROUP_ID, ";
    qStr += "         GROUP_NAME, ";
    qStr += "         PARENT_ID  ";
    qStr += " FROM    (SELECT * FROM " + accHelper->Accounts_Group_TableName + " ";
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

    return DatabaseHelper::getSingletonQueryResult(qStr).toString();
}


float LedgerHelper::getTotalPurchaseValue(QDate fromDate, QDate toDate)
{
    qDebug()<<Q_FUNC_INFO;
    QString qStr;
    PurchaseVoucherDatabaseHelper phelper;
    qStr = "";
    qStr += " SELECT sum(Purchase_invoice_main."+ phelper.Purchase_Invoice_Main_Total + ")";
    qStr += " FROM "+ phelper.Purchase_Invoice_Main_TableName + "" ; // fro table , not column
    qStr += " WHERE Voucher_Date >= " + db->getDateString(fromDate);
    qStr += " AND Voucher_Date <= " + db->getDateString(toDate);
    return DatabaseHelper::getSingletonQueryResult(qStr).toFloat() ;

}
float LedgerHelper::getTotalSalesValue(QDate fromDate, QDate toDate)
{
    qDebug()<<Q_FUNC_INFO;
    QString qStr;
    salesVoucherDatabaseHelper shelper;
    qStr = "";
    qStr += " SELECT sum(Sales_invoice_main."+ shelper.Sales_Invoice_Main_Total + ")";
    qStr += " FROM "+ shelper.Sales_Invoice_Main_TableName + "" ; // fro table , not column
    qStr += " WHERE Voucher_Date >= " + db->getDateString(fromDate);
    qStr += " AND Voucher_Date <= " + db->getDateString(toDate);
    return DatabaseHelper::getSingletonQueryResult(qStr).toFloat() ;

}

QSqlQueryModel *LedgerHelper::getTopDebtors(QDate fromDate, QDate toDate, int limit)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    TransactionHelper *t= new TransactionHelper();
    salesVoucherDatabaseHelper shelper;

    AccountGroupMasterDatabaseHelper acc;
    QString groupID = acc.getGroupIDByName("Sundry Debtors");

    //Ledgers within group

    QString qStr;
    qStr += " SELECT LEDGER_NAME as 'Customer',  ";
    qStr += " CLOSING as 'Balance' FROM ";
    qStr += " (SELECT LED.LEDGER_NAME, LED.LEDGER_ID,  ";
//    qStr += " CAST(COALESCE(LED.OPENING_BALANCE,0)+ ";
    qStr += " COALESCE(SUM(TRANS.DEBIT_AMOUNT),0) - COALESCE(SUM(TRANS.CREDIT_AMOUNT),0) AS DECIMAL(10,2)) AS CLOSING ";
    qStr += " FROM " + ledHelper->Ledger_Master_TableName + " LED ";
    qStr += " INNER JOIN  ";
    qStr += " (SELECT  GROUP_ID, ";
    qStr += "         GROUP_NAME, ";
    qStr += "         PARENT_ID  ";
    qStr += " FROM    (SELECT * FROM " + accHelper->Accounts_Group_TableName + " ";
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
    qStr += " WHERE CLOSING <>0";
    qStr += " ORDER BY 2 desc LIMIT " + QString::number(limit);

    qDebug()<<qStr;

    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}




