#ifndef BRANDSDATAMODEL_H
#define BRANDSDATAMODEL_H

#include <QString>
#include <QJsonObject>
#include <QJsonArray>

class BrandsDataModel
{
public:
    BrandsDataModel();
    int brandID = 0;
    QString brandName;
    QString brandDesc;

    bool fromExternal = false;
    int action;

    QJsonObject objToJson(BrandsDataModel obj);
    QJsonObject objPtrToJson(BrandsDataModel *obj);
    BrandsDataModel JsonToObj(QJsonObject json);
    BrandsDataModel *JsonToObjPtr(QJsonObject json);
};

#endif // BRANDSDATAMODEL_H
