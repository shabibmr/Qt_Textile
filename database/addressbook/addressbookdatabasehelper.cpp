#include "addressbookdatabasehelper.h"

AddressBookDatabaseHelper::AddressBookDatabaseHelper()
{
    db = new DatabaseHelper(DatabaseValues::connectionString);
}

int AddressBookDatabaseHelper::startTransaction()
{
    db->startTransaction();

}

int AddressBookDatabaseHelper::insertContact(AddressBookDataModel add)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    startTransaction();

    QMap<QString,QVariant> map;



    add.addressId = generateAddressId();

    map.insert(Address_Book_AddressId,add.addressId);
    map.insert(Address_Book_ContactName,add.ContactName);
    map.insert(Address_Book_emailAddress,add.email);
    map.insert(Address_Book_PhoneNumber,add.PhoneNumber);
    map.insert(Address_Book_Company,add.CompanyName);
    map.insert(Address_Book_ledgerId,add.ledgerId);
    map.insert(Address_Book_Narration,add.notes);
    map.insert(Address_Book_Building,add.Building);
    map.insert(Address_Book_Street,add.Street);
    map.insert(Address_Book_Location,add.location);
    map.insert(Address_Book_City,add.city);
    map.insert(Address_Book_Country,add.country);
    map.insert(Address_Book_POBox,add.POBox);
    map.insert(Address_Book_Mobile_Number,add.mobileNumber);
    map.insert(Address_Book_Designation,add.Designation);
    map.insert(Address_Book_Type,add.Type);
    add.address.replace("\"", "");
    add.address.replace("\n", "\\n");
    add.address.replace("\r", "\\r");
    add.address.replace("\t", "\\t");
    add.address.replace("\"", "");
    map.insert(Address_Book_address,add.address);
    map.insert(Address_Book_Fax,add.Fax);
    map.insert(Address_Book_Route,add.route);
    map.insert(Address_Book_Code,add.code);
    map.insert(Address_Book_ContactUuid, add.ContactUuid);
    map.insert(Address_Book_isActive, add.isActive);
    map.insert(Address_Book_DateOfBirth, add.DateOfBirth);
    map.insert(Address_Book_DeliveryCharges, add.deliveryCharges);
    QJsonArray array;
    QString jsonStr = "{[";
    for(int i=0;i<add.LocationDetails.length();i++){

        array.append(add.LocationDetails[i]);
//        qDebug()<<Q_FUNC_INFO<<__LINE__<<add.LocationDetails[i]<<add.LocationDetails[i].replace("\\\"", "");



         jsonStr += + "\\\"" +  add.LocationDetails[i].replace("\"", "") + "\\\",";
    }
    jsonStr.chop(1);
    jsonStr += "]}";

//    qDebug()<<Q_FUNC_INFO<<__LINE__<<jsonStr;
    jsonStr.replace("\n", "\\n");
    jsonStr.replace("\r", "\\r");
    jsonStr.replace("\t", "\\t");
//    qDebug()<<Q_FUNC_INFO<<__LINE__<<jsonStr;

/*    QJsonDocument data(array);
    qDebug()<<Q_FUNC_INFO<<__LINE__<<data.toJson(QJsonDocument::Compact);
    map.insert(Address_Book_LocationDetails,data.toJson(QJsonDocument::Compact));*/



    QJsonDocument data(array);
    map.insert(Address_Book_LocationDetails, data.toJson(QJsonDocument::Compact));
    qDebug()<<"JSON DOC : "<<data.toJson();

    db->insertRecordsPlus(map,Address_Book_TableName);


    return commit();

}

