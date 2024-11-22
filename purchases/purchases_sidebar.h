#ifndef PURCHASES_SIDEBAR_H
#define PURCHASES_SIDEBAR_H

#include <QWidget>
#include <QToolButton>
#include <customwidgets/defaultdashboard.h>
#include "customwidgets/section.h"
#include <QDebug>
#include "finance/masters/ledgers/ledgermaster.h"
#include "inventory/transactions/purchaseorders/purchaseorders.h"
#include "finance/transactions/creditnote/creditnote.h"
#include "finance/transactions/payments/payment.h"
#include "finance/transactions/purchases/purchaseentry.h"

#include "purchases/reports/PurchaseOrdersPending/purchaseorderspending.h"
#include "inventory/reports/BelowReorderLevel/belowreorderlevel.h"
#include "finance/reports/LedgerReport/ledgerreport.h"
#include "purchases/reports/PurchaseRegister/purchaseregister.h"
#include "purchases/reports/PurchaseOrderBook/purchaseorderbook.h"
#include "inventory/reports/StockReport/stockreport.h"
#include "purchases/reports/PaymentOutstanding/paymentoutstanding.h"
#include "purchases/reports/ItemsReserved/itemsreserved.h"
#include "purchases/reports/CostEstimates/costestimates.h"
#include "counter/Reports/Restaurant/restaurantclosingstock.h"

namespace Ui {
class Purchases_SideBar;
}

class Purchases_SideBar : public QWidget
{
    Q_OBJECT

public:
    explicit Purchases_SideBar(QWidget *parent = 0);
    ~Purchases_SideBar();
    DefaultDashBoard *dash;
    Section* sectionReports ;

    QToolButton* PurchaseOrders;
    QToolButton* Vendors;
    QToolButton* PurchaseVouchers;
    QToolButton* Payments;
    QToolButton* CreditNotes;

    QToolButton* PurchaseOrdersPendingButton;
    QToolButton* ReorderStatusButton;
    QToolButton* LedgerReportButton;
    QToolButton* PurchaseRegisterButton;
    QToolButton* PurchaseOrderBookButton;
    QToolButton* StockReportButton;
    QToolButton* PaymentOutstandingButton;
    QToolButton* ReservedItemsButton;
    QToolButton* CostEstimatesButton;

    ledgerMaster* ledgerMasterWidget;
    purchaseOrders* purchaseOrdersWidget;
    creditNote* creditNoteWidget;
    Payment* PaymentWidget;
    PurchaseEntry* PurchaseEntryWidget;

    PurchaseOrdersPending* PurchaseOrdersPendingWidget;
    ReOrderLevel* ReorderStatusWidget;
    LedgerReport* LedgerReportWidget;
    PurchaseRegister* PurchaseRegisterWidget;
    PurchaseOrderBook* PurchaseOrderBookWidget;
    RestaurantClosingStock* StockReportWidget;
    PaymentOutstanding* PaymentOutstandingWidget;
    ItemsReserved* ItemsReservedWidget;
    CostEstimates* CostEstimatesWidget;

private:
    Ui::Purchases_SideBar *ui;

public slots:
    void clearLayout();
    void resetColors();

    void PurchaseOrdersClicked();
    void VendorsClicked();
    void PurchaseVouchersClicked();
    void PaymentsClicked();
    void CreditNotesClicked();

    void PurchaseOrdersPendingClicked();
    void ReorderStatusClicked();
    void LedgerReportClicked();
    void PurchaseRegisterClicked();
    void PurchaseOrderBookClicked();
    void StockReportClicked();
    void PaymentsOutstandingClicked();
    void ReservedItemsClicked();
    void CostEstimatesClicked();
};

#endif // PURCHASES_SIDEBAR_H
