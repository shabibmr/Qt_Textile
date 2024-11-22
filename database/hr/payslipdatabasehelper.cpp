#include "payslipdatabasehelper.h"
#include "database/Settings/configsettingsvariables.h"

QString PayslipDatabaseHelper::Employee_Salary_Details_Tablename = "Employee_Salary_Details";

PayslipDatabaseHelper::PayslipDatabaseHelper()
{
    db = new DatabaseHelper();
}

int PayslipDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

int PayslipDatabaseHelper::insertEmployeeSalary(EmployeeSalaryDataModel *emp)
{
    qDebug()<<Q_FUNC_INFO;
    this->startTransaction();

    QMap<QString,QVariant> map2;
    map2.insert(Employee_Salary_Employee_ID, emp->EmployeeID );
    map2.insert(Employee_Salary_BasicSalary, emp->BasicSalary);
    map2.insert(Employee_Salary_DA, emp->DA);
    map2.insert(Employee_Salary_HRA, emp->HRA);
    map2.insert(Employee_Salary_Conveyance, emp->Conveyance);
    map2.insert(Employee_Salary_MonthlyLeave, emp->MonthlyLeave);
    map2.insert(Employee_Salary_WeeklyLeave, emp->WeeklyLeave);
    map2.insert(Employee_Salary_AnnualLeave, emp->AnnualLeave);
    map2.insert(Employee_Salary_WorkingHours, emp->WorkingHours);
    map2.insert(Employee_Salary_OverTimePay, emp->overTimePay);
    map2.insert(Employee_Salary_SalaryLedger, emp->salaryLedgerId);
    map2.insert(Employee_Salary_AdvSalaryLedger, emp->advLedgerId);
    map2.insert(Employee_Salary_isActive, emp->isActive);

    db->insertRecordsPlus(map2,Employee_Salary_Details_Tablename);

    return commit();
}

int PayslipDatabaseHelper::updateEmployeeSalary(EmployeeSalaryDataModel *emp, QString oldID)
{
    qDebug()<<Q_FUNC_INFO<<oldID;
    this->startTransaction();

    QMap<QString,QVariant> map2;
    map2.insert(Employee_Salary_BasicSalary, emp->BasicSalary);
    map2.insert(Employee_Salary_DA, emp->DA);
    map2.insert(Employee_Salary_HRA, emp->HRA);
    map2.insert(Employee_Salary_Conveyance, emp->Conveyance);
    map2.insert(Employee_Salary_MonthlyLeave, emp->MonthlyLeave);
    map2.insert(Employee_Salary_WeeklyLeave, emp->WeeklyLeave);
    map2.insert(Employee_Salary_AnnualLeave, emp->AnnualLeave);
    map2.insert(Employee_Salary_WorkingHours, emp->WorkingHours);
    map2.insert(Employee_Salary_OverTimePay, emp->overTimePay);
    map2.insert(Employee_Salary_SalaryLedger, emp->salaryLedgerId);
    map2.insert(Employee_Salary_AdvSalaryLedger, emp->advLedgerId);
    map2.insert(Employee_Salary_isActive, emp->isActive);

    QMap<QString,QVariant> map3;
    map3.insert(Employee_Salary_Employee_ID, emp->EmployeeID );
    db->updateRecordsPlus(map2,map3,Employee_Salary_Details_Tablename);

    return commit();
}

int PayslipDatabaseHelper::insertPayslip(PayslipDataModel *payslip)
{
    this->startTransaction();

    QMap<QString,QVariant> map1;
    map1.insert(Payslip_Main_StartDate, payslip->Period_StartDate);
    map1.insert(Payslip_Main_EndDate, payslip->Period_EndDate);
    map1.insert(Payslip_Main_CreatedBy, payslip->Created_By);
    map1.insert(Payslip_Main_CreatedOn, payslip->Date_Created);
    map1.insert(Payslip_Main_EmpID, payslip->empId);
    map1.insert(Payslip_Main_Voucher_No, payslip->voucherNo);
    map1.insert(Payslip_Main_Voucher_Type, payslip->voucherType);
    map1.insert(Payslip_Main_Voucher_Prefix, payslip->voucherPrefix);

    payslip->id = db->insertRecordsPlus(map1,Payslip_Main_Tablename);

//    for(EmployeeSalaryDataModel *emp:payslip->empList){
//        QMap<QString,QVariant> map2;
//        map2.insert(Payslip_Details_PayslipNo, payslip->id);
//        map2.insert(Payslip_Details_EmpID, emp->EmployeeID);
//        map2.insert(Payslip_Details_BasicSalary, emp->BasicSalary);
//        map2.insert(Payslip_Details_DA, emp->DA);
//        map2.insert(Payslip_Details_HRA, emp->HRA);
//        map2.insert(Payslip_Details_Conveyance, emp->Conveyance);
//        map2.insert(Payslip_Details_DaysWorked, emp->NumOfDaysWorked);
//        map2.insert(Payslip_Details_TotalWorkingDays, emp->NumOfWorkingDays);
//        map2.insert(Payslip_Details_TotalLeave, emp->TotalLeave);
//        map2.insert(Payslip_Details_UnpaidLeave, emp->UnpaidLeave);
//        map2.insert(Payslip_Details_SalaryDeducted, emp->SalaryDeducted);
//        map2.insert(Payslip_Details_SalaryPerDay, emp->SalaryPerDay);
//        map2.insert(Payslip_Details_OvertimePay, emp->TotalOvertimePayment);

//        db->insertRecordsPlus(map2,Payslip_Details_Tablename);

//    }

    return commit();
}

