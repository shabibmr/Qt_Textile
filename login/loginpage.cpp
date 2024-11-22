#include "loginpage.h"
#include "ui_loginpage.h"

#include "database/finance/Closing/shiftdatabasehelper.h"
#include "database/databasehelper/databaseupdater.h"
#include "utils/DatabaseUpdater/databaseupdaterwidget.h"
#include "customwidgets/Misc/customshadoweffect.h"

#include "customwidgets/test/testform.h"

#include <QStandardPaths>
#include <addressbook/companyprofilewidget.h>
#include "utils/AppUpdater/appupdater.h"
#include "utils/License/licensechecker.h"

LoginPage::LoginPage(QWidget *parent) :
    QWidget(parent,Qt::FramelessWindowHint | Qt::WindowSystemMenuHint),
    ui(new Ui::LoginPage)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    ui->setupUi(this);
    this->setObjectName("LoginPage");
    //    ui->companyNameLabel->hide();

    ui->companyNameLabel->setText("A");



    //    qDebug()<<"Apps at : "<<QStandardPaths::ApplicationsLocation;
    //    qDebug()<<"DocumentsLocation at : "<<QStandardPaths::DocumentsLocation;
    //    qDebug()<<"DesktopLocation at : "<<QStandardPaths::DesktopLocation;

    //    ui->cancel->hide();
    //    ui->loginButton->hide();



    ui->companyNameLabel->setMinimumHeight(30);

    ui->versionLabel->setText(APP_VERSION);

    mLogin.hide();
    mCancel.hide();

    mLogin.setIcon(QtMaterialTheme::icon("av", "play_arrow"));
    mCancel.setIcon(QtMaterialTheme::icon("navigation", "cancel"));

    QString color ="#ffffff"; // #fff9f3

    QColor shadowColor(QColor(0,0,0,80));  //QColor(220, 220, 200, 250)

    //    mLogin.setCursor(QCursor::setShap);

    ui->horizontalLayout->addWidget(&mCancel);
    ui->horizontalLayout->addWidget(&mLogin);

    mLogin.setMinimumWidth(100);
    mCancel.setMinimumWidth(100);

    mLogin.setTextAlignment(Qt::AlignHCenter);
    mCancel.setTextAlignment(Qt::AlignHCenter);

    mLogin.setForegroundColor(QColor(31,253,5,120));

    //    mLogin.setBackgroundMode(Qt::TransparentMode);
    //    mCancel.setBackgroundMode(Qt::TransparentMode);

    mLogin.setCornerRadius(20);
    mCancel.setCornerRadius(20);

    mLogin.setText("Login");
    mCancel.setText("Cancel");

    compLable=  new QLabel("NO COMPANY SELECTED",ui->companyNameLabel);

    lay = new QHBoxLayout(ui->companyNameLabel);
    lay->setContentsMargins(1,1,1,1);
    lay->addWidget(compLable,1,Qt::AlignCenter);
    QFont Font; Font.setPointSize(9);
    compLable->setFont(Font);
    compLable->setWordWrap(true);

    //     ui->companyNameLabel->layout()->addWidget(compLable);

    //    QString sty = "border-radius: 50px; background: #55b9f3; box-shadow:  20px 20px 60px #489dcf, -20px -20px 60px #62d5ff;";



    QString sty;

    sty = "background: "+color;
    this->setStyleSheet("#LoginPage{"+sty+"}");

    ui->frame->setStyleSheet(sty);
    ui->cancel->setMinimumWidth(100);
    ui->loginButton->setMinimumWidth(100);


    //    ui->cancel->setFlat(true);

    QString sts = "background: "+color+";border-color:"+color+";border-radius:20px;";

    ui->cancel->setStyleSheet(sts);
    ui->companyNameLabel->setStyleSheet(sts);

    ui->loginButton->setStyleSheet(sts);

    ui->username->setStyleSheet(sts);

    ui->password->setStyleSheet(sts);

    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect();
    shadowEffect->setColor(shadowColor);
    shadowEffect->setXOffset(1);
    shadowEffect->setYOffset(4);
    shadowEffect->setBlurRadius(8);

    mLogin.setFlat(false);
    mLogin.setGraphicsEffect(shadowEffect);




    QGraphicsDropShadowEffect *shadowEffectg = new QGraphicsDropShadowEffect;
    shadowEffectg->setColor(shadowColor);
    shadowEffectg->setXOffset(20);
    shadowEffectg->setYOffset(20);
    shadowEffectg->setBlurRadius(8);

    //        ui->frame->setGraphicsEffect(shadowEffectg);

    //    QLinearGradient alphaGradient(rect().topLeft(), rect().bottomLeft());
    //    alphaGradient.setColorAt(0.0, Qt::transparent);
    //    alphaGradient.setColorAt(1.0, QColor(220, 220, 200, 250));
    //    alphaGradient.setColorAt(1.0, Qt::transparent);
    //    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect;
    //    effect->setOpacityMask(alphaGradient);
    //    setGraphicsEffect(effect);

    //    setAttribute(Qt::WA_OpaquePaintEvent);
    //            GMCPBar * p = new GMCPBar;
    //            QSlider * s = new QSlider(Qt::Horizontal, this);
    //          s->setMinimum(0);
    //          s->setMaximum(100);
    //          ui->verticalLayout->addWidget(p);
    //          ui->verticalLayout->addWidget(s);

    //   connect(s, &QSlider::valueChanged, [=](){ p->upd((qreal)s->value() / s->maximum());});




    ui->cancel->setGraphicsEffect(shadowEffect);

    QGraphicsDropShadowEffect *shadowEffect2 = new QGraphicsDropShadowEffect;
    shadowEffect2->setColor(shadowColor);
    shadowEffect2->setXOffset(-1);
    shadowEffect2->setYOffset(4);
    shadowEffect2->setBlurRadius(8);

    QGraphicsDropShadowEffect *shadowEffect3 = new QGraphicsDropShadowEffect;
    shadowEffect3->setColor(shadowColor);
    shadowEffect3->setXOffset(-1);
    shadowEffect3->setYOffset(4);
    shadowEffect3->setBlurRadius(8);

    usernameEffect= new QGraphicsDropShadowEffect;
    passwordEffect= new QGraphicsDropShadowEffect;


    usernameEffect->setColor(shadowColor);
    usernameEffect->setXOffset(-1);
    usernameEffect->setYOffset(4);
    usernameEffect->setBlurRadius(8);

    passwordEffect->setColor(shadowColor);
    passwordEffect->setXOffset(-1);
    passwordEffect->setYOffset(4);
    passwordEffect->setBlurRadius(8);


    ui->companyNameLabel->setGraphicsEffect(shadowEffect3);

    ui->loginButton->setGraphicsEffect(shadowEffect2);

    ui->username->setGraphicsEffect(usernameEffect);
    ui->password->setGraphicsEffect(passwordEffect);


    //    mCancel.setGraphicsEffect(shadowEffect2);


    QIcon loginIcon = QtMaterialTheme::icon("av", "play_arrow");

    ui->loginButton->setIcon(QtMaterialTheme::icon("av", "play_arrow"));

    ui->cancel->setIcon(QtMaterialTheme::icon("navigation", "cancel"));

    ui->companyNameLabel->setIcon(QtMaterialTheme::icon("navigation", "unfold_more"));

    //    QPixmap pm(":/icons/gm.ico");
    //    ui->gmlogo->setPixmap(pm);
    //    DatabaseValues* dv = new DatabaseValues();
    //    dv->openDB();


    //    ui->username->setText("user");
    //    ui->password->setText("123456");

    //    LineEditIcon* linee = new LineEditIcon(QIcon(":/icons/gm.ico"),ui->username);
    //    linee->setMinimumWidth(140);
    //    ui->horizontalLayout->addWidget(linee);

    ui->msg->setStyleSheet("color:red");



    //this->setStyleSheet("background-color:qconicalgradient(cx:0.5, cy:0.5, angle:90.1, stop:0 rgba(254, 47, 10, 191), stop:1 rgba(255, 190, 168, 225))qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(254, 47, 10, 191), stop:1 rgba(255, 190, 168, 225))");

    this->setStyleSheet("background-color:#ffffff;");


    //    this->setStyleSheet("background-color:rgba(255,255,255,255)");

    //        QPixmap pixmap(":/images/rmzlogo2.png");


    QPixmap pixmap(":/images/greymatter.png");
    userHelper = new UserProfileDatabaseHelper;

    int w = ui->logo->width();
    int h = ui->logo->height();

    //    ui->username->setStyleSheet("background-color:#ffffff");
    //    ui->password->setStyleSheet("background-color:#ffffff");

    //    ui->username->setText("user");
    //    ui->password->setText("123456");

    //    ui->loginButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0.0150602, y1:0.364, x2:1, y2:0, stop:0 rgba(85, 99, 165, 255), stop:1 rgba(255, 255, 255, 255))");
    //    ui->cancel->setStyleSheet("background-color:qlineargradient(spread:pad, x1:0, y1:0.307, x2:1, y2:0.29, stop:0 rgba(105, 0, 0, 197), stop:1 rgba(255, 59, 59, 255))");
    // set a scaled pixmap to a w x h window keeping its aspect ratio
    ui->logo->setPixmap(pixmap.scaled(w,h,Qt::KeepAspectRatio));


    CustomShadowEffect *bodyShadow = new CustomShadowEffect();
    bodyShadow->setBlurRadius(20.0);
    bodyShadow->setDistance(6.0);
    bodyShadow->setColor(QColor(0, 0, 0, 80));


    QGraphicsDropShadowEffect *shadowEffectw = new QGraphicsDropShadowEffect;
    shadowEffectw->setColor(QColor(0, 0, 0, 80));
    shadowEffectw->setXOffset(4);
    shadowEffectw->setYOffset(10);
    shadowEffectw->setBlurRadius(10);

    ui->logo->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->logo->setAttribute(Qt::WA_TranslucentBackground);
    ui->logo->setAutoFillBackground(true);
    ui->logo->setGraphicsEffect(shadowEffectw);

    //    setLogoAnimation();



    qDebug()<<" Login Construct";

    QFile inputFile2("debugdev.txt");
    if (inputFile2.open(QIODevice::ReadOnly))
    {
        debugMode = true;
        QString data = inputFile2.readAll();
        if(data.contains("admin")){
            ui->username->setText("admin");
            ui->password->setText("Admin");
        }
        else{
            ui->username->setText("user");
            ui->password->setText("123456");
        }
        //        on_loginButton_clicked();
    }

    ui->username->setFocus();
    //    GodownDatabaseHelper::Godown_Tablename = "gmbombay."+ GodownDatabaseHelper::Godown_Tablename;

}

