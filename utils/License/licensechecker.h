#ifndef LICENSECHECKER_H
#define LICENSECHECKER_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QTimer>

#include "database/Settings/ConfigurationSettingsDatabaseHelper.h"
#include "utils/SettingsHelper/settingshelper.h"
#include "datamodels/Settings/licenseinfodatamodel.h"
#include "QJsonDocument"
#include <QInputDialog>

class LicenseChecker : public QObject
{
    Q_OBJECT
public:
    explicit LicenseChecker(QString Org , QObject *parent = nullptr);

    QString url = "http://31.220.109.198/algoERP/new12.php?action=";
    QNetworkReply *reply;
    QNetworkAccessManager* mgr;

    SettingsHelper *settings;
    LicenseInfoDataModel licenseInfo;

    QString organizationName;

    ~LicenseChecker();
signals:
    void requestStarted();
    void requestFinished();

public slots:

    bool getLicenseInfo();
    void replyFinished(QNetworkReply *reply);
    void getLicenseKey();
    void updateLicenseKey();
    bool checkValidity();
    void addCompany();
    void addLicenseEntry();

    void sendRequest(QString action, QString args);
    void saveSettings();
    void getCompanyId();

    void showMessage(QString text);
};

#endif // LICENSECHECKER_H
