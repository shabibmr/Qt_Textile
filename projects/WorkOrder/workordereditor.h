#ifndef WORKORDEREDITOR_H
#define WORKORDEREDITOR_H

#include <QWidget>
#include <QPointer>
#include <QKeyEvent>
#include <QLabel>
#include <QSpacerItem>
#include <QMessageBox>

#include <login/loginvalues.h>
#include <datamodels/projects/Quotation/quotationstatus.h>
#include <datamodels/transaction/gmvouchertypes.h>

#include "customwidgets/TableViews/stockentrytableview.h"
#include "customwidgets/Models/gmworkordermodel.h"
#include "customwidgets/SearchWidgets/Ledger/gmledgerlinewidget.h"
#include "customwidgets/SearchWidgets/Ledger/gmledgerlinematerial.h"
#include "customwidgets/SearchWidgets/Employee/gmemployeelinematerial.h"
#include <customwidgets/Delegates/WorkOrder/workorderdelegate.h>

#include "customwidgets/Material/components/qtmaterialfab.h"
#include "customwidgets/Material/components/qtmaterialsnackbar.h"
#include "customwidgets/Material/components/qtmaterialdrawer.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "customwidgets/Material/components/qtmaterialappbar.h"

#include "customwidgets/Basic/gmlineedit.h"
#include "customwidgets/Basic/gmdateedit.h"
#include "customwidgets/Basic/gmplaintextedit.h"
#include "customwidgets/gmcombobox.h"

#include "sharetools/print/printerhelper.h"
#include "sharetools/email/email.h"
#include "sharetools/excel/excel.h"

#include "database/inventory/workorder/workorderdatabasehelper.h"
#include "inventory/transactions/StockRequest/stockrequesteditor.h"
#include "customwidgets/showvoucherswidget.h"
#include "customwidgets/barcodeprintwidget.h"
#include "customwidgets/itemchecklistwidget.h"

#include "CommonUi/Transactions/vouchereditorbase.h"



class WorkOrderEditor : public VoucherEditorBase
{
    Q_OBJECT

    bool itemsChecked = false;

    GeneralVoucherDataObject* v1;
//    bool itemsChecked = false;


    GeneralVoucherDatabaseHelper *vHelper;

    QTabWidget* tablesTab;
    StockEntryTableView *DataTableWidget;                 //X
    StockEntryTableView *table;


    QtMaterialFloatingActionButton *completedButton;
    QtMaterialFloatingActionButton *undoButton;

     ShowVouchersWidget *selectVoucherWidget;
//    QtMaterialIconButton *printButton;

    QPointer<GMWorkOrderModel> model;

    QPointer<GMWorkOrderModel> model2;

    PrinterHelper *printHelper;

    QStringList lis;

public:
    explicit WorkOrderEditor(VoucherDBAbstract *dbhelper, VoucherEditorBase *parent = nullptr);
    ~WorkOrderEditor();

    void initExtraActions();

//    void initDbHelpers();

    void initVoucher();

    void initGlobalObjects();

//    void setWidgets();

    void resetWidgets() override;



public slots :
    void showVoucher() override;
//    void closeEditor() override;
    void saveVoucher() override;
//    void deleteVoucher() override;
    void askForPrint();
    void exportVoucher();
    void getVouchersToImport();
    void printVoucher();
    void importVoucher(GeneralVoucherDataObject *voucher2);
    void importItemsFromVoucher(GeneralVoucherDataObject *voucher2);
    void sendEmail();
    void markCompleted();


    void undoTask();
    void restSummaryTable();
    void printBarcode(int row);
    void showItemChecklistDialog();
signals:
//    void closing();
//    void NotificationAccepted();

private slots:
    void noVoucherToImport();
//    void showItemChecklistDialog();


    // VoucherEditorBase interface
public:
    virtual void setDrawerLayout() override;
};

#endif // WORKORDEREDITOR_H
