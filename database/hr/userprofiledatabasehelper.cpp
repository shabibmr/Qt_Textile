#include "userprofiledatabasehelper.h"
#include "database/Settings/ConfigurationSettingsDatabaseHelper.h"

QString UserProfileDatabaseHelper::Employee_Details_TableName = "Employee_Details";
QString UserProfileDatabaseHelper::Employee_Salary_Details_Tablename = "Employee_Salary_Details";


UserProfileDatabaseHelper::UserProfileDatabaseHelper(DatabaseHelper *db)
{
    this->db = db;
}

UserProfileDatabaseHelper::~UserProfileDatabaseHelper()
{

}

int UserProfileDatabaseHelper::startTransaction()
{
    db->startTransaction();

}

int UserProfileDatabaseHelper::insertUserProfile(UserProfileDataModel *obj)
{
    this->startTransaction();
    QMap<QString,QVariant> map;

    map.insert(Employee_Details_Name, obj->Name);
    map.insert(Employee_Details_Fathers_Name, obj->Fathers_Name);
    map.insert(Employee_Details_Date_of_Birth, obj->Date_of_Birth);
    map.insert(Employee_Details_Gender, obj->Gender);
    map.insert(Employee_Details_Nationality, obj->Nationality);
    map.insert(Employee_Details_Marital_Status, obj->Marital_Status);
    map.insert(Employee_Details_Email, obj->Email);
    map.insert(Employee_Details_Phone, obj->Phone);
    map.insert(Employee_Details_Address, obj->Address);
    map.insert(Employee_Details_Username, obj->Username);
    map.insert(Employee_Details_Password, obj->Password);
    map.insert(Employee_Details_Employee_ID, obj->Employee_ID);
    map.insert(Employee_Details_Designation, obj->Designation);
    map.insert(Employee_Details_Department, obj->Department);
    map.insert(Employee_Details_Date_of_Joining, obj->Date_of_Joining);
    map.insert(Employee_Details_Passport_No, obj->Passport_No);
    map.insert(Employee_Details_Passport_Expiry, obj->Passport_Expiry);
    map.insert(Employee_Details_Visa_No, obj->Visa_No);
    map.insert(Employee_Details_Visa_Expiry, obj->Visa_Expiry);
    map.insert(Employee_Details_Bank_Account_No, obj->Bank_Account_No);
    map.insert(Employee_Details_Bank_Name, obj->Bank_Name);
    map.insert(Employee_Details_Bank_Branch, obj->Bank_Branch);
    map.insert(Employee_Details_Bank_Code, obj->Bank_Code);
    map.insert(Employee_Details_Highest_Qualification, obj->Highest_Qualification);
    map.insert(Employee_Details_Date_of_Qualification, obj->Date_of_Qualification);
    map.insert(Employee_Details_Emergency_Contact_Name, obj->Emergency_Contact_Name);
    map.insert(Employee_Details_Emergency_Contact_No, obj->Emergency_Contact_No);
    map.insert(Employee_Details_Emergency_Contact_Relation, obj->Emergency_Contact_Relation);
    map.insert(Employee_Details_User_Group, obj->userGroup);
    map.insert(Employee_Details_Show_Emp, obj->showEmployee);
    map.insert(Employee_Details_Privelage, obj->privelage);
    map.insert(Employee_Details_Last_Modified, QDateTime::currentDateTime());


    obj->_id = db->insertRecordsPlus(map,Employee_Details_TableName);

    commit();
    return obj->_id;
}

