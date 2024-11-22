#include "uomdatamodel.h"

UomDataModel::UomDataModel()
{

}

UomDataModel::~UomDataModel()
{

}

QJsonObject UomDataModel::ObjToJson(UomDataModel obj)
{
    QJsonObject jsonObject;
    qDebug()<<Q_FUNC_INFO<<obj._id;
    jsonObject.insert("Uom_id",obj._id);
    jsonObject.insert("UomName",obj.UomName);
    jsonObject.insert("UomSymbol",obj.UomSymbol);
    jsonObject.insert("compoundIsTrue",obj.compoundIsTrue);
    jsonObject.insert("decimalPoints",obj.decimalPoints);
    jsonObject.insert("convRate",obj.convRate);
    jsonObject.insert("baseItem",obj.baseItem);
    jsonObject.insert("compundName",obj.compundName);
    jsonObject.insert("Narration",obj.Narration);
    qDebug()<<jsonObject;
    return jsonObject;
}

QJsonObject UomDataModel::ObjPtrToJson(UomDataModel *obj)
{
    QJsonObject jsonObject;

        jsonObject.insert("Uom_id",obj->_id);
        jsonObject.insert("UomName",obj->UomName);
        jsonObject.insert("UomSymbol",obj->UomSymbol);
        jsonObject.insert("compoundIsTrue",obj->compoundIsTrue);
        jsonObject.insert("decimalPoints",obj->decimalPoints);
        jsonObject.insert("convRate",obj->convRate);
        jsonObject.insert("baseItem",obj->baseItem);
        jsonObject.insert("compundName",obj->compundName);
        jsonObject.insert("Narration",obj->Narration);

        return jsonObject;
}


UomDataModel UomDataModel::JsonToObj(QJsonObject json)
{
    UomDataModel obj;

    obj._id = json["Uom_id"].toString();
    obj.UomName = json["UomName"].toString();
    obj.UomSymbol = json["UomSymbol"].toString();
    obj.compoundIsTrue = json["compoundIsTrue"].toBool();
    obj.decimalPoints = json["decimalPoints"].toInt();
    obj.convRate = json["convRate"].toVariant().toFloat();
    obj.baseItem = json["baseItem"].toString();
    obj.compundName = json["compundName"].toString();
    obj.Narration = json["Narration"].toString();

    qDebug()<<Q_FUNC_INFO<<__LINE__<<obj._id<<json["Uom_id"]<<json;

    return obj;
}

UomDataModel *UomDataModel::JsonToObjPtr(QJsonObject json)
{
    UomDataModel *obj = new UomDataModel();

    obj->_id = json["Uom_id"].toString();
    obj->UomName = json["UomName"].toString();
    obj->UomSymbol = json["UomSymbol"].toString();
    obj->compoundIsTrue = json["compoundIsTrue"].toBool();
    obj->decimalPoints = json["decimalPoints"].toInt();
    obj->convRate = json["convRate"].toVariant().toFloat();
    obj->baseItem = json["baseItem"].toString();
    obj->compundName = json["compundName"].toString();
    obj->Narration = json["Narration"].toString();

    return obj;
}
