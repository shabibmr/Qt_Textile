#ifndef QUOTATIONEDITOR_H
#define QUOTATIONEDITOR_H

#include <QWidget>
#include <QPointer>
#include <QKeyEvent>
#include <QLabel>
#include <QSpacerItem>
#include <QMessageBox>

#include <login/loginvalues.h>
#include <datamodels/projects/Quotation/quotationstatus.h>
#include <datamodels/transaction/gmvouchertypes.h>

#include "customwidgets/TableViews/inventoryvouchertableview.h"
#include "customwidgets/Models/gminventoryitemmodel.h"
#include "customwidgets/SearchWidgets/Ledger/gmledgerlinewidget.h"
#include "customwidgets/SearchWidgets/Ledger/gmledgerlinematerial.h"
#include "customwidgets/SearchWidgets/Employee/gmemployeelinematerial.h"
#include <customwidgets/Delegates/Inventory/gminventorytransactiondelegate.h>
#include <customwidgets/Delegates/Inventory/gminventorylistitemdelegate.h>

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

#include <QSplitter>

#include <QIdentityProxyModel>


#include "database/finance/proforma/proformadatabasehelper.h"
namespace Ui {
class QuotationEditor;
}

class QuotationEditor : public QWidget
{
    Q_OBJECT

    QString windowTitle = "Quotation";
    GeneralVoucherDataObject* voucher;
    QString bgcolor = "#e0f7fa";
    QColor appBarColor;


    LedgerMasterDatabaseHelper *ledgerHelper;
    ProformaDatabaseHelper *dbHelper;
    UomDataBaseHelper *uomHelper;
    UserProfileDatabaseHelper *userHelper;
    SalesInventoryItemDatabaseHelper *itemsHelper;

    InventoryVoucherTableView *DataTableWidget;

    QListView *listView;//X

    GMEmployeeLineMaterial *SalesmanSearchBox;
    GMLedgerLineMaterial *mainLedgerWidget;

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
    QPointer<GMInventoryItemModel> model;

    PrinterHelper *printHelper;

    QStringList lis;
public:
    explicit QuotationEditor(QWidget *parent = nullptr);
    ~QuotationEditor();

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
    Ui::QuotationEditor *ui;
};


class ListProxyModel : public QIdentityProxyModel
{
    Q_OBJECT


public:

    ListProxyModel(GeneralVoucherDataObject *voucher){this->voucher=voucher;}
    virtual int columnCount(const QModelIndex& idx) const { return 1; }

    virtual QVariant data(const QModelIndex& idx, int role) const {
        qDebug()<<idx.row()<<idx.column();
        if(!checkIndex(idx))
            return QVariant();
        if(idx.row()>voucher->InventoryItems.size())
            return QVariant();
        if(role == Qt::DisplayRole ||role == Qt::EditRole)
            return QVariant::fromValue(voucher->InventoryItems[idx.row()].BaseItem.ItemName);
        if(role == Qt::UserRole+9)
             return QVariant::fromValue(voucher->InventoryItems[idx.row()].BaseItem);

        return QVariant();
    }

    virtual int rowCount(const QModelIndex &parent) const {return voucher->InventoryItems.size();}
private:
    GeneralVoucherDataObject *voucher;

    // QAbstractItemModel interface


};


#endif // QUOTATIONEDITOR_H
