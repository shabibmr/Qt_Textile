#ifndef SALESVOUCHEREDITOR_H
#define SALESVOUCHEREDITOR_H

#include <QWidget>
#include <QPointer>
#include <QKeyEvent>
#include <QLabel>
#include <QSpacerItem>
#include <QMessageBox>
#include <QInputDialog>
#include <QGroupBox>

#include "CommonUi/Transactions/vouchereditorbase.h"

#include <login/loginvalues.h>
#include <datamodels/projects/Quotation/quotationstatus.h>
#include <datamodels/transaction/gmvouchertypes.h>
#include "datamodels/transaction/requirementvoucherdataobject.h"

#include "customwidgets/TableViews/inventoryvouchertableview.h"
#include "customwidgets/Models/gminventoryitemmodel.h"
#include "customwidgets/SearchWidgets/Ledger/gmledgerlinewidget.h"
#include "customwidgets/SearchWidgets/Ledger/gmledgerlinematerial.h"
#include "customwidgets/SearchWidgets/Employee/gmemployeelinematerial.h"
#include "customwidgets/SearchWidgets/Godown/gmgodownlinematerial.h"
#include "customwidgets/SearchWidgets/Pricelist/gmpricelistlinematerial.h"

#include <customwidgets/Delegates/Inventory/gminventorytransactiondelegate.h>
#include "customwidgets/Material/components/qtmaterialfab.h"
#include "customwidgets/Material/components/qtmaterialsnackbar.h"
#include "customwidgets/Material/components/qtmaterialdrawer.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "customwidgets/Material/components/qtmaterialappbar.h"
#include "customwidgets/Basic/Material/gmmaterialcombobox.h"
#include "finance/transactions/LedgerView/ledgerwisevouchertableview.h"

#include "customwidgets/Basic/gmlineedit.h"
#include "customwidgets/Basic/gmdateedit.h"
#include "customwidgets/Basic/gmplaintextedit.h"

#include "sharetools/print/printerhelper.h"
#include "sharetools/print/printdotmatrix.h"

#include "sharetools/email/email.h"
#include "sharetools/excel/excel.h"
#include "sharetools/sharetools.h"

#include "database/finance/salesvoucher/salesvoucherdatabasehelper.h"
#include "database/Notifications/receivechanneldatabasehelper.h"
#include "customwidgets/showvoucherswidget.h"
#include "voucherOperations/coupledvoucherhandler.h"

#include "counter/MainScreen/addressdialog.h"

#include "database/inventory/godown/godowndatabasehelper.h"

#include "database/voucherdbabstract.h"
#include "CommonUi/Transactions/InventoryLedger/inventoryledgertransactionseditor.h"

//#include <QPainter>
//#include <QPaintEvent>

class SalesVoucherEditor : public InventoryLedgerTransactionsEditor
{
    Q_OBJECT

public:
    explicit SalesVoucherEditor(VoucherDBAbstract *dbHelper,InventoryLedgerTransactionsEditor *parent = nullptr);
//    ~SalesVoucherEditor();

public slots :
    void saveVoucher() override;
    void printVoucher(QPrinter *printer) override;
    void exportToPdf(QPrinter *printer) override;
    void importVoucher(GeneralVoucherDataObject *voucher2);
//    void importItemsFromVoucher(GeneralVoucherDataObject *voucher2);


signals:


private slots:
    void getVoucherstoImport();

};

#endif // SALESVOUCHEREDITOR_H
