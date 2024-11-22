#include "attendancedatabasehelper.h"

AttendanceDatabaseHelper::AttendanceDatabaseHelper()
{
    qDebug()<<Q_FUNC_INFO;
    db = new DatabaseHelper();
}

int AttendanceDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

int AttendanceDatabaseHelper::insertAttendance(AttendanceListDataModel *objList)
{
    this->startTransaction();


    for(AttendanceDataModel *obj:objList->attendanceList){
        QMap<QString,QVariant> map;
        qDebug()<<obj->employee.EmployeeID;
        map.insert(Attendance_Details_EmpId, obj->employee.EmployeeID);
        map.insert(Attendance_Details_Date, objList->date);
        map.insert(Attendance_Details_StartTime, obj->StartTime);
        map.insert(Attendance_Details_EndTime, obj->EndTime);
        map.insert(Attendance_Details_WorkingHours, obj->WorkingHours);
        map.insert(Attendance_Details_Overtime, obj->Overtime);
        map.insert(Attendance_Details_Halfday, obj->Halfday);
        map.insert(Attendance_Details_Narration, obj->Narration);

        db->insertRecordsPlus(map,Attendance_Details_Tablename);
    }

    return commit();
}

int AttendanceDatabaseHelper::updateAttendance(AttendanceListDataModel *objList)
{
    this->startTransaction();

    for(AttendanceDataModel *obj:objList->attendanceList){
        qDebug()<<obj->Date<<obj->EndTime<<obj->StartTime<<obj->Halfday<<obj->Overtime<<obj->WorkingHours;

        QMap<QString,QVariant> map;
        QMap<QString, QVariant> clause;
        clause.insert(Attendance_Details_EmpId, obj->employee.EmployeeID);
        clause.insert(Attendance_Details_Date, obj->Date);
        map.insert(Attendance_Details_StartTime, obj->StartTime);
        map.insert(Attendance_Details_EndTime, obj->EndTime);
        map.insert(Attendance_Details_WorkingHours, obj->WorkingHours);
        map.insert(Attendance_Details_Overtime, obj->Overtime);
        qDebug()<<obj->Overtime<<"db overtime";
        map.insert(Attendance_Details_Halfday, obj->Halfday);
        map.insert(Attendance_Details_Narration, obj->Narration);

        if(obj->newEntry){
            qDebug()<<"insrting new entry";
            map.insert(Attendance_Details_EmpId, obj->employee.EmployeeID);
            map.insert(Attendance_Details_Date, objList->date);
            db->insertRecordsPlus(map,Attendance_Details_Tablename);
        }
        else{
            qDebug()<<" update entry";
            db->updateRecordsPlus(map,clause,Attendance_Details_Tablename);
            obj->newEntry = false;
        }
    }

    return commit();
}

int AttendanceDatabaseHelper::deleteAttendance(AttendanceListDataModel *objList)
{
    this->startTransaction();

    for(AttendanceDataModel *obj:objList->attendanceList){

        QMap<QString, QVariant> map2;
        map2.insert(Attendance_Details_EmpId, obj->employee.EmployeeID);
        map2.insert(Attendance_Details_Date, obj->Date);

        db->deleteRecordsPlus(map2,Attendance_Details_Tablename);
    }
    return commit();

}

int AttendanceDatabaseHelper::commit()
{
    if(flag){
        db->commitTransaction();
    }
    else{
        db->rollBackTransaction();
        return 0;
    }
}

QString AttendanceDatabaseHelper::getMaxID()
{

    QSqlDatabase db = QSqlDatabase::database(DatabaseValues::connectionString);
    QSqlQuery query(db);
    int id =0;
    QString qStr = "select max(_id) from "+ Attendance_Details_Tablename;
    query.exec(qStr);
    while(query.next()){
        id = query.value(0).toInt();
    }
    return QString::number(id+1);

}

