#include "salesinventorygroupsdatabasehelper.h"
#include <QMessageBox>
#include "database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h"

QString SalesInventoryGroupsDatabaseHelper::Sales_Inventory_Group_TableName = "Sales_Inventory_Groups";

SalesInventoryGroupsDatabaseHelper::SalesInventoryGroupsDatabaseHelper(DatabaseHelper *db)
{
    this->db = db;
}

SalesInventoryGroupsDatabaseHelper::~SalesInventoryGroupsDatabaseHelper()
{
    //    delete db;
}

int SalesInventoryGroupsDatabaseHelper::insertSalesInventoryGroup(InventoryItemGroupDataModel *obj)
{
    QMap<QString,QVariant> map;

    db->startTransaction();

    if(obj->GroupID == ""){
        obj->GroupID = obj->ParentGroupId+DatabaseValues::idGen+getMaxID();
    }

    map.insert(Sales_Inventory_Group_GroupId,obj->GroupID);
    map.insert(Sales_Inventory_Group_GroupName,obj->GroupName);
    map.insert(Sales_Inventory_Group_GroupName_Arabic,obj->GroupNameArabic);
    map.insert(Sales_Inventory_Group_ParentGroupID,obj->ParentGroupId);
    map.insert(Sales_Inventory_Group_GroupType,obj->GroupType);
    map.insert(Sales_Inventory_Group_lastModified, QDateTime::currentDateTime());
    db->insertRecordsPlus(map,Sales_Inventory_Group_TableName);

    return commit();

}


int SalesInventoryGroupsDatabaseHelper::updateSalesInventoryGroup(InventoryItemGroupDataModel *obj, QString oldID)
{

    QMap<QString,QVariant> map;

    db->startTransaction();

    map.insert(Sales_Inventory_Group_GroupId,obj->GroupID);
    map.insert(Sales_Inventory_Group_GroupName,obj->GroupName);
    map.insert(Sales_Inventory_Group_GroupName_Arabic,obj->GroupNameArabic);
    map.insert(Sales_Inventory_Group_ParentGroupID,obj->ParentGroupId);
    map.insert(Sales_Inventory_Group_GroupType,obj->GroupType);
    map.insert(Sales_Inventory_Group_lastModified, QDateTime::currentDateTime());


    QMap<QString,QVariant> map1;

    map1.insert(Sales_Inventory_Group_GroupId,oldID);

    db->updateRecordsPlus(map,map1,Sales_Inventory_Group_TableName);

    return commit();
}

int SalesInventoryGroupsDatabaseHelper::deleteSalesInventoryGroup(QString oldID)
{
    QMap<QString,QVariant> map1;
    db->startTransaction();
    QSqlQuery q(QSqlDatabase::database(DatabaseValues::connectionString));
    q.exec("select _id FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where Group_Id='"+oldID+"'");
    if(q.size()>0){
        QMessageBox box;box.setText("Inventory Group is Used.\nCannot Delete Group!!!"); box.exec();
        return 0;
    }
    map1.insert(Sales_Inventory_Group_id,oldID);

    db->deleteRecordsPlus(map1,Sales_Inventory_Group_TableName);

    return commit();
}

int SalesInventoryGroupsDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

int SalesInventoryGroupsDatabaseHelper::commit()
{
    if(flag){
        if(db->commitTransaction())
            return true;
        else
            return false;
    }
    else{
        db->rollBackTransaction();
        return 0;
    }
}

QString SalesInventoryGroupsDatabaseHelper::getMaxID()
{
    QSqlDatabase db = QSqlDatabase::database(DatabaseValues::connectionString);
    QSqlQuery query(db);
    int id =0;
    QString qStr = "select max(_id) from "+Sales_Inventory_Group_TableName;
    query.exec(qStr);
    while(query.next()){
        id = query.value(0).toInt();
    }
    return QString::number(id+1);
}



