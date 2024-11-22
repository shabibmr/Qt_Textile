#include "companyselectorwidget.h"
#include "ui_companyselectorwidget.h"
#include <QToolButton>
#include <QListWidget>
#include "database/databasehelper/databasevalues.h"
#include "customwidgets/Misc/customshadoweffect.h"
#include "login/loginpage.h"
#include "utils/SettingsHelper/settingshelper.h"

QStringList CompanySelectorWidget::loggedInBranches = QStringList();



CompanySelectorWidget::CompanySelectorWidget(bool isRestart, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompanySelectorWidget)
{
    ui->setupUi(this);

    this->setWindowTitle("Select Company");
    initDBHelpers();
    box = new QCheckBox(this);

    if(isRestart)
        defaultDbFound  =true;
    //    setView();

}

CompanySelectorWidget::~CompanySelectorWidget()
{
    delete ui;
}

void CompanySelectorWidget::initDBHelpers()
{
    dv.addDbConnection("information_schema", "info");
    dbHelper= new CompanyProfileDataBaseHelper();


    cList = dbHelper->getAvailableCompanies();
    //    qDebug()<<Q_FUNC_INFO<<__LINE__<<cList;

    for(int i=0; i<cList.size(); i++){
        QPair<QString, QString> pair;
        pair.first = cList[i][1];
        pair.second = cList[i][0]+"#"+QString::number(i);
        cMap.insert(cList[i][0], pair );
        //        dv.addDbConnection(cList[i][0], cList[i][0]+"_"+QString::number(i));
    }

    //    qDebug()<<Q_FUNC_INFO<<__LINE__<<QSqlDatabase::connectionNames();
}

void CompanySelectorWidget::setView()
{
    //    qDebug()<<Q_FUNC_INFO<<__LINE__;
    QLayout *lay = new QVBoxLayout(this);

    for(int i=0;i<cList.size();i++){
        QtMaterialFlatButton *selectButton = new QtMaterialFlatButton(this);
        selectButton->setText(cList[i][1]);
        selectButton->setTextAlignment(Qt::AlignLeft);
        selectButton->setIcon(QtMaterialTheme::icon("social", "domain") );
        selectButton->setProperty("pos", i);
        connect(selectButton,&QtMaterialFlatButton::clicked,[=](bool v){
            //            int i = sender()->property("pos").toInt();
            qDebug()<<Q_FUNC_INFO<<__LINE__<<cList[i][1]<<cList[i][0];
            switchDB(cList[i][0]);
            this->close();

        });
        lay->addWidget(selectButton);
        //        lay->addSpacerItem(new QSpacerItem(10,10));
    }

    QScrollArea *scroll = new QScrollArea();
    scroll->setObjectName("POSMain");

    scroll->setStyleSheet("QWidget#POSMain{background-color:#e0f7fa}");


    scroll->verticalScrollBar()->
            setStyleSheet(QString::fromUtf8("QScrollBar:vertical {"
                                            "    border: 1px solid #999999;"
                                            "    background:white;"
                                            "    width:20px;    "
                                            "    margin: 0px 0px 0px 0px;"
                                            "}"
                                            "QScrollBar::handle:vertical {"
                                            "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
                                            "    stop: 0  rgb(0,255,0), stop: 0.5 rgb(0,255,0),  stop:1 rgb(0,255,0));"
                                            "    min-height: 0px;"
                                            ""
                                            "}"
                                            "QScrollBar::add-line:vertical {"
                                            "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
                                            "    stop: 0  rgb(0,255,0), stop: 0.5 rgb(0,255,0),  stop:1 rgb(0,255,0));"
                                            "    height: px;"
                                            "    subcontrol-position: bottom;"
                                            "    subcontrol-origin: margin;"
                                            "}"
                                            "QScrollBar::sub-line:vertical {"
                                            "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
                                            "    stop: 0  rgb(0,255,0), stop: 0.5 rgb(0,255,0),  stop:1 rgb(0,255,0));"
                                            "    height: 0px;"
                                            "    subcontrol-position: top;"
                                            "    subcontrol-origin: margin;"
                                            "}"
                                            ""));



    scroll->setLayout(lay);
    scroll->setAlignment(Qt::AlignCenter);
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(scroll);
    ui->verticalLayout->addLayout(vLayout);

    //    qDebug()<<Q_FUNC_INFO<<__LINE__;

    //    qDebug()<<Q_FUNC_INFO<<__LINE__;
    QHBoxLayout *hLay = new QHBoxLayout;
    //    qDebug()<<Q_FUNC_INFO<<__LINE__;
    hLay->addWidget(new QLabel("Set as Default"));
    //    qDebug()<<Q_FUNC_INFO<<__LINE__;
    hLay->addWidget(box);
    //    qDebug()<<Q_FUNC_INFO<<__LINE__;
    hLay->addStretch(1);
    //    qDebug()<<Q_FUNC_INFO<<__LINE__;

    ui->verticalLayout->addLayout(hLay);
    //    qDebug()<<Q_FUNC_INFO<<__LINE__;
}

