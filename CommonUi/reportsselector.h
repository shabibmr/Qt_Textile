#ifndef REPORTSSELECTOR_H
#define REPORTSSELECTOR_H

#include <QWidget>
#include "counter/Reports/Restaurant/restaurantclosingstock.h"
#include "inventory/reports/GroupwiseReport/groupwisereport.h"
#include "inventory/reports/PurchaseReport/purchasereport.h"
#include "inventory/reports/DebitNoteReport/debitnotereport.h"
#include "inventory/reports/CreditNoteReport/creditnotereport.h"
#include "inventory/reports/WastageReport/wastagereport.h"
#include "inventory/transactions/ItemwiseSalesOrder/itemwisesodatewisereport.h"
#include "inventory/reports/MovementAnalysis/godownwiseitemmovement.h"
#include "inventory/reports/MovementAnalysis/groupwiseitemmovement.h"
#include "inventory/reports/MovementAnalysis/ledgerwiseitemmovement.h"
#include "Notifications/notificationslist.h"

#include "finance/reports/BankReconciliation/bankreconciliation.h"
#include "finance/masters/ledgers/ledgeropeningvalues.h"
#include "finance/reports/GSTReport/gstreport.h"
#include "counter/Reports/billwisereport.h"
#include "hr/attendanceeditor.h"
#include "finance/reports/ProfitAndLossReport/profitandlossreport.h"
#include "counter/Reports/shiftwisereport.h"
#include "counter/Reports/waiterwiseservicewisereport.h"

#include "dashboard/accountsmaindashboard.h"
#include "dashboard/customerdashboard.h"
#include "dashboard/inventoryitemdashboard.h"
#include "dashboard/salesmaindash.h"
#include "dashboard/purchasesmaindash.h"
#include "dashboard/receiptsmaindash.h"


#include <QMessageBox>

class ReportsSelector : public QWidget
{
    Q_OBJECT
public:
    ReportsSelector(int reportId, QWidget *parent = nullptr);

    int reportID=0;

    RestaurantClosingStock *StockReportWidget;
    PurchaseReport *PurchaseReportWidget;
    DebitNoteReport *DebitNoteReportWidget;
    CreditNoteReport *CreditNoteReportWidget;
    WastageReport *WastageReportWidget;
    NotificationsList *notificationWidget;
    BankReconciliation *bankReconciliationWidget;
    AttendanceEditor *attendanceEditor;
    LedgerOpeningValues *ledgerOpeningEditor;
    GSTReport *gstReportWidget;
    BillwiseReport *billwiseReportWidget;

public slots:
    void selectReport();

private slots:
    void openStockReport();
    void openGroupwiseSales();
    void openPurchaseReport();
    void openDebitNoteReport();
    void openCreditNoteReport();
    void openWastageReport();
    void openNotifications();
    void openBankReconciliation();
    void openAttendanceEditor();
    void openLedgerOpening();
    void openGSTReports();
    void openBillwiseReport();
    void openAccountsDashboard();
    void openItemsDashboard();
    void openCustomersDashboard();
    void openSalesDashboard();
    void openPurchaseDashboard();
    void openReceiptsDashboard();

    void openItemwiseOrderReport();
    void openGodownwiseItemMovement();
    void openGroupwiseItemMovement();
    void openLedgerwiseItemMovement();
    void openShiftwiseReport();
    void openSalesmanwiseServiceReport();

    void openTodaysBills();
    void openBalanceSheet();
    void openProfitLoss();



signals:
    void reportOpened(QWidget *, QString title)    ;
    void selectedVoucher(GeneralVoucherDataObject *voucher);
};

#endif // REPORTSSELECTOR_H
