#include "uomdatabasehelper.h"


QString UomDataBaseHelper::UOM_TableName = "UOMLIST";
QString UomDataBaseHelper::UOM_UOMID  = "UOM_ID";
QString UomDataBaseHelper::UOM_UOMName = "UOM_Name";
QString UomDataBaseHelper::UOM_UOMSymbol = "UOM_Symbol";
QString UomDataBaseHelper::UOM_DecimalPoints = "UOM_Decimal_Points";
QString UomDataBaseHelper::UOM_Narration = "UOM_Narration";
QString UomDataBaseHelper::UOM_Last_Modified = "Last_Modified";

QString UomDataBaseHelper::UOM_Conversion_TableName = "UOM_Conversion";
QString UomDataBaseHelper::UOM_Conversion_id = "_id";
QString UomDataBaseHelper::UOM_Conversion_BaseUnit = "BaseUnit";
QString UomDataBaseHelper::UOM_Conversion_ToUnit = "ToUnit";
QString UomDataBaseHelper::UOM_Conversion_Value = "ConValue";
QString UomDataBaseHelper::UOM_Conversion_Narration  = "Narration";
QString UomDataBaseHelper::UOM_Conversion_ItemId = "itemID";
QString UomDataBaseHelper::UOM_Conversion_BarCode = "barCode";

QString UomDataBaseHelper::qStrUomForItemDetail =  " (SELECT distinct "
        + UOM_Conversion_ItemId + ", "
        + UOM_Conversion_ToUnit + " as UOM, "
        + "(SELECT "+ UOM_UOMName + " FROM "+UOM_TableName + " basetable where "
        + "basetable."+UOM_UOMID +" = contable." + UOM_Conversion_ToUnit + ") as UomName, "
        + "(SELECT "+ UOM_UOMSymbol + " FROM "+UOM_TableName + " basetable where "
        + "basetable."+UOM_UOMID +" = contable." + UOM_Conversion_ToUnit + ") as UomSymbol, "
        + "round("+UOM_Conversion_Value + ",3) as conValue,  "
        + "coalesce(" + UOM_Conversion_BarCode +", '') as barCode "
        + " FROM " + UOM_Conversion_TableName + " contable "
        + " UNION "
        + " SELECT B." +  UOM_Conversion_ItemId + ", "
        + " case when cast(A." + UOM_Conversion_BaseUnit + " as int) = cast(B." + UOM_Conversion_ToUnit
        + " as int) then A." + UOM_Conversion_ToUnit
        + " when cast(A." + UOM_Conversion_ToUnit + " as int) = cast(B." + UOM_Conversion_ToUnit
        + " as int) then A." + UOM_Conversion_BaseUnit
        + " else '' END as UOM, "
        + " case when A." + UOM_Conversion_BaseUnit + " = B." + UOM_Conversion_ToUnit
        + " then (SELECT "+UOM_UOMName + " FROM "+UOM_TableName + " basetable where "
        + "basetable."+UOM_UOMID +" = A." + UOM_Conversion_ToUnit + ") "
        + " when A." + UOM_Conversion_ToUnit + " = B." + UOM_Conversion_ToUnit
        + " then (SELECT "+UOM_UOMName + " FROM "+UOM_TableName + " basetable where "
        + "basetable."+UOM_UOMID +" = A." + UOM_Conversion_BaseUnit + ") "
        + " else '' END as UomName, "
        + " case when A." + UOM_Conversion_BaseUnit + " = B." + UOM_Conversion_ToUnit
        + " then (SELECT "+UOM_UOMSymbol + " FROM "+UOM_TableName + " basetable where "
        + "basetable."+UOM_UOMID +" = A." + UOM_Conversion_ToUnit + ") "
        + " when A." + UOM_Conversion_ToUnit + " = B." + UOM_Conversion_ToUnit
        + " then (SELECT "+UOM_UOMSymbol + " FROM "+UOM_TableName + " basetable where "
        + "basetable."+UOM_UOMID +" = A." + UOM_Conversion_BaseUnit + ") "
        + " else '' END as UomSymbol, "
        + " case when cast(A." + UOM_Conversion_BaseUnit + " as int) = cast(B." + UOM_Conversion_ToUnit
        + " as int) then cast( B." + UOM_Conversion_Value + " * A." + UOM_Conversion_Value + " as decimal(10,3))"
        + " when cast(A." + UOM_Conversion_ToUnit + " as int) = cast(B." + UOM_Conversion_ToUnit
        + " as int) then cast(B." + UOM_Conversion_Value + " / A." + UOM_Conversion_Value + " as decimal(10,3))"
        + " else 1 END,  "
        + " '' as barCode "
        + " FROM " + UOM_Conversion_TableName + " A "
        + " INNER JOIN " +  UOM_Conversion_TableName + " B "
        + " ON ( cast(A." + UOM_Conversion_BaseUnit + " as int) = cast(B." + UOM_Conversion_ToUnit
        + " as int) OR cast(A." + UOM_Conversion_ToUnit + " as int) = cast(B." + UOM_Conversion_ToUnit + " as int))"
        + " AND A." + UOM_Conversion_ItemId + " = '' )";


