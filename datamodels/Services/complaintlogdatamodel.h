#ifndef COMPLAINTLOGDATAMODEL_H
#define COMPLAINTLOGDATAMODEL_H

#include <QDate>
#include <QString>

class ComplaintLogDataModel
{
public:
    ComplaintLogDataModel();

    int ComplaintDetailsItemID =0;
    QDate timeStamp;
    QDate Date;
    int status =0;
    int addedByID =0;
    int technicianID=0;
    QString addedByName;
    QString technicianame;
    QString remarks;
};

#endif // COMPLAINTLOGDATAMODEL_H
