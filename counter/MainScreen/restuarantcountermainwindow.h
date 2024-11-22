#ifndef RESTUARANTCOUNTERMAINWINDOW_H
#define RESTUARANTCOUNTERMAINWINDOW_H

#include "datamodels/transaction/generalvoucherdataobject.h"
#include "database/finance/salesvoucher/salesvoucherdatabasehelper.h"
#include "database/finance/salesvoucher/salesvoucherdeleteddatabasehelper.h"

#include "finance/reports/SalesOrders/pendingorders.h"

#include "database/inventory/salesorder/salesorderdatabasehelper.h"
#include "database/inventory/pricelist/pricelistdatabasehelper.h"
#include "database/Settings/ConfigurationSettingsDatabaseHelper.h"
#include "QKeyEvent"
#include <QtGui>
#include <QSerialPort>

#include "customwidgets/gmitemsearch.h"
#include "finance/transactions/calculator/transactioncalculator.h"
#include "finance/transactions/sales/salesadjustmentwidget.h"
#include "projects/WorkOrder/workordergenerator.h"

#include "finance/reports/GSTReport/gstreport.h"
#include "finance/reports/GSTReport/vatreport.h"

#include <QTranslator>
#include "counter/Shift/shiftentry.h"
#include "counter/Shift/restaurantshiftentry.h"
#include "finance/reports/LedgerReport/ledgerreport.h"
//#include "finance/reports/DatewiseSales/datewisesalesreport.h"
#include "finance/reports/BalanceSheet/balancesheet.h"
#include "finance/reports/ProfitAndLossReport/profitandlossreport.h"
#include "finance/reports/TrialBalance/trialbalance.h"
#include "counter/MainScreen/passwordentry.h"


#include "counter/Reports/todaysbills.h"
#include "counter/Reports/Restaurant/restaurantsalesordersreport.h"
#include "counter/Reports/modeofservicewisereport.h"
#include "counter/Reports/dailyclosingsalesreport.h"
#include "counter/Reports/shiftwisereport.h"
#include "counter/MainScreen/modeofservicewidget.h"
#include "counter/MainScreen/shiftvalueseditor.h"

#include "counter/Reports/waiterwisesalesreport.h"
#include "counter/MainScreen/customerscreen.h"

#include "inventory/reports/GroupwiseReport/groupwisereport.h"


#include "counter/Reports/billwisereport.h"
#include "counter/Reports/Restaurant/zomatoreport.h"
#include "counter/Reports/complimentarysalesreport.h"
#include "counter/Reports/discountsreport.h"
#include "Notifications/notificationslist.h"


#include "restaurantitemdetail.h"
#include "hr/adduserprofile.h"

#include "database/hr/userprofiledatabasehelper.h"

#include "datamodels/projects/Quotation/quotationstatus.h"
#include "login/loginvalues.h"
#include "counter/Shift/shiftentry.h"
#include "counter/Shift/closingentry.h"
#include "assignsalesman.h"
#include "tableselect.h"
#include "restaurantextras.h"

#include "modeofpay.h"
#include "counter/Reports/Restaurant/restaurantsaveddrafts.h"
#include "denominations.h"
#include "addressdialog.h"
#include "counter/MainScreen/shiftentrywidget.h"

#include "salesgroups.h"
#include "salesitems.h"

#include "counter/Settings/settings.h"
#include "counter/Settings/helpcontents.h"
#include "counter/Settings/about.h"

#include <QTimer>
#include <QMainWindow>
#include <QMessageBox>
#include <QCompleter>

#include <QSqlQueryModel>
#include <QSqlRecord>
#include "counter/Settings/kotprintersettings.h"
#include "sharetools/sms/smswidget.h"

#include <counter/PrinterHelper/counterprinterhelper.h>
#include <QInputDialog>

#include "counter/Reports/Restaurant/restaurantclosingstock.h"

#include "customwidgets/Material/components/qtmaterialsnackbar.h"
#include "customwidgets/Material/components/qtmaterialfab.h"
#include "customwidgets/Material/components/qtmaterialbadge.h"


#include "QtSerialPort/QSerialPort"
#include "database/databasehelper/databasevalues.h"

#include <QSerialPort>
#include <QStringRef>
#include "CommonUi/uicontroller.h"

#include <QPropertyAnimation>
#include "database/hr/usergroupdatabasehelper.h"
#include "inventory/reports/WorkOrderReport/kotreport.h"
#include "counter/Shift/dailyclosingreport.h"
#include "counter/Reports/waiterwiseservicewisereport.h"



namespace Ui {
class RestuarantCounterMainWindow;
}

class RestuarantCounterMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RestuarantCounterMainWindow(int id = 0, QString voucherPrefix = "0", QMainWindow *parent = 0);
    ~RestuarantCounterMainWindow();

    QSerialPort *serial;
    QString dStr;
    UiController *uiCtrl;

    int billsCount = 0;
    int draftsCount = 0;

    QtMaterialSnackbar* bar;
    QtMaterialFloatingActionButton *notification;
    QtMaterialBadge *badge;

    QPropertyAnimation *animation;

    bool Inclusive = true;
    bool allowEmptyAddressFlag = true;

    bool qtyFlag = false;

    bool askToPrintKOT = true;
    bool showModFlag = false;
    bool modeSet = false;

    bool saveOnRefreshFlag = false;

    int lang =0;

    bool serveOnlyPrefix = false;

    bool autoAllotRefTableFlag=false;

    bool isKot = false;

    bool printOn=true;

    bool disableItemEdit=false;

    bool serverSalesOrder=true;

    bool allowNegativeStock = false;

    bool allowOrderCopyPrint = true;

    bool printCancelledKOTFlag = true;

    bool deleteKOTItemWithoutRemarksFlag = true;

    int freeCount=0;

    int freeUsed =0;

    int colCount;
    int buttWidth;
    int buttHeight;



    QToolButton *plusToolButton;
    QToolButton *minusToolButton;
    QToolButton *itemDeleteToolButton;
    QToolButton *cashOutToolButton;
    QToolButton *modeOfPayToolButton;
    QToolButton *draftToolButton;
    QToolButton *extrasToolButton;
    QToolButton *plus50ToolButton;
    QToolButton *refreshToolButton;
    QToolButton *draftedBillsToolButton;


    QString WelcomeMessage;

    QDialog *passWordShiftDialog;
    QLabel *textLabel;
    QLineEdit* passwordShiftLine;

    QLineEdit* customerLine;

    GeneralVoucherDataObject* voucher;

    GeneralVoucherDataObject* deletedvoucher;

    SalesInventoryItemDatabaseHelper *itemsHelper;

    PricelistDatabaseHelper *priceHelper;
    UomDataBaseHelper *uomHelper;

    SettingsHelper *settings;


    QSqlQueryModel* itemsQuery;

    salesVoucherDatabaseHelper* salesHelper;

    SalesOrderDatabaseHelper* salesOrderHelper;

    salesVoucherDeletedDatabaseHelper* deletedSalesHelper;

    WorkOrderDatabaseHelper *workOrderHelper;

    LedgerMasterDatabaseHelper* ledHelper;

    UserProfileDatabaseHelper* userHelper;

    AccountGroupMasterDatabaseHelper *accHelper;

    UserGroupDatabaseHelper *userGroupHelper;

    NotificationsList *notificationWidget;

    ModeOfPay* ModeOfPayWidget;

    RestaurantSavedDrafts* SavedDraftsWidget;

    AddressDialog* AddressDialogWidget;

    RestaurantShiftEntry* ShiftEntryWidgett;

    Denominations* DenominationsWidget;

    ClosingEntry* ClosingEntryWidget;

    SalesGroups* SalesGroupsWidget;

    SalesItems* SalesItemsWidget;

    ModeOfServiceWidget *ModeOfService;

    CounterPrinterHelper *printer;

    bool printKotCopy=true;

    int kotNo=1;

    QString previousTable;

    QPointer<GMItemSearchLite> itemLine;

    CustomerScreen *customerScreenWidget;

    WorkOrderGenerator *kotGenerator;

    WorkOrderGenerator *tabKotGenerator;

    QTranslator l;

    void initDraftedBillsWidget();

    bool checkDraftsPermission();

//    bool saveVoucherStarted = false;
    
    
    void hideCustomerScreen(bool show);

    
    void loadLanguage();

private slots:

    void on_plusToolButton_clicked();

    void on_minusToolButton_clicked();

    void on_itemDeleteToolButton_clicked();

    void SalesOrderSelected(QString vno);

    void refreshPage();

    void on_plus50ToolButton_clicked();

    void on_extrasToolButton_clicked();

    void on_draftedBillsToolButton_clicked();

    void on_draftToolButton_clicked();

    void OKclickedOnDraft();

    void on_modeOfPayToolButton_clicked();

    void on_cashOutToolButton_clicked();

    void on_pickCallToolButton_clicked();

//    void on_deliveryToolButton_clicked();

//    void on_takeAwayToolButton_clicked();

//    void on_dineInToolButton_clicked();

    void on_selectTableToolButton_clicked();

    void on_selectWaiterToolButton_clicked();

//    void on_tableWidget_clicked(const QModelIndex &index);

    void calcTotals();

    void connectMenuActions();

    void openInventoryGroups();

    void openInventoryItems();

    void openAccountGroups();

    void openLedgers();

    void openEmployees();

    void openSettings();

    void openUserGroups();

    void openMeasurements();

    void openPurchaseEntry();

    void openSalesEntry();

    void openPaymentEntry();

    void openReceiptEntry();

    void openReceiptNote();

    void openJournalEntry();

    void openPurchaseReturn();
