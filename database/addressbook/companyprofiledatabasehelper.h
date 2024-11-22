#ifndef COMPANYPROFILEDATABASEHELPER_H
#define COMPANYPROFILEDATABASEHELPER_H

#include "database/databasehelper/databasehelper.h"
#include "datamodels/addressbook/companyprofiledatamodel.h"

class CompanyProfileDataBaseHelper
{
public:
    CompanyProfileDataBaseHelper();

    DatabaseHelper *db;
    bool flag = true;

    QString CompanyProfile_Tablename = "Company_Profile";
    QString CompanyProfile_id = "_id";
    QString CompanyProfile_Company_Name = "Company_Name";
    QString CompanyProfile_Branch = "Branch";
    QString CompanyProfile_AddressLine = "AddressLine";
    QString CompanyProfile_Email = "Email";
    QString CompanyProfile_Phone_Number = "Phone_Number";
    QString CompanyProfile_WebSite = "WebSite";
    QString CompanyProfile_TRN = "TRN";
    QString CompanyProfile_Logo = "Logo";
    QString CompanyProfile_Manager_Name = "Manager_Name";
    QString CompanyProfile_Currency = "Currency";
    QString CompanyProfile_Bill_Tag_Line = "Bill_Tag_Line";
    QString CompanyProfile_City = "City";
    QString CompanyProfile_Country = "Country";
    QString CompanyProfile_LedgerId = "LedgerId";
    QString CompanyProfile_Location = "Location";
    QString CompanyProfile_Building = "Building";
    QString CompanyProfile_POBox = "POBox";
    QString CompanyProfile_Street = "Street";
    QString CompanyProfile_Fax = "Fax";
    QString CompanyProfile_Date_Of_Establishment = "Date_Of_Establishment";
    QString CompanyProfile_Mobile_Number = "Mobile_Number";
    QString CompanyProfile_Notes = "Notes";
    QString CompanyProfile_License_Number = "License_Number";
    QString CompanyProfile_toMail = "To_Mail_Addresses";
    QString CompanyProfile_Installation_Date = "Installation_Date";
    QString CompanyProfile_Exp_Date = "Expiry_Date";
    QString CompanyProfile_Exp_Msg = "Expiry_Msg";

    QString CompanyProfile_UseBom = "UseBom";
    QString CompanyProfile_FssaiLicense = "FssaiLicense";
    QString CompanyProfile_BankDetails = "Bank_Details";

    CompanyProfileDataModel getCompanyDetails();

    int startTransaction();

    int insertCompany(CompanyProfileDataModel *Company);

    int updateCompany(CompanyProfileDataModel *Company, int oldID);

    int deleteCompany(int oldID);

    int commit();

    QList<QStringList> getAvailableCompanies();

};

#endif // COMPANYPROFILEDATABASEHELPER_H
