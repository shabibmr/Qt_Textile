#ifndef LOGINVALUES_H
#define LOGINVALUES_H
#include <QString>
#include "datamodels/hr/usergroupdatamodel.h"
#include "datamodels/addressbook/companyprofiledatamodel.h"
#include "datamodels/Settings/licenseinfodatamodel.h"

class LoginValues
{
public:
    LoginValues();


    static QString userName;
    static int userID;
    static QString userEmail;
    static QString userPhone;
    static QString empName;
    static int Privilege;
    static QString voucherPrefix;
    static QString toMailAddresses;
    static bool permLogin;
    static bool test;
    static bool initLogin;

    static QString getUserName();
    static void setUserName(const QString &value);
    static int getUserID();
    static void setUserID(int value);
    static QString getUserEmail();
    static void setUserEmail(const QString &value);
    static QString getUserPhone();
    static void setUserPhone(const QString &value);
    static QString getEmpName();
    static void setEmpName(const QString &value);

    static UserGroupDataModel model;
//    static UserGroupDataModel *accessPermissions;
    static UserGroupDataModel getModel();
    static void setModel(const UserGroupDataModel &value);

    static int warningBillCount;
    static QString warningMessage;
    static bool autoUpdate;

    static CompanyProfileDataModel company;
    static CompanyProfileDataModel getCompany();
    static void setCompany(const CompanyProfileDataModel &value);
    static void setPrivelage(int value);
    static void setVoucherPrefix(const QString &value);
    static QString getToMailAddresses();
    static void setToMailAddresses(const QString &value);

    static bool getTest();
    static void setTest(bool value);
//    static UserGroupDataModel *getAccessPermissions();
//    static void setAccessPermissions(UserGroupDataModel *value);
    static void setInitLogin(bool value);
    static void setPermLogin(bool value);
    static void setWarningBillCount(int value);
    static void setWarningMessage(const QString &value);
    static void setAutoUpdate(bool value);
};

#endif // LOGINVALUES_H
