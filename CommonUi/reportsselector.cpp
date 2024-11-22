#include "reportsselector.h"

#include "counter/Reports/Restaurant/restaurantclosingstock.h"
#include "counter/Reports/todaysbills.h"
#include "inventory/reports/PurchaseReport/purchasereport.h"
#include "inventory/reports/DebitNoteReport/debitnotereport.h"
#include "inventory/reports/CreditNoteReport/creditnotereport.h"
#include "inventory/reports/WastageReport/wastagereport.h"
#include "inventory/transactions/ItemwiseSalesOrder/itemwisesodatewisereport.h"
#include "Notifications/notificationslist.h"

#include "finance/reports/BankReconciliation/bankreconciliation.h"
#include "finance/masters/ledgers/ledgeropeningvalues.h"
#include "finance/reports/GSTReport/gstreport.h"
#include "counter/Reports/billwisereport.h"
#include "hr/attendanceeditor.h"
#include "finance/reports/BalanceSheet/balancesheet.h"

#include "dashboard/accountsmaindashboard.h"
#include "dashboard/customerdashboard.h"
#include "dashboard/inventoryitemdashboard.h"
#include "dashboard/salesmaindash.h"
#include "dashboard/purchasesmaindash.h"
#include "dashboard/receiptsmaindash.h"

ReportsSelector::ReportsSelector(int reportId, QWidget *parent) : QWidget(parent)
{
    this->reportID = reportId;

    qDebug()<<Q_FUNC_INFO<<__LINE__<<reportId;
}

void ReportsSelector::selectReport()
{
    switch (reportID) {

    case 1000:{
        openStockReport();
        break;
    }
    case 1001:{

        openDebitNoteReport();
        break;
    }
    case 1002:{

        openCreditNoteReport();
        break;
    }
    case 1003:{

        openWastageReport();
        break;
    }
    case 1004:{

        openPurchaseReport();
        break;
    }
    case 1005:{
        openNotifications();
        break;
    }
    case 1006:{
        openBankReconciliation();
        break;
    }
    case 1007:{
        openAttendanceEditor();
        break;
    }
    case 1008:{
        openLedgerOpening();
        break;
    }
    case 1009:{
        openGSTReports();
        break;
    }
    case 1010:{
        openBillwiseReport();
        break;
    }
    case 1011:{
        openAccountsDashboard();
        break;
    }
    case 1012:{
        openItemsDashboard();
        break;
    }
    case 1013:{
        openCustomersDashboard();
        break;
    }
    case 1014:{
        openSalesDashboard();
        break;
    }
    case 1015:{
        openPurchaseDashboard();
        break;
    }
    case 1016:{
        openReceiptsDashboard();
        break;
    }
    case 1017:{
        openItemwiseOrderReport();
        break;
    }
    case 1018:{
        openGodownwiseItemMovement();
        break;
    }
    case 1019:{
        openLedgerwiseItemMovement();
        break;
    }
    case 1020:{
        openGroupwiseItemMovement();
        break;
    }
    case 1021:{
        openGroupwiseSales();
        break;
    }
    case 1022:{
        openTodaysBills();
        break;
    }

    case 1025:{
        openBalanceSheet();
        break;
    }

    case 1026:{
        openProfitLoss();
        break;
    }
    case 1027:{
        openShiftwiseReport();
        break;
    }
    case 1028:{
        openSalesmanwiseServiceReport();
        break;
    }


    default:
    {
        QMessageBox box; box.setText("Report not found!"); box.exec();
        break;
    }
    }
}

void ReportsSelector::openStockReport()
{
    RestaurantClosingStock *StockReportWidget = new RestaurantClosingStock(this);
    emit reportOpened(StockReportWidget, "Stock Report");
}

void ReportsSelector::openGroupwiseSales()
{
    GroupwiseReport *GroupwiseReportWidget = new GroupwiseReport(this);
    emit reportOpened(GroupwiseReportWidget, "Groupwise Sales Report");
}

void ReportsSelector::openPurchaseReport()
{
    PurchaseReport *PurchaseReportWidget = new PurchaseReport(this);
    emit reportOpened(PurchaseReportWidget, "Purchase Report");

}

void ReportsSelector::openDebitNoteReport()
{
    DebitNoteReport *DebitNoteReportWidget = new DebitNoteReport(this);
    emit reportOpened(DebitNoteReportWidget, "Debit Note Report");
}

void ReportsSelector::openCreditNoteReport()
{
    CreditNoteReport *CreditNoteReportWidget = new CreditNoteReport(this);
    emit reportOpened(CreditNoteReportWidget, "Credit Note Report");
}

