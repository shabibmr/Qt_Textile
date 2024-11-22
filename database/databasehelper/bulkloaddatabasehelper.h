#ifndef BULKLOADDATABASEHELPER_H
#define BULKLOADDATABASEHELPER_H

#include "databasevalues.h"

#include <QMap>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QSqlQuery>
#include <QJsonArray>
#include <QDate>
#include <QSqlQueryModel>

class BulkLoadDatabaseHelper
{
public:
    BulkLoadDatabaseHelper(QString selectString =DatabaseValues::connectionString);
    ~BulkLoadDatabaseHelper();
    QSqlDatabase db;
    QSqlDatabase *dbArray;

    QMap<QString,QString> dataList;


    DatabaseValues *dv;
    static QString clientId;

    static QJsonObject x;
    static QJsonArray* logJson;

    QString logErr;
    static bool transFlag;

    static QString errMSG;

    QMap<QString, int> tableMap;


    void createConnection(){
        DatabaseValues *dv = new DatabaseValues();
        dv->openDB();

    }

    void startTransaction();
    bool rollBackTransaction();
    bool commitTransaction();
    void clearJsonArray(QJsonArray array);
    QString escapeJson(QString jsonString);


    qint64 createInsertQuery(QMap<QString,QVariant> map,QString tablename);

    static QMap<QString, QVariant> runDML(QString qStr, QString ls = DatabaseValues::dbString);
    void writeQueryFiles();
    bool readQueryFiles();
    bool runStoredQueries();
};

#endif // BULKLOADDATABASEHELPER_H
