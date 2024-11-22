#ifndef DATABASEVALUES_H
#define DATABASEVALUES_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

namespace gmdb{
    class DatabaseHelper;
}

class DatabaseValues
{

public:

    DatabaseValues();
    ~DatabaseValues();

    static const QString masterDbNameE;

    QSqlDatabase db;
    static QString dbName ;
    static int dbPort;
    static QString dbUser;
    static QString dbPassword;
    static QString dbHost;

    static QString ClientID;
    static QString voucherPrefix;

    static bool isPOS;
    static bool configDbStatus ;

    static QString idGen;

    void openDB();
    void dropDB();


    static QString dbString;
    static QString connectionString;
//    static QString masters_dbName;

    void addDbConnection(QString dbName, QString connStr);
    void removeDbConnection(QString connStr);


    static QString getMasterDbNameE();
    static void setDbName(const QString &value);
    static void setConnectionString(const QString &value);
    static void setIsPOS(bool value);
    static void setConfigDbStatus(bool newConfigDbStatus);
};

#endif // DATABASEVALUES_H