void ReportsSelector::openWastageReport()
{
    WastageReport *WastageReportWidget = new WastageReport(this);
    emit reportOpened(WastageReportWidget, "Wastage Report");
}

void ReportsSelector::openNotifications()
{
    NotificationsList *notificationWidget = new NotificationsList(this);
    emit reportOpened(notificationWidget, "Notifications");
}

void ReportsSelector::openBankReconciliation()
{
    BankReconciliation *bankReconciliationWidget = new BankReconciliation(this);
    emit reportOpened(bankReconciliationWidget, "Bank Reconciliation");
}

void ReportsSelector::openAttendanceEditor()
{
    AttendanceEditor *attendanceEditor = new AttendanceEditor(this);
    emit reportOpened(attendanceEditor, "Attendance Editor");
}

void ReportsSelector::openLedgerOpening()
{
    LedgerOpeningValues *ledgerOpeningEditor = new LedgerOpeningValues(this);
    emit reportOpened(ledgerOpeningEditor, "Ledger Opening");
}

void ReportsSelector::openGSTReports()
{
    GSTReport* gstReportWidget = new GSTReport();
    emit reportOpened(gstReportWidget, "GST Reports");
}

void ReportsSelector::openBillwiseReport()
{
    BillwiseReport *billwiseReportWidget = new BillwiseReport(this);

    QObject::connect(billwiseReportWidget,&BillwiseReport::selectedVoucher,
                     this,[=](GeneralVoucherDataObject *voucher){
        emit selectedVoucher(voucher);
    });
    emit reportOpened(billwiseReportWidget, "Billwise Report");
}

void ReportsSelector::openAccountsDashboard()
{
    AccountsMainDashboard* widget = new AccountsMainDashboard(this);
    emit reportOpened(widget, "Accounts Dashboard");
}

void ReportsSelector::openItemsDashboard()
{
    InventoryItemDashboard* widget = new InventoryItemDashboard(this);
    emit reportOpened(widget, "Items Dashboard");
}

void ReportsSelector::openCustomersDashboard()
{
    CustomerDashboard* widget = new CustomerDashboard();
    emit reportOpened(widget, "Customers Dashboard");
}

void ReportsSelector::openSalesDashboard()
{
    SalesMainDash* widget = new SalesMainDash(this);
    emit reportOpened(widget, "Sales Dashboard");
}

void ReportsSelector::openPurchaseDashboard()
{
    PurchasesMainDash* widget = new PurchasesMainDash(this);
    emit reportOpened(widget, "Purchase Dashboard");
}

void ReportsSelector::openReceiptsDashboard()
{
    ReceiptsMainDash* widget = new ReceiptsMainDash();
    emit reportOpened(widget, "Receipts Dashboard");
}

void ReportsSelector::openItemwiseOrderReport()
{
    ItemwiseSODatewiseReport *widget = new ItemwiseSODatewiseReport();
    emit reportOpened(widget, "Itemwise Order Reports");
}

void ReportsSelector::openGodownwiseItemMovement()
{
    GodownwiseItemMovement *widget = new GodownwiseItemMovement();
    emit reportOpened(widget, "Godownwise Item Movement");
}

void ReportsSelector::openGroupwiseItemMovement()
{
    GroupwiseItemMovement *widget = new GroupwiseItemMovement();
    emit reportOpened(widget, "Account Groupwise Item Movement");
}

void ReportsSelector::openLedgerwiseItemMovement()
{
    LedgerwiseItemMovement *widget = new LedgerwiseItemMovement();
    emit reportOpened(widget, "Ledgerwise Item Movement");
}

void ReportsSelector::openShiftwiseReport()
{
    ShiftWiseReport *widget = new ShiftWiseReport();
    emit reportOpened(widget, "Shiftwise Report");
}

void ReportsSelector::openSalesmanwiseServiceReport()
{
    WaiterwiseServicewiseReport *widget = new WaiterwiseServicewiseReport();
    emit reportOpened(widget, "Salesmanwise Report");
}

void ReportsSelector::openTodaysBills()
{
    TodaysBills *widget = new TodaysBills();
    emit reportOpened(widget, "Today's Bills");
}

void ReportsSelector::openBalanceSheet()
{
    BalanceSheet *widget = new BalanceSheet();
    emit reportOpened(widget, "Balance Sheet");
}

void ReportsSelector::openProfitLoss()
{
    ProfitAndLossReport *widget = new ProfitAndLossReport();
    emit reportOpened(widget, "Profit And Loss Report");
}
