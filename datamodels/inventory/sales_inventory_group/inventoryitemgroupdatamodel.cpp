#include "inventoryitemgroupdatamodel.h"

InventoryItemGroupDataModel::InventoryItemGroupDataModel()
{

}

InventoryItemGroupDataModel::~InventoryItemGroupDataModel()
{

}

QJsonObject InventoryItemGroupDataModel::objToJson(InventoryItemGroupDataModel obj)
{
    QJsonObject jsonObject;

    jsonObject.insert("_id",obj._id);
    jsonObject.insert("GroupID",obj.GroupID);
    jsonObject.insert("GroupName",obj.GroupName);
    jsonObject.insert("GroupNameArabic",obj.GroupNameArabic);
    jsonObject.insert("ParentGroupId",obj.ParentGroupId);
    jsonObject.insert("ParentGroupName",obj.ParentGroupName);
    jsonObject.insert("GroupType",obj.GroupType);

    return jsonObject;

}

QJsonObject InventoryItemGroupDataModel::objPtrToJson(InventoryItemGroupDataModel *obj)
{
    QJsonObject jsonObject;

    jsonObject.insert("_id",obj->_id);
    jsonObject.insert("GroupID",obj->GroupID);
    jsonObject.insert("GroupName",obj->GroupName);
    jsonObject.insert("GroupNameArabic",obj->GroupNameArabic);
    jsonObject.insert("ParentGroupId",obj->ParentGroupId);
    jsonObject.insert("ParentGroupName",obj->ParentGroupName);
    jsonObject.insert("GroupType",obj->GroupType);

    return jsonObject;

}

InventoryItemGroupDataModel *InventoryItemGroupDataModel::JsonToObjPtr(QJsonObject json)
{
    InventoryItemGroupDataModel *obj = new InventoryItemGroupDataModel();

    obj->_id = json["_id"].toString();
    obj->GroupID = json["GroupID"].toString();
    obj->GroupName = json["GroupName"].toString();
    obj->GroupNameArabic = json["GroupNameArabic"].toString();
    obj->ParentGroupId = json["ParentGroupId"].toString();
    obj->ParentGroupName = json["ParentGroupName"].toString();
    obj->GroupType = json["GroupType"].toString();
    return obj;

}
