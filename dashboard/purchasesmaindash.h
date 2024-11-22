#ifndef PURCHASESMAINDASH_H
#define PURCHASESMAINDASH_H

#include <QWidget>
#include <QTableView>

#include "customwidgets/Basic/gmcardwidget.h"
#include "customwidgets/Basic/gmdateedit.h"
#include "database/Reports/ledgerhelper.h"
#include "database/Reports/inventoryhelper.h"


namespace Ui {
class PurchasesMainDash;
}

class PurchasesMainDash : public QWidget
{
    Q_OBJECT

public:
    explicit PurchasesMainDash(QWidget *parent = nullptr);
    ~PurchasesMainDash();

    LedgerHelper *ledHelper;
    InventoryHelper *invHelper;

    QHBoxLayout *filterLayout;

    GMCardWidget *pendingPurchaseCard;
    GMCardWidget *deliveryExpectedCard;
    GMCardWidget *negativeStockCard;
    GMCardWidget *paymentPendingCard;
    GMCardWidget *topVendorsCard;
    GMCardWidget *purchaseStatsCard;
    GMCardWidget *totalPurchaseValueCard;
    GMCardWidget *lastPurchasesCard;
    GMCardWidget *mostBoughtItems;

    QTableView *pendingPurchaseTable;
    QTableView *deliveryExpectedTable;
    QTableView *negativeStockTable;
    QTableView *paymentPendingTable;
    QTableView *topVendorsTable;
    QTableView *purchaseStatsTable;
    QLabel *totalPurchaseValueTable;
    QTableView *lastPurchasesTable;
    QTableView *mostBoughtItemsTable;


    QDate fromDate = QDate::currentDate();
    QDate toDate = QDate::currentDate();

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

    void setWidgets();
    void setFilterLayout();

private slots:
    void setValues();

private:
    Ui::PurchasesMainDash *ui;
};

#endif // PURCHASESMAINDASH_H