QMap<QString, QString> SalesInventoryGroupsDatabaseHelper::getSalesInventoryGroupDataAsMap()
{
    QMap<QString, QString> InventoryGroups;

    QString qStr = "SELECT "+Sales_Inventory_Group_GroupId+" , "+Sales_Inventory_Group_GroupName+
            "  FROM "+Sales_Inventory_Group_TableName+" order by "+Sales_Inventory_Group_GroupName;
    // qDebug()<<qStr;
    QSqlDatabase db = QSqlDatabase::database(DatabaseValues::connectionString);
    QSqlQuery query(db);
    query.prepare(qStr);
    query.exec();
    while (query.next()) {
        InventoryGroups.insert(query.value(0).toString(),query.value(1).toString());
    }

    return InventoryGroups;
}

QMap<QString, QString> SalesInventoryGroupsDatabaseHelper::getSalesInventoryParentGroupAsMap(QString groupID)
{
//    QString qStr1 = " SELECT T1.groupID, T1.group_name FROM (";
//   qStr1 += "  SELECT  @r AS groupID, " + Sales_Inventory_Group_GroupName + ", ";
//   qStr1 += "  (SELECT @r := " + Sales_Inventory_Group_ParentGroupID + " FROM ";
//   qStr1 += Sales_Inventory_Group_TableName + " WHERE " + Sales_Inventory_Group_GroupId + " = groupID) AS parent_id, ";
//   qStr1 += " @l := @l + 1 AS lvl FROM ";
//   qStr1 += " (SELECT @r := '" + groupID + "' , @l := 0) vars, " + Sales_Inventory_Group_TableName + " m ";
//   qStr1 += "     WHERE @r>=0 ) T1 ";
//   qStr1 += " WHERE T1.lvl >1 ";
//   qStr1 += " ORDER BY T1.lvl DESC";

   QString qStr = " Select " + Sales_Inventory_Group_GroupId + "," + Sales_Inventory_Group_GroupName ;
   qStr += " from " +  Sales_Inventory_Group_TableName + " where " + Sales_Inventory_Group_GroupId + " not in ";
   qStr += " (SELECT " +  Sales_Inventory_Group_GroupId + " FROM ";
   qStr += " (SELECT * FROM " + Sales_Inventory_Group_TableName + " ORDER BY " + Sales_Inventory_Group_ParentGroupID ;
   qStr += " , " +  Sales_Inventory_Group_GroupId + ") GROUPS_SORTED, ";
   qStr += " (SELECT @PV := '" + groupID + "') INITIALISATION ";
   qStr += " WHERE   FIND_IN_SET(PARENT_ID, @PV) ";
   qStr += " AND     LENGTH(@PV := CONCAT(@PV, ',', GROUP_ID)) ";
   qStr += " union";
   qStr += " select " + Sales_Inventory_Group_GroupId + " from " + Sales_Inventory_Group_TableName ;
   qStr += " where " + Sales_Inventory_Group_GroupId + " = '" + groupID + "' )";
   qStr += " order by 2";



   qStr = "select group_id, group_name from " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +"  where Group_ID not in(";
   qStr += " select group_id from ";
   qStr += " (with recursive cte (group_id, group_name, parent_id) as (";
   qStr += "     select     grp.group_id,grp.group_name,grp.parent_id";
   qStr += "     from " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +"  grp";
   qStr += "     union all ";
   qStr += "     select     p.group_id, p.group_name,p.parent_id";
   qStr += "     from       " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +"  p";
   qStr += "     inner join cte ";
   qStr += "     on p.group_id = cte.parent_id )";
   qStr += "    select  cte.group_id, group_name";
   qStr += "    from cte where cte.parent_id = '" + groupID + "'";
   qStr += "  UNION";
   qStr += "  select group_id, group_name from " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +"  where group_id = '" + groupID + "'";
   qStr += "    )itemGrp1";
   qStr += "    )  ";
   qStr += " ORDER BY group_name asc";


    QMap<QString, QString> InventoryGroups;

//    QString qStr = "SELECT "+Sales_Inventory_Group_GroupId+" , "+Sales_Inventory_Group_GroupName+
//            "  FROM "+Sales_Inventory_Group_TableName+" order by "+Sales_Inventory_Group_GroupName;
     qDebug()<<qStr;
    QSqlDatabase db = QSqlDatabase::database(DatabaseValues::connectionString);
    QSqlQuery query(db);
    query.prepare(qStr);
    query.exec();
    while (query.next()) {
        InventoryGroups.insert(query.value(0).toString(),query.value(1).toString());
    }

    qDebug()<<InventoryGroups;
    return InventoryGroups;

}

