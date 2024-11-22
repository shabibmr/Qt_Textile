#ifndef BILLOFMATERIALSINGREDIENTSDATABASEHELPER_H
#define BILLOFMATERIALSINGREDIENTSDATABASEHELPER_H

#include "datamodels/Counter/restaurantingredientsdatamodel.h"
#include "database/databasehelper/databasehelper.h"
#include "datamodels/inventory/sales_inventory_item/compounditemdataobject.h"


#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMap>

class BillOfMaterialsIngredientsDataBaseHelper
{
public:
    BillOfMaterialsIngredientsDataBaseHelper();

    DatabaseHelper *db;
    bool flag=true;

    static QString Bill_Of_Materials_Ingredients_TableName ;
    static QString Bill_Of_Materials_Ingredients_id ;
    static QString Bill_Of_Materials_Ingredients_Sales_Item ;
    static QString Bill_Of_Materials_Ingredients_Sales_Item_Qty ;
    static QString Bill_Of_Materials_Ingredients_Purchase_Item ;
    static QString Bill_Of_Materials_Ingredients_Purchase_Item_Qty ;
    static QString Bill_Of_Materials_Ingredients_Purchase_Unit ;
    static QString Bill_Of_Materials_Ingredients_TakeAway ;

    int startTransaction();
    int insertBillOfMaterials(QList<RestaurantIngredientsDataModel> obj);
    int deleteBillOfMaterials(QString SalesInventoryItemID);
    static QList<RestaurantIngredientsDataModel> getBillOfMaterials(QString SalesInventoryItemID);
    int commit();

    QSqlQueryModel *getSalesItemByPurchaseItemModel(QString itemId);
    QList<CompoundItemDataObject> getBOMBySalesItemList(QList<CompoundItemDataObject> itemList);



};

#endif // BILLOFMATERIALSINGREDIENTSDATABASEHELPER_H
