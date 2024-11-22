#ifndef INVENTORYITEMIMAGESDATABASEHELPER_H
#define INVENTORYITEMIMAGESDATABASEHELPER_H

#include <QMap>
#include <QDebug>
#include <QImage>

#include "database/databasehelper/databasehelper.h"
#include "datamodels/inventory/sales_inventory_item/itemimagedatamodel.h"

class InventoryItemImagesDatabaseHelper
{
public:
    InventoryItemImagesDatabaseHelper();

    QString tablename ="sales_inventory_images";
    QString itemIDCol ="Item_Id";
    QString fileCol ="Image";
    QString width ="width";
    QString height="height";
    QString Remarks = "Narration";


    int startTransaction();

    int insertItemImage(ItemImageDatamodel image);

    int updateItemImage(ItemImageDatamodel image);

    int deleteitemImage(ItemImageDatamodel image);

    int commit();

    DatabaseHelper *db;

    bool flag = true;


    ItemImageDatamodel getItemImageAsQimage(ItemImageDatamodel itemImage);

};

#endif // INVENTORYITEMIMAGESDATABASEHELPER_H
