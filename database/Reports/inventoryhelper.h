#ifndef INVENTORYHELPER_H
#define INVENTORYHELPER_H

#include "database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h"
#include "datamodels/transaction/generalvoucherdataobject.h"
#include "database/finance/ItemTransaction/itemtransactiondatabasehelper.h"
#include "datamodels/Counter/countersettingsdatamodel.h"
#include <QSqlQueryModel>
#include "database/finance/salesvoucher/salesvoucherdatabasehelper.h"
#include "database/finance/purchasevoucher/purchasevoucherdatabasehelper.h"
#include "database/inventory/salesorder/salesorderdatabasehelper.h"
#include "database/inventory/workorder/workorderdatabasehelper.h"

class InventoryHelper
{
public:
    InventoryHelper();
    DatabaseHelper* db;

    float getClosingStockofItemByDate(QString itemID,QDate date);
    QSqlQueryModel *getItemWiseSalesByDateModel(QDateTime fromDate,QDateTime toDate);
    QSqlQueryModel *getItemWiseSalesWithGroupByDateModel(QDateTime fromDate,QDateTime toDate);
    QSqlQueryModel *getItemWiseSalesWithGroupAndModeByDateModel(QDateTime fromDate,QDateTime toDate);

    QSqlQueryModel *getItemWiseWaiterWise(QDateTime fromDate,QDateTime toDate,int SalesmanID);

    QSqlQueryModel *getReOrderLevelReport(QDate  dateTo);



    QSqlQueryModel *getClosingForRestaurant(QDateTime fromDate,QDateTime toDate); 

    QSqlQueryModel *getSalesMarginReport(QDateTime fromDate,QDateTime toDate);

    QSqlQueryModel *getLedgerwiseItemwise(QString itemID, QDateTime fromDate,QDateTime toDate);

    QSqlQueryModel *getLedgerwiseItemwiseDetailed(QString itemID, QDateTime fromDate, QDateTime toDate);


    QSqlQueryModel *getAccGroupwiseItemMovement(QString groupID, QString godownId, QDateTime fromDate, QDateTime toDate);

    QSqlQueryModel *getItemTransaction(QString itemID, QDateTime fromDate, QDateTime toDate);


    QSqlQueryModel *getNegativeStockList(QDateTime toDate, int limit=9999999);

    QSqlQueryModel *getItemWisePurchaseByDateModel(QDateTime fromDate, QDateTime toDate);
    QSqlQueryModel *getGroupWiseSalesDetailsByGroup(QString groupID, QDateTime fromDate, QDateTime toDate, QString godownId = "%");
    QSqlQueryModel *getGroupwiseSalesByModeOfService(QString groupID, QDateTime fromDate, QDateTime toDate);
    float getPurchaseCostAverageByItem(QString itemID);

    QSqlQueryModel *getIngredientsByWorkOrders(QStringList voucherList);
    QSqlQueryModel *getWastageQueryByDate(QDateTime dfrom, QDateTime dto);
    QSqlQueryModel *getWastageQueryByDateVDATE(QDateTime dfrom, QDateTime dto, int limit=999999);

    QSqlQueryModel *getComplimentarySalesForMail(QDateTime fromDate, QDateTime toDate);

    QSqlQueryModel *getItemWisePurchaseOrderByDateModel(QDateTime fromDate, QDateTime toDate);

    QSqlQueryModel *getItemwiseDatewiseTransactionsModel(QDateTime fromDate, QDateTime toDate);

    QSqlQueryModel *getItemwiseTransactionSummaryByVoucherType(QDateTime fromDate, QDateTime toDate, int voucherType);

