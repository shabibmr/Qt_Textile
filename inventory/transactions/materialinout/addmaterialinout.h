#ifndef ADDMATERIALINOUT_H
#define ADDMATERIALINOUT_H

#include <QWidget>
#include <QPointer>
#include <QKeyEvent>
#include <QStandardItemModel>
#include <QToolButton>
#include "projects/requirements/additemrequirement.h"
#include "database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h"
#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"
#include "datamodels/transaction/generalvoucherdataobject.h"
#include "database/inventory/materialinout/materialinoutdatabasehelper.h"

namespace Ui {
class AddMaterialInOut;
}

class AddMaterialInOut : public QWidget
{
    Q_OBJECT

public:
    explicit AddMaterialInOut(QWidget *parent = 0);
    ~AddMaterialInOut();
    //widgets
    addItemRequirement *addItemRequirementWidget;

    //common
    QString projectID="";
    QString vNo = "";
    QString vPrefix = "";

    //databases
    GeneralVoucherDataObject *voucher;
    materialinoutDatabaseHelper *dbHelper;
    SalesInventoryItemDatabaseHelper *itemsHelper;
    LedgerMasterDatabaseHelper *ledgerHelper;

    //models
    QStandardItemModel* model;
    QStandardItemModel* modelLedger;

    //item specific
    QList<inventoryItemDataModel> itemsList;
    QList<QSharedPointer<inventoryItemDataModel>> itemsListPtr;

    QList<CompoundItemDataObject> ordersList;
    QList<LedgerMasterDataModel> allLedgers;
    QStringList NamesForCompleter;
    QStringList NamesForLineEdit;
    QStringList LedNames;
    bool openNewWidget = true;\

    //ledger specific
    QList <LedgerMasterDataModel> ordersLedList;
    QList<QSharedPointer<LedgerMasterDataModel>> allLedgersPtr ;
    bool openNewWidgetLedger = true;

private slots:
    void on_addItemPushButton_clicked();
    void keyPressEvent(QKeyEvent *e);
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_itemTableWidget_doubleClicked(const QModelIndex &index);

public slots:
    void setTitle(QString Text);
    void setProjectID(QString projID);
    void showVoucher(GeneralVoucherDataObject voucher);
    void setItemTable();
    void getItemDetails(inventoryItemDataModel obj);
    void appendItem(CompoundItemDataObject item,int row);
    void delButtonClicked(bool);
    void calcTotal();
    void resetFlag(int);
    void setLedgerTable();
    void getLedgerDetails(int i);
    void deleteLedgerButtonClicked(bool);
    void appendLedger(LedgerMasterDataModel item,int row);
    void calcLedgerTotal();
    void resetLedgerWidgetFlag();

signals:
    void closing();

private:
    Ui::AddMaterialInOut *ui;
};

#endif // ADDMATERIALINOUT_H
