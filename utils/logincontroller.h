#ifndef LOGINCONTROLLER_H
#define LOGINCONTROLLER_H

#include <QObject>

#include "customwidgets/Company/companyselectorwidget.h"
#include "utils/DatabaseUpdater/databaseupdaterwidget.h"
#include "utils/DatabaseUpdater/configdbupdater.h"
#include "login/loginvalues.h"
#include "login/loginpage.h"
#include "customwidgets/Misc/customshadoweffect.h"

class LoginController : public QObject
{
    Q_OBJECT
public:
    LoginController(bool isRestart = false, int userid = 0,
                    QString voucherPrefix = "0",QObject *parent = nullptr);
    ~LoginController();

    CompanySelectorWidget *companySelectWid;
    DatabaseUpdater *dbUpdater;
    UiMain *mw;

    bool showMessages = false;
    bool configDbStatus = false;

    bool isRestart = false;
    int userId = 0;
    QString voucherPrefix = "0";

    void updateAllDbs();
    bool updateDB();
    void selectDefaultCompany();
    bool getConfigDbStatus() const;

    void selectDB(QString dbName);

    bool checkLicense();
public slots:
    void showLoginPage(QString dbName);
    void openUIMain();
    void openCompanySelector();
    void setLoginValues();
    void loginCancel();
    void updateConfig();
};

#endif // LOGINCONTROLLER_H
