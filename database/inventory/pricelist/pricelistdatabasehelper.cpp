#include "pricelistdatabasehelper.h"
#include "database/Settings/configsettingsvariables.h"


QString PricelistDatabaseHelper::PriceListName = "Price_List_Name";
QString PricelistDatabaseHelper::PriceLists_TableName = "Price_Lists";
QString PricelistDatabaseHelper::PriceListID = "Price_List_ID";
QString PricelistDatabaseHelper::PriceListEntries_TableName = "PriceList_Entries";
QString PricelistDatabaseHelper::PriceListEntries_ItemID = "Item_Id";

PricelistDatabaseHelper::PricelistDatabaseHelper(DatabaseHelper *db)
{
    this->db = db;
}

PricelistDatabaseHelper::~PricelistDatabaseHelper()
{
    //    delete db;
}

int PricelistDatabaseHelper::insertPriceList(PriceListDataObject *obj)
{
    qDebug()<<"Starting Insert";
    db = new DatabaseHelper();
    db->startTransaction();

    QMap<QString,QVariant> map;
    if(obj->isDefault == true){
        QMap<QString,QVariant> mapx;
        mapx.insert(PriceListDefault,false);
        db->updateRecordsPlus(mapx,map,PriceLists_TableName);
    }

    map.insert(PriceListName,obj->priceListName);
    map.insert(PriceListStartDate,obj->dateFrom.toString());
    map.insert(PriceListEndDate,obj->dateto.toString());
    map.insert(PriceListDefault,obj->isDefault);
    map.insert(PriceListShowInPos,obj->showinPOS);
    map.insert(PriceList_Last_Modified, QDateTime::currentDateTime());
    qDebug()<<"Inserting Price List ";
    int ListId = db->insertRecordsPlus(map,PriceLists_TableName);


    qDebug()<<"Inserting Price List details no"<<obj->items.size()<<"and list id = "<<ListId;
    for(inventoryItemDataModel item:obj->items){
        QMap<QString,QVariant> map1;
        map1.insert(PriceListEntries_PriceListID,ListId);
        map1.insert(PriceListEntries_ItemID,item.ItemID);
        map1.insert(PriceListEntries_Price,item.price);
        map1.insert(PriceListEntries_Percent,item.discountPercentage);
        map1.insert(PriceListEntries_UomId,item.uomObject._id);
        map1.insert(PriceListEntries_Last_Modified, QDateTime::currentDateTime());
        qDebug()<<"uom added "<<item.uomObject._id;
        db->insertRecordsPlus(map1,PriceListEntries_TableName);
    }

    return commit();
}

int PricelistDatabaseHelper::updatePriceList(PriceListDataObject *obj, int id)
{
    startTransaction();

    //    QMap<QString,QVariant> mapxx;
    //    if(obj->isDefault == true){
    //        QMap<QString,QVariant> mapx;
    //        mapx.insert(PriceListDefault,false);
    //        db->updateRecordsPlus(mapx,mapxx,PriceLists_TableName);
    //    }

    QMap<QString,QVariant> map1;
    map1.insert(PriceListID,id);

    db->deleteRecordsPlus(map1,PriceListEntries_TableName);
    QMap<QString,QVariant> map;
    map.insert(PriceListDefault,obj->isDefault);
    map.insert(PriceListName,obj->priceListName);
    map.insert(PriceListStartDate,obj->dateFrom);
    map.insert(PriceListEndDate,obj->dateto);
    map.insert(PriceListDefault,obj->isDefault);
    map.insert(PriceListShowInPos,obj->showinPOS);
    map.insert(PriceList_Last_Modified, QDateTime::currentDateTime());
    db->updateRecordsPlus(map,map1,PriceLists_TableName);

    for(inventoryItemDataModel item:obj->items){
        QMap<QString,QVariant> map2;
        map2.insert(PriceListEntries_PriceListID,id);
        map2.insert(PriceListEntries_ItemID,item.ItemID);
        map2.insert(PriceListEntries_Price,item.price);
        map2.insert(PriceListEntries_Percent,item.discountPercentage);
        qDebug()<<"uom added "<<item.uomObject._id<<item.uomObject.UomName;
        map2.insert(PriceListEntries_UomId,item.uomObject._id);
        map2.insert(PriceListEntries_Last_Modified, QDateTime::currentDateTime());
        db->insertRecordsPlus(map2,PriceListEntries_TableName);

        SalesInventoryItemDatabaseHelper itemHelper;
        itemHelper.updateLastModified(item.ItemID);
    }


    return commit();
}

