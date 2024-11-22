#include "logincontroller.h"
#include "utils/License/licensechecker.h"
#include "utils/AppUpdater/appupdater.h"

LoginController::LoginController(bool isRestart, int userid, QString voucherPrefix,QObject *parent)
    : QObject(parent)
{
    this->isRestart = isRestart;
    this->voucherPrefix = voucherPrefix;
    this->userId = userid;

    companySelectWid = new CompanySelectorWidget(isRestart); // db list wil be ready here
    connect(companySelectWid, &CompanySelectorWidget::loginRequired,
            this, &LoginController::showLoginPage);
    companySelectWid->setView();
    companySelectWid->hide();

    connect(companySelectWid, &CompanySelectorWidget::companySwitched, this, [=](QString val){
        qDebug()<<"set db"<<DatabaseValues::connectionString;
        setLoginValues();
        mw->setCompanyName(val);
        //        mw->setActiveCompanyLabel(val);
        //        bool isPos = ConfigurationSettingsDatabaseHelper::getValue(showPOS, DatabaseValues::isPOS).toBool();
        //        qDebug()<<Q_FUNC_INFO<<__LINE__<<isPos;
        //        DatabaseValues::setIsPOS(isPos);
        if(DatabaseValues::isPOS){
            mw->openPOSWindow(userId, voucherPrefix);
        }
    });




    //    ConfigDbUpdater *configUpdater = new ConfigDbUpdater;
    //    connect(configUpdater, &ConfigDbUpdater::importCompleted, this, [=](){
    //        configDbStatus = true;
    //    });
    //    configUpdater->importConfigDb();

    dbUpdater = new DatabaseUpdater;
    updateConfig();

    QFile inputFile2("debugdev.txt");
    if (inputFile2.open(QIODevice::ReadOnly))
    {
        showMessages = true;
    }
}

bool LoginController::getConfigDbStatus() const
{
    qDebug()<<Q_FUNC_INFO<<configDbStatus;
    return configDbStatus;
}

void LoginController::showLoginPage(QString dbName)
{
    if(isRestart){
        QFile File("restartlog.txt");
        File.open(QFile::WriteOnly | QFile::Text | QFile::Append);

        QString functionName = Q_FUNC_INFO;
        QString errorData = " \n showloginpage ";

        File.write(errorData.toUtf8());
        File.close();

        setLoginValues();
        openUIMain();
        companySelectWid->addLoggedBranch();

        //        return;
    }
    else {


        updateDB();
        //    checkLicense();

        qDebug()<<Q_FUNC_INFO<<__LINE__;
        LoginPage *login = new LoginPage;
        login->setCompanyName(companySelectWid->getCompanyName(dbName));
        login->setWindowFlag(Qt::WindowStaysOnTopHint);
        login->setCompSelector(companySelectWid);
        connect(login, &LoginPage::loginSuccess, this, [=](){
            companySelectWid->addLoggedBranch();
            qDebug()<<Q_FUNC_INFO<<__LINE__<<LoginValues::model.Name<<LoginValues::company.CompanyName<<LoginValues::initLogin;

            //        if(!LoginValues::initLogin)
            mw->setActiveCompanyLabel(LoginValues::company.CompanyName + " - " + LoginValues::company.branch);
            if(DatabaseValues::isPOS && LoginValues::initLogin){ // to filter out initial login vs company switch
                mw->openPOSWindow();
            }
            //        setLoginValues();
        });

        connect(login, &LoginPage::showUiMain, this, &LoginController::openUIMain);
        connect(login, &LoginPage::loginCancelled, this, &LoginController::loginCancel);

        QWidget *widgetLogin = new QWidget();
        QHBoxLayout *lay = new QHBoxLayout;
        widgetLogin->setLayout(lay);
        lay->addWidget(login);
        widgetLogin->setAttribute(Qt::WA_TranslucentBackground);
        widgetLogin->setWindowFlags(Qt::FramelessWindowHint);
        widgetLogin->setStyleSheet("background: #fff9f3");
        QGraphicsDropShadowEffect *wndShadow = new QGraphicsDropShadowEffect;
        wndShadow->setBlurRadius(9.0);
        wndShadow->setColor(QColor(0, 0, 0, 160));
        wndShadow->setXOffset(4.0);
        wndShadow->setYOffset(4.0);

        CustomShadowEffect *bodyShadow = new CustomShadowEffect();
        bodyShadow->setBlurRadius(20.0);
        bodyShadow->setDistance(6.0);
        bodyShadow->setColor(QColor(0, 0, 0, 80));
        widgetLogin->setAutoFillBackground(true);
        widgetLogin->setGraphicsEffect(bodyShadow);
        widgetLogin->setGraphicsEffect(wndShadow);
        widgetLogin->show();

        QObject::connect(login, SIGNAL(cloes()), widgetLogin,  SLOT(close()));
        login->setFocus();
    }
}

