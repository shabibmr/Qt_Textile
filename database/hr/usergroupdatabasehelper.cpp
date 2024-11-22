#include "usergroupdatabasehelper.h"
#include "database/Settings/uisettingsdatabasehelper.h"

UserGroupDatabaseHelper::UserGroupDatabaseHelper(DatabaseHelper *db)
{
    this->db = db;
}

UserGroupDatabaseHelper::~UserGroupDatabaseHelper()
{

}

int UserGroupDatabaseHelper::startTransaction()
{
    db->startTransaction();

}

int UserGroupDatabaseHelper::insertUserGroup(UserGroupDataModel *obj)
{
    this->startTransaction();
    QMap<QString,QVariant> map;
    map.insert(User_Group_Permissions_Name, obj->Name);

    int id = db->insertRecordsPlus(map,User_Group_Permissions_TableName);

    for(int i=0; i<obj->permissions.size();i++){
        QMap<QString,QVariant> map2;
        map2.insert(User_Group_Ui_Permissions_UserGroupId, id);
        map2.insert(User_Group_Ui_Permissions_UiId, obj->permissions[i].ui_id);
        map2.insert(User_Group_Ui_Permissions_Create, obj->permissions[i].allowCreate);
        map2.insert(User_Group_Ui_Permissions_Read, obj->permissions[i].allowRead);
        map2.insert(User_Group_Ui_Permissions_Update, obj->permissions[i].allowUpdate);
        map2.insert(User_Group_Ui_Permissions_Delete, obj->permissions[i].allowDelete);
        map2.insert(User_Group_Ui_Permissions_Share, obj->permissions[i].allowShare);
        map2.insert(User_Group_Ui_Permissions_Extras, obj->permissions[i].extras);

        db->insertRecordsPlus(map2,User_Group_Ui_Permissions_TableName);

    }



    return commit();
}

int UserGroupDatabaseHelper::updateUserGroup(UserGroupDataModel *obj, int oldID)
{
    this->startTransaction();

    QMap<QString,QVariant> map;
    map.insert(User_Group_Permissions_Name, obj->Name);


    QMap<QString,QVariant> map3;
    map3.insert(User_Group_Permissions_id,oldID);

    db->updateRecordsPlus(map,map3,User_Group_Permissions_TableName);

    QMap<QString,QVariant> map4;
    map4.insert(User_Group_Ui_Permissions_UserGroupId,oldID);
    db->deleteRecordsPlus(map4, User_Group_Ui_Permissions_TableName);

    for(int i=0; i<obj->permissions.size();i++){
        QMap<QString,QVariant> map2;
        map2.insert(User_Group_Ui_Permissions_UserGroupId, oldID);
        map2.insert(User_Group_Ui_Permissions_UiId, obj->permissions[i].ui_id);
        map2.insert(User_Group_Ui_Permissions_Create, obj->permissions[i].allowCreate);
        map2.insert(User_Group_Ui_Permissions_Read, obj->permissions[i].allowRead);
        map2.insert(User_Group_Ui_Permissions_Update, obj->permissions[i].allowUpdate);
        map2.insert(User_Group_Ui_Permissions_Delete, obj->permissions[i].allowDelete);
        map2.insert(User_Group_Ui_Permissions_Share, obj->permissions[i].allowShare);
        map2.insert(User_Group_Ui_Permissions_Extras, obj->permissions[i].extras);

        db->insertRecordsPlus(map2,User_Group_Ui_Permissions_TableName);

    }

    return commit();
}

int UserGroupDatabaseHelper::deleteUserGroup(int oldID)
{
    this->startTransaction();
    QMap<QString,QVariant> map3;
    map3.insert(User_Group_Permissions_id,oldID);
    db->deleteRecordsPlus(map3,User_Group_Permissions_TableName);

    return commit();
}

int UserGroupDatabaseHelper::commit()
{
    if(flag){
        db->commitTransaction();
        return 1;
    }
    else{
        db->rollBackTransaction();
        return 0;
    }
}

int UserGroupDatabaseHelper::getMaxID()
{
    QString qStr = "SELECT MAX("+User_Group_Permissions_id+") FROM " + User_Group_Permissions_TableName;
    qDebug()<<qStr;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    int vNo = 1;
    query.prepare(qStr);
    if(query.exec()){
        while(query.next())
            vNo = query.value(0).toString().toInt();
    }
    else{
        qDebug()<<query.lastError();
    }
    return vNo;
}

