#ifndef TABLERESERVATIONDATAMODEL_H
#define TABLERESERVATIONDATAMODEL_H

#include <QObject>
#include "datamodels/addressbook/addressbookdatamodel.h"
#include "tablemasterdatamodel.h"

class TableReservationDataModel : public QObject
{
    Q_OBJECT
public:
    explicit TableReservationDataModel(QObject *parent = nullptr);

    int _id=-1;
    QString reservationID;
    QString contactID;

    int pax;

    QDate addedDate;
    QDate resDate;

    QTime fromTime;
    QTime toTime;

    QString narration;

    TableMasterDataModel *table;

    QList<TableMasterDataModel*> combinedTables;

    bool isWaitingList;

    bool sendEmail;


signals:
};

#endif // TABLERESERVATIONDATAMODEL_H
