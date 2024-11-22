#ifndef ATTENDANCEDATAMODEL_H
#define ATTENDANCEDATAMODEL_H

#include <QString>
#include <QDateTime>
#include <QMetaType>

#include "datamodels/hr/userprofiledatamodel.h"
#include "datamodels/hr/employeesalarydatamodel.h"

class AttendanceDataModel
{
public:
    AttendanceDataModel();
    ~AttendanceDataModel();

    int _id;

    EmployeeSalaryDataModel employee;
//    QString EmpId;
    QDate Date;
    QDateTime StartTime;
    QDateTime EndTime;
    int WorkingHours=0;
    int Overtime=0;
    bool Halfday=false;
    QString Narration;
    bool newEntry=false;

};
Q_DECLARE_METATYPE(AttendanceDataModel *)

#endif // ATTENDANCEDATAMODEL_H
