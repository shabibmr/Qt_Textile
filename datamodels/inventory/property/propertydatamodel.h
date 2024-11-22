#ifndef PROPERTYDATAMODEL_H
#define PROPERTYDATAMODEL_H

#include <QStringList>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

class PropertyDataModel
{
public:

    PropertyDataModel();
    ~PropertyDataModel();

    int PropertyID=0;
    QString PropertyName;
    QString PropertyUnits;

    QStringList PropertyValue;

    QString itemProperty_ID;
    QString ItemProperty_itemID;
    QString ItemProperty_PropertyID;
    QString ItemProperty_DefaultValues;

    int numColums;

    QJsonObject ObjToJson(PropertyDataModel obj);
    PropertyDataModel JsonToObj(QJsonObject obj);
};

#endif // PROPERTYDATAMODEL_H
