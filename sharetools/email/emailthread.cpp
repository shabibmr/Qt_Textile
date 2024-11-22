#include "emailthread.h"
#include "SMTP/gmail/smtp.h"
#include <QMessageBox>
#include <database/Settings/ConfigurationSettingsDatabaseHelper.h>


EmailThread::EmailThread(QObject *parent) : QObject (parent)
{
//    server = ConfigurationSettingsDatabaseHelper::getValue(emailsmtp,"reports@algoray.in").toString();
}

void EmailThread::run(){
    if(uname.length()<3|| toAddress.length()<3)
        return;


    qDebug()<<uname;
    qDebug()<<password;
    Smtp* smtp = new Smtp(uname,password,server, port);

    QObject::connect(smtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)),Qt::QueuedConnection);

    if( !files.isEmpty() )
        smtp->sendMail(uname, toAddress, subject,Message, files );
    else
        smtp->sendMail(uname, toAddress, subject,Message);

}

void EmailThread::setToAddress(const QString &value)
{
    toAddress = value;
}


void EmailThread::mailSent(QString s)
{
    qDebug()<<Q_FUNC_INFO;
    QMessageBox box; box.setText(s+" to "+toAddress); box.exec();
}



