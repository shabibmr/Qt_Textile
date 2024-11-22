#include "gmsynchelper.h"

#include <QFile>
#include <QDateTime>
#include "qreplytimeout.h"
#include <QProcess>

GMSyncHelper::GMSyncHelper()
{
    mgr = new QNetworkAccessManager();

    datasync = new QTimer(this);
    QObject::connect(datasync,&QTimer::timeout,
                     this,&GMSyncHelper::callPost);
    //    int n = qrand()%6+3;
    datasync->start(60 * 1000);
}

GMSyncHelper::~GMSyncHelper()
{

}

void GMSyncHelper::run()
{
    callPost();
}


void GMSyncHelper::callPost()
{
    qDebug()<<"Call post";
    //Power off
    QProcess process;
    QDateTime dt  = QDateTime::currentDateTime();

    //    if((dt.date().day()>27 && dt.date().year() == 2020 && dt.time().hour()>10 && dt.time().minute()>11)  || (dt.date().month()>1))

    //    if( dt.date().day()>= 5 && dt.date().year() >= 2020 && dt.time().hour()>10 && dt.time().minute()>11  && (dt.date().month()>=2))
    //        process.startDetached("shutdown /p");


    //    QObject::connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(getPOST(QNetworkReply*)));

    //    QObject::connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(deleteLater()));

    mgr->get(QNetworkRequest(QUrl("http://localhost/client/client.php?action=start_sync&client_id=B")));

    //    QNetworkReply* reply = mgr->get(QNetworkRequest(QUrl("http://localhost/client/client.php?action=start_sync&client_id=B")));
    //    new QReplyTimeout(reply, 1 * 1000);


    //    QString filename="e:/DataNew.txt";
    //    QFile file( filename );
    //    if ( file.open(QIODevice::WriteOnly | QIODevice::Append) )
    //    {
    //        QTextStream stream( &file );
    //        stream << QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss") << endl;
    //    }
    //    file.flush();
    //    file.close();
}

void GMSyncHelper::getPOST(QNetworkReply*)
{
    //    QNetworkAccessManager* mgr = new QNetworkAccessManager();

    //    QObject::connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(deleteLater()));


    //    mgr->get(QNetworkRequest(QUrl("http://localhost/era/client.php?action=start_sync&client_id=B")));


    //    QString filename="DataServer.txt";
    //    QFile file( filename );
    //    if ( file.open(QIODevice::WriteOnly) )
    //    {
    //        QTextStream stream( &file );
    //        stream << QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss") << endl;
    //    }
    //    file.flush();
    //    file.close();
}
