#include "receivechanneldatabasehelper.h"

receiveChannelDatabaseHelper::receiveChannelDatabaseHelper()
{
    db = new DatabaseHelper();

}

QSqlQueryModel *receiveChannelDatabaseHelper::getUnreadNotifications()
{
    LedgerMasterDatabaseHelper led;

    QString qStr = " SELECT ";
    qStr += "max(" + Receive_Channel_id + "), ";
    qStr += " (SELECT distinct Ledger_Name FROM " + led.Ledger_Master_TableName + " where DbName = " + Receive_Channel_FromDbName + ") as 'From Ledger',";
    qStr += Receive_Channel_TypeOfData + " as Type, ";
    qStr += "max(" + Receive_Channel_Timestamp + ") as 'Received Time', ";
    qStr += " (case when min(" + Receive_Channel_Action + ") = 1001 then 'NEW' ";
    qStr += "when min("+ Receive_Channel_Action + ") = 1002 then 'UPDATE' ";
    qStr += "when min("+ Receive_Channel_Action + ") = 1003 then 'DELETE' end) as Action, ";

    qStr += " case when " + Receive_Channel_TypeOfData + " = 'Voucher' then ";
    qStr += " json_extract(" + Receive_Channel_Data + ", '$.Voucher_Type') ";
    qStr += " when " + Receive_Channel_TypeOfData + " = 'Inventory Item' then ";
    qStr += " json_extract(" + Receive_Channel_Data + ", '$.BaseItem.Item_Name')";
    qStr += " when " + Receive_Channel_TypeOfData + " = 'Inventory Group' then ";
    qStr += " json_extract(" + Receive_Channel_Data + ", '$.GroupName')";
    qStr += " when " + Receive_Channel_TypeOfData + " = 'Brand' then ";
    qStr += " json_extract(" + Receive_Channel_Data + ", '$.Brand_Name')";
    qStr += " when " + Receive_Channel_TypeOfData + " = 'UOM' then ";
    qStr += " json_extract(" + Receive_Channel_Data + ", '$.UomName')";
    qStr += " when " + Receive_Channel_TypeOfData + " = 'Acknowledgement' then ";
    qStr += " concat( json_extract(" + Receive_Channel_Data + ", '$.Status')) ";
    qStr += " end as Description, ";
    qStr += Receive_Channel_TransId + " as 'Mark as Read' ";
    qStr += " FROM " + Receive_Channel_TableName + " WHERE " + Receive_Channel_Status + " = 0";
    qStr += " AND " + Receive_Channel_TypeOfData + " not like '%Acknowledgement%' ";
    qStr += " GROUP BY " + Receive_Channel_TransId + ", " + Receive_Channel_TypeOfData;

//    qDebug()<<qStr;

    QSqlQueryModel *model =  DatabaseHelper::getQueryModel(qStr, DatabaseValues::connectionString);

//    qDebug()<<" in db Model has"<<model->rowCount()<<" rows . _id = "<<model->record(0).value(0).toInt();
    return model;
}

QSqlQueryModel *receiveChannelDatabaseHelper::getAllNotifications()
{
    LedgerMasterDatabaseHelper led;

    QString qStr = " SELECT ";
    qStr += "max(" + Receive_Channel_id + "), ";
    qStr += " (SELECT distinct Ledger_Name FROM " + led.Ledger_Master_TableName + " where DbName = " + Receive_Channel_FromDbName + ") as 'From Ledger',";
    qStr += Receive_Channel_TypeOfData + " as Type, ";
    qStr += "max(" + Receive_Channel_Timestamp + ") as 'Received Time', ";
    qStr += " (case when min(" + Receive_Channel_Action + ") = 1001 then 'NEW' ";
    qStr += "when min("+ Receive_Channel_Action + ") = 1002 then 'UPDATE' ";
    qStr += "when min("+ Receive_Channel_Action + ") = 1003 then 'DELETE' end) as Action, ";

    qStr += " case when " + Receive_Channel_TypeOfData + " = 'Voucher' then ";
    qStr += " json_extract(" + Receive_Channel_Data + ", '$.Voucher_Type') ";
    qStr += " when " + Receive_Channel_TypeOfData + " = 'Inventory Item' then ";
    qStr += " json_extract(" + Receive_Channel_Data + ", '$.BaseItem.Item_Name')";
    qStr += " when " + Receive_Channel_TypeOfData + " = 'Inventory Group' then ";
    qStr += " json_extract(" + Receive_Channel_Data + ", '$.GroupName')";
    qStr += " when " + Receive_Channel_TypeOfData + " = 'Brand' then ";
    qStr += " json_extract(" + Receive_Channel_Data + ", '$.Brand_Name')";
    qStr += " when " + Receive_Channel_TypeOfData + " = 'UOM' then ";
    qStr += " json_extract(" + Receive_Channel_Data + ", '$.UomName')";
    qStr += " when " + Receive_Channel_TypeOfData + " = 'Acknowledgement' then ";
    qStr += " concat( json_extract(" + Receive_Channel_Data + ", '$.Status')) ";
    qStr += " end as Description, ";
    qStr += Receive_Channel_Status + " as Status, ";
    qStr += Receive_Channel_TransId + " as 'Mark as UnRead' ";
    qStr += " FROM " + Receive_Channel_TableName + " WHERE " ;
    qStr +=  Receive_Channel_TypeOfData + " not like '%Acknowledgement%' ";
    qStr += " GROUP BY " + Receive_Channel_TransId + ", " + Receive_Channel_TypeOfData;
    qStr += " ORDER BY 4 desc ";

//    qDebug()<<qStr;

    QSqlQueryModel *model =  DatabaseHelper::getQueryModel(qStr, DatabaseValues::connectionString);

//    qDebug()<<" in db Model has"<<model->rowCount()<<" rows . _id = "<<model->record(0).value(0).toInt();
    return model;
}