int UserProfileDatabaseHelper::updateUserProfile(UserProfileDataModel *obj, int oldID)
{
    this->startTransaction();

    QMap<QString,QVariant> map;

    map.insert(Employee_Details_Name, obj->Name);
    map.insert(Employee_Details_Fathers_Name, obj->Fathers_Name);
    map.insert(Employee_Details_Date_of_Birth, obj->Date_of_Birth);
    map.insert(Employee_Details_Gender, obj->Gender);
    map.insert(Employee_Details_Nationality, obj->Nationality);
    map.insert(Employee_Details_Marital_Status, obj->Marital_Status);
    map.insert(Employee_Details_Email, obj->Email);
    map.insert(Employee_Details_Phone, obj->Phone);
    map.insert(Employee_Details_Address, obj->Address);
    map.insert(Employee_Details_Username, obj->Username);
    map.insert(Employee_Details_Password, obj->Password);
    map.insert(Employee_Details_Employee_ID, obj->Employee_ID);
    map.insert(Employee_Details_Designation, obj->Designation);
    map.insert(Employee_Details_Department, obj->Department);
    map.insert(Employee_Details_Date_of_Joining, obj->Date_of_Joining);
    map.insert(Employee_Details_Passport_No, obj->Passport_No);
    map.insert(Employee_Details_Passport_Expiry, obj->Passport_Expiry);
    map.insert(Employee_Details_Visa_No, obj->Visa_No);
    map.insert(Employee_Details_Visa_Expiry, obj->Visa_Expiry);
    map.insert(Employee_Details_Bank_Account_No, obj->Bank_Account_No);
    map.insert(Employee_Details_Bank_Name, obj->Bank_Name);
    map.insert(Employee_Details_Bank_Branch, obj->Bank_Branch);
    map.insert(Employee_Details_Bank_Code, obj->Bank_Code);
    map.insert(Employee_Details_Highest_Qualification, obj->Highest_Qualification);
    map.insert(Employee_Details_Date_of_Qualification, obj->Date_of_Qualification);
    map.insert(Employee_Details_Emergency_Contact_Name, obj->Emergency_Contact_Name);
    map.insert(Employee_Details_Emergency_Contact_No, obj->Emergency_Contact_No);
    map.insert(Employee_Details_Emergency_Contact_Relation, obj->Emergency_Contact_Relation);
    map.insert(Employee_Details_User_Group, obj->userGroup);
    map.insert(Employee_Details_Show_Emp, obj->showEmployee);
    map.insert(Employee_Details_Privelage, obj->privelage);
    map.insert(Employee_Details_Last_Modified, QDateTime::currentDateTime());


    QMap<QString,QVariant> map3;
    map3.insert(Employee_Details_id,oldID);
    db->updateRecordsPlus(map,map3,Employee_Details_TableName);

    return commit();
}

int UserProfileDatabaseHelper::deleteUserProfile(int oldID)
{
    this->startTransaction();
    QMap<QString,QVariant> map3;
    map3.insert(Employee_Details_id,oldID);
    db->deleteRecordsPlus(map3,Employee_Details_TableName);

    return commit();
}

int UserProfileDatabaseHelper::commit()
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

int UserProfileDatabaseHelper::getMaxID()
{
    QString qStr = "SELECT MAX("+Employee_Details_id+") FROM " + Employee_Details_TableName;
    //    qDebug()<<qStr;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    int vNo = 1;
    query.prepare(qStr);
    if(query.exec()){
        while(query.next())
            vNo = query.value(0).toString().toInt();
    }
    else{
        qDebug()<<query.lastError();
    }
    return vNo;
}

