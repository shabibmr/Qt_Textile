#include "compounditemdataobject.h"

CompoundItemDataObject::CompoundItemDataObject()
{
    subItems.clear();
}

CompoundItemDataObject::~CompoundItemDataObject()
{

    //    qDebug()<<"Destroying compound";
}

QJsonObject CompoundItemDataObject::ObjToJson(CompoundItemDataObject obj)
{
    QJsonObject jsonObject;
//    qDebug()<<"INVENTORY ITEM OBJECT TO JSON";
    inventoryItemDataModel i;
    jsonObject.insert("BaseItem", i.objToJson(obj.BaseItem));
    QJsonArray subItemJArray;
    for(inventoryItemDataModel item:subItems){
        subItemJArray.append(i.objToJson(item));
    }
    jsonObject.insert("subItems",subItemJArray);
    jsonObject.insert("subItemsTotal", obj.subItemsTotal);
    return jsonObject;

}

QJsonObject CompoundItemDataObject::ObjPtrToJson(CompoundItemDataObject *obj)
{
    QJsonObject jsonObject;
//    qDebug()<<"INVENTORY ITEM OBJECT TO JSON";
    inventoryItemDataModel i;
    jsonObject.insert("BaseItem", i.objToJson(obj->BaseItem));
//    qDebug()<<jsonObject;
    QJsonArray subItemJArray;
    for(inventoryItemDataModel item:obj->subItems){
        subItemJArray.append(i.objToJson(item));
    }
    jsonObject.insert("subItems",subItemJArray);
    jsonObject.insert("subItemsTotal", obj->subItemsTotal);
    return jsonObject;
}

CompoundItemDataObject CompoundItemDataObject::JsonToObj(QJsonObject json)
{
    CompoundItemDataObject obj;
    inventoryItemDataModel i;

//    qDebug()<<"COMPOUND ITEM JSON TO OBJECT";
    obj.BaseItem = i.JsonToObj(json["BaseItem"].toObject());
    QJsonArray subItemJArray = json["subItems"].toArray();
    for(QJsonValue subItem:subItemJArray){
        obj.subItems.append(i.JsonToObj(subItem.toObject()));
    }
    obj.subItemsTotal = json["subItemsTotal"].toVariant().toFloat();

    return obj;

}