void LoginController::openUIMain()
{

    QString errorData = "\n restarting . ui main "  ;

    qDebug()<<Q_FUNC_INFO<<__LINE__;
    //    setLoginValues();
    //    if(LoginValues::initLogin)
    //        delete mw;
    bool isPos = ConfigurationSettingsDatabaseHelper::getValue(showPOS, DatabaseValues::isPOS).toBool();
    qDebug()<<Q_FUNC_INFO<<__LINE__<<isPos;

    errorData += QString::number(isPos);
    DatabaseValues::setIsPOS(isPos);

    mw = new UiMain(isRestart, userId, voucherPrefix);

    connect(mw, &UiMain::openCompanySelector, this, &LoginController::openCompanySelector);
    connect(mw, &UiMain::UiShow, this, [=](QString dbName){
        qDebug()<<Q_FUNC_INFO<<__LINE__<<dbName;
        QString str = companySelectWid->getCompanyName(dbName);
        mw->setCompanyName(str);
        qDebug()<<Q_FUNC_INFO<<__LINE__;
        DatabaseValues::setDbName(companySelectWid->getDbName(dbName));
    });
    mw->setCompanyName(LoginValues::getCompany().CompanyName);
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    mw->setAttribute(Qt::WA_DeleteOnClose);
    mw->setWindowTitle(LoginValues::getCompany().branch);

    qDebug()<<Q_FUNC_INFO<<__LINE__;
    if(!isPos || isRestart){
        errorData += "\n showmaxim " + QString::number(DatabaseValues::isPOS);
        mw->showMaximized();
    }
    qDebug()<<Q_FUNC_INFO<<__LINE__;

    if(isRestart){
        QFile File("restartlog.txt");
        File.open(QFile::WriteOnly | QFile::Text | QFile::Append);

        QString functionName = Q_FUNC_INFO;
        File.write(errorData.toUtf8());
        File.close();
    }
    qDebug()<<Q_FUNC_INFO<<__LINE__;
}

LoginController::~LoginController()
{

}

void LoginController::updateAllDbs()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__<<companySelectWid->cList.size();
    if(companySelectWid->cList.size() == 0){
        QMessageBox box; box.setText("No data found"); box.exec();
    }
    for(int i=0; i< companySelectWid->cList.size(); i++){
        QString connStr = companySelectWid->cList[i][0]+"_"+QString::number(i);
        qDebug()<<Q_FUNC_INFO<<__LINE__<<connStr;
        DatabaseValues::setConnectionString(connStr);
        //        qDebug()<<Q_FUNC_INFO<<__LINE__<<"******* checking db updates for ********"<<companySelectWid->cList[i]<<connStr;
        updateDB(/*connStr, companySelectWid->cList[i][1]*/);

    }
}

bool LoginController::updateDB(/*QString connStr, QString companyName*/)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__<<DatabaseValues::connectionString;
    DatabaseHelper *db = new DatabaseHelper(DatabaseValues::connectionString);

    ConfigurationSettingsDatabaseHelper *cHelper = new ConfigurationSettingsDatabaseHelper(db);

    dbUpdater->setUpdateAlgoCOnfig(false);
    dbUpdater->setDbhelper(cHelper);

    qDebug()<<Q_FUNC_INFO<<__LINE__;
    if(dbUpdater->readDBChangesFile(":/json/dbchanges.json").size() == 0 ){
        if(showMessages){
            QMessageBox box; box.setText("Unable to read updates. Check configuration"); box.exec();
        }
        return false;
    }

    else{
        qDebug()<<Q_FUNC_INFO<<__LINE__;
        if(!cHelper->checkConfigTableExists()){
            qDebug()<<"config table doesnt exst";
            if(!dbUpdater->runZerothDbUpdate())
                return false;

        }
        qDebug()<<Q_FUNC_INFO<<__LINE__;

        if(dbUpdater->checkNewUpdates()){
            qDebug()<<"db updates";
            QWidget *widget = new QWidget;
            QMessageBox::StandardButton reply = QMessageBox::Yes;
            if(showMessages){
                reply = QMessageBox::question(widget,"DB Updates ",
                                              "Updates found. Do you want to update?",
                                              QMessageBox::Yes|QMessageBox::No);
            }
            if(reply==QMessageBox::Yes)
            {
                dbUpdater->makeUpdates();
                dbUpdater->updateVersionNumber();
            }
            delete widget;
        }
        // else no new updates
    }
    return true;
}

