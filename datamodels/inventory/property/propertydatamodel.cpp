#include "propertydatamodel.h"

PropertyDataModel::PropertyDataModel()
{

}

PropertyDataModel::~PropertyDataModel()
{
    //qDebug()<<"deleting property";
}

QJsonObject PropertyDataModel::ObjToJson(PropertyDataModel obj)
{
    QJsonObject jsonObject;
    jsonObject.insert("PropertyID",obj.PropertyID);
    jsonObject.insert("PropertyName",obj.PropertyName);

    QJsonArray propertyValueJArray;
    for(QString a:PropertyValue){
        propertyValueJArray.append(a);
    }
    jsonObject.insert("PropertyValue",propertyValueJArray);

    QJsonArray propertyNamesJArray;



    jsonObject.insert("PropertyNamesList",propertyNamesJArray);
    jsonObject.insert("PropertyUnits",obj.PropertyUnits);
    jsonObject.insert("itemProperty_ID",obj.itemProperty_ID);
    jsonObject.insert("ItemProperty_itemID",obj.ItemProperty_itemID);
    jsonObject.insert("ItemProperty_PropertyID",obj.ItemProperty_PropertyID);
    jsonObject.insert("ItemProperty_DefaultValues",obj.ItemProperty_DefaultValues);
    jsonObject.insert("numColums",obj.numColums);

    return jsonObject;

}

PropertyDataModel PropertyDataModel::JsonToObj(QJsonObject json)
{
    PropertyDataModel obj;

    obj.PropertyID = json["PropertyID"].toInt();
    obj.PropertyName = json["PropertyName"].toString();
    QJsonArray propertyValueJArray = json["PropertyValue"].toArray();
    for(QJsonValue a:propertyValueJArray){
            obj.PropertyValue.append(a.toString());
        }

    obj.PropertyUnits = json["PropertyUnits"].toString();
    obj.itemProperty_ID = json["itemProperty_ID"].toString();
    obj.ItemProperty_itemID = json["ItemProperty_itemID"].toString();
    obj.ItemProperty_PropertyID = json["ItemProperty_PropertyID"].toString();
    obj.ItemProperty_DefaultValues = json["ItemProperty_DefaultValues"].toString();
    obj.numColums = json["numColums"].toInt();

    return obj;
}
