#ifndef ITEMLABELDATABASEHELPER_H
#define ITEMLABELDATABASEHELPER_H

#include <QString>
#include "datamodels/inventory/Label/itemlabeldatamodel.h"
#include "database/databasehelper/databasehelper.h"

class ItemLabelDatabaseHelper
{
public:
    ItemLabelDatabaseHelper();

    QString Item_Label_Data_TableName = "Item_Label_Data";

    QString Item_Label_Data_id = "_id";
    QString Item_Label_Data_ItemID = "Item_Id";
    QString Item_Label_Data_Nutritions = "Nutritions";
    QString Item_Label_Data_MRP = "MRP";
    QString Item_Label_Data_AllergenAdvice = "Allergen_Advice";
    QString Item_Label_Data_NetWeight = "Net_Weight";
    QString Item_Label_Data_Unit = "Unit";
    QString Item_Label_Data_Ingredients = "Ingredients";
    QString Item_Label_Data_isVeg = "Is_Veg";


    DatabaseHelper *db;
    bool flag=true;

    int startTransaction();
    int insertLabelData(ItemLabelDataModel* obj);
    int updateLabelData(ItemLabelDataModel* obj);
    int deleteLabelData(QString SalesInventoryItemID);
    ItemLabelDataModel *getLabelData(QString SalesInventoryItemID);
    int commit();



};

#endif // ITEMLABELDATABASEHELPER_H
