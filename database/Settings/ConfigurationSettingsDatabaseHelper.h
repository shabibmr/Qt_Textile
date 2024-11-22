#ifndef CONFIGURATIONSETTINGSDATABASEHELPER_H
#define CONFIGURATIONSETTINGSDATABASEHELPER_H

#include "database/databasehelper/databasehelper.h"
#include "configsettingsvariables.h"

class ConfigurationSettingsDatabaseHelper
{
public:
    ConfigurationSettingsDatabaseHelper(DatabaseHelper *db = new DatabaseHelper(DatabaseValues::connectionString));
//    ConfigurationSettingsDatabaseHelper();
    ~ConfigurationSettingsDatabaseHelper();
    static QVariant getVariableValue(QString key);

    static QString Config_Settings_TableName ;
    static QString Algo_Config_Settings_TableName ;

    static QString Config_Settings_id          ;
    static QString Config_Settings_Key         ;
    static QString Config_Settings_Value       ;
    static QString Config_Settings_TimeStamp   ;
    static QString Config_Settings_Data_Type   ;
    static QString Config_Settings_TextToDisplay;
    static QString Config_Settings_ValueType ;
    static QString Config_Settings_PossibleValues ;
    static QString Config_Settings_IsLOgged ;

    static bool upsertSettings(QString key,QVariant value);
    static bool checkKeyExist(QString key);
    bool deleteSettings(QString key);

    bool checkConfigTableExists();
    bool createConfigTable();

    static QVariant getValue(QString key,QVariant defaultValue);
    void setDb(DatabaseHelper *value);

    static QVariant getAlgoConfigValue(QString key, QVariant value);
    static QVariant getAlgoConfigVariableValue(QString key);
    static bool upsertAlgoConfigSettings(QString key, QVariant value);
    bool checkAlgoConfigTableExists();
    static bool checkAlgoConfigKeyExist(QString key);
private:
    DatabaseHelper *db;


};

#endif // CONFIGURATIONSETTINGSDATABASEHELPER_H