QList<QSharedPointer<UserProfileDataModel> > UserProfileDatabaseHelper::getAllUsersAsPtr()
{
    QList<QSharedPointer<UserProfileDataModel> > data;

    QString qStr = "SELECT ";
    qStr += "e." + Employee_Details_id + ", ";
    qStr += "e." + Employee_Details_TimeStamp+ ", ";
    qStr += "e." + Employee_Details_Name  + ", ";
    qStr += "e." + Employee_Details_Fathers_Name + ", ";
    qStr += "e." + Employee_Details_Date_of_Birth + ", ";
    qStr += "e." + Employee_Details_Gender  + ", ";
    qStr += "e." + Employee_Details_Nationality + ", ";
    qStr += "e." + Employee_Details_Marital_Status+ ", ";
    qStr += "e." + Employee_Details_Email + ", ";
    qStr += "e." + Employee_Details_Phone + ", ";
    qStr += "e." + Employee_Details_Address + ", ";
    qStr += "e." + Employee_Details_Username + ", ";
    qStr += "e." + Employee_Details_Password + ", ";
    qStr += "e." + Employee_Details_Employee_ID + ", ";
    qStr += "e." + Employee_Details_Designation + ", ";
    qStr += "e." + Employee_Details_Department + ", ";
    qStr += "e." + Employee_Details_Date_of_Joining + ", ";
    qStr += "e." + Employee_Details_Passport_No + ", ";
    qStr += "e." + Employee_Details_Passport_Expiry + ", ";
    qStr += "e." + Employee_Details_Visa_No + ", ";
    qStr += "e." + Employee_Details_Visa_Expiry + ", ";
    qStr += "e." + Employee_Details_Bank_Account_No + ", ";
    qStr += "e." + Employee_Details_Bank_Name + ", ";
    qStr += "e." + Employee_Details_Bank_Branch + ", ";
    qStr += "e." + Employee_Details_Bank_Code + ", ";
    qStr += "e." + Employee_Details_Highest_Qualification + ", ";
    qStr += "e." + Employee_Details_Date_of_Qualification + ", ";
    qStr += "e." + Employee_Details_Emergency_Contact_Name + ", ";
    qStr += "e." + Employee_Details_Emergency_Contact_No + ", ";
    qStr += "e." + Employee_Details_Emergency_Contact_Relation + ", ";
    qStr += "e." + Employee_Details_User_Group+", ";
    qStr += "e." + Employee_Details_Show_Emp + ", ";
    qStr += Employee_Salary_BasicSalary + ", ";
    qStr += Employee_Salary_DA + ", " + Employee_Salary_HRA + ", "+ Employee_Salary_Conveyance + ", ";
    qStr += Employee_Salary_AnnualLeave + ", " + Employee_Salary_WeeklyLeave + ", " + Employee_Salary_MonthlyLeave + ", ";
    qStr += Employee_Salary_WorkingHours + ", " + Employee_Salary_OverTimePay ;
    qStr += " FROM "+ Employee_Details_TableName + " e LEFT OUTER JOIN " + Employee_Salary_Details_Tablename + " s ";
    qStr += " ON e." + Employee_Details_id + " = s." + Employee_Salary_Employee_ID;
    qStr += " ORDER BY "+ Employee_Details_Name;

//    qDebug()<<qStr;




    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    if(query.exec(qStr)){
        qDebug()<<"empl query.size = " <<query.size();
        UserProfileDataModel* obj;
        int i=0;
        while(query.next()){
            i=0;
            obj= new UserProfileDataModel();
            obj->_id = query.value(i++).toInt();
            obj->Timestamp = query.value(i++).toDateTime();
            obj->Name = query.value(i++).toString();
            obj->Fathers_Name = query.value(i++).toString();
            obj->Date_of_Birth = query.value(i++).toDate();
            obj->Gender = query.value(i++).toString();
            obj->Nationality = query.value(i++).toString();
            obj->Marital_Status = query.value(i++).toString();
            obj->Email = query.value(i++).toString();
            obj->Phone = query.value(i++).toString();
            obj->Address = query.value(i++).toString();
            obj->Username = query.value(i++).toString();
            obj->Password = query.value(i++).toString();
            obj->Employee_ID = query.value(i++).toString();
            obj->Designation = query.value(i++).toString();
            obj->Department = query.value(i++).toString();
            obj->Date_of_Joining = query.value(i++).toDate();
            obj->Passport_No = query.value(i++).toString();
            obj->Passport_Expiry = query.value(i++).toDate();
            obj->Visa_No = query.value(i++).toString();
            obj->Visa_Expiry = query.value(i++).toDate();
            obj->Bank_Account_No = query.value(i++).toString();
            obj->Bank_Name = query.value(i++).toString();
            obj->Bank_Branch = query.value(i++).toString();
            obj->Bank_Code = query.value(i++).toString();
            obj->Highest_Qualification = query.value(i++).toString();
            obj->Date_of_Qualification = query.value(i++).toDate();
            obj->Emergency_Contact_Name = query.value(i++).toString();
            obj->Emergency_Contact_No = query.value(i++).toString();
            obj->Emergency_Contact_Relation = query.value(i++).toString();
            obj->userGroup = query.value(i++).toInt();
            obj->showEmployee = query.value(i++).toBool();

//            obj->BasicSalary = query.value(i++).toFloat();
//            obj->DA = query.value(i++).toFloat();
//            obj->HRA = query.value(i++).toFloat();
//            obj->Conveyance = query.value(i++).toFloat();
//            obj->AnnualLeave = query.value(i++).toInt();
//            obj->WeeklyLeave = query.value(i++).toInt();
//            obj->MonthlyLeave = query.value(i++).toInt();
//            obj->WorkingHours = query.value(i++).toInt();
//            obj->overTimePay = query.value(i++).toInt();

            data.push_back(QSharedPointer<UserProfileDataModel>(obj));


        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<qStr;
    }
    return data;
}

QSqlQueryModel *UserProfileDatabaseHelper::getEmployeeQueryForSearch(QString DepartmentID)
{
    QString qStr;
    qStr = "SELECT ";
    qStr += Employee_Details_id + ", ";
    qStr += Employee_Details_Name+ ", ";
    qStr += Employee_Details_Department  + " ";
    qStr += " FROM ";
    qStr += Employee_Details_TableName;

    return  DatabaseHelper::getQueryModel(qStr);
}



UserProfileDataModel UserProfileDatabaseHelper::getEmployeeByID(int id)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__<<id;
    UserProfileDataModel obj;
    QString qStr = "SELECT ";
    qStr += Employee_Details_id + ", ";
    qStr += Employee_Details_TimeStamp+ ", ";
    qStr += Employee_Details_Name  + ", ";
    qStr += Employee_Details_Fathers_Name + ", ";
    qStr += Employee_Details_Date_of_Birth + ", ";
    qStr += Employee_Details_Gender  + ", ";
    qStr += Employee_Details_Nationality + ", ";
    qStr += Employee_Details_Marital_Status+ ", ";
    qStr += Employee_Details_Email + ", ";
    qStr += Employee_Details_Phone + ", ";
    qStr += Employee_Details_Address + ", ";
    qStr += Employee_Details_Username + ", ";
    qStr += Employee_Details_Password + ", ";
    qStr += Employee_Details_Employee_ID + ", ";
    qStr += Employee_Details_Designation + ", ";
    qStr += Employee_Details_Department + ", ";
    qStr += Employee_Details_Date_of_Joining + ", ";
    qStr += Employee_Details_Passport_No + ", ";
    qStr += Employee_Details_Passport_Expiry + ", ";
    qStr += Employee_Details_Visa_No + ", ";
    qStr += Employee_Details_Visa_Expiry + ", ";
    qStr += Employee_Details_Bank_Account_No + ", ";
    qStr += Employee_Details_Bank_Name + ", ";
    qStr += Employee_Details_Bank_Branch + ", ";
    qStr += Employee_Details_Bank_Code + ", ";
    qStr += Employee_Details_Highest_Qualification + ", ";
    qStr += Employee_Details_Date_of_Qualification + ", ";
    qStr += Employee_Details_Emergency_Contact_Name + ", ";
    qStr += Employee_Details_Emergency_Contact_No + ", ";
    qStr += Employee_Details_Emergency_Contact_Relation + ", ";
    qStr += Employee_Details_User_Group+", ";
    qStr += Employee_Details_Show_Emp;
    qStr += " FROM "+ Employee_Details_TableName ;
    qStr += " where " + Employee_Details_id + "='"+QString::number(id)+"'";


    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    if(query.exec(qStr)){

        int i=0;
        while(query.next()){
            i=0;
            obj._id = query.value(i++).toInt();
            obj.Timestamp = query.value(i++).toDateTime();
            obj.Name = query.value(i++).toString().toUpper();
            obj.Fathers_Name = query.value(i++).toString();
            obj.Date_of_Birth = query.value(i++).toDate();
            obj.Gender = query.value(i++).toString();
            obj.Nationality = query.value(i++).toString();
            obj.Marital_Status = query.value(i++).toString();
            obj.Email = query.value(i++).toString();
            obj.Phone = query.value(i++).toString();
            obj.Address = query.value(i++).toString();
            obj.Username = query.value(i++).toString();
            obj.Password = query.value(i++).toString();
            obj.Employee_ID = query.value(i++).toString();
            obj.Designation = query.value(i++).toString();
            obj.Department = query.value(i++).toString();
            obj.Date_of_Joining = query.value(i++).toDate();
            obj.Passport_No = query.value(i++).toString();
            obj.Passport_Expiry = query.value(i++).toDate();
            obj.Visa_No = query.value(i++).toString();
            obj.Visa_Expiry = query.value(i++).toDate();
            obj.Bank_Account_No = query.value(i++).toString();
            obj.Bank_Name = query.value(i++).toString();
            obj.Bank_Branch = query.value(i++).toString();
            obj.Bank_Code = query.value(i++).toString();
            obj.Highest_Qualification = query.value(i++).toString();
            obj.Date_of_Qualification = query.value(i++).toDate();
            obj.Emergency_Contact_Name = query.value(i++).toString();
            obj.Emergency_Contact_No = query.value(i++).toString();
            obj.Emergency_Contact_Relation = query.value(i++).toString();
            obj.userGroup = query.value(i++).toInt();
            obj.showEmployee = query.value(i++).toBool();


        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<qStr;
    }

//    QString qStr2 = " SELECT " + Employee_Salary_BasicSalary + ", ";
//    qStr2 += Employee_Salary_DA + ", " + Employee_Salary_HRA + ", "+ Employee_Salary_Conveyance + ", ";
//    qStr2 += Employee_Salary_AnnualLeave + ", " + Employee_Salary_WeeklyLeave + ", " + Employee_Salary_MonthlyLeave + ", ";
//    qStr2 += Employee_Salary_WorkingHours + " FROM " + Employee_Salary_Details_Tablename;
//    qStr2 += " where " + Employee_Salary_Employee_ID + "='"+QString::number(obj._id)+"'";

////    qDebug()<<qStr2;

//    if(query.exec(qStr2)){
//        while (query.next()) {
//            int i=0;

////            obj.BasicSalary = query.value(i++).toFloat();
////            obj.DA = query.value(i++).toFloat();
////            obj.HRA = query.value(i++).toFloat();
////            obj.Conveyance = query.value(i++).toFloat();
////            obj.AnnualLeave = query.value(i++).toInt();
////            obj.WeeklyLeave = query.value(i++).toInt();
////            obj.MonthlyLeave = query.value(i++).toInt();
////            obj.WorkingHours = query.value(i++).toInt();

//        }
//    }

//    else{
//        qDebug()<<query.lastError();
//        qDebug()<<qStr2;
//    }

    return obj;
}

QString UserProfileDatabaseHelper::getEmployeeNameFromID(int ids)
{
    QString id ="";
    QString qStr = "SELECT "+ Employee_Details_Name+ " FROM "+Employee_Details_TableName +" WHERE "+Employee_Details_id
            +" = :name";
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);
    query.bindValue(":name",ids);
    if(!query.exec()){
        qDebug()<<query.lastError();
    }
    else{
        while(query.next()){
            id = query.value(0).toString().toUpper();
        }
    }
    qDebug()<<id;
    return id;
}

