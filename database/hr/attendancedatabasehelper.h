#ifndef ATTENDANCEDATABASEHELPER_H
#define ATTENDANCEDATABASEHELPER_H

#include "database/databasehelper/databasehelper.h"
#include "datamodels/hr/attendancelistdatamodel.h"
#include "database/hr/userprofiledatabasehelper.h"
#include "database/hr/payslipdatabasehelper.h"
#include <QSqlQueryModel>

class AttendanceDatabaseHelper
{
public:
    AttendanceDatabaseHelper();
    ~AttendanceDatabaseHelper();

    QString Attendance_Details_Tablename = "Attendance_Details";

    QString Attendance_Details_id = "_id";
    QString Attendance_Details_EmpId = "Employee_Id";
    QString Attendance_Details_Date = "Working_Day";
    QString Attendance_Details_StartTime = "Start_Time";
    QString Attendance_Details_EndTime = "End_Time";
    QString Attendance_Details_WorkingHours = "Working_Hours";
    QString Attendance_Details_Overtime = "Overtime";
    QString Attendance_Details_Halfday = "HalfDay";
    QString Attendance_Details_Narration = "Narration";

    bool flag = true;

    DatabaseHelper *db;

    int startTransaction();

    int insertAttendance(AttendanceListDataModel *obj);

    int updateAttendance(AttendanceListDataModel *obj);

    int deleteAttendance(AttendanceListDataModel *obj);

    int commit();

    QString getMaxID();

    QSqlQueryModel *getMonthlyPayslipReportModel(QDate startDate, QDate endDate);

    QSqlQueryModel *getAttendanceReportModel(QDate startDate, QDate endDate);

    AttendanceListDataModel *getEmployeeList();

    AttendanceListDataModel *getAttendanceListByDate(QDate date);

    PayslipDataModel *getMonthlyPayslipReport(QDate startDate, QDate endDate);
};

#endif // ATTENDANCEDATABASEHELPER_H
