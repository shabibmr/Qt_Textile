#include "ledgerhelpermt.h"

LedgerHelperMT::LedgerHelperMT(QObject *parent) : QObject(parent)
{

}


void  *LedgerHelperMT::getLedgerTransValueByFrequency(int voucherType, QDate fromDate, QDate toDate, QString LedgerId)
{
    db = QSqlDatabase::addDatabase("QMYSQL", QString::number( QDateTime::currentDateTime().toSecsSinceEpoch()+voucherType));


    db.setDatabaseName(DatabaseValues::dbName);
    db.setHostName(DatabaseValues::dbHost);
    db.setPort(DatabaseValues::dbPort);
    db.setPassword(DatabaseValues::dbPassword);
    db.setUserName(DatabaseValues::dbUser);
    db.open();

    QString tableName = GMVoucherTypes::getVoucherMainTablename(voucherType);
    qDebug()<<Q_FUNC_INFO<<tableName;
    QString qStr = " SELECT 'Daily' as Period, ";
    qStr += " (SELECT round(COALESCE(SUM(Total),0),2) as 'Todays Sales' FROM " + tableName ;
    qStr += " WHERE Ledger_ID like '" + LedgerId + "' AND Voucher_Date = CURRENT_DATE) as 'Current Total', ";
    qStr += " (SELECT COALESCE(round(SUM(Total)/Count(*),2),0) as 'Daily Average Sales' ";
    qStr += " FROM ( SELECT COALESCE(SUM(Total),0) as Total FROM " + tableName ;
    qStr += " where Ledger_ID like '" + LedgerId + "' ";
    qStr += " AND Voucher_Date >= " + DatabaseHelper:: getDateStringStatic(fromDate);
    qStr += " AND Voucher_Date <= " + DatabaseHelper:: getDateStringStatic(toDate);
    qStr += " group by Voucher_Date )A) as 'Average'";
    qStr += " UNION ";
    qStr += " SELECT 'Week' , (SELECT round(COALESCE(SUM(Total),0),2) as WeeklySum ";
    qStr += " FROM " + tableName + " where  Ledger_ID like '" + LedgerId + "' ";
    qStr += " AND Voucher_Date >= subDate(CURRENT_DATE, DayOfWeek(CURRENT_DATE)) ";
    qStr += " AND Voucher_Date <= CURRENT_DATE) as 'CurrentTotal', ";
    qStr += " (SELECT COALESCE(round(SUM(WeeklySum)/COUNT(*),2),0) as WeeklyAvg ";
    qStr += " FROM (SELECT SUM(Total) as WeeklySum, week(Voucher_Date) ";
    qStr += " FROM " + tableName + " where Ledger_ID like '" + LedgerId + "' ";
    qStr += " AND Voucher_Date >= " + DatabaseHelper:: getDateStringStatic(fromDate);
    qStr += " AND Voucher_Date <= " + DatabaseHelper:: getDateStringStatic(toDate);
    qStr += " group by YEARWEEK(Voucher_Date) ) A ) as 'Average'";
    qStr += " UNION ";
    qStr += " SELECT 'Month', (SELECT round(COALESCE(SUM(Total),0),2) as MonthSum ";
    qStr += " FROM " + tableName + " where  Ledger_ID like '" + LedgerId + "' ";
    qStr += " AND Voucher_Date >= DATE_FORMAT(NOW() ,'%Y-%m-01') ";
    qStr += " AND Voucher_Date <= CURRENT_DATE) as 'MonthTotal', ";
    qStr += " (SELECT COALESCE(round(SUM(MonthSum)/COUNT(*),2),0) as WeeklyAvg ";
    qStr += " FROM (SELECT SUM(Total) as MonthSum FROM " + tableName ;
    qStr += " where Ledger_ID like '" + LedgerId + "' ";
    qStr += " AND Voucher_Date >= " +DatabaseHelper:: getDateStringStatic(fromDate);
    qStr += " AND Voucher_Date <= " +DatabaseHelper:: getDateStringStatic(toDate);
    qStr += " group by MONTH(Voucher_Date) ) A ) as 'Average' ";
    qStr += " UNION ";
    qStr += " SELECT 'Quarter', (SELECT round(COALESCE(SUM(Total),0),2) as QuarterSum ";
    qStr += " FROM " + tableName + " where  Ledger_ID like '" + LedgerId + "' ";
    qStr += " AND Voucher_Date >= MAKEDATE(YEAR(CURDATE()), 1) + INTERVAL QUARTER(CURDATE()) QUARTER";
    qStr += " - INTERVAL 1 QUARTER AND Voucher_Date <= CURRENT_DATE) as 'QuarterTotal', ";
    qStr += " (SELECT COALESCE(round(SUM(QuarterSum)/COUNT(*),2),0) as WeeklyAvg ";
    qStr += " FROM (SELECT SUM(Total) as QuarterSum FROM " + tableName ;
    qStr += " where Ledger_ID like '" + LedgerId + "' ";
    qStr += " AND Voucher_Date >= " + DatabaseHelper:: getDateStringStatic(fromDate);
    qStr += " AND Voucher_Date <= " + DatabaseHelper:: getDateStringStatic(toDate);
    qStr += " group by QUARTER(Voucher_Date) ) A ) as 'Average' ";
    qStr += " UNION ";
    qStr += " SELECT 'Year', (SELECT round(COALESCE(SUM(Total),0),2) as AnnualSum ";
    qStr += " FROM " + tableName + " where  Ledger_ID like '" + LedgerId + "' ";
    qStr += " AND Voucher_Date >= DATE_FORMAT(NOW() ,'%Y-01-01') ";
    qStr += " AND Voucher_Date <= CURRENT_DATE) as 'AnnualTotal', ";
    qStr += " (SELECT COALESCE(round(SUM(AnnualSum)/COUNT(*),2),0) as AnnualAvg ";
    qStr += " FROM (SELECT SUM(Total) as AnnualSum FROM " + tableName ;
    qStr += " where Ledger_ID like '" + LedgerId + "' ";
    qStr += " AND Voucher_Date >= " +DatabaseHelper:: getDateStringStatic(fromDate);
    qStr += " AND Voucher_Date <= " + DatabaseHelper:: getDateStringStatic(toDate);
    qStr += " group by YEAR(Voucher_Date) ) A ) as 'Average' ";

//    qDebug()<<qStr;

    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery(qStr,db);
    if (model->lastError().isValid()){
        qDebug()<<"&&&&&&&&&&"<<qStr<<"&&&&&&&&&&&&&&&&&&&&&&&&&&&";
        qDebug() << model->lastError();
    }

    db.close();
    emit readComplete(model);
    db.close();
}


void *LedgerHelperMT::getQueryModelofQueryString(QString qStr)
{

    db = QSqlDatabase::addDatabase("QMYSQL", QString::number(QDateTime::currentDateTime().toSecsSinceEpoch())+"x");
    db.setDatabaseName(DatabaseValues::dbName);
    db.setDatabaseName(DatabaseValues::dbName);
    db.setHostName(DatabaseValues::dbHost);
    db.setPort(DatabaseValues::dbPort);
    db.setPassword(DatabaseValues::dbPassword);
    db.setUserName(DatabaseValues::dbUser);
    db.open();

    qDebug()<<qStr;

    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery(qStr,db);
    if (model->lastError().isValid()){
        qDebug()<<"&&&&&&&&&&"<<qStr<<"&&&&&&&&&&&&&&&&&&&&&&&&&&&";
        qDebug() << model->lastError();
    }
    emit readComplete(model);

}
