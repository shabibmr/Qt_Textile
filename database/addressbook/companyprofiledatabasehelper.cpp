#include "companyprofiledatabasehelper.h"
#include <QMessageBox>
#include <QFile>

CompanyProfileDataBaseHelper::CompanyProfileDataBaseHelper()
{
    db = new DatabaseHelper();
}

CompanyProfileDataModel CompanyProfileDataBaseHelper::getCompanyDetails()
{
    CompanyProfileDataModel company;

    QString qStr = "SELECT " +
            CompanyProfile_Company_Name + ", "+
            CompanyProfile_Branch + ", "+
            CompanyProfile_AddressLine + ", "+
            CompanyProfile_Email + ", "+
            CompanyProfile_Phone_Number + ", "+
            CompanyProfile_WebSite + ", "+
            CompanyProfile_TRN + ", "+
            CompanyProfile_FssaiLicense + ", "+
            CompanyProfile_Logo + ", "+
            CompanyProfile_Manager_Name + ", "+
            CompanyProfile_Currency + ", "+
            CompanyProfile_Bill_Tag_Line + ", "+
            CompanyProfile_City + ", "+
            CompanyProfile_Country + ", "+
            CompanyProfile_LedgerId + ", "+
            CompanyProfile_Location + ", "+
            CompanyProfile_Building + ", "+
            CompanyProfile_POBox + ", "+
            CompanyProfile_Street + ", "+
            CompanyProfile_Fax + ", "+
            CompanyProfile_Date_Of_Establishment + ", "+
            CompanyProfile_Mobile_Number + ", "+
            CompanyProfile_Notes + ", "+
            CompanyProfile_License_Number + ", "+
            CompanyProfile_toMail +", "+
            CompanyProfile_UseBom +" , "+
            CompanyProfile_Installation_Date + ", "+
            CompanyProfile_BankDetails + ", " +
            CompanyProfile_Exp_Date +", " +
            CompanyProfile_Exp_Msg +
            " FROM " + CompanyProfile_Tablename +
            " WHERE "+ CompanyProfile_id + "= 1";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    query.prepare(qStr);

    if(query.exec()){
        if(query.size()<1){
            QMessageBox box; box.setText("No Company Found"); box.exec();
        }
        else{
            int i=0;
            while(query.next()){
                i=0;
                company.CompanyName = query.value(i++).toString();
                company.branch = query.value(i++).toString();
                company.addressLine = query.value(i++).toString();
                company.email = query.value(i++).toString();
                company.PhoneNumber = query.value(i++).toString();
                company.webSite = query.value(i++).toString();
                company.trn = query.value(i++).toString();
                company.fssaiLicenseNo = query.value(i++).toString();
                company.logo = query.value(i++).toByteArray();
                company.ManagerName = query.value(i++).toString();
                company.Currency = query.value(i++).toString();
                company.billTagLine = query.value(i++).toString();
                company.city = query.value(i++).toString();
                company.country = query.value(i++).toString();
                company.ledgerId = query.value(i++).toString();
                company.location = query.value(i++).toString();
                company.Building = query.value(i++).toString();
                company.POBox = query.value(i++).toString();
                company.Street = query.value(i++).toString();
                company.Fax = query.value(i++).toString();
                company.DateOfEstablishment = query.value(i++).toDate();
                company.mobileNumber = query.value(i++).toString();
                company.notes = query.value(i++).toString();
                company.LicenseNumber = query.value(i++).toString();
                company.toMailAddresses = query.value(i++).toString();
                QFile inputFile2("debugdev.txt");
                if (inputFile2.open(QIODevice::ReadOnly))
                {
                    company.toMailAddresses = "fathimarafiq.fr@gmail.com|";
                }

                company.useBomForStockCalc = query.value(i++).toBool();
                company.InstallationDate = query.value(i++).toDate();
                company.bankDetails = query.value(i++).toString();
                company.ExpiryDate =query.value(i++).toDate();
                company.expMsg =  query.value(i++).toString();
            }
        }
    }
    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
        qDebug()<<"Hello";
        QMessageBox box; box.setText("No Company Found"); box.exec();
    }
    return company;
}

int CompanyProfileDataBaseHelper::startTransaction()
{
    db->startTransaction();
}