UomDataBaseHelper::UomDataBaseHelper(DatabaseHelper *db)
{
    this->db = db ;
}

UomDataBaseHelper::~UomDataBaseHelper()
{

}

int UomDataBaseHelper::startTransaction()
{
    db->startTransaction();
}

int UomDataBaseHelper::commit()
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

int UomDataBaseHelper::insertUOM(UomDataModel *obj)
{
    startTransaction();
    QMap<QString,QVariant> map;
    map.insert(UOM_UOMName,obj->UomName);
    map.insert(UOM_DecimalPoints,obj->decimalPoints);
    map.insert(UOM_UOMSymbol,obj->UomSymbol);
    map.insert(UOM_Narration,obj->Narration);
    map.insert(UOM_Last_Modified, QDateTime::currentDateTime());

    db->insertRecordsPlus(map,UOM_TableName);

    return commit();
}

int UomDataBaseHelper::updateUOM(UomDataModel *obj, QString id)
{
    startTransaction();
    QMap<QString,QVariant> map;
    map.insert(UOM_UOMName,obj->UomName);
    map.insert(UOM_DecimalPoints,obj->decimalPoints);
    map.insert(UOM_UOMSymbol,obj->UomSymbol);
    map.insert(UOM_Narration,obj->Narration);
    map.insert(UOM_Last_Modified, QDateTime::currentDateTime());

    QMap<QString,QVariant> map1;
    map1.insert(UOM_UOMID,id);


    db->updateRecordsPlus(map,map1,UOM_TableName);
    return commit();
}

int UomDataBaseHelper::deleteUOM(QString id)
{
    startTransaction();

    QMap<QString,QVariant> map1;
    map1.insert(UOM_UOMID,id);

    db->deleteRecordsPlus(map1,UOM_TableName);
    return commit();
}

QString UomDataBaseHelper::getMaxID()
{

}

int UomDataBaseHelper::insertUOMConversion(UomConversionDataModel *obj)
{
    startTransaction();
    QMap<QString,QVariant> map;
    map.insert(UOM_Conversion_BaseUnit, obj->BaseUnit);
    map.insert(UOM_Conversion_ToUnit, obj->ToUnit);
    map.insert(UOM_Conversion_Value, obj->conValue);
    map.insert(UOM_Conversion_Narration,obj->Narration);
    map.insert(UOM_Conversion_ItemId, obj->itemID);
    map.insert(UOM_Conversion_BarCode, obj->barCode);

    db->insertRecordsPlus(map,UOM_Conversion_TableName);

    return commit();
}

int UomDataBaseHelper::updateUOMConversion(UomConversionDataModel *obj, QString id)
{

    startTransaction();
    QMap<QString,QVariant> map;
    map.insert(UOM_Conversion_BaseUnit, obj->BaseUnit);
    map.insert(UOM_Conversion_ToUnit, obj->ToUnit);
    map.insert(UOM_Conversion_Value, obj->conValue);
    map.insert(UOM_Conversion_Narration,obj->Narration);
    map.insert(UOM_Conversion_BarCode, obj->barCode);

    QMap<QString,QVariant> map1;
    map1.insert(UOM_Conversion_id,id);


    db->updateRecordsPlus(map,map1,UOM_Conversion_TableName);
    return commit();
}

