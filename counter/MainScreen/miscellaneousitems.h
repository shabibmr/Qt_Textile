#ifndef MISCELLANEOUSITEMS_H
#define MISCELLANEOUSITEMS_H

#include "datamodels/inventory/sales_inventory_item/compounditemdataobject.h"
#include <database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h>
#include <QWidget>

namespace Ui {
class MiscellaneousItems;
}

class MiscellaneousItems : public QWidget
{
    Q_OBJECT

public:
    explicit MiscellaneousItems(QWidget *parent = 0);
    ~MiscellaneousItems();

    CompoundItemDataObject *miscItem;
    SalesInventoryItemDatabaseHelper *itemhelper;

private slots:
    void on_fruitsButton_clicked();

    void on_vegetablesButton_clicked();

    void on_OKButton_clicked();

    void on_lineEdit_returnPressed();

signals:
    void misc(CompoundItemDataObject);

private:
    Ui::MiscellaneousItems *ui;
};

#endif // MISCELLANEOUSITEMS_H
