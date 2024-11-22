#include "userprofiledatamodel.h"

UserProfileDataModel::UserProfileDataModel()
{
    Date_of_Birth =  QDate::fromString("2000-01-01","yyyy-MM-dd");
    Date_of_Joining =  QDate::fromString("2000-01-01","yyyy-MM-dd");
    Date_of_Qualification=  QDate::fromString("2000-01-01","yyyy-MM-dd");
    Visa_Expiry=  QDate::fromString("2000-01-01","yyyy-MM-dd");
    Passport_Expiry =  QDate::fromString("2000-01-01","yyyy-MM-dd");
    Timestamp = QDateTime::fromString("2000-01-01 00:00:00", "yyyy-MM-dd HH:mm:ss");
}

QJsonObject UserProfileDataModel::ObjToJson(UserProfileDataModel obj)
{
    QJsonObject jsonObject;

    jsonObject.insert("_id",obj._id);
    jsonObject.insert("Name",obj.Name);
    jsonObject.insert("Fathers_Name",obj.Fathers_Name);
    jsonObject.insert("Timestamp",obj.Timestamp.toString());
    jsonObject.insert("Date_of_Birth",obj.Date_of_Birth.toString());
    jsonObject.insert("Gender",obj.Gender);
    jsonObject.insert("Nationality",obj.Nationality);
    jsonObject.insert("Marital_Status",obj.Marital_Status);
    jsonObject.insert("Email",obj.Email);
    jsonObject.insert("Phone",obj.Phone);
    jsonObject.insert("Address",obj.Address);
    jsonObject.insert("Username",obj.Username);
    jsonObject.insert("Password",obj.Password);
    jsonObject.insert("userGroup",obj.userGroup);
    jsonObject.insert("Employee_ID",obj.Employee_ID);
    jsonObject.insert("Designation",obj.Designation);
    jsonObject.insert("Department",obj.Department);
    jsonObject.insert("Date_of_Joining",obj.Date_of_Joining.toString());
    jsonObject.insert("Passport_No",obj.Passport_No);
    jsonObject.insert("Passport_Expiry",obj.Passport_Expiry.toString());
    jsonObject.insert("Visa_No",obj.Visa_No);
    jsonObject.insert("Visa_Expiry",obj.Visa_Expiry.toString());
    jsonObject.insert("Bank_Account_No",obj.Bank_Account_No);
    jsonObject.insert("Bank_Name",obj.Bank_Name);
    jsonObject.insert("Bank_Branch",obj.Bank_Branch);
    jsonObject.insert("Bank_Code",obj.Bank_Code);
    jsonObject.insert("Highest_Qualification",obj.Highest_Qualification);
    jsonObject.insert("Date_of_Qualification",obj.Date_of_Qualification.toString());
    jsonObject.insert("Emergency_Contact_Name",obj.Emergency_Contact_Name);
    jsonObject.insert("Emergency_Contact_No",obj.Emergency_Contact_No);
    jsonObject.insert("Emergency_Contact_Relation",obj.Emergency_Contact_Relation);

    return jsonObject;
}

QJsonObject UserProfileDataModel::ObjPtrToJson(UserProfileDataModel *obj)
{
    QJsonObject jsonObject;

    jsonObject.insert("_id",obj->_id);
    jsonObject.insert("Name",obj->Name);
    jsonObject.insert("Fathers_Name",obj->Fathers_Name);
    jsonObject.insert("Timestamp",obj->Timestamp.toString());
    jsonObject.insert("Date_of_Birth",obj->Date_of_Birth.toString());
    jsonObject.insert("Gender",obj->Gender);
    jsonObject.insert("Nationality",obj->Nationality);
    jsonObject.insert("Marital_Status",obj->Marital_Status);
    jsonObject.insert("Email",obj->Email);
    jsonObject.insert("Phone",obj->Phone);
    jsonObject.insert("Address",obj->Address);
    jsonObject.insert("Username",obj->Username);
    jsonObject.insert("Password",obj->Password);
    jsonObject.insert("userGroup",obj->userGroup);
    jsonObject.insert("Employee_ID",obj->Employee_ID);
    jsonObject.insert("Designation",obj->Designation);
    jsonObject.insert("Department",obj->Department);
    jsonObject.insert("Date_of_Joining",obj->Date_of_Joining.toString());
    jsonObject.insert("Passport_No",obj->Passport_No);
    jsonObject.insert("Passport_Expiry",obj->Passport_Expiry.toString());
    jsonObject.insert("Visa_No",obj->Visa_No);
    jsonObject.insert("Visa_Expiry",obj->Visa_Expiry.toString());
    jsonObject.insert("Bank_Account_No",obj->Bank_Account_No);
    jsonObject.insert("Bank_Name",obj->Bank_Name);
    jsonObject.insert("Bank_Branch",obj->Bank_Branch);
    jsonObject.insert("Bank_Code",obj->Bank_Code);
    jsonObject.insert("Highest_Qualification",obj->Highest_Qualification);
    jsonObject.insert("Date_of_Qualification",obj->Date_of_Qualification.toString());
    jsonObject.insert("Emergency_Contact_Name",obj->Emergency_Contact_Name);
    jsonObject.insert("Emergency_Contact_No",obj->Emergency_Contact_No);
    jsonObject.insert("Emergency_Contact_Relation",obj->Emergency_Contact_Relation);

    return jsonObject;
}

UserProfileDataModel UserProfileDataModel::JsonToObj(QJsonObject json)
{

}

UserProfileDataModel *UserProfileDataModel::JsonToObjPtr(QJsonObject json)
{

}
