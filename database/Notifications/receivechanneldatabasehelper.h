#ifndef RECEIVECHANNELDATABASEHELPER_H
#define RECEIVECHANNELDATABASEHELPER_H

#include <QMap>
#include <QDebug>
#include <QString>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include "database/databasehelper/databasehelper.h"
#include "datamodels/transaction/generalvoucherdataobject.h"
#include "datamodels/Notification/notificationdatamodel.h"
#include "datamodels/projects/Quotation/quotationstatus.h"
#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"


class receiveChannelDatabaseHelper
{
public:
    receiveChannelDatabaseHelper();

    QString Receive_Channel_TableName = "ReceiveChannel";
    QString Receive_Channel_id = "id";
    QString Receive_Channel_Data = "Notification_Data";
    QString Receive_Channel_FromDbName = "FromDBName";
    QString Receive_Channel_Timestamp = "Time_Stamp";
    QString Receive_Channel_TypeOfData = "TypeOfData";
    QString Receive_Channel_TransId = "Generated_TransactionID";
//    QString Receive_Channel_IsReceived = "IsReceived";
//    QString Receive_Channel_IsAccepted = "IsAccepted";
//    QString Receive_Channel_IsRejected = "IsRejected";
    QString Receive_Channel_Status = "Status";
    QString Receive_Channel_Action = "Action";
    QString Receive_Channel_NotificationId = "Notification_Id";

    int startTransaction();

    QSqlQueryModel *getUnreadNotifications();
    NotificationDataModel *getNotificationById(int id);
    QString getLedgerIdByDbName(QString dbName);

    int commit();


    DatabaseHelper *db;
    bool flag=true;
    void updateNotificationStatus(int id, QString GeneratedTransId, int status);
    QSqlQueryModel *getAllNotifications();
};

#endif // RECEIVECHANNELDATABASEHELPER_H
