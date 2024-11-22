#include "restaurantingredientsdatamodel.h"
#include <QVariant>

RestaurantIngredientsDataModel::RestaurantIngredientsDataModel()
{

}

QJsonObject RestaurantIngredientsDataModel::objToJson(RestaurantIngredientsDataModel obj)
{
    QJsonObject jsonObject;

    jsonObject.insert("SalesItem", obj.SalesItem);
    jsonObject.insert("SalesItemQty",  QString::number(obj.SalesItemQty,'f',2));
    jsonObject.insert("PurchaseItem", obj.PurchaseItem);
    jsonObject.insert("PurchaseItemQty",  QString::number(obj.PurchaseItemQty,'f',2));
    jsonObject.insert("TakeAway", obj.TakeAway);
    jsonObject.insert("PurchaseUom", obj.PurchaseUom);
    jsonObject.insert("conValue", QString::number(obj.conValue,'f',2));
    jsonObject.insert("purchaseItemSection", obj.purchaseItemSection);

    return jsonObject;
}

RestaurantIngredientsDataModel RestaurantIngredientsDataModel::JsonToObj(QJsonObject json)
{
    RestaurantIngredientsDataModel obj;

    obj.SalesItem = json["SalesItem"].toString();
    obj.SalesItemQty = json["SalesItemQty"].toVariant().toFloat();
    obj.PurchaseItem = json["PurchaseItem"].toString();
    obj.PurchaseItemQty = json["PurchaseItemQty"].toVariant().toFloat();
    obj.TakeAway = json["TakeAway"].toBool();
    obj.PurchaseUom = json["PurchaseUom"].toString();
    obj.conValue = json["conValue"].toVariant().toFloat();
    obj.purchaseItemSection = json["purchaseItemSection"].toString();

    return obj;

}
