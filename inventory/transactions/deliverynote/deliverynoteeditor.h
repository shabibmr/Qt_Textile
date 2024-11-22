#ifndef DELIVERYNOTEEDITOR_H
#define DELIVERYNOTEEDITOR_H

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
#include "customwidgets/SearchWidgets/Godown/gmgodownlinematerial.h"
#include "counter/MainScreen/itemdetail.h"
#include "finance/transactions/sales/salesvouchereditor.h"



#include "customwidgets/Basic/gmlineedit.h"
#include "customwidgets/Basic/gmdateedit.h"
#include "customwidgets/Basic/gmplaintextedit.h"


#include "sharetools/print/printerhelper.h"
#include "sharetools/email/email.h"
#include "sharetools/excel/excel.h"


#include "database/inventory/deliverynote/deliverynotedatabasehelper.h"
#include "customwidgets/showvoucherswidget.h"

#include "CommonUi/Transactions/InventoryLedger/inventoryledgertransactionseditor.h"

class DeliveryNoteEditor : public InventoryLedgerTransactionsEditor
{
    Q_OBJECT


    DeliveryNoteDatabaseHelper *dbHelper;

    SalesVoucherEditor *SalesVoucherEditorWidget;
    ShowVouchersWidget *selectVoucherWidget;

public:
    explicit DeliveryNoteEditor(DeliveryNoteDatabaseHelper *db, InventoryLedgerTransactionsEditor *parent = nullptr);
    ~DeliveryNoteEditor();

    void initExtraActions();
    void setDrawerLayout() override;
public slots :
    void saveVoucher() override;

    void importVoucher(GeneralVoucherDataObject *voucher2);
//    void importItemsFromVoucher(GeneralVoucherDataObject *voucher2);
    void exportVoucher();
    void sendEmail();
    void noVoucherToImport();

    void getVoucherstoImport();

signals:
//    void closing();
//    void NotificationAccepted();
//    void NotificationRejected(QString reason);
//    void saveCompleted();
private slots:
    void SalesVoucherSaved();


    // VoucherEditorBase interface
public:
    virtual void printVoucher(QPrinter *printer) override;
};

#endif // DELIVERYNOTEEDITOR_H
