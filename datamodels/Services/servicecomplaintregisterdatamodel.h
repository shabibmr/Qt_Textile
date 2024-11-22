#ifndef SERVICECOMPLAINTREGISTERDATAMODEL_H
#define SERVICECOMPLAINTREGISTERDATAMODEL_H

#include <QString>

#include "servicecomplaintdescriptionmodel.h"


class ServiceComplaintRegisterDataModel
{
public:
    ServiceComplaintRegisterDataModel();

    int id=0;
    QString voucherNo;
    int addedByID=0;
    QString addedByName;

    QString location;
    QString Narration;

    bool isUnderWarranty;

    float inspectionCharge=0;

    int ComplaintStatus=0;
    QDate date;
    QDateTime TimeCreated;
    QList<ServiceComplaintDescriptionModel> Items;

};

#endif // SERVICECOMPLAINTREGISTERDATAMODEL_H
