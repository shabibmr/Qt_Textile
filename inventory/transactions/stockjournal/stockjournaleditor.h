#ifndef STOCKJOURNALEDITOR_H
#define STOCKJOURNALEDITOR_H

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
#include "customwidgets/Models/gmstockjournalmodel.h"
#include "customwidgets/SearchWidgets/Ledger/gmledgerlinewidget.h"
#include "customwidgets/SearchWidgets/Ledger/gmledgerlinematerial.h"
#include "customwidgets/SearchWidgets/Employee/gmemployeelinematerial.h"
#include <customwidgets/Delegates/StockJournal/stockjournaldelegate.h>
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


#include "database/inventory/stockjournal/stockjournaldatabasehelper.h"
#include "CommonUi/Transactions/vouchereditorbase.h"



class StockJournalEditor : public VoucherEditorBase
{
    Q_OBJECT


    StockEntryTableView *DataTableWidget;                 //X

    QPointer<GMStockJournalModel> model;

    PrinterHelper *printHelper;

    QStringList lis;

public:
    explicit StockJournalEditor(VoucherDBAbstract *db, VoucherEditorBase *parent = nullptr);
    ~StockJournalEditor();

    void initExtraActions();

    void initVoucher();

    void initGlobalObjects();

    void resetWidgets() override;

public slots :
    void showVoucher() override;
//    void closeEditor();
    void saveVoucher() override;
//    void deleteVoucher();
    void askForPrint();
    void printVoucher();
    void importVoucher(GeneralVoucherDataObject *voucher2);
    void importItemsFromVoucher(GeneralVoucherDataObject *voucher2);

    void sendEmail();


    void showItemDetail(int row);
signals:
//    void closing();


};

#endif // STOCKJOURNALEDITOR_H