int PayslipDatabaseHelper::updatePayslip(PayslipDataModel *payslip)
{
    this->startTransaction();

    QMap<QString,QVariant> map1;
    map1.insert(Payslip_Main_StartDate, payslip->Period_StartDate);
    map1.insert(Payslip_Main_EndDate, payslip->Period_EndDate);
    map1.insert(Payslip_Main_CreatedBy, payslip->Created_By);
    map1.insert(Payslip_Main_CreatedOn, payslip->Date_Created);

    QMap<QString,QVariant> map3;
    map3.insert(Payslip_Main_id, payslip->id);

    payslip->id = db->updateRecordsPlus(map1,map3,Payslip_Main_Tablename);

    QMap<QString,QVariant> map4;
    map4.insert(Payslip_Details_PayslipNo, payslip->id);
    db->deleteRecordsPlus(map4,Payslip_Details_Tablename);

    for(EmployeeSalaryDataModel *emp:payslip->empList){
        QMap<QString,QVariant> map2;
        map2.insert(Payslip_Details_PayslipNo, payslip->id);
        map2.insert(Payslip_Details_EmpID, emp->EmployeeID);
        map2.insert(Payslip_Details_BasicSalary, emp->BasicSalary);
        map2.insert(Payslip_Details_DA, emp->DA);
        map2.insert(Payslip_Details_HRA, emp->HRA);
        map2.insert(Payslip_Details_Conveyance, emp->Conveyance);
        map2.insert(Payslip_Details_DaysWorked, emp->NumOfDaysWorked);
        map2.insert(Payslip_Details_TotalWorkingDays, emp->NumOfWorkingDays);
        map2.insert(Payslip_Details_TotalLeave, emp->TotalLeave);
        map2.insert(Payslip_Details_UnpaidLeave, emp->UnpaidLeave);
        map2.insert(Payslip_Details_SalaryDeducted, emp->SalaryDeducted);
        map2.insert(Payslip_Details_SalaryPerDay, emp->SalaryPerDay);
        map2.insert(Payslip_Details_OvertimePay, emp->TotalOvertimePayment);

        db->insertRecordsPlus(map2,Payslip_Details_Tablename);

    }

    return commit();
}

int PayslipDatabaseHelper::deleteEmployeeSalary(QString oldID)
{
    this->startTransaction();

    QMap<QString,QVariant> map3;
    map3.insert(Employee_Salary_Employee_ID, oldID );
    db->deleteRecordsPlus(map3,Employee_Salary_Details_Tablename);

    return commit();
}

int PayslipDatabaseHelper::commit()
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

EmployeeSalaryDataModel *PayslipDatabaseHelper::getEmpSalaryDetailsAsPtr(QString empID)
{
    EmployeeSalaryDataModel* obj= new EmployeeSalaryDataModel();
    obj->EmployeeID = empID;

    QString qStr = "SELECT ";
    qStr += Employee_Salary_Employee_ID + ", ";
    qStr += Employee_Salary_BasicSalary + ", ";
    qStr += Employee_Salary_DA + ", " + Employee_Salary_HRA + ", "+ Employee_Salary_Conveyance + ", ";
    qStr += Employee_Salary_AnnualLeave + ", " + Employee_Salary_WeeklyLeave + ", " + Employee_Salary_MonthlyLeave + ", ";
    qStr += Employee_Salary_WorkingHours + ", " + Employee_Salary_OverTimePay + ", ";
    qStr += Employee_Salary_SalaryLedger + ", " + Employee_Salary_AdvSalaryLedger;
    qStr += " FROM "+  Employee_Salary_Details_Tablename + " s ";
    qStr += " WHERE " + Employee_Salary_Employee_ID + " = "  + empID;


    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    if(query.exec(qStr)){
        if(query.first()){
            int i=1;
            qDebug()<<qStr;
            obj->BasicSalary = query.value(i++).toFloat();
            obj->DA = query.value(i++).toFloat();
            obj->HRA = query.value(i++).toFloat();
            obj->Conveyance = query.value(i++).toFloat();
            obj->AnnualLeave = query.value(i++).toInt();
            obj->WeeklyLeave = query.value(i++).toInt();
            obj->MonthlyLeave = query.value(i++).toInt();
            obj->WorkingHours = query.value(i++).toInt();
            obj->overTimePay = query.value(i++).toInt();
            obj->salaryLedgerId = query.value(i++).toString();
            obj->advLedgerId = query.value(i++).toString();

        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<qStr;
    }
    return obj;
}

PayslipDataModel *PayslipDatabaseHelper::getPayslipVoucherByEmployee(QString empId, QDate fromDate, QDate toDate)
{
    PayslipDataModel *obj = new PayslipDataModel;
    QString qStr = " SELECT " + Payslip_Main_id + ", " + Payslip_Main_StartDate + ", " + Payslip_Main_EndDate;
    qStr += ", "+ Payslip_Main_EmpID + ", " + Payslip_Main_Voucher_No + ", ";
    qStr += Payslip_Main_Voucher_Type + ", "+ Payslip_Main_Voucher_Prefix;
    qStr += " FROM " + Payslip_Main_Tablename + " Where " +
            Payslip_Main_StartDate + " = " + db->getDateString(fromDate);
    qStr += " AND " + Payslip_Main_EndDate + " = " + db->getDateString(toDate);
    qStr += " AND " + Payslip_Main_EmpID + " = '" + empId + "'";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    if(query.exec(qStr)){
        if(query.first()){
            int i=0;
               obj->id = query.value(i++).toInt();
               obj->Period_StartDate = query.value(i++).toDate();
               obj->Period_EndDate = query.value(i++).toDate();
               obj->empId = query.value(i++).toString();
               obj->voucherNo = query.value(i++).toString();
               obj->voucherType = query.value(i++).toString();
               obj->voucherPrefix = query.value(i++).toString();
        }
    }
    return obj;
}
