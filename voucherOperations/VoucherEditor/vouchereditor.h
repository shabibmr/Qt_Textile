#ifndef VOUCHEREDITOR_H
#define VOUCHEREDITOR_H

#include <QWidget>
#include <QPointer>
#include <QKeyEvent>
#include <QLabel>
#include <QSpacerItem>
#include <QMessageBox>
#include <QInputDialog>
#include <QGroupBox>

#include <login/loginvalues.h>
#include <datamodels/projects/Quotation/quotationstatus.h>
#include <datamodels/transaction/gmvouchertypes.h>
#include "datamodels/transaction/requirementvoucherdataobject.h"

#include "customwidgets/TableViews/inventoryvouchertableview.h"
#include "customwidgets/Models/gminventoryitemmodel.h"
#include "customwidgets/SearchWidgets/Ledger/gmledgerlinewidget.h"
#include "customwidgets/SearchWidgets/Ledger/gmledgerlinematerial.h"
#include "customwidgets/SearchWidgets/Employee/gmemployeelinematerial.h"
#include <customwidgets/Delegates/Inventory/gminventorytransactiondelegate.h>
#include "customwidgets/Material/components/qtmaterialfab.h"
#include "customwidgets/Material/components/qtmaterialsnackbar.h"
#include "customwidgets/Material/components/qtmaterialdrawer.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "customwidgets/Material/components/qtmaterialappbar.h"
#include "customwidgets/Basic/Material/gmmaterialcombobox.h"

#include "customwidgets/Basic/gmlineedit.h"
#include "customwidgets/Basic/gmdateedit.h"
#include "customwidgets/Basic/gmplaintextedit.h"

#include "sharetools/print/printerhelper.h"
#include "sharetools/email/email.h"
#include "sharetools/excel/excel.h"
#include "sharetools/sharetools.h"

#include "database/finance/salesvoucher/salesvoucherdatabasehelper.h"
#include "database/Notifications/receivechanneldatabasehelper.h"
#include "customwidgets/showvoucherswidget.h"
#include "voucherOperations/coupledvoucherhandler.h"

#include "counter/MainScreen/addressdialog.h"
#include "customwidgets/Misc/getschedule.h"

//#include <QPainter>
//#include <QPaintEvent>


class VoucherEditor : public QWidget
{
    Q_OBJECT

    QString windowTitle="Template";
    QString bgcolor = "#e0f7fa";
    QColor appBarColor;

    GeneralVoucherDataObject* voucher;
    bool isCoupledVoucher = false;

    LedgerMasterDatabaseHelper *ledgerHelper;
    AccountGroupMasterDatabaseHelper *accHelper;
    salesVoucherDatabaseHelper *dbHelper;
    UomDataBaseHelper *uomHelper;
    UserProfileDatabaseHelper *userHelper;
    SalesInventoryItemDatabaseHelper *itemsHelper;
    receiveChannelDatabaseHelper *rHelper;
    PricelistDatabaseHelper *priceHelper;

    InventoryVoucherTableView *DataTableWidget;                 //X
     GMInventoryTransactionDelegate *itemDelegate;


//    GMEmployeeSearchWidget *SalesmanSearchBox;

    GMLedgerLineMaterial *mainLedgerWidget;
    GMEmployeeLineMaterial *SalesmanSearchBox;
    GMMaterialComboBox *priceListWidget;

    QtMaterialFloatingActionButton *saveButton;
    QtMaterialFloatingActionButton *rejectButton;
    QtMaterialIconButton *backButton;
    QtMaterialFloatingActionButton *shareButton;
    QtMaterialIconButton *printButton;
    QtMaterialIconButton *contactButton;
    QtMaterialAppBar *appBar;
    QtMaterialIconButton *importButton;

    QtMaterialSnackbar *snackBar;
    QtMaterialDrawer *drawer;

    QPointer<GMDateEdit> voucherDateWidget;
    QPointer<GMLineEdit> voucherNumberWidget;
    QPointer<GMPlainTextEdit> voucherNarrationWidget;
    QPointer<GMInventoryItemModel> model;

    PrinterHelper *printHelper;
    ShareTools *shareOptionsWidget;


    QStringList lis;

//    QPointer<QVBoxLayout> advanceLayout;
//    QWidget *advCanvas;
    CoupledVoucherHandler *coupledVoucherHandler;

    AddressDialog *AddressDialogWidget;

    QString backgroundColor = "#e0f7fa";
    QString textColor = "black";
    QString selectionColor = "#e0f7fa";
    QString highlightedColor = "#00acc1";
    QString headerBgColor= "#00bcd4"; //#0082c8
    QString iconColor = "white";
    QString titleColor = "white";

    QSize iconSize = QSize(18,18);

public:
    explicit VoucherEditor(QWidget *parent = nullptr);
    ~VoucherEditor();

    void initExtraActions();
    void initDbHelpers();
    void initVoucher();
    void initGlobalObjects();
    void setWidgets();
    void resetWidgets();
    void setMainLedger(LedgerMasterDataModel led);


public slots :
    void showVoucher(GeneralVoucherDataObject *obj);
    void closeEditor();
    void saveVoucher();
    void rejectVoucher();
    void deleteVoucher();
    void askForPrint();
    void printVoucher();
    void importVoucher(GeneralVoucherDataObject *voucher2);
    void importItemsFromVoucher(GeneralVoucherDataObject *voucher2);
    void setBillwiseMapping();
    void sendEmail();

    void setAdvancePaid(float amount);

    void getVoucherstoImport();
    void showContactDetails();
    void showShareOptions();
signals:
    void closing();
    void NotificationAccepted();
    void NotificationRejected(QString reason);
    void saveCompleted();

private:

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // VOUCHEREDITOR_H
