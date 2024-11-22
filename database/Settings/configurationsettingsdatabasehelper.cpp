#include "ConfigurationSettingsDatabaseHelper.h"
#include "database/databasehelper/databasehelper.h"

QString ConfigurationSettingsDatabaseHelper::Config_Settings_TableName ="Config_Settings";
QString ConfigurationSettingsDatabaseHelper::Algo_Config_Settings_TableName ="algo_config.algo_config_settings";

QString ConfigurationSettingsDatabaseHelper::Config_Settings_id          = "_id";
QString ConfigurationSettingsDatabaseHelper::Config_Settings_Key         = "GMKey";
QString ConfigurationSettingsDatabaseHelper::Config_Settings_Value       = "GMValue";
QString ConfigurationSettingsDatabaseHelper::Config_Settings_TimeStamp   = "Time_Stamp";
QString ConfigurationSettingsDatabaseHelper::Config_Settings_Data_Type   = "Data_Type";
QString ConfigurationSettingsDatabaseHelper::Config_Settings_TextToDisplay = "Text_To_Display";
QString ConfigurationSettingsDatabaseHelper::Config_Settings_ValueType = "Value_Type";
QString ConfigurationSettingsDatabaseHelper::Config_Settings_PossibleValues = "Possible_Values";
QString ConfigurationSettingsDatabaseHelper::Config_Settings_IsLOgged = "IsLogged";

ConfigurationSettingsDatabaseHelper::ConfigurationSettingsDatabaseHelper(DatabaseHelper *db)
//ConfigurationSettingsDatabaseHelper::ConfigurationSettingsDatabaseHelper()
{
    //    db = new DatabaseHelper();
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    this->db = db;
}

QVariant ConfigurationSettingsDatabaseHelper::getVariableValue(QString key)
{
    //    qDebug()<<"5555";
    QString qStr;
    qStr = "SELECT coalesce(`"+ Config_Settings_Value + "`,0) FROM " + Config_Settings_TableName;
    qStr+= " WHERE `";
    qStr+= Config_Settings_Key +"` ='"+key+"'";

    //    QMap<QString, QVariant> map;
    //    map = db->getSingletonQueryResultandError(qStr);

    //    if(map["Status"] == "SUCCESS")
    //        return map["Result"];
    //    else{
    //        if(map["Result"].toString().contains( Config_Settings_TableName + "' doesn't exist ", Qt::CaseInsensitive)){
    //            if(createConfigTable())
    return DatabaseHelper::getSingletonQueryResult(qStr);
    //        }

    //    }

}


QVariant ConfigurationSettingsDatabaseHelper::getAlgoConfigVariableValue(QString key)
{
    //    qDebug()<<"5555";
    QString qStr;
    qStr = "SELECT coalesce(`"+ Config_Settings_Value + "`,0) FROM " + Algo_Config_Settings_TableName;
    qStr+= " WHERE `";
    qStr+= Config_Settings_Key +"` ='"+key+"'";

    return DatabaseHelper::getSingletonQueryResult(qStr);

}


bool ConfigurationSettingsDatabaseHelper::upsertSettings(QString key, QVariant value)
{
    DatabaseHelper::StartTransaction();
    QMap<QString,QVariant> map;
    map.insert(Config_Settings_Key,key);
    map.insert(Config_Settings_Value,value);

    QMap<QString,QVariant> clause;
    clause.insert(Config_Settings_Key,key);
    if(checkKeyExist(key)){
        //UPDATE
        //        qDebug()<<"update";
        DatabaseHelper::updateRecordsStatic(map,clause,Config_Settings_TableName);
    }
    else{
        //INSERT
        //        qDebug()<<"insert";
        DatabaseHelper::insertRecordsPlusStatic(map,Config_Settings_TableName);
    }
    DatabaseHelper::CommitTransaction();
}

bool ConfigurationSettingsDatabaseHelper::upsertAlgoConfigSettings(QString key, QVariant value)
{
    DatabaseHelper::StartTransaction();
    QMap<QString,QVariant> map;
    map.insert(Config_Settings_Key,key);
    map.insert(Config_Settings_Value,value);

    QMap<QString,QVariant> clause;
    clause.insert(Config_Settings_Key,key);
    if(checkAlgoConfigKeyExist(key)){
        //UPDATE
        //        qDebug()<<"update";
        DatabaseHelper::updateRecordsStatic(map,clause,Algo_Config_Settings_TableName);
    }
    else{
        //INSERT
        //        qDebug()<<"insert";
        DatabaseHelper::insertRecordsPlusStatic(map,Algo_Config_Settings_TableName);
    }
    DatabaseHelper::CommitAlgoConfigTransaction();
}


