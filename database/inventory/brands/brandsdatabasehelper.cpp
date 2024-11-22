#include "brandsdatabasehelper.h"
#include "QMessageBox"
#include "database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h"
#include "database/Settings/configsettingsvariables.h"

QString BrandsDatabaseHelper::brandTableName = "Brands";

BrandsDatabaseHelper::BrandsDatabaseHelper(DatabaseHelper *db)
{
    this->db = db;

}

int BrandsDatabaseHelper::insertBrand(BrandsDataModel *obj)
{
    this->startTransaction();
    qDebug()<<"Saving Brand";
    QMap<QString,QVariant> map;
    map.insert(brandName,obj->brandName);
    map.insert(brandDesc,obj->brandDesc);

    obj->brandID = db->insertRecordsPlus(map,brandTableName);
    qDebug()<<Q_FUNC_INFO<<obj->brandID;
    return commit();
}

int BrandsDatabaseHelper::updateBrand(BrandsDataModel *obj, QString id)
{
    this->startTransaction();
    QMap<QString,QVariant> map;
    map.insert(brandName,obj->brandName);
    map.insert(brandDesc,obj->brandDesc);

    QMap<QString,QVariant> map1;
    map1.insert(brandID,id);


    db->updateRecordsPlus(map,map1,brandTableName);
    return commit();
}

int BrandsDatabaseHelper::deleteBrand(int id)
{
    this->startTransaction();
    QSqlQuery q(QSqlDatabase::database(DatabaseValues::connectionString));
    q.exec("select _id FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where Brand_Id='"+id+"'");
    if(q.size()>0){
        QMessageBox box;box.setText("Brand is Used in Inventory.\nCannot Delete Brand!!!"); box.exec();
        return 0;
    }

    QMap<QString,QVariant> map1;
    map1.insert(brandID,id);


    db->deleteRecordsPlus(map1,brandTableName);
    return commit();
}

int BrandsDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

int BrandsDatabaseHelper::commit()
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



QMap<int, QString> BrandsDatabaseHelper::getAllBrandsAsMap()
{
    QMap<int, QString> InventoryGroups;

    QString qStr = "SELECT "+brandID+" , "+brandName+
            "  FROM "+brandTableName+" order by "+brandName;
    // qDebug()<<qStr;
    QSqlDatabase db = QSqlDatabase::database(DatabaseValues::connectionString);
    QSqlQuery query(db);
    query.prepare(qStr);
    query.exec();
    while (query.next()) {
        InventoryGroups.insert(query.value(0).toInt(),query.value(1).toString());
    }

    return InventoryGroups;
}

QString BrandsDatabaseHelper::getBrandNameByID(QString id)
{
    QString name = "";
    QString qStr = "SELECT "+brandName + " FROM " + brandTableName
            + " WHERE "+brandID + "=:id";
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

BrandsDataModel *BrandsDatabaseHelper::getBrandById(int id)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    BrandsDataModel *obj = new BrandsDataModel;
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    QString qStr = "SELECT "+brandName + ", " +  brandDesc +" FROM " + brandTableName
            + " WHERE "+brandID + "='"+ QString::number(id) + "'";
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);
    query.bindValue(":id",id);

    qDebug()<<Q_FUNC_INFO<<__LINE__;

    if(query.exec()){
        if (query.first()) {
            qDebug()<<Q_FUNC_INFO<<__LINE__;
            obj->brandID = id;
            obj->brandName = query.value(0).toString();
            obj->brandDesc = query.value(1).toString();

        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery(); qDebug()<<"219";
    }

    qDebug()<<Q_FUNC_INFO<<__LINE__;
    return obj;

}

QList<BrandsDataModel*> BrandsDatabaseHelper::getAllBrands()
{
    QList<BrandsDataModel *> data;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "SELECT "+
            brandID +", "+
            brandName+ ", "+
            brandDesc
            +" from "+brandTableName + " order by " + brandName;
    query.exec(qStr);
    // qDebug()<<qStr;
    while(query.next()){
        BrandsDataModel *obj = new BrandsDataModel;
        obj->brandID = query.value(0).toInt();
        obj->brandName=query.value(1).toString();
        obj->brandDesc = query.value(2).toString();
        data.append(obj);

    }
    return data;
}

QSqlQueryModel *BrandsDatabaseHelper::getBrandsQueryModel()
{
    QString qStr = "SELECT ";
    qStr +=   brandID +", ";
    qStr +=   brandName+ ", ";
    qStr +=  brandDesc;
    qStr +=  " from "+brandTableName + " order by " + brandName;
    return DatabaseHelper::getQueryModel(qStr);
}

bool BrandsDatabaseHelper::checkItemExists(int BrandID)
{
    QString qStr = " SELECT 1 FROM " + brandTableName + " WHERE " + this->brandID + " = '" + BrandID + "' ";

    if(db->getSingletonQueryResult(qStr).toInt() == 1)
        return true;
    else
        return false;

}
