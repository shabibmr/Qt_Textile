#ifndef COMPANYPROFILEDATAMODEL_H
#define COMPANYPROFILEDATAMODEL_H

#include <QDate>
#include <QObject>

class CompanyProfileDataModel
{
public:
    CompanyProfileDataModel();

    QString CompanyName;
    QString branch;
    QString addressLine;
    QString email;
    QString PhoneNumber;
    QString webSite;
    QString trn;
    QString fssaiLicenseNo;
    QByteArray logo;

    QString ManagerName;
    QString Currency;
    QString billTagLine;

    QString city;
    QString country;
    QString ledgerId;
    QString location;
    QString Building;
    QString POBox;
    QString Street;
    QString Fax;

    QDate DateOfEstablishment;
    QString mobileNumber;
    QString notes;

    QString LicenseNumber;
    QDate InstallationDate;
    QDate ExpiryDate;
    QString expMsg;
    QString toMailAddresses;
    QString bankDetails;

    bool useBomForStockCalc;
    bool isInclusive;
    bool isTokenSystem;
    bool fifoEvaluation;
};

#endif // COMPANYPROFILEDATAMODEL_H
