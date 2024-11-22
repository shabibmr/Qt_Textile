#include "categorydatamodel.h"

CategoryDataModel::CategoryDataModel()
{

}

QJsonObject CategoryDataModel::objToJson(CategoryDataModel obj)
{
    QJsonObject jsonObject;

    jsonObject.insert("Category_id",obj.CategoryID);
    jsonObject.insert("Category_Name",obj.CategoryName);
    jsonObject.insert("Category_Description",obj.CategoryDesc);

    return jsonObject;
}

QJsonObject CategoryDataModel::objPtrToJson(CategoryDataModel *obj)
{
    QJsonObject jsonObject;

    jsonObject.insert("Category_id",obj->CategoryID);
    jsonObject.insert("Category_Name",obj->CategoryName);
    jsonObject.insert("Category_Description",obj->CategoryDesc);

    return jsonObject;
}

CategoryDataModel CategoryDataModel::JsonToObj(QJsonObject json)
{
    CategoryDataModel obj ;

    obj.CategoryID= json["Category_id"].toInt();
    obj.CategoryName = json["Category_Name"].toString();
    obj.CategoryDesc = json["Category_Description"].toString();

    return obj;
}

CategoryDataModel *CategoryDataModel::JsonToObjPtr(QJsonObject json)
{
    CategoryDataModel *obj = new CategoryDataModel();

    obj->CategoryID= json["Category_id"].toInt();
    obj->CategoryName = json["Category_Name"].toString();
    obj->CategoryDesc = json["Category_Description"].toString();

    return obj;
}
