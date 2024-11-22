#ifndef RESTAURANTINGREDIENTSDATAMODEL_H
#define RESTAURANTINGREDIENTSDATAMODEL_H

#include <QString>
#include <QJsonObject>

class RestaurantIngredientsDataModel
{
public:
    RestaurantIngredientsDataModel();

    int _id;
    QString SalesItem;
    float SalesItemQty;
    QString PurchaseItem;
    float PurchaseItemQty;
    bool TakeAway;
    QString PurchaseUom;
    float conValue=1;
    QString purchaseItemSection;


    QJsonObject objToJson(RestaurantIngredientsDataModel obj);
    RestaurantIngredientsDataModel JsonToObj(QJsonObject json);
};

#endif // RESTAURANTINGREDIENTSDATAMODEL_H
