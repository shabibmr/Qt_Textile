#ifndef ADDITEMREQUIREMENT_H
#define ADDITEMREQUIREMENT_H

#include <QWidget>
#include <QCloseEvent>
#include "datamodels/inventory/sales_inventory_item/compounditemdataobject.h"
#include "database/inventory/pricelist/pricelistdatabasehelper.h"
#include "database/inventory/property/propertydatabasehelper.h"
#include "database/Reports/inventoryhelper.h"

#include <QStandardItem>
#include <QComboBox>
#include <QCompleter>
#include "customwidgets/gmsearchlineedit.h"
#include "customwidgets/gmitemsearch.h"
#include <QToolButton>
#include <QPointer>
#include <QDoubleValidator>
#include "customwidgets/Validator/myvalidator.h"
#include <QAction>

namespace Ui {
class addItemRequirement;
}

class addItemRequirement : public QWidget
{
    Q_OBJECT

public:

    explicit addItemRequirement(QWidget *parent = 0);
    ~addItemRequirement();
    CompoundItemDataObject Item;
    inventoryItemDataModel baseItem;
    PropertyDatabaseHelper *propHelper;
    PricelistDatabaseHelper *priceListHelper;
    UomDataBaseHelper *uomHelper;
    QList<UomConversionDataModel> UomConvList;
    float convalue =1;
    QString uomId;

    InventoryHelper iv;
    InventoryHelper* invHelper;

    qint64 RequirementID=0;

    float maxQuantity=1000000;


    bool priceEdit = true;

    QList<inventoryItemDataModel> subOrdersList;
    QStringList NamesForCompleter;
    QStringList NamesForLineEdit;
    QStandardItemModel* model;


    SalesInventoryItemDatabaseHelper *itemsHelper;
    bool openNewWidget = true;
    QString projectID="";
    int itemListNumber;

    void setItemsListPtr(const QList<QSharedPointer<inventoryItemDataModel> > value);

    qint64 getRequirementID() const;

    void setRequirementID(qint64 value);


    void setMaxQuantity(float value);

private slots:
    void on_buttonBox_rejected();

    void on_buttonBox_accepted();
    void on_AddSubItemButton_clicked();

    void on_quantityLineEdit_textChanged(const QString &arg1);

    void on_priceLineEdit_textChanged(const QString &arg1);

    void on_discountLineEdit_textChanged(const QString &arg1);

    void on_itemTotalLineEdit_returnPressed();

    void closeEvent(QCloseEvent *e);

    void on_subItemsTable_doubleClicked(const QModelIndex &index);

    void showEvent(QShowEvent* event);

    void on_lengthLineEdit_textChanged(const QString &arg1);

    void on_optionalCheckBox_stateChanged(int arg1);

    void on_rowCount_valueChanged(int arg1);

    void on_vatPercent_editingFinished();

    void on_quantityLineEdit_returnPressed();

    void on_priceLineEdit_returnPressed();

    void on_discountLineEdit_returnPressed();

    void on_vatPercent_returnPressed();

    void uomIndexChanged(int index);
signals:

    void closing(int);
    void addNewItem(CompoundItemDataObject ,int);
    void insertNewItem(CompoundItemDataObject ,int);

public slots:
    void setTitle(QString Text);
    void hideSubItemsTab(bool val=true);
    void setItem(CompoundItemDataObject obj);
    void setProperty(QString ItemId);
    void edit(CompoundItemDataObject obj);
    void appendSubItem(CompoundItemDataObject obj,int row);
    void setListNumber(int r);
    void calcTotal();

    void showOptional();
    void calcSubItemsTotal();
    void getFirstItemDetail(inventoryItemDataModel obj);

    void setSubItemTable();
    void getSubItemDetails(inventoryItemDataModel obj);
    void appendItem(CompoundItemDataObject item,int row);
    void setProjectID(QString projID);
    void delButtonClicked(bool);
    void resetFlag(int);

    void DisablePriceEdit(bool);
    void DisableItemUpdate(bool);

    void disableDesriptionEdit(bool);

private:
    Ui::addItemRequirement *ui;
};

#endif // ADDITEMREQUIREMENT_H
