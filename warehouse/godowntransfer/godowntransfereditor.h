#ifndef GODOWNTRANSFEREDITOR_H
#define GODOWNTRANSFEREDITOR_H

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
#include "customwidgets/Models/gmgodowntransfermodel.h"
#include "customwidgets/SearchWidgets/Ledger/gmledgerlinewidget.h"
#include "customwidgets/SearchWidgets/Ledger/gmledgerlinematerial.h"
#include "customwidgets/SearchWidgets/Employee/gmemployeesearchwidget.h"

#include "customwidgets/SearchWidgets/Godown/gmgodownlinematerial.h"


#include <customwidgets/Delegates/StockJournal/stockjournaldelegate.h>
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


#include "database/inventory/godowntransfer/godowntransferdatabasehelper.h"

namespace Ui {
class GodownTransferEditor;
}

class GodownTransferEditor : public QWidget
{
    Q_OBJECT
    QString windowTitle="Godown Transfer";
    GeneralVoucherDataObject* voucher;
    QString bgcolor = "#e0f7fa";
    QColor appBarColor;

    GodownTransferDatabaseHelper *dbHelper;
    UomDataBaseHelper *uomHelper;
    UserProfileDatabaseHelper *userHelper;
    SalesInventoryItemDatabaseHelper *itemsHelper;
    GodownDatabaseHelper *godownHelper;

    StockEntryTableView *DataTableWidget;                 //X

    GMEmployeeSearchWidget *SalesmanSearchBox;

    GMGodownLineMaterial *FromGodown;
    GMGodownLineMaterial *ToGodown;



//    GMLedgerLineMaterial *mainLedgerWidget;

    QtMaterialFloatingActionButton *saveButton;
    QtMaterialIconButton *backButton;
    QtMaterialFloatingActionButton *shareButton;
    QtMaterialIconButton *printButton;
    QtMaterialAppBar *appBar;

    QtMaterialSnackbar *snackBar;
    QtMaterialDrawer *drawer;

    QPointer<GMDateEdit> voucherDateWidget;
    QPointer<GMLineEdit> voucherNumberWidget;
    QPointer<GMPlainTextEdit> voucherNarrationWidget;
    QPointer<GMGodownTransferModel> model;

    PrinterHelper *printHelper;

    QStringList lis;

public:
    explicit GodownTransferEditor(QWidget *parent = nullptr);
    ~GodownTransferEditor();

    void initExtraActions();

    void initDbHelpers();

    void initVoucher();

    void initGlobalObjects();

    void setWidgets();

    void resetWidgets();

public slots :
    void showVoucher(GeneralVoucherDataObject *obj);
    void closeEditor();
    void saveVoucher();
    void deleteVoucher();
    void askForPrint();
    void printVoucher();
    void importVoucher(GeneralVoucherDataObject *voucher2);
    void importItemsFromVoucher(GeneralVoucherDataObject *voucher2);

    void sendEmail();


signals:
    void closing();

private:
    Ui::GodownTransferEditor *ui;
};

#endif // GODOWNTRANSFEREDITOR_H
