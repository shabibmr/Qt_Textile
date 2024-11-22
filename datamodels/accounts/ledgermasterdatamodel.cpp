#include "ledgermasterdatamodel.h"

LedgerMasterDataModel::LedgerMasterDataModel()
{
    openingBalanceDate = QDate::fromString("2000-01-01","yyyy-MM-dd");
    Birth_Date = QDate::fromString("2000-01-01","yyyy-MM-dd");
    voucherDate = QDate::fromString("2000-01-01","yyyy-MM-dd");
    timestamp = QDateTime::fromString("2000-01-01","yyyy-MM-dd");
}

QJsonObject LedgerMasterDataModel::ObjToJson(LedgerMasterDataModel obj)
{
    QJsonObject jsonObject;
//    qDebug()<<"LEDGER OBJECT TO JSON";

    jsonObject.insert("Ledger_Id",obj.LedgerID);
    jsonObject.insert("Ledger_Name",obj.LedgerName);
    jsonObject.insert("ledgerNameArabic",obj.ledgerNameArabic);
    jsonObject.insert("Group_Id",obj.LedgerGroupId);
    jsonObject.insert("Ledger_Group_Name",obj.LedgerGroupName);
//    jsonObject.insert("openingBalance",obj.openingBalance);
//    jsonObject.insert("openingBalanceDate",obj.openingBalanceDate.toString());
//    jsonObject.insert("closingBalance",obj.closingBalance);
//    jsonObject.insert("totalTurnover",obj.totalTurnover);
    jsonObject.insert("LedgerType",obj.LedgerType);
    jsonObject.insert("narration",obj.narration);
    jsonObject.insert("City",obj.City);
    jsonObject.insert("Address",obj.Address);
    jsonObject.insert("emailAddress",obj.emailAddress);
    jsonObject.insert("phoneNumber",obj.phoneNumber);
    jsonObject.insert("fax",obj.fax);
    jsonObject.insert("parentCompany",obj.parentCompany);
    jsonObject.insert("mobileNumber",obj.mobileNumber);
    jsonObject.insert("website",obj.website);
    jsonObject.insert("ContactPersonName",obj.ContactPersonName);
    jsonObject.insert("ContactPersonNumber",obj.ContactPersonNumber);
    jsonObject.insert("ContactPersonEmail",obj.ContactPersonEmail);
    jsonObject.insert("PoBox",obj.PoBox);
    jsonObject.insert("Country",obj.Country);
    jsonObject.insert("TRN",obj.TRN);
    jsonObject.insert("defaultPriceListID",obj.defaultPriceListID);
    jsonObject.insert("amount",obj.amount);
    jsonObject.insert("listid",obj.listid);
    jsonObject.insert("State",obj.State);
    jsonObject.insert("Birth_Date",obj.Birth_Date.toString());
    jsonObject.insert("Credit_Period",obj.Credit_Period);
    jsonObject.insert("isIndividual",obj.isIndividual);
    jsonObject.insert("crAmount",obj.crAmount);
    jsonObject.insert("drAmount",obj.drAmount);
    jsonObject.insert("voucherNo",obj.voucherNo);
    jsonObject.insert("voucherDate",obj.voucherDate.toString());
    jsonObject.insert("voucherType",obj.voucherType);
    jsonObject.insert("voucherPrefix",obj.voucherPrefix);
    jsonObject.insert("timestamp",obj.timestamp.toString());
    jsonObject.insert("isInvoiceItem",obj.isInvoiceItem);
    jsonObject.insert("DefaultTaxLedger",obj.DefaultTaxLedger);
    jsonObject.insert("TypeOfSupply",obj.TypeOfSupply);
//    jsonObject.insert("taxRate",obj.taxRate);
    jsonObject.insert("AgainstLedger",obj.AgainstLedger);
    jsonObject.insert("hasBillwiseMappings",obj.hasBillwiseMappings);
    jsonObject.insert("DbName", obj.dbName);

    return jsonObject;

}

