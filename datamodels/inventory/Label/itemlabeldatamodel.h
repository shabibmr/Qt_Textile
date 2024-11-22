#ifndef ITEMLABELDATAMODEL_H
#define ITEMLABELDATAMODEL_H

#include <QString>
#include <QJsonObject>

class ItemLabelDataModel
{
public:
    ItemLabelDataModel();


    int id;
    QString Item_Id;
    QJsonObject Nutritions ;
    float MRP ;
    QString AllergenAdvice ;
    float NetWeight ;
    QString Unit ;
    QString Ingredients ;
    bool isVeg ;

    QString Energy;
    QString Carbohydrate;
    QString Fat;
    QString Protein;
    QString Sugar;
    QString TransFat;
    QString SaturatedFat;
    QString Cholestrol;
    QString CrudeFibre;
    QString MonoUnsaturatedFat;
    QString PolyUnsaturatedFat;
    QString DietaryFibre;

    void convertNutritionFactsJson(ItemLabelDataModel *obj);
    void parseNutritionFactsJson(ItemLabelDataModel *obj);

};

#endif // ITEMLABELDATAMODEL_H
