#ifndef LICENSEINFODATAMODEL_H
#define LICENSEINFODATAMODEL_H

#include <QDate>
#include <QJsonObject>
#include <QJsonDocument>

class LicenseInfoDataModel
{
public:
    LicenseInfoDataModel();

    QString organizationName;
    QString companyId;
    QString contactName;
    QString contactPhone;
    bool autoUpdate;
    QDate LastUpdatedOn;
    QDate LicenseUpdatedDate=QDate::currentDate();
    int licenseDurationPeriod =50;
    int warning_billCount = 0;
    bool forceStop = true;
    QString licenseKey;

    QString fatalMessage = "";
    QString warningMessage = "";

    void JsonToObj(QString jsonStr);

};

#endif // LICENSEINFODATAMODEL_H
