#ifndef STOCKREQUESTEDITOR_H
#define STOCKREQUESTEDITOR_H

#include <QWidget>
#include <QPointer>
#include <QKeyEvent>
#include <QLabel>
#include <QSpacerItem>
#include <QMessageBox>
#include <QComboBox>

#include <login/loginvalues.h>
#include <datamodels/projects/Quotation/quotationstatus.h>
#include <datamodels/transaction/gmvouchertypes.h>

#include "customwidgets/TableViews/stockentrytableview.h"
#include "customwidgets/Models/gmstockrequestmodel.h"
#include "customwidgets/SearchWidgets/Ledger/gmledgerlinewidget.h"
#include "customwidgets/SearchWidgets/Ledger/gmledgerlinematerial.h"
#include "customwidgets/SearchWidgets/Employee/gmemployeelinematerial.h"
#include <customwidgets/Delegates/StockRequest/stockrequestdelegate.h>
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


#include "database/inventory/StockRequest/stockrequestdatabasehelper.h"
#include "CommonUi/Transactions/vouchereditorbase.h"


class StockRequestEditor : public VoucherEditorBase
{
    Q_OBJECT


    StockEntryTableView *DataTableWidget;                 //X

    QtMaterialIconButton *printButton;


    QPointer<GMComboBox> fromGodownWidget;
    QPointer<GMComboBox> toGodownWidget;
    QPointer<GMStockRequestModel> model;


    PrinterHelper *printHelper;

    QStringList lis;
public:
    explicit StockRequestEditor(VoucherDBAbstract *db, VoucherEditorBase *parent = nullptr);
    ~StockRequestEditor();

    void initExtraActions();

    void initDbHelpers();

    void initVoucher();

    void initGlobalObjects();

    void setWidgets();

    void resetWidgets() override;



public slots :
    void showVoucher() override;
//    void closeEditor();
    void saveVoucher();
//    void deleteVoucher();
    void askForPrint();
    void printVoucher();
    void importVoucher(GeneralVoucherDataObject *voucher2);
    void importItemsFromVoucher(GeneralVoucherDataObject *voucher2);
    void sendEmail();
    void importWorkOrder(GeneralVoucherDataObject *voucher);

signals:
//    void closing();
//    void NotificationAccepted();


};

#endif // STOCKREQUESTEDITOR_H