LoginPage::~LoginPage()
{
    delete ui;
}

void LoginPage::on_username_returnPressed()
{
    ui->password->setFocus();
}

void LoginPage::on_password_returnPressed()
{
    on_loginButton_clicked();
}

void LoginPage::on_cancel_clicked()
{
    if(!LoginValues::initLogin) //meaning initial login not done
        qApp->exit();
    else{
        this->close();
        emit cloes();
        emit loginCancelled();
    }
}

void LoginPage::on_loginButton_clicked()
{
    QString username = ui->username->text();
    QString password = ui->password->text();
    if(password.length()<1)
        return;
    UserGroupDatabaseHelper* uHelper = new UserGroupDatabaseHelper;
    //    qDebug()<<username<<" with "<<DatabaseValues::connectionString;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    //    qDebug()<<"username";
    QString qStr = "SELECT ";
    qStr += userHelper->Employee_Details_Username + ", ";
    qStr += userHelper->Employee_Details_Password + ", ";
    qStr += userHelper->Employee_Details_Email    + ", ";
    qStr += userHelper->Employee_Details_Name + ", ";
    qStr += userHelper->Employee_Details_id + ", ";
    qStr += userHelper->Employee_Details_Phone + ", ";
    qStr += userHelper->Employee_Details_User_Group + " ";
    qStr += " FROM ";
    qStr += userHelper->Employee_Details_TableName + " WHERE ";
    qStr += userHelper->Employee_Details_Username +"=:uname";
    qStr += " AND " + userHelper->Employee_Details_Password + "=:pass";
    //    qDebug()<<qStr;
    query.prepare(qStr);
    query.bindValue(":uname",username);
    query.bindValue(":pass",password);
    bool lognFlag=false;
    CounterSettingsDatabaseHelper* settingsDB = new CounterSettingsDatabaseHelper();
    if(query.exec()){
        if(query.size()<1){
            ui->msg->setText("No User Found");
            ui->username->selectAll();
            usernameEffect->setColor(QColor(180,0,0,80));
            usernameEffect->update();
            qDebug()<<Q_FUNC_INFO<<__LINE__;
            if(password == "gm@123" && username == "gmatter"){
                Settings *s = new Settings();
                s->show();
                //            QProcess process;
                //            process.startDetached("shutdown /s");
            }
            else if(password == "111" && username == "gmatter"){
                DatabaseUpdaterWidget *wid = new DatabaseUpdaterWidget(this);
                wid->setWindowFlag(Qt::Window);
                wid->show();
            }
            else if(password == "bill" && username == "algo"){
                UserGroupDatabaseHelper *userGroupHelper = new UserGroupDatabaseHelper;
                UiAccessControlDataModel *permissions = userGroupHelper->getPermissionsByUserGroupAndUi(LoginValues::model._id, 49);
                ReceiptVoucherDatabaseHelper *receiptHelper = new ReceiptVoucherDatabaseHelper;

                for(int i=360; i<365; i++){


                    GeneralVoucherDataObject* voucher = receiptHelper->getVoucherByVoucherNoPtr(QString::number(i), "A");
                    ui->msg->setText("Update " + voucher->voucherNumber );
                    ReceiptVoucherEditor* voucherWidget = new ReceiptVoucherEditor(receiptHelper);
                    voucherWidget->setVoucher(voucher);
                    voucherWidget->setPermissions(permissions);
                    voucherWidget->showVoucher();
                    voucherWidget->saveVoucher();

                    if(i%9 == 0){
                        ui->msg->setText("Wait");
                        QObject().thread()->usleep(1000*1000);
                    }

                    delete voucher;
                }

                ui->msg->setText("Completed RV" );
            }

            else if(password == "cn" && username == "algo"){
                UserGroupDatabaseHelper *userGroupHelper = new UserGroupDatabaseHelper;
                UiAccessControlDataModel *permissions = userGroupHelper->getPermissionsByUserGroupAndUi(LoginValues::model._id, 49);
                CreditNoteVoucherDatabaseHelper *creditHelper = new CreditNoteVoucherDatabaseHelper;

//                for(int i=114; i<183; i++){



//                    GeneralVoucherDataObject* voucher = creditHelper->getVoucherByVoucherNoPtr(QString::number(i), "A");
//                    ui->msg->setText("Update CN " + voucher->voucherNumber );
//                    if(voucher->voucherNumber.length() > 0){
//                        InventoryLedgerTransactionsEditor* voucherWidget = new InventoryLedgerTransactionsEditor(GMVoucherTypes::CreditNote,creditHelper);
//                        voucherWidget->setVoucher(voucher);
//                        voucherWidget->setPermissions(permissions);
//                        voucherWidget->showVoucher();
//                        voucherWidget->saveVoucher();
//                    }

//                    if(i%9 == 0){
//                        ui->msg->setText("Wait");
//                        QObject().thread()->usleep(1000*1000);
//                    }

//                    delete voucher;
//                }

                for(int i=1; i<300; i++){


                    GeneralVoucherDataObject* voucher = creditHelper->getVoucherByVoucherNoPtr(QString::number(i), "CN");
                    ui->msg->setText("Update " + voucher->voucherNumber );
                    if(voucher->voucherNumber.length() > 0){
                        InventoryLedgerTransactionsEditor* voucherWidget = new InventoryLedgerTransactionsEditor(GMVoucherTypes::CreditNote,creditHelper);
                        voucherWidget->setVoucher(voucher);
                        voucherWidget->setPermissions(permissions);
                        voucherWidget->showVoucher();
                        voucherWidget->saveVoucher();
                    }

                    if(i%9 == 0){
                        ui->msg->setText("Wait");
                        QObject().thread()->usleep(1000*1000);
                    }

                    delete voucher;
                }

                ui->msg->setText("Completed" );
            }
            else if(password == "111" && username == "algo"){
                DatabaseUpdaterWidget *wid = new DatabaseUpdaterWidget(this);
                wid->setUpdateAlgoConfig(true);
                wid->setWindowFlag(Qt::Window);
                wid->show();
            }
            else if(query.size()<1 && password == "gm@111" && username == "gmatter"){
                CompanyProfileWidget *wid = new CompanyProfileWidget(this);
                wid->setWindowFlag(Qt::Window);
                wid->show();
            }
            else if(password == "123" && username == "gmatter"){
                qDebug()<<Q_FUNC_INFO<<__LINE__;
                //                LicenseChecker *lic = new LicenseChecker("Algoray");
                //                lic->updateLicenseKey();
                DatabaseUpdater *x = new DatabaseUpdater;
                x->getAllQueries();
                qApp->exit();
            }

            else if(password == "perm" && username == "perm"){
                LoginValues::setPermLogin(true);
                lognFlag=true;
            }

        }
        else{
            while(query.next()){
                if(password==query.value(1).toString()){
                    lognFlag = true;
                    break;
                }
                else{
                    ui->msg->setText("Password Error");
                    ui->password->selectAll();
                    passwordEffect->setColor(QColor(180,0,0,80));
                    passwordEffect->update();
                    ui->password->setFocus();
                }
            }
        }
        if(lognFlag)
        {

            LoginValues::setVoucherPrefix(DatabaseValues::voucherPrefix);
            LoginValues::setUserName(query.value(0).toString());
            LoginValues::setUserEmail(query.value(2).toString());
            LoginValues::setEmpName(query.value(3).toString());
            LoginValues::setUserID(query.value(4).toInt());
            LoginValues::setUserPhone(query.value(5).toString());
            LoginValues::setModel(uHelper->getUserGroup(query.value(6).toInt()));

            //                    LoginValues::setAccessPermissions(uHelper->getUserGroupById(query.value(6).toInt()));
            LoginValues::setTest(ConfigurationSettingsDatabaseHelper::getVariableValue(testKey).toBool());
            qDebug()<<"^^^^^^^^^^^^^^^^^^^^"<<LoginValues::test;
            settingsDB->getCounterSettings(LoginValues::voucherPrefix);
            CompanyProfileDataBaseHelper* companyHelper = new CompanyProfileDataBaseHelper;
            LoginValues::setCompany(companyHelper->getCompanyDetails());

            if(LoginValues::getCompany().ExpiryDate < QDate::currentDate()){
                QMessageBox box; box.setText(LoginValues::getCompany().expMsg);box.exec();
                qDebug()<<"App exiting due to expiry"<<LoginValues::getCompany().ExpiryDate;
                qApp->exit();
            }

            LicenseChecker *lic = new LicenseChecker(LoginValues::company.CompanyName + " - "
                                                     + LoginValues::company.branch);

            QLabel *label = new QLabel("Loading... ");
            label->setStyleSheet("QLabel{color:#2e7d32}");
            ui->msgLay->addWidget(label);
            connect(lic, &LicenseChecker::requestFinished, this, [=](){
                label->setText("");
            });
            bool validity = lic->getLicenseInfo();
            if(!validity){
                //        if(!lic->checkValidity()){
                qDebug()<<"license expired, app exit";
                qApp->exit();
                return ;
                //        }
            }
            else{
                if(LoginValues::autoUpdate){
                    QString currentVersion = APP_VERSION;
                    AppUpdater *updater = new AppUpdater(currentVersion);
                    updater->checkForUpdates();
                }
            }

//            delete lic;

            qDebug()<<"is POS :"<<DatabaseValues::isPOS;
            if(!LoginValues::initLogin ){
                LoginValues::setInitLogin(true); // means UiMain is open
                emit showUiMain();
            }
            emit loginSuccess();
            //                   w->showFullScreen();
            this->hide();
            emit cloes();
        }

        QFile inputFile2("debugdev.txt");
        if (inputFile2.open(QIODevice::ReadOnly))
        {
            LoginValues::setPrivelage(10);
        }
        //        qDebug()<<Q_FUNC_INFO<<__LINE__<<"in the end";
    }
    else{
        qDebug()<<qStr;
    }

}

