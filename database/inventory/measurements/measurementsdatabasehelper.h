#ifndef MEASUREMENTSDATABASEHELPER_H
#define MEASUREMENTSDATABASEHELPER_H

#include "database/databasehelper/databasehelper.h"
#include "datamodels/inventory/measurements/contactmeasurement.h"

class MeasurementsDatabaseHelper
{
public:
    MeasurementsDatabaseHelper(DatabaseHelper *db= new DatabaseHelper());

    QString MEASUREMENT_MASTER_tablename ="measurement_master";
    QString MEASUREMENT_MASTER_ID = "_ID";
    QString MEASUREMENT_MASTER_MEASURE_id = "MEASUREMENT_id";
    QString MEASUREMENT_MASTER_NAME = "MEASUREMENT_NAME";

    QString MEASUREMENT_LABLES_TABLENAME = "measurement_lables";
    QString MEASUREMENT_LABLES_ID = "_ID";
    QString MEASUREMENT_LABLES_Master_ID="Measure_Master_ID";
    QString MEASUREMENT_LABLES_LABLE_ID ="Measure_Lable_ID";
    QString MEASUREMENT_LABLES_Lable_Name="Measure_Lable_Name";

    QString CONTACT_MEASURE_MAIN_TABLENAME = "contact_measurements_main";
    QString CONTACT_MEASURE_id = "_id";
    QString CONTACT_MEASURE_Contact_Measurement_ID="Contact_Measurement_ID";
    QString CONTACT_MEASURE_Contact_ID="Contact_ID";
    QString CONTACT_MEASURE_Measure_Master_ID="Measure_Master_ID";
    QString CONTACT_MEASURE_Reference="MEASURE_Reference";

    QString CONTACT_MEASURE_Details_TableName="contact_measurements_details";
    QString CONTACT_MEASURE_Details_ID ="_ID";
    QString CONTACT_MEASURE_Measurement_ID = "Contact_Measurement_ID";
    QString CONTACT_MEASURE_Lable_ID="Measure_Lable_ID";
    QString CONTACT_MEASURE_Lable_Name="Measure_Lable_Name";
    QString CONTACT_MEASURE_VALUE = "Measurement_Value";
    QString CONTACT_MEASURE_CREATED_DATE = "CREATED_DATE";
    QString CONTACT_MEASURE_LAST_MODIFIED_ON = "last_modified_on";


    QString Voucher_Measurement_Mapping_TableName = "Voucher_Measurement_Mapping";
    QString Voucher_Measurement_Mapping_id = "_id";
    QString Voucher_Measurement_Mapping_Contact_Measure_ID = "Contact_Measurement_ID";
    QString Voucher_Measurement_Mapping_Transaction_ID = "Transaction_ID";
    QString Voucher_Measurement_Mapping_Time_Stamp = "Time_Stamp";
    QString Voucher_Measurement_Mapping_Item_Trans_Id = "Item_Trans_Id";



    int insertMeasurementMaster(MeasurementMaster *obj);
    int updateMeasurementMaster(MeasurementMaster *obj);
    int deleteMeasurementMaster(MeasurementMaster *obj);

    MeasurementMaster *getMeasurementMasterByID(QString id);

    QSqlQueryModel *getAllMeasurements();
    QList<MeasurementLabel*> getLablesOfMeasurement(MeasurementMaster *obj);

    int insertContactMeasurement(ContactMeasurement *obj);
    int updateContactMeasurement(ContactMeasurement *obj);
    int deleteContactMeasurement(ContactMeasurement *obj);

    ContactMeasurement *getContactMeasurement(ContactMeasurement *cMeasurement = new ContactMeasurement());
    QSqlQueryModel *getMeasurementReferencesByContact(QString contact);

    QList<ContactMeasurement*> getAllMeasurementsByContact(QString contactID);

    int startTransaction();
    int commit();


    bool flag = true;
    DatabaseHelper *db;
};

#endif // MEASUREMENTSDATABASEHELPER_H
