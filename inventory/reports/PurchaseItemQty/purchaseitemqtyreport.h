#ifndef PURCHASEITEMQTYREPORT_H
#define PURCHASEITEMQTYREPORT_H

#include <QWidget>
#include <QSqlQueryModel>

#include "customwidgets/gmsearchlineedit.h"
#include "customwidgets/SearchWidgets/Inventoryitem/gmitemsearchlite.h"
#include "customwidgets/gmledgersearch.h"

#include "database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h"
#include "database/Counter/billofmaterialsingredientsdatabasehelper.h"
#include "customwidgets/test/gmitemsortfilterproxymodel.h"

namespace Ui {
class PurchaseItemQtyReport;
}

class PurchaseItemQtyReport : public QWidget
{
    Q_OBJECT

public:
    explicit PurchaseItemQtyReport(QWidget *parent = 0);
    ~PurchaseItemQtyReport();
    SalesInventoryItemDatabaseHelper *itemsHelper;
    BillOfMaterialsIngredientsDataBaseHelper *ingHelper;
    QSqlQueryModel *itemsModel;
    GMItemSearchProxyModel *sort;
    QString itemID;

private slots:
    void on_pushButton_clicked();
    void getItemDetails(inventoryItemDataModel);

private:
    Ui::PurchaseItemQtyReport *ui;

    void setTableView();

};

#endif // PURCHASEITEMQTYREPORT_H
