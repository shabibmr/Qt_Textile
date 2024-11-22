#ifndef PURCHASEORDERVOUCHEREDITOR_H
#define PURCHASEORDERVOUCHEREDITOR_H

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

#include "finance/transactions/purchases/purchasevouchereditor.h"

#include "customwidgets/Basic/gmlineedit.h"
#include "customwidgets/Basic/gmdateedit.h"
#include "customwidgets/Basic/gmplaintextedit.h"


#include "sharetools/print/printerhelper.h"
#include "sharetools/email/emailthread.h"
#include "sharetools/excel/excelreporthelper.h"


#include "database/inventory/purchaseorder/purchaseorderdatabasehelper.h"
#include "CommonUi/Transactions/InventoryLedger/inventoryledgertransactionseditor.h"


class PurchaseOrderVoucherEditor : public InventoryLedgerTransactionsEditor
{
    Q_OBJECT


public:
    explicit PurchaseOrderVoucherEditor(PurchaseOrderDatabaseHelper *db, InventoryLedgerTransactionsEditor *parent = nullptr);
    ~PurchaseOrderVoucherEditor();

    void setDrawerLayout() override;

//    void initExtraActions();

//    void initDbHelpers();

//    void initVoucher();

//    void initGlobalObjects();

//    void setWidgets();

    void resetWidgets() override;



public slots :
//    void showVoucher(GeneralVoucherDataObject *obj);
//    void closeEditor();
    void saveVoucher()override;
//    void printVoucher(QPrinter *printer) override;

    void AddItemstoVoucher(QList<CompoundItemDataObject> items,QString ledID="");
    void sendEmail();

    void createExcel();
    void exportToPurchase();

    void askForMail();
    void PurchaseVoucherSaved();
signals:
//    void closing();
//    void NotificationAccepted();
//    void NotificationRejected(QString reason);

private slots:
    void exportToReceiptNote();
    void ReceiptNoteSaved();

};

#endif // PURCHASEORDERVOUCHEREDITOR_H
