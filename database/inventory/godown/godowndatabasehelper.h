#ifndef GODOWNDATABASEHELPER_H
#define GODOWNDATABASEHELPER_H

#include "datamodels/inventory/godown/godowndataobject.h"

#include "database/databasehelper/databasehelper.h"

#include <QMap>
#include <QDebug>
#include "login/loginvalues.h"

class GodownDatabaseHelper
{
public:
    GodownDatabaseHelper(DatabaseHelper *db = new DatabaseHelper());
    ~GodownDatabaseHelper();

    static QString Godown_Tablename ;
    static QString Godown_id             ;
    static QString Godown_GodownID       ;
    static QString GodownName            ;
    static QString GodownLocation        ;
    static QString GodownNarration       ;
    static QString Godown_IsProfitCentre ;
    static QString Godown_IsDefault      ;
    static QString Godown_IsProduction   ;
    static QString Godown_Last_Modified;



    bool flag = true;
    int startTransaction();
    int commit();

    DatabaseHelper *db;

    int insertGodown(GodownDataObject *obj);
    int updateGodown(GodownDataObject *obj,QString id);
    int deleteGodown(QString id);

    QString getMaxID();
    QList<GodownDataObject*> getAllGodowns();
    QMap<QString, QString> getGodownForSearchAsMap();
    QString getGodownNameById(QString godownId);
    GodownDataObject* getGodownById(QString godownId);
    QList<GodownDataObject *> getAllProfitCentres();
    QSqlQueryModel *getGodownForSearchModel();
    GodownDataObject getGodownObjById(QString godownId);
    QString getGodownIdByName(QString godownName);
    static QString getDefaultGodown();

};

#endif // GODOWNDATABASEHELPER_H
