#ifndef ACCOUNTINGREPORTDATABASEHELPER_H
#define ACCOUNTINGREPORTDATABASEHELPER_H

#include <QSqlQueryModel>
#include "database/databasehelper/databasehelper.h"
#include "database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h"
#include "database/finance/TransactionHelper/transactionhelper.h"
#include "database/finance/ItemTransaction/itemtransactiondatabasehelper.h"


class AccountingReportDatabaseHelper
{
public:
    AccountingReportDatabaseHelper();

    DatabaseHelper* db;
    LedgerMasterDatabaseHelper led;
    AccountGroupMasterDatabaseHelper acc;
    QSqlQueryModel *getBalanceForAllLedgers(QDate fromDate, QDate toDate);
    QSqlQueryModel *getTrialBalance(QDate date);
    QSqlQueryModel *getBalanceByGroupType(QString groupType, QDate fromDate, QDate toDate);
    QSqlQueryModel *getLedgerBalanceByGroup(QString groupID, QDate fromDate, QDate toDate);
    float getStockValueByDate(QDate dateTo);
    float getBalanceByGroup(QString groupID, QDate fromDate, QDate toDate);
    float getProfitAndLossValue(QDate fromDate, QDate toDate);
    float getOpeningBalanceDifference(QDate fromDate, QDate toDate);
    float getRestaurantStockValueByDate(QDate dateTo);
};

#endif // ACCOUNTINGREPORTDATABASEHELPER_H
