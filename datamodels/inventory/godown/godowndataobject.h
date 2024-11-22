#ifndef GODOWNDATAOBJECT_H
#define GODOWNDATAOBJECT_H

#include <QString>
#include <QJsonObject>
#include <QJsonArray>

class GodownDataObject
{
public:
    QString GodownID;
    QString GodownName;
    QString GodownLocation;
    QString GodownNarration;
    bool isProfitCentre = false;
    bool isDefault;
    bool isProductionCentre = false;

    bool fromExternal=false;
    int action;


    GodownDataObject();
    ~GodownDataObject();

    QJsonObject objToJson(GodownDataObject obj);
    QJsonObject objPtrToJson(GodownDataObject *obj);
    GodownDataObject JsonToObj(QJsonObject json);
    GodownDataObject *JsonToObjPtr(QJsonObject json);
};

#endif // GODOWNDATAOBJECT_H
