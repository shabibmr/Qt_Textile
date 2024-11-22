#ifndef USERPROFILEDATABASEHELPER_H
#define USERPROFILEDATABASEHELPER_H

#include "datamodels/hr/userprofiledatamodel.h"
#include "datamodels/hr/employeesalarydatamodel.h"
#include "database/databasehelper/databasehelper.h"
#include "database/hr/usergroupdatabasehelper.h"
#include <QSqlQueryModel>

#include <QMap>
#include <QDebug>

class UserProfileDatabaseHelper
{
public:
    UserProfileDatabaseHelper(DatabaseHelper *db = new DatabaseHelper());
    ~UserProfileDatabaseHelper();

    static QString Employee_Details_TableName ;

    QString Employee_Details_id = "_id";
    QString Employee_Details_TimeStamp = "TimeStamp";
    QString Employee_Details_Name = "Name";
    QString Employee_Details_Fathers_Name = "Fathers_Name";
    QString Employee_Details_Date_of_Birth = "Date_of_Birth";
    QString Employee_Details_Gender = "Gender";
    QString Employee_Details_Nationality = "Nationality";
    QString Employee_Details_Marital_Status = "Marital_Status";
    QString Employee_Details_Email = "Email";
    QString Employee_Details_Phone = "Phone";
    QString Employee_Details_Address = "Address";
    QString Employee_Details_Username = "UserName";
    QString Employee_Details_Password = "Password";
    QString Employee_Details_Employee_ID = "Employee_ID";
    QString Employee_Details_Designation = "Designation";
    QString Employee_Details_Department = "Department";
    QString Employee_Details_Date_of_Joining = "Date_of_Joining";
    QString Employee_Details_Passport_No = "Passport_No";
    QString Employee_Details_Passport_Expiry = "Passport_Expiry";
    QString Employee_Details_Visa_No = "Visa_No";
    QString Employee_Details_Visa_Expiry = "Visa_Expiry";
    QString Employee_Details_Bank_Account_No = "Bank_Account_No";
    QString Employee_Details_Bank_Name = "Bank_Name";
    QString Employee_Details_Bank_Branch = "Bank_Branch";
    QString Employee_Details_Bank_Code = "Bank_Code";
    QString Employee_Details_Highest_Qualification = "Highest_Qualification";
    QString Employee_Details_Date_of_Qualification = "Date_of_Qualification";
    QString Employee_Details_Emergency_Contact_Name = "Emergency_Contact_Name";
    QString Employee_Details_Emergency_Contact_No = "Emergency_Contact_No";
    QString Employee_Details_Emergency_Contact_Relation = "Emergency_Contact_Relation";
    QString Employee_Details_User_Group = "UserGroupID";
    QString Employee_Details_Show_Emp = "Show_Employee";
    QString Employee_Details_Privelage = "privilege";
    QString Employee_Details_Last_Modified ="Last_Modified";


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


    bool flag = true;
    DatabaseHelper *db;

    int startTransaction();

    int insertUserProfile(UserProfileDataModel *obj);

    int updateUserProfile(UserProfileDataModel *obj, int oldID);

    int deleteUserProfile(int oldID);

    int commit();

    int getMaxID();

    QList<QSharedPointer<UserProfileDataModel>> getAllUsersAsPtr();


    QSqlQueryModel *getEmployeeQueryForSearch(QString DepartmentID="");

    UserProfileDataModel getEmployeeByID(int id);

    QString getEmployeeNameFromID(int id);

    QMap<int,QString> getAllUsers(QString group="");

    QSqlQueryModel *visaExpiryList(int numDays);
    QSqlQueryModel *passportExpiryList(int numDays);


};

#endif // USERPROFILEDATABASEHELPER_H
