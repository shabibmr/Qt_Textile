#ifndef SENDCHANNELDATABASEHELPER_H
#define SENDCHANNELDATABASEHELPER_H

#include <QMap>
#include <QDebug>
#include <QString>
#include "database/databasehelper/databasehelper.h"
#include "datamodels/transaction/generalvoucherdataobject.h"
#include "datamodels/Notification/notificationdatamodel.h"

class sendChannelDatabaseHelper
{
public:
    sendChannelDatabaseHelper();

    QString Send_Channel_TableName = "SendChannel";
    QString Send_Channel_id = "id";
    QString Send_Channel_Data = "Notification_Data";
    QString Send_Channel_ToDbName = "ToDBName";
    QString Send_Channel_Timestamp = "Time_Stamp";
    QString Send_Channel_TypeOfData = "TypeOfData";
    QString Send_Channel_TransId = "Generated_TransactionID";
    QString Send_Channel_Action = "Action";
    QString Send_Channel_NotificationId = "Notification_Id";

    int startTransaction();

    int insertNotification(NotificationDataModel *obj);

    int commit();

    bool flag = true;
    DatabaseHelper *db;

    QString generateNotificationId();
};

#endif // SENDCHANNELDATABASEHELPER_H