void LoginController::selectDefaultCompany()
{
    QString errorData = "\n select default company " ;

    qDebug()<<Q_FUNC_INFO<<companySelectWid->cList.size();

    if(companySelectWid->cList.size() == 0){
        QMessageBox box; box.setText("No data found"); box.exec();
        errorData += "\n xxxxx ";
        qDebug()<<Q_FUNC_INFO<<companySelectWid->cList.size()<<"Exiting App";
        qApp->exit();


    }
    else{
        qDebug()<<Q_FUNC_INFO<<companySelectWid->cList.size()<<"Else";
        if(!companySelectWid->selectDefaultBranch()){
            //        qDebug()<<Q_FUNC_INFO<<__LINE__;
            companySelectWid->showMaximized();
            //        QMessageBox box; box.setText("Company not found"); box.exec();
            //        qApp->exit();
        }
        errorData += "\n sssss ";
    }

    if(isRestart){
        QFile File("restartlog.txt");
        File.open(QFile::WriteOnly | QFile::Text | QFile::Append);

        QString functionName = Q_FUNC_INFO;
        File.write(errorData.toUtf8());
        File.close();
    }
    qDebug()<<Q_FUNC_INFO<<__LINE__;
}

void LoginController::selectDB(QString dbName)
{
    if(companySelectWid->cList.size() == 0){
        QMessageBox box; box.setText("No data found"); box.exec();
        qApp->exit();
    }
    else{
        if(isRestart){
            QFile File("restartlog.txt");
            File.open(QFile::WriteOnly | QFile::Text | QFile::Append);

            QString functionName = Q_FUNC_INFO;
            QString errorData = "\n select db . dbname= " + dbName ;

            File.write(errorData.toUtf8());
            File.close();
        }
        companySelectWid->switchDB(dbName);
    }
    qDebug()<<Q_FUNC_INFO<<__LINE__;
}

bool LoginController::checkLicense()
{
    LicenseChecker *lic = new LicenseChecker(LoginValues::company.CompanyName + " - "
                                             + LoginValues::company.branch);
    QLabel *label = new QLabel("Loading... ");
    label->setStyleSheet("QLabel{color:#2e7d32}");
    //    ui->msgLay->addWidget(label);
    connect(lic, &LicenseChecker::requestFinished, this, [=](){
        label->setText("");
    });
    bool validity = lic->getLicenseInfo();
    if(!validity){
        //        if(!lic->checkValidity()){
        qDebug()<<"license expired, app exit";
        qApp->exit();
        return false;
        //        }
    }
    else{
        if(LoginValues::autoUpdate){
            QString currentVersion = APP_VERSION;
            AppUpdater *updater = new AppUpdater(currentVersion);
            updater->checkForUpdates();
        }
    }

    return true;
}

void LoginController::openCompanySelector()
{


    companySelectWid->resize(500,500);
    companySelectWid->setWindowFlags(Qt::Window);
    companySelectWid->show();

}

