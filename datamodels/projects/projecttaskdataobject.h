#ifndef PROJECTTASKDATAOBJECT_H
#define PROJECTTASKDATAOBJECT_H

#include <QString>
#include <QDate>

class ProjectTaskDataObject
{
public:
    ProjectTaskDataObject();
    ~ProjectTaskDataObject();

    QString TaskID;
    QString AssignedPersion;
    QString TaskDescription;
    QDate TaskAssignedDate;
    QDate LastEditedDate;
    QString completionStatus;

    QString ProjectID;

};

#endif // PROJECTTASKDATAOBJECT_H