int AddressBookDatabaseHelper::updateContact(AddressBookDataModel add, int oldID)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    startTransaction();

    QMap<QString,QVariant> map;

    map.insert(Address_Book_ContactName,add.ContactName);
    map.insert(Address_Book_emailAddress,add.email);
    map.insert(Address_Book_PhoneNumber,add.PhoneNumber);
    map.insert(Address_Book_Company,add.CompanyName);
    map.insert(Address_Book_ledgerId,add.ledgerId);
    map.insert(Address_Book_Narration,add.notes);
    map.insert(Address_Book_Building,add.Building);
    map.insert(Address_Book_Street,add.Street);
    map.insert(Address_Book_Location,add.location);
    map.insert(Address_Book_City,add.city);
    map.insert(Address_Book_Country,add.country);
    map.insert(Address_Book_POBox,add.POBox);
    map.insert(Address_Book_Mobile_Number,add.mobileNumber);
    map.insert(Address_Book_Designation,add.Designation);
    map.insert(Address_Book_Type,add.Type);
    add.address.replace("\n", "\\n");
    add.address.replace("\r", "\\r");
    add.address.replace("\t", "\\t");
    add.address.replace("\"", "");
    map.insert(Address_Book_address,add.address);
    map.insert(Address_Book_Fax,add.Fax);
    map.insert(Address_Book_Route,add.route);
    map.insert(Address_Book_Code,add.code);
    map.insert(Address_Book_ContactUuid, add.ContactUuid);
    map.insert(Address_Book_isActive, add.isActive);
    map.insert(Address_Book_DateOfBirth, add.DateOfBirth);
    map.insert(Address_Book_DeliveryCharges, add.deliveryCharges);
    QJsonArray array;
    QString jsonStr = "[";
    for(int i=0;i<add.LocationDetails.length();i++){
//        qDebug()<<Q_FUNC_INFO<<__LINE__<<add.LocationDetails[i];
        array.append(add.LocationDetails[i]);

         jsonStr += + "\\\"" +  add.LocationDetails[i].replace("\"", "") + "\\\",";
//         qDebug()<<Q_FUNC_INFO<<__LINE__<<jsonStr;
    }
    jsonStr.chop(1);
    jsonStr += "]";

    jsonStr.replace("\n", "\\n");
    jsonStr.replace("\r", "\\r");
    jsonStr.replace("\t", "\\t");

/*    QJsonDocument data(array);
    qDebug()<<Q_FUNC_INFO<<__LINE__<<data.toJson(QJsonDocument::Compact);
    map.insert(Address_Book_LocationDetails,data.toJson(QJsonDocument::Compact));*/

    QJsonDocument data(array);
    map.insert(Address_Book_LocationDetails, data.toJson(QJsonDocument::Compact));

    QMap<QString,QVariant> clause;
    clause.insert(Address_Book_AddressId,add.addressId);
    db->updateRecordsPlus(map,clause,Address_Book_TableName);

    return commit();

}

int AddressBookDatabaseHelper::deleteContact(int oldID)
{
    startTransaction();

    QMap<QString,QVariant> clause;
    clause.insert(Address_Book_id,oldID);
    db->deleteRecordsPlus(clause,Address_Book_TableName);

    return commit();

}

int AddressBookDatabaseHelper::commit()
{

    if(flag){
        db->commitTransaction();
        return 1;
    }
    else{
        db->rollBackTransaction();
        return 0;
    }
}

int AddressBookDatabaseHelper::getMaxID()
{

}

void AddressBookDatabaseHelper::test()
{
    testWidget = new Form;
    testWidget->setWindowFlags(Qt::Window);
    testWidget->show();
}

QList<QSharedPointer<AddressBookDataModel> > AddressBookDatabaseHelper::getAllContactsAsPtr()
{

}

QList<QSharedPointer<AddressBookSmall>> AddressBookDatabaseHelper::getAllContactsForSearchAsPtr()
{
    QList<QSharedPointer<AddressBookSmall>> data;

    QString qStr = "SELECT " +
            Address_Book_id + ", "+
            Address_Book_ContactName + ", " +
            Address_Book_Company + ", " +
            Address_Book_PhoneNumber + " "
                                       " FROM " + Address_Book_TableName +
            " WHERE "+Address_Book_Type + "='1'"
                                          " ORDER BY "+Address_Book_ContactName;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    if(query.exec(qStr)){
        AddressBookSmall *obj;
        while(query.next()){
            obj=new AddressBookSmall();
            obj->_id = query.value(0).toInt();
            obj->Name = query.value(1).toString();
            obj->CompanyName = query.value(2).toString();
            obj->phoneNumber = query.value(3).toString();
            data.push_back(QSharedPointer<AddressBookSmall>(obj));
        }
    }
    else{
        qDebug()<<query.lastQuery();
    }
    return data;
}

