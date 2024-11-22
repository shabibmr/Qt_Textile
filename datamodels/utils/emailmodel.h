#ifndef EMAILMODEL_H
#define EMAILMODEL_H

#include <QObject>
#include "login/loginvalues.h"

class EmailModel : public QObject
{
    Q_OBJECT
public:
    explicit EmailModel(QObject *parent = nullptr);

    QStringList toMailList;
    QStringList filesList;

    QString subject;
    QString body;

    QString fromAddress=LoginValues::getCompany().email;
    QString password="Passcode-1";

    int port=465;
    QString server= "smtp.gmail.com";

signals:

};

#endif // EMAILMODEL_H
