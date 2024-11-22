#ifndef PURCHASEVOUCHEREDITOR_H
#define PURCHASEVOUCHEREDITOR_H

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
#include "customwidgets/SearchWidgets/Godown/gmgodownlinematerial.h"
#include <customwidgets/Delegates/Inventory/gminventorytransactiondelegate.h>
#include "customwidgets/Material/components/qtmaterialfab.h"
#include "customwidgets/Material/components/qtmaterialsnackbar.h"
#include "customwidgets/Material/components/qtmaterialdrawer.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "customwidgets/Material/components/qtmaterialappbar.h"
#include "finance/transactions/LedgerView/ledgerwisevouchertableview.h"


#include "customwidgets/Basic/gmlineedit.h"
#include "customwidgets/Basic/gmdateedit.h"
#include "customwidgets/Basic/gmplaintextedit.h"
#include "customwidgets/checklistpromptdialog.h"

#include "CommonUi/Transactions/InventoryLedger/inventoryledgertransactionseditor.h"



#include "sharetools/print/printerhelper.h"
#include "sharetools/email/email.h"
#include "sharetools/excel/excel.h"


#include "database/finance/purchasevoucher/purchasevoucherdatabasehelper.h"
#include "database/Notifications/receivechanneldatabasehelper.h"
#include "customwidgets/showvoucherswidget.h"
#include "voucherOperations/coupledvoucherhandler.h"

#include "customwidgets/Material/components/qtmaterialdialog.h"



class PurchaseVoucherEditor : public InventoryLedgerTransactionsEditor
{
    Q_OBJECT


    bool itemsChecked = false;
    bool stat1 = false;


    PrinterHelper *printHelper;

    QStringList lis;

    // CoupledVoucherHandler *coupledVoucherHandler;


public:
    explicit PurchaseVoucherEditor(VoucherDBAbstract *db, InventoryLedgerTransactionsEditor *parent = nullptr);
    ~PurchaseVoucherEditor();




public slots :
    void saveVoucher();
    void printVoucher();
    void sendEmail();
    void setBillwiseMapping() override;
//    void getVoucherstoImport();
    void setTabs();
//    void setMainLedger(LedgerMasterDataModel led);
//    void setSalesMan(int id);

signals:
//    void closing();
//    void saveCompleted();
//    void NotificationAccepted();
//    void NotificationRejected(QString reason);

private slots:
//    void showItemDetail(int row);
    void showItemChecklistDialog();

};

#endif // PURCHASEVOUCHEREDITOR_H
