#ifndef LEDGERMASTERDATABASEHELPER_H
#define LEDGERMASTERDATABASEHELPER_H

#include "database/databasehelper/databasehelper.h"
#include "datamodels/accounts/ledgermasterdatamodel.h"
#include "database/inventory/pricelist/pricelistdatabasehelper.h"
#include "../accountmaster/accountgroupmasterdatabasehelper.h"
#include "database/addressbook/addressbookdatabasehelper.h"
#include "database/finance/BillwiseRecords/billwiserecordsdatabasehelper.h"
#include "database/finance/TransactionHelper/transactionhelper.h"
#include "login/loginvalues.h"

#include <QMap>
#include <QDebug>
#include <QSqlQueryModel>

class LedgerMasterDatabaseHelper
{
public:
    LedgerMasterDatabaseHelper(DatabaseHelper *db = new DatabaseHelper());
    ~LedgerMasterDatabaseHelper();

    static QString Ledger_Master_TableName;
    static QString Ledger_Master_slno;
    static QString Ledger_Master_LedgerID;
    static QString Ledger_Master_LedgerName;
    static QString Ledger_Master_LedgerGroupId;
    static QString Ledger_Master_openingBalance;
    static QString Ledger_Master_openingBalanceDate;

    static QString Ledger_Master_closingBalance;     //calulate
    static QString Ledger_Master_totalTurnover;      //calulate

    static QString Ledger_Master_LedgerType;
    static QString Ledger_Master_narration;
    static QString Ledger_Master_City;
    static QString Ledger_Master_Address;
    static QString Ledger_Master_emailAddress;
    static QString Ledger_Master_phoneNumber;
    static QString Ledger_Master_mobileNumber;
    static QString Ledger_Master_website;

    static QString Ledger_Master_ContactPersonName;
    static QString Ledger_Master_ContactPersonNumber;
    static QString Ledger_Master_PoBox;
    static QString Ledger_Master_Country;
    static QString Ledger_Master_TRN;
    static QString Ledger_Master_Fax;
    static QString Ledger_Master_ParentCompany ;

    static QString Ledger_Master_Type_Of_Supply ;
    static QString Ledger_Master_Tax_Rate ;
    static QString Ledger_Master_Default_Tax_Ledger;

    static QString Ledger_Master_defaultPriceList;
    static QString Ledger_Master_DBName;
    static QString Ledger_Master_State ;
    static QString Ledger_Master_Birth_Date;
    static QString Ledger_Master_Credit_Period;
    static QString Ledger_Master_Credit_Limit;
    static QString Ledger_Master_isIndividual;
    
    static QString Ledger_Master_isFrequent;
    static QString Ledger_Master_DiscountPercentage;

    //8-10
    static QString Ledger_Master_Last_Updated;
    static QString Ledger_Master_Distance;
    static QString Ledger_Master_DeliveryRatePerKM;




    bool flag = true;
    DatabaseHelper *db;

    int startTransaction();

    int insertLedgerMasterItem(LedgerMasterDataModel *obj,int ContactId=0);

    int updateLedgerMasterItem(LedgerMasterDataModel *obj, QString oldID);

    int deleteLedgerMasterItem(QString oldID);

    int commit();

    int updateLedgerTypeByGroup(QString GroupId, QString LedgerType);

    QString getMaxID();

    QString getLedgerNameByID(QString id);

    QString getLedgerIDByName(QString ledgerName);

    QList<LedgerMasterDataModel> getAllLedgers();

    QSqlQueryModel *getAllLedgersModel();

    QList<QSharedPointer<LedgerMasterDataModel>> getAllLedgersAsPtr();

    QList<LedgerMasterDataModel> getAllLedgersUnderGroup(QString groupID);
    QList<LedgerMasterDataModel> getAllLedgersUnderGroupRecursive(QString groupID);

    LedgerMasterDataModel getLedgerObjectByID(QString id);

    LedgerMasterDataModel *getLedgerPtrById(QString id);

    QMap<QString,QString> getLedgersAsMap();

    QList<LedgerMasterDataModel> getAllLedgersUnderGroupByName(QString groupName);

    LedgerMasterDataModel getLastInsertedLedger();

    QSqlQueryModel* getLedgerQueryString(QStringList groupList= QStringList());

    static QSqlQueryModel *getLedgerQueryModel(QString groupID="");

    void updateOpeningValue(QString ledgerID,float OpeningValue );
    QStringList getLedgersListHavingDbName();
    QString getDbNameByLedgerName(QString ledgerName);


    QSqlQueryModel *getLedgersUnderGroups(QStringList groupList);


    QSqlQueryModel *getLedForModeofPay();

    QString getAllLedgerIDsUnderGroupRecursiveAsString(QString groupID);
    QStringList getDbNamesList();
};

#endif // LEDGERMASTERDATABASEHELPER_H