QSqlQueryModel* SalesInventoryGroupsDatabaseHelper::getSalesInventoryGroupDataAsModel()
{

    QString qStr = "SELECT "+Sales_Inventory_Group_GroupId+" , "
            +Sales_Inventory_Group_GroupName+
            ",'','','','',''  FROM "+Sales_Inventory_Group_TableName+" order by "+Sales_Inventory_Group_GroupName;

    return DatabaseHelper::getQueryModel(qStr);;
}



QMap<QString, QString> SalesInventoryGroupsDatabaseHelper::getSalesInventoryGroupDataAsMapForSales()
{
    QMap<QString, QString> InventoryGroups;

    QString qStr = "SELECT "+Sales_Inventory_Group_GroupId+" , "+Sales_Inventory_Group_GroupName+
            "  FROM "+Sales_Inventory_Group_TableName;
    qStr +=" where ";
    qStr += Sales_Inventory_Group_GroupType + "= 2";

    qStr +=" order by "+Sales_Inventory_Group_GroupName;
    // qDebug()<<qStr;
    QSqlDatabase db = QSqlDatabase::database(DatabaseValues::connectionString);
    QSqlQuery query(db);
    query.prepare(qStr);
    query.exec();
    while (query.next()) {
        InventoryGroups.insert(query.value(0).toString(),query.value(1).toString());
    }

    return InventoryGroups;
}



QList<InventoryItemGroupDataModel> SalesInventoryGroupsDatabaseHelper::getALLgroupObjects()
{
    QList<InventoryItemGroupDataModel> data;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "SELECT GROUP_ID,GROUP_NAME,PARENT_ID,Group_Type,Group_Name_Arabic from "+Sales_Inventory_Group_TableName
            + " where _id>1 order by group_name";
    query.exec(qStr);
    qDebug()<<qStr;
    while(query.next()){
        InventoryItemGroupDataModel obj;
        obj.GroupID = query.value(0).toString();
        obj.GroupName=query.value(1).toString();
        obj.ParentGroupName = getGroupNameByID(query.value(2).toString());
        obj.GroupType =query.value(3).toString();
        obj.GroupNameArabic = query.value(4).toString();
        data.append(obj);

    }
    return data;
}

QSqlQueryModel *SalesInventoryGroupsDatabaseHelper::getAllInventoryGroupsModel()
{

    QString qStr = " SELECT ";
    qStr += Sales_Inventory_Group_GroupId + " as 'Group Id', ";
    qStr += Sales_Inventory_Group_GroupName + " as 'Group Name', ";
    qStr += "(SELECT " + Sales_Inventory_Group_GroupName + " FROM " + Sales_Inventory_Group_TableName + " B ";
    qStr += " WHERE B." + Sales_Inventory_Group_GroupId + " = A." + Sales_Inventory_Group_ParentGroupID + ") as 'Parent Group', ";
    qStr += Sales_Inventory_Group_GroupType + " as 'Group Type' ";
    qStr += " FROM " + Sales_Inventory_Group_TableName + " A";


    return DatabaseHelper::getQueryModel(qStr);;

}

QList<QSharedPointer<InventoryItemGroupDataModel> > SalesInventoryGroupsDatabaseHelper::getALLgroupObjectsAsPtr()
{
    QList<QSharedPointer<InventoryItemGroupDataModel>> data;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "SELECT GROUP_ID,GROUP_NAME,PARENT_ID,Group_Type,Group_Name_Arabic from "+Sales_Inventory_Group_TableName;
    InventoryItemGroupDataModel* obj ;
    query.exec(qStr);

    while(query.next()){
        obj = new InventoryItemGroupDataModel;
        obj->GroupID = query.value(0).toString();
        obj->GroupName=query.value(1).toString();
        obj->ParentGroupName = getGroupNameByID(query.value(2).toString());
        obj->GroupType =query.value(3).toString();
        obj->GroupNameArabic = query.value(4).toString();
        data.push_back(QSharedPointer<InventoryItemGroupDataModel>(obj));
    }
    return data;
}

