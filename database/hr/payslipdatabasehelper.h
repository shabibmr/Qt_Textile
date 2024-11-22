#ifndef PAYSLIPDATABASEHELPER_H
#define PAYSLIPDATABASEHELPER_H

#include <QString>
#include "database/databasehelper/databasehelper.h"
#include "datamodels/hr/employeesalarydatamodel.h"
#include "datamodels/hr/payslipdatamodel.h"


class PayslipDatabaseHelper
{
public:
    PayslipDatabaseHelper();


    static QString Employee_Salary_Details_Tablename ;
    QString Employee_Salary_JoiningDate = "Joining_Date";
    QString Employee_Salary_Employee_ID = "Employee_ID";
    QString Employee_Salary_BasicSalary = "Basic_Salary";
    QString Employee_Salary_Conveyance = "Conveyance";
    QString Employee_Salary_HRA = "HRA";
    QString Employee_Salary_DA = "DA";
    QString Employee_Salary_MonthlyLeave = "Monthly_Leave";
    QString Employee_Salary_WeeklyLeave = "Weekly_Leave";
    QString Employee_Salary_AnnualLeave = "Annual_Leave";
    QString Employee_Salary_WorkingHours = "Working_Hours";
    QString Employee_Salary_OverTimePay = "OverTime_Pay";
    QString Employee_Salary_SalaryLedger = "Salary_Ledger";
    QString Employee_Salary_AdvSalaryLedger = "Adv_Salary_Ledger";
    QString Employee_Salary_isActive = "isActive";

    QString Payslip_Main_Tablename = "Payslip_Main";
    QString Payslip_Main_id = "_id";
    QString Payslip_Main_Period = "Period";
    QString Payslip_Main_StartDate = "Salary_Period_Start";
    QString Payslip_Main_EndDate = "Salary_Period_End";
    QString Payslip_Main_CreatedBy = "Created_By";
    QString Payslip_Main_CreatedOn = "Date_Created";
    QString Payslip_Main_EmpID = "Employee_ID";
    QString Payslip_Main_Voucher_No = "Voucher_No";
    QString Payslip_Main_Voucher_Type = "Voucher_Type";
    QString Payslip_Main_Voucher_Prefix = "Voucher_Prefix";


    QString Payslip_Details_Tablename = "Payslip_Details";
    QString Payslip_Details_PayslipNo = "Payslip_id";
    QString Payslip_Details_EmpID = "Employee_ID";
    QString Payslip_Details_BasicSalary = "Basic_Salary";
    QString Payslip_Details_Conveyance = "Conveyance";
    QString Payslip_Details_HRA = "HRA";
    QString Payslip_Details_DA = "DA";
    QString Payslip_Details_TotalWorkingDays = "Total_Working_Days";
    QString Payslip_Details_DaysWorked = "Num_Days_Worked";
    QString Payslip_Details_TotalLeave = "Total_Leave";
    QString Payslip_Details_UnpaidLeave = "Unpaid_Leave";
    QString Payslip_Details_SalaryDeducted = "Salary_Deducted";
    QString Payslip_Details_SalaryPerDay = "Salary_Per_Day";
    QString Payslip_Details_OvertimePay = "Overtime_Pay_Total";


    bool flag = true;
    DatabaseHelper *db;

    int startTransaction();

    int insertEmployeeSalary(EmployeeSalaryDataModel *obj);

    int updateEmployeeSalary(EmployeeSalaryDataModel *emp, QString oldID);

    int insertPayslip(EmployeeSalaryDataModel *emp);

    int deleteEmployeeSalary(QString oldID);

    int commit();


    EmployeeSalaryDataModel *getEmpSalaryDetailsAsPtr(QString empID);
    int insertPayslip(PayslipDataModel *payslip);
    int updatePayslip(PayslipDataModel *payslip);
    PayslipDataModel *getPayslipVoucherByEmployee(QString empId, QDate fromDate, QDate toDate);
};

#endif // PAYSLIPDATABASEHELPER_H
