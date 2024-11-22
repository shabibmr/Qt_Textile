#include "loginvalues.h"

QString LoginValues::userName="";
int LoginValues::userID=0;
QString LoginValues::userEmail="";
QString LoginValues::userPhone="";
QString LoginValues::empName="";
int LoginValues::Privilege=0;
QString LoginValues::voucherPrefix="A";    //DO SOMETHING
bool LoginValues::permLogin=false;
bool LoginValues::test=false;
bool LoginValues::initLogin=false;
int LoginValues::warningBillCount=0;
QString LoginValues::warningMessage="";
bool LoginValues::autoUpdate=false;

UserGroupDataModel LoginValues::model;
CompanyProfileDataModel LoginValues::company;
//UserGroupDataModel *LoginValues::accessPermissions;

LoginValues::LoginValues()
{
}

void LoginValues::setPermLogin(bool value)
{
    permLogin = value;
}

void LoginValues::setInitLogin(bool value)
{
    initLogin = value;
}

void LoginValues::setTest(bool value)
{
    test = value;
}

bool LoginValues::getTest()
{
    return test;
}


void LoginValues::setVoucherPrefix(const QString &value)
{
    voucherPrefix = value;
}

void LoginValues::setPrivelage(int value)
{
    Privilege = value;
}

QString LoginValues::getEmpName()
{
    return empName;
}

void LoginValues::setEmpName(const QString &value)
{
    empName = value;
}

//UserGroupDataModel *LoginValues::getAccessPermissions()
//{
//    return accessPermissions;
//}

//void LoginValues::setAccessPermissions(UserGroupDataModel *value)
//{
//    accessPermissions = value;
//}

UserGroupDataModel LoginValues::getModel()
{
    return model;
}

CompanyProfileDataModel LoginValues::getCompany()
{
    return company;
}

void LoginValues::setCompany(const CompanyProfileDataModel &value)
{
    company = value;
}

void LoginValues::setModel(const UserGroupDataModel &value)
{
    model = value;
}

void LoginValues::setAutoUpdate(bool value)
{
    autoUpdate = value;
}

void LoginValues::setWarningMessage(const QString &value)
{
    warningMessage = value;
}

void LoginValues::setWarningBillCount(int value)
{
    warningBillCount = value;
}

QString LoginValues::getUserPhone()
{
    return userPhone;
}

void LoginValues::setUserPhone(const QString &value)
{
    userPhone = value;
}

QString LoginValues::getUserEmail()
{
    return userEmail;
}

void LoginValues::setUserEmail(const QString &value)
{
    userEmail = value;
}

int LoginValues::getUserID()
{
    return userID;
}

void LoginValues::setUserID(int value)
{
    userID = value;
}

QString LoginValues::getUserName()
{
    return userName;
}

void LoginValues::setUserName(const QString &value)
{
    userName = value;
}
