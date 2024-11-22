#ifndef PROJECTSDATABASEHELPER_H
#define PROJECTSDATABASEHELPER_H
#include "database/databasehelper/databasehelper.h"
#include "datamodels/projects/projectsdataobject.h"

class ProjectsDatabaseHelper
{
public:
    ProjectsDatabaseHelper();

    bool flag = true;
    DatabaseHelper *db;

    QString ProjectMaster_TableName = "Project_Master";
    QString ProjectMaster_ProjectID = "_id";
    QString ProjectMaster_ProjectName = "Project_Name";
    QString ProjectMaster_ProjectDescription = "Project_Description";
    QString ProjectMaster_Detail = "Detail";
    QString ProjectMaster_EstimatedProjectStartDate = "Estimated_Start_Date";
    QString ProjectMaster_EstimatedProjectCompletionDate = "Estimated_End_Date";
    QString ProjectMaster_ProjectStartDate = "Start_Date";
    QString ProjectMaster_ProjectCompletionDate = "End_Date";
    QString ProjectMaster_ProjectLedgerId = "Ledger_ID";
    QString ProjectMaster_ContactPerson = "Contact_Person";
    QString ProjectMaster_ContactPhone = "Phone";
    QString ProjectMaster_EstimatedValue = "Estimated_Value";

    QString ProjectMaster_location = "Location";
    QString ProjectMaster_address ="Address";
    QString ProjectMaster_emailAddress ="Email";
    QString ProjectMaster_ownerID ="Project_Owner";
    QString ProjectMaster_ProjectStatusID ="Project_Status";

    QString ProjectMaster_closureProbablity ="";



    int startTransaction();

    int insertProject(ProjectsDataObject *obj);

    int updateProject(ProjectsDataObject *obj, QString oldID);

    int deleteProject(QString oldID);

    int commit();

    int getMaxID();


    QList<ProjectsDataObject> getAllProjects();

};

#endif // PROJECTSDATABASEHELPER_H