int UomDataBaseHelper::deleteUOMConversion(QString itemID)
{
    startTransaction();

    QMap<QString,QVariant> map1;
    map1.insert(UOM_Conversion_ItemId,itemID);

    db->deleteRecordsPlus(map1,UOM_Conversion_TableName);
    return commit();
}

int UomDataBaseHelper::deleteBaseUOMConversion(UomConversionDataModel *obj)
{
    startTransaction();

    QMap<QString,QVariant> map1;
    map1.insert(UOM_Conversion_ItemId,obj->itemID);
    map1.insert(UOM_Conversion_BaseUnit,obj->BaseUnit);
    map1.insert(UOM_Conversion_ToUnit,obj->ToUnit);

    db->deleteRecordsPlus(map1,UOM_Conversion_TableName);
    return commit();
}

QList<UomDataModel> UomDataBaseHelper::getAllUOM()
{
    QList<UomDataModel> data;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "SELECT "+ UOM_UOMID+
            ", "+ UOM_UOMName +", "+
            UOM_DecimalPoints+", "+
            UOM_UOMSymbol + ", " +
            UOM_Narration +
            " FROM " + UOM_TableName;
    qDebug()<<qStr;
    if(!query.exec(qStr)){
        qDebug()<<query.lastError();
    }

    while (query.next()) {
        UomDataModel obj;
        obj._id = query.value(0).toString();
        obj.UomName = query.value(1).toString();
        obj.decimalPoints = query.value(2).toInt();
        obj.UomSymbol = query.value(3).toString();
        obj.Narration = query.value(4).toString();
        data.append(obj);
    }
    return data;
}

QList<UomDataModel*> UomDataBaseHelper::getAllUOMasPtr()
{
    QList<UomDataModel*> data;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "SELECT "+ UOM_UOMID+
            ", "+ UOM_UOMName +", "+
            UOM_DecimalPoints+", "+
            UOM_UOMSymbol + ", " +
            UOM_Narration +
            " FROM " + UOM_TableName;
    qDebug()<<qStr;
    if(!query.exec(qStr)){
        qDebug()<<query.lastError();
    }

    while (query.next()) {
        UomDataModel *obj = new UomDataModel();
        obj->_id = query.value(0).toString();
        obj->UomName = query.value(1).toString();
        obj->decimalPoints = query.value(2).toInt();
        obj->UomSymbol = query.value(3).toString();
        obj->Narration = query.value(4).toString();
        data.append(obj);
    }
    return data;
}


UomDataModel UomDataBaseHelper::getUomObjectByID(QString uomiD)
{
    UomDataModel data;
    qDebug()<<"uomId"<<uomiD;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "SELECT "+ UOM_UOMID+
            ", "+ UOM_UOMName +", "+
            UOM_DecimalPoints+", "+
            UOM_UOMSymbol + ", " +
            UOM_Narration +
            " FROM " + UOM_TableName;

    qStr += " where "+UOM_UOMID +"=:a1";

    query.prepare(qStr);
    query.bindValue(":a1",uomiD);

    if(!query.exec()){
        qDebug()<<query.lastError();
    }

    while (query.next()) {
        data._id = query.value(0).toString();
        data.UomName = query.value(1).toString();
        data.decimalPoints = query.value(2).toInt();
        data.UomSymbol = query.value(3).toString();
        data.Narration = query.value(4).toString();

    }
    qDebug()<<Q_FUNC_INFO<<__LINE__<<data._id;
    return data;
}

UomDataModel *UomDataBaseHelper::getUomPtrByID(QString uomiD)
{
    UomDataModel *data = new UomDataModel;
    qDebug()<<"uomId"<<uomiD;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "SELECT "+ UOM_UOMID+
            ", "+ UOM_UOMName +", "+
            UOM_DecimalPoints+", "+
            UOM_UOMSymbol + ", " +
            UOM_Narration +
            " FROM " + UOM_TableName;

    qStr += " where "+UOM_UOMID +"=:a1";

    query.prepare(qStr);
    query.bindValue(":a1",uomiD);

    if(!query.exec()){
        qDebug()<<query.lastError();
    }

    while (query.next()) {
        data->_id = query.value(0).toString();
        data->UomName = query.value(1).toString();
        data->decimalPoints = query.value(2).toInt();
        data->UomSymbol = query.value(3).toString();
        data->Narration = query.value(4).toString();

    }
    qDebug()<<Q_FUNC_INFO<<__LINE__<<data->_id;
    return data;
}
int UomDataBaseHelper::getUOMDecimalCountByID(QString id)
{
    int cnt = 0;
    QString qStr = "SELECT "+UOM_DecimalPoints + " FROM " + UOM_TableName
            + " WHERE "+UOM_UOMID + "=:id";
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);
    query.bindValue(":id",id);

    if(query.exec()){
        while (query.next()) {
            cnt =  query.value(0).toInt();

        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery(); qDebug()<<"219";
    }
    return cnt;
}

