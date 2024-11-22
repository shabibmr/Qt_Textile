#ifndef USERPROFILEDATAMODEL_H
#define USERPROFILEDATAMODEL_H

#include <QString>
#include <QDate>
#include <QDateTime>
#include <QJsonObject>
#include <QMetaType>

class UserProfileDataModel
{
public:
    UserProfileDataModel();
    int _id;
    QString Name;
    QString Fathers_Name;
    QDateTime Timestamp;
    QDate Date_of_Birth;
    QString Gender;
    QString Nationality;
    QString Marital_Status;
    QString Email;
    QString Phone;
    QString Address;
    QString Username;
    QString Password;
    int userGroup;
    QString Employee_ID;
    QString Designation;
    QString Department;
    QDate Date_of_Joining;
    QString Passport_No;
    QDate Passport_Expiry;
    QString Visa_No;
    QDate Visa_Expiry;
    QString Bank_Account_No;
    QString Bank_Name;
    QString Bank_Branch;
    QString Bank_Code;
    QString Highest_Qualification;
    QDate Date_of_Qualification;
    QString Emergency_Contact_Name;
    QString Emergency_Contact_No;
    QString Emergency_Contact_Relation;
    bool showEmployee;
    int privelage;


    QString googleUUID;
    QString gmailAddress;
    QString defaultVoucherPrefix;
    bool allowAllPrefix;


    bool fromExternal = false;
    int action;

//    float BasicSalary;
//    float HRA;
//    float DA;
//    float Conveyance;
//    int MonthlyLeave;
//    int WeeklyLeave;
//    int AnnualLeave;
//    int WorkingHours;
//    int overTimePay;

//    // Payment

//    int NumOfDaysWorked;
//    int PaidLeave;
//    int TotalLeave;
//    float SalaryPerDay;
//    float SalaryDeducted;
//    float TotalOvertimePayment;


    QJsonObject ObjToJson(UserProfileDataModel obj);
    QJsonObject ObjPtrToJson(UserProfileDataModel *obj);
    UserProfileDataModel JsonToObj(QJsonObject json);
    UserProfileDataModel *JsonToObjPtr(QJsonObject json);

};

Q_DECLARE_METATYPE(UserProfileDataModel)
#endif // USERPROFILEDATAMODEL_H
