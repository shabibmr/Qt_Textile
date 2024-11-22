#include "myservice.h"

#include "gmsynchelper.h"
#include <QFile>

MyService::MyService(int argc, char **argv) : QtService<QCoreApplication>(argc,argv,"GMSYNC")
{
    try
    {
        //Set the service information
        setServiceDescription("GM SYNC Service.");
        setServiceFlags(QtServiceBase::CanBeSuspended);
        datasync = new QTimer(this);
    }
    catch(...)
    {
        qCritical() << "An unknown error in the constructor";
    }
}

MyService::~MyService()
{
    try
    {
        //TO DO - release any memory
    }
    catch(...)
    {
        qCritical() << "An unknown error in the deconstructor";
    }
}

void MyService::start()
{
    try
    {
        QCoreApplication *app = application();
        qDebug() << "Service started!";
        qDebug() << app->applicationDirPath();
        //MyClass.dosomething();
        GMSyncHelper *helper = new GMSyncHelper();
        //helper->ca();
//        callPost();
//        datasync = new QTimer();

//        QObject::connect(datasync,SIGNAL(timeout()),this,SLOT(callPost()));
//        datasync->start(5000);


    }
    catch(...)
    {
        qCritical() << "An unknown error in the start";
    }
}

void MyService::pause()
{
    try
    {
        qDebug() << "Service paused!";
    }
    catch(...)
    {
        qCritical() << "An unknown error in the pause";
    }
}

void MyService::resume()
{
    try
    {
        qDebug() << "Service resumed!";
    }
    catch(...)
    {
        qCritical() << "An unknown error in the resume";
    }
}

void MyService::stop()
{
    try
    {
        datasync->stop();
        datasync->deleteLater();
        qDebug() << "Service stopped!";
    }
    catch(...)
    {
        qCritical() << "An unknown error in the stop";
    }
}

void MyService::callPost()
{
    qDebug()<<"Call post";
    QNetworkAccessManager* mgr = new QNetworkAccessManager();
//    QObject::connect(mgr,SIGNAL(finished(QNetworkReply*)),mgr,SLOT(getPOST(QNetworkReply*)));

    QObject::connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(deleteLater()));

    mgr->get(QNetworkRequest(QUrl("http://localhost/webservice.php?action=test_insert")));

    QString filename="c:/wspace/Data1.txt";
    QFile file( filename );
    if ( file.open(QIODevice::WriteOnly | QIODevice::Append) )
    {
        QTextStream stream( &file );
        stream << QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss") << endl;
    }
    file.flush();
    file.close();
}

