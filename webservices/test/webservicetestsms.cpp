#include "webservicetestsms.h"


webservicetestSMS::webservicetestSMS(QString Message, QStringList Numbers)
{
    this->Message = Message;
    this->Numbers =Numbers;
}

void webservicetestSMS::testWeb()
{
    const QString url ="https://control.msg91.com/api/postsms.php";

    const QUrl URL = QUrl(url);

    QNetworkRequest request(URL);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("authkey", "294557A82vI9urVR7T5d80cafe");
    QNetworkAccessManager nam;

    QNetworkReply *reply = nam.post(request,"");

    while (!reply->isFinished())
    {
        qDebug()<<"Got Reply";
        qApp->processEvents();
    }

    QByteArray response_data = reply->readAll();

    QJsonDocument json2 = QJsonDocument::fromJson(response_data);

    qDebug()<<"Response: ";
    qDebug()<<json2;
    reply->deleteLater();
}

void webservicetestSMS::sendSMS()
{

    const QString url ="https://api.msg91.com/api/v2/sendsms";

    const QUrl URL = QUrl(url);

    QNetworkRequest request(URL);

    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("authkey", "294557AoaasHnnMv85e590136P1");
    QJsonObject json = getJson();
    QNetworkAccessManager nam;

    //    QString xml = "<MESSAGE> "
    //                  "<AUTHKEY>294584AP1MsGx3dl5d80fd0d</AUTHKEY> "
    //                  "<SENDER>Algory</SENDER>"
    //                  " <ROUTE>Template</ROUTE> "
    //                  "<CAMPAIGN>XML API</CAMPAIGN> "
    //                  "<COUNTRY>91</COUNTRY> "
    //                  "<SMS TEXT=\"\" >"
    //                  " <ADDRESS TO=\"9656904077\"></ADDRESS>"
    //                  " <ADDRESS TO=\"9947887979\"></ADDRESS>"
    //                  " </SMS> </MESSAGE>";

    qDebug()<<json;

    QNetworkReply *reply = nam.post(request, QJsonDocument(json).toJson());

    while (!reply->isFinished())
    {
        qApp->processEvents();
    }

    QByteArray response_data = reply->readAll();

    QJsonDocument json2 = QJsonDocument::fromJson(response_data);

    qDebug()<<"Response:";
    qDebug()<<json2;
    reply->deleteLater();

}

QJsonObject webservicetestSMS::getJson()
{
    QJsonObject json;
    json.insert("sender", "CakeSt");
    json.insert("route", "4");
    json.insert("country", "91");

    QJsonArray jsonArray;
    QJsonArray jsonArrayofMessages;


    QJsonObject messagex;
    messagex.insert("message",Message);

    QJsonArray jsonArrayNumbers;
    for(QString number:Numbers)
        if(number.length()==10)
            jsonArrayNumbers.append(QJsonValue(number));



    messagex.insert("to",jsonArrayNumbers);
    jsonArrayofMessages.append(messagex);
    json.insert("sms",jsonArrayofMessages);

    return  json;
}