AddressBookDataModel AddressBookDatabaseHelper::getContact(int id)
{
    AddressBookDataModel obj;

    QString qStr = "SELECT " +
            Address_Book_id + ", "+
            Address_Book_ContactName + ", " +
            Address_Book_Company + ", " +
            Address_Book_PhoneNumber + ", " +
            Address_Book_Location + ", " +
            Address_Book_ledgerId + ", " +
            Address_Book_Building + ", " +
            Address_Book_City + ", " +
            Address_Book_Country + ", " +
            Address_Book_emailAddress + ", " +
            Address_Book_Designation + ", " +
            Address_Book_Type + ", " +
            Address_Book_Narration + ", " +
            Address_Book_Mobile_Number + ", " +
            Address_Book_POBox + ", " +
            Address_Book_Street + ", " +
            Address_Book_address + ", " +
            Address_Book_Fax + ", " +
            Address_Book_AddressId + " " +
            " FROM " + Address_Book_TableName +
            " WHERE " + Address_Book_id +"='"+QString::number(id)+"'";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);


    if(query.exec()){
        while (query.next()) {
            obj._id = query.value(0).toInt();
            obj.ContactName = query.value(1).toString();
            obj.CompanyName = query.value(2).toString();
            obj.PhoneNumber = query.value(3).toString();
            obj.location = query.value(4).toString();
            obj.ledgerId = query.value(5).toString();
            obj.Building = query.value(6).toString();
            obj.city = query.value(7).toString();
            obj.country = query.value(8).toString();
            obj.email = query.value(9).toString();
            obj.Designation = query.value(10).toString();
            obj.Type = query.value(11).toInt();
            obj.notes = query.value(12).toString();
            obj.mobileNumber = query.value(13).toString();
            obj.POBox = query.value(14).toString();
            obj.Street = query.value(15).toString();
            obj.address = query.value(16).toString();
            obj.Fax = query.value(17).toString();
            obj.addressId  = query.value(18).toString();
        }
    }
    else{
        qDebug()<<"No records";
        qDebug()<<qStr;
    }
    return obj;

}

AddressBookDataModel AddressBookDatabaseHelper::getContactByAddressID(QString addrId)
{
    AddressBookDataModel obj;

    QString qStr = "SELECT " +
                   Address_Book_id + ", "+
                   Address_Book_ContactName + ", " +
                   Address_Book_Company + ", " +
                   Address_Book_PhoneNumber + ", " +
                   Address_Book_Location + ", " +
                   Address_Book_ledgerId + ", " +
                   Address_Book_Building + ", " +
                   Address_Book_City + ", " +
                   Address_Book_Country + ", " +
                   Address_Book_emailAddress + ", " +
                   Address_Book_Designation + ", " +
                   Address_Book_Type + ", " +
                   Address_Book_Narration + ", " +
                   Address_Book_Mobile_Number + ", " +
                   Address_Book_POBox + ", " +
                   Address_Book_Street + ", " +
                   Address_Book_address + ", " +
                   Address_Book_Fax + ", " +
                   Address_Book_AddressId + " " +
                   " FROM " + Address_Book_TableName +
                   " WHERE " + Address_Book_AddressId +"='"+addrId+"'";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);


    if(query.exec()){
        while (query.next()) {
            obj._id = query.value(0).toInt();
            obj.ContactName = query.value(1).toString();
            obj.CompanyName = query.value(2).toString();
            obj.PhoneNumber = query.value(3).toString();
            obj.location = query.value(4).toString();
            obj.ledgerId = query.value(5).toString();
            obj.Building = query.value(6).toString();
            obj.city = query.value(7).toString();
            obj.country = query.value(8).toString();
            obj.email = query.value(9).toString();
            obj.Designation = query.value(10).toString();
            obj.Type = query.value(11).toInt();
            obj.notes = query.value(12).toString();
            obj.mobileNumber = query.value(13).toString();
            obj.POBox = query.value(14).toString();
            obj.Street = query.value(15).toString();
            obj.address = query.value(16).toString();
            obj.Fax = query.value(17).toString();
            obj.addressId  = query.value(18).toString();
        }
    }
    else{
        qDebug()<<"No records";
        qDebug()<<qStr;
    }
    return obj;
}

