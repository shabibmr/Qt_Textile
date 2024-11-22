#ifndef SALESCOUNTERWINDOW_H
#define SALESCOUNTERWINDOW_H

#include "datamodels/transaction/generalvoucherdataobject.h"
#include "customwidgets/numberpad.h"
#include "database/finance/salesvoucher/salesvoucherdatabasehelper.h"
#include "database/inventory/salesorder/salesorderdatabasehelper.h"
#include <math.h>
#include <QSerialPort>

#include "finance/transactions/purchases/purchaseentry.h"
#include "finance/transactions/payments/payment.h"
#include "finance/transactions/receipts/receiptentry.h"
#include "finance/transactions/journals/journal.h"
#include "inventory/transactions/stockentry/stockentry.h"
#include "inventory/transactions/stockjournal/stockjournals.h"
#include "inventory/transactions/salesorders/salesorders.h"

#include "projects/WorkOrder/workorderlist.h"

#include "inventory/masters/sales_inventory_groups/inventorygroup.h"
#include "inventory/masters/sales_inventory_items/inventoryitem.h"
#include "inventory/transactions/ItemwiseSalesOrder/iteemwisesalesorderlist.h"
#include "inventory/masters/uom/uom.h"
#include "finance/masters/accountgroups/accountgroups.h"
#include "inventory/masters/godowns/godown.h"
#include "counter/Shift/shiftentry.h"
#include "finance/masters/ledgers/ledgermaster.h"
#include "finance/reports/LedgerReport/ledgerreport.h"
#include "finance/reports/BalanceSheet/balancesheet.h"
#include "finance/reports/Daybook/daybook.h"
#include "finance/reports/ProfitAndLossReport/profitandlossreport.h"
#include "finance/reports/TrialBalance/trialbalance.h"
#include "finance/reports/VATReport/vatreport.h"
#include "finance/reports/GroupwiseOutstanding/groupwiseoutstanding.h"

#include "counter/Reports/todaysbills.h"
#include "counter/Reports/salesordersreport.h"
#include "inventory/reports/GroupwiseReport/groupwisereport.h"
#include "inventory/reports/ItemwiseReport/itemwisereport.h"
#include "inventory/reports/ItemwiseReport/itemwisepurchasereport.h"
#include "inventory/reports/NegativeStock/negativestock.h"
#include "inventory/reports/MovementAnalysis/groupwiseitemmovement.h"
#include "inventory/reports/MovementAnalysis/ledgerwiseitemmovement.h"
#include "inventory/reports/MovementAnalysis/movementanalysis.h"

#include "inventory/pricelist/pricelist.h"



#include "customwidgets/LedgersView/ledgersdisplaycontroller.h"

#include "inventory/reports/StockReport/stockreport.h"
#include "inventory/reports/BelowReorderLevel/belowreorderlevel.h"
#include "itemdetail.h"
#include "inventory/masters/sales_inventory_groups/inventorygroup.h"
#include "hr/adduserprofile.h"
#include "login/loginpage.h"

#include "datamodels/projects/Quotation/quotationstatus.h"
#include "login/loginvalues.h"
#include "finance/transactions/calculator/transactioncalculator.h"
#include "counter/Shift/shiftentry.h"
#include "counter/Shift/closingentry.h"

#include "modeofpay.h"
#include "saveddrafts.h"
#include "denominations.h"
#include "addressdialog.h"

#include "counter/Settings/settings.h"
#include "counter/Settings/helpcontents.h"
#include "counter/Settings/about.h"

#include "miscellaneousitems.h"
#include "passwordentry.h"
#include "database/hr/userprofiledatabasehelper.h"
#include "finance/transactions/creditnote/creditnote.h"
#include "finance/transactions/debitnote/debitnote.h"

#include <finance/reports/SalesMarginReport/salesmarginreport.h>
#include "customwidgets/SearchWidgets/Inventoryitem/Completer/gminventoryitemsearch.h"
#include "customwidgets/SearchWidgets/Uom/uomsearchwidget.h"

#include <QTimer>
#include <QMainWindow>
#include <QMessageBox>
#include "counter/PrinterHelper/counterprinterhelper.h"

#include <QProcess>
#include <QPointer>
#include <QSerialPort>
#include <QStringRef>

namespace Ui {
class SalesCounterWindow;
}

class SalesCounterWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit SalesCounterWindow(int id = 0,QWidget *parent = 0);
    ~SalesCounterWindow();

    GeneralVoucherDataObject* voucher;

    SalesInventoryItemDatabaseHelper *itemsHelper;

    salesVoucherDatabaseHelper* salesHelper;

    SalesOrderDatabaseHelper* salesOrderHelper;

    LedgerMasterDatabaseHelper* ledHelper;

    NumberPad* numPad;

    ModeOfPay* ModeOfPayWidget;

    SavedDrafts* SavedDraftsWidget;

//    AddressDialog* AddressDialogWidget;

    ShiftEntry* ShiftEntryWidget;

    Denominations* DenominationsWidget;

    ClosingEntry* ClosingEntryWidget;

    CounterPrinterHelper *printer;

    QSerialPort *serial;

    int billsCount = 0;

    GMInventoryItemSearch* lineNameLine;


public slots:
    void ModeOfPaySlot();
    void addNewItemComp(inventoryItemDataModel item);
    float getRoundOff(float Amount, float multiple);
    void openClosingEntry();
    void openPurchaseItemwise();
    void openLedgerwiseItemMovement();
    void openGroupwiseItemMovement();
    void openItemTransactions();

    void openNegativeStockList();

    void openDash();



private slots:

    void setTable();

    void setAdditionalTable();

    void on_cashPaidLineEdit_textChanged(const QString &arg1);

    void on_itemDeleteToolButton_clicked();

    void on_refreshToolButton_clicked();

    void on_cashOutToolButton_clicked();

    void on_draftToolButton_clicked();

    void on_modeOfPayToolButton_clicked();

    void on_draftedBillsToolButton_clicked();

    void on_pickCallToolButton_clicked();

    void calcTotals();

    void openInventoryItems();
    void openUOMList();

    void openAccountGroups();

    void openShiftWidget(float value);

    void openSalesEntry();

    void openPurchaseEntry();

    void openPaymentEntry();

    void openReceiptEntry();

    void openJournalEntry();

    void openPurchaseReturn();

    void openSalesReturn();

    void openOrdersEntry();

    void connectMenuActions();

    void closeApplication();

    void openLedgers();

    void openLedgerReport();

    void openDayBook();

    void openTrialBalance();

    void openProfitLoss();

    void openBalanceSheet();

    void openVATReport();

    void openOutstandingReport();

    void openClosingStock();

    void openInventoryGroups();

    void openTodaysBills();

    void openStockReport();

    void openItemwiseReport();

    void openGroupwiseReport();

    void openSalesReport();

    void openSalesOrders();

    void openReOrderReport();

    void numpadSlots(QString text);

    void plusClicked();

    void minusClicked();

    void dotClicked();

    void deleteClicked();

    void clearClicked();

    void showTime();

    void addNewItem();

    void on_tableWidget_clicked(const QModelIndex &index);

    void saveVoucher();

    void saveDraft();

    void setVoucher(GeneralVoucherDataObject *obj);

    void SearchInventoryItems();

    void addNewItemToList(CompoundItemDataObject);

    void createNewItem(QString text);

    void on_reportsToolButton_clicked();

    void on_searchItemButton_clicked();

    void on_shiftCloseButton_clicked();

    void on_screenLockButton_clicked();

    void on_adminButton_clicked();

    void on_openTillButton_clicked();

    void on_priceQuantityToggleButton_clicked();

    void on_miscellaneousButton_clicked();

    void getCustomerDetails(QString Address, QString Phone, QString Name);

    void printVoucher();

    void openItemDetail();

    void openEmployees();

    void openGodowns();

    void openPriceList();


    void openSettings();

    void on_priceQuantityToggleButton_toggled(bool checked);

    void selectRow(int row);

    void openHelpContents();

    void openAbout();

    void openBillwiseReport();

    void openServicewiseReport();

    void openDailyClosing();

    void openShiftwiseReport();

    void openTimeBasedReport();

    void openWaiterwiseReport();

    void openComplimentaryReport();

    void openDiscountReport();

    void openDenominations();

    void openGroupSummary();

    void openOutstandingPayable();

    void openOutstandingReceivable();

    void showMenuBar();

    void openTillUSB();

    void openStockEntry();

    void openStockJournal();

    void on_phoneNoLineEdit_returnPressed();

    void setTableHeading();

    void readData();

    void writeData(const QByteArray &data);

    void closeSerialPort();

    void openSerialPort();

    void handleError(QSerialPort::SerialPortError error);

    void openSalesMarginReport();

    void openLedgerwise();
    void openSalesordersVoucher();
    void openWorkOrder();

    void on_tableWidget_doubleClicked(const QModelIndex &index);

private:
    Ui::SalesCounterWindow *ui;
};

#endif // SALESCOUNTERWINDOW_H
