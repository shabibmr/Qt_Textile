#ifndef EMPLOYEESALARYDATAMODEL_H
#define EMPLOYEESALARYDATAMODEL_H

#include "userprofiledatamodel.h"

class EmployeeSalaryDataModel
{
public:
    EmployeeSalaryDataModel();

    QString EmployeeID;
    QString EmployeeName;

    UserProfileDataModel *emp;

    float BasicSalary=0;
    float HRA=0;
    float DA=0;
    float Conveyance=0;
    int MonthlyLeave=0;
    int WeeklyLeave=0;
    int AnnualLeave=0;
    int WorkingHours=0;
    int overTimePay=0;
    int overTimeHours=0;
    bool isActive=true;

    QString salaryLedgerId;
    QString advLedgerId;

    int NumOfWorkingDays=0;
    int NumOfDaysWorked=0;
    int PaidLeave=0;
    int TotalLeave=0;
    int UnpaidLeave=0;
    float SalaryPerDay=0;
    float SalaryDeducted=0;
    float TotalOvertimePayment=0;
    float totalSalary=0;
//    QString Period;
//    QDate startDate;
//    QDate endDate;
//    QString CreatedBy;
//    QDate DateCreated;

};

#endif // EMPLOYEESALARYDATAMODEL_H
