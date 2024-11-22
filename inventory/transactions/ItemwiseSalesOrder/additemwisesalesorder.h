#ifndef ADDITEMWISESALESORDER_H
#define ADDITEMWISESALESORDER_H

#include <QWidget>
#include <datamodels/Counter/countersettingsdatamodel.h>
#include "database/inventory/salesorder/itemwisesalesorderdatabasehelper.h"
#include "customwidgets/TableViews/itemwiseentrytableview.h"
#include "customwidgets/SearchWidgets/Inventoryitem/Completer/gminventoryitemsearch.h"
#include "customwidgets/SearchWidgets/Employee/gmemployeesearchwidget.h"
#include "customwidgets/Delegates/Orders/gmitemwiseordersdelegate.h"
#include "customwidgets/Material/components/qtmaterialsnackbar.h"
#include "sharetools/sharetools.h"
#include "customwidgets/Material/components/qtmaterialappbar.h"
#include "customwidgets/Models/gmitemwiseordermodel.h"
#include "customwidgets/SearchWidgets/Ledger/gmledgerlinematerial.h"
#include "customwidgets/SearchWidgets/Godown/gmgodownlinematerial.h"
#include "customwidgets/SearchWidgets/Employee/gmemployeelinematerial.h"


#include "login/loginvalues.h"

#include "customwidgets/Basic/gmlineedit.h"
#include "customwidgets/Basic/gmdateedit.h"
#include "customwidgets/Basic/gmplaintextedit.h"

namespace Ui {
class AddItemwiseSalesOrder;

}



class AddItemwiseSalesOrder : public QWidget
{
    Q_OBJECT

public:
    explicit AddItemwiseSalesOrder(QString voucherItemwiseType,
                                   QString vType,
                                   VoucherDBAbstract *dbHelper,
                                   ItemwiseSalesOrderDatabaseHelper *db1Helper,
                                   QWidget *parent = nullptr);

    ~AddItemwiseSalesOrder();

    GeneralVoucherDataObject *voucher;

    LedgerMasterDatabaseHelper *ledHelper;
    UomDataBaseHelper *uomHelper;
    UserProfileDatabaseHelper *salesmanHelper;
    SalesInventoryItemDatabaseHelper *itemHelper;
    ItemwiseSalesOrderDatabaseHelper *db1Helper;
    VoucherDBAbstract *dbHelper;
    PricelistDatabaseHelper *priceHelper;
    AccountGroupMasterDatabaseHelper *accHelper;

    ShareTools *shareOptionsWidget;
    GMInventoryItemSearch *ItemSearchBox;
    GMEmployeeLineMaterial *SalesmanSearchBox;
    ItemwiseEntryTableView *DataTableWidget;
    GMItemwiseOrderModel *model;
    QtMaterialIconButton *shareButton;

    QString windowTitle="Itemwise Order Entry";
    QString bgcolor = "#e0f7fa";
    QColor appBarColor;

    QPointer<GMDateEdit> voucherDateWidget;
    QPointer<GMLineEdit> voucherNumberWidget;
    QPointer<GMPlainTextEdit> voucherNarrationWidget;

    QtMaterialFloatingActionButton *saveButton;
    GMGodownLineMaterial *priceListWidget;
    QtMaterialAppBar *appBar;

    QString backgroundColor = "#e0f7fa";
    QString textColor = "black";
    QString selectionColor = "#e0f7fa";
    QString highlightedColor = "#00acc1";
    QString headerBgColor= "#00bcd4"; //#0082c8
    QString iconColor = "white";
    QString titleColor = "white";

    QtMaterialIconButton *backButton;

    QString voucherType;
    QString voucherItemwiseType;

    QLabel *windowLabel;


    void initExtraActions();
    void initDbHelpers();
    void initVoucher();
    void initGlobalObjects();
    void setWidgets();
    void resetWidgets();
    void askForPrint();

    void setWindowTitle(const QString &value);

private slots:

    void saveVoucher();
//    void on_buttonBox_rejected();
//    void on_printButton_clicked();


public slots:

    void printVoucher();

    void setVoucher(GeneralVoucherDataObject *voucher);
    void showShareOptions();
signals:

    void closing();
private:
    Ui::AddItemwiseSalesOrder *ui;
};


#endif // ADDITEMWISESALESORDER_H
