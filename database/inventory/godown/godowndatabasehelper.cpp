#include "godowndatabasehelper.h"
#include "database/Settings/ConfigurationSettingsDatabaseHelper.h"

QString GodownDatabaseHelper::Godown_Tablename =  "GODOWN";
QString GodownDatabaseHelper::Godown_id             ="_id";
QString GodownDatabaseHelper::Godown_GodownID       ="Godown_ID";
QString GodownDatabaseHelper::GodownName            ="Godown_Name";
QString GodownDatabaseHelper::GodownLocation        ="Godown_Location";
QString GodownDatabaseHelper::GodownNarration       ="Godown_Narration";
QString GodownDatabaseHelper::Godown_IsProfitCentre = "isProfitCentre";
QString GodownDatabaseHelper::Godown_IsDefault      = "isDefault";
QString GodownDatabaseHelper::Godown_IsProduction   = "isProduction";
QString GodownDatabaseHelper::Godown_Last_Modified   = "Last_Modified";
GodownDatabaseHelper::GodownDatabaseHelper(DatabaseHelper *db)
{
    this->db = db;

}

GodownDatabaseHelper::~GodownDatabaseHelper()
{
    //    delete db;
}



int GodownDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

int GodownDatabaseHelper::commit()
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

int GodownDatabaseHelper::insertGodown(GodownDataObject *obj)
{
    startTransaction();

    QMap<QString,QVariant> mapxx;
    if(obj->isDefault == true){
        QMap<QString,QVariant> mapx;
        mapx.insert(Godown_IsDefault,false);
        db->updateRecordsPlus(mapx,mapxx,Godown_Tablename);
    }

    obj->GodownID = LoginValues::voucherPrefix + getMaxID();
    QMap<QString,QVariant> map;
    map.insert(Godown_GodownID, obj->GodownID);
    map.insert(GodownName,obj->GodownName);
    map.insert(GodownLocation,obj->GodownLocation);
    map.insert(GodownNarration,obj->GodownNarration);
    map.insert(Godown_IsProfitCentre, obj->isProfitCentre);
    map.insert(Godown_IsDefault, obj->isDefault);
    map.insert(Godown_IsProduction,obj->isProductionCentre);
    map.insert(Godown_Last_Modified, QDateTime::currentDateTime());



    qint64 id = db->insertRecordsPlus(map,Godown_Tablename);

    return commit();
}

int GodownDatabaseHelper::updateGodown(GodownDataObject *obj, QString id)
{
    startTransaction();
    QMap<QString,QVariant> mapxx;
    if(obj->isDefault == true){
        QMap<QString,QVariant> mapx;
        mapx.insert(Godown_IsDefault,false);
        db->updateRecordsPlus(mapx,mapxx,Godown_Tablename);
    }
    QMap<QString,QVariant> map;
    map.insert(GodownName,obj->GodownName);
    map.insert(GodownLocation,obj->GodownLocation);
    map.insert(GodownNarration,obj->GodownNarration);
    map.insert(Godown_IsProfitCentre, obj->isProfitCentre);
    map.insert(Godown_IsDefault, obj->isDefault);
    map.insert(Godown_IsProduction,obj->isProductionCentre);
    map.insert(Godown_Last_Modified, QDateTime::currentDateTime());
    QMap<QString,QVariant> map1;
    map1.insert(Godown_GodownID,id);
    db->updateRecordsPlus(map,map1,Godown_Tablename);
    return commit();
}

int GodownDatabaseHelper::deleteGodown(QString id)
{
    startTransaction();
    QMap<QString,QVariant> map1;
    map1.insert(Godown_GodownID,id);
    db->deleteRecordsPlus(map1,Godown_Tablename);
    return commit();
}

QString GodownDatabaseHelper::getMaxID()
{
    QSqlDatabase db = QSqlDatabase::database(DatabaseValues::connectionString);
    QSqlQuery query(db);
    int id =0;
    QString qStr = "select max(_id) from "+Godown_Tablename;
    query.exec(qStr);
    while(query.next()){
        id = query.value(0).toInt();
    }
    return QString::number(id+1);
}

QList<GodownDataObject*> GodownDatabaseHelper::getAllGodowns()
{
    QList<GodownDataObject*> data;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    QString qStr = "Select "
            + Godown_GodownID+", "
            + GodownName+", "
            + GodownLocation+", "
            + GodownNarration+ ", "
            + Godown_IsProfitCentre +", "
            + Godown_IsDefault+", "
            + Godown_IsProduction
            + " from "
            + Godown_Tablename;
    query.exec(qStr);
    while(query.next()){
        GodownDataObject* obj = new GodownDataObject();
        obj->GodownID = query.value(0).toString();
        obj->GodownName = query.value(1).toString();
        obj->GodownLocation =query.value(2).toString();
        obj->GodownNarration = query.value(3).toString();
        obj->isProfitCentre = query.value(4).toBool();
        obj->isDefault = query.value(5).toBool();
        obj->isProductionCentre = query.value(6).toBool();
        data.append(obj);
    }
    qDebug()<<qStr;
    return data;
}

