#include "configdbupdater.h"
#include <QMessageBox>
#include <QFile>


ConfigDbUpdater::ConfigDbUpdater()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    readConfigSql();
}

void ConfigDbUpdater::importConfigDb()
{
    if(!checkDbVersion() )  // true if no updates
        updateRequired = true;
    checkDbExists();
    qDebug()<<Q_FUNC_INFO<<__LINE__<<updateRequired;

    if(updateRequired){
        qDebug()<<"**********************"<<Q_FUNC_INFO<<__LINE__<<data;
        if( DatabaseHelper::executeDDL(data, "algo_config")["SUCCESS"].toBool())
            importStatus(1, QProcess::NormalExit);
        else {
            importStatus(2, QProcess::CrashExit);
        }
    }
    else {
//        QMessageBox box; box.setText("algo_config not udpated"); box.exec();
        qDebug()<<"CONFIG DB NOT UPDATED";
    }

}

bool ConfigDbUpdater::checkDbExists()
{
     qDebug()<<Q_FUNC_INFO<<__LINE__;
    if(!DatabaseHelper::checkDbExists("algo_config")){
          qDebug()<<Q_FUNC_INFO<<__LINE__<<"algo_config not found, creating ";
          if(!DatabaseHelper::executeDDL("CREATE DATABASE ALGO_CONFIG", "info")["SUCCESS"].toBool())
          {
//              QMessageBox box; box.setText("Create db failed"); box.exec();
          }
          DatabaseValues dv;
          dv.addDbConnection("algo_config", "algo_config");
          updateRequired = true;
    }
    else{
        qDebug()<<Q_FUNC_INFO<<__LINE__;
        return true;
    }

}

void ConfigDbUpdater::importStatus(int exitCode, QProcess::ExitStatus status)
{
    if(status == QProcess::NormalExit){
//        QMessageBox box;
//        box.setText("Import completed " + QString::number( exitCode));
//        box.exec();
        emit importCompleted();
        updateSettings();


    }

    else {
//        QMessageBox box;
//        box.setText("Import failed " + QString::number( exitCode));
//        box.exec();
    }
}

void ConfigDbUpdater::readConfigSql()
{
    // read qrc :/sql/algo_config.sql

    QFile qrcFile(":/sql/algo_config.sql");
    if(qrcFile.exists()){
        qrcFile.open(QIODevice::ReadOnly);
        qDebug()<<Q_FUNC_INFO<<__LINE__<<qrcFile.size();
    }
    else
        qDebug()<<Q_FUNC_INFO<<__LINE__<<"cant readd";

    data = qrcFile.readAll();
    QString data1 = data;

    qDebug()<<Q_FUNC_INFO<<__LINE__<<data1;

    // write into QCoreApplication::applicationDirPath() + "/algo_config.sql "

//    QFile sqlFile(sqlFileName);
//    sqlFile.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
//    sqlFile.write(data);
//    sqlFile.close();


}

bool ConfigDbUpdater::checkDbVersion()
{

    if(!settings.keyExist(versionKey)) // first time, import anyway
        return false;
    else {
        if(settings.getValue(versionKey) < APP_VERSION)  // app_version has changed, import
            return false;
        else return true;
    }

}

bool ConfigDbUpdater::updateSettings()
{
//    return false;
    SettingsHelper settings;
    settings.setValue("appVersion", APP_VERSION);

}
