#ifndef WEBSERVICETESTSMS_H
#define WEBSERVICETESTSMS_H
#include <QtNetwork>

class webservicetestSMS
{
public:
    webservicetestSMS(QString,QStringList);

    void sendSMS();

    void testWeb();
private:
    QString Message;
    QStringList Numbers;

    QJsonObject getJson();
};

#endif // WEBSERVICETESTSMS_H
