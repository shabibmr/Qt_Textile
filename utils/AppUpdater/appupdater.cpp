#include "appupdater.h"
#include "utils/DatabaseUpdater/configdbupdater.h"

AppUpdater::AppUpdater( QString ver,QString url)
{
    m_updater = QSimpleUpdater::getInstance();
    jsonURL = url;

    QString version = ver;
    bool customAppcast = false;
    bool downloaderEnabled = true;
    bool notifyOnFinish = false;
    bool notifyOnUpdate = true;
    bool mandatoryUpdate = false;

    /* Apply the settings */
    m_updater->setModuleVersion (jsonURL, version);
    m_updater->setNotifyOnFinish (jsonURL, notifyOnFinish);
    m_updater->setNotifyOnUpdate (jsonURL, notifyOnUpdate);
    m_updater->setUseCustomAppcast (jsonURL, customAppcast);
    m_updater->setDownloaderEnabled (jsonURL, downloaderEnabled);
    m_updater->setMandatoryUpdate (jsonURL, mandatoryUpdate);
}

void AppUpdater::checkForUpdates()
{

    connect(m_updater, &QSimpleUpdater::downloadFinished, this, [=](QString url, QString filePath){
        ConfigDbUpdater *dbupdater = new ConfigDbUpdater;
        dbupdater->importConfigDb();
    });
    m_updater->checkForUpdates(jsonURL);
}
