#ifndef ADDRESSBOOKDATABASEHELPER_H
#define ADDRESSBOOKDATABASEHELPER_H

#include "database/databasehelper/databasehelper.h"
#include "datamodels/addressbook/addressbookdatamodel.h"
#include "datamodels/addressbook/addressbooksmall.h"

#include "customwidgets/form.h"

class AddressBookDatabaseHelper
{
public:
    AddressBookDatabaseHelper();

    Form *testWidget;

    DatabaseHelper *db;
    bool flag = true;

    QString Address_Book_TableName = "Address_Book";
    QString Address_Book_id = "_id";
    QString Address_Book_AddressId = "Address_Id";
    QString Address_Book_ContactName = "Name";
    QString Address_Book_emailAddress = "Email";
    QString Address_Book_PhoneNumber = "Phone_Number";
    QString Address_Book_Company = "Company_Name";
    QString Address_Book_address = "Address";
    QString Address_Book_ledgerId = "Ledger";
    QString Address_Book_Narration = "Narration";
    QString Address_Book_Building = "Building";
    QString Address_Book_Street = "Street";
    QString Address_Book_Location = "Location";
    QString Address_Book_City = "City";
    QString Address_Book_Country = "Country";
    QString Address_Book_POBox = "POBox";
    QString Address_Book_Mobile_Number = "Mobile_Number";
    QString Address_Book_Designation = "Designation";
    QString Address_Book_isCompanyEmployee = "isCompanyEmployee";
    QString Address_Book_isIndividual = "isIndividual";
    QString Address_Book_Type = "Type";
    QString Address_Book_Fax = "Fax";
    QString Address_Book_Route = "Route";
    QString Address_Book_Code = "CustomerCode";
    QString Address_Book_isActive = "isActive";
    QString Address_Book_DeliveryCharges = "Delivery_Charges";
    QString Address_Book_ContactUuid = "Contact_Uuid";
    QString Address_Book_DateOfBirth = "DateOfBirth";
    QString Address_Book_LocationDetails = "Location_Details";

    QString Customer_Details_TableName = "Customer_Details";
    QString Customer_Details_id = "_id";
    QString Customer_Details_Phone = "Phone";
    QString Customer_Details_Name = "Name";
    QString Customer_Details_Address = "Address";
    QString Customer_Details_Route = "Route";

    int startTransaction();

    int insertContact(AddressBookDataModel add);

    int updateContact(AddressBookDataModel add, int oldID);

    int deleteContact(int oldID);

    int commit();

    int getMaxID();

    void test();

    QList<QSharedPointer<AddressBookDataModel>> getAllContactsAsPtr();

    QList<QSharedPointer<AddressBookSmall>> getAllContactsForSearchAsPtr();

    AddressBookDataModel getContact(int id);

    AddressBookDataModel getContactByAddressID(QString addrId);


    int updateLedgerID(QString ledId,int id);

    QString getLedgerIDFromContactID(int id);

    //NOT USED
    AddressBookDataModel getContactByPhone(QString phone);


    AddressBookDataModel *getContactPtrByPhone(QString phone);
    AddressBookDataModel *getContactPtrByCode(QString code);
    AddressBookDataModel *getContactPtrByName(QString name);

    QSqlQueryModel *getAddressesForSearchModel();

    QStringList getAllRoutesForSearch();


    bool checkUuidExists(QString uuid);
    bool checkCodeExists(QString code);
    QString generateAddressId();

    QString generateNewCustomerCode();
    QSqlQueryModel *getNameAddressesForSearchModel();
};

#endif // ADDRESSBOOKDATABASEHELPER_H