int AddressBookDatabaseHelper::updateLedgerID(QString ledId,int id)
{
    QMap<QString,QVariant> map;
    map.insert(Address_Book_ledgerId,ledId);
    QMap<QString,QVariant> clause;
    clause.insert(Address_Book_id,id);
    db->updateRecordsPlus(map,clause,Address_Book_TableName);
}

QString AddressBookDatabaseHelper::getLedgerIDFromContactID(int id)
{
    QString ledId="";
    QString qStr = "SELECT "+Address_Book_ledgerId+ " FROM "+ Address_Book_TableName +
            + " WHERE "+Address_Book_id + " =:id";
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);
    query.bindValue(":id",id);
    if(query.exec()){
        while(query.next())
            ledId = query.value(0).toString();
    }
    else{
        qDebug()<<query.lastQuery();
        qDebug()<<query.lastError();
    }
    return ledId;
}

AddressBookDataModel AddressBookDatabaseHelper::getContactByPhone(QString phone)
{
    AddressBookDataModel obj;

    QString qStr = " SELECT " + Customer_Details_Name + ", ";
    qStr += Customer_Details_Phone + ", " + Customer_Details_Address + ", " + Customer_Details_Route;
    qStr += " FROM " + Customer_Details_TableName;
    qStr += " WHERE " + Customer_Details_Phone + " = '" + phone + "'";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);

    if(query.exec()){
        obj.ContactName = query.value(0).toString();
        obj.PhoneNumber = query.value(1).toString();
        obj.address = query.value(2).toString();
        obj.route = query.value(3).toString();

    }


}

AddressBookDataModel *AddressBookDatabaseHelper::getContactPtrByPhone(QString phone)
{
    AddressBookDataModel *obj = new AddressBookDataModel;

    QString qStr = " SELECT " + Address_Book_id + ", " + Address_Book_AddressId ;
    qStr += ", " + Address_Book_ContactName + ", ";
    qStr += Address_Book_PhoneNumber + ", " + Address_Book_address + ", " + Address_Book_Route + ", " + Address_Book_Code;
    qStr += ", " + Address_Book_isActive + ", " + Address_Book_DeliveryCharges +", ";
    qStr += Address_Book_ledgerId + ", ";
    qStr += Address_Book_emailAddress + ", ";
    qStr += Address_Book_ContactUuid + ", " ;
    qStr += Address_Book_LocationDetails;
    qStr += " FROM " + Address_Book_TableName;
    qStr += " WHERE " + Address_Book_PhoneNumber + " = '" + phone + "'";
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QJsonArray array;
    QJsonDocument doc;
    qDebug()<<qStr;
    query.prepare(qStr);
    if(query.exec()){
        if(query.first()){
            int i=0;
//            qDebug()<<Q_FUNC_INFO<<__LINE__<<query.value(0).toInt()<<qStr;
            obj->_id = query.value(i++).toInt();
            obj->addressId  = query.value(i++).toString();
            obj->ContactName = query.value(i++).toString();
            obj->PhoneNumber = query.value(i++).toString();
            obj->address = query.value(i++).toString();
            obj->address.replace("\\n", "\n");
            obj->address.replace("\\r", "\r");
            obj->address.replace("\\t", "\t");
            obj->route = query.value(i++).toString();
            obj->code = query.value(i++).toString();
            obj->isActive = query.value(i++).toBool();
            obj->deliveryCharges = query.value(i++).toFloat();
            obj->ledgerId = query.value(i++).toString();
            obj->email = query.value(i++).toString();
            obj->ContactUuid = query.value(i++).toString();
            //commented by FR
            QJsonParseError *error = new QJsonParseError;
            QVariant var = query.value(i++);


            doc = QJsonDocument::fromJson(var.toByteArray(),error);
            qDebug()<<Q_FUNC_INFO<<__LINE__<<error->error<<doc.array()<<var;

            array = doc.array();

            for(int i=0;i<array.size();i++){
                obj->LocationDetails.append(array.at(i).toString());
            }

            // copied from next fn
//            doc = QJsonDocument::fromJson(query.value(i++).toByteArray());
//            array = doc.array();
//            for(int i=0;i<array.size();i++){
//                obj->LocationDetails.append(array.at(i).toString());
//            }

        }

    }
    qDebug()<<obj->LocationDetails<<obj->address;
    if(obj->LocationDetails.size()==0 && obj->address.length()>0)
        obj->LocationDetails.append(obj->address);

    qDebug()<<Q_FUNC_INFO<<__LINE__<<obj->LocationDetails<<obj->address;


    return  obj;
}

