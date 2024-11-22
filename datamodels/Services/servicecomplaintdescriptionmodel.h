#ifndef SERVICECOMPLAINTDESCRIPTIONMODEL_H
#define SERVICECOMPLAINTDESCRIPTIONMODEL_H

#include <QObject>
#include <QDate>
#include "complaintlogdatamodel.h"
class ServiceComplaintDescriptionModel
{
public:
    ServiceComplaintDescriptionModel();
    int _id =0;
    QString Voucher_id ;
    QString itemFrominventoryID;
    QString ItemNameDescripion;
    QString SerialNumber;
    QString ItemComplaintDetail;
    int ItemComplaintStatusid =0 ;
    int ItemNonCompletionReasonid =0;

    QString ItemNonCompletionRemarks;
    QString TechnicianRemarks ;

    int TechnicianFixedID ;
    int InspectedByID ;

    QDate FixedDate;
    bool isUnderWarranty ;

    QList<ComplaintLogDataModel> itemLog;

};

#endif // SERVICECOMPLAINTDESCRIPTIONMODEL_H