QSqlQueryModel *AttendanceDatabaseHelper::getMonthlyPayslipReportModel(QDate startDate, QDate endDate)
{

    PayslipDatabaseHelper user;

    QString qStr = " select employee_id,  ( select name from   " +UserProfileDatabaseHelper::Employee_Details_TableName +"   where _id = X.employee_id) Employee, ";
    qStr += " worked_days as 'No of \n Days Worked', total_num_of_days as 'Total no of \n Working days',";
    qStr += " basic_salary as 'Basic Salary', hra 'HRA', da 'DA', conveyance 'Conveyance', ";
    qStr += " total_num_of_Days - worked_days as 'Total leave taken',  paid_leave 'Paid leave', ";
    //    qStr += " basic_salary/total_num_of_days as salary_per_day, ";
    qStr += " basic_salary/total_num_of_days*(total_num_of_Days - worked_days - paid_leave) as 'Salary \n Deducted' , ";
    qStr += " OverTime 'Overtime \n (hours)' ,OverTime_Pay 'Overtime Pay' , ";
    qStr += " basic_salary+ da+ hra + conveyance - (basic_salary/total_num_of_days*(total_num_of_Days - worked_days - paid_leave)) + OverTime_Pay as 'Total Salary'";
    qStr += " from (";
    qStr += " SELECT s." + Attendance_Details_EmpId + ", ";
    qStr += " sum(case when a." + Attendance_Details_WorkingHours + " >0 and " + Attendance_Details_Halfday + " =0 then 1 ";
    qStr += " when a." + Attendance_Details_WorkingHours + " >0 and " + Attendance_Details_Halfday + " = 1 then 0.5 else 0 end ) as worked_days , ";
    qStr += " case when E.date_of_joining  >= "+ db->getDateString(startDate);
    qStr += " then datediff("+ db->getDateString(endDate) + ", date_of_joining) else ";
    qStr += " datediff(" + db->getDateString(endDate) + ", " + db->getDateString(startDate) + ") end as total_num_of_days, ";
    qStr += " sum(a." + Attendance_Details_Overtime + ") as OverTime, sum(a.OverTime) * s." + user.Employee_Salary_OverTimePay + "  as OverTime_Pay, ";
    qStr += " s." + user.Employee_Salary_BasicSalary + ", s." + user.Employee_Salary_HRA + ", s." + user.Employee_Salary_DA + " ,";
    qStr += " s." + user.Employee_Salary_Conveyance + ", s." + user.Employee_Salary_MonthlyLeave + " as paid_leave  ";
    qStr +=  " FROM " + Attendance_Details_Tablename + " a JOIN " +UserProfileDatabaseHelper::Employee_Details_TableName +" E ON a.Employee_Id = E._id ";
    qStr += " join " + user.Employee_Salary_Details_Tablename + " s";
    qStr += " on a." + Attendance_Details_EmpId + " = s." + user.Employee_Salary_Employee_ID ;
    qStr += " WHERE " + Attendance_Details_Date + " >= " + db->getDateString(startDate);
    qStr += " and " + Attendance_Details_Date + " <= " + db->getDateString(endDate);
    qStr += " group by Employee_Id) X ";

    return db->getQueryModel(qStr);

}

QSqlQueryModel *AttendanceDatabaseHelper::getAttendanceReportModel(QDate startDate, QDate endDate)
{
    qDebug()<<Q_FUNC_INFO;
    QString qStr = " select employee_id as Employee, worked_days, ";
    qStr += " total_num_of_Days - worked_days as number_of_leaves_taken,  ";
    qStr += " OverTime from ";
    qStr += " ( SELECT a." + Attendance_Details_EmpId + ", E.Name as Employee, ";
    qStr += " sum(case when a." + Attendance_Details_WorkingHours + " >0 and " + Attendance_Details_Halfday + " =0 then 1 ";
    qStr += " when a." + Attendance_Details_WorkingHours + " >0 and " + Attendance_Details_Halfday + " = 1 then 0.5 else 0 end ) as worked_days , ";
    qStr += " case when E.date_of_joining  >= "+ db->getDateString(startDate);
    qStr += " then datediff("+ db->getDateString(endDate) + ", date_of_joining) else ";
    qStr += " datediff(" + db->getDateString(endDate) + ", " + db->getDateString(startDate) + ") end as total_num_of_days, ";
    qStr += " sum(a." + Attendance_Details_Overtime + ") as OverTime ";
    qStr += " FROM " + Attendance_Details_Tablename + " a JOIN " +UserProfileDatabaseHelper::Employee_Details_TableName +" E ON A.Employee_Id = E._id ";
    qStr += " WHERE " + Attendance_Details_Date + " >= " + db->getDateString(startDate);
    qStr += " and " + Attendance_Details_Date + " <= " + db->getDateString(endDate);
    qStr += " group by Employee_Id) X ";

    return db->getQueryModel(qStr);

}