QMap<int, QString> UserGroupDatabaseHelper::getUserGroups()
{
    QMap<int,QString> map;
    QString qStr = "SELECT ";
    qStr    += User_Group_Permissions_id + ", ";
    qStr    += User_Group_Permissions_Name +" ";
    qStr    += " from "+ User_Group_Permissions_TableName;
    qDebug()<<qStr;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    if(query.exec(qStr)){
        while(query.next()){
            map.insert(query.value(0).toInt(),query.value(1).toString());
        }
    }
    else{
        qDebug()<<query.lastError();
    }
    return map;
}

UserGroupDataModel UserGroupDatabaseHelper::getUserGroup(int id)
{

    UserGroupDataModel obj;

    QString qStr = " SELECT ";
    qStr += User_Group_Permissions_Name;

    qStr += " , "+ User_Group_Permissions_HR;
    qStr += " , "+ User_Group_Permissions_Purchases;
    qStr += " , "+ User_Group_Permissions_WareHouse;
    qStr += " , "+ User_Group_Permissions_HelpDesk;
    qStr += " , "+ User_Group_Permissions_Services;
    qStr += " , "+ User_Group_Permissions_Finances;
    qStr += " , "+ User_Group_Permissions_Inventory;
    qStr += " from "+ User_Group_Permissions_TableName + " WHERE "+ User_Group_Permissions_id
            + "='"+QString::number(id)+"'";

    //    qDebug()<<qStr;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.exec(qStr);

    while (query.next()) {
        int i=0;
        obj._id = id;
        obj.Name = query.value(i++).toString();

        obj.HR=query.value(i++).toBool();
        obj.Purchases=query.value(i++).toBool();
        obj.WareHouse=query.value(i++).toBool();
        obj.HelpDesk=query.value(i++).toBool();
        obj.Services=query.value(i++).toBool();
        obj.Finances=query.value(i++).toBool();
        obj.Inventory=query.value(i++).toBool();

    }


     qStr = " SELECT ";

    qStr += User_Group_Ui_Permissions_UiId + ", ";
    qStr += User_Group_Ui_Permissions_Create + ", ";
    qStr += User_Group_Ui_Permissions_Read + ", ";
    qStr += User_Group_Ui_Permissions_Update + ", ";
    qStr += User_Group_Ui_Permissions_Delete + ", ";
    qStr += User_Group_Ui_Permissions_Share + ", ";
    qStr += User_Group_Ui_Permissions_Extras + " ";
    qStr += " FROM " + User_Group_Ui_Permissions_TableName;
    qStr += " WHERE " + User_Group_Ui_Permissions_UserGroupId + " = " + QString::number(id);


    if(query.exec(qStr)){

        while(query.next()){
            int i=0;
            UiAccessControlDataModel perm;
            perm.ui_id = query.value(i++).toInt();
            perm.userGroupId = id;
            perm.allowCreate = query.value(i++).toBool();
            perm.allowRead = query.value(i++).toBool();
            perm.allowUpdate = query.value(i++).toBool();
            perm.allowDelete = query.value(i++).toBool();
            perm.allowShare = query.value(i++).toBool();
//            obj.extras = query.value(i++).toString();
            obj.permissions.append(perm);
        }
    }
    else{
        qDebug()<<query.lastError();
    }

    return obj;

}


