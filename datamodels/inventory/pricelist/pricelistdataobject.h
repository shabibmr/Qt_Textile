#ifndef PRICELISTDATAOBJECT_H
#define PRICELISTDATAOBJECT_H
#include <datamodels/inventory/sales_inventory_item/inventoryitemdatamodel.h>

class PriceListDataObject
{
public:
    PriceListDataObject();
    ~PriceListDataObject();

    QList<inventoryItemDataModel> items;
    int priceListID;
    QString priceListName;
    QDate dateFrom;
    QDate dateto;
    bool isDefault = false;
    bool showinPOS = true;
};

#endif // PRICELISTDATAOBJECT_H
