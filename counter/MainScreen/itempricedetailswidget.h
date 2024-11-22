#ifndef ITEMPRICEDETAILSWIDGET_H
#define ITEMPRICEDETAILSWIDGET_H

#include "customwidgets/Basic/Material/gmmaterialnumberedit.h"
#include "database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h"

#include <QWidget>

namespace Ui {
class ItemPriceDetailsWidget;
}

class ItemPriceDetailsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ItemPriceDetailsWidget(CompoundItemDataObject *Item,
                                    SalesInventoryItemDatabaseHelper *itemHelper,
                                    QWidget *parent = nullptr);
    ~ItemPriceDetailsWidget();

    CompoundItemDataObject *item;
    SalesInventoryItemDatabaseHelper *itemHelper;

    GMMaterialNumberEdit *stdCostEdit;
    GMMaterialNumberEdit *lppEdit;
    GMMaterialNumberEdit *lspEdit;

    float stdCost = 0;
    float lpp = 0;
    float lsp = 0;

    void setWidgets();
    void setValues();

private:
    Ui::ItemPriceDetailsWidget *ui;
};

#endif // ITEMPRICEDETAILSWIDGET_H
