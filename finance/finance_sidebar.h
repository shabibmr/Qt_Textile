#ifndef FINANCE_SIDEBAR_H
#define FINANCE_SIDEBAR_H

#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include "customwidgets/section.h"
#include <QPushButton>

#include "inventory/masters/sales_inventory_items/additems.h"
#include "customwidgets/Misc/sidebarbutton.h"
#include "customwidgets/defaultdashboard.h"
#include "finance/masters/ledgers/ledgermaster.h"
#include "finance/masters/accountgroups/accountgroups.h"
#include "finance/masters/termsconditions/termslist.h"
#include "finance/masters/ledgers/ledgeropeningvalues.h"

#include "finance/transactions/sales/salesvoucherlist.h"
#include "finance/transactions/purchases/purchaseentry.h"
#include "inventory/transactions/salesorders/salesorders.h"
#include "inventory/transactions/purchaseorders/purchaseorders.h"
#include "finance/transactions/payments/payment.h"
#include "finance/transactions/receipts/receiptentry.h"
#include "finance/transactions/journals/journal.h"
#include "finance/transactions/creditnote/creditnote.h"
#include "finance/transactions/debitnote/debitnote.h"
#include "inventory/transactions/deliverynote/deliverynotes.h"
#include "finance/transactions/contra/contravoucherlist.h"
#include "finance/transactions/Proforma/proformainvoicelist.h"
#include "projects/quotations/quotaitonslist.h"

#include "voucherOperations/VocuherList/templatelist.h"
#include "CommonUi/uicontroller.h"


//#include "counter/MainScreen/restuarantcountermainwindow.h"

#include "finance/reports/DayBook/daybook.h"
#include "finance/reports/LedgerReport/ledgerreport.h"
#include "finance/reports/CustomerReport/customerreport.h"
#include "finance/reports/Group/groupsummary.h"
#include "finance/reports/Group/groupvouchers.h"
#include "finance/reports/group/groupsummary.h"
#include "finance/reports/group/groupvouchers.h"
#include "finance/reports/CashBankBook/cashbankbook.h"
#include "finance/reports/GroupwiseOutstanding/groupwiseoutstanding.h"
#include "finance/reports/LedgerwiseOutstanding/ledgerwiseoutstanding.h"
#include "finance/reports/SalesmanwiseReport/salesmanwisereport.h"
#include "finance/reports/GSTReport/gstreport.h"
#include "counter/Reports/waiterwisesalesreport.h"
#include "finance/reports/TrialBalance/trialbalance.h"
#include "finance/reports/BalanceSheet/balancesheet.h"
#include "finance/reports/ProfitAndLossReport/profitandlossreport.h"
#include "Notifications/notificationslist.h"

#include "finance/reports/DatewiseSales/datewisesalesreport.h"
#include "finance/reports/SalesMarginReport/salesmarginreport.h"

#include "counter/Reports/discountsreport.h"
#include "counter/Reports/complimentarysalesreport.h"
#include "counter/Reports/shiftwisereport.h"
#include "counter/Reports/billwisereport.h"
#include "finance/reports/BankReconciliation/bankreconciliation.h"
#include <customwidgets/Misc/sidebarbutton.h>

namespace Ui {
class Finance_SideBar;
}

class Finance_SideBar : public QWidget
{
    Q_OBJECT

public:
    explicit Finance_SideBar(QWidget *parent = 0);
    ~Finance_SideBar();
    DefaultDashBoard *dash;
    QListWidgetItem *item1;

    SideBarButton *accountsButton;
    SideBarButton *Ledger;
    SideBarButton *LedgerOpening;
    SideBarButton *TermsButton;

    SideBarButton *salesInvoice;
    SideBarButton *purchaseInvoice;
    SideBarButton *POSButton;

    SideBarButton *salesOrdersButton;
    SideBarButton *purchaseOrdersButton;
    SideBarButton *payment;
    SideBarButton *receipt;
    SideBarButton *journalButton;
    SideBarButton *creditNoteButton;
    SideBarButton *debitNoteButton;
    SideBarButton *deliveryNote;
    SideBarButton *ContraVoucherButton;
    SideBarButton *notification;
    SideBarButton *proformaButton;
    SideBarButton *quotationButton;

    SideBarButton *templateButton;

