#include "measurementsdatabasehelper.h"
#include <QSqlRecord>

MeasurementsDatabaseHelper::MeasurementsDatabaseHelper(DatabaseHelper *db)
{
    this->db= db;
}

int MeasurementsDatabaseHelper::insertMeasurementMaster(MeasurementMaster *obj)
{
    startTransaction();
    QMap<QString,QVariant> map;
    if(obj->measurementMasterID.length()==0){
        obj->measurementMasterID = db->getUUID();
    }

    map.insert(MEASUREMENT_MASTER_MEASURE_id,obj->measurementMasterID);
    map.insert(MEASUREMENT_MASTER_NAME,obj->measurementName);

    db->insertRecordsPlus(map,MEASUREMENT_MASTER_tablename);

    for(int i=0;i<obj->lables.length();i++){
        map.clear();
        if(obj->lables[i].labelID.length()==0){
            obj->lables[i].labelID = db->getUUID();
        }
        map.insert(MEASUREMENT_LABLES_LABLE_ID,obj->lables[i].labelID);
        map.insert(MEASUREMENT_LABLES_Lable_Name,obj->lables[i].labelName);
        map.insert(MEASUREMENT_LABLES_Master_ID,obj->measurementMasterID);
        db->insertRecordsPlus(map,MEASUREMENT_LABLES_TABLENAME);
    }
    return commit();
}

int MeasurementsDatabaseHelper::updateMeasurementMaster(MeasurementMaster *obj)
{
    startTransaction();
    QMap<QString,QVariant> map,clause;


    map.insert(MEASUREMENT_MASTER_MEASURE_id,obj->measurementMasterID);
    map.insert(MEASUREMENT_MASTER_NAME,obj->measurementName);

    clause.insert(MEASUREMENT_MASTER_MEASURE_id,obj->measurementMasterID);
    db->updateRecordsPlus(map,clause,MEASUREMENT_MASTER_tablename);
    clause.clear();

    clause.insert(MEASUREMENT_LABLES_Master_ID,obj->measurementMasterID);
    db->deleteRecordsPlus(clause,MEASUREMENT_LABLES_TABLENAME);

    for(int i=0;i<obj->lables.length();i++){
        map.clear();
        if(obj->lables[i].labelID.length()==0){
            obj->lables[i].labelID = db->getUUID();
        }
        map.insert(MEASUREMENT_LABLES_LABLE_ID,obj->lables[i].labelID);
        map.insert(MEASUREMENT_LABLES_Lable_Name,obj->lables[i].labelName);
        map.insert(MEASUREMENT_LABLES_Master_ID,obj->measurementMasterID);
        db->insertRecordsPlus(map,MEASUREMENT_LABLES_TABLENAME);
    }
    return commit();
}

int MeasurementsDatabaseHelper::deleteMeasurementMaster(MeasurementMaster *obj)
{
    startTransaction();
    QMap<QString,QVariant> clause;

    clause.insert(MEASUREMENT_MASTER_MEASURE_id,obj->measurementMasterID);
    db->deleteRecordsPlus(clause,MEASUREMENT_MASTER_tablename);

    clause.clear();
    clause.insert(MEASUREMENT_LABLES_Master_ID,obj->measurementMasterID);
    db->deleteRecordsPlus(clause,MEASUREMENT_LABLES_TABLENAME);
    return commit();
}