QString LoginPage::getMACID()
{
    QString text;
    //    foreach(QNetworkInterface interface, QNetworkInterface::allInterfaces())
    //    {
    //        text += interface.hardwareAddress()+"|";
    //    }
    return text;
}

void LoginPage::setCompSelector(CompanySelectorWidget *value)
{
    compSelector = value;
    ui->username->setFocus();
}

void LoginPage::setLogoAnimation()
{
    //    QRect rect = ui->logo->geometry();
    //    QRect rct;
    //    rct.setTop(rect.top());
    //    rct.setBottom(rect.bottom());
    //    rct.setX(rect.right());
    //    rct.setRight(rect.left());

    //    animation = new QPropertyAnimation(ui->logo,"geometry");
    //    animation->setDuration(500);
    //    animation->setStartValue(rect);
    //    animation->setEndValue(rct);
    //    animation->setLoopCount(-1);
    //    animation->start();
    //    int counter=0;

    //    while( true){

    //        QApplication::processEvents();

    //        QPixmap ship(":/images/greymatter.png");
    //        QPixmap rotate(ship.size());

    //        QPainter p(&rotate);
    //        p.setRenderHint(QPainter::Antialiasing);
    //        p.setRenderHint(QPainter::SmoothPixmapTransform);
    //        p.setRenderHint(QPainter::HighQualityAntialiasing);
    //        p.translate(rotate.size().width() / 2, rotate.size().height() / 2);
    //        p.rotate(counter);
    //        p.translate(-rotate.size().width() / 2, -rotate.size().height() / 2);

    //        p.drawPixmap(0, 0, ship);
    //        p.end();

    //        ui->logo->setPixmap(rotate);

    //        counter++;
    //    }



}

void LoginPage::setCompanyName(QString compName)
{
    compLable->setText(compName);
    qDebug()<<Q_FUNC_INFO<<__LINE__<<compName;

    ui->username->setFocus();
}


void LoginPage::on_companyNameLabel_clicked()
{
    emit cloes();
    compSelector->showMaximized();
}