UserGroupDataModel *UserGroupDatabaseHelper::getUserGroupById(int id)
{

    UserGroupDataModel *obj = new UserGroupDataModel;

    QString qStr = " SELECT ";
    qStr += User_Group_Permissions_Name;

    qStr += " , "+ User_Group_Permissions_HR;
    qStr += " , "+ User_Group_Permissions_Purchases;
    qStr += " , "+ User_Group_Permissions_WareHouse;
    qStr += " , "+ User_Group_Permissions_HelpDesk;
    qStr += " , "+ User_Group_Permissions_Services;
    qStr += " , "+ User_Group_Permissions_Finances;
    qStr += " , "+ User_Group_Permissions_Inventory;
    qStr += " from "+ User_Group_Permissions_TableName + " WHERE "+ User_Group_Permissions_id
            + "='"+QString::number(id)+"'";

    //    qDebug()<<qStr;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    if(query.exec(qStr)){

        while (query.next()) {
            int i=0;
            obj->_id = id;
            obj->Name = query.value(i++).toString();

            obj->HR=query.value(i++).toBool();
            obj->Purchases=query.value(i++).toBool();
            obj->WareHouse=query.value(i++).toBool();
            obj->HelpDesk=query.value(i++).toBool();
            obj->Services=query.value(i++).toBool();
            obj->Finances=query.value(i++).toBool();
            obj->Inventory=query.value(i++).toBool();

        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }


     qStr = " SELECT ";

    qStr += User_Group_Ui_Permissions_UiId + ", ";
    qStr += "(SELECT Ui_Name From algo_config.UI_CONFIG where _id = " + User_Group_Ui_Permissions_UiId + "), ";

    qStr += User_Group_Ui_Permissions_Create + ", ";
    qStr += User_Group_Ui_Permissions_Read + ", ";
    qStr += User_Group_Ui_Permissions_Update + ", ";
    qStr += User_Group_Ui_Permissions_Delete + ", ";
    qStr += User_Group_Ui_Permissions_Share + ", ";
    qStr += User_Group_Ui_Permissions_Extras + " ";
    qStr += " FROM " + User_Group_Ui_Permissions_TableName;
    qStr += " WHERE " + User_Group_Ui_Permissions_UserGroupId + " = " + QString::number(id);

    qDebug()<<Q_FUNC_INFO<<__LINE__<<qStr;


    if(query.exec(qStr)){

        while(query.next()){
            int i=0;
            UiAccessControlDataModel perm;
            perm.ui_id = query.value(i++).toInt();
            perm.UiName = query.value(i++).toString();
            perm.userGroupId = id;
            perm.allowCreate = query.value(i++).toBool();
            perm.allowRead = query.value(i++).toBool();
            perm.allowUpdate = query.value(i++).toBool();
            perm.allowDelete = query.value(i++).toBool();
            perm.allowShare = query.value(i++).toBool();
//            obj.extras = query.value(i++).toString();
            obj->permissions.append(perm);
        }
    }
    else{
        qDebug()<<query.lastError();
    }

    return obj;

}

QString UserGroupDatabaseHelper::getUserGroupIDByName(QString groupName)
{

}

QString UserGroupDatabaseHelper::getUserGroupNameById(int id)
{

}

UiAccessControlDataModel *UserGroupDatabaseHelper::getPermissionsByUserGroupAndUi(int usergroup, int ui_id)
{
    qDebug()<<Q_FUNC_INFO<<usergroup<<ui_id;

    UiAccessControlDataModel *obj = new UiAccessControlDataModel();

    QString qStr = " SELECT ";

    qStr += User_Group_Ui_Permissions_UiId + ", ";
    qStr += User_Group_Ui_Permissions_Create + ", ";
    qStr += User_Group_Ui_Permissions_Read + ", ";
    qStr += User_Group_Ui_Permissions_Update + ", ";
    qStr += User_Group_Ui_Permissions_Delete + ", ";
    qStr += User_Group_Ui_Permissions_Share + ", ";
    qStr += User_Group_Ui_Permissions_Extras + " ";
    qStr += " FROM " + User_Group_Ui_Permissions_TableName;
    qStr += " WHERE " + User_Group_Ui_Permissions_UserGroupId + " = " + QString::number(usergroup);
    qStr += " AND " + User_Group_Ui_Permissions_UiId + " = " + QString::number(ui_id);

    qDebug()<<qStr;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    if(query.exec(qStr)){

        while(query.next()){
            int i=0;
            obj->ui_id = query.value(i++).toInt();
            obj->userGroupId = usergroup;
            obj->allowCreate = query.value(i++).toBool();
            obj->allowRead = query.value(i++).toBool();
            obj->allowUpdate = query.value(i++).toBool();
            obj->allowDelete = query.value(i++).toBool();
            obj->allowShare = query.value(i++).toBool();
//            obj.extras = query.value(i++).toString();


        }
    }
    else{
        qDebug()<<query.lastError();
    }

    return obj;

}

UiAccessControlDataModel *UserGroupDatabaseHelper::getPermissionsByUserGroupAndVoucherType(int usergroup, QString voucherType)
{
    int ui_id = UiSettingsDatabaseHelper::getUiIdByVoucherType( voucherType);

    return getPermissionsByUserGroupAndUi(usergroup, ui_id );

}