AddressBookDataModel *AddressBookDatabaseHelper::getContactPtrByCode(QString code)
{
    AddressBookDataModel *obj = new AddressBookDataModel;

    QString qStr = " SELECT " + Address_Book_id + ", " + Address_Book_AddressId ;
    qStr += ", " + Address_Book_ContactName + ", ";
    qStr += Address_Book_PhoneNumber + ", " + Address_Book_address + ", " + Address_Book_Route + ", " + Address_Book_Code;
    qStr += ", " + Address_Book_isActive + ", " + Address_Book_DeliveryCharges +", ";
    qStr += Address_Book_ledgerId + ", ";
    qStr += Address_Book_emailAddress + ", ";
    qStr += Address_Book_ContactUuid + ", " ;
    qStr += Address_Book_LocationDetails;
    qStr += " FROM " + Address_Book_TableName;
    qStr += " WHERE " + Address_Book_Code + " = '" + code + "'";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QJsonArray array;
    QJsonDocument doc;
    query.prepare(qStr);

    if(query.exec()){
        if(query.first()){
            int i=0;
            obj->_id = query.value(i++).toInt();
            obj->addressId  = query.value(i++).toString();
            obj->ContactName = query.value(i++).toString();
            obj->PhoneNumber = query.value(i++).toString();
            obj->address = query.value(i++).toString();
            obj->address.replace("\\n", "\n");
            obj->address.replace("\\r", "\r");
            obj->address.replace("\\t", "\t");
            obj->route = query.value(i++).toString();
            obj->code = query.value(i++).toString();
            obj->isActive = query.value(i++).toBool();
            obj->deliveryCharges = query.value(i++).toFloat();
            obj->ledgerId = query.value(i++).toString();
            obj->email = query.value(i++).toString();
            obj->ContactUuid = query.value(i++).toString();
            doc = QJsonDocument::fromJson(query.value(i++).toByteArray());
            array = doc.array();
            for(int i=0;i<array.size();i++){
                obj->LocationDetails.append(array.at(i).toString());
            }


        }

    }
    if(obj->LocationDetails.length()==0 && obj->address.length()>0)
        obj->LocationDetails.append(obj->address);


    return  obj;
}

QSqlQueryModel *AddressBookDatabaseHelper::getAddressesForSearchModel()
{
    QString qStr = " SELECT " + Address_Book_id + ", " + Address_Book_ContactName + ", ";
    qStr += Address_Book_PhoneNumber + ", " + Address_Book_address + ", " + Address_Book_Route + ", " + Address_Book_Code;
    qStr += " FROM " + Address_Book_TableName +" order by "+ Address_Book_PhoneNumber;
    return DatabaseHelper::getQueryModel(qStr);

}
QSqlQueryModel *AddressBookDatabaseHelper::getNameAddressesForSearchModel()
{
    QString qStr = " SELECT " + Address_Book_id + ", " + Address_Book_ContactName + ", ";
    qStr += Address_Book_PhoneNumber + ", " + Address_Book_address + ", " + Address_Book_Route + ", " + Address_Book_Code;
    qStr += " FROM " + Address_Book_TableName +" order by "+ Address_Book_ContactName;

    return DatabaseHelper::getQueryModel(qStr);
}