QString CompanySelectorWidget::getCompanyName(QString dbName)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__<<dbName<<"DBs Count : "<<cMap.size();

    QMapIterator<QString, QPair<QString, QString>> i(cMap);
    while(i.hasNext()){
        i.next();
        qDebug()<<dbName<<"SECOND"<<i.value().second<<dbName;
        if(i.value().second.startsWith(dbName+"#",Qt::CaseInsensitive)){
            qDebug()<<"found";
            qDebug()<<Q_FUNC_INFO<<__LINE__<<i.value().first;
            return i.value().first;
        }
    }

    return LoginValues::company.CompanyName;
}

QString CompanySelectorWidget::getDbName(QString dbStr)
{
//        qDebug()<<Q_FUNC_INFO<<__LINE__<<dbStr<<cMap;

    QMapIterator<QString, QPair<QString, QString>> i(cMap);
    while(i.hasNext()){
        i.next();
        if(i.value().second == dbStr)
            return i.key();
    }
    return DatabaseValues::dbName;
}

bool CompanySelectorWidget::selectDefaultBranch()
{
    dv.removeDbConnection("info");
    dv.removeDbConnection("algo_config");

    //    QSettings settings("Algoray", "GM.ERP");
    SettingsHelper settings;

    //    QString dbName =   settings.getValue(valString).toString();
    //    qDebug()<<Q_FUNC_INFO<<__LINE__<<"default db is "<<dbName;

    // get current working directory
    // defaultDBKey would be CWD_DefaultDBName

    qDebug()<<Q_FUNC_INFO<<qApp->applicationDirPath();

    QString dir =qApp->applicationDirPath().split("/").last();
    qDebug()<<Q_FUNC_INFO<<__LINE__<<dir;

    defaultDBKey = dir + "_DefaultDBName";

    if(!settings.keyExist(defaultDBKey)){
        if(settings.keyExist("DefaultDBName")){
            settings.setValue(defaultDBKey,settings.getValue("DefaultDBName").toString() ) ;
        }
        else
            return false;
    }
//    else {
        QString dbName = settings.getValue(defaultDBKey).toString();
        if(cMap.contains(dbName)){
            defaultDbFound = true;
            switchDB(dbName);
            //            qDebug()<<Q_FUNC_INFO<<__LINE__;
            return true;
        }
        else{
            //            qDebug()<<Q_FUNC_INFO<<__LINE__<<"reset key";
//            settings.resetKey(defaultDBKey);
            return false;
        }

//    }
}

void CompanySelectorWidget::switchDB(QString dbName)
{
    //    qDebug()<<Q_FUNC_INFO<<cMap[dbName].second;

    QString errorData = " \n switch DB " + dbName;

    dv.removeDbConnection("gmdb"); // removing initial connection
    dv.removeDbConnection("info");
    DatabaseValues::setConnectionString(cMap[dbName].second);
    DatabaseValues::setDbName(dbName);
    if(!loggedInBranches.contains(dbName)){
        qDebug()<<Q_FUNC_INFO<<__LINE__;
        dv.addDbConnection(dbName, cMap[dbName].second);
        errorData += " \n emit login required " + cMap[dbName].second;
        emit loginRequired(dbName);
        // show login page
    }
    else{
        errorData += " \n emit company switchd " + cMap[dbName].first;
        emit companySwitched(cMap[dbName].first);
    }
    //    qDebug()<<Q_FUNC_INFO<<__LINE__<<DatabaseValues::connectionString;

    setDefaultBranch(dbName);


    QFile File("restartlog.txt");
    File.open(QFile::WriteOnly | QFile::Text | QFile::Append);

    QString functionName = Q_FUNC_INFO;
    File.write(errorData.toUtf8());
    File.close();

    //    if(loginRequired)
    //        showLoginPage();

}

void CompanySelectorWidget::setDefaultBranch(QString dbName)
{
    //    QSettings settings("Algoray", "GM.ERP");
    SettingsHelper settings;
    QString defaultBranch = settings.getValue(defaultDBKey).toString();
    if(defaultBranch.length() == 0 || box->isChecked() || !defaultDbFound){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this,"Set Default","Set " + cMap[dbName].first + " as default? ",
                                      QMessageBox::Yes|QMessageBox::No);
        if(reply==QMessageBox::Yes){

            settings.setValue(defaultDBKey,dbName);
            defaultDbFound = true;
            //            settings.sync();
        }

        else
            return;
    }

}

int CompanySelectorWidget::getLoggedInUserByBranch(QString dbName)
{
    return branchUserMap[dbName];

}

//void CompanySelectorWidget::showLoginPage()
//{
//    qDebug()<<Q_FUNC_INFO<<__LINE__<<loggedInBranches<<DatabaseValues::dbName;
//    if(!loggedInBranches.contains(DatabaseValues::dbName)){
//        qDebug()<<Q_FUNC_INFO<<__LINE__;
//        emit loginRequired();
//        // show login page
//    }

//}

void CompanySelectorWidget::addLoggedBranch()
{
    loggedInBranches.append(DatabaseValues::dbName);
    qDebug()<<Q_FUNC_INFO<<__LINE__<<loggedInBranches;
    branchUserMap.insert(DatabaseValues::dbName, LoginValues::userID);

}


