#include "categorydatabasehelper.h"
#include "QMessageBox"
#include "database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h"
#include "database/Settings/configsettingsvariables.h"

QString CategoryDatabaseHelper::CategoryTableName = "Category";

CategoryDatabaseHelper::CategoryDatabaseHelper(DatabaseHelper *db)
{
    this->db = db;

}

int CategoryDatabaseHelper::insertCategory(CategoryDataModel *obj)
{
    this->startTransaction();
    qDebug()<<"Saving Category";
    QMap<QString,QVariant> map;
    map.insert(CategoryName,obj->CategoryName);
    map.insert(CategoryDesc,obj->CategoryDesc);

    obj->CategoryID = db->insertRecordsPlus(map,CategoryTableName);
    qDebug()<<Q_FUNC_INFO<<obj->CategoryID;
    return commit();
}

int CategoryDatabaseHelper::updateCategory(CategoryDataModel *obj, QString id)
{
    this->startTransaction();
    QMap<QString,QVariant> map;
    map.insert(CategoryName,obj->CategoryName);
    map.insert(CategoryDesc,obj->CategoryDesc);

    QMap<QString,QVariant> map1;
    map1.insert(Category_CategoryID,id);


    db->updateRecordsPlus(map,map1,CategoryTableName);
    return commit();
}

int CategoryDatabaseHelper::deleteCategory(int id)
{
    this->startTransaction();
    QSqlQuery q(QSqlDatabase::database(DatabaseValues::connectionString));
    q.exec("select _id FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" where Category_Id='"+id+"'");
    if(q.size()>0){
        QMessageBox box;box.setText("Category is Used in Inventory.\nCannot Delete Category!!!"); box.exec();
        return 0;
    }

    QMap<QString,QVariant> map1;
    map1.insert(Category_CategoryID,id);


    db->deleteRecordsPlus(map1,CategoryTableName);
    return commit();
}

int CategoryDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

int CategoryDatabaseHelper::commit()
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



QMap<int, QString> CategoryDatabaseHelper::getAllCategoryAsMap()
{
    QMap<int, QString> InventoryGroups;

    QString qStr = "SELECT "+Category_CategoryID+" , "+CategoryName+
            "  FROM "+CategoryTableName+" order by "+CategoryName;
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

QString CategoryDatabaseHelper::getCategoryNameByID(int id)
{
    QString name = "";
    QString qStr = "SELECT "+CategoryName + " FROM " + CategoryTableName
            + " WHERE "+Category_CategoryID + "=:id";
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

CategoryDataModel *CategoryDatabaseHelper::getCategoryById(int id)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    CategoryDataModel *obj = new CategoryDataModel;
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    QString qStr = "SELECT "+CategoryName + ", " +  CategoryDesc +" FROM " + CategoryTableName
            + " WHERE "+Category_CategoryID + "='"+ QString::number(id) + "'";
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);
    query.bindValue(":id",id);

    qDebug()<<Q_FUNC_INFO<<__LINE__;

    if(query.exec()){
        if (query.first()) {
            qDebug()<<Q_FUNC_INFO<<__LINE__;
            obj->CategoryID = id;
            obj->CategoryName = query.value(0).toString();
            obj->CategoryDesc = query.value(1).toString();

        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery(); qDebug()<<"219";
    }

    qDebug()<<Q_FUNC_INFO<<__LINE__;
    return obj;

}

QList<CategoryDataModel*> CategoryDatabaseHelper::getAllCategory()
{
    QList<CategoryDataModel *> data;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "SELECT "+
            Category_CategoryID +", "+
            CategoryName+ ", "+
            CategoryDesc
            +" from "+CategoryTableName + " order by " + CategoryName;
    query.exec(qStr);
    // qDebug()<<qStr;
    while(query.next()){
        CategoryDataModel *obj = new CategoryDataModel;
        obj->CategoryID = query.value(0).toInt();
        obj->CategoryName=query.value(1).toString();
        obj->CategoryDesc = query.value(2).toString();
        data.append(obj);

    }
    return data;
}

QSqlQueryModel *CategoryDatabaseHelper::getCategoryQueryModel()
{
    QString qStr = "SELECT ";
    qStr +=   Category_CategoryID +", ";
    qStr +=   CategoryName+ ", ";
    qStr +=  CategoryDesc;
    qStr +=  " from "+CategoryTableName + " order by " + CategoryName;
    return DatabaseHelper::getQueryModel(qStr);
}

bool CategoryDatabaseHelper::checkItemExists(int CategoryID)
{
    QString qStr = " SELECT 1 FROM " + CategoryTableName + " WHERE " + this->Category_CategoryID + " = '" + CategoryID + "' ";

    if(db->getSingletonQueryResult(qStr).toInt() == 1)
        return true;
    else
        return false;

}
