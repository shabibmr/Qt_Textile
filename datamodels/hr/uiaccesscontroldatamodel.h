#ifndef UIACCESSCONTROLDATAMODEL_H
#define UIACCESSCONTROLDATAMODEL_H

#include <QJsonObject>
#include <QMetaType>

class UiAccessControlDataModel
{
public:
    UiAccessControlDataModel();

    int _id;
    int ui_id;
    QString UiName;
    int userGroupId;
    bool allowCreate = false;
    bool allowRead = false;
    bool allowUpdate = false;
    bool allowDelete = false;
    bool allowShare = false;
    QJsonObject extras;

    QString module;
};

Q_DECLARE_METATYPE(UiAccessControlDataModel)
#endif // UIACCESSCONTROLDATAMODEL_H