UomConversionDataModel* UomDataBaseHelper::getUOMConversionById(QString id)
{
    UomConversionDataModel *obj = new UomConversionDataModel();

    QString qStr = "SELECT " + UOM_Conversion_id + ", ";
    qStr += "(SELECT " + UOM_UOMName + " from " + UOM_TableName ;
    qStr += " WHERE " + UOM_UOMID + " = " + UOM_Conversion_BaseUnit + "), ";
    qStr += "(SELECT " + UOM_UOMName + " from " + UOM_TableName ;
    qStr += " WHERE " + UOM_UOMID + " = " + UOM_Conversion_ToUnit + "), ";
    qStr += UOM_Conversion_Value + " , ";
    qStr += UOM_Conversion_BarCode + ", ";
    qStr += UOM_Conversion_Narration ;
    qStr += " FROM " + UOM_Conversion_TableName;
    qStr += " WHERE " + UOM_Conversion_id + " = '" + id + "' ";
    qDebug()<<qStr;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    if(!query.exec(qStr)){
        qDebug()<<query.lastError();
        qDebug()<<qStr;
    }

    while (query.next()) {
        int i=0;
        obj->_id = query.value(i++).toString();
        obj->BaseUnit = query.value(i++).toString();
        obj->ToUnit = query.value(i++).toString();
        obj->conValue = query.value(i++).toFloat();
        obj->barCode = query.value(i++).toString();
        obj->Narration = query.value(i++).toString();
    }

    return obj;

}

QSqlQueryModel *UomDataBaseHelper::getDefaultUOMConversions()
{
    QSqlQueryModel *model = new QSqlQueryModel();

    QString qStr = "SELECT " + UOM_Conversion_id + ", ";
    qStr += "(SELECT " + UOM_UOMName + " FROM " + UOM_TableName ;
    qStr += " WHERE " + UOM_UOMID + " = " + UOM_Conversion_ToUnit + ") as 'Base Unit', ";
    qStr += "(SELECT " + UOM_UOMName + " FROM " + UOM_TableName ;
    qStr += " WHERE " + UOM_UOMID + " = " + UOM_Conversion_BaseUnit + ") as 'To Unit', ";
    qStr += UOM_Conversion_Value + " as 'Conversion Value' ";
    qStr += " FROM " + UOM_Conversion_TableName;
    qStr += " WHERE " +  UOM_Conversion_ItemId + " = '' ";

    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}

QMap<int, QString> UomDataBaseHelper::getUOMForSearch()
{
    QMap<int,QString> map;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "SELECT "+ UOM_UOMID+
            ", "+ UOM_UOMName +
            " FROM " + UOM_TableName;
    qDebug()<<qStr;
    if(query.exec(qStr)){
        while (query.next()) {
            map.insert(query.value(0).toInt(),query.value(1).toString());
        }
    }
    else
    {
        qDebug()<<query.lastError();
    }
    return map;
}

QSqlQueryModel *UomDataBaseHelper::getUomConversionByItem(QString Item)
{
    QSqlQueryModel *model = new QSqlQueryModel();

    QString qStr = "SELECT " ;
    qStr += UOM_Conversion_id + " as 'UOM Conversion Id', ";
    qStr += "(SELECT " + UOM_UOMName + " from " + UOM_TableName ;
    qStr += " WHERE " + UOM_UOMID + " = " + UOM_Conversion_ToUnit + ") as 'Sub Unit', ";
    qStr += UOM_Conversion_Value + " as 'Conversion Value', ";
    qStr += UOM_Conversion_BarCode + " as 'Bar Code' ";
    qStr += " FROM " + UOM_Conversion_TableName;
    qStr += " WHERE " + UOM_Conversion_ItemId + "= '" + Item + "' ";
    qStr += " AND " + UOM_Conversion_BaseUnit + " <>"+ UOM_Conversion_ToUnit;

    qDebug()<<qStr;

    model = DatabaseHelper::getQueryModel(qStr);
    return model;
}

