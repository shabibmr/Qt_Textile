#ifndef PROJECTSDATAOBJECT_H
#define PROJECTSDATAOBJECT_H

#include <QString>
#include <QDate>

class ProjectsDataObject
{
public:
    ProjectsDataObject();
    ~ProjectsDataObject();

    QString ProjectID;
    QString ProjectName;
    QString ProjectDescription;
    QDate EstimatedProjectStartDate;
    QDate EstimatedProjectCompletionDate;
    QDate ProjectStartDate;
    QDate ProjectCompletionDate;
    QString ProjectLedgerId;
    QString ContactPerson;
    QString ContactPhone;
    float EstimatedValue =0;
    float closureProbablity =0;
    QString location;
    QString address;
    QString emailAddress;
    QString ownerID;
    QString ProjectStatusID;
    QString Detail;


};

#endif // PROJECTSDATAOBJECT_H
