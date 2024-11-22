#ifndef INVENTORYLEDGERTRANSACTIONSEDITOR_H
#define INVENTORYLEDGERTRANSACTIONSEDITOR_H

#include <QWidget>
#include <QPointer>
#include <QKeyEvent>
#include <QLabel>
#include <QSpacerItem>
#include <QMessageBox>
#include <QInputDialog>
#include <QGroupBox>

#include "CommonUi/Transactions/vouchereditorbase.h"

#include "customwidgets/TableViews/inventoryvouchertableview.h"
#include "finance/transactions/LedgerView/ledgerwisevouchertableview.h"
#include "customwidgets/Models/gminventoryitemmodel.h"
#include <customwidgets/Delegates/Inventory/gminventorytransactiondelegate.h>


#include "sharetools/print/printerhelper.h"
#include "sharetools/print/printdotmatrix.h"

#include "sharetools/email/email.h"
#include "sharetools/excel/excel.h"
#include "sharetools/sharetools.h"

#include "database/finance/salesvoucher/salesvoucherdatabasehelper.h"
#include "customwidgets/showvoucherswidget.h"
#include "voucherOperations/coupledvoucherhandler.h"

//#include <QPainter>
//#include <QPaintEvent>



class InventoryLedgerTransactionsEditor : public VoucherEditorBase
{
    Q_OBJECT

    bool isCoupledVoucher = false;
    int coupledVoucherStatus ;
    int voucherType;
    int transType;

    InventoryVoucherTableView *DataTableWidget;                 //X
    GMInventoryTransactionDelegate *itemDelegate;

    LedgerwiseVoucherTableView* ledgerTable;

    QWidget* extraWidget;
    QtMaterialIconButton *importButton;
    QPointer<GMInventoryItemModel> model;

//    PrinterHelper *printHelper;

    CoupledVoucherHandler *coupledVoucherHandler;



public:
    explicit InventoryLedgerTransactionsEditor(int voucherType, VoucherDBAbstract *dbHelper,VoucherEditorBase *parent = nullptr);

    void initExtraActions();

    void initVoucher();

    void initGlobalObjects();

    void resetWidgets() override;

    void setMainLedger(LedgerMasterDataModel led) override;

    void setSalesMan(int user) override;

    void setGodown(QString godownId) override;

    void setVoucherType(int value);

public slots :
    void showVoucher() override;
    void saveVoucher() override;
    void printVoucher(QPrinter *printer) override;
    void exportToPdf(QPrinter *printer) override;
    void importVoucher(GeneralVoucherDataObject *voucher2) override;
    void importItemsFromVoucher(GeneralVoucherDataObject *voucher2) override;
    virtual void setBillwiseMapping();
    void setAdvancePaid(float amount) override;
    void getVoucherstoImport();
    void setPrices(QString priceList);
    void showItemDetail(int row);
    void calcTotals();
    void showCoupledWidget();

    void setDrawerLayout() override;



    void initCoupledWidget();
};

#endif // InventoryLedgerTransactionsEditor_H
