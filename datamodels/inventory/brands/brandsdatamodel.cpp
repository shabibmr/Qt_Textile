#include "brandsdatamodel.h"

BrandsDataModel::BrandsDataModel()
{

}

QJsonObject BrandsDataModel::objToJson(BrandsDataModel obj)
{
    QJsonObject jsonObject;

    jsonObject.insert("_id",obj.brandID);
    jsonObject.insert("Brand_Name",obj.brandName);
    jsonObject.insert("Brand_Description",obj.brandDesc);

    return jsonObject;
}

QJsonObject BrandsDataModel::objPtrToJson(BrandsDataModel *obj)
{
    QJsonObject jsonObject;

    jsonObject.insert("_id",obj->brandID);
    jsonObject.insert("Brand_Name",obj->brandName);
    jsonObject.insert("Brand_Description",obj->brandDesc);

    return jsonObject;
}

BrandsDataModel BrandsDataModel::JsonToObj(QJsonObject json)
{
    BrandsDataModel obj ;

    obj.brandID= json["_id"].toInt();
    obj.brandName = json["Brand_Name"].toString();
    obj.brandDesc = json["Brand_Description"].toString();

    return obj;
}

BrandsDataModel *BrandsDataModel::JsonToObjPtr(QJsonObject json)
{
    BrandsDataModel *obj = new BrandsDataModel();

    obj->brandID= json["_id"].toInt();
    obj->brandName = json["Brand_Name"].toString();
    obj->brandDesc = json["Brand_Description"].toString();

    return obj;
}
