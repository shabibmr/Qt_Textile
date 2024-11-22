#ifndef SALESINVENTORYGROUPSDATABASEHELPER_H
#define SALESINVENTORYGROUPSDATABASEHELPER_H

#include "datamodels/inventory/sales_inventory_group/inventoryitemgroupdatamodel.h"
#include "datamodels/Notification/notificationdatamodel.h"
#include "database/databasehelper/databasehelper.h"
#include "database/Notifications/sendchanneldatabasehelper.h"
#include "login/loginvalues.h"
#include <QSharedPointer>
#include <QSqlQueryModel>
#include <QString>
#include <QMap>
class SalesInventoryGroupsDatabaseHelper
{
public:

    DatabaseHelper *db;

    SalesInventoryGroupsDatabaseHelper(DatabaseHelper *db = new DatabaseHelper());
    ~SalesInventoryGroupsDatabaseHelper();

    bool flag = true;


    static QString Sales_Inventory_Group_TableName ;

    QString Sales_Inventory_Group_id="_id";
    QString Sales_Inventory_Group_GroupName = "Group_Name";
    QString Sales_Inventory_Group_GroupName_Arabic = "Group_Name_Arabic";
    QString Sales_Inventory_Group_GroupId = "Group_Id";
    QString Sales_Inventory_Group_ParentGroupID = "Parent_ID";
    QString Sales_Inventory_Group_GroupType="Group_Type";
    QString Sales_Inventory_Group_lastModified="Last_Modified";


    int insertSalesInventoryGroup(InventoryItemGroupDataModel *obj);

    int updateSalesInventoryGroup(InventoryItemGroupDataModel *obj, QString oldID);

    int deleteSalesInventoryGroup(QString oldID);

    int startTransaction();

    int commit();

    QString getMaxID();

    QMap<QString,QString> getSalesInventoryGroupDataAsMap();
    QMap<QString, QString> getSalesInventoryParentGroupAsMap(QString groupID);

    QMap<QString,QString> getSalesInventoryGroupDataAsMapForSales();

    QList<InventoryItemGroupDataModel> getALLgroupObjects();
    QSqlQueryModel *getAllInventoryGroupsModel();
    QList<QSharedPointer<InventoryItemGroupDataModel>> getALLgroupObjectsAsPtr();
    QString getGroupNameByID(QString id);

    InventoryItemGroupDataModel getInvGroupByID(QString id);
    InventoryItemGroupDataModel *getInvGroupPtrByID(QString id);


    QSqlQueryModel *getSalesInventoryGroupDataAsModel();
    QList<InventoryItemGroupDataModel> getSalesInventoryGroupDataAsMapForSalesAslist();
    bool checkItemExists(QString groupID);
};

#endif // SALESINVENTORYGROUPSDATABASEHELPER_H
