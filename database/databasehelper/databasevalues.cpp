#include "databasevalues.h"
#include <QFile>
#include <QMessageBox>
#include <QSqlError>
const QString DatabaseValues::masterDbNameE="";
QString DatabaseValues::dbName = "gmrestaurant";
int DatabaseValues::dbPort=3306;
QString DatabaseValues::dbUser="user2grey";
QString DatabaseValues::dbPassword="user4uae";
QString DatabaseValues::dbHost="localhost";
QString DatabaseValues::ClientID="B";
QString DatabaseValues::voucherPrefix="A";
bool DatabaseValues::isPOS=true;
QString  DatabaseValues::idGen="X";
QString DatabaseValues::dbString="gmdb";
QString DatabaseValues::connectionString="gmdb";
bool DatabaseValues::configDbStatus=true;

DatabaseValues::DatabaseValues()
{

//    QFile file(":texts/ctrl.txt");
//    if (!file.open(QIODevice::ReadOnly))
//    {
//        qDebug() << file.errorString();
//    }
//    // QStringList wordList;
//    while (!file.atEnd()) {
//        QByteArray line = file.readLine();
//        QString values = line;
//        //   wordList.append(line.split('|').at(2));
//        QStringList array = values.split("|");
//        dbHost = array[0];
//        dbName = array[1];
//        if(array.size()>1)
//            dbPort = array[2].toInt();
//        if(array.size()>2)
//            dbUser = array[3];
//        if(array.size()>3)
//            dbPassword = array[4];
//        if(array.size()>4)
//            ClientID = array[5];
//        if(array.size()>5)
//            voucherPrefix = array[6];
//        if(array.size()>6)
//            isPOS = array[7]=="1"?true:false;
//        if(array.size()>7)
//            idGen = array[8];
//    }


    QFile inputFile("ctrl.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
        qDebug()<<"Found CTRL :";
        QString line;
        QTextStream in(&inputFile);
        while (!in.atEnd())
        {
            line  = in.readLine();
            QStringList array = line.split("|");
            if(line.split("|").size()>1){
                qDebug()<<array[0]<<array[1];
                dbHost = array[0];
                dbName = array[1];
//                dbName = "information_schema";
                if(array.size()>1)
                    dbPort = array[2].toInt();
                if(array.size()>2){
//                    dbUser = array[3];
                }
                if(array.size()>3)
                {
//                    dbPassword = array[4];
                }
                if(array.size()>4)
                    ClientID = array[5];
                if(array.size()>5)
                    voucherPrefix = array[6];
                if(array.size()>6)
                    isPOS = array[7]=="1"?true:false;
                if(array.size()>7)
                    idGen = array[8];

//                qDebug()<<array[0]<<array[1]<<array[2]<<array[3]<<array[4];
                qDebug()<<dbHost<<dbName<<dbPort<<dbUser<<dbPassword;

//                QMessageBox box; box.setText(dbHost+"-"+dbName+"-"+QString::number(dbPort)); box.exec();
            }
        }
        inputFile.close();
        //        QMessageBox box; box.setText("Host : "+dbHost);box.exec();
    }
    else{
        //        QMessageBox box; box.setText("Cannot Open File");box.exec();
    }

    // FR - cmmented on 10/10
    dbUser="user2grey";
    dbPassword = "user2grey";

    db = QSqlDatabase::addDatabase("QMYSQL","gmdb");
    db.setDatabaseName(dbName);
    db.setHostName(dbHost);
    db.setPort(dbPort);
    db.setPassword(dbPassword);
    db.setUserName(dbUser);
}

DatabaseValues::~DatabaseValues()
{
    qDebug()<<"Deleting dbValues";
}

void DatabaseValues::setConfigDbStatus(bool newConfigDbStatus)
{
    configDbStatus = newConfigDbStatus;
}

void DatabaseValues::setIsPOS(bool value)
{
    isPOS = value;
}

void DatabaseValues::setDbName(const QString &value)
{
    qDebug()<<Q_FUNC_INFO<<value;
    dbName = value;
}

QString DatabaseValues::getMasterDbNameE()
{
    return masterDbNameE;
}

void DatabaseValues::openDB()
{
//    qDebug()<<"DB NAME :"<<dbName;
//    qDebug()<<"Host :"<<dbHost;
//    qDebug()<<"Port :"<<dbPort;

    if(db.open()){

    }
    else{

        QMessageBox box;
        box.setText("Cannot find Data : "+db.lastError().text());
        box.exec();
        qDebug()<<db.lastError();
    }
}


void DatabaseValues::addDbConnection(QString dbName, QString connStr){

    qDebug()<<Q_FUNC_INFO<<dbName<<connStr;

    QString hostname = dbHost;
    QString databaseName = dbName;
    QString userName = dbUser;
    QString password = dbPassword;
    int portNo = dbPort;
    DatabaseValues::setDbName(dbName);

    QSqlDatabase::removeDatabase(connStr);
    db = QSqlDatabase::addDatabase("QMYSQL",connStr);
    db.setDatabaseName(dbName);
    db.setHostName(hostname);
    db.setPort(portNo);
    db.setPassword(userName);
    db.setUserName(password);

    openDB();
}

void DatabaseValues::removeDbConnection(QString connStr)
{
    QSqlDatabase::removeDatabase(connStr);
}


void DatabaseValues::dropDB()
{
    db.close();
}

void DatabaseValues::setConnectionString(const QString &value)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    connectionString = value;
}



