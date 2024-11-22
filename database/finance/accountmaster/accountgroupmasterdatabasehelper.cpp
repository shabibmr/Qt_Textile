#include "accountgroupmasterdatabasehelper.h"
#include "database/Settings/configsettingsvariables.h"

QString AccountGroupMasterDatabaseHelper::Accounts_Group_TableName = "account_groups";



AccountGroupMasterDatabaseHelper::AccountGroupMasterDatabaseHelper(DatabaseHelper *db)
{
    this->db = db;
}

AccountGroupMasterDatabaseHelper::~AccountGroupMasterDatabaseHelper()
{

}

QList<AccountGroupDataModel> AccountGroupMasterDatabaseHelper::getAllAccounts()
{
   // qDebug()<<"All Group Data";
    QList<AccountGroupDataModel> accountsList;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qstr = "Select "+Accounts_Group_GroupId+", "+Accounts_Group_GroupName+", "+
            Accounts_Group_ParentGroupID+ " from "+Accounts_Group_TableName +" order by "+Accounts_Group_GroupName;

    query.prepare(qstr);
    if(query.exec()){

        while(query.next()){
            AccountGroupDataModel obj;
            obj.groupID = query.value(0).toString();
            obj.groupName = query.value(1).toString();
            obj.parentGroupId = query.value(2).toString();
            obj.parentGroupName = getGroupNameByID(obj.parentGroupId);
            accountsList.append(obj);
        }
    }
    else{
        qDebug()<<query.lastQuery();
    }
    return accountsList;
}

QList<AccountGroupDataModel> AccountGroupMasterDatabaseHelper::getPrimaryAccounts()
{
   // qDebug()<<"All Group Data";
    QList<AccountGroupDataModel> accountsList;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qstr = "Select "+Accounts_Group_GroupId+", "+Accounts_Group_GroupName+", "+
            Accounts_Group_ParentGroupID+ " from "+Accounts_Group_TableName +
            " where " +  Accounts_Group_ParentGroupID + " = '0' order by "+Accounts_Group_GroupType;

    query.prepare(qstr);
    if(query.exec()){

        while(query.next()){
            AccountGroupDataModel obj;
            obj.groupID = query.value(0).toString();
            obj.groupName = query.value(1).toString();
            obj.parentGroupId = query.value(2).toString();
            obj.parentGroupName = getGroupNameByID(obj.parentGroupId);
            accountsList.append(obj);
        }
    }
    else{
        qDebug()<<query.lastQuery();
    }


    return accountsList;
}

QList<AccountGroupDataModel> AccountGroupMasterDatabaseHelper::getPrimaryAccountsByGroupType(QString groupType)
{
    // qDebug()<<"All Group Data";
     QList<AccountGroupDataModel> accountsList;
     QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
     QString qstr = "Select "+Accounts_Group_GroupId+", "+Accounts_Group_GroupName+", "+
             Accounts_Group_ParentGroupID+ " from "+Accounts_Group_TableName +
             " where " +  Accounts_Group_ParentGroupID + " = '0'  and " + Accounts_Group_GroupType + " = '" + groupType + "'" +
             " order by "+Accounts_Group_GroupType;

     qDebug()<<qstr;

     query.prepare(qstr);
     if(query.exec()){

         while(query.next()){
             AccountGroupDataModel obj;
             obj.groupID = query.value(0).toString();
             obj.groupName = query.value(1).toString();
             obj.parentGroupId = query.value(2).toString();
             obj.parentGroupName = getGroupNameByID(obj.parentGroupId);
             accountsList.append(obj);
         }
     }
     else{
         qDebug()<<query.lastQuery();
     }


     return accountsList;
}

