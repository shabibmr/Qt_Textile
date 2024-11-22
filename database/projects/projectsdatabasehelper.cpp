#include "projectsdatabasehelper.h"

ProjectsDatabaseHelper::ProjectsDatabaseHelper()
{
    db = new DatabaseHelper();
}

int ProjectsDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

int ProjectsDatabaseHelper::insertProject(ProjectsDataObject *obj)
{
    this->startTransaction();


    qDebug()<<"inserting data";

    QMap<QString,QVariant> map;

    map.insert(ProjectMaster_ProjectName,obj->ProjectName);
    map.insert(ProjectMaster_ProjectDescription,obj->ProjectDescription);
    map.insert(ProjectMaster_ownerID,obj->ownerID);
    map.insert(ProjectMaster_location,obj->location);
    map.insert(ProjectMaster_address,obj->address);
    map.insert(ProjectMaster_ProjectStatusID,obj->ProjectStatusID);
    map.insert(ProjectMaster_ProjectStartDate,obj->ProjectStartDate);
    map.insert(ProjectMaster_ProjectCompletionDate,obj->ProjectCompletionDate);
    map.insert(ProjectMaster_EstimatedProjectCompletionDate,obj->EstimatedProjectCompletionDate);
    map.insert(ProjectMaster_EstimatedValue,obj->EstimatedValue);
    map.insert(ProjectMaster_ProjectLedgerId,obj->ProjectLedgerId);
    map.insert(ProjectMaster_emailAddress,obj->emailAddress);
    map.insert(ProjectMaster_ContactPerson,obj->ContactPerson);
    map.insert(ProjectMaster_ContactPhone,obj->ContactPhone);
    map.insert(ProjectMaster_Detail,obj->Detail);

    db->insertRecordsPlus(map,ProjectMaster_TableName);
    return commit();
}

int ProjectsDatabaseHelper::updateProject(ProjectsDataObject *obj, QString oldID)
{
    this->startTransaction();
    QMap<QString,QVariant> map;

    map.insert(ProjectMaster_ProjectName,obj->ProjectName);
    map.insert(ProjectMaster_ProjectDescription,obj->ProjectDescription);
    map.insert(ProjectMaster_ownerID,obj->ownerID);
    map.insert(ProjectMaster_location,obj->location);
    map.insert(ProjectMaster_address,obj->address);
    map.insert(ProjectMaster_ProjectStatusID,obj->ProjectStatusID);
    map.insert(ProjectMaster_ProjectStartDate,obj->ProjectStartDate);
    map.insert(ProjectMaster_ProjectCompletionDate,obj->ProjectCompletionDate);
    map.insert(ProjectMaster_EstimatedProjectCompletionDate,obj->EstimatedProjectCompletionDate);
    map.insert(ProjectMaster_EstimatedValue,obj->EstimatedValue);
    map.insert(ProjectMaster_ProjectLedgerId,obj->ProjectLedgerId);
    map.insert(ProjectMaster_emailAddress,obj->emailAddress);
    map.insert(ProjectMaster_ContactPerson,obj->ContactPerson);
    map.insert(ProjectMaster_ContactPhone,obj->ContactPhone);
    map.insert(ProjectMaster_Detail,obj->Detail);

    QMap<QString,QVariant> map1;
    map1.insert(ProjectMaster_ProjectID,oldID);

    db->updateRecordsPlus(map,map1,ProjectMaster_TableName);


    return commit();
}

int ProjectsDatabaseHelper::deleteProject(QString oldID)
{
    this->startTransaction();
    QMap<QString,QVariant> map1;
    map1.insert(ProjectMaster_ProjectID,oldID);

    db->deleteRecordsPlus(map1,ProjectMaster_TableName);

    return commit();
}

int ProjectsDatabaseHelper::commit()
{
    if(flag){
        db->commitTransaction();
        return 1;
    }
    else{
        db->rollBackTransaction();
        return 0;
    }
}

int ProjectsDatabaseHelper::getMaxID()
{

}

QList<ProjectsDataObject> ProjectsDatabaseHelper::getAllProjects()
{
       QList<ProjectsDataObject> projList;

       return projList;
}
