#include "propertydatabasehelper.h"

PropertyDatabaseHelper::PropertyDatabaseHelper()
{
    db = new DatabaseHelper();
}

PropertyDatabaseHelper::~PropertyDatabaseHelper()
{
//    delete db;
}

int PropertyDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

int PropertyDatabaseHelper::commit()
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

int PropertyDatabaseHelper::insertProperty(PropertyDataModel *obj)
{
    startTransaction();

    QMap<QString,QVariant> map;
    //map.insert(Proprties_PropertyID,obj->PropertyID);
    map.insert(Proprties_PropertyName,obj->PropertyName);
//    map.insert(Proprties_PropertyUnit,obj->PropertyUnits);
    db->insertRecordsPlus(map,Proprties_TableName);

    return commit();
}

int PropertyDatabaseHelper::updateProperty(PropertyDataModel *obj)
{
    startTransaction();

    QMap<QString,QVariant> map;
    map.insert(Proprties_PropertyID,obj->PropertyID);
    map.insert(Proprties_PropertyName,obj->PropertyName);
//    map.insert(Proprties_PropertyUnit,obj->PropertyUnits);

    QMap<QString,QVariant> map1;

    map1.insert(Proprties_PropertyID,obj->PropertyID);
    db->updateRecordsPlus(map,map1,Proprties_TableName);

    return commit();
}

int PropertyDatabaseHelper::deleteProperty(QString id)
{
    startTransaction();

    QMap<QString,QVariant> map1;

    map1.insert(Proprties_PropertyID,id);
    db->deleteRecordsPlus(map1,Proprties_TableName);
    return commit();
}

QString PropertyDatabaseHelper::getMaxID()
{
    QSqlDatabase db = QSqlDatabase::database(DatabaseValues::connectionString);
    QSqlQuery query(db);
    int id =0;
    QString qStr = "select max(_id) from "+Proprties_TableName;
    query.exec(qStr);
    while(query.next()){
        id = query.value(0).toInt();
    }
    return QString::number(id+1);
}

QList<PropertyDataModel> PropertyDatabaseHelper::getAllProperties()
{
    QList<PropertyDataModel> data;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "SELECT "+ Proprties_PropertyID+", "+ Proprties_PropertyName +", "+
            " from "+Proprties_TableName;

    query.exec(qStr);
    while(query.next()){
        PropertyDataModel obj;
        obj.PropertyID = query.value(0).toInt();
        obj.PropertyName = query.value(1).toString();

        data.append(obj);
    }
    return data;

}

QString PropertyDatabaseHelper::getPropertyNameByID(QString id)
{
    QSqlDatabase db = QSqlDatabase::database(DatabaseValues::connectionString);
    QString qStr = "Select "+Proprties_PropertyName+" from "+Proprties_TableName+" where "+Proprties_PropertyID
            +" = :id";
   // qDebug()<<qStr <<id;
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

QString PropertyDatabaseHelper::getPropertyIDByName(QString name)
{

    QSqlDatabase db = QSqlDatabase::database(DatabaseValues::connectionString);
    QString qStr = "Select "+Proprties_PropertyID+" from "+Proprties_TableName+" where "+Proprties_PropertyName
            +" = :id";
   // qDebug()<<qStr <<id;
    QSqlQuery query(db);
    QString id = "";

    query.prepare(qStr);
    query.bindValue(":id",name);
    query.exec();

    if(query.next()){
        id = query.value(0).toString();
    }
    return id;
}

QList<PropertyDataModel> PropertyDatabaseHelper::getPropertiesOfItem(QString id)
{
    QList<PropertyDataModel> propList;
    QString qStr = "";
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);
    query.bindValue(":id",id);
    if(query.exec()){
        PropertyDataModel data;

        data.PropertyName =getPropertyNameByID(query.value(0).toString());
        data.ItemProperty_DefaultValues = query.value(1).toString();
    }
    else{
        qDebug()<<query.lastError();
    }

    return propList;
}

QSqlQueryModel *PropertyDatabaseHelper::getPropertyMasterList()
{
    QString qStr = "SELECT "+ Proprties_PropertyID+", "+ Proprties_PropertyName +" "+
            " from "+Proprties_TableName;

    return DatabaseHelper::getQueryModel(qStr);
}

PropertyDataModel *PropertyDatabaseHelper::getPropertyById(int id)
{
    QString qStr = "SELECT "+ Proprties_PropertyID+", "+ Proprties_PropertyName +", "+
            " from "+Proprties_TableName;
    qStr += " WHERE "+Proprties_PropertyID +"=:ID";

   QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
   QSharedPointer<PropertyDataModel> prop (new PropertyDataModel());
   query.prepare(qStr);
   query.bindValue(":ID",id);

   if(query.exec()){
       while(query.next()){
            prop->PropertyID = query.value(0).toInt();
            prop->PropertyName = query.value(1).toString();
       }
   }

   return  prop.data();
}
