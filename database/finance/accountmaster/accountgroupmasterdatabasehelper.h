#ifndef ACCOUNTGROUPMASTERDATABASEHELPER_H
#define ACCOUNTGROUPMASTERDATABASEHELPER_H

#include "datamodels/accounts/accountgroupdatamodel.h"
#include "database/databasehelper/databasehelper.h"

#include <QMap>
#include <QDebug>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"
#include "login/loginvalues.h"


class AccountGroupMasterDatabaseHelper
{
public:
    AccountGroupMasterDatabaseHelper(DatabaseHelper *db = new DatabaseHelper());
    ~AccountGroupMasterDatabaseHelper();

    QList<AccountGroupDataModel> getAllAccounts();

    AccountGroupDataModel getGroupById(QString id);
    AccountGroupDataModel *getGroupPtrById(QString id);

    QString getGroupNameByID(QString id);

    QString getGroupIDByName(QString Name);

    static QString Accounts_Group_TableName ;

    QString Accounts_Group_id="_id";
    QString Accounts_Group_GroupName = "Group_Name";
    QString Accounts_Group_GroupId = "Group_Id";
    QString Accounts_Group_ParentGroupID = "Parent_ID";
    QString Accounts_Group_GroupType="Group_Type";
    QString Accounts_Group_Category="Group_Category";
    QString Accounts_Group_Default="DefaultRecord";

    QString Accounts_Group_Last_Modified="Last_Modified";


    bool flag = true;
    DatabaseHelper *db;

    int startTransaction();

    int insertAccountGroup(AccountGroupDataModel *obj);

    int updateAccountGroup(AccountGroupDataModel *obj, QString oldID);

    int deleteAccountGroup(QString oldID);

    QMap<QString,QString> getAllGroupsAsMap();

    QString getMaxID();

    QString getGroupType(QString groupID);

    bool isDefaultRecord(QString groupID);

    int commit();

    QSqlQueryModel *getAllAccountForSearchModel();

    QSqlQueryModel *getAllAccountsModel();

    QList<AccountGroupDataModel> getPrimaryAccounts();
    QList<AccountGroupDataModel> getPrimaryAccountsByGroupType(QString groupType);

    QList<AccountGroupDataModel> getAccountsUnderParentGroup(QString groupId);
    QList<AccountGroupDataModel> getAccountsUnderParentGroupRecursive(QString groupId);

    QStringList  getAccountsUnderParentGroupRecurStrList(QString groupId);

    QMap<QString, QString> getAccountParentGroupAsMap(QString groupID);
};

#endif // ACCOUNTGROUPMASTERDATABASEHELPER_H
