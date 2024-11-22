#include "tablereservationdatamodel.h"

TableReservationDataModel::TableReservationDataModel(QObject *parent)
    : QObject{parent}
{
    addedDate = QDate::currentDate();
    resDate = QDate::currentDate();
    fromTime = QTime::fromString("12:00","hh:mm");
    toTime = fromTime.addSecs(30*60);
}
