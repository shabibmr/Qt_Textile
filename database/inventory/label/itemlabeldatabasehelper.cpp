#include "itemlabeldatabasehelper.h"

ItemLabelDatabaseHelper::ItemLabelDatabaseHelper()
{
    db = new DatabaseHelper();
}

int ItemLabelDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

int ItemLabelDatabaseHelper::insertLabelData(ItemLabelDataModel *obj)
{
    QMap<QString,QVariant> map;

    map.insert(Item_Label_Data_ItemID,obj->Item_Id);
    map.insert(Item_Label_Data_Nutritions, obj->Nutritions);
    map.insert(Item_Label_Data_Ingredients, obj->Ingredients);
    map.insert(Item_Label_Data_AllergenAdvice, obj->AllergenAdvice);
    map.insert(Item_Label_Data_MRP,obj->MRP);
    map.insert(Item_Label_Data_isVeg,obj->isVeg);
    map.insert(Item_Label_Data_NetWeight, obj->NetWeight);
    map.insert(Item_Label_Data_Unit,obj->Unit);

    db->insertRecordsPlus(map,Item_Label_Data_TableName);

}

int ItemLabelDatabaseHelper::updateLabelData(ItemLabelDataModel *obj)
{

    QMap<QString,QVariant> map;
    QMap<QString,QVariant> map2;

    map2.insert(Item_Label_Data_ItemID,obj->Item_Id);

    obj->convertNutritionFactsJson(obj);

    map.insert(Item_Label_Data_Nutritions, obj->Nutritions);
    map.insert(Item_Label_Data_Ingredients, obj->Ingredients);
    map.insert(Item_Label_Data_AllergenAdvice, obj->AllergenAdvice);
    map.insert(Item_Label_Data_MRP,obj->MRP);
    map.insert(Item_Label_Data_isVeg,obj->isVeg);
    map.insert(Item_Label_Data_NetWeight, obj->NetWeight);
    map.insert(Item_Label_Data_Unit,obj->Unit);

    db->updateRecordsPlus(map,map2,Item_Label_Data_TableName);
}

int ItemLabelDatabaseHelper::deleteLabelData(QString SalesInventoryItemID)
{

}

ItemLabelDataModel *ItemLabelDatabaseHelper::getLabelData(QString SalesInventoryItemID)
{

}

int ItemLabelDatabaseHelper::commit()
{
    if(flag){
        db->commitTransaction();
        return 1;
    }
    else{
        db->rollBackTransaction();
        return 0;
    }

}
