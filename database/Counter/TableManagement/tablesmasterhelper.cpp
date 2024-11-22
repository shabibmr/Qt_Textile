#include "tablesmasterhelper.h"

TablesMasterHelper::TablesMasterHelper() {
    db = new DatabaseHelper();
}

void TablesMasterHelper::startTransaction()
{
    db->startTransaction();
}

void TablesMasterHelper::commit()
{
    db->commitTransaction();
}

void TablesMasterHelper::insertTableMaster(TableMasterDataModel *table)
{
    startTransaction();

    QMap<QString,QVariant> map;
    table->tableID = DatabaseHelper::getUUID();

    map.insert(TablesMaster_Table_Name,table->tableName);
    map.insert(TablesMaster_Table_ID,table->tableID);

    map.insert(TablesMaster_Capacity,table->capacity);
    map.insert(TablesMaster_Min,table->min);
    map.insert(TablesMaster_Max,table->maxCapacity);
    map.insert(TablesMaster_Floor,table->floor);


    db->insertRecordsPlus(map,TablesMaster_TableName);
    commit();

}

void TablesMasterHelper::updateTableMaster(TableMasterDataModel *table)
{
    startTransaction();

    QMap<QString,QVariant> map;
    map.insert(TablesMaster_Table_ID,table->tableID);
    map.insert(TablesMaster_Table_Name,table->tableName);
    map.insert(TablesMaster_Capacity,table->capacity);
    map.insert(TablesMaster_Min,table->min);
    map.insert(TablesMaster_Max,table->maxCapacity);
    map.insert(TablesMaster_Floor,table->floor);
    QMap<QString,QVariant> clause;
    clause.insert(TablesMaster_id,table->_id);

    db->updateRecordsPlus(map,clause,TablesMaster_TableName);
    commit();
}

void TablesMasterHelper::deleteTableMaster(TableMasterDataModel *table)
{
    startTransaction();
    QMap<QString,QVariant> clause;
    clause.insert(TablesMaster_id,table->_id);
    db->deleteRecordsPlus(clause,TablesMaster_TableName);
    commit();
}

TableMasterDataModel *TablesMasterHelper::getTableById(QString tableId)
{
    TableMasterDataModel *table = new TableMasterDataModel();

    QString qStr ="Select ";
    qStr += TablesMaster_id +", ";
    qStr += TablesMaster_Table_ID +", ";
    qStr += TablesMaster_Table_Name +", ";
    qStr += TablesMaster_Capacity +", ";
    qStr += TablesMaster_Min +", ";
    qStr += TablesMaster_Max +", ";
    qStr += TablesMaster_Floor ;
    qStr += " FROM "+TablesMaster_TableName;
    qStr += " WHERE ";
    qStr += TablesMaster_Table_ID + "=:id";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);
    query.bindValue(":id",tableId);
    if(query.exec()){
        while(query.next()){
            table->_id=query.value(0).toInt();
            table->tableID = query.value(1).toString();
            table->tableName=query.value(2).toString();
            table->capacity=query.value(3).toInt();
            table->min=query.value(4).toInt();
            table->maxCapacity=query.value(5).toInt();
            table->floor=query.value(6).toString();
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastError();
    }

    return table;
}

QSqlQueryModel *TablesMasterHelper::getTableNamesForSearch()
{
    QString qStr = "SELECT ";
    qStr += TablesMaster_Table_ID+", ";
    qStr += TablesMaster_Table_Name;
    qStr += " FROM "+ TablesMaster_TableName;
    qStr += " ORDER BY "+TablesMaster_Table_Name;

    return DatabaseHelper::getQueryModelL(qStr);
}
