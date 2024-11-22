#ifndef WASTAGEENTRYEDITOR_H
#define WASTAGEENTRYEDITOR_H

#include <QWidget>
#include <QComboBox>
#include <QPointer>
#include <QKeyEvent>
#include <QLabel>
#include <QSpacerItem>
#include <QMessageBox>

#include <login/loginvalues.h>
#include <datamodels/projects/Quotation/quotationstatus.h>
#include <datamodels/transaction/gmvouchertypes.h>

#include "customwidgets/TableViews/stockentrytableview.h"
#include "customwidgets/Models/gmwastageentrymodel.h"
#include "customwidgets/SearchWidgets/Ledger/gmledgerlinewidget.h"
#include "customwidgets/SearchWidgets/Ledger/gmledgerlinematerial.h"
#include "customwidgets/SearchWidgets/Employee/gmemployeelinematerial.h"
#include <customwidgets/Delegates/StockEntry/stockentrydelegate.h>
#include "customwidgets/Material/components/qtmaterialfab.h"
#include "customwidgets/Material/components/qtmaterialsnackbar.h"
#include "customwidgets/Material/components/qtmaterialdrawer.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "customwidgets/Material/components/qtmaterialappbar.h"

#include "customwidgets/Basic/gmlineedit.h"
#include "customwidgets/Basic/gmdateedit.h"
#include "customwidgets/Basic/gmplaintextedit.h"
#include "customwidgets/gmcombobox.h"
#include "customwidgets/Basic/Material/gmmaterialcombobox.h"
#include "counter/MainScreen/itemdetail.h"


#include "sharetools/print/printerhelper.h"
#include "sharetools/email/email.h"
#include "sharetools/excel/excel.h"


#include "database/inventory/defective/defectivedatabasehelper.h"
#include "CommonUi/Transactions/vouchereditorbase.h"


class WastageEntryEditor : public VoucherEditorBase
{
    Q_OBJECT

    StockEntryTableView *DataTableWidget;                 //X

    QPointer<GMWastageEntryModel> model;
    PrinterHelper *printHelper;

    QStringList lis;

public:
    explicit WastageEntryEditor(VoucherDBAbstract *db, VoucherEditorBase *parent = nullptr);
    ~WastageEntryEditor();
    void initExtraActions();

//    void initDbHelpers();

    void initVoucher();

    void initGlobalObjects();

//    void setWidgets();

    void resetWidgets() override;



public slots :
    void showItemDetail(int row);

    void showVoucher() override;
//    void closeEditor();
    void saveVoucher() override;
//    void deleteVoucher();
    void askForPrint();
    void printVoucher();
    void importVoucher(GeneralVoucherDataObject *voucher2);
    void importItemsFromVoucher(GeneralVoucherDataObject *voucher2);

    void sendEmail();


signals:
//    void closing();

};

#endif // WASTAGEENTRYEDITOR_H