QMap<QString, QString> GodownDatabaseHelper::getGodownForSearchAsMap()
{
    QMap<QString, QString> map;

    QList<GodownDataObject*> data;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    QString qStr = "Select "+Godown_GodownID+", "+GodownName
            + " from "
            + Godown_Tablename + " order by "+Godown_IsDefault + " desc ";
    query.exec(qStr);
    while(query.next()){
        map.insert(query.value(0).toString(),  query.value(1).toString());
    }
    return map;
}

QString GodownDatabaseHelper::getGodownNameById(QString godownId)
{
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString name;

    QString qStr = "Select "+GodownName;
    qStr += " from " + Godown_Tablename;
    qStr += " Where " + Godown_GodownID + " = '" + godownId + "'";

    return DatabaseHelper::getSingletonQueryResult(qStr).toString();

}

QString GodownDatabaseHelper::getGodownIdByName(QString godownName)
{
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString name;

    QString qStr = "Select "+Godown_GodownID;
    qStr += " from " + Godown_Tablename;
    qStr += " Where " + GodownName + " = '" + godownName + "'";
    query.exec(qStr);
    while(query.next()){
        name = query.value(0).toString();
    }
    return name;

}

QString GodownDatabaseHelper::getDefaultGodown()
{
    qDebug()<<Q_FUNC_INFO;
    QString qStr = " SELECT " + Godown_GodownID + " FROM " + Godown_Tablename;
    qStr += " WHERE " + Godown_IsDefault + " = 1 ";
    QVariant godown = DatabaseHelper::getSingletonQueryResult(qStr);
    qDebug()<<"default godown is "<<godown;
    if(godown == NULL){
        qStr = " SELECT " + Godown_GodownID + " FROM " + Godown_Tablename;
        qStr += " LIMIT 1 ";

        godown = DatabaseHelper::getSingletonQueryResult(qStr);

        qDebug()<<"first godown entry is "<<godown;
    }

    return godown.toString();

}


GodownDataObject *GodownDatabaseHelper::getGodownById(QString godownId)
{
    GodownDataObject* obj = new GodownDataObject();

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    QString qStr = "Select "+Godown_GodownID+", "+GodownName+", "+GodownLocation+", "+GodownNarration+ ", "+ Godown_IsProfitCentre
            +", "
            + Godown_IsDefault+", "+ Godown_IsProduction +" ";
    qStr += " from " + Godown_Tablename;
    qStr += " Where " + Godown_GodownID + " = '" + godownId + "'";
    query.exec(qStr);
    while(query.next()){
        obj->GodownID = query.value(0).toString();
        obj->GodownName = query.value(1).toString();
        obj->GodownLocation =query.value(2).toString();
        obj->GodownNarration = query.value(3).toString();
        obj->isProfitCentre = query.value(4).toBool();
        obj->isDefault = query.value(5).toBool();
        obj->isProductionCentre = query.value(6).toBool();
    }
    return obj;
}


GodownDataObject GodownDatabaseHelper::getGodownObjById(QString godownId)
{
    GodownDataObject obj ;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    QString qStr = "Select "+Godown_GodownID+", "+GodownName+", "+GodownLocation+", "+GodownNarration+ ", "+ Godown_IsProfitCentre
            +", "
            + Godown_IsDefault+", "+Godown_IsProduction;
    qStr += " from " + Godown_Tablename;
    qStr += " Where " + Godown_GodownID + " = '" + godownId + "'";
    query.exec(qStr);
    while(query.next()){
        obj.GodownID = query.value(0).toString();
        obj.GodownName = query.value(1).toString();
        obj.GodownLocation =query.value(2).toString();
        obj.GodownNarration = query.value(3).toString();
        obj.isProfitCentre = query.value(4).toBool();
        obj.isDefault = query.value(5).toBool();
        obj.isProductionCentre = query.value(6).toBool();
    }
    return obj;
}

QList<GodownDataObject *> GodownDatabaseHelper::getAllProfitCentres()
{
    QList<GodownDataObject*> data;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    QString qStr = "Select "+Godown_GodownID+", "+GodownName+", "+GodownLocation+", "+GodownNarration+ ", "+ Godown_IsProfitCentre +", "
            + Godown_IsDefault+" ";
    qStr += " from " + Godown_Tablename;
    qStr += " Where " + Godown_IsProfitCentre + " = 1 ";
    query.exec(qStr);
    while(query.next()){
        GodownDataObject* obj = new GodownDataObject();
        obj->GodownID = query.value(0).toString();
        obj->GodownName = query.value(1).toString();
        obj->GodownLocation =query.value(2).toString();
        obj->GodownNarration = query.value(3).toString();
        obj->isProfitCentre = query.value(4).toBool();
        obj->isDefault = query.value(5).toBool();
        data.append(obj);
    }
    return data;
}


QSqlQueryModel *GodownDatabaseHelper::getGodownForSearchModel()
{
    QString qStr = "Select "+Godown_GodownID+", "+GodownName+", "+GodownLocation+", "+GodownNarration+ " from "
            + Godown_Tablename + " order by "+Godown_IsDefault +" desc ";

    return DatabaseHelper::getQueryModel(qStr);;

}
