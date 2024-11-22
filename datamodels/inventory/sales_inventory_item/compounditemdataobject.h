#ifndef COMPOUNDITEMDATAOBJECT_H
#define COMPOUNDITEMDATAOBJECT_H

#include <QDebug>

#include "datamodels/inventory/sales_inventory_item/inventoryitemdatamodel.h"


class CompoundItemDataObject
{
public:
    CompoundItemDataObject();
    ~CompoundItemDataObject();


    inventoryItemDataModel BaseItem;
    QList<inventoryItemDataModel> subItems;
    float subItemsTotal = 0;

    bool fromExternal = false;
    int Action;

    QJsonObject ObjToJson(CompoundItemDataObject obj);
    QJsonObject ObjPtrToJson(CompoundItemDataObject *obj);
    CompoundItemDataObject JsonToObj(QJsonObject json);

};

Q_DECLARE_METATYPE(CompoundItemDataObject)
#endif // COMPOUNDITEMDATAOBJECT_H