QList<AccountGroupDataModel> AccountGroupMasterDatabaseHelper::getAccountsUnderParentGroup(QString groupId)
{
    QList<AccountGroupDataModel> accountsList;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qstr = "Select "+Accounts_Group_GroupId+", "+Accounts_Group_GroupName+", "+
            Accounts_Group_ParentGroupID+ " from "+Accounts_Group_TableName +
            " where " +  Accounts_Group_ParentGroupID + " = '" + groupId +"' ";

    query.prepare(qstr);
    if(query.exec()){

        while(query.next()){
            AccountGroupDataModel obj;
            obj.groupID = query.value(0).toString();
            obj.groupName = query.value(1).toString();
            obj.parentGroupId = query.value(2).toString();
            obj.parentGroupName = getGroupNameByID(obj.parentGroupId);
            accountsList.append(obj);
        }
    }
    else{
        qDebug()<<query.lastQuery();
    }


    return accountsList;
}

QList<AccountGroupDataModel> AccountGroupMasterDatabaseHelper::getAccountsUnderParentGroupRecursive(QString groupId)
{
    QList<AccountGroupDataModel> accountsList;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qstr = "Select "+Accounts_Group_GroupId+", "+Accounts_Group_GroupName+", "+
            Accounts_Group_ParentGroupID+ " from "+Accounts_Group_TableName +
            " where " +  Accounts_Group_GroupId + " in " ;
    qstr += " ((SELECT  GROUP_ID ";
    qstr += " FROM    (SELECT * FROM  " + AccountGroupMasterDatabaseHelper::Accounts_Group_TableName +"  ";
    qstr += "          ORDER BY PARENT_ID, GROUP_ID) GROUPS_SORTED, ";
    qstr += "         (SELECT @PV := '" + groupId + "') INITIALISATION ";
    qstr += " WHERE   FIND_IN_SET(PARENT_ID, @PV) ";
    qstr += " AND     LENGTH(@PV := CONCAT(@PV, ',', GROUP_ID))) ";
    qstr += " UNION  ";
    qstr += " (SELECT  GROUP_ID ";
    qstr += " WHERE GROUP_ID = '" + groupId + "'))  ";


    query.prepare(qstr);
    if(query.exec()){

        while(query.next()){
            AccountGroupDataModel obj;
            obj.groupID = query.value(0).toString();
            obj.groupName = query.value(1).toString();
            obj.parentGroupId = query.value(2).toString();
            obj.parentGroupName = getGroupNameByID(obj.parentGroupId);
            accountsList.append(obj);
        }
    }
    else{
        qDebug()<<query.lastQuery();
        qDebug()<<query.lastError();
    }


    return accountsList;
}

QStringList AccountGroupMasterDatabaseHelper::getAccountsUnderParentGroupRecurStrList(QString groupId)
{
    QStringList accountsList;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qstr = "Select "+Accounts_Group_GroupId+", "+Accounts_Group_GroupName+", "+
            Accounts_Group_ParentGroupID+ " from "+Accounts_Group_TableName +
            " where " +  Accounts_Group_GroupId + " in " ;
    qstr += " (SELECT  GROUP_ID ";
    qstr += " FROM    (SELECT * FROM  " + AccountGroupMasterDatabaseHelper::Accounts_Group_TableName +"  ";
    qstr += "          ORDER BY PARENT_ID, GROUP_ID) GROUPS_SORTED, ";
    qstr += "         (SELECT @PV := '" + groupId + "') INITIALISATION ";
    qstr += " WHERE   FIND_IN_SET(PARENT_ID, @PV) ";
    qstr += " AND     LENGTH(@PV := CONCAT(@PV, ',', GROUP_ID)) ";
    qstr += " UNION  ";
    qstr += " SELECT  GROUP_ID ";
    qstr += " WHERE GROUP_ID = '" + groupId + "') AS ACC_GROUP ";
    query.prepare(qstr);
    if(query.exec()){

        while(query.next()){
           accountsList.append(query.value(0).toString());
        }
    }
    else{
        qDebug()<<query.lastQuery();
    }


    return accountsList;
}