bool ConfigurationSettingsDatabaseHelper::checkAlgoConfigKeyExist(QString key)
{
    //    qDebug()<<Q_FUNC_INFO<<__LINE__<<key;

    QString qStr;
    qStr = "SELECT 1 FROM " + Algo_Config_Settings_TableName;
    qStr+= " WHERE `";
    qStr+= Config_Settings_Key +"` ='"+key+"'";

    //    qDebug()<<"QSTR : "<<qStr;

    QMap<QString, QVariant> map = DatabaseHelper::getSingletonQueryResultandErrorStatic(qStr) ;
    //   qDebug()<<"at check key exist Map Status : "<<map["Status"] ;
    if(map["Status"].toString() == "SUCCESS")
    {
        //       qDebug()<<Q_FUNC_INFO<<__LINE__;
        //       qDebug()<<"at check key exist Map Status : "<<map["Result"] ;
        if(map["Result"].toInt() == 1)
            return true;
        else
            return false;
    }
    else
        return false;

    //    return  db->getSingletonQueryResult(qStr) != NULL;
}


bool ConfigurationSettingsDatabaseHelper::checkKeyExist(QString key)
{
    //    qDebug()<<Q_FUNC_INFO<<__LINE__<<key;

    QString qStr;
    qStr = "SELECT 1 FROM " + Config_Settings_TableName;
    qStr+= " WHERE `";
    qStr+= Config_Settings_Key +"` ='"+key+"'";

    //    qDebug()<<"QSTR : "<<qStr;

    QMap<QString, QVariant> map = DatabaseHelper::getSingletonQueryResultandErrorStatic(qStr) ;
    //   qDebug()<<"at check key exist Map Status : "<<map["Status"] ;
    if(map["Status"].toString() == "SUCCESS")
    {
        //       qDebug()<<Q_FUNC_INFO<<__LINE__;
        //       qDebug()<<"at check key exist Map Status : "<<map["Result"] ;
        if(map["Result"].toInt() == 1)
            return true;
        else
            return false;
    }
    else
        return false;

    //    return  db->getSingletonQueryResult(qStr) != NULL;
}

bool ConfigurationSettingsDatabaseHelper::deleteSettings(QString key)
{
    QMap<QString,QVariant> clause;
    clause.insert(Config_Settings_Key,key);
    db->deleteRecordsPlus(clause,Config_Settings_TableName);
}

bool ConfigurationSettingsDatabaseHelper::checkConfigTableExists()
{
    QString qStr = " SELECT " + Config_Settings_id + ", " + Config_Settings_Key + ", " + Config_Settings_Value +
            " FROM " + Config_Settings_TableName;

    QMap<QString, QVariant> map;
    map = db->getSingletonQueryResultandError(qStr);

    if(map["Status"] == "SUCCESS")
        return true;
    else
        return false;
}

bool ConfigurationSettingsDatabaseHelper::checkAlgoConfigTableExists()
{
    QString qStr = " SELECT " + Config_Settings_id + ", " + Config_Settings_Key + ", " + Config_Settings_Value +
            " FROM " + Algo_Config_Settings_TableName;

    QMap<QString, QVariant> map;
    map = db->getSingletonQueryResultandError(qStr);

    if(map["Status"] == "SUCCESS")
        return true;
    else
        return false;
}


bool ConfigurationSettingsDatabaseHelper::createConfigTable()
{
    QString qStr = " CREATE TABLE `config_settings` ( ";
    qStr += " `_id` int(11) NOT NULL AUTO_INCREMENT,";
    qStr += " `GMKey` varchar(100) NOT NULL,";
    qStr += " `GMValue` varchar(250) DEFAULT NULL,";
    qStr += " `Time_Stamp` timestamp NOT NULL DEFAULT current_timestamp(),";
    qStr += " `Data_Type` varchar(50) DEFAULT NULL,";
    qStr += " `Text_To_Display` varchar(200) DEFAULT NULL,";
    qStr += " `Value_Type` varchar(100) DEFAULT NULL,";
    qStr += " `Possible_Values` varchar(500) DEFAULT NULL,";
    qStr += " `IsLogged` tinyint(1) DEFAULT NULL,";
    qStr += " PRIMARY KEY (`_id`)";
    qStr += ") ";

    return db->executeDDL(qStr).value("SUCCESS").toBool();
}

QVariant ConfigurationSettingsDatabaseHelper::getValue(QString key, QVariant value)
{
    //    qDebug()<<Q_FUNC_INFO<<__LINE__;
    if(checkKeyExist(key)){
        //        qDebug()<<Q_FUNC_INFO<<__LINE__;
        return getVariableValue(key);
    }
    else{
        upsertSettings(key,value);
        return value;
    }
}

QVariant ConfigurationSettingsDatabaseHelper::getAlgoConfigValue(QString key, QVariant value)
{
        qDebug()<<Q_FUNC_INFO<<__LINE__;
    if(checkAlgoConfigKeyExist(key)){
        //        qDebug()<<Q_FUNC_INFO<<__LINE__;
        return getAlgoConfigVariableValue(key);
    }
    else{
        upsertAlgoConfigSettings(key,value);
        return value;
    }
}

void ConfigurationSettingsDatabaseHelper::setDb(DatabaseHelper *value)
{
    db = value;
}