    QSqlQueryModel *getProductionRateModel(QDate fromDate, QDate toDate);
    QSqlQueryModel *getConsumptionModel(QDate fromDate, QDate toDate, int limit =5);
    QSqlQueryModel *getItemwiseSalesByDateAndPoc(QDateTime fromDate, QDateTime toDate, QString PhNo);
    QSqlQueryModel *getDistinctCustomersPOC(QDate from, QDate to);
    QSqlQueryModel *getDatewiseSalesByGroup(QString groupID, QDateTime fromDate, QDateTime toDate, int Interval=1);
    QSqlQueryModel *getDatewisePurchaseByGroup(QString groupID, QDateTime fromDate, QDateTime toDate, int Interval=1);
    QSqlQueryModel *getGroupWisePurchaseDetailsByGroup(QString groupID, QDateTime fromDate, QDateTime toDate);
    QSqlQueryModel *getDatewiseTransactionsByGroup(QString groupID, QDateTime fromDate, QDateTime toDate, int Interval=1);
    QSqlQueryModel *getAllTransactionsByGroup(QString groupID, QDateTime fromDate, QDateTime toDate);
    QSqlQueryModel *getDatewiseSalesByGroupAndModeOfService(QString groupID, QDateTime fromDate, QDateTime toDate, int Interval=1);
    QSqlQueryModel *getDetailedSalesByGroup(QString groupID, QDateTime fromDate, QDateTime toDate, QString godownId = "%");
    QSqlQueryModel *getDatewiseDetailedSalesByGroup(QString groupID, QDateTime fromDate, QDateTime toDate, int Interval=1);
    QSqlQueryModel *getDatewiseDetailedPurchaseByGroup(QString groupID, QDateTime fromDate, QDateTime toDate, int Interval=1);
    QSqlQueryModel *getDetailedPurchaseByGroup(QString groupID, QDateTime fromDate, QDateTime toDate);
    QSqlQueryModel *getGroupWiseSalesFilterByLedger(QString groupID, QDateTime fromDate, QDateTime toDate, QString LedgerId);
    QSqlQueryModel *getDetailedSalesByGroupFilterByLedger(QString groupID, QDateTime fromDate, QDateTime toDate, QString LedgerId);
    QSqlQueryModel *getDetailedGroupwiseSalesByModeOfService(QString groupID, QDateTime fromDate, QDateTime toDate);
    QSqlQueryModel *getGroupWisePurchaseFilterByLedger(QString groupID, QDateTime fromDate, QDateTime toDate, QString LedgerId);
    QSqlQueryModel *getDetailedPurchaseByGroupFilterByLedger(QString groupID, QDateTime fromDate, QDateTime toDate, QString LedgerId);
    QSqlQueryModel *getGroupwiseTransactionByVoucherType(QString voucherType, QString groupID, QDateTime fromDate, QDateTime toDate, QString ledgerID = "%");
    QSqlQueryModel *getDetailedTransactionByGroupByVoucherType(QString voucherType, QString groupID, QDateTime fromDate, QDateTime toDate, QString ledgerID = "%");
    QSqlQueryModel *getDatewiseTransactionByGroupByVoucherType(QString voucherType, QString groupID, QDateTime fromDate, QDateTime toDate,QString ledgerID = "%", int Interval=1);
    QSqlQueryModel *getDatewiseTransactionByVoucherTypeByGroupAndReason(QString voucherType, QString groupID, QDateTime fromDate, QDateTime toDate, QString LedgerID="%", int Interval=1);
    QSqlQueryModel *getDetailedTransactionByVoucherTypeAndReason(QString voucherType, QString groupID, QDateTime fromDate, QDateTime toDate, QString ledgerID = "%");
    QSqlQueryModel *getGroupwiseTransactionByVoucherTypeAndReason(QString voucherType, QString groupID, QDateTime fromDate, QDateTime toDate, QString ledgerID = "%");
    QSqlQueryModel *getDatewiseDetailedTransactionByVoucherType(QString voucherType, QString groupID, QDateTime fromDate, QDateTime toDate, QString ledgerID = "%", int Interval = 1);
    QSqlQueryModel *getItemwiseTimeBasedSales(QString itemID, QDateTime fromDate, QDateTime toDate);
    QSqlQueryModel *getItemCrDrBetDate(QString ItemID, QDate fromDate, QDate toDate, QString voucherType = "%", QString godown = "%");
    QSqlQueryModel *getTopCustomerByItem(QString itemID, QDate fromDate, QDate toDate, int limit = 5);
    QSqlQueryModel *getTopVendorByItem(QString itemID, QDate fromDate, QDate toDate, int limit = 5);
    QSqlQueryModel *getLastTransactionsByItem(QString itemID, int voucherType, QDate fromDate, QDate toDate, int limit =5);
    QSqlQueryModel *getMostSoldItems(QDate fromDate, QDate toDate, int limit=5);
    QSqlQueryModel *getMostBoughtItems(QDate fromDate, QDate toDate, int limit=5);
    QSqlQueryModel *getAveragePurchaseInterval(QString itemID, QDate fromDate, QDate toDate);
    QSqlQueryModel *getGodownwiseSalesModel(QDate fromDate, QDate toDate, QString godownId);
    QSqlQueryModel *getGroupwiseMovementByLedger(QString groupID, QString ledgerID, QDate fromDate, QDate toDate);
    QSqlQueryModel *getItemTransValueByFrequency(int voucherType, QDate fromDate, QDate toDate, QString LedgerId);
    QSqlQueryModel *getRankBySalesValue(QString itemId, QDate fromDate, QDate toDate);
    QString getSalesOrderByModeOfService( QDate fromDate, QDate toDate);

    QSqlQueryModel *getSalesOrderByTime(QDate fromDate, QDate toDate);
    QSqlQueryModel *getItemwiseDeliveryNoteModel(QDateTime fromDate, QDateTime toDate);
    QSqlQueryModel *getItemwiseDatewisePurchasePriceModel(QDateTime fromDate, QDateTime toDate);
    QSqlQueryModel *getGroupWiseWODetailsByGroup(QString groupID, QDateTime fromDate, QDateTime toDate, QString godownId = "%");
    QSqlQueryModel *getDatewiseWOByGroup(QString groupID, QDateTime fromDate, QDateTime toDate, int Interval = 1);
    QSqlQueryModel *getDatewiseDetailedWOByGroup(QString groupID, QDateTime fromDate, QDateTime toDate, int Interval = 1);
    QSqlQueryModel *getDetailedWOByGroup(QString groupID, QDateTime fromDate, QDateTime toDate);
    QSqlQueryModel *getGroupwiseMovementByGodown(QString groupID, QString godownId, QDate fromDate, QDate toDate);
    QSqlQueryModel *getItemwiseMovementByGodownAndGroup(QString groupID, QString godownId, QDate fromDate, QDate toDate);
    QSqlQueryModel *getItemwiseMovementByLedgerAndItemGroup(QString groupID, QString ledgerID, QDate fromDate, QDate toDate);
    QSqlQueryModel *getDetailedAccGroupwiseItemMovement(QString invGroupID, QString accGroupId, QDateTime fromDate, QDateTime toDate);
    QSqlQueryModel *getItemwiseReceiptNoteModel(QDateTime fromDate, QDateTime toDate);
};

#endif // INVENTORYHELPER_H