AccountGroupDataModel AccountGroupMasterDatabaseHelper::getGroupById(QString id)
{
    AccountGroupDataModel obj;

    QString qstr = "Select "+Accounts_Group_GroupId+", "+Accounts_Group_GroupName+", "+ Accounts_Group_Category + ", "+
            Accounts_Group_ParentGroupID+ " from "+Accounts_Group_TableName
            + " where "+Accounts_Group_GroupId + " = '"+ id + "'"
            +" order by "+Accounts_Group_GroupName;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qstr);
    qDebug()<<qstr;
//    query.bindValue(":id",id);
    query.exec();
    int i=0;
    if(query.next()){
        obj.groupID = query.value(i++).toString();
        obj.groupName = query.value(i++).toString();
        obj.category =  query.value(i++).toString();
        obj.parentGroupId = query.value(i++).toString();
        obj.parentGroupName = getGroupNameByID(obj.parentGroupId);
    }

    return obj;

}

AccountGroupDataModel *AccountGroupMasterDatabaseHelper::getGroupPtrById(QString id)
{
    AccountGroupDataModel* obj = new AccountGroupDataModel();

    QString qstr = "Select "+Accounts_Group_GroupId+", "+Accounts_Group_GroupName+", "+ Accounts_Group_Category + ", "+
            Accounts_Group_ParentGroupID+ ", " + Accounts_Group_GroupType +  " from "+Accounts_Group_TableName
            + " where "+Accounts_Group_GroupId + " = '"+ id + "'"
            +" order by "+Accounts_Group_GroupName;
    qDebug()<<qstr;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qstr);
    qDebug()<<qstr;
//    query.bindValue(":id",id);
    query.exec();
    int i=0;
    if(query.next()){
        obj->groupID = query.value(i++).toString();
        obj->groupName = query.value(i++).toString();
        obj->category =  query.value(i++).toString();
        obj->parentGroupId = query.value(i++).toString();
        obj->parentGroupName = getGroupNameByID(obj->parentGroupId);
        obj->groupType = query.value(i++).toString();
        qDebug()<<obj->groupType;
    }

    return obj;
}

QString AccountGroupMasterDatabaseHelper::getGroupNameByID(QString id)
{
    QSqlDatabase db = QSqlDatabase::database(DatabaseValues::connectionString);
    QString qStr = "Select Group_Name from "+Accounts_Group_TableName+" where "+Accounts_Group_GroupId+" like :id";
    //qDebug()<<qStr <<id;
    QSqlQuery query(db);
    QString name = "";

    query.prepare(qStr);
    query.bindValue(":id",id);
    query.exec();

    if(query.next()){
        name = query.value(0).toString();
    }

    return name;
}

QString AccountGroupMasterDatabaseHelper::getGroupIDByName(QString Name)
{
    QString id ="";
    QString qStr = "SELECT "+ Accounts_Group_GroupId+ " FROM "+Accounts_Group_TableName +" WHERE "+Accounts_Group_GroupName
            +" = :name";
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);
    query.bindValue(":name",Name);
    if(!query.exec()){
        qDebug()<<query.lastError();
    }
    else{
        while(query.next()){
            id = query.value(0).toString();
        }
    }

    return id;

}



int AccountGroupMasterDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

int AccountGroupMasterDatabaseHelper::insertAccountGroup(AccountGroupDataModel *obj)
{
    this->startTransaction();
    QMap<QString,QVariant> map;
    if(obj->groupID ==""){
        qDebug()<<obj->parentGroupId<<LoginValues::voucherPrefix<<obj->groupName;
        obj->groupID = obj->parentGroupId +LoginValues::voucherPrefix + getMaxID();
    }
    map.insert(Accounts_Group_GroupId,obj->groupID);
    map.insert(Accounts_Group_GroupName,obj->groupName);
    map.insert(Accounts_Group_ParentGroupID,obj->parentGroupId);
    map.insert(Accounts_Group_GroupType,obj->groupType);
    map.insert(Accounts_Group_Category,obj->category);
    map.insert(Accounts_Group_Last_Modified, QDateTime::currentDateTime());


//    db->startTransaction();
    db->insertRecordsPlus(map,Accounts_Group_TableName);

    return commit();
}


