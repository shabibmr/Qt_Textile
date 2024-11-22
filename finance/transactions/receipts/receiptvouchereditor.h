#ifndef RECEIPTVOUCHEREDITOR_H
#define RECEIPTVOUCHEREDITOR_H

#include <QWidget>
#include <QPointer>
#include <QKeyEvent>
#include <QLabel>
#include <QSpacerItem>
#include <QMessageBox>

#include <login/loginvalues.h>
#include <datamodels/projects/Quotation/quotationstatus.h>
#include <datamodels/transaction/gmvouchertypes.h>

#include "customwidgets/TableViews/ledgervouchertableview.h"
#include "customwidgets/Models/gmreceiptentrymodel.h"
#include "customwidgets/Bank/chequewidget.h"

#include "customwidgets/SearchWidgets/Ledger/gmledgerlinewidget.h"
#include "customwidgets/SearchWidgets/Ledger/gmledgerlinematerial.h"
#include "customwidgets/SearchWidgets/Employee/gmemployeelinematerial.h"
#include <customwidgets/Delegates/Ledgers/gmledgertransactiondelegate.h>
#include "customwidgets/Material/components/qtmaterialfab.h"
#include "customwidgets/Material/components/qtmaterialsnackbar.h"
#include "customwidgets/Material/components/qtmaterialdrawer.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "customwidgets/Material/components/qtmaterialappbar.h"
//#include "finance/transactions/BillwiseRecords/addbillwiserecord.h"


#include "customwidgets/Basic/gmlineedit.h"
#include "customwidgets/Basic/gmdateedit.h"
#include "customwidgets/Basic/gmplaintextedit.h"


#include "sharetools/print/printerhelper.h"
#include "sharetools/email/email.h"
#include "sharetools/excel/excel.h"
#include "counter/PrinterHelper/counterprinterhelper.h"


#include "database/finance/receiptvoucher/receiptvoucherdatabasehelper.h"
#include "datamodels/accounts/chequedetail.h"
#include "CommonUi/Transactions/vouchereditorbase.h"


class ReceiptVoucherEditor : public VoucherEditorBase
{
    Q_OBJECT

//    QString windowTitle="Receipt Voucher";
//    QString bgcolor = "#e0f7fa";
//    QColor appBarColor;

    QList<int> avlcols {1,2};

//    GeneralVoucherDataObject* voucher;
    ChequeDetail *chqObj;
    ChequeWidget *chequeWidget;
    bool chequeEntryCompleted =false;

//    LedgerMasterDatabaseHelper *ledgerHelper;
//    AccountGroupMasterDatabaseHelper *accHelper;
//    ReceiptVoucherDatabaseHelper *dbHelper;
//    UomDataBaseHelper *uomHelper;
//    UserProfileDatabaseHelper *userHelper;
//    SalesInventoryItemDatabaseHelper *itemsHelper;
//    GodownDatabaseHelper *godownHelper;

    LedgervoucherTableView *DataTableWidget;                 //X

//    GMEmployeeLineMaterial *SalesmanSearchBox;
//    GMLedgerLineMaterial *mainLedgerWidget;

//    QtMaterialFloatingActionButton *saveButton;
//    QtMaterialIconButton *backButton;
//    QtMaterialFloatingActionButton *shareButton;
//    QtMaterialIconButton *printButton;
//    QtMaterialAppBar *appBar;

//    QtMaterialSnackbar *snackBar;
//    QtMaterialDrawer *drawer;


//    QPointer<GMDateEdit> voucherDateWidget;
//    QPointer<GMLineEdit> voucherNumberWidget;
//    QPointer<GMPlainTextEdit> voucherNarrationWidget;
    QPointer<GMReceiptEntryModel> model;

//    CounterPrinterHelper *printHelper;

    QStringList lis;

public:
    explicit ReceiptVoucherEditor(VoucherDBAbstract *db, VoucherEditorBase *parent = nullptr);
    ~ReceiptVoucherEditor();
    void initExtraActions();

//    void initDbHelpers();

    void initVoucher();

    void initGlobalObjects();

    void resetWidgets() override;



public slots :
    void showVoucher() override;
    void saveVoucher() override;
    void printVoucher(QPrinter *printer) override;
    bool setChequeData();
    void sendEmail();


    void setBillwiseMapping(LedgerMasterDataModel *ledger, QDate voucherDate);
signals:
//    void closing();
//    void NotificationAccepted();

};

#endif // RECEIPTVOUCHEREDITOR_H
