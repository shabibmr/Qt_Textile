#ifndef SALESORDERVOUCHEREDITOR_H
#define SALESORDERVOUCHEREDITOR_H

#include <QWidget>
#include <QPointer>
#include <QKeyEvent>
#include <QLabel>
#include <QSpacerItem>
#include <QMessageBox>
#include <QInputDialog>

#include <login/loginvalues.h>
#include <datamodels/projects/Quotation/quotationstatus.h>
#include <datamodels/transaction/gmvouchertypes.h>

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
#include "finance/transactions/LedgerView/ledgerwisevouchertableview.h"

#include "customwidgets/Basic/gmlineedit.h"
#include "customwidgets/Basic/gmdateedit.h"
#include "customwidgets/Basic/gmdatetimeedit.h"

#include "customwidgets/Basic/gmplaintextedit.h"

#include "counter/MainScreen/addressdialog.h"


#include "sharetools/print/printerhelper.h"
#include "sharetools/email/email.h"
#include "sharetools/excel/excel.h"


#include "database/inventory/salesorder/salesorderdatabasehelper.h"
#include "voucherOperations/coupledvoucherhandler.h"

#include "CommonUi/Transactions/InventoryLedger/inventoryledgertransactionseditor.h"



class SalesOrderVoucherEditor : public InventoryLedgerTransactionsEditor
{
    Q_OBJECT


    QPointer<GMDateTimeEdit> deliveryDateWidget;

    SalesOrderDatabaseHelper *dbHelper;
//    VoucherEditorSelector *vEditor;


//    void askForPrint();
public:
    explicit SalesOrderVoucherEditor(SalesOrderDatabaseHelper *db = new SalesOrderDatabaseHelper, InventoryLedgerTransactionsEditor *parent = nullptr);
//    ~SalesOrderVoucherEditor();

    void setDrawerLayout() override;

//    void initDbHelpers();

//    void initVoucher();

    void initGlobalObjects();

//    void setWidgets();

    void resetWidgets() override;



public slots :
    void showVoucher() override;
//    void closeEditor();
    void saveVoucher() override;
//    void askForPrint();
    void importVoucher(GeneralVoucherDataObject *voucher2);
    void importItemsFromVoucher(GeneralVoucherDataObject *voucher2);
    void exportToSalesVoucher();
    void exportToDeliveryNote();
    void sendEmail();
    void SalesVoucherSaved();
    void DeliveryNoteSaved();
    void printVoucher(QPrinter *printer) override;
    void exportToPdf(QPrinter *printer) override;

    void printKOT();

signals:
//    void closing();
//    void NotificationAccepted();
//    void NotificationRejected(QString reason);


};

#endif // SALESORDERVOUCHEREDITOR_H