int AccountGroupMasterDatabaseHelper::updateAccountGroup(AccountGroupDataModel *obj, QString oldID)
{
    this->startTransaction();
    QMap<QString,QVariant> map;
    QMap<QString,QVariant> map1;

    LedgerMasterDatabaseHelper* ledHelper = new LedgerMasterDatabaseHelper();
    map.insert(Accounts_Group_GroupId,obj->groupID);
    map.insert(Accounts_Group_GroupName,obj->groupName);
    map.insert(Accounts_Group_ParentGroupID,obj->parentGroupId);
    qDebug()<<obj->groupType;
    map.insert(Accounts_Group_GroupType,obj->groupType);
    map.insert(Accounts_Group_Category,obj->category);
    map.insert(Accounts_Group_Last_Modified, QDateTime::currentDateTime());

    map1.insert(Accounts_Group_GroupId,oldID);
//    db->startTransaction();
    db->updateRecordsPlus(map,map1,Accounts_Group_TableName);

    ledHelper->updateLedgerTypeByGroup(oldID, obj->groupType);

    return commit();
}

int AccountGroupMasterDatabaseHelper::deleteAccountGroup(QString oldID)
{
    this->startTransaction();
    QMap<QString,QVariant> map1;
    map1.insert(Accounts_Group_id,oldID);

//    db->startTransaction();
    db->deleteRecordsPlus(map1,Accounts_Group_TableName);
    return commit();
}


QMap<QString, QString> AccountGroupMasterDatabaseHelper::getAccountParentGroupAsMap(QString groupID)
{
//    QString qStr = " SELECT T1.groupID, T1.group_name FROM (";
//   qStr += "  SELECT  @r AS groupID, " + Sales_Inventory_Group_GroupName + ", ";
//   qStr += "  (SELECT @r := " + Sales_Inventory_Group_ParentGroupID + " FROM ";
//   qStr += Sales_Inventory_Group_TableName + " WHERE " + Sales_Inventory_Group_GroupId + " = groupID) AS parent_id, ";
//   qStr += " @l := @l + 1 AS lvl FROM ";
//   qStr += " (SELECT @r := '" + groupID + "' , @l := 0) vars, " + Sales_Inventory_Group_TableName + " m ";
//   qStr += "     WHERE @r>=0 ) T1 ";
//   qStr += " WHERE T1.lvl >1 ";
//   qStr += " ORDER BY T1.lvl DESC";

   QString qStr = " Select " + Accounts_Group_GroupId + "," + Accounts_Group_GroupName ;
   qStr += " from " +  Accounts_Group_TableName + " where " + Accounts_Group_GroupId + " not in ";
   qStr += " (SELECT " +  Accounts_Group_GroupId + " FROM ";
   qStr += " (SELECT * FROM " + Accounts_Group_TableName + " ORDER BY " + Accounts_Group_ParentGroupID ;
   qStr += " , " +  Accounts_Group_GroupId + ") GROUPS_SORTED, ";
   qStr += " (SELECT @PV := '" + groupID + "') INITIALISATION ";
   qStr += " WHERE   FIND_IN_SET(PARENT_ID, @PV) ";
   qStr += " AND     LENGTH(@PV := CONCAT(@PV, ',', GROUP_ID)) ";
   qStr += " union";
   qStr += " select " + Accounts_Group_GroupId + " from " + Accounts_Group_TableName ;
   qStr += " where " + Accounts_Group_GroupId + " = '" + groupID + "' )";

    QMap<QString, QString> AccountGroups;

//    QString qStr = "SELECT "+Sales_Inventory_Group_GroupId+" , "+Sales_Inventory_Group_GroupName+
//            "  FROM "+Sales_Inventory_Group_TableName+" order by "+Sales_Inventory_Group_GroupName;
    // qDebug()<<qStr;
    QSqlDatabase db = QSqlDatabase::database(DatabaseValues::connectionString);
    QSqlQuery query(db);
    query.prepare(qStr);
    query.exec();
    while (query.next()) {
        AccountGroups.insert(query.value(0).toString(),query.value(1).toString());
    }

    return AccountGroups;

}

