#ifndef DATABASEUPDATER_H
#define DATABASEUPDATER_H

#include "database/Settings/ConfigurationSettingsDatabaseHelper.h"
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFileDialog>
#include <QObject>
#include <QMessageBox>

//#include "databasehelper.h"

class DatabaseUpdater
{
public:
    DatabaseUpdater(ConfigurationSettingsDatabaseHelper *dbHelper = new ConfigurationSettingsDatabaseHelper);
    ~DatabaseUpdater();

    double curr_DB_version = 0;
    double new_DB_version ; // get from json FILE
    QJsonArray dbchanges;
    bool updateAlgoCOnfig = false;

    bool checkNewUpdates();

    void makeUpdates();

    bool runCommand(QString val);

    void updateVersionNumber();

    QMap<QString, QVariant> getQueriesOfVersion(double version);

    QJsonArray readDBChangesFile(QString fileName);

    void writeDBChangesFile(QString fileName, QJsonArray newEntry);

    double getLatestDbVersion(double vNo);

    bool runZerothDbUpdate();

    void setDbhelper(ConfigurationSettingsDatabaseHelper *value);

    void getAllQueries();

    void setUpdateAlgoCOnfig(bool newUpdateAlgoCOnfig);

    void updateConfigVersionNumber();
private:
    ConfigurationSettingsDatabaseHelper *dbhelper;
};

#endif // DATABASEUPDATER_H
