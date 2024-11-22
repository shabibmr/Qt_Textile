#include "itemlabeldatamodel.h"

ItemLabelDataModel::ItemLabelDataModel()
{

}

void ItemLabelDataModel::convertNutritionFactsJson(ItemLabelDataModel *obj)
{
    obj->Nutritions.insert("Energy", obj->Energy);
    obj->Nutritions.insert("Carbohydrate", obj->Carbohydrate);
    obj->Nutritions.insert("Fat", obj->Fat);
    obj->Nutritions.insert("Protein", obj->Protein);
    obj->Nutritions.insert("Sugar", obj->Sugar);
    obj->Nutritions.insert("TransFat", obj->TransFat) ;
    obj->Nutritions.insert("SaturatedFat", obj->SaturatedFat);
    obj->Nutritions.insert("Cholestrol", obj->Cholestrol);
    obj->Nutritions.insert("CrudeFibre", obj->CrudeFibre);
    obj->Nutritions.insert("MonoUnsaturatedFat", obj->MonoUnsaturatedFat);
    obj->Nutritions.insert("PolyUnsaturatedFat", obj->PolyUnsaturatedFat);
    obj->Nutritions.insert("DietaryFibre", obj->DietaryFibre);

}

void ItemLabelDataModel::parseNutritionFactsJson(ItemLabelDataModel *obj)
{
    obj->Energy = obj->Nutritions["Energy"].toString();
    obj->Carbohydrate = obj->Nutritions["Carbohydrate"].toString();
    obj->Fat = obj->Nutritions["Fat"].toString();
    obj->Protein = obj->Nutritions["Protein"].toString();
    obj->Sugar = obj->Nutritions["Sugar"].toString();
    obj->TransFat = obj->Nutritions["TransFat"].toString();
    obj->SaturatedFat = obj->Nutritions["SaturatedFat"].toString();
    obj->Cholestrol = obj->Nutritions["Cholestrol"].toString();
    obj->CrudeFibre = obj->Nutritions["CrudeFibre"].toString();
    obj->MonoUnsaturatedFat= obj->Nutritions["MonoUnsaturatedFat"].toString();
    obj->PolyUnsaturatedFat= obj->Nutritions["PolyUnsaturatedFat"].toString();
    obj->DietaryFibre= obj->Nutritions["DietaryFibre"].toString();

}

