#ifndef ATTENDANCELISTDATAMODEL_H
#define ATTENDANCELISTDATAMODEL_H

#include "datamodels/hr/attendancedatamodel.h"

class AttendanceListDataModel
{
public:
    AttendanceListDataModel();

    QList<AttendanceDataModel *> attendanceList;
    QDate date;
    bool newEntry=false;
};

#endif // ATTENDANCELISTDATAMODEL_H
