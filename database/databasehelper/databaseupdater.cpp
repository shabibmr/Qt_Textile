#include "databaseupdater.h"
#define EPSILON 1e-12
#include <QtMath>

DatabaseUpdater::DatabaseUpdater(ConfigurationSettingsDatabaseHelper *dbHelper)
{
    dbhelper = dbHelper;
    //    qDebug()<<"Database updater";
    //    readDBChangesFile();
}

DatabaseUpdater::~DatabaseUpdater()
{
    //    delete  dbhelper;
}

void DatabaseUpdater::setUpdateAlgoCOnfig(bool newUpdateAlgoCOnfig)
{
    updateAlgoCOnfig = newUpdateAlgoCOnfig;
}



bool DatabaseUpdater::checkNewUpdates()
{
    if(updateAlgoCOnfig)
        curr_DB_version = dbhelper->getAlgoConfigVariableValue(dbverison).toDouble();
    else
        curr_DB_version = dbhelper->getVariableValue(dbverison).toDouble();
    new_DB_version = getLatestDbVersion(curr_DB_version); // get from json FILE
    qDebug()<<Q_FUNC_INFO<<__LINE__<<curr_DB_version<<new_DB_version;
    //    QMessageBox box; box.setText("current version" + QString::number(curr_DB_version) +
    //                                 "latest version available " + QString::number(new_DB_version));
    //    box.exec();
    if(new_DB_version > curr_DB_version)
        return true;
    else
        return false;
}

void DatabaseUpdater::makeUpdates()
{
    double db_updated_version = curr_DB_version;
    //    double prev_version = curr_DB_version;
    qDebug()<<"current DB version is "<<curr_DB_version;
    double i=curr_DB_version + 0.1;

    while( i < new_DB_version   || qFabs(i-new_DB_version) < EPSILON ){
        //        qDebug()<<Q_FUNC_INFO<<__LINE__<<i<<new_DB_version<<EPSILON;
        //        qDebug()<<"check version "<<i;
        QMap<QString, QVariant> map = getQueriesOfVersion(i);
        if(map.size() > 0){ // continue loop if no entry for given version
            //            qDebug()<<Q_FUNC_INFO<<__LINE__<<"found query for version "<<i;
            QString qStr = map["Query"].toString();
            bool StopOnError = map["StopOnError"].toBool();
            if(!runCommand(qStr) ){
                //                qDebug()<<"query failed. Stop on error : "<<StopOnError;
                //                db_updated_version = prev_version;
                //                qDebug()<<"db prev version changed to "<<prev_version;
                if(StopOnError)
                    break;
            }
            else {
                qDebug()<<"query successful";
            }
            db_updated_version = i;
            qDebug()<<"db updated version changed to "<<db_updated_version;

        }
        else {
            qDebug()<<"no query in this version, continue"<<i<<db_updated_version;
        }
        i += 0.1;
    }
//    qDebug()<<Q_FUNC_INFO<<__LINE__<<i<<new_DB_version<<EPSILON<<db_updated_version;
    new_DB_version =  db_updated_version;
}

bool DatabaseUpdater::runCommand(QString qStr)
{
    //    qDebug()<<"Run query: "<<qStr;
    QMap<QString, QVariant> map = DatabaseHelper::executeDDL(qStr);

    return map["SUCCESS"].toBool();
}

void DatabaseUpdater::updateVersionNumber()
{
    //    qDebug()<<"Updating version num to "<<new_DB_version;
    dbhelper->upsertSettings(dbverison,QString::number(new_DB_version,'d',2));
}

void DatabaseUpdater::updateConfigVersionNumber()
{
    //    qDebug()<<"Updating version num to "<<new_DB_version;
    dbhelper->upsertAlgoConfigSettings(dbverison,QString::number(new_DB_version,'d',2));
}

QMap<QString, QVariant> DatabaseUpdater::getQueriesOfVersion(double version)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__<<version;
    QMap<QString, QVariant> map;

    for(QJsonValue entry:dbchanges){
        QJsonObject entryObj = entry.toObject();
        qDebug()<<Q_FUNC_INFO<<__LINE__<<(qFabs(entryObj[dbverison].toVariant().toDouble() - (version)) );
        if(entryObj[dbverison].toVariant().toString() ==  QString::number( version)){
            qDebug()<<Q_FUNC_INFO<<__LINE__<<entryObj["Query"].toString();
            //        if(entryObj[dbverison].toVariant().toDouble() == version){
            map.insert("Query", entryObj["Query"].toString());
            map.insert("StopOnError", entryObj["StopOnError"].toBool());
            break;
        }
    }

    //    if(map.size()>0)
    return  map;
}

QJsonArray DatabaseUpdater::readDBChangesFile(QString fileName)
{
    //    qDebug()<<Q_FUNC_INFO<<__LINE__;

    QFile file(fileName);  // :/json/json/dbchanges.json doesnt work
    file.open(QIODevice::ReadOnly);

    QString data = file.readAll();

    QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
    dbchanges = doc.array();

    file.close();

    //    qDebug()<<Q_FUNC_INFO<<__LINE__<<dbchanges;

    return dbchanges;

}

void DatabaseUpdater::writeDBChangesFile(QString fileName, QJsonArray newEntry)
{
    qDebug()<<"Updating File : "<<fileName;
    QJsonDocument doc(newEntry);

    QFile File(fileName);
    File.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
    File.write(doc.toJson());
    File.close();

}

double DatabaseUpdater::getLatestDbVersion(double vNo)
{
    double latestVersion = vNo;
    for(QJsonValue entry:dbchanges){
        QJsonObject entryObj = entry.toObject();
        if(entryObj[dbverison].toDouble() > latestVersion)
            latestVersion = entryObj[dbverison].toDouble();
    }
    return latestVersion;
}

bool DatabaseUpdater::runZerothDbUpdate()
{
    if(runCommand(getQueriesOfVersion(0)["Query"].toString())){
        new_DB_version = 0;
        updateVersionNumber();
        return true;
    }
    else {
        return false;
    }
}

void DatabaseUpdater::setDbhelper(ConfigurationSettingsDatabaseHelper *value)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__<<DatabaseValues::connectionString;
    dbhelper = value;
    //    dbhelper->setDb(value);
}

void DatabaseUpdater::getAllQueries()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    readDBChangesFile(":/json/dbchanges.json");

    qDebug()<<Q_FUNC_INFO<<__LINE__;
    QString str = "";

    for(QJsonValue entry:dbchanges){
        QJsonObject entryObj = entry.toObject();
        if(entryObj["DB_Version"].toInt() > 232 )
            str += entryObj["Query"].toString() + "; ";
    }

    qDebug()<<str;

}