int PricelistDatabaseHelper::deletePriceList(QString id)
{
    startTransaction();
    QMap<QString,QVariant> map1;
    map1.insert(PriceListID,id);

    db->deleteRecordsPlus(map1,PriceLists_TableName);

    QMap<QString,QVariant> map2;
    map2.insert(PriceListEntries_PriceListID,id);

    db->deleteRecordsPlus(map2,PriceListEntries_TableName);

    return commit();
}

QString PricelistDatabaseHelper::getMaxID()
{
    QSqlDatabase db = QSqlDatabase::database(DatabaseValues::connectionString);
    QSqlQuery query(db);
    int id =0;
    QString qStr = "select max(_id) from "+PriceListEntries_TableName;
    query.exec(qStr);
    while(query.next()){
        id = query.value(0).toInt();
    }
    return QString::number(id+1);
}

QString PricelistDatabaseHelper::getPriceListNameByID(QString id)
{
    QSqlDatabase db = QSqlDatabase::database(DatabaseValues::connectionString);
    QString qStr = "Select "+PriceListName+" from "+PriceLists_TableName+
                   " where "+ PriceListID +" like :id";
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

int PricelistDatabaseHelper::getPriceListIDByName(QString name)
{
    QSqlDatabase db = QSqlDatabase::database(DatabaseValues::connectionString);
    QString qStr = "Select "+PriceListID+" from "+PriceLists_TableName+
                   " where "+ PriceListName +" like :name";
    // qDebug()<<qStr <<name;
    QSqlQuery query(db);
    int id = 0;

    query.prepare(qStr);
    query.bindValue(":name",name);
    query.exec();

    if(query.next()){
        id = query.value(0).toInt();
    }

    return id;

}

float PricelistDatabaseHelper::getPriceByItemID(QString itemID, QString priceListID)
{
    QString qStr;
    SalesInventoryItemDatabaseHelper shelp;
    float value=0;

    if(priceListID.length()==0){
        qStr += "SELECT "+shelp.Sales_Inventory_Price + " from ";
        qStr += shelp.Sales_Inventory_Table_Name+ " where ";
        qStr += shelp.Sales_Inventory_ItemId + "='"+itemID +"'";
    }
    else{
        qStr += "SELECT "+PriceListEntries_Price+ " FROM "+PriceListEntries_TableName;
        qStr += " where "+PriceListEntries_ItemID +"= '"+itemID +"' and ";
        qStr += PriceListEntries_PriceListID + "='"+priceListID+"'";

    }

    QSqlQueryModel *model = DatabaseHelper::getQueryModel(qStr);


    if(model->rowCount()==0){
        qStr = "SELECT "+shelp.Sales_Inventory_Price + " from ";
        qStr += shelp.Sales_Inventory_Table_Name+ " where ";
        qStr += shelp.Sales_Inventory_ItemId + "='"+itemID +"'";
        value = DatabaseHelper::getSingletonQueryResult(qStr).toFloat();
    }
    else{

        value = model->record(0).value(0).toFloat();
    }

    return value;

}

float PricelistDatabaseHelper::getPriceByItemIDAndUOM(QString itemID, QString priceListID, QString uomId)
{
    QString qStr;
    SalesInventoryItemDatabaseHelper shelp;
    float value=-1;


    qStr = " SELECT distinct coalesce(s.price, -1)  as calculatedPrice ";
    qStr += " FROM " + PriceListEntries_TableName + " s RIGHT OUTER JOIN ";
    qStr +=  UomDataBaseHelper::qStrUomForItemDetail + " as subq ";
    qStr +=  " ON s." + PriceListEntries_ItemID + " = subq.itemID ";
    qStr += " AND (s.UOM_Id =  subq.uom )";
    qStr += " WHERE " + PriceListEntries_PriceListID + " = '" + priceListID + "'";
    qStr += " AND " + PriceListEntries_ItemID + " = '" + itemID + "'";
    qStr += " AND " + PriceListEntries_UomId + " = '" + uomId + "'";

    //    qDebug()<<qStr;
    value = DatabaseHelper::getSingletonQueryResult(qStr).toFloat();

    if(value != NULL)
        return value;
    else
        return -1;

}


float PricelistDatabaseHelper::getPriceByItemIDAndDate(QString itemID, QString priceListID, QDate Date)
{

}

QSqlQueryModel *PricelistDatabaseHelper::getAllPricesOfItem(QString itemID)
{
    //    qDebug()<<"Get Price";
    QString qStr = " SELECT main." + PriceListID + " as 'Id' , " + PriceListName + " as 'PriceList' , ";
    qStr += PriceListEntries_Price+ " as Price FROM "+ PriceLists_TableName + " main JOIN " + PriceListEntries_TableName + " entry ";
    qStr += " ON main." + PriceListID + " = entry." + PriceListEntries_PriceListID;
    qStr += " where "+PriceListEntries_ItemID +"= '"+itemID +"'";


    return DatabaseHelper::getQueryModel(qStr);

}

QMap<QString, float> PricelistDatabaseHelper::getAllPricesOfItemAsMap(QString itemID)
{

    QString qStr = " SELECT main." + PriceListID + " as 'Id' , " + PriceListName + " as 'PriceList' , ";
    qStr += PriceListEntries_Price+ " as Price FROM "+ PriceLists_TableName + " main JOIN " ;
    qStr += PriceListEntries_TableName + " entry ";
    qStr += " ON main." + PriceListID + " = entry." + PriceListEntries_PriceListID;
    qStr += " where "+PriceListEntries_ItemID +"= '"+itemID +"'";

    //    qDebug()<<qStr;
    QMap<QString,float> map;
    QSqlQueryModel *model =  DatabaseHelper::getQueryModel(qStr);

    for (int i=0;i<model->rowCount();i++) {
        map.insert(model->record(i).value(0).toString(),  model->record(i).value(2).toFloat());
    }

    return map;
}

void PricelistDatabaseHelper::deletePricesByItem(QString itemID)
{
    QMap<QString,QVariant> clause;
    clause.insert(PriceListEntries_ItemID,itemID);
    startTransaction();
    db->deleteRecordsPlus(clause,PriceListEntries_TableName);
    commit();
}

void PricelistDatabaseHelper::insertItemPrices(QString itemID, QString priceID, QString uomID, float price)
{
    startTransaction();
    QMap<QString,QVariant> map1;
    map1.insert(PriceListEntries_PriceListID,priceID);
    map1.insert(PriceListEntries_ItemID,itemID);
    map1.insert(PriceListEntries_UomId, uomID );
    map1.insert(PriceListEntries_Price,price);

    db->insertRecordsPlus(map1,PriceListEntries_TableName);
    commit();

}

QMap<QString, QString> PricelistDatabaseHelper::getPriceListAsMap()
{
    QMap<QString, QString> pricesMap;

    QString qStr = "SELECT "+PriceListID+" , "+PriceListName+
                   "  FROM "+PriceLists_TableName+" order by "+PriceListName;
    //    qDebug()<<qStr;
    QSqlDatabase db = QSqlDatabase::database(DatabaseValues::connectionString);
    QSqlQuery query(db);
    query.prepare(qStr);
    query.exec();
    while (query.next()) {
        pricesMap.insert(query.value(0).toString(),query.value(1).toString());
    }

    return pricesMap;

}

QList<PriceListDataObject> PricelistDatabaseHelper::getAllPriceLists(bool all)
{
    QList<PriceListDataObject> data;
    QString qStr = "SELECT " + PriceListID+ ", " + PriceListName+", "+ PriceListStartDate + ", "+ PriceListEndDate
                   + " FROM "+ PriceLists_TableName;
    if(!all)
    qStr += " WHERE "+ PriceListShowInPos + "=1";
    qStr += " ORDER BY "+PriceListID;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    if(query.exec(qStr)){
        while(query.next()){
            PriceListDataObject obj;
            obj.priceListID = query.value(0).toInt();
            obj.priceListName = query.value(1).toString();
            obj.dateFrom = query.value(2).toDate();
            obj.dateto = query.value(3).toDate();
            data.append(obj);
        }
    }
    else{
        qDebug()<<query.lastError();
    }
    return data;
}

QSqlQueryModel *PricelistDatabaseHelper::getPriceListsAsQueryModel()
{
    QString qStr = "";
    qStr += " SELECT ";
    qStr += PriceListID +" , ";
    qStr += PriceListName + "'Price List', ";
    qStr += PriceListStartDate + " 'Start Date', ";
    qStr += PriceListEndDate + " 'End Date', ";
    qStr += PriceListShowInPos + " 'show' ";
    qStr += " FROM " + PriceLists_TableName;


    return  DatabaseHelper::getQueryModel(qStr);;

}


PriceListDataObject PricelistDatabaseHelper::getPriceListByName(QString priceName)
{
    //    qDebug()<<"get Price Obj";
    int priceID = getPriceListIDByName(priceName);

    PriceListDataObject data;
    SalesInventoryItemDatabaseHelper* invhelper = new SalesInventoryItemDatabaseHelper();
    QString qStr = "SELECT "+PriceListEntries_ItemID +", "+PriceListEntries_Price +", "+PriceListEntries_Percent
                   + " FROM " + PriceListEntries_TableName + " WHERE " + PriceListEntries_PriceListID + " = :ID";
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);
    query.bindValue(":ID",priceID);


    if(query.exec()){
        while (query.next()) {
            inventoryItemDataModel* item = new inventoryItemDataModel();

            item->ItemID = query.value(0).toString();
            item->ItemName = invhelper->getInventoryItemNameByID(query.value(0).toString());
            item->price = query.value(1).toFloat();
            item->discountPercentage = query.value(2).toFloat();
            item->brandName ="";
            item->ItemCode = "";

            data.items.append(*item);
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<qStr;
    }

    qStr = "SELECT "+ PriceListStartDate+", "+ PriceListEndDate + " FROM "+ PriceLists_TableName
           + " WHERE "+ PriceListID + "=:id";
    query.prepare(qStr);
    query.bindValue(":id",priceID);
    if(query.exec()){
        while(query.next()){
            data.dateFrom = query.value(0).toDate();
            data.dateto = query.value(1).toDate();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<qStr;
    }
    data.priceListName = priceName;
    data.priceListID = priceID;
    return data;
}

PriceListDataObject PricelistDatabaseHelper::getPriceListByID(int priceListID)
{
    //    qDebug()<<"get Price Obj";

    PriceListDataObject data;
    SalesInventoryItemDatabaseHelper* invhelper = new SalesInventoryItemDatabaseHelper();
    QString qStr = "SELECT "+PriceListEntries_ItemID +", "+PriceListEntries_Price +", "+PriceListEntries_Percent
                   + " FROM " + PriceListEntries_TableName + " WHERE " + PriceListEntries_PriceListID + " = :ID";
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);
    query.bindValue(":ID",priceListID);


    if(query.exec()){
        while (query.next()) {
            inventoryItemDataModel* item = new inventoryItemDataModel();

            item->ItemID = query.value(0).toString();
            item->ItemName = invhelper->getInventoryItemNameByID(query.value(0).toString());
            item->price = query.value(1).toFloat();
            item->discountPercentage = query.value(2).toFloat();
            item->brandName ="";
            item ->ItemCode = "";

            data.items.append(*item);
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<qStr;
    }
    QString priceName="";
    qStr = "SELECT "+ PriceListStartDate+", "+ PriceListEndDate +","+ PriceListName + " FROM "+ PriceLists_TableName
           + " WHERE "+ PriceListID + "=:id";
    query.prepare(qStr);
    query.bindValue(":id",priceListID);
    if(query.exec()){
        while(query.next()){
            data.dateFrom = query.value(0).toDate();
            data.dateto = query.value(1).toDate();
            priceName = query.value(2).toString();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<qStr;
    }
    data.priceListName = priceName;
    data.priceListID = priceListID;
    return data;
}

PriceListDataObject *PricelistDatabaseHelper::getPriceListByIDasPtr(int priceListID)
{
    PriceListDataObject *data = new PriceListDataObject;
    SalesInventoryItemDatabaseHelper* invhelper = new SalesInventoryItemDatabaseHelper();
    UomDataBaseHelper *uomHelper = new UomDataBaseHelper;

    QString qStr = "SELECT distinct "+PriceListEntries_ItemID +", "+PriceListEntries_Price +", "+PriceListEntries_Percent + ", " + PriceListEntries_UomId;
    qStr += ", subq.convalue , subq.UomName, subq.UomSymbol ";
    qStr += " FROM " + PriceListEntries_TableName + " s LEFT OUTER JOIN ";
    qStr +=  UomDataBaseHelper::qStrUomForItemDetail + " as subq ";
    qStr +=  " ON s." + PriceListEntries_ItemID + " = subq.itemID ";

    qStr += " AND s." + PriceListEntries_UomId + " = subq.UOM ";
    qStr += " WHERE " + PriceListEntries_PriceListID + " = :ID";
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);
    query.bindValue(":ID",priceListID);

    qDebug()<<qStr;


    if(query.exec()){
        while (query.next()) {
            inventoryItemDataModel item ;
            item.ItemID = query.value(0).toString();
            item.ItemName = invhelper->getInventoryItemNameByID(item.ItemID);
            //            item.GroupName = invhelper->getInventoryItemNameByID(item.ItemID);
            item.price = query.value(1).toFloat();
            item.discountPercentage = query.value(2).toFloat();
            item.uomObject._id = query.value(3).toString();
            qDebug()<<Q_FUNC_INFO<<__LINE__<<item.uomObject._id;
            if(item.uomObject._id.length() == 0){
                item.uomObject = uomHelper->getUomObjectByID(invhelper->getInventoryItemByID(item.ItemID).defaultUOMID);
                qDebug()<<Q_FUNC_INFO<<__LINE__<<item.uomObject._id;
            }
            else{

                item.uomObject.convRate = query.value(4).toFloat();
                item.uomObject.UomName = query.value(5).toString();
                item.uomObject.UomSymbol = query.value(6).toString();
            }
            item.brandName ="";
            item.ItemCode = "";
            data->items.append(item);
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<qStr;
    }

    QString priceName="";
    qStr = "SELECT "+ PriceListStartDate+", "+ PriceListEndDate +","+ PriceListName
           +","+ PriceListDefault+", "+PriceListShowInPos+ " FROM "+ PriceLists_TableName
           + " WHERE "+ PriceListID + "=:id";
    query.prepare(qStr);
    query.bindValue(":id",priceListID);
    if(query.exec()){
        while(query.next()){
            data->dateFrom = query.value(0).toDate();
            data->dateto = query.value(1).toDate();
            priceName = query.value(2).toString();
            data->isDefault =  query.value(3).toBool();
            data->showinPOS = query.value(4).toBool();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<qStr;
    }
    data->priceListName = priceName;
    data->priceListID = priceListID;

    for(int i=0;i< data->items.length();i++){
        qDebug()<<data->items[i].ItemName<<data->items[i].uomObject.UomSymbol;
    }
    return data;
}

QString PricelistDatabaseHelper::getDefaultPriceList(QString ledgerID)
{
    QString qStr;
    LedgerMasterDatabaseHelper led;

    if(ledgerID.length() >0){
        qStr = " SELECT " + led.Ledger_Master_defaultPriceList + " from ";
        qStr += led.Ledger_Master_TableName + " WHERE " + led.Ledger_Master_LedgerID + " = '" +ledgerID + "'";
        QString val = DatabaseHelper::getSingletonQueryResult(qStr).toString();
        if(val.length() > 0)
            return val;
    }

    qStr = "SELECT "+PriceListID + " from "+PriceLists_TableName + " WHERE " ;
    qStr += PriceListDefault +"=1";


    QString val = DatabaseHelper::getSingletonQueryResult(qStr).toString();
    return val;
}

int PricelistDatabaseHelper::startTransaction()
{
    db->startTransaction();

}

int PricelistDatabaseHelper::commit()
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