QJsonObject LedgerMasterDataModel::ObjPtrToJson(LedgerMasterDataModel *obj)
{
    QJsonObject jsonObject;
//    qDebug()<<"LEDGER OBJECT TO JSON";

//    jsonObject.insert("LedgerID",obj->LedgerID);
//    jsonObject.insert("LedgerName",obj->LedgerName);
//    jsonObject.insert("ledgerNameArabic",obj->ledgerNameArabic);
//    jsonObject.insert("LedgerGroupId",obj->LedgerGroupId);
//    jsonObject.insert("LedgerGroupName",obj->LedgerGroupName);
//    jsonObject.insert("openingBalance",obj->openingBalance);
//    jsonObject.insert("openingBalanceDate",obj->openingBalanceDate.toString());
//    jsonObject.insert("closingBalance",obj->closingBalance);
//    jsonObject.insert("totalTurnover",obj->totalTurnover);
//    jsonObject.insert("LedgerType",obj->LedgerType);
//    jsonObject.insert("narration",obj->narration);
//    jsonObject.insert("City",obj->City);
//    jsonObject.insert("Address",obj->Address);
//    jsonObject.insert("emailAddress",obj->emailAddress);
//    jsonObject.insert("phoneNumber",obj->phoneNumber);
//    jsonObject.insert("fax",obj->fax);
//    jsonObject.insert("parentCompany",obj->parentCompany);
//    jsonObject.insert("mobileNumber",obj->mobileNumber);
//    jsonObject.insert("website",obj->website);
//    jsonObject.insert("ContactPersonName",obj->ContactPersonName);
//    jsonObject.insert("ContactPersonNumber",obj->ContactPersonNumber);
//    jsonObject.insert("ContactPersonEmail",obj->ContactPersonEmail);
//    jsonObject.insert("PoBox",obj->PoBox);
//    jsonObject.insert("Country",obj->Country);
//    jsonObject.insert("TRN",obj->TRN);
//    jsonObject.insert("defaultPriceListID",obj->defaultPriceListID);
//    jsonObject.insert("amount",obj->amount);
//    jsonObject.insert("listid",obj->listid);
//    jsonObject.insert("State",obj->State);
//    jsonObject.insert("Birth_Date",obj->Birth_Date.toString());
//    jsonObject.insert("Credit_Period",obj->Credit_Period);
//    jsonObject.insert("isIndividual",obj->isIndividual);
//    jsonObject.insert("crAmount",obj->crAmount);
//    jsonObject.insert("drAmount",obj->drAmount);
//    jsonObject.insert("voucherNo",obj->voucherNo);
//    jsonObject.insert("voucherDate",obj->voucherDate.toString());
//    jsonObject.insert("voucherType",obj->voucherType);
//    jsonObject.insert("voucherPrefix",obj->voucherPrefix);
//    jsonObject.insert("timestamp",obj->timestamp.toString());
//    jsonObject.insert("isInvoiceItem",obj->isInvoiceItem);
//    jsonObject.insert("DefaultTaxLedger",obj->DefaultTaxLedger);
//    jsonObject.insert("TypeOfSupply",obj->TypeOfSupply);
//    jsonObject.insert("taxRate",obj->taxRate);
//    jsonObject.insert("AgainstLedger",obj->AgainstLedger);
//    jsonObject.insert("hasBillwiseMappings",obj->hasBillwiseMappings);
//    jsonObject.insert("DbName", obj->dbName);


    jsonObject.insert("LEDGER_ID",obj->LedgerID);
    jsonObject.insert("Ledger_Name",obj->LedgerName);
    jsonObject.insert("ledgerNameArabic",obj->ledgerNameArabic);
    jsonObject.insert("Group_Id",obj->LedgerGroupId);
    jsonObject.insert("LedgerGroupName",obj->LedgerGroupName);
    jsonObject.insert("openingBalance",obj->openingBalance);
    jsonObject.insert("openingBalanceDate",obj->openingBalanceDate.toString());
    jsonObject.insert("Closing_Balance",obj->closingBalance);
    jsonObject.insert("Turn_Over",obj->totalTurnover);
    jsonObject.insert("Ledger_Type",obj->LedgerType);
    jsonObject.insert("Narration",obj->narration);
    jsonObject.insert("City",obj->City);
    jsonObject.insert("Address",obj->Address);
    jsonObject.insert("Email",obj->emailAddress);
    jsonObject.insert("Phone_Number",obj->phoneNumber);
    jsonObject.insert("Fax",obj->fax);
    jsonObject.insert("parentCompany",obj->parentCompany);
    jsonObject.insert("Mobile_Number",obj->mobileNumber);
    jsonObject.insert("Website",obj->website);
    jsonObject.insert("Contact_Person_Name",obj->ContactPersonName);
    jsonObject.insert("Contant_Person_Number",obj->ContactPersonNumber);
    jsonObject.insert("ContactPersonEmail",obj->ContactPersonEmail);
    jsonObject.insert("PoBox",obj->PoBox);
    jsonObject.insert("Country",obj->Country);
    jsonObject.insert("TRN",obj->TRN);
    jsonObject.insert("defaultPriceListID",obj->defaultPriceListID);
    jsonObject.insert("amount",obj->amount);
    jsonObject.insert("listid",obj->listid);
    jsonObject.insert("State",obj->State);
    jsonObject.insert("Birth_Date",obj->Birth_Date.toString());
    jsonObject.insert("Credit_Period",obj->Credit_Period);
    jsonObject.insert("isIndividual",obj->isIndividual);
    jsonObject.insert("crAmount",obj->crAmount);
    jsonObject.insert("drAmount",obj->drAmount);
    jsonObject.insert("Voucher_No",obj->voucherNo);
    jsonObject.insert("voucherDate",obj->voucherDate.toString());
    jsonObject.insert("voucherType",obj->voucherType);
    jsonObject.insert("Voucher_Prefix",obj->voucherPrefix);
    jsonObject.insert("TimeStamp",obj->timestamp.toString());
    jsonObject.insert("isInvoiceItem",obj->isInvoiceItem);
    jsonObject.insert("Default_Tax_Ledger",obj->DefaultTaxLedger);
    jsonObject.insert("Type_Of_Supply",obj->TypeOfSupply);
    jsonObject.insert("Tax_Rate",obj->taxRate);
    jsonObject.insert("AgainstLedger",obj->AgainstLedger);
    jsonObject.insert("hasBillwiseMappings",obj->hasBillwiseMappings);
    jsonObject.insert("DbName", obj->dbName);
    return jsonObject;

}