QList<UomConversionDataModel> UomDataBaseHelper::getUomByItem(QString item)
{
    QList<UomConversionDataModel> data;
    qDebug()<<"getUomByItem called";
    //    QList<UomConversionDataModel> data;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "SELECT distinct " ;
    qStr += UOM_Conversion_id + " as 'UOM Conversion Id', ";
    qStr += UOM_Conversion_ToUnit + ", ";
    qStr += UOM_Conversion_Value + " as 'Conversion Value',  ";
    qStr += UOM_Conversion_BarCode + " as BarCode ";
    qStr += " FROM " + UOM_Conversion_TableName;
    qStr += " WHERE " + UOM_Conversion_ItemId + "= '" + item + "' ";
    qStr += " UNION ";
    qStr += " SELECT A." +  UOM_Conversion_id + " as 'UOM Conversion Id', ";
    qStr += " case when cast(A." + UOM_Conversion_BaseUnit + " as int) = cast(B." + UOM_Conversion_ToUnit +" as int)";
    qStr += " then A." + UOM_Conversion_ToUnit;
    qStr += " when cast(A." + UOM_Conversion_ToUnit + " as int) = cast(B." + UOM_Conversion_ToUnit + " as int)" ;
    qStr += " then A." + UOM_Conversion_BaseUnit;
    qStr += " else '' END , ";
    qStr += " case when cast(A." + UOM_Conversion_BaseUnit + " as int) = cast(B." + UOM_Conversion_ToUnit +" as int)" ;
    qStr += " then B." + UOM_Conversion_Value + " * A." + UOM_Conversion_Value;
    qStr += " when cast(A." + UOM_Conversion_ToUnit + " as int) = cast(B." + UOM_Conversion_ToUnit + " as int)" ;
    qStr += " then cast(B." + UOM_Conversion_Value + " / A." + UOM_Conversion_Value + " as decimal(10,3))";
    qStr += " else 1 END,  ";
    qStr += " '' as BarCode ";
    qStr += " FROM " + UOM_Conversion_TableName + " A ";
    qStr += " INNER JOIN " +  UOM_Conversion_TableName + " B " ;
    qStr += " ON ( cast(A." + UOM_Conversion_BaseUnit + " as int) = cast(B." + UOM_Conversion_ToUnit +" as int) ";
    qStr += " OR cast(A." + UOM_Conversion_ToUnit + " as int) = cast(B." + UOM_Conversion_ToUnit + " as int))";
    qStr += " AND B." + UOM_Conversion_ItemId + "= '" + item + "' ";
    qStr += " AND A." + UOM_Conversion_ItemId + " = '' ";

    qDebug()<<qStr;
    if(!query.exec(qStr)){
        qDebug()<<query.lastError();
    }

    while (query.next()) {
        UomConversionDataModel obj;
        obj._id = query.value(0).toString();
        obj.ToUnit = query.value(1).toString();
        obj.conValue = query.value(2).toFloat();
        obj.barCode = query.value(3).toString();
        data.append(obj);
    }
    return data;
}