MeasurementMaster *MeasurementsDatabaseHelper::getMeasurementMasterByID(QString id)
{
    QString qStr1 =" SELECT ";
    qStr1 += MEASUREMENT_MASTER_MEASURE_id + ", ";
    qStr1 += MEASUREMENT_MASTER_NAME + " FROM ";
    qStr1 += MEASUREMENT_MASTER_tablename + " WHERE ";
    qStr1 += MEASUREMENT_MASTER_MEASURE_id + " = :id";


    QString qStr2 =" SELECT ";
    qStr2 += MEASUREMENT_LABLES_LABLE_ID + ", ";
    qStr2 += MEASUREMENT_LABLES_Lable_Name + " ";
    qStr2 +=  " FROM ";
    qStr2 += MEASUREMENT_LABLES_TABLENAME + " WHERE ";
    qStr2 += MEASUREMENT_LABLES_Master_ID + " = :id";


    QSqlQuery query1(QSqlDatabase::database(DatabaseValues::connectionString));
    QSqlQuery query2(QSqlDatabase::database(DatabaseValues::connectionString));
    MeasurementMaster *master  =new MeasurementMaster();
    query1.prepare(qStr1);
    query1.bindValue(":id",id);
    if(query1.exec()){
        while(query1.next()){
            master->measurementMasterID = query1.value(0).toString();
            master->measurementName = query1.value(1).toString();
        }
    }
    else{
        qDebug()<<qStr1;
        qDebug()<<query1.lastError();
    }

    query2.prepare(qStr2);
    query2.bindValue(":id",id);

    qDebug()<<qStr2<<id;
    if(query2.exec()){
        while(query2.next()){
            MeasurementLabel lbl ;//= new MeasurementLabel();
            lbl.labelID = query2.value(0).toString();
            lbl.labelName =  query2.value(1).toString();
            master->lables.append(lbl);
        }
    }
    else{
        qDebug()<<qStr2;
        qDebug()<<query2.lastError();
    }
    return master;
}

QSqlQueryModel *MeasurementsDatabaseHelper::getAllMeasurements()
{
    QString qStr = "SELECT ";
    qStr += MEASUREMENT_MASTER_MEASURE_id +", ";
    qStr += MEASUREMENT_MASTER_NAME;
    qStr += " FROM "+ MEASUREMENT_MASTER_tablename;
    return db->getQueryModel(qStr);
}

QList<MeasurementLabel *> MeasurementsDatabaseHelper::getLablesOfMeasurement(MeasurementMaster *obj)
{

}

int MeasurementsDatabaseHelper::insertContactMeasurement(ContactMeasurement *obj)
{
    startTransaction();
    QMap<QString,QVariant> map;
    if(obj->contactMeasureID.length()==0){
        obj->contactMeasureID = db->getUUID();
    }

    map.insert(CONTACT_MEASURE_Contact_Measurement_ID,obj->contactMeasureID);
    map.insert(CONTACT_MEASURE_Contact_ID,obj->contactID);
    map.insert(CONTACT_MEASURE_Measure_Master_ID,obj->measurement->measurementMasterID);
    map.insert(CONTACT_MEASURE_Reference,obj->reference);
    map.insert(CONTACT_MEASURE_CREATED_DATE,QDate::currentDate());
    map.insert(CONTACT_MEASURE_LAST_MODIFIED_ON,QDate::currentDate());



    db->insertRecordsPlus(map,CONTACT_MEASURE_MAIN_TABLENAME);

    for(int i=0;i<obj->lables.length();i++){
        map.clear();

        map.insert(CONTACT_MEASURE_Lable_ID,obj->lables[i].lable.labelID);
        map.insert(CONTACT_MEASURE_Lable_Name,obj->lables[i].lable.labelName);
        map.insert(CONTACT_MEASURE_VALUE,obj->lables[i].value);
        map.insert(CONTACT_MEASURE_Measurement_ID,obj->contactMeasureID);
        qDebug()<<"Inserting : "<<obj->lables[i].lable.labelName<<obj->lables[i].value;
        db->insertRecordsPlus(map,CONTACT_MEASURE_Details_TableName);
    }
    return commit();
}

