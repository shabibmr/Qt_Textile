#include "licenseinfodatamodel.h"
#include <QDebug>

LicenseInfoDataModel::LicenseInfoDataModel()
{

}

void LicenseInfoDataModel::JsonToObj(QString jsonStr)
{
//    jsonStr.replace("\\\"", "\"");
    qDebug()<<Q_FUNC_INFO<<__LINE__<<jsonStr;
    QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8());
    QJsonObject json = doc.object();

    QString date_format = "yyyy-MM-dd";

    qDebug()<<Q_FUNC_INFO<<json;
    json = json["data"].toObject();

    this->organizationName = json["Organization_Name"].toString();
    this->contactName = json["Contact_Name"].toString();
    this->contactPhone = json["Contact_Phone"].toString();
    this->autoUpdate = json["Auto_Update"].toBool();
    this->LastUpdatedOn = QDate::fromString( json["Last_Updated"].toString() , date_format);
    this->LicenseUpdatedDate = QDate::fromString( json["License_Updated"].toString() , date_format);
    this->licenseDurationPeriod = json["License_Duration"].toString().toInt();
    this->licenseKey = json["License_Key"].toString();
    this->warning_billCount = json["billCount_for_warning"].toInt();
    this->forceStop = json["Force_Stop"].toBool();
    this->fatalMessage = json["Fatal_Message"].toString();
    this->warningMessage = json["Warning_Message"].toString();

    qDebug()<<Q_FUNC_INFO<<this->LicenseUpdatedDate<<json["License_Updated"]<<json["Force_Stop"].toBool();

}