LedgerMasterDataModel LedgerMasterDataModel::JsonToObj(QJsonObject json)
{
    LedgerMasterDataModel obj;

//    qDebug()<<"LEDGER JSON TO OBJECT";
    obj.LedgerID = json["LedgerID"].toString();
    obj.LedgerName = json["LedgerName"].toString();
    obj.ledgerNameArabic = json["ledgerNameArabic"].toString();
    obj.LedgerGroupId = json["LedgerGroupId"].toString();
    obj.LedgerGroupName = json["LedgerGroupName"].toString();
    obj.openingBalance = json["openingBalance"].toVariant().toFloat();
    obj.openingBalanceDate = json["openingBalanceDate"].toVariant().toDate();
    obj.closingBalance = json["closingBalance"].toVariant().toFloat();
    obj.totalTurnover = json["totalTurnover"].toVariant().toFloat();
    obj.LedgerType = json["LedgerType"].toString();
    obj.narration = json["narration"].toString();
    obj.City = json["City"].toString();
    obj.Address = json["Address"].toString();
    obj.emailAddress = json["emailAddress"].toString();
    obj.phoneNumber = json["phoneNumber"].toString();
    obj.fax = json["fax"].toString();
    obj.parentCompany = json["parentCompany"].toString();
    obj.mobileNumber = json["mobileNumber"].toString();
    obj.website = json["website"].toString();
    obj.ContactPersonName = json["ContactPersonName"].toString();
    obj.ContactPersonNumber = json["ContactPersonNumber"].toString();
    obj.ContactPersonEmail = json["ContactPersonEmail"].toString();
    obj.PoBox = json["PoBox"].toString();
    obj.Country = json["Country"].toString();
    obj.TRN = json["TRN"].toString();
    obj.defaultPriceListID = json["defaultPriceListID"].toString();
    obj.amount = json["amount"].toVariant().toFloat();
    obj.listid = json["listid"].toInt();
    obj.State = json["State"].toString();
    obj.Birth_Date = json["Birth_Date"].toVariant().toDate();
    obj.Credit_Period = json["Credit_Period"].toInt();
    obj.isIndividual = json["isIndividual"].toBool();
    obj.crAmount = json["crAmount"].toVariant().toFloat();
    obj.drAmount = json["drAmount"].toVariant().toFloat();
    obj.voucherNo = json["voucherNo"].toString();
    obj.voucherDate = json["voucherDate"].toVariant().toDate();
    obj.voucherType = json["voucherType"].toString();
    obj.voucherPrefix = json["voucherPrefix"].toString();
    obj.timestamp = json["timestamp"].toVariant().toDateTime();
    obj.isInvoiceItem = json["isInvoiceItem"].toBool();
    obj.DefaultTaxLedger = json["DefaultTaxLedger"].toString();
    obj.TypeOfSupply = json["TypeOfSupply"].toString();
    obj.taxRate = json["taxRate"].toVariant().toFloat();
    obj.AgainstLedger = json["AgainstLedger"].toString();
    obj.hasBillwiseMappings = json["hasBillwiseMappings"].toBool();
    obj.dbName = json["DbName"].toString();
    return obj;

}