int MeasurementsDatabaseHelper::updateContactMeasurement(ContactMeasurement *obj)
{
    startTransaction();
    QMap<QString,QVariant> map,clause;
    if(obj->contactMeasureID.length()==0){
        obj->contactMeasureID = db->getUUID();
    }

    map.insert(CONTACT_MEASURE_Contact_Measurement_ID,obj->contactMeasureID);
    map.insert(CONTACT_MEASURE_Contact_ID,obj->contactID);
    map.insert(CONTACT_MEASURE_Measure_Master_ID,obj->measurement->measurementMasterID);
    map.insert(CONTACT_MEASURE_Reference,obj->reference);
    map.insert(CONTACT_MEASURE_CREATED_DATE,QDate::currentDate());
    map.insert(CONTACT_MEASURE_LAST_MODIFIED_ON,QDate::currentDate());

    clause.insert(CONTACT_MEASURE_Contact_Measurement_ID,obj->contactMeasureID);

    db->updateRecordsPlus(map,clause,CONTACT_MEASURE_MAIN_TABLENAME);

    clause.clear();
    clause.insert(CONTACT_MEASURE_Measurement_ID,obj->contactMeasureID);
    db->deleteRecordsPlus(clause,CONTACT_MEASURE_Details_TableName);

    for(int i=0;i<obj->lables.length();i++){
        map.clear();

        map.insert(CONTACT_MEASURE_Lable_ID,obj->lables[i].lable.labelID);
        map.insert(CONTACT_MEASURE_Lable_Name,obj->lables[i].lable.labelName);
        map.insert(CONTACT_MEASURE_VALUE,obj->lables[i].value);
        map.insert(CONTACT_MEASURE_Measurement_ID,obj->contactMeasureID);
        qDebug()<<"Updating : "<<obj->lables[i].lable.labelName<<obj->lables[i].value;
        db->insertRecordsPlus(map,CONTACT_MEASURE_Details_TableName);
    }
    return commit();
}

int MeasurementsDatabaseHelper::deleteContactMeasurement(ContactMeasurement *obj)
{

}

