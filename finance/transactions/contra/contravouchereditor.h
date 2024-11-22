#ifndef CONTRAVOUCHEREDITOR_H
#define CONTRAVOUCHEREDITOR_H

#include <QWidget>
#include <QPointer>
#include <QKeyEvent>
#include <QLabel>
#include <QSpacerItem>
#include <QMessageBox>

#include <login/loginvalues.h>
#include <datamodels/projects/Quotation/quotationstatus.h>
#include <datamodels/transaction/gmvouchertypes.h>

#include "customwidgets/TableViews/ledgervouchertableview.h"
#include "customwidgets/Models/gmpaymententrymodel.h"

#include "customwidgets/SearchWidgets/Ledger/gmledgerlinewidget.h"
#include "customwidgets/SearchWidgets/Ledger/gmledgerlinematerial.h"
#include "customwidgets/SearchWidgets/Employee/gmemployeesearchwidget.h"
#include <customwidgets/Delegates/Ledgers/gmledgertransactiondelegate.h>
#include "customwidgets/Material/components/qtmaterialfab.h"
#include "customwidgets/Material/components/qtmaterialsnackbar.h"
#include "customwidgets/Material/components/qtmaterialdrawer.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "customwidgets/Material/components/qtmaterialappbar.h"



#include "customwidgets/Basic/gmlineedit.h"
#include "customwidgets/Basic/gmdateedit.h"
#include "customwidgets/Basic/gmplaintextedit.h"


#include "sharetools/print/printerhelper.h"
#include "sharetools/email/email.h"
#include "sharetools/excel/excel.h"


#include "database/finance/contra/contravoucherdatabasehelper.h"
#include "CommonUi/Transactions/vouchereditorbase.h"

namespace Ui {
class ContraVoucherEditor;
}

class ContraVoucherEditor : public VoucherEditorBase
{
    Q_OBJECT

//    QString windowTitle="Contra Voucher";
//    QString bgcolor = "#e0f7fa";
//    QColor appBarColor;

    QList<int> avlcols {1,2};

//    GeneralVoucherDataObject* voucher;


//    LedgerMasterDatabaseHelper *ledgerHelper;
//    ContraVoucherDatabaseHelper *dbHelper;
//    UomDataBaseHelper *uomHelper;
//    UserProfileDatabaseHelper *userHelper;
//    SalesInventoryItemDatabaseHelper *itemsHelper;

    LedgervoucherTableView *DataTableWidget;                 //X

//    GMEmployeeSearchWidget *SalesmanSearchBox;
//    GMLedgerLineMaterial *mainLedgerWidget;

//    QtMaterialFloatingActionButton *saveButton;
//    QtMaterialIconButton *backButton;
//    QtMaterialFloatingActionButton *shareButton;
//    QtMaterialIconButton *printButton;
//    QtMaterialAppBar *appBar;

//    QtMaterialSnackbar *snackBar;
//    QtMaterialDrawer *drawer;


//    QPointer<GMDateEdit> voucherDateWidget;
//    QPointer<GMLineEdit> voucherNumberWidget;
//    QPointer<GMPlainTextEdit> voucherNarrationWidget;
    QPointer<GMPaymentEntryModel> model;

//    PrinterHelper *printHelper;

    QStringList lis;

public:
    explicit ContraVoucherEditor(VoucherDBAbstract *db, QWidget *parent = nullptr);
//    ~ContraVoucherEditor();

    void initExtraActions();

//    void initDbHelpers();

    void initVoucher();

    void initGlobalObjects();

    void resetWidgets() override;



public slots :
    void showVoucher() override;
    void saveVoucher() override;
    void printVoucher(QPrinter *printer) override;

    void sendEmail();


signals:

private:
    Ui::ContraVoucherEditor *ui;
};

#endif // CONTRAVOUCHEREDITOR_H
