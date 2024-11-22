#ifndef ADDRESSBOOKDATAMODEL_H
#define ADDRESSBOOKDATAMODEL_H
#include <QDebug>
#include <QString>
#include <QDate>
#include <QJsonObject>
#include <QJsonArray>
//#include "datamodels/inventory/measurements/contactmeasurement.h"

class AddressBookDataModel
{
public:
    AddressBookDataModel();

    int _id=0;
    QString addressId="";
    QString ContactName;
    QString ContactUuid;
    QString PhoneNumber;
    QString email;
    QString address;
    QString route;
    QString code;
    QString city;
    QString country;
    QString ledgerId;
	QString CompanyName;
	QString location;
	QString EmployeeId;
    QDate DateOfBirth;
	QString mobileNumber;
	QString notes;
	QString Designation;
	QString DesignationID;
    QString Building;
    bool isCompanyEmployee;
	bool isIndividual;
    int Type;
    QString POBox;
    QString Street;
    QString Fax;
    QStringList LocationDetails;
    bool isActive = false;
    float deliveryCharges =0;

    //ContactMeasurement *measurement;

    QJsonObject ObjToJson(AddressBookDataModel obj);
    AddressBookDataModel JsonToObj(QJsonObject json);
};

#endif // ADDRESSBOOKDATAMODEL_H
