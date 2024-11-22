#ifndef ACCOUNTGROUPDATAMODEL_H
#define ACCOUNTGROUPDATAMODEL_H
#include <QString>
#include <QJsonObject>
#include <QJsonArray>


class AccountGroupDataModel
{
public:
    AccountGroupDataModel();

    QString groupName;
    QString groupNameArabic;
    QString groupID;
    QString groupType;
    QString groupAlias;
    QString category;
    QString parentGroupId;
    QString parentGroupName;

    bool fromExternal = false;
    int action;

    QJsonObject objToJson(AccountGroupDataModel obj);
    AccountGroupDataModel JsonToObj(QJsonObject json);
    AccountGroupDataModel *JsonToObjPtr(QJsonObject json);
    QJsonObject objPtrToJson(AccountGroupDataModel *obj);
};

#endif // ACCOUNTGROUPDATAMODEL_H