AttendanceListDataModel *AttendanceDatabaseHelper::getEmployeeList()
{
    AttendanceListDataModel *attList = new AttendanceListDataModel();
    PayslipDatabaseHelper user;

    QString qStr = " SELECT " + user.Employee_Salary_Employee_ID + ", ";
    qStr += " (SELECT Name from   " +UserProfileDatabaseHelper::Employee_Details_TableName +"   where _id = e." + user.Employee_Salary_Employee_ID + ") , ";
    qStr += user.Employee_Salary_WorkingHours;
    qStr += " from " + user.Employee_Salary_Details_Tablename + " e ";
    qStr += " where " + user.Employee_Salary_isActive + " = 1 ";

    qDebug()<<qStr;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    if(query.exec(qStr)){
        while (query.next()) {
            AttendanceDataModel *obj = new AttendanceDataModel;
            EmployeeSalaryDataModel emp ;
            emp.EmployeeID = query.value(0).toString();
            emp.EmployeeName = query.value(1).toString();
            emp.WorkingHours = query.value(2).toInt();
            obj->employee = emp;
            obj->StartTime = QDateTime::currentDateTime();
            obj->EndTime = QDateTime::currentDateTime().addSecs(emp.WorkingHours*3600);
            obj->WorkingHours = emp.WorkingHours;
            attList->attendanceList.append(obj);
        }
    }
    else {
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }

    attList->date = QDate::currentDate();
    qDebug()<<attList->attendanceList.size();
    return attList;
}

