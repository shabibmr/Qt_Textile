#ifndef CONFIGDBUPDATER_H
#define CONFIGDBUPDATER_H

#include <QDebug>
#include "database/databasehelper/databasevalues.h"
#include "database/databasehelper/databasehelper.h"
#include <QProcess>
#include <QApplication>

#include"utils/SettingsHelper/settingshelper.h"

class ConfigDbUpdater : public QObject
{
    Q_OBJECT

public:
    ConfigDbUpdater();
    
//    QString sqlFileName = QCoreApplication::applicationDirPath() + "/algo_config.sql ";
    QByteArray data;
    bool updateRequired  = false;
    SettingsHelper settings;
    QString versionKey = "appVersion";

    void importConfigDb();
    bool checkDbExists();
    void importStatus(int exitCode, QProcess::ExitStatus status);
    void readConfigSql();
    bool checkDbVersion();
    bool updateSettings();
signals:
    void importCompleted();
};

#endif // CONFIGDBUPDATER_H