AddressBookDataModel *AddressBookDatabaseHelper::getContactPtrByName(QString name)
{
    AddressBookDataModel *obj = new AddressBookDataModel;

    QString qStr = " SELECT " + Address_Book_id + ", " + Address_Book_AddressId ;
    qStr += ", " + Address_Book_ContactName + ", ";
    qStr += Address_Book_PhoneNumber + ", " + Address_Book_address + ", " + Address_Book_Route + ", " + Address_Book_Code;
    qStr += ", " + Address_Book_isActive + ", " + Address_Book_DeliveryCharges +", ";
    qStr += Address_Book_ledgerId + ", ";
    qStr += Address_Book_emailAddress + ", ";
    qStr += Address_Book_ContactUuid + ", ";
    qStr += Address_Book_LocationDetails;
    qStr += " FROM " + Address_Book_TableName;
    qStr += " WHERE " + Address_Book_ContactName + " = '" + name + "'";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QJsonArray array;
    QJsonDocument doc;
    query.prepare(qStr);

    if(query.exec()){
        if(query.first()){
            int i=0;
            obj->_id = query.value(i++).toInt();
            obj->addressId  = query.value(i++).toString();
            obj->ContactName = query.value(i++).toString();
            obj->PhoneNumber = query.value(i++).toString();
            obj->address = query.value(i++).toString();
            obj->address.replace("\\n", "\n");
            obj->address.replace("\\r", "\r");
            obj->address.replace("\\t", "\t");
            obj->route = query.value(i++).toString();
            obj->code = query.value(i++).toString();
            obj->isActive = query.value(i++).toBool();
            obj->deliveryCharges = query.value(i++).toFloat();
            obj->ledgerId = query.value(i++).toString();
            obj->email = query.value(i++).toString();
            obj->ContactUuid = query.value(i++).toString();
            doc = QJsonDocument::fromJson(query.value(i++).toByteArray());
            array = doc.array();
            for(int i=0;i<array.size();i++){

                qDebug()<<Q_FUNC_INFO<<__LINE__<<array.at(i).toString();
//                QString locStr = array.at(i).toString();
//                locStr.replace("\\n", "\n");
//                locStr.replace("\\r", "\r");
//                locStr.replace("\\t", "\t");
                obj->LocationDetails.append(array.at(i).toString());
            }
        }

    }

    return  obj;

}

bool AddressBookDatabaseHelper::checkUuidExists(QString uuid)
{
    QString qStr = "  SELECT 1 from " + Address_Book_TableName + " WHERE " + Address_Book_AddressId + " = '" + uuid + "'";
    if( DatabaseHelper::getSingletonQueryResult(qStr).toInt() == 1)
        return true;
    else
        return false;
}

bool AddressBookDatabaseHelper::checkCodeExists(QString code)
{
    QString qStr = "  SELECT 1 from " + Address_Book_TableName + " WHERE " + Address_Book_Code + " = '" + code + "'";
    if( DatabaseHelper::getSingletonQueryResult(qStr).toInt() == 1)
        return true;
    else
        return false;
}

QString AddressBookDatabaseHelper::generateAddressId()
{
    QString addrId;
    do{
        qDebug()<<Q_FUNC_INFO<<__LINE__<<"generating new id";
        addrId = db->getUUID();
    } while (checkUuidExists(addrId));

    return addrId;
}

QString AddressBookDatabaseHelper::generateNewCustomerCode()
{
    QString qStr = " SELECT max(cast("+ Address_Book_Code + " as int)) + 1 FROM " + Address_Book_TableName;
    return DatabaseHelper::getSingletonQueryResult(qStr).toString();
}




QStringList AddressBookDatabaseHelper::getAllRoutesForSearch()
{
    QString qStr = " SELECT distinct coalesce(" + Address_Book_Route + ",'') FROM " + Address_Book_TableName;

    QStringList routes;


    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);

    if(query.exec()){
        while (query.next()) {
            routes<<query.value(0).toString();
        }
    }
    return routes;

}

