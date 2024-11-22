#ifndef TABLERESERVATIONDATABASEHELPER_H
#define TABLERESERVATIONDATABASEHELPER_H
#include "database/databasehelper/databasehelper.h"
#include "datamodels/Counter/tablereservationdatamodel.h"
class TableReservationDatabaseHelper
{
public:
    TableReservationDatabaseHelper();

    QString TablesRes_Table_Name="Table_Reservation";
    QString TableRes_id="_id";
    QString TableRes_ReservationID = "Reservation_ID";
    QString TableRes_AddedDate = "Added_Date";
    QString TableRes_ResDate = "Reservation_Date";
    QString TableRes_FromTime = "from_time";
    QString TableRes_ToTime = "to_time";
    QString TableRes_PAX = "pax";
    QString TableRes_ContactID = "contact_id";
    QString TableRes_Narration = "narration";
    QString TableRes_TableID = "table_id";







    void insertReservation(TableReservationDataModel *obj);
    void updateReservation(TableReservationDataModel *obj);
    void deleteReservation(TableReservationDataModel *obj);


    DatabaseHelper *db;

    void startTransaction();
    void commit();
    TableReservationDataModel *getTableReservationById(QString id);


};

#endif // TABLERESERVATIONDATABASEHELPER_H