QSqlQueryModel* UomDataBaseHelper::getUomModelByItem(QString itemID)
{

    QString qStr = "SELECT distinct " ;
    qStr += UOM_Conversion_ToUnit + ", ";
    qStr += "(SELECT "+UOM_UOMName + " FROM "+UOM_TableName + " basetable where ";
    qStr += "basetable."+UOM_UOMID +" = contable.";
    qStr += UOM_Conversion_ToUnit + ") as Name, ";
    qStr += UOM_Conversion_Value + " as 'Conversion Value',  ";
    qStr += UOM_Conversion_BarCode + "  as Barcode ";
    qStr += " FROM " + UOM_Conversion_TableName +" contable ";
    qStr += " WHERE " + UOM_Conversion_ItemId + "= '" + itemID + "' ";
    qStr += " UNION ";
    //    qStr += " SELECT A." +  UOM_Conversion_id + " as 'UOM Conversion Id', ";
    qStr += " SELECT ";
    qStr += " case when A." + UOM_Conversion_BaseUnit + " = B." + UOM_Conversion_ToUnit ;
    qStr += " then A." + UOM_Conversion_ToUnit;
    qStr += " when A." + UOM_Conversion_ToUnit + " = B." + UOM_Conversion_ToUnit ;
    qStr += " then A." + UOM_Conversion_BaseUnit;
    qStr += " else '' END , ";

    qStr += " case when A." + UOM_Conversion_BaseUnit + " = B." + UOM_Conversion_ToUnit ;
    qStr += " then (SELECT "+UOM_UOMName + " FROM "+UOM_TableName + " basetable where ";
    qStr += "basetable."+UOM_UOMID +" = A." + UOM_Conversion_ToUnit + ") ";
    qStr += " when A." + UOM_Conversion_ToUnit + " = B." + UOM_Conversion_ToUnit ;
    qStr += " then (SELECT "+UOM_UOMName + " FROM "+UOM_TableName + " basetable where ";
    qStr += "basetable."+UOM_UOMID +" = A." + UOM_Conversion_BaseUnit + ") ";
    qStr += " else '' END , ";

    qStr += " case when A." + UOM_Conversion_BaseUnit + " = B." + UOM_Conversion_ToUnit ;
    qStr += " then B." + UOM_Conversion_Value + " * A." + UOM_Conversion_Value;
    qStr += " when A." + UOM_Conversion_ToUnit + " = B." + UOM_Conversion_ToUnit ;
    qStr += " then cast(B." + UOM_Conversion_Value + " / A." + UOM_Conversion_Value + " as decimal(10,3))";
    qStr += " else 1 END ,  ";
    qStr += " ''  as Barcode ";
    qStr += " FROM " + UOM_Conversion_TableName + " A ";
    qStr += " INNER JOIN " +  UOM_Conversion_TableName + " B " ;
    qStr += " ON ( A." + UOM_Conversion_BaseUnit + " = B." + UOM_Conversion_ToUnit;
    qStr += " OR A." + UOM_Conversion_ToUnit + " = B." + UOM_Conversion_ToUnit + ")";
    qStr += " AND B." + UOM_Conversion_ItemId + "= '" + itemID + "' ";
    qStr += " AND A." + UOM_Conversion_ItemId + " = '' ";

    qDebug()<<qStr<<"getUomModelByItem";

    QSqlQueryModel* model = new QSqlQueryModel();
    model = DatabaseHelper::getQueryModel(qStr);
    return model;

}


QString UomDataBaseHelper::getUOMIDByName(QString name)
{
    QString id = "";
    QString qStr = "SELECT "+UOM_UOMID + " FROM " + UOM_TableName
            + " WHERE "+UOM_UOMName + "=:id";
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);
    query.bindValue(":id",name);

    if(query.exec()){
        while (query.next()) {
            id =  query.value(0).toString();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery(); qDebug()<<"219";
    }
    return id;
}

QString UomDataBaseHelper::getUOMNameById(QString id)
{
    QString name = "";
    QString qStr = "SELECT "+UOM_UOMName + " FROM " + UOM_TableName
            + " WHERE "+UOM_UOMID + "=:id";
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);
    query.bindValue(":id",id);

    if(query.exec()){
        while (query.next()) {
            name =  query.value(0).toString();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery(); qDebug()<<"219";
    }
    return name;
}

bool UomDataBaseHelper::checkItemExists(QString uomID)
{
    QString qStr = " SELECT 1 FROM " + UOM_TableName + " WHERE " + this->UOM_UOMID + " = '" + uomID + "' ";

    if(db->getSingletonQueryResult(qStr).toInt() == 1)
        return true;
    else
        return false;

}

bool UomDataBaseHelper::checkBarCodeExists(UomConversionDataModel *uom)
{
    QString qStr = " SELECT 1 FROM " + UOM_Conversion_TableName + " WHERE " + UOM_Conversion_BarCode + " = '" + uom->barCode + "'";
    qStr += " AND " + UOM_Conversion_id + " <> " + uom->_id;

    if(db->getSingletonQueryResult(qStr).toInt() == 1)
        return true;
    else
        return false;

}
