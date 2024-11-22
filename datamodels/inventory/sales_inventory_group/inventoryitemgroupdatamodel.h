#ifndef INVENTORYITEMGROUPDATAMODEL_H
#define INVENTORYITEMGROUPDATAMODEL_H

#include <QString>
#include <QJsonObject>
#include <QJsonArray>

class InventoryItemGroupDataModel
{
public:
    InventoryItemGroupDataModel();
    ~InventoryItemGroupDataModel();

    QString _id;
    QString GroupID;
    QString GroupName;
    QString GroupNameArabic;
    QString ParentGroupId;
    QString ParentGroupName;
    QString GroupType;

    bool fromExternal = false;
    int action;

    QJsonObject objToJson(InventoryItemGroupDataModel obj);
    QJsonObject objPtrToJson(InventoryItemGroupDataModel *obj);
    InventoryItemGroupDataModel JsonToObj(QJsonObject json);
    InventoryItemGroupDataModel *JsonToObjPtr(QJsonObject json);

};

#endif // INVENTORYITEMGROUPDATAMODEL_H
