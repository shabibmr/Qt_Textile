#ifndef UOMDATAMODEL_H
#define UOMDATAMODEL_H
#include <QDebug>
#include <QJsonObject>

class UomDataModel
{
public:
    UomDataModel();
    ~UomDataModel();

    QString _id;
    QString UomName;
    QString UomSymbol;
    bool compoundIsTrue;
    int decimalPoints=0;
    float convRate=1;
    QString barCode;
    QString baseItem;
    QString compundName;
    QString Narration;

    bool fromExternal = false;
    int action;

    QJsonObject ObjToJson(UomDataModel obj);
    QJsonObject ObjPtrToJson(UomDataModel *obj);
    UomDataModel JsonToObj(QJsonObject json);
    UomDataModel *JsonToObjPtr(QJsonObject json);

};
Q_DECLARE_METATYPE(UomDataModel)

#endif // UOMDATAMODEL_H