    SideBarButton *DayBookButton;
    SideBarButton *LedgerReportButton;
    SideBarButton *CustomerReportButton;
    SideBarButton *GroupSummaryButton;
    SideBarButton *GroupVouchersButton;
    SideBarButton *CashBankBookButton;
    SideBarButton *GroupWiseOutstandingButton;
    SideBarButton *LedgerwiseOutstandingButton;
    SideBarButton *SalesmanwiseButton;
    SideBarButton *DatewiseSales;

    SideBarButton *SalesMarginButton;

    SideBarButton *TrialBalanceButton;
    SideBarButton *BalanceSheetButton;
    SideBarButton *ProfitLossButton;
    SideBarButton *GSTReportButton;

    SideBarButton *DiscountButton;
    SideBarButton *CompliButton;
    SideBarButton *ShiftButton;
    SideBarButton *BillwiseButton;
    SideBarButton *bankReconciliationButton;



    accountGroups* accountGroupsWidget;
    ledgerMaster* ledgerMasterWidget;
    LedgerOpeningValues* LedgerOpeningValuesWidget;



    SalesVoucherList* SalesVoucherListWidget;
    TermsList* TermsListWidget;
    PurchaseEntry* PurchaseEntryWidget;
    salesOrders* salesOrdersWidget;
    purchaseOrders* purchaseOrdersWidget;
    Payment* PaymentWidget;
    ReceiptEntry* ReceiptEntryWidget;
    journal* journalWidget;
    creditNote* creditNoteWidget;
    debitNote* debitNoteWidget;
    deliveryNotes* deliveryNotesWidget;
    ContraVoucherList* contraVoucherListWidget;
    NotificationsList *notificationWidget;
    Daybook* DayBookWidget;
    LedgerReport* LedgerReportWidget;
    CustomerReport *CustomerReportWidget;
    GroupSummary* GroupSummaryWidget;
    GroupVouchers* GroupVouchersWidget;
    CashBankBook* CashBankBookWidget;
    GroupwiseOutstanding* GroupwiseOutstandingWidget;
    LedgerwiseOutstanding* LedgerwiseOutstandingWidget;
    SalesmanwiseReport* SalesmanwiseReportWidget;
    WaiterWiseSalesReport* waiterwiseWidget;
    TrialBalance* TrialBalanceWidget;
    ProfitAndLossReport* ProfitAndLossReportWidget;
    GSTReport* GSTReportWidget;
    BalanceSheet* BalanceSheetWidget;
    ProformaInvoiceList* ProformaInvoiceListWidget;
    QuotaitonsList* QuotationWidget;
//    RestuarantCounterMainWindow *POSWidget;

    DiscountsReport *DiscReportWidget;
    ComplimentarySalesReport * compliWidget;
    ShiftWiseReport *shiftReport;
    BankReconciliation *bankReconWid;
    DatewiseSalesReport *dateWisereportWdget;

    QFrame *getLine();

public slots:
    void resetAllColors();
    void accountsGroupClicked();
    void OpeningClicked();
    void TermsButtonClicked();
    void LedgerClicked();
    void salesInvoiceClicked();
    void POSClicked();
    void purchaseInvoiceClicked();
    void salesOrdersClicked();
    void purchaseOrdersClicked();
    void paymentClicked();
    void receiptClicked();
    void journalClicked();
    void creditNoteClicked();
    void debitNoteClicked();
    void deliveryNoteClicked();
    void contraVoucherClicked();
    void proformaInvoiceClicked();
    void quotationInvoiceClicked();
    void margin();
    void notificationClicked();
    void bankReconciliationClicked();
    void TemplateButtonClicked();


    void DayBookClicked();
    void LedgerReportClicked();
    void GroupSummaryClicked();
    void GroupVouchersClicked();
    void CashBankBookClicked();
    void GroupWiseOutstandingClicked();
    void LedgerwiseOutstandingClicked();
    void SalesmanwiseClicked();
    void TrialBalanceClicked();
    void BalanceSheetClicked();
    void ProfitLossClicked();
    void GSTReportClicked();

    void DiscountClicked();
    void CompliClicked();
    void ShiftReportClicked();

    void billwiseClicked();

    void searchText(QString text);

    void clearLayout();

    void CustomerReportClicked();
    void DatewiseSalesClicked();
signals:

    void searchTextchanged(QString);

private:
    Ui::Finance_SideBar *ui;
};

#endif // FINANCE_SIDEBAR_H