LedgerMasterDataModel *LedgerMasterDataModel::JsonToObjPtr(QJsonObject json)
{

    LedgerMasterDataModel *obj = new LedgerMasterDataModel();

//    qDebug()<<"LEDGER JSON TO OBJECT";
    obj->LedgerID = json["LedgerID"].toString();
    obj->LedgerName = json["LedgerName"].toString();
    obj->ledgerNameArabic = json["ledgerNameArabic"].toString();
    obj->LedgerGroupId = json["LedgerGroupId"].toString();
    obj->LedgerGroupName = json["LedgerGroupName"].toString();
    obj->openingBalance = json["openingBalance"].toVariant().toFloat();
    obj->openingBalanceDate = json["openingBalanceDate"].toVariant().toDate();
    obj->closingBalance = json["closingBalance"].toVariant().toFloat();
    obj->totalTurnover = json["totalTurnover"].toVariant().toFloat();
    obj->LedgerType = json["LedgerType"].toString();
    obj->narration = json["narration"].toString();
    obj->City = json["City"].toString();
    obj->Address = json["Address"].toString();
    obj->emailAddress = json["emailAddress"].toString();
    obj->phoneNumber = json["phoneNumber"].toString();
    obj->fax = json["fax"].toString();
    obj->parentCompany = json["parentCompany"].toString();
    obj->mobileNumber = json["mobileNumber"].toString();
    obj->website = json["website"].toString();
    obj->ContactPersonName = json["ContactPersonName"].toString();
    obj->ContactPersonNumber = json["ContactPersonNumber"].toString();
    obj->ContactPersonEmail = json["ContactPersonEmail"].toString();
    obj->PoBox = json["PoBox"].toString();
    obj->Country = json["Country"].toString();
    obj->TRN = json["TRN"].toString();
    obj->defaultPriceListID = json["defaultPriceListID"].toString();
    obj->amount = json["amount"].toVariant().toFloat();
    obj->listid = json["listid"].toInt();
    obj->State = json["State"].toString();
    obj->Birth_Date = json["Birth_Date"].toVariant().toDate();
    obj->Credit_Period = json["Credit_Period"].toInt();
    obj->isIndividual = json["isIndividual"].toBool();
    obj->crAmount = json["crAmount"].toVariant().toFloat();
    obj->drAmount = json["drAmount"].toVariant().toFloat();
    obj->voucherNo = json["voucherNo"].toString();
    obj->voucherDate = json["voucherDate"].toVariant().toDate();
    obj->voucherType = json["voucherType"].toString();
    obj->voucherPrefix = json["voucherPrefix"].toString();
    obj->timestamp = json["timestamp"].toVariant().toDateTime();
    obj->isInvoiceItem = json["isInvoiceItem"].toBool();
    obj->DefaultTaxLedger = json["DefaultTaxLedger"].toString();
    obj->TypeOfSupply = json["TypeOfSupply"].toString();
    obj->taxRate = json["taxRate"].toVariant().toFloat();
    obj->AgainstLedger = json["AgainstLedger"].toString();
    obj->hasBillwiseMappings = json["hasBillwiseMappings"].toBool();
    obj->dbName = json["DbName"].toString();
    return obj;
}
