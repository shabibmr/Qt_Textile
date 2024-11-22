#ifndef PROPERTYDATABASEHELPER_H
#define PROPERTYDATABASEHELPER_H


#include "datamodels/inventory/property/propertydatamodel.h"
#include "database/databasehelper/databasehelper.h"
#include <QSharedPointer>
#include <QMap>
#include <QDebug>

class PropertyDatabaseHelper
{
public:
    PropertyDatabaseHelper();
    ~PropertyDatabaseHelper();
    DatabaseHelper *db;

    bool flag = true;
    int startTransaction();

    int commit();

    QString Proprties_TableName = "Properties";
    QString Proprties_PropertyID = "Property_ID";
    QString Proprties_PropertyName = "Property_Name";
    QString Proprties_SelectType = "Select_Type";
    QString Proprties_DataType = "Data_Type";
    QString Proprties_Values = "Values";

    QString Item_Properties_TableName = "Sales_Inventory_Item_Properties";
    QString Item_Properties_id = "_id";
    QString Item_Properties_PropertyID = "Property_ID";
    QString Item_Properties_Item_ID = "Item_ID";

    QString Item_Properties_DefaultValue = "Default_Value";

    int insertProperty(PropertyDataModel *obj);
    int updateProperty(PropertyDataModel *obj);
    int deleteProperty(QString id);
    QString getMaxID();

    QList<PropertyDataModel> getAllProperties();

    QString getPropertyNameByID(QString id);
    QString getPropertyIDByName(QString name);

    QList<PropertyDataModel> getPropertiesOfItem(QString id);

    QSqlQueryModel *getPropertyMasterList();

    PropertyDataModel *getPropertyById(int id);

};

#endif // PROPERTYDATABASEHELPER_H