int CompanyProfileDataBaseHelper::insertCompany(CompanyProfileDataModel *Company)
{
    QMap<QString,QVariant> map;
    map.insert(CompanyProfile_Company_Name,Company->CompanyName);
    map.insert(CompanyProfile_Branch,Company->branch);
    map.insert(CompanyProfile_AddressLine,Company->addressLine);
    map.insert(CompanyProfile_Email,Company->email);
    map.insert(CompanyProfile_Phone_Number,Company->PhoneNumber);
    map.insert(CompanyProfile_WebSite,Company->webSite);
    map.insert(CompanyProfile_TRN,Company->trn);
    map.insert(CompanyProfile_FssaiLicense,Company->fssaiLicenseNo);

    map.insert(CompanyProfile_Logo,Company->logo);
    map.insert(CompanyProfile_Manager_Name,Company->ManagerName);
    map.insert(CompanyProfile_Currency,Company->Currency);
    map.insert(CompanyProfile_Bill_Tag_Line,Company->billTagLine);
    map.insert(CompanyProfile_City,Company->city);
    map.insert(CompanyProfile_Country,Company->country);
    map.insert(CompanyProfile_LedgerId,Company->ledgerId);
    map.insert(CompanyProfile_Location,Company->location);
    map.insert(CompanyProfile_Building,Company->Building);
    map.insert(CompanyProfile_POBox,Company->POBox);
    map.insert(CompanyProfile_Street,Company->Street);
    map.insert(CompanyProfile_Fax,Company->Fax);
    map.insert(CompanyProfile_Date_Of_Establishment,Company->DateOfEstablishment);
    map.insert(CompanyProfile_Mobile_Number,Company->mobileNumber);
    map.insert(CompanyProfile_Notes,Company->notes);
    map.insert(CompanyProfile_License_Number,Company->LicenseNumber);
    map.insert(CompanyProfile_toMail,Company->toMailAddresses);
    map.insert(CompanyProfile_UseBom,Company->useBomForStockCalc);
    map.insert(CompanyProfile_Installation_Date,Company->InstallationDate);

    db->insertRecordsPlus(map,CompanyProfile_Tablename);

}

int CompanyProfileDataBaseHelper::updateCompany(CompanyProfileDataModel *Company, int oldID)
{
    QMap<QString,QVariant> map;
    map.insert(CompanyProfile_Company_Name,Company->CompanyName);
    map.insert(CompanyProfile_Branch,Company->branch);
    map.insert(CompanyProfile_AddressLine,Company->addressLine);
    map.insert(CompanyProfile_Email,Company->email);
    map.insert(CompanyProfile_Phone_Number,Company->PhoneNumber);
    map.insert(CompanyProfile_WebSite,Company->webSite);
    map.insert(CompanyProfile_TRN,Company->trn);
    map.insert(CompanyProfile_FssaiLicense,Company->fssaiLicenseNo);

    map.insert(CompanyProfile_Logo,Company->logo);
    map.insert(CompanyProfile_Manager_Name,Company->ManagerName);
    map.insert(CompanyProfile_Currency,Company->Currency);
    map.insert(CompanyProfile_Bill_Tag_Line,Company->billTagLine);
    map.insert(CompanyProfile_City,Company->city);
    map.insert(CompanyProfile_Country,Company->country);
    map.insert(CompanyProfile_LedgerId,Company->ledgerId);
    map.insert(CompanyProfile_Location,Company->location);
    map.insert(CompanyProfile_Building,Company->Building);
    map.insert(CompanyProfile_POBox,Company->POBox);
    map.insert(CompanyProfile_Street,Company->Street);
    map.insert(CompanyProfile_Fax,Company->Fax);
    map.insert(CompanyProfile_Date_Of_Establishment,Company->DateOfEstablishment);
    map.insert(CompanyProfile_Mobile_Number,Company->mobileNumber);
    map.insert(CompanyProfile_Notes,Company->notes);
    map.insert(CompanyProfile_License_Number,Company->LicenseNumber);
    map.insert(CompanyProfile_toMail,Company->toMailAddresses);
    map.insert(CompanyProfile_UseBom,Company->useBomForStockCalc);
    map.insert(CompanyProfile_Installation_Date,Company->InstallationDate);

    QMap<QString,QVariant> ClauseMap;
    ClauseMap.insert(CompanyProfile_id,oldID);

    startTransaction();
    db->updateRecordsPlus(map,ClauseMap,CompanyProfile_Tablename);
    commit();

}

int CompanyProfileDataBaseHelper::deleteCompany(int oldID)
{
    QMap<QString,QVariant> ClauseMap;
    ClauseMap.insert(CompanyProfile_id,oldID);
    db->deleteRecordsPlus(ClauseMap,CompanyProfile_Tablename);

}

int CompanyProfileDataBaseHelper::commit()
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

QList<QStringList> CompanyProfileDataBaseHelper::getAvailableCompanies()
{
   QString qStr;

   QStringList dbNames;

   qStr = "show databases";
   QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

   if(query.exec(qStr)){
       while (query.next()) {
//           qDebug()<<query.value(0).toString();
           if(query.value(0).toString().startsWith(""))
               dbNames<<query.value(0).toString();
       }
   }
   else{
       qDebug()<<Q_FUNC_INFO<<query.lastError();
   }

   QList<QStringList> CompNames;
   for(int i=0;i<dbNames.length();i++){
       QStringList data;
       qStr ="select "+CompanyProfile_Company_Name + " , ";
       qStr += CompanyProfile_Branch+" ";
       qStr += " from "+dbNames[i]+"."+CompanyProfile_Tablename;
//       qStr += " where (select )";
       if(query.exec(qStr)){
           while (query.next()) {
                   data<<dbNames[i];
                   data<<query.value(0).toString() + " - "+ query.value(1).toString();
//                    qDebug()<<"Db Detail : "<<data;
                   CompNames.append(data);
           }
       }
       else{
           qDebug()<<query.lastError();
       }
   }
   qDebug()<<"Returnong count #: "<<CompNames.length();
   return CompNames;

}