QMap<int, QString> UserProfileDatabaseHelper::getAllUsers(QString group)
{

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    qDebug()<<"350 at user db";
    UserGroupDatabaseHelper *grpHelper = new UserGroupDatabaseHelper;
    QMap<int,QString> map;
    QString clause = Employee_Details_Show_Emp+"='1'";
    if(group.length()>0){
        QString str = "SELECT "+grpHelper->User_Group_Permissions_id+ " from "+grpHelper->User_Group_Permissions_TableName
                + " where "+grpHelper->User_Group_Permissions_Name +"='"+group+"'" ;

        //        qDebug()<<str;

        clause += " and "+Employee_Details_User_Group + "='";
        if(query.exec(str)){
            while(query.next()){
                group = query.value(0).toString();
            }
            clause += group+"'";
        }
        else{
            qDebug()<<query.lastError();
            qDebug()<<query.lastQuery();
            clause = Employee_Details_Show_Emp+"='1'";
        }

    }

    QString qStr = " SELECT ";
    qStr += Employee_Details_id +", ";
    qStr += Employee_Details_Name + " ";
    qStr += " from " + Employee_Details_TableName;
    qStr += " where "+ clause;
    qDebug()<<qStr;
    if(query.exec(qStr)){
        while(query.next()){
            map.insert(query.value(0).toInt(),query.value(1).toString().toUpper());
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<qStr;
    }

    delete grpHelper;
    return map;
}

QSqlQueryModel *UserProfileDatabaseHelper::visaExpiryList(int numDays)
{
    QString qStr = "SELECT  " + Employee_Details_Name + ", " + Employee_Details_Visa_Expiry;
    qStr += " FROM " + Employee_Details_TableName;
    qStr += " WHERE DATEDIFF(" + Employee_Details_Visa_Expiry + ", CURRENT_DATE ) <= " +QString::number(numDays* 30);

    qDebug()<<Q_FUNC_INFO;
    return DatabaseHelper::getQueryModel(qStr);
}

QSqlQueryModel *UserProfileDatabaseHelper::passportExpiryList(int numDays)
{
    QString qStr = "SELECT  " + Employee_Details_Name + ", " + Employee_Details_Passport_Expiry;
    qStr += " FROM " + Employee_Details_TableName;
    qStr += " WHERE DATEDIFF(" + Employee_Details_Passport_Expiry + ", CURRENT_DATE ) <= " +QString::number(numDays * 30);

    return DatabaseHelper::getQueryModel(qStr);
}