AttendanceListDataModel *AttendanceDatabaseHelper::getAttendanceListByDate(QDate date)
{
    qDebug()<<Q_FUNC_INFO;
    AttendanceListDataModel *attList = new AttendanceListDataModel();
    PayslipDatabaseHelper user;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    bool data=true;

    QString qStr1 = " SELECT a." + Attendance_Details_id + ", ";
    qStr1 += Attendance_Details_Date + ", a." + Attendance_Details_EmpId + ", ";
    qStr1 += " emp.Name, ";
    qStr1 += " sal." + user.Employee_Salary_WorkingHours+ ", ";
    qStr1 += Attendance_Details_StartTime + ", " + Attendance_Details_EndTime + ", ";
    qStr1 += " a." + Attendance_Details_WorkingHours + ", a." + Attendance_Details_Overtime + ", ";
    qStr1 +=  " a." + Attendance_Details_Halfday + ", a." + Attendance_Details_Narration ;
    qStr1 += " from   " +UserProfileDatabaseHelper::Employee_Details_TableName +"   emp left outer join ";
    qStr1 +=  Attendance_Details_Tablename + " a ";
    qStr1 += " on emp._id = a.employee_id";
    qStr1 += " join " + PayslipDatabaseHelper::Employee_Salary_Details_Tablename + " sal on emp._id = sal.employee_id";
    qStr1 += " and " + Attendance_Details_Date + " = " + db->getDateString(date);

    qDebug()<<qStr1;

    query.prepare(qStr1);
    if(query.exec()){

        if(query.size()>0){
            while (query.next()) {
                AttendanceDataModel *obj = new AttendanceDataModel;
                EmployeeSalaryDataModel emp ;
                int i=0;
                obj->_id = query.value(i++).toInt();
                obj->Date = query.value(i++).toDate();
                emp.EmployeeID = query.value(i++).toString();
                emp.EmployeeName = query.value(i++).toString();
                emp.WorkingHours = query.value(i++).toInt();

                obj->employee = emp;

                obj->StartTime = query.value(i++).toDateTime();
                obj->EndTime = query.value(i++).toDateTime();
                obj->WorkingHours = query.value(i++).toInt();
                obj->Overtime = query.value(i++).toInt();
                obj->Halfday = query.value(i++).toBool();
                obj->Narration = query.value(i++).toString();
                obj->newEntry = false;
                attList->attendanceList.append(obj);
                attList->newEntry=false;

            }
        }
        //        else {
        //            data = false;
        //        }
    }
    else {
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }

    qDebug()<<"att list size"<<attList->attendanceList.size();


    //    if(!data){
    QString qStr = " SELECT " + user.Employee_Salary_Employee_ID + ", ";
    qStr += " (SELECT Name from   " +UserProfileDatabaseHelper::Employee_Details_TableName +"   where _id = e." + user.Employee_Salary_Employee_ID + ") , ";
    qStr += user.Employee_Salary_WorkingHours;
    qStr += " from " + user.Employee_Salary_Details_Tablename + " e ";
    qStr += " where " + user.Employee_Salary_isActive + " = 1 ";
    qStr += " and (select date_of_joining from " +UserProfileDatabaseHelper::Employee_Details_TableName +" where _id = e.employee_id) <= " + db->getDateString(date);
    qStr += " and " + user.Employee_Salary_Employee_ID + " not in ( ";
    qStr += " select " + Attendance_Details_EmpId + " FROM " + Attendance_Details_Tablename;
    qStr += " where " + Attendance_Details_Date + " = " + db->getDateString(date) + ")";

    qDebug()<<qStr;

    if(query.exec(qStr)){
        if(attList->attendanceList.size() == 0)
            attList->newEntry=true;
        while (query.next()) {
            AttendanceDataModel *obj = new AttendanceDataModel;
            EmployeeSalaryDataModel emp ;
            emp.EmployeeID = query.value(0).toString();
            emp.EmployeeName = query.value(1).toString();
            emp.WorkingHours = query.value(2).toInt();
            obj->employee = emp;
            obj->StartTime = QDateTime(date);
            obj->StartTime.setTime(QTime::currentTime());
            obj->EndTime = obj->StartTime.addSecs(emp.WorkingHours*3600);
            obj->WorkingHours = emp.WorkingHours;
            obj->newEntry=true;
            attList->attendanceList.append(obj);
        }
    }
    else {
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    //    }

    attList->date = date;
    qDebug()<<"att list size 111"<<attList->attendanceList.size();
    return attList;

}

PayslipDataModel *AttendanceDatabaseHelper::getMonthlyPayslipReport(QDate startDate, QDate endDate)
{

    PayslipDatabaseHelper user;
    PayslipDataModel *obj = new PayslipDataModel();
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    QString qStr = " select employee_id, ";
    qStr += "( select name from   " +UserProfileDatabaseHelper::Employee_Details_TableName +"   where _id = X.employee_id), ";
    qStr += " worked_days, total_num_of_days, basic_salary, hra, da, conveyance, paid_leave, ";
    qStr += " total_num_of_Days - worked_days as num_of_leaves_taken,  ";
    qStr += " basic_salary/total_num_of_days as salary_per_day, ";
    qStr += " basic_salary/total_num_of_days*(total_num_of_Days - worked_days - paid_leave) as salary_deducted , ";
    qStr += " OverTime,OverTime_Pay , ";
    qStr += " basic_salary+ da+ hra + conveyance - (basic_salary/total_num_of_days*(total_num_of_Days - worked_days - paid_leave)) + OverTime_Pay as total from (";
    qStr += " SELECT s." + Attendance_Details_EmpId + ", ";
    qStr += " sum(case when a." + Attendance_Details_WorkingHours + " >0 and " + Attendance_Details_Halfday + " =0 then 1 ";
    qStr += " when a." + Attendance_Details_WorkingHours + " >0 and " + Attendance_Details_Halfday + " = 1 then 0.5 else 0 end ) as worked_days , ";
    qStr += " day(last_day(" + db->getDateString(startDate) + ")) as total_num_of_days, ";
    qStr += " sum(a." + Attendance_Details_Overtime + ") as OverTime, sum(a.OverTime) * s." + user.Employee_Salary_OverTimePay + "  as OverTime_Pay, ";
    qStr += " s." + user.Employee_Salary_BasicSalary + ", s." + user.Employee_Salary_HRA + ", s." + user.Employee_Salary_DA + " ,";
    qStr += " s." + user.Employee_Salary_Conveyance + ", s." + user.Employee_Salary_MonthlyLeave + " as paid_leave  ";
    qStr +=  " FROM " + Attendance_Details_Tablename + " a";
    qStr += " join " + user.Employee_Salary_Details_Tablename + " s";
    qStr += " on a." + Attendance_Details_EmpId + " = s." + user.Employee_Salary_Employee_ID ;
    qStr += " RIGHT OUTER JOIN   " +UserProfileDatabaseHelper::Employee_Details_TableName +"   emp ON s.employee_id = emp._id ";
    qStr += " WHERE " + Attendance_Details_Date + " >= " + db->getDateString(startDate);
    qStr += " and " + Attendance_Details_Date + " <= " + db->getDateString(endDate);
    qStr += " group by Employee_Id) X ";

    qDebug()<<qStr;

    query.prepare(qStr);
    if(query.exec()){
        while(query.next()){
            int i=0;
            EmployeeSalaryDataModel *emp = new EmployeeSalaryDataModel();
            emp->EmployeeID = query.value(i++).toString();
            emp->EmployeeName = query.value(i++).toString();
            emp->NumOfDaysWorked = query.value(i++).toInt();
            emp->NumOfWorkingDays = query.value(i++).toInt();
            emp->BasicSalary = query.value(i++).toInt();
            emp->HRA = query.value(i++).toInt();
            emp->DA = query.value(i++).toInt();
            emp->Conveyance = query.value(i++).toInt();
            emp->PaidLeave = query.value(i++).toInt();
            emp->TotalLeave = query.value(i++).toInt();
            emp->SalaryPerDay = query.value(i++).toInt();
            emp->SalaryDeducted = query.value(i++).toInt();
            emp->overTimeHours = query.value(i++).toInt();
            emp->TotalOvertimePayment =query.value(i++).toInt();
            emp->totalSalary = query.value(i++).toInt();

            obj->empList.append(emp);
            obj->numOfWorkingDays = emp->NumOfWorkingDays;
        }
    }
    else {
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }

    return obj;
}

