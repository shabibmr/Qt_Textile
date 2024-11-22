#ifndef RECEIPTNOTEVOUCHEREDITOR_H
#define RECEIPTNOTEVOUCHEREDITOR_H

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
#include "customwidgets/SearchWidgets/Godown/gmgodownlinematerial.h"
#include "customwidgets/Material/components/qtmaterialfab.h"
#include "customwidgets/Material/components/qtmaterialsnackbar.h"
#include "customwidgets/Material/components/qtmaterialdrawer.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "customwidgets/Material/components/qtmaterialappbar.h"
#include "finance/transactions/purchases/purchasevouchereditor.h"
#include "counter/MainScreen/itemdetail.h"


#include "customwidgets/Basic/gmlineedit.h"
#include "customwidgets/Basic/gmdateedit.h"
#include "customwidgets/Basic/gmplaintextedit.h"


#include "sharetools/print/printerhelper.h"
#include "sharetools/email/email.h"
#include "sharetools/excel/excel.h"


#include "database/inventory/receiptnote/receiptnotedatabasehelper.h"
#include "customwidgets/showvoucherswidget.h"

#include "CommonUi/Transactions/InventoryLedger/inventoryledgertransactionseditor.h"


class ReceiptNoteVoucherEditor : public InventoryLedgerTransactionsEditor
{
    Q_OBJECT


    PrinterHelper *printHelper;

    bool itemsChecked = false;

public:
    explicit ReceiptNoteVoucherEditor(VoucherDBAbstract *db, InventoryLedgerTransactionsEditor *parent = nullptr);
    ~ReceiptNoteVoucherEditor();

void setDrawerLayout() override;

public slots :
    void saveVoucher() override;

    void exportVoucher();


    void showItemChecklistDialog();
signals:
//    void closing();
//    void NotificationAccepted();
//    void NotificationRejected(QString reason);
//    void saveCompleted();

};

#endif // RECEIPTNOTEVOUCHEREDITOR_H
