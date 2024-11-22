#include "requirementvoucherdataobject.h"

//QJsonArray RequirementVoucherDataObject::ReqVoucherList;

QString RequirementVoucherDataObject::voucherNo;
QString RequirementVoucherDataObject::voucherType;
QString RequirementVoucherDataObject::voucherPrefix;
QString RequirementVoucherDataObject::mappingType;

RequirementVoucherDataObject::RequirementVoucherDataObject()
{

}

QJsonObject RequirementVoucherDataObject::AddVoucherToVoucherMapping(QString voucherNo, QString voucherPrefix, QString voucherType, QString desc)
{
    QJsonObject req;
    req.insert("MappingType", desc);
    req.insert("VoucherNo", voucherNo);
    req.insert("VoucherType", voucherType);
    req.insert("VoucherPrefix", voucherPrefix);
    return req;
}

QMap<QString, QString> RequirementVoucherDataObject::GetVoucherToVoucherMappingByType(QJsonArray reqVoucherList, QString mapType)
{
    QMap<QString, QString> map;

    for(QJsonValue json:reqVoucherList){

        QJsonObject mapObj = json.toObject();

        if(mapObj["MappingType"].toString() == mapType){

            map.insert("MappingType", mapObj["MappingType"].toString());
            map.insert("VoucherNo", mapObj["VoucherNo"].toString());
            map.insert("VoucherType", mapObj["VoucherType"].toString());
            map.insert("VoucherPrefix", mapObj["VoucherPrefix"].toString());

        }
    }

    return map;

}

//void RequirementVoucherDataObject::GetVoucherToVoucherMappingByType(QJsonArray reqVoucherList, QString mapType)
//{

//    for(QJsonValue map:reqVoucherList){

//        QJsonObject mapObj = map.toObject();

//        if(mapObj["MappingType"].toString() == mapType){
//            mappingType = mapObj["MappingType"].toString();
//            voucherNo = mapObj["VoucherNo"].toString();
//            voucherType = mapObj["VoucherType"].toString();
//            voucherPrefix = mapObj["VoucherPrefix"].toString();
//        }
//    }

//}


