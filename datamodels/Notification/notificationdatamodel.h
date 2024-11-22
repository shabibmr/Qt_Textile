#ifndef NOTIFICATIONDATAMODEL_H
#define NOTIFICATIONDATAMODEL_H

#include <QJsonObject>
#include <QDateTime>

class NotificationDataModel
{
public:
    NotificationDataModel();

    int id;
    QString NotificationData;
    QJsonObject notificationJson;
    QString ToDBName;
    QString FromDBName;
    QDateTime timeStamp;
    QString TypeOfData;
    QString GeneratedTransactionId;
    int Action;
    bool isReceived=false;
    bool isAccepted=false;
    bool isRejected=false;

    QJsonObject getAcknowledgementMessage(int status);

};

#endif // NOTIFICATIONDATAMODEL_H
