#ifndef EMAILTHREAD_H
#define EMAILTHREAD_H

#include <QThread>
#include <QtCore>
#include "login/loginvalues.h"


class EmailThread : public QObject
{
    Q_OBJECT
public:
    explicit EmailThread(QObject *parent = nullptr);
    void run();

    QString uname="reports_ecuisine@algoray.in";//LoginValues::getCompany().email;//ecuisineauto
    QString password="Passcode-1";
    int port=465;
    QString server= "smtp.titan.email";//"smtp.gmail.com";
    QStringList files;
    QString toAddress = "", subject = "Counter Open",Message="hi";
    void setToAddress(const QString &value);

public slots:
    void mailSent(QString s);

};

#endif // EMAILTHREAD_H
