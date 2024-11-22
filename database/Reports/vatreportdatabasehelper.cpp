#include "vatreportdatabasehelper.h"

VatReportDatabaseHelper::VatReportDatabaseHelper(QObject *parent) : QObject(parent)
{
    db = new DatabaseHelper;
}

QSqlQueryModel *VatReportDatabaseHelper::getSalesVatReport(QDate fromDate, QDate toDate)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString qStr = "SELECT ";




    model = DatabaseHelper::getQueryModel(qStr);

    return model;
}

QSqlQueryModel *VatReportDatabaseHelper::getPurchaseVatReport(QDate fromDate, QDate toDate)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString qStr = "";

    model = DatabaseHelper::getQueryModel(qStr);

    return model;
}

QSqlQueryModel *VatReportDatabaseHelper::getSalesReturnVatReport(QDate fromDate, QDate toDate)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString qStr = "";

    model = DatabaseHelper::getQueryModel(qStr);

    return model;
}

QSqlQueryModel *VatReportDatabaseHelper::getPurchaseReturnVatReport(QDate fromDate, QDate toDate)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString qStr = "";

    model = DatabaseHelper::getQueryModel(qStr);

    return model;
}

float VatReportDatabaseHelper::getSalesTotal(QString billsFrom, QString billsTo)
{
    LedgerMasterDatabaseHelper led;
    salesVoucherDatabaseHelper shelper;

    QString qStr;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    float value =0;

    qStr ="SELECT SUM("+shelper.Sales_Invoice_Main_Total+") FROM ";
    qStr += shelper.Sales_Invoice_Main_TableName + " WHERE ";
    qStr += "CAST("+shelper.Sales_Invoice_Main_Voucher_No+" AS UNSIGNED)" + ">= CAST(" + billsFrom +" AS UNSIGNED)";
    qStr += " and ";
    qStr += "CAST("+shelper.Sales_Invoice_Main_Voucher_No+" AS UNSIGNED)" + "<= CAST(" + billsTo+ " AS UNSIGNED)";


    query.prepare(qStr);
    if(query.exec()){
        while(query.next()){
            value = query.value(0).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }

    return value;
}

float VatReportDatabaseHelper::outputVatTotal(QString billsFrom, QString billsTo)
{
    LedgerMasterDatabaseHelper led;
    TransactionHelper trans;
    salesVoucherDatabaseHelper shelper;

    QString ledID = "0x2x14x6";
    QString qStr;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    float value =0;

    qStr += "SELECT SUM("+trans.Transaction_Helper_Credit_Amount+"-"+trans.Transaction_Helper_Debit_Amount+")";
    qStr += " From "+trans.Transaction_Helper_TableName + " where ";
    qStr += trans.Transaction_Helper_LedgerID + "='"+ledID+"'";
    qStr += " and ";
    qStr += "Cast("+trans.Transaction_Helper_VoucherNo + " as UNSIGNED) >= CAST("+billsFrom +" AS UNSIGNED)" ;
    qStr += " and ";
    qStr += "Cast("+trans.Transaction_Helper_VoucherNo + " as UNSIGNED) <= CAST("+billsTo +" AS UNSIGNED)" ;
    qStr += " and ";
    qStr += trans.Transaction_Helper_VoucherType + "='"+shelper.VoucherType+"'";

    query.prepare(qStr);
    if(query.exec()){
        while(query.next()){
            value = query.value(0).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return value;
}


float VatReportDatabaseHelper::getAmountByLedger(QString ledID, QDate fromDate, QDate toDate, QString voucherType)
{
    LedgerMasterDatabaseHelper led;
    TransactionHelper trans;
//    salesVoucherDatabaseHelper shelper;

//    QString ledID = "0x7x5";
    QString qStr;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    float value =0;

    qStr += " SELECT ( select " + led.Ledger_Master_LedgerName + " from " + led.Ledger_Master_TableName ;
    qStr += " where " + led.Ledger_Master_LedgerID + " = trans."  + trans.Transaction_Helper_LedgerID + "), ";
    qStr += " ABS(SUM("+trans.Transaction_Helper_Credit_Amount+"-"+trans.Transaction_Helper_Debit_Amount+"))";
    qStr += " From " +trans.Transaction_Helper_TableName + " trans where ";
    qStr += trans.Transaction_Helper_LedgerID + "='"+ledID+"'";
    qStr += " and ";
    qStr += trans.Transaction_Helper_VoucherDate + " >= "+ db->getDateString(fromDate) ;
    qStr += " and ";
    qStr += trans.Transaction_Helper_VoucherDate + " <= "+ db->getDateString(toDate) ;
    qStr += " and ";
    qStr += trans.Transaction_Helper_VoucherType + "='"+voucherType+"'";
//    qStr += " group by " + trans.Transaction_Helper_LedgerID;

    if(ledID == "0x7xSV5")
        qDebug()<<qStr;

    query.prepare(qStr);
    if(query.exec()){
        while(query.next()){
            value = query.value(1).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }

    return value;
}


QSqlQueryModel *VatReportDatabaseHelper::getTaxReportModelByHSN( QDate fromDate, QDate toDate)
{
    LedgerMasterDatabaseHelper led;
    TransactionHelper trans;
    ItemTransactionDatabaseHelper itemTrans;
//    salesVoucherDatabaseHelper shelper;

//    QString ledID = "0x7x5";
    QString qStr;

    DatabaseHelper *db = new DatabaseHelper;

    qStr += " select HSN_Code,'' Description,  default_uom_id as UOM , cast(Taxable_Value as decimal(10,2)) as 'Taxable Value', Vat_Rate as 'Tax Rate',";
    qStr += " cast(max(CGST) as decimal(10,2)) as CGST, cast(Max(SGST) as decimal(10,2)) SGST, cast(Max(IGST) as decimal(10,2)) IGST, ";
    qStr += " cast(Taxable_Value + max(cgst) + max(sgst) + max(igst) as decimal(10,2)) as 'Total' ";
    qStr += " from ( ";
    qStr += " SELECT HSN_Code, (select uom_name from uomlist where uom_id = default_uom_id ) default_uom_id,  abs(sum(cr_amount - dr_amount)) as Taxable_Value, Vat_Rate, ledger_id, ";
    qStr += " ( case when Ledger_ID like '0x2x14xOCG%' then  ABS(SUM(Credit_Amount-Debit_Amount)) else 0 end) as CGST, ";
    qStr += " ( case when Ledger_ID like '0x2x14xOSG%' then  ABS(SUM(Credit_Amount-Debit_Amount)) else 0 end) as SGST, ";
    qStr += " ( case when Ledger_ID like '0x2x14xOIG%' then  ABS(SUM(Credit_Amount-Debit_Amount)) else 0 end) as IGST ";
    qStr += " From Transaction_Helper trans ";
    qStr += " join transaction_item_helper itemtrans on ";
    qStr += " trans.voucher_id = itemtrans.voucherID ";
    qStr += " ";
    qStr += " join " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item ";
    qStr += " on itemtrans.itemid = item.item_id ";
    qStr += " ";
    qStr += " where ";
    qStr += "  (Ledger_ID='0x2x14xOCG25' or ledger_id = '0x2x14xOSG25') ";
    qStr += " and voucher_Date >=str_to_date('" + fromDate.toString("yyyy-MM-dd") + "','%Y-%m-%d')" ;
    qStr += " and voucher_Date <=str_to_date('" + toDate.toString("yyyy-MM-dd") + "','%Y-%m-%d')" ;
    qStr += " and item.Vat_Rate = 5 ";
    qStr += " ";
    qStr += " group by hsn_code, ledger_id";
    qStr += "     ) A ";
    qStr += " group by HSN_Code";

    qDebug()<<"&&&&&&HSN5"<<qStr;

    qStr += " UNION "    ;


    qStr += " select HSN_Code,'' Description,  default_uom_id,  cast(Taxable_Value as decimal(10,2)) as 'Taxable Value', Vat_Rate as 'Tax Rate',";
    qStr += " cast(max(CGST) as decimal(10,2)) as CGST, cast(Max(SGST) as decimal(10,2)) SGST, cast(Max(IGST) as decimal(10,2)) IGST, ";
    qStr += " cast(Taxable_Value + max(cgst) + max(sgst) + max(igst) as decimal(10,2)) as 'Total' ";
    qStr += "from ( ";
    qStr += " SELECT HSN_Code, (select uom_name from uomlist where uom_id = default_uom_id ) default_uom_id,   abs(sum(cr_amount - dr_amount)) as Taxable_Value, Vat_Rate, ledger_id, ";
    qStr += " ( case when Ledger_ID like '0x2x14xOCG%' then  ABS(SUM(Credit_Amount-Debit_Amount)) else 0 end) as CGST, ";
    qStr += " ( case when Ledger_ID like '0x2x14xOSG%' then  ABS(SUM(Credit_Amount-Debit_Amount)) else 0 end) as SGST, ";
    qStr += " ( case when Ledger_ID like '0x2x14xOIG%' then  ABS(SUM(Credit_Amount-Debit_Amount)) else 0 end) as IGST ";
    qStr += " From Transaction_Helper trans ";
    qStr += " join transaction_item_helper itemtrans on ";
    qStr += " trans.voucher_id = itemtrans.voucherID ";
    qStr += " ";
    qStr += " join " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item ";
    qStr += " on itemtrans.itemid = item.item_id ";
    qStr += " ";
    qStr += " where ";
    qStr += "  (Ledger_ID='0x2x14xOCG6' or ledger_id = '0x2x14xOSG6') ";
    qStr += " and voucher_Date >=" + db->getDateString(fromDate) + " and voucher_Date <= " + db->getDateString(toDate);
    qStr += " and item.Vat_Rate = 12 ";
    qStr += " ";
    qStr += " group by hsn_code, ledger_id";
    qStr += "     ) A ";
    qStr += " group by HSN_Code";

    qStr += " UNION ";


    qStr += " select HSN_Code,'' Description,  default_uom_id,  cast(Taxable_Value as decimal(10,2)) as 'Taxable Value', Vat_Rate as 'Tax Rate',";
    qStr += " cast(max(CGST) as decimal(10,2)) as CGST, cast(Max(SGST) as decimal(10,2)) SGST, cast(Max(IGST) as decimal(10,2)) IGST, ";
    qStr += " cast(Taxable_Value + max(cgst) + max(sgst) + max(igst) as decimal(10,2)) as 'Total' ";
            qStr += "from ( ";
    qStr += " SELECT HSN_Code,(select uom_name from uomlist where uom_id = default_uom_id )  default_uom_id,  abs(sum(cr_amount - dr_amount)) as Taxable_Value, Vat_Rate, ledger_id, ";
    qStr += " ( case when Ledger_ID like '0x2x14xOCG%' then  ABS(SUM(Credit_Amount-Debit_Amount)) else 0 end) as CGST, ";
    qStr += " ( case when Ledger_ID like '0x2x14xOSG%' then  ABS(SUM(Credit_Amount-Debit_Amount)) else 0 end) as SGST, ";
    qStr += " ( case when Ledger_ID like '0x2x14xOIG%' then  ABS(SUM(Credit_Amount-Debit_Amount)) else 0 end) as IGST ";
    qStr += " From Transaction_Helper trans ";
    qStr += " join transaction_item_helper itemtrans on ";
    qStr += " trans.voucher_id = itemtrans.voucherID ";
    qStr += " ";
    qStr += " join " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item ";
    qStr += " on itemtrans.itemid = item.item_id ";
    qStr += " ";
    qStr += " where ";
    qStr += "  (Ledger_ID='0x2x14xOCG9' or ledger_id = '0x2x14xOSG9') ";
    qStr += " and voucher_Date >=" + db->getDateString(fromDate) + " and voucher_Date <= " + db->getDateString(toDate);
    qStr += " and item.Vat_Rate = 18 ";
    qStr += " ";
    qStr += " group by hsn_code, ledger_id";
    qStr += "     ) A ";
    qStr += " group by HSN_Code";

    qStr += " UNION ";


    qStr += " select HSN_Code, '' Description, default_uom_id,   cast(Taxable_Value as decimal(10,2)) as 'Taxable Value', Vat_Rate as 'Tax Rate',";
    qStr += " cast(max(CGST) as decimal(10,2)) as CGST, cast(Max(SGST) as decimal(10,2)) SGST, cast(Max(IGST) as decimal(10,2)) IGST, ";
    qStr += " cast(Taxable_Value + max(cgst) + max(sgst) + max(igst) as decimal(10,2)) as 'Total' ";
    qStr += "from ( ";
    qStr += " SELECT HSN_Code,(select uom_name from uomlist where uom_id = default_uom_id )default_uom_id,   abs(sum(cr_amount - dr_amount)) as Taxable_Value, Vat_Rate, ledger_id, ";
    qStr += " ( case when Ledger_ID like '0x2x14xOCG%' then  ABS(SUM(Credit_Amount-Debit_Amount)) else 0 end) as CGST, ";
    qStr += " ( case when Ledger_ID like '0x2x14xOSG%' then  ABS(SUM(Credit_Amount-Debit_Amount)) else 0 end) as SGST, ";
    qStr += " ( case when Ledger_ID like '0x2x14xOIG%' then  ABS(SUM(Credit_Amount-Debit_Amount)) else 0 end) as IGST ";
    qStr += " From Transaction_Helper trans ";
    qStr += " join transaction_item_helper itemtrans on ";
    qStr += " trans.voucher_id = itemtrans.voucherID ";
    qStr += " ";
    qStr += " join " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" item ";
    qStr += " on itemtrans.itemid = item.item_id ";
    qStr += " ";
    qStr += " where ";
    qStr += "  (Ledger_ID='0x2x14xOCG14' or ledger_id = '0x2x14xOSG14') ";
    qStr += " and voucher_Date >=" + db->getDateString(fromDate) + " and voucher_Date <= " + db->getDateString(toDate);
    qStr += " and item.Vat_Rate = 28 ";
    qStr += " ";
    qStr += " group by hsn_code, ledger_id";
    qStr += "     ) A ";
    qStr += " group by HSN_Code";


    qStr  = " select distinct hsn_code, '' Description, (select uom_name from uomlist where uom_id = default_uom_id) 'UOM', sum(cr_amount) 'Taxable Value', Vat_Rate 'Tax Rate', ";
    qStr += " round(sum(cr_amount) * (vat_rate /200),2) as CGST, ";
    qStr += " round(sum(cr_amount) * (vat_rate /200),2) as SGST,  ";
    qStr += " round(sum(cr_amount) + (sum(cr_amount) * (vat_rate /100)) , 2) as Total " ;
    qStr += " from transaction_item_helper itemtrans join Sales_Inventory_Items item ";
    qStr += " on itemtrans.itemid = item.item_id where ";
    qStr += " voucherDate >= " + db->getDateString(fromDate);
    qStr += " and voucherDate <= " + db->getDateString(toDate);
    qStr += " and voucherType = 'SalesVoucher' ";
    qStr += " group by hsn_code ";

    return db->getQueryModel(qStr);
}

QSqlQueryModel *VatReportDatabaseHelper::getCessReportModel(QString billsFrom, QString billsTo)
{
    LedgerMasterDatabaseHelper led;
    TransactionHelper trans;
    salesVoucherDatabaseHelper shelper;
    QString qStr;
    
    qStr += "SELECT " + trans.Transaction_Helper_VoucherNo + " as 'Voucher No',";
    qStr += " SUM("+trans.Transaction_Helper_Credit_Amount+"-"+trans.Transaction_Helper_Debit_Amount+") as 'Cess Amount' ";
    qStr += " From "+trans.Transaction_Helper_TableName + " where ";
    qStr += trans.Transaction_Helper_LedgerID + "='0x2x14xCess'";
    qStr += " and ";
    qStr += "Cast("+trans.Transaction_Helper_VoucherNo + " as UNSIGNED) >= CAST("+billsFrom +" AS UNSIGNED)" ;
    qStr += " and ";
    qStr += "Cast("+trans.Transaction_Helper_VoucherNo + " as UNSIGNED) <= CAST("+billsTo +" AS UNSIGNED)" ;
    qStr += " and ";
    qStr += trans.Transaction_Helper_VoucherType + "='"+shelper.VoucherType+"'";
    qStr += " group by " + trans.Transaction_Helper_VoucherNo;

    return db->getQueryModel(qStr);
}

QSqlQueryModel *VatReportDatabaseHelper::getB2BOutputModel(QDate fromDate, QDate toDate)
{
    QString qStr;
    qStr += " select (select ledger_name from ledger_master where Ledger_ID = Against_Ledger) as B, ";
    qStr += " concat('Output @', rate, char(37)) T , sum(Taxable), sum(CGST), sum(SGST), sum(IGST), ";
    qStr += " sum(Taxable)+ sum(CGST)+ sum(SGST)+ sum(IGST) as 'Total' from ( ";
    qStr += " SELECT Ledger, Against_Ledger, (case when Ledger_ID like '%SV%' ";
    qStr += " then cast(substr(Ledger, instr(Ledger, '@')+2, instr(Ledger, char(37))-instr(Ledger, '@')-2) as decimal(10,1)) ";
    qStr += " else cast(substr(Ledger, instr(Ledger, '@')+2, instr(Ledger, char(37))-instr(Ledger, '@')-2) as decimal(10,1))*2 end) as rate, ";
    qStr += " (case when Ledger_ID like '%SV%'  then amount else 0 end) as Taxable, ";
    qStr += "                 (case when Ledger_ID like '%SG%' then amount else 0 end) as CGST, ";
    qStr += "                 (case when Ledger_ID like '%CG%' then amount else 0 end) as SGST, ";
    qStr += "                 (case when Ledger_ID like '%IG%' then amount else 0 end) as IGST FROM ( ";
    qStr += " SELECT led.ledger_id,Ledger_Name as Ledger, trans.against_ledger, ";
    qStr += " ABS(SUM(Credit_Amount-Debit_Amount)) as amount";
    qStr += " From ledger_master led left outer join Transaction_Helper trans ";
    qStr += " on led.LEDGER_ID = trans.Ledger_ID ";
    qStr += " and Voucher_Date >=str_to_date('2020-01-01','%Y-%m-%d %H:%i:%s') ";
    qStr += " AND Voucher_Date <=str_to_date('2020-12-01','%Y-%m-%d %H:%i:%s') ";
    qStr += " where (led.ledger_id like '%SV%' or led.Ledger_ID like '%OSG%' or ";
    qStr += " led.Ledger_ID like '%OCG%' or led.Ledger_ID like '%OIG%')";
    qStr += " and Against_Ledger in (select Ledger_ID from ledger_master where trn is not null) ";
    qStr += " group by led.ledger_id, trans.against_ledger )A ";
    qStr += " ) B group by rate,  against_ledger";
    qStr += " order by against_ledger, rate";


}

float VatReportDatabaseHelper::getSales5Total(QString billsFrom, QString billsTo)
{
    LedgerMasterDatabaseHelper led;
    TransactionHelper trans;
    salesVoucherDatabaseHelper shelper;

    QString ledID = "0x7x5";
    QString qStr;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    float value =0;

    qStr += "SELECT SUM("+trans.Transaction_Helper_Credit_Amount+"-"+trans.Transaction_Helper_Debit_Amount+")";
    qStr += " From "+trans.Transaction_Helper_TableName + " where ";
    qStr += trans.Transaction_Helper_LedgerID + "='"+ledID+"'";
    qStr += " and ";
    qStr += "Cast("+trans.Transaction_Helper_VoucherNo + " as UNSIGNED) >= CAST("+billsFrom +" AS UNSIGNED)" ;
    qStr += " and ";
    qStr += "Cast("+trans.Transaction_Helper_VoucherNo + " as UNSIGNED) <= CAST("+billsTo +" AS UNSIGNED)" ;
    qStr += " and ";
    qStr += trans.Transaction_Helper_VoucherType + "='"+shelper.VoucherType+"'";


    query.prepare(qStr);
    if(query.exec()){
        while(query.next()){
            value = query.value(0).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return value;
}

float VatReportDatabaseHelper::getPurchaseTotal(QDate fromDate, QDate toDate)
{
    PurchaseVoucherDatabaseHelper shelper;
    LedgerMasterDatabaseHelper led;


    QString qStr;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    float value =0;

    qStr ="SELECT SUM("+shelper.Purchase_Invoice_Main_Total+") FROM ";
    qStr += shelper.Purchase_Invoice_Main_TableName + " WHERE ";
    qStr += shelper.Purchase_Invoice_Main_Voucher_Date + ">='"+ fromDate.toString("yyyy-MM-dd")+"'";
    qStr += " and ";
    qStr += shelper.Purchase_Invoice_Main_Voucher_Date + "<='"+ toDate.toString("yyyy-MM-dd")+"'";


    query.prepare(qStr);
    if(query.exec()){
        while(query.next()){
            value = query.value(0).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return value;
}

float VatReportDatabaseHelper::inputVatTotal(QDate fromDate, QDate toDate)
{
    LedgerMasterDatabaseHelper led;
    TransactionHelper trans;
    PurchaseVoucherDatabaseHelper shelper;


    QString ledID="0x2x14x7";
    QString qStr;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    float value =0;

    qStr += "SELECT SUM("+trans.Transaction_Helper_Debit_Amount+"-"+trans.Transaction_Helper_Credit_Amount+")";
    qStr += " From "+trans.Transaction_Helper_TableName + " where ";
    qStr += trans.Transaction_Helper_LedgerID + "='"+ledID+"'";
    qStr += " and ";
    qStr += trans.Transaction_Helper_VoucherDate + ">= '"+fromDate.toString("yyyy-MM-dd") +"'";
    qStr += " and ";
    qStr += trans.Transaction_Helper_VoucherDate + "<= '"+toDate.toString("yyyy-MM-dd") +"'";


    query.prepare(qStr);
    if(query.exec()){
        while(query.next()){
            value = query.value(0).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return value;
}

float VatReportDatabaseHelper::getPurchase5Total(QDate fromDate, QDate toDate)
{
    LedgerMasterDatabaseHelper led;
    TransactionHelper trans;
    PurchaseVoucherDatabaseHelper shelper;


    QString ledID="0x8x8";
    QString qStr;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    float value =0;

    qStr += "SELECT SUM("+trans.Transaction_Helper_Debit_Amount+"-"+trans.Transaction_Helper_Credit_Amount+")";
    qStr += " From "+trans.Transaction_Helper_TableName + " where ";
    qStr += trans.Transaction_Helper_LedgerID + "='"+ledID+"'";
    qStr += " and ";
    qStr += trans.Transaction_Helper_VoucherDate + ">= '"+fromDate.toString("yyyy-MM-dd") +"'";
    qStr += " and ";
    qStr += trans.Transaction_Helper_VoucherDate + "<= '"+toDate.toString("yyyy-MM-dd") +"'";


    query.prepare(qStr);
    if(query.exec()){
        while(query.next()){
            value = query.value(0).toFloat();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    return value;
}

