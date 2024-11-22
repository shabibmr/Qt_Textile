#ifndef MATERIALINOUTEDITOR_H
#define MATERIALINOUTEDITOR_H

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



#include "customwidgets/Basic/gmlineedit.h"
#include "customwidgets/Basic/gmdateedit.h"
#include "customwidgets/Basic/gmplaintextedit.h"


#include "sharetools/print/printerhelper.h"
#include "sharetools/email/emailthread.h"
#include "sharetools/excel/excelreporthelper.h"


#include "database/inventory/materialinout/materialinoutdatabasehelper.h"



namespace Ui {
class MaterialInOutEditor;
}

class MaterialInOutEditor : public QWidget
{
    Q_OBJECT

    QString windowTitle="Material In/out";
    GeneralVoucherDataObject* voucher;
    QString bgcolor = "#e0f7fa";
    QColor appBarColor;


    LedgerMasterDatabaseHelper *ledgerHelper;
    materialinoutDatabaseHelper *dbHelper;
    UomDataBaseHelper *uomHelper;
    UserProfileDatabaseHelper *userHelper;
    SalesInventoryItemDatabaseHelper *itemsHelper;

    StockEntryTableView *DataTableWidget;                 //X

    GMEmployeeLineMaterial *SalesmanSearchBox;
    GMLedgerLineMaterial *mainLedgerWidget;

    QtMaterialFloatingActionButton *saveButton;
    QtMaterialFloatingActionButton *rejectButton;
    QtMaterialIconButton *backButton;
    QtMaterialFloatingActionButton *shareButton;
    QtMaterialIconButton *printButton;
    QtMaterialIconButton *emailButton;

    QtMaterialAppBar *appBar;

    QtMaterialSnackbar *snackBar;
    QtMaterialDrawer *drawer;


    QPointer<GMDateEdit> voucherDateWidget;
    QPointer<GMLineEdit> voucherNumberWidget;
    QPointer<GMPlainTextEdit> voucherNarrationWidget;
    QPointer<GMStockJournalModel> model;

    PrinterHelper *printHelper;

    QStringList lis;

public:
    explicit MaterialInOutEditor(QWidget *parent = nullptr);
    ~MaterialInOutEditor();
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
    Ui::MaterialInOutEditor *ui;
};

#endif // MATERIALINOUTEDITOR_H
