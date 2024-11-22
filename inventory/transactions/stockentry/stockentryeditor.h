#ifndef STOCKENTRYEDITOR_H
#define STOCKENTRYEDITOR_H

#include <QWidget>
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
#include "customwidgets/Models/gmstockentrymodel.h"
#include "customwidgets/SearchWidgets/Ledger/gmledgerlinewidget.h"
#include "customwidgets/SearchWidgets/Ledger/gmledgerlinematerial.h"
#include "customwidgets/SearchWidgets/Employee/gmemployeelinematerial.h"
#include <customwidgets/Delegates/StockEntry/stockentrydelegate.h>
#include "customwidgets/Material/components/qtmaterialfab.h"
#include "customwidgets/Material/components/qtmaterialsnackbar.h"
#include "customwidgets/Material/components/qtmaterialdrawer.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "customwidgets/Material/components/qtmaterialappbar.h"
#include "counter/MainScreen/itemdetail.h"

#include "customwidgets/Basic/gmlineedit.h"
#include "customwidgets/Basic/gmdateedit.h"
#include "customwidgets/Basic/gmplaintextedit.h"


#include "sharetools/print/printerhelper.h"
#include "sharetools/email/email.h"
#include "sharetools/excel/excel.h"

#include "CommonUi/Transactions/vouchereditorbase.h"

#include "database/inventory/stockentry/stockentrydatabasehelper.h"


namespace Ui {
class StockEntryEditor;
}

class StockEntryEditor : public VoucherEditorBase
{
    Q_OBJECT

    StockEntryTableView *DataTableWidget;                 //X
    QPointer<GMStockEntryModel> model;

    PrinterHelper *printHelper;

    QStringList lis;



public:
    explicit StockEntryEditor(VoucherDBAbstract *dbHelper, VoucherEditorBase *parent = nullptr);
    void initExtraActions();
    void initVoucher();
    void initGlobalObjects();
    void resetWidgets() override;

public slots :
    void showVoucher() override;
    void saveVoucher() override;
    void printVoucher();
    void importVoucher(GeneralVoucherDataObject *voucher2);
    void importItemsFromVoucher(GeneralVoucherDataObject *voucher2);
    void importAllStockItems();
    void sendEmail();


signals:
//    void closing();

private slots:
    void showItemDetail(int row);
private:
    Ui::StockEntryEditor *ui;
};

#endif // STOCKENTRYEDITOR_H