ContactMeasurement *MeasurementsDatabaseHelper::getContactMeasurement(ContactMeasurement *cMeasurement)
{

    QString qStr = "SELECT ";

    // qStr += CONTACT_MEASURE_Contact_Measurement_ID;
    // qStr += ", " + CONTACT_MEASURE_Measure_Master_ID;
    // qStr += ", " + CONTACT_MEASURE_Reference;
    // qStr += ", " + CONTACT_MEASURE_Contact_ID;
    // qStr += ", " + MEASUREMENT_MASTER_NAME; // Add MEASUREMENT_MASTER_NAME
    // qStr += " FROM " + CONTACT_MEASURE_MAIN_TABLENAME;
    // qStr += " INNER JOIN " + MEASUREMENT_MASTER_tablename; // Join with measurement_master table
    // qStr += " ON " + CONTACT_MEASURE_MAIN_TABLENAME + "." + CONTACT_MEASURE_Measure_Master_ID;
    // qStr += " = " + MEASUREMENT_MASTER_tablename + "." + MEASUREMENT_MASTER_ID;



    qStr += CONTACT_MEASURE_Contact_Measurement_ID;
    qStr += ", " + CONTACT_MEASURE_Measure_Master_ID;
    qStr += ", " + CONTACT_MEASURE_Reference;
    qStr += ", " + CONTACT_MEASURE_Measure_Master_ID;
    qStr += ", ";
    qStr += "(SELECT ";
    qStr += "mm."+MEASUREMENT_MASTER_NAME + " FROM "+ MEASUREMENT_MASTER_tablename + " mm ";
    qStr += " WHERE mm."+MEASUREMENT_MASTER_MEASURE_id+"= cmm."+CONTACT_MEASURE_Measure_Master_ID +") As '";
    qStr += MEASUREMENT_MASTER_NAME +"' "; // Add MEASUREMENT_MASTER_NAME
    qStr += " FROM " + CONTACT_MEASURE_MAIN_TABLENAME + " cmm ";
    qStr += " WHERE " + CONTACT_MEASURE_Contact_Measurement_ID + " = :id";

    QSqlQuery query((QSqlDatabase::database(DatabaseValues::connectionString)));
    query.prepare(qStr);
    query.bindValue(":id", cMeasurement->contactMeasureID);
    qDebug()<<qStr<<cMeasurement->contactMeasureID;
    // QList<ContactMeasurement*> contactMeasurements; // List to hold all retrieved contact measurements
    ContactMeasurement* contactMeasurement = cMeasurement;

    if (query.exec()) {

        while (query.next()) {
            // Create a new instance of ContactMeasurement

            // Populate the ContactMeasurement fields using query results
            qDebug()<<Q_FUNC_INFO<<__LINE__;

            contactMeasurement->contactMeasureID = query.value(query.record().indexOf(CONTACT_MEASURE_Contact_Measurement_ID)).toString();
            qDebug()<<Q_FUNC_INFO<<__LINE__;
            contactMeasurement->reference = query.value(query.record().indexOf(CONTACT_MEASURE_Reference)).toString();
           // contactMeasurement->contact = new AddressBookDataModel();
           // contactMeasurement->contact ->addressId = query.value(query.record().indexOf(CONTACT_MEASURE_Contact_ID)).toString();
            // Create and populate MeasurementMaster instance
            qDebug()<<Q_FUNC_INFO<<__LINE__;
            MeasurementMaster* measurement = getMeasurementMasterByID(query.value(query.record().indexOf(CONTACT_MEASURE_Measure_Master_ID)).toString());
            // measurement->measurementMasterID = query.value(query.record().indexOf(MEASUREMENT_MASTER_ID)).toString();
            // measurement->measurementMasterID = query.value(query.record().indexOf(CONTACT_MEASURE_Measure_Master_ID)).toString();
            // measurement->measurementName = query.value(query.record().indexOf(MEASUREMENT_MASTER_NAME)).toString();

            contactMeasurement->measurement = measurement; // Associate MeasurementMaster with ContactMeasurement

            // Add the populated ContactMeasurement pointer to the list
            // contactMeasurements.append(contactMeasurement);
        }
    } else {
        qDebug() << query.lastError();
        qDebug() << qStr;
    }

    QString qStr1 = " SELECT ";

    qStr1 += CONTACT_MEASURE_Lable_ID +", ";
    qStr1 += "(select "+MEASUREMENT_LABLES_Lable_Name+" from "+ MEASUREMENT_LABLES_TABLENAME + " ml WHERE ";
    qStr1 += "ml."+MEASUREMENT_LABLES_LABLE_ID + " = cd."+ CONTACT_MEASURE_Lable_ID+" ) as '";

    qStr1 += CONTACT_MEASURE_Lable_Name +"' , ";
    qStr1 += CONTACT_MEASURE_VALUE +" FROM  ";
    qStr1 += CONTACT_MEASURE_Details_TableName;
    qStr1 += " cd WHERE ";
    qStr1 += CONTACT_MEASURE_Measurement_ID + "=:mID";

    qDebug()<<qStr1<<contactMeasurement->contactMeasureID;
    query.prepare(qStr1);
    query.bindValue(":mID",contactMeasurement->contactMeasureID);

    if(query.exec()){
        while(query.next()){
            ContactMeasurementLable lbl;
            lbl.lable.labelID = query.value(query.record().indexOf(CONTACT_MEASURE_Lable_ID)).toString();
            lbl.lable.labelName = query.value(query.record().indexOf(CONTACT_MEASURE_Lable_Name)).toString();
            lbl.value = query.value(query.record().indexOf(CONTACT_MEASURE_VALUE)).toString();
            contactMeasurement->lables.append(lbl);
            contactMeasurement->valueMap.insert(lbl.lable.labelID,lbl.value);
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<qStr1;
    }
    qDebug()<<"Length of Details : "<<contactMeasurement->lables.length();
    return contactMeasurement;
}

QSqlQueryModel *MeasurementsDatabaseHelper::getMeasurementReferencesByContact(QString contactID)
{
    QString qStr = "SELECT ";
    qStr += CONTACT_MEASURE_Contact_Measurement_ID;
    qStr += ", " + CONTACT_MEASURE_Measure_Master_ID;
    qStr += ", " + CONTACT_MEASURE_Reference;
    qStr += ", " ;
    qStr += "(SELECT ";
    qStr += "mm."+MEASUREMENT_MASTER_NAME + " FROM "+ MEASUREMENT_MASTER_tablename + " mm ";
    qStr += " WHERE mm."+MEASUREMENT_MASTER_MEASURE_id+"= cmm."+CONTACT_MEASURE_Measure_Master_ID +") As '";
    qStr += MEASUREMENT_MASTER_NAME +"' "; // Add MEASUREMENT_MASTER_NAME

    qStr += " FROM " + CONTACT_MEASURE_MAIN_TABLENAME + " cmm ";
    qStr += " WHERE " + CONTACT_MEASURE_Contact_ID + " = '"+contactID+"'";
    qDebug()<<"measu query "<<qStr;
    return DatabaseHelper::getQueryModel(qStr);


}

QList<ContactMeasurement *> MeasurementsDatabaseHelper::getAllMeasurementsByContact(QString contactID)
{

    QString qStr = "SELECT ";
    qStr += CONTACT_MEASURE_Contact_Measurement_ID;
    qStr += ", " + CONTACT_MEASURE_Measure_Master_ID;
    qStr += ", " + CONTACT_MEASURE_Reference;
    qStr += ", " + MEASUREMENT_MASTER_NAME; // Add MEASUREMENT_MASTER_NAME
    qStr += " FROM " + CONTACT_MEASURE_MAIN_TABLENAME;
    qStr += " INNER JOIN " + MEASUREMENT_MASTER_tablename; // Join with measurement_master table
    qStr += " ON " + CONTACT_MEASURE_MAIN_TABLENAME + "." + CONTACT_MEASURE_Measure_Master_ID;
    qStr += " = " + MEASUREMENT_MASTER_tablename + "." + MEASUREMENT_MASTER_ID;
    qStr += " WHERE " + CONTACT_MEASURE_Contact_ID + " = :contactID";

    qDebug()<<qStr;
    QSqlQuery query((QSqlDatabase::database(DatabaseValues::connectionString)));
    query.prepare(qStr);
    query.bindValue(":contactID", contactID);
    qDebug()<<qStr;
    QList<ContactMeasurement*> contactMeasurements; // List to hold all retrieved contact measurements

    if (query.exec()) {
        while (query.next()) {
            // Create a new instance of ContactMeasurement
            ContactMeasurement* contactMeasurement = new ContactMeasurement();

            // Populate the ContactMeasurement fields using query results
            contactMeasurement->_id = query.value(query.record().indexOf(CONTACT_MEASURE_Contact_Measurement_ID)).toInt();
            contactMeasurement->contactMeasureID = query.value(query.record().indexOf(CONTACT_MEASURE_Contact_Measurement_ID)).toString();
            contactMeasurement->reference = query.value(query.record().indexOf(CONTACT_MEASURE_Reference)).toString();

            // Create and populate MeasurementMaster instance
            MeasurementMaster* measurement = new MeasurementMaster();
            measurement->_id = query.value(query.record().indexOf(MEASUREMENT_MASTER_ID)).toInt();
            measurement->measurementMasterID = query.value(query.record().indexOf(MEASUREMENT_MASTER_ID)).toString();
            measurement->measurementName = query.value(query.record().indexOf(MEASUREMENT_MASTER_NAME)).toString();

            contactMeasurement->measurement = measurement; // Associate MeasurementMaster with ContactMeasurement

            // Add the populated ContactMeasurement pointer to the list
            contactMeasurements.append(contactMeasurement);
        }
    } else {
        qDebug() << query.lastError();
        qDebug() << qStr;
    }

    return contactMeasurements;

}

int MeasurementsDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

int MeasurementsDatabaseHelper::commit()
{
    return db->commitTransaction();
}
