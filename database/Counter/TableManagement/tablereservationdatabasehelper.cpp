#include "tablereservationdatabasehelper.h"

TableReservationDatabaseHelper::TableReservationDatabaseHelper() {}

void TableReservationDatabaseHelper::insertReservation(TableReservationDataModel *obj)
{
    startTransaction();
    QMap<QString,QVariant> map;
    obj->reservationID = DatabaseHelper::getUUID();

    map.insert(TableRes_ReservationID,obj->reservationID);
    map.insert(TableRes_ResDate,obj->resDate);
    map.insert(TableRes_AddedDate,obj->addedDate);
    map.insert(TableRes_FromTime,obj->fromTime);
    map.insert(TableRes_ToTime,obj->toTime);
    map.insert(TableRes_PAX,obj->pax);
    map.insert(TableRes_Narration,obj->narration);
    map.insert(TableRes_ContactID,obj->contactID);
    map.insert(TableRes_TableID,obj->table->tableID);

    db->insertRecordsPlus(map,TablesRes_Table_Name);
    commit();
}

void TableReservationDatabaseHelper::updateReservation(TableReservationDataModel *obj)
{
    QMap<QString,QVariant> map;

    map.insert(TableRes_ReservationID,obj->reservationID);
    map.insert(TableRes_ResDate,obj->resDate);
    map.insert(TableRes_AddedDate,obj->addedDate);
    map.insert(TableRes_FromTime,obj->fromTime);
    map.insert(TableRes_ToTime,obj->toTime);
    map.insert(TableRes_PAX,obj->pax);
    map.insert(TableRes_Narration,obj->narration);
    map.insert(TableRes_ContactID,obj->contactID);
    map.insert(TableRes_TableID,obj->table->tableID);

}

void TableReservationDatabaseHelper::deleteReservation(TableReservationDataModel *obj)
{

}

void TableReservationDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

void TableReservationDatabaseHelper::commit()
{
    db->commitTransaction();
}

TableReservationDataModel *TableReservationDatabaseHelper::getTableReservationById(QString id)
{

}
