#ifndef PAYSLIPDATAMODEL_H
#define PAYSLIPDATAMODEL_H

#include <QDateTime>
#include "userprofiledatamodel.h"
#include "attendancelistdatamodel.h"

class PayslipDataModel
{
public:
    PayslipDataModel();

    int id;
    QDate Period_StartDate;
    QDate Period_EndDate;
    QDate Date_Created;
    QDate Created_By;

    int numOfWorkingDays=0;

    QList<EmployeeSalaryDataModel *> empList;

    QString empId;
    QString voucherNo;
    QString voucherType;
    QString voucherPrefix;

};

#endif // PAYSLIPDATAMODEL_H
