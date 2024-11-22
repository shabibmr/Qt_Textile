#include "sendchanneldatabasehelper.h"

sendChannelDatabaseHelper::sendChannelDatabaseHelper()
{
    db = new DatabaseHelper();
}

int sendChannelDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

int sendChannelDatabaseHelper::insertNotification(NotificationDataModel *obj)
{
        QMap<QString, QVariant> map9;
        map9.insert(Send_Channel_Data, obj->NotificationData.replace("\"","\\\""));
        map9.insert(Send_Channel_ToDbName, obj->ToDBName);
        map9.insert(Send_Channel_TypeOfData, obj->TypeOfData);
        map9.insert(Send_Channel_TransId, obj->GeneratedTransactionId);
        map9.insert(Send_Channel_Action, obj->Action);
        map9.insert(Send_Channel_Timestamp, QDateTime::currentDateTime());
        map9.insert(Send_Channel_NotificationId, generateNotificationId());
        startTransaction();
        db->insertRecordsPlus(map9,Send_Channel_TableName);
        return commit();

}

int sendChannelDatabaseHelper::commit()
{
    if(flag){
       return db->commitTransaction();

    }
    else{
        db->rollBackTransaction();
        flag=true;
        return 0;
    }

}

QString sendChannelDatabaseHelper::generateNotificationId()
{
//    QString qStr = " SELECT count(*) from sendchannel ";
//    QString no = db->getSingletonQueryResult(qStr).toString();

//    return "0" + db->clientId + no;

    return db->getUUID();
}
