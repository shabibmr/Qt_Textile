#include "accountgroupdatamodel.h"

AccountGroupDataModel::AccountGroupDataModel()
{

}

AccountGroupDataModel *AccountGroupDataModel::JsonToObjPtr(QJsonObject json)
{

    AccountGroupDataModel *obj = new AccountGroupDataModel();

    obj->groupName = json["Group_Name"].toString();
    obj->groupNameArabic = json["groupNameArabic"].toString();
    obj->groupID = json["Group_ID"].toString();
    obj->groupType = json["Group_Type"].toString();
    obj->groupAlias = json["groupAlias"].toString();
    obj->category = json["category"].toString();
    obj->parentGroupId = json["Parent_ID"].toString();
    obj->parentGroupName = json["parentGroupName"].toString();

    return obj;

}

QJsonObject AccountGroupDataModel::objToJson(AccountGroupDataModel obj)
{
    QJsonObject jsonObject;

    jsonObject.insert("Group_Name",obj.groupName);
    jsonObject.insert("groupNameArabic",obj.groupNameArabic);
    jsonObject.insert("Group_ID",obj.groupID);
    jsonObject.insert("Group_Type",obj.groupType);
    jsonObject.insert("groupAlias",obj.groupAlias);
    jsonObject.insert("ParentGroupName",obj.parentGroupName);
    jsonObject.insert("Parent_ID",obj.parentGroupId);
    jsonObject.insert("category",obj.category);

    return jsonObject;
}

QJsonObject AccountGroupDataModel::objPtrToJson(AccountGroupDataModel *obj)
{
    QJsonObject jsonObject;

    jsonObject.insert("Group_Name",obj->groupName);
    jsonObject.insert("groupNameArabic",obj->groupNameArabic);
    jsonObject.insert("Group_ID",obj->groupID);
    jsonObject.insert("Group_Type",obj->groupType);
    jsonObject.insert("groupAlias",obj->groupAlias);
    jsonObject.insert("ParentGroupName",obj->parentGroupName);
    jsonObject.insert("Parent_ID",obj->parentGroupId);
    jsonObject.insert("category",obj->category);

    return jsonObject;
}
