#ifndef CLOSINGSTOCKHELPER_H
#define CLOSINGSTOCKHELPER_H

#include "database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h"
#include "database/finance/ItemTransaction/itemtransactiondatabasehelper.h"
#include "database/finance/purchasevoucher/purchasevoucherdatabasehelper.h"
#include "login/loginvalues.h"
#include "database/inventory/godown/godowndatabasehelper.h"


class ClosingStockHelper
{
public:
    ClosingStockHelper();

    DatabaseHelper *db;

    float getClosingStockOfItemTillDateWithBom(QString itemID, QDate date, bool bomFlag=false);
    float getClosingStockOfItemTillDate(QString itemID, QDate date);
    QSqlQueryModel *getClosingStockListModelWithBom(QDate dateTo);
    QSqlQueryModel *getClosingStockByGodown(QDate fromDate, QDate toDate, QString godownID);


    // Stock of all items regardless of group or godown

    QSqlQueryModel *getClosingStockListModelWithStdCost(QDate fromDate, QDate dateTo, bool bomFlag=  LoginValues::company.useBomForStockCalc);
    QSqlQueryModel *getClosingStockListModelWithLastPurchasePrice(QDate fromDate, QDate dateTo, bool bomFlag=  LoginValues::company.useBomForStockCalc);
    QSqlQueryModel *getClosingStockListModelWithFifoPriceAndBom(QDate fromDate, QDate dateTo, bool bomFlag=  LoginValues::company.useBomForStockCalc);

    // Stock of all groups under given group --> drill down to item ==> no godown

    QSqlQueryModel *getClsStkByGroupWithLastPurchPriceAndBomSummary(QString GroupId, QDate fromDate, QDate dateTo, bool bomFlag= LoginValues::company.useBomForStockCalc);
    QSqlQueryModel *getClsStkByGroupWithStdCostAndBomSummary(QString GroupId, QDate fromDate, QDate dateTo, bool bomFlag =  LoginValues::company.useBomForStockCalc);
    QSqlQueryModel *getClsStkByGroupWithFifoPriceAndBomSummary(QString GroupId, QDate fromDate, QDate dateTo, bool bomFlag=  LoginValues::company.useBomForStockCalc);

    // stock of all items under group hierarchy

    QSqlQueryModel *getClsStkByGroupWithStdCostAndBomDetailed(QString GroupId, QDate fromDate, QDate dateTo, bool bomFlag=  LoginValues::company.useBomForStockCalc);
    QSqlQueryModel *getClsStkByGroupWithLastPurchPriceAndBomDetailed(QString GroupId, QDate fromDate, QDate dateTo, bool bomFlag=  LoginValues::company.useBomForStockCalc);
    QSqlQueryModel *getClsStkByGroupWithFifoPriceAndBomDetailed(QString GroupId, QDate fromDate, QDate dateTo, bool bomFlag=  LoginValues::company.useBomForStockCalc);

    // use below -- stock of group (can be drilled down) in godown (All godowns if no input)

    QSqlQueryModel *getGroupwiseClosingStockByGodownAndLPP(QDate fromDate, QDate toDate, QString godownID, QString groupID, bool show0s, bool bomFlag=  LoginValues::company.useBomForStockCalc, bool showTransWithinPeriod = true, bool showGodownTransfers = true, bool showVarianceVal =false);
    QSqlQueryModel *getGroupwiseClosingStockByGodownAndStdCost(QDate fromDate, QDate dateTo, QString godownID, QString GroupId, bool show0s, bool bomFlag=  LoginValues::company.useBomForStockCalc);
    QSqlQueryModel *getGroupwiseClosingStockByGodownAndFifoPrice(QDate fromDate, QDate dateTo, QString godownID, QString GroupId, bool show0s, bool bomFlag=  LoginValues::company.useBomForStockCalc);
    QSqlQueryModel *getItemwiseClosingStockOfGrpByGodownAndLPP(QDate fromDate, QDate dateTo, QString godownID, QString GroupId, bool show0s, bool showTransWithinPeriod = true, bool showGodownTransfers = false, bool bomFlag=  LoginValues::company.useBomForStockCalc, bool showVariance =false);
    QSqlQueryModel *getItemwiseClosingStockOfGrpByGodownAndStdCost(QDate fromDate, QDate dateTo, QString godownID, QString GroupId, bool show0s, bool bomFlag=  LoginValues::company.useBomForStockCalc);
    QSqlQueryModel *getItemwiseClosingStockOfGrpByGodownAndFifoPrice(QDate fromDate, QDate dateTo, QString godownID, QString GroupId, bool show0s, bool bomFlag=  LoginValues::company.useBomForStockCalc);

    QSqlQueryModel *getGodownwiseStockOfItem(QString itemId, QDate toDate);
};

#endif // CLOSINGSTOCKHELPER_H
