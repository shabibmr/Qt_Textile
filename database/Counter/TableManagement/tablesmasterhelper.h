#ifndef TABLESMASTERHELPER_H
#define TABLESMASTERHELPER_H
#include "database/databasehelper/databasehelper.h"
#include "datamodels/Counter/tablemasterdatamodel.h"

class TablesMasterHelper
{
public:
    TablesMasterHelper();

    QString TablesMaster_TableName = "Tables_Master";
    QString TablesMaster_id="_id";
    QString TablesMaster_Table_ID="Table_ID";
    QString TablesMaster_Table_Name="Table_Name";
    QString TablesMaster_Capacity = "Capacity";
    QString TablesMaster_Floor = "Floor";
    QString TablesMaster_Min="Min";
    QString TablesMaster_Max="Max";

    DatabaseHelper *db;

    void startTransaction();
    void commit();

    void insertTableMaster(TableMasterDataModel *table);
    void updateTableMaster(TableMasterDataModel *table);
    void deleteTableMaster(TableMasterDataModel *table);


    TableMasterDataModel *getTableById(QString tableId);
    QSqlQueryModel *getTableNamesForSearch();

};

#endif // TABLESMASTERHELPER_H
