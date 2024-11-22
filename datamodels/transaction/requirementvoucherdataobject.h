#ifndef REQUIREMENTVOUCHERDATAOBJECT_H
#define REQUIREMENTVOUCHERDATAOBJECT_H

#include <QJsonObject>
#include <QJsonArray>
#include <QMap>


class RequirementVoucherDataObject
{
public:
    RequirementVoucherDataObject();

//    QJsonObject ReqVoucherList;

//   static QJsonArray ReqVoucherList;

   static QString voucherNo;
   static QString voucherType;
   static QString voucherPrefix;
   static QString mappingType;

    static QJsonObject AddVoucherToVoucherMapping(QString voucherNo, QString voucherPrefix, QString voucherType, QString mappingType);
//    static void GetVoucherToVoucherMappingByType(QJsonArray reqVoucherList, QString mappingType);
    QMap<QString, QString> GetVoucherToVoucherMappingByType(QJsonArray reqVoucherList, QString mappingType);
};

#endif // REQUIREMENTVOUCHERDATAOBJECT_H