//    void openReceiptNote();

    void openShiftWidget(ShiftDataModel *obj);

    void openTodaysBills();

    void openSalesReport();

    void openPendingOrders();

    void openKOTReport();

    void openItemwiseReport();

    void openGroupwiseReport();

    void openServicewiseReport();

    void openDailyClosing();

    void openShiftwiseReport();

    void openTimeBasedReport();

    void openWaiterwiseReport();

    void openZomatoReport();

    void openComplimentaryReport();

    void openDiscountReport();

    void openBillwiseReport();

    void openDayBook();

    void openLedgerReport();

    void openGroupSummary();

    void openSalesOrders();

    void openStockReport();

    void openReOrderReport();

    void openOutstandingPayable();

    void openOutstandingReceivable();

    void openPurchaseItemReport();

    void openTrialBalance();

    void openProfitLoss();

    void openBalanceSheet();

    void openNotifications();
    void openVATReport();

    void openHelpContents();

    void openAbout();

    void setTable();

    void closeApplication();

    void showTime();

    void addNewItemBySearch(inventoryItemDataModel BaseItem);

    void saveVoucher();

    void saveDraft();

//    void SearchInventoryItems();

    void addNewItemToList(CompoundItemDataObject);

    void createNewItem(QString text);

    void printVoucher();

    void openRestaurantItemDetail();

    void selectRow(int row);

//    void on_pickCallToolButton_toggled(bool checked);

    void setMainScreen();

    void openDenominations();

    void changeTable();

    void changeWaiterNameAndDraft(int salesManID);

    void SwitchSalesMan(int salesManID);

    void on_tableWidget_doubleClicked(const QModelIndex &index);

    void changeWaiterNameAndSave(int salesManID);

    void sendToKOT();

    void on_switchButton_clicked();

    void showExtrasWidget();

    void showMenuBar();

    void lockScreen();

    void openStockEntry();

    void openStockJournal();

    void openStockRequest();

    void tabSync();

    void on_airportButton_clicked();

    void openUOMList();


//    void justGetCustomerDetails(QString Address, QString Phone, QString Name, QString route = "");




    void on_tableWidget_clicked(const QModelIndex &index);


    void on_printOnOff_clicked();

    bool checkAllStocks();

    void editQtyByKeyboard();

    void qtyEditCompleted();

    void openSalesMarginReport();
    void openSMSWidget();


    void on_refreshToolButton_clicked();

    void showPermissionDenied();

    void openDailyClosingReport();
    void openWaiterwiseService();

public slots:

    void keyPressEvent(QKeyEvent *e);

    void setVoucher(GeneralVoucherDataObject *obj);

    void groupButtonClicked(QString grpID);

    void itemButtonClicked(QString itmID);

    void changeTableAndDraft();

    void switchOption();

    void changeTableToDraft();

    void openKotSettings();

    void openTill();

//    void getCustomerDetails(QString Address, QString Phone, QString Name, QString route="");

    void ModeOfPaySlot();

    float getRoundOff(float Amount, int multiple);

    void settleVoucherAsCash(GeneralVoucherDataObject *voucher);

    void switchPaymentMode(GeneralVoucherDataObject *voucher);

    void readData();

    void setLoginValues(int user);

    void writeData(const QByteArray &data);

    void closeSerialPort();

    void openSerialPort();

    void handleError(QSerialPort::SerialPortError error);

    void OKclickedOnSaveVoucher();

    void openDatewiseReport();

    void ChangeFont();

    void showDayNight();

    void deleteVoucher(GeneralVoucherDataObject *obj);
    void openDefectiveEntry();

    void openPurchaseSummaryReport();
    void openPurchaseOrderSummaryReport();

    void openPurchaseOrders();
    void openIndents();

    void openPriceLists();
    void openCustomerwiseReport();
    void saveDeleted(bool sendMail=true, QString narration = "");
    void MailDeletedList(QString name ="");
    void openCustomersLedger();
    void openSalesAdjustment();
    void openVisitorsList();

    void hideAdjustAction();
    void openItemDetail(int row);

    void openGSTReport();
    void openDatewisePurchase();
    void openCreditNote();
    void SalesDeletedReport();
    void openGodowns();
    void openDeletedBills();
    void openLedgerwiseItemsReport();
    void openReceiptSummary();
    void openPaymentSummary();
    void setBillwiseMapping();
    void appendDeletedItem(CompoundItemDataObject c);
    void openRceiptNoteSummary();

    void switchLang();
    void openTablesMaster();
    void openReservations();
    void hideMenuBar();

signals:
    void sendSalesGroupID(QString);
    void uiSelected(int ui_id, bool isConfigInDb);
    void closeMain();
    void minimizeMain();
    void switchCompany();

private:
    Ui::RestuarantCounterMainWindow *ui;

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *event) override;

    // QWidget interface
protected:
    void changeEvent(QEvent *event) override;
};

#endif // RESTUARANTCOUNTERMAINWINDOW_H