QMap<QString,QString> AccountGroupMasterDatabaseHelper::getAllGroupsAsMap()
{
    QMap<QString,QString> GroupNames;

    QString qStr = "Select Group_ID,group_Name from  " + AccountGroupMasterDatabaseHelper::Accounts_Group_TableName +"   order by Group_name ";
    qDebug()<<qStr;

   // qDebug()<<"Opening Database";
    QSqlDatabase db = QSqlDatabase::database(DatabaseValues::connectionString);

    qDebug()<<"Database opened";
    QSqlQuery query(db);

    query.prepare(qStr);
    query.exec();
    while(query.next()){
        GroupNames.insert(query.value(0).toString(),query.value(1).toString());
    }
    qDebug()<<GroupNames.size()<<"map size";
    return GroupNames;
}

QString AccountGroupMasterDatabaseHelper::getMaxID()
{
    QSqlDatabase db = QSqlDatabase::database(DatabaseValues::connectionString);
    QSqlQuery query(db);
    int id =0;
    QString qStr = "select max(_id) from "+Accounts_Group_TableName;
    query.exec(qStr);
    while(query.next()){
        id = query.value(0).toInt();
    }
    return QString::number(id+1);
}

QString AccountGroupMasterDatabaseHelper::getGroupType(QString groupID)
{
    QSqlDatabase db = QSqlDatabase::database(DatabaseValues::connectionString);
    QSqlQuery query(db);
    QString id ;
    QString qStr = "select "+Accounts_Group_GroupType+" from "+Accounts_Group_TableName;
    qStr += " where "+Accounts_Group_GroupId + "='"+ groupID +"'";
    query.exec(qStr);
    while(query.next()){
        id = query.value(0).toString();
    }
    return id;
}

bool AccountGroupMasterDatabaseHelper::isDefaultRecord(QString groupID)
{
    QSqlDatabase db = QSqlDatabase::database(DatabaseValues::connectionString);
    QSqlQuery query(db);
    bool id = false;
    QString qStr = "select "+Accounts_Group_Default+" from "+Accounts_Group_TableName;
    qStr += " where "+Accounts_Group_GroupId + "='"+ groupID +"'";
    query.exec(qStr);
    while(query.next()){
        id = query.value(0).toBool();
    }
    return id;
}

int AccountGroupMasterDatabaseHelper::commit()
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

QSqlQueryModel *AccountGroupMasterDatabaseHelper::getAllAccountForSearchModel()
{
        QSqlQueryModel *query = new QSqlQueryModel();
        QString qStr = " SELECT ";
        qStr += Accounts_Group_GroupId + ", ";
        qStr += Accounts_Group_GroupName + ", ";
        qStr += Accounts_Group_ParentGroupID + ", ";
        qStr += Accounts_Group_Category + ", ";
        qStr += Accounts_Group_GroupType;
        qStr += " FROM " + Accounts_Group_TableName;

        query->setQuery(qStr,QSqlDatabase::database(DatabaseValues::connectionString));

        return DatabaseHelper::getQueryModel(qStr);

}

QSqlQueryModel *AccountGroupMasterDatabaseHelper::getAllAccountsModel()
{
    QSqlQueryModel *query = new QSqlQueryModel();

    QString qStr = " SELECT ";
    qStr += Accounts_Group_GroupId + " as 'Group Id', ";
    qStr += Accounts_Group_GroupName + " as 'Group Name', ";
    qStr += "(SELECT " + Accounts_Group_GroupName + " FROM " + Accounts_Group_TableName + " B ";
    qStr += " WHERE B." + Accounts_Group_GroupId + " = A." + Accounts_Group_ParentGroupID + ") as 'Parent Group' ";
    qStr += " FROM " + Accounts_Group_TableName + " A";
    qStr += " ORDER BY " + Accounts_Group_id;



    return DatabaseHelper::getQueryModel(qStr);

}
