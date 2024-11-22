#ifndef INVENTORYITEMDASHBOARD_H
#define INVENTORYITEMDASHBOARD_H

#include <QWidget>
#include <QHeaderView>
#include <QScrollArea>
#include "database/Reports/inventoryhelper.h"
#include "database/Reports/closingstockhelper.h"
#include "database/Reports/ledgerhelper.h"
#include "customwidgets/Basic/gmcardwidget.h"
#include "customwidgets/Basic/gmdateedit.h"
#include "customwidgets/SearchWidgets/Inventoryitem/gmitemsearchlite.h"
#include "customwidgets/Material/components/qtmaterialscrollbar.h"

namespace Ui {
class InventoryItemDashboard;
}

class InventoryItemDashboard : public QWidget
{
    Q_OBJECT

public:
    explicit InventoryItemDashboard(QWidget *parent = nullptr);
    ~InventoryItemDashboard();


    QHBoxLayout *filterLayout;
    QVBoxLayout *itemLayout;

    QVBoxLayout *allItemsLayout;

    SalesInventoryItemDatabaseHelper *itemHelper;
    SalesInventoryGroupsDatabaseHelper *grpHelper;
    InventoryHelper *dbHelper;
    ClosingStockHelper *stkHelper;

    inventoryItemDataModel item;

    LedgerMasterDataModel *ledger;
    QDate fromDate = QDate::currentDate();
    QDate toDate = QDate::currentDate();

    GMCardWidget *stockCard       ;
    GMCardWidget *lppCard         ;
    GMCardWidget *lspCard         ;
    GMCardWidget *godownStockCard ;
    GMCardWidget *salesMarginCard ;
    GMCardWidget *timeBasedSalesCard  ;
    GMCardWidget *topCustomersCard;
    GMCardWidget *rankCard;
    GMCardWidget *avgShelfPeriodCard;
    GMCardWidget *avgPurchIntervalCard;
    GMCardWidget *reorderLevelCard;
    GMCardWidget *salesStatisticsCard;
    GMCardWidget *purchaseStatisticsCard;
    GMCardWidget *salesRetStatisticsCard;
    GMCardWidget *purchaseRetStatisticsCard;
    GMCardWidget *lastSalesCard;
    GMCardWidget *lastPurchaseCard;

    QTableView *timeBasedTable;
    QTableView *godownWiseStockTable;
    QTableView *salesStatsTable;
    QTableView *purchaseStatsTable;
    QTableView *salesRetStatTable;
    QTableView *purchaseRetStatTable;
    QTableView *topCustomersTable;
    QTableView *lastSalesTable;
    QTableView *lastPurchaseTable;

    QLabel *stockLabel;
    QLabel *lppLabel;
    QLabel *lspLabel;
    QLabel *salesMarginLabel;
    QLabel *rankLabel;
    QLabel *avgShelfPeriodLabel;
    QLabel *avgPurchIntervalLabel;
    QLabel *reorderLevelLabel;

    QString bgcolor = "#ffffff";
    QString textColor = "#006064";
    QString iconColor = "#00bcd4";
    QString tableViewSS = "QTableView::item{"
                          "gridline-color : #ffffff;"
                          "border: 0px solid #fffff8;"
                          "}"
                          "QTableView::item:selected{"
                          "color:#00acc1;"
                          "background-color:" + bgcolor + ";"
                          "}"
                          "QTableView{"
                          "background-color: #ffffff;"
                          "color: #006064;"
                          "border : none;"
                          "/*selection-background-color: #999*/}"
                          "QHeaderView::section:horizontal"
                          "{"
                          "background-color: #006064;"
                          "color: white;"
                          "border: 1px solid #ffffff;"
                           "}";



    void initDbHelpers();

    void setWidgets();
    void setAllItemsLayout();
    void setFilterLayout();
    void setItemLayout();

private slots:
    void setValues();


private:
    Ui::InventoryItemDashboard *ui;
};

#endif // INVENTORYITEMDASHBOARD_H