NotificationDataModel *receiveChannelDatabaseHelper::getNotificationById(int id)
{
    NotificationDataModel *obj = new NotificationDataModel();
    int i=0;

    QString qStr = "SELECT ";
    qStr += Receive_Channel_id +", ";
    qStr += Receive_Channel_Data +", ";
    qStr += Receive_Channel_TransId + ", ";
    qStr += Receive_Channel_Timestamp + ", ";
    qStr += Receive_Channel_FromDbName + ", ";
    qStr += Receive_Channel_TypeOfData + ", ";
    qStr += " (select min(" + Receive_Channel_Action + ") from " + Receive_Channel_TableName + " r2 where r2." + Receive_Channel_TransId +
            "= r1." + Receive_Channel_TransId + " and r2." + Receive_Channel_Status + " = 0)  , ";
    qStr += Receive_Channel_Status ;
    qStr += " FROM " + Receive_Channel_TableName + " r1 WHERE " + Receive_Channel_id + " = '" + QString::number(id) + "'";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);
    qDebug()<<qStr<<"get notification data";

    if(query.exec()){
        if(query.first()){
            obj->id = query.value(i++).toInt();
            obj->NotificationData = query.value(i++).toString().replace("\\\"","\"");
            obj->GeneratedTransactionId = query.value(i++).toString();
            obj->timeStamp = query.value(i++).toDateTime();
            obj->FromDBName = query.value(i++).toString();
            obj->TypeOfData = query.value(i++).toString();
            obj->Action = query.value(i++).toInt();
            if(query.value(i).toInt() == QuotationStatus::NotificationAccepted)
                obj->isAccepted  = true;
            else if(query.value(i).toInt() == QuotationStatus::NotificationRejected)
                obj->isRejected = true;
        }
    }

    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }

    return obj;

}

QString receiveChannelDatabaseHelper::getLedgerIdByDbName(QString dbName)
{
    QString ledgerId="";
    LedgerMasterDatabaseHelper led;

    QString qStr = " SELECT Ledger_Id FROM " + led.Ledger_Master_TableName + " where DbName = '" + dbName + "'";
//    qDebug()<<qStr;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);

    if(query.exec()){
        if(query.first()){
            ledgerId = query.value(0).toString();
        }
    }
    else {
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }

    qDebug()<<ledgerId;
    return ledgerId;
}

void receiveChannelDatabaseHelper::updateNotificationStatus(int id, QString GeneratedTransId, int status)
{

    qDebug()<<Q_FUNC_INFO<<__LINE__;

    startTransaction();

    QMap<QString, QVariant> map9;
    QMap<QString, QVariant> map10;
    QString qStr = " SELECT " + Receive_Channel_NotificationId + " FROM " + Receive_Channel_TableName ;
    qStr += " WHERE " + Receive_Channel_TransId + " = '" + GeneratedTransId + "' ";
    qStr += " AND " + Receive_Channel_id + " <= " + QString::number(id);

    qDebug()<<Q_FUNC_INFO<<__LINE__<<qStr;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);

    if(query.exec()){
        while(query.next()){
            qDebug()<<Q_FUNC_INFO<<__LINE__<<"update id "<<query.value(0).toString();
            map10.insert(Receive_Channel_NotificationId, query.value(0).toString());
            map9.insert(Receive_Channel_Status, status);

            db->updateRecordsPlus(map9,map10, Receive_Channel_TableName);
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }


    commit();

}

int receiveChannelDatabaseHelper::startTransaction()
{
    db->startTransaction();
}


int receiveChannelDatabaseHelper::commit()
{
    if(flag){
        db->commitTransaction();
        return 1;
    }
    else{
        db->rollBackTransaction();
        flag=true;
        return 0;
    }

}
