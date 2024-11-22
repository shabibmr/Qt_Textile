#include "addressbookdatamodel.h"

AddressBookDataModel::AddressBookDataModel()
{
    DateOfBirth = QDate::fromString("2000-01-01","yyyy-MM-dd");
    // measurement = new ContactMeasurement();

}

QJsonObject AddressBookDataModel::ObjToJson(AddressBookDataModel obj)
{

    QJsonObject jsonObject;
    qDebug()<<"CONTACT OBECT TO JSON";

    jsonObject.insert("_id",obj._id);
    jsonObject.insert("ContactName",obj.ContactName);
    jsonObject.insert("PhoneNumber",obj.PhoneNumber);
    jsonObject.insert("email",obj.email);
    jsonObject.insert("address",obj.address);
    jsonObject.insert("city",obj.city);
    jsonObject.insert("country",obj.country);
    jsonObject.insert("ledgerId",obj.ledgerId);
    jsonObject.insert("CompanyName",obj.CompanyName);
    jsonObject.insert("location",obj.location);
    jsonObject.insert("EmployeeId",obj.EmployeeId);
    jsonObject.insert("DateOfBirth",obj.DateOfBirth.toString());
    jsonObject.insert("mobileNumber",obj.mobileNumber);
    jsonObject.insert("notes",obj.notes);
    jsonObject.insert("Designation",obj.Designation);
    jsonObject.insert("DesignationID",obj.DesignationID);
    jsonObject.insert("Building",obj.Building);
    jsonObject.insert("isCompanyEmployee",obj.isCompanyEmployee);
    jsonObject.insert("isIndividual",obj.isIndividual);
    jsonObject.insert("Type",obj.Type);
    jsonObject.insert("POBox",obj.POBox);
    jsonObject.insert("Street",obj.Street);
    jsonObject.insert("Fax",obj.Fax);

    qDebug()<<"RETURN CONTACT JSON";
    return jsonObject;
}

AddressBookDataModel AddressBookDataModel::JsonToObj(QJsonObject json)
{
    AddressBookDataModel obj;

    qDebug()<<"CONTACT JSON TO OBJECT";
    QString date_format = "ddd MMM d yyyy";

    obj._id = json["_id"].toInt();
    obj.ContactName = json["ContactName"].toString();
    obj.PhoneNumber = json["PhoneNumber"].toString();
    obj.email = json["email"].toString();
    obj.address = json["address"].toString();
    obj.city = json["city"].toString();
    obj.country = json["country"].toString();
    obj.ledgerId = json["ledgerId"].toString();
    obj.CompanyName = json["CompanyName"].toString();
    obj.location = json["location"].toString();
    obj.EmployeeId = json["EmployeeId"].toString();
    obj.DateOfBirth = QDate::fromString(json["DateOfBirth"].toString(), date_format);
    obj.mobileNumber = json["mobileNumber"].toString();
    obj.notes = json["notes"].toString();
    obj.Designation = json["Designation"].toString();
    obj.DesignationID = json["DesignationID"].toString();
    obj.Building = json["Building"].toString();
    obj.isCompanyEmployee = json["isCompanyEmployee"].toBool();
    obj.isIndividual = json["isIndividual"].toBool();
    obj.Type = json["Type"].toInt();
    obj.POBox = json["POBox"].toString();
    obj.Street = json["Street"].toString();
    obj.Fax = json["Fax"].toString();

    return obj;
}