void LoginController::setLoginValues()
{
    if(isRestart){
        QFile File("restartlog.txt");
        File.open(QFile::WriteOnly | QFile::Text | QFile::Append);

        QString functionName = Q_FUNC_INFO;
        QString errorData = "\n set login values " + DatabaseValues::dbName ;

        File.write(errorData.toUtf8());
        File.close();
    }

    UserProfileDatabaseHelper userHelper;
    UserGroupDatabaseHelper *uHelper = new UserGroupDatabaseHelper;
    if(!isRestart)
        userId = companySelectWid->getLoggedInUserByBranch(DatabaseValues::dbName);
    UserProfileDataModel user = userHelper.getEmployeeByID(userId);

    LoginValues::setVoucherPrefix(DatabaseValues::voucherPrefix);
    LoginValues::setUserName(user.Username);
    LoginValues::setUserEmail(user.Email);
    LoginValues::setEmpName(user.Name);
    LoginValues::setUserID(userId);
    LoginValues::setUserPhone(user.Phone);
    //    QMessageBox box; box.setText("set user group " + QString::number(user.userGroup) +
    //                                 " name " + user.Username + " userid " + QString::number(userId) );
    //    box.exec();
    LoginValues::setModel(uHelper->getUserGroup(user.userGroup));


    LoginValues::setTest(ConfigurationSettingsDatabaseHelper::getVariableValue(testKey).toBool());
    CounterSettingsDatabaseHelper* settingsDB = new CounterSettingsDatabaseHelper();
    CompanyProfileDataBaseHelper* companyHelper = new CompanyProfileDataBaseHelper;
    settingsDB->getCounterSettings(LoginValues::voucherPrefix);
    LoginValues::setCompany(companyHelper->getCompanyDetails());

//    qDebug()<<Q_FUNC_INFO<<__LINE__<<LoginValues::company.CompanyName;


}

void LoginController::loginCancel()
{
    // switch back to last db
//    qDebug()<<Q_FUNC_INFO<<__LINE__<<CompanySelectorWidget::loggedInBranches.last();
    companySelectWid->switchDB(CompanySelectorWidget::loggedInBranches.last());

}

void LoginController::updateConfig()
{
    DatabaseValues dv;
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    dv.addDbConnection("algo_config", "algo_config");
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    if(!DatabaseValues::configDbStatus){
        qDebug()<<Q_FUNC_INFO<<__LINE__;
//        dv.removeDbConnection("algo_config");
        ConfigDbUpdater *configUpdater = new ConfigDbUpdater;
        connect(configUpdater, &ConfigDbUpdater::importCompleted, this, [=](){
            configDbStatus = true;
        });
        configUpdater->importConfigDb();
        dv.addDbConnection("algo_config", "algo_config");
    }

    dv.setConnectionString("algo_config");
    qDebug()<<Q_FUNC_INFO<<__LINE__<<DatabaseValues::connectionString;

    DatabaseHelper *db = new DatabaseHelper(DatabaseValues::connectionString);

    ConfigurationSettingsDatabaseHelper *cHelper = new ConfigurationSettingsDatabaseHelper(db);


    dbUpdater->setDbhelper(cHelper);
    dbUpdater->setUpdateAlgoCOnfig(true);

    if(dbUpdater->readDBChangesFile(":/json/algo_config.json").size() == 0 ){
        if(showMessages){
            QMessageBox box; box.setText("Unable to read updates. Check configuration"); box.exec();
        }
        return ;
    }

    else{
//        qDebug()<<Q_FUNC_INFO<<__LINE__;
//        if(!cHelper->checkAlgoConfigTableExists()){
//            qDebug()<<Q_FUNC_INFO<<"config table doesnt exst";
//            ConfigDbUpdater *configUpdater = new ConfigDbUpdater;
//            connect(configUpdater, &ConfigDbUpdater::importCompleted, this, [=](){
//                configDbStatus = true;
//            });
//            configUpdater->importConfigDb();
//            //            if(!dbUpdater->runZerothDbUpdate())
//            //                return ;

//        }
        qDebug()<<Q_FUNC_INFO<<__LINE__;

        if(dbUpdater->checkNewUpdates()){
            qDebug()<<Q_FUNC_INFO<<__LINE__<<"db updates";
            QWidget *widget = new QWidget;
            QMessageBox::StandardButton reply = QMessageBox::Yes;
            if(showMessages){
                reply = QMessageBox::question(widget,"DB Updates ",
                                              "Updates found. Do you want to update?",
                                              QMessageBox::Yes|QMessageBox::No);
            }
            if(reply==QMessageBox::Yes)
            {
                dbUpdater->makeUpdates();
                dbUpdater->updateConfigVersionNumber();
            }
            delete widget;
        }
        // else no new updates
    }
}
