#ifndef CATEGORYDATAMODEL_H
#define CATEGORYDATAMODEL_H

#include <QString>
#include <QJsonObject>
#include <QJsonArray>

class CategoryDataModel
{
public:
    CategoryDataModel();
    int CategoryID = 0;
    QString CategoryName;
    QString CategoryDesc;

    bool fromExternal = false;
    int action;

    QJsonObject objToJson(CategoryDataModel obj);
    QJsonObject objPtrToJson(CategoryDataModel *obj);
    CategoryDataModel JsonToObj(QJsonObject json);
    CategoryDataModel *JsonToObjPtr(QJsonObject json);
};

#endif // CATEGORYDATAMODEL_H
