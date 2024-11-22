#ifndef DATABASEHELPER_H
#define DATABASEHELPER_H

#include "databasevalues.h"

#include <QMap>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QSqlQuery>
#include <QJsonArray>
#include <QDate>
#include <QSqlQueryModel>


template<typename TableModel>
struct EagerlyLoaded : public TableModel {
    static_assert(std::is_base_of<QAbstractTableModel, TableModel>::value,
    "EagerlyLoaded works only with QAbstractTableModel subclasses");
    explicit EagerlyLoaded(QObject* parent = nullptr):TableModel(parent) {
        QObject::connect(this, &QAbstractTableModel::modelReset,
                         [this]{ while(this->canFetchMore()) this->fetchMore(); });
    }
};


class DatabaseHelper
{

public:


//    DatabaseHelper();
//    explicit DatabaseHelper(QString selectString =DatabaseValues::connectionString,QObject *parent=Q_NULLPTR);
    DatabaseHelper(QString selectString =DatabaseValues::connectionString);
     ~DatabaseHelper();
    static QSqlDatabase db;
    QSqlDatabase *dbArray;

    DatabaseValues *dv;
    static QString clientId;

    static QJsonObject x;
    static QJsonArray* logJson;

    static QString logErr;
    static bool transFlag;

    static QString errMSG;


    void createConnection(){
        DatabaseValues *dv = new DatabaseValues();
        dv->openDB();

    }

    void startTransaction();
    bool rollBackTransaction();
    bool commitTransaction();
    void clearJsonArray(QJsonArray array);

    int insertRecords(QMap<QString,QString> map,QString tablename);
    qint64 insertRecordsPlus(QMap<QString,QVariant> map,QString tablename);

    static qint64 insertRecordsPlusStatic(QMap<QString,QVariant> map,QString tablename);


    void updateRecords(QMap<QString,QString> data,QMap<QString,QString> clause,QString tablename);
    int updateRecordsPlus(QMap<QString,QVariant> data,QMap<QString,QVariant> clause,QString tablename);

    static int updateRecordsStatic(QMap<QString,QVariant> data,QMap<QString,QVariant> clause,QString tablename);

    void deleteRecords(QMap<QString,QString> clause,QString tablename);
    void deleteRecordsPlus(QMap<QString,QVariant> clause,QString tablename);
    static void deleteRecordsStatic(QMap<QString,QVariant> clause,QString tablename);


    static QSqlQueryModel *getQueryModel(QString qStr,QString ls = DatabaseValues::connectionString);

    static QSqlQueryModel *getGMQueryModel(QString qStr, QString ls= DatabaseValues::connectionString);

    QString getDateString(QDate date);
    QString getDateTimeString(QDateTime dateTime);

    static QString getDateStringStatic(QDate date);

    static QString getDateTimeStringStatic(QDateTime dateTime);


    static QVariant getSingletonQueryResult(QString qStr,QString ls = DatabaseValues::connectionString);
    static QMap<QString, QVariant> executeDDL(QString qStr,QString ls = DatabaseValues::connectionString);

    static QString getUUID();

    static QString escapeJson(QString jsonString);

    int createVoucherNoProcedure();

    QMap<QString, QVariant> getSingletonQueryResultandError(QString qStr, QString ls = DatabaseValues::connectionString);

    static QMap<QString, QVariant> getSingletonQueryResultandErrorStatic(QString qStr, QString ls = DatabaseValues::connectionString);
    static QMap<QString, QVariant> runDML(QString qStr, QString ls = DatabaseValues::connectionString);
    static void StartTransaction();

    static QString getTransactionID(QString vPrefix);
    static bool CommitTransaction();
    static bool RollBackTransaction();
    static QSqlQueryModel *getQueryModelL(QString qStr, QString ls= DatabaseValues::connectionString);

    static QString jsonToString(QJsonObject json);
    static QJsonObject strToJson(QString str);

    static bool checkDbExists(QString dbName);
    QString escapeJson2(QString jsonString);

     static bool CommitAlgoConfigTransaction();
private:
    static QString selectString;
};

#endif // DATABASEHELPER_H
