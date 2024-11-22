#ifndef GMITEMSEARCHLITE_H
#define GMITEMSEARCHLITE_H

#include <QObject>
#include <QLineEdit>
#include "datamodels/inventory/sales_inventory_item/inventoryitemdatamodel.h"
#include "database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h"

#include "QSqlQueryModel"
#include "gmitemsearchlistwidgetlite.h"


class GMItemSearchLite : public QLineEdit
{
    Q_OBJECT
public:
    explicit GMItemSearchLite(QSqlQueryModel *model,QLineEdit *parent=Q_NULLPTR);

    QStringList list;
    SalesInventoryItemDatabaseHelper *itemsHelper;

    QSqlQueryModel* itemsModel;
    bool accflag=false;

    GMItemSearchListWidgetLite *listWidget;
    void setGroupFlag(bool value);
    inventoryItemDataModel item;

public slots:
    void checkEdit();
    void editTextChanged(QString text);
    void itemSelected(QString,QString);
    void noneSelected();
    void itemCustomBarcodeSelected(inventoryItemDataModel item);

signals:

    void SelectedItemID(inventoryItemDataModel);
    void SelectedGroup(QString);
    void clearItem();
    void plusButton();
    void minusButton();
    void ItemSet();
    void blankenterpressed();
    void astriskpressed();
    void SalesOrderSelectedSignal(QString vno);
private slots:
    void SOSelectedSlot(QString Vno);
};

#endif // GMITEMSEARCHLITE_H