QString SalesInventoryGroupsDatabaseHelper::getGroupNameByID(QString id)
{
    QSqlDatabase db = QSqlDatabase::database(DatabaseValues::connectionString);
    QString qStr = "Select Group_Name from "+Sales_Inventory_Group_TableName+" where "+Sales_Inventory_Group_GroupId
            +" = '" + id +"'";
    qDebug()<<qStr;
    QSqlQuery query(db);
    QString name = "";

    query.prepare(qStr);
    //    query.bindValue(":id",id);
    query.exec();

    if(query.first()){
        name = query.value(0).toString();
    }
    qDebug()<<Q_FUNC_INFO<<__LINE__<<name;
    return name;
}

InventoryItemGroupDataModel SalesInventoryGroupsDatabaseHelper::getInvGroupByID(QString id)
{
    InventoryItemGroupDataModel data;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "SELECT GROUP_ID,GROUP_NAME,PARENT_ID,Group_Type,Group_Name_Arabic from "+Sales_Inventory_Group_TableName;
    qStr += " where "+Sales_Inventory_Group_GroupId + "='"+id+"'";

    qDebug()<<qStr;
    if(query.exec(qStr)){
        while(query.next()){

            data.GroupID = query.value(0).toString();
            data.GroupName=query.value(1).toString();
            data.ParentGroupName = getGroupNameByID(query.value(2).toString());
            data.GroupType =query.value(3).toString();
            data.GroupNameArabic = query.value(4).toString();

        }
    }
    else{
        qDebug()<<qStr;
    }


    qDebug()<< "got group "<<data.GroupName;
    return data;
}
QList<InventoryItemGroupDataModel> SalesInventoryGroupsDatabaseHelper::getSalesInventoryGroupDataAsMapForSalesAslist()
{
    QList<InventoryItemGroupDataModel> InventoryGroups;

    QString qStr = "SELECT "+Sales_Inventory_Group_GroupId+" , "+Sales_Inventory_Group_GroupName+
            " , "+Sales_Inventory_Group_id+ +", "+Sales_Inventory_Group_GroupName_Arabic +
            "  FROM "+Sales_Inventory_Group_TableName;
    qStr +=" where ";
    qStr += Sales_Inventory_Group_GroupType + "= 2";

    qStr +=" order by "+Sales_Inventory_Group_GroupName;
    // qDebug()<<qStr;
    QSqlDatabase db = QSqlDatabase::database(DatabaseValues::connectionString);
    QSqlQuery query(db);
    query.prepare(qStr);
    query.exec();
    while (query.next()) {
        InventoryItemGroupDataModel item;
        item.GroupName = query.value(1).toString();
        item.GroupID = query.value(0).toString();
        item._id = query.value(2).toInt();
        item.GroupNameArabic = query.value(3).toString().toUtf8();
        InventoryGroups.append(item);

    }

    return InventoryGroups;
}

InventoryItemGroupDataModel *SalesInventoryGroupsDatabaseHelper::getInvGroupPtrByID(QString id)
{

    InventoryItemGroupDataModel* data = new InventoryItemGroupDataModel();
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "SELECT GROUP_ID,GROUP_NAME,PARENT_ID,Group_Type,Group_Name_Arabic from "+Sales_Inventory_Group_TableName;
    qStr += " where "+Sales_Inventory_Group_GroupId + "='"+id+"'";

    qDebug()<<qStr;
    if(query.exec(qStr)){
        while(query.next()){

            data->GroupID = query.value(0).toString();
            data->GroupName=query.value(1).toString();
            data->ParentGroupId=query.value(2).toString();
            data->ParentGroupName = getGroupNameByID(data->ParentGroupId);
            data->GroupType =query.value(3).toString();
            data->GroupNameArabic = query.value(4).toString();

        }
    }
    else{
        qDebug()<<qStr;
    }

    qDebug()<< "got group "<<data->GroupName;
    return data;
}


bool SalesInventoryGroupsDatabaseHelper::checkItemExists(QString groupID)
{
    QString qStr = " SELECT 1 FROM " + Sales_Inventory_Group_TableName + " WHERE " + Sales_Inventory_Group_GroupId + " = '" + groupID + "' ";

    if(db->getSingletonQueryResult(qStr).toInt() == 1)
        return true;
    else
        return false;

}
