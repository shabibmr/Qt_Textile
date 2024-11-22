#include "gmfileuploadhelper.h"
#include <QFileInfo>
#include <QNetworkReply>
#include <QMetaType>
#include <QFile>
#include <QDataStream>
#include <QUrlQuery>
#include <QHttpPart>
#include <QJsonObject>
#include <QJsonDocument>
#include <QThread>
#include <database/Settings/ConfigurationSettingsDatabaseHelper.h>

GMFileUploadHelper::GMFileUploadHelper(QString m_fileName,QString fileName,QObject *parent) : QObject(parent)
{
    this->justFileName=fileName;
    m_manager = new QNetworkAccessManager(this);
    connect(m_manager, &QNetworkAccessManager::finished, this, &GMFileUploadHelper::uploadFinished);
    this->m_fileName =m_fileName;
    serverIP = ConfigurationSettingsDatabaseHelper::getValue(inventoryImageUploadURL,"localhost").toString();
    fileUploadURL = "http://"+serverIP+"/saveimg.php";

}

GMFileUploadHelper::~GMFileUploadHelper()
{

}


void GMFileUploadHelper::uploadFinished(QNetworkReply *reply)
{
    qDebug()<<"DONE";
    qDebug()<<QString(reply->readAll());
    // If the upload was successful, then we display the answer from the server
    if(reply->error() == QNetworkReply::NoError)
    {
        qDebug()<<__LINE__<<"Uploaded"<<reply->readAll();
        emit uploadedSignal(true,"");
    }
    else
    {
        qDebug() << "ERROR!"<<reply->url().toString()<<fileUploadURL;
        qDebug() << reply->errorString();
        emit uploadedSignal(false,reply->errorString()+ " to "+fileUploadURL);

    }
}

void GMFileUploadHelper::uploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    emit progressSignal(bytesSent,bytesTotal);

}

void GMFileUploadHelper::downloadFinished(QNetworkReply *reply)
{
    qDebug()<<"Downloaded";
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray data=reply->readAll();
        qDebug()<<__LINE__<<"Downloaded Size : "<<reply->readAll().size();

        emit downLoaded(data);
    }
    else
    {
        qDebug() << "ERROR!"<<reply->url().toString();
        qDebug() << reply->errorString();
        emit uploadedSignal(false,"Download Error :"+reply->errorString());

    }
}

void GMFileUploadHelper::multipart()
{

    m_file = new QFile(m_fileName);


    if (m_file->open(QIODevice::ReadOnly))
    {
        QJsonObject filejson;
        filejson.insert("fileName",justFileName);
        QJsonValue value(m_file->readAll().toBase64().constData());

        filejson.insert("image",  value);
        request = new QNetworkRequest();

        request->setUrl(QUrl(fileUploadURL+"?action=save_image"));
        request->setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        // Start upload
        qDebug()<<"Uploading"<<Q_FUNC_INFO<<__LINE__;
        QJsonDocument doc(filejson);
        QByteArray data = doc.toJson();
        QNetworkReply *reply = m_manager->post(*request, data);


        // And connect to the progress upload signal
        connect(reply, &QNetworkReply::uploadProgress, this, &GMFileUploadHelper::uploadProgress);
        return;
    }
    else{
        qDebug()<<"File Read Error : "<<m_file->errorString()<<m_file->fileName();
    }

}

void GMFileUploadHelper::readFile(QString fileName)
{

    r_manager = new QNetworkAccessManager();
    connect(r_manager, &QNetworkAccessManager::finished, this, &GMFileUploadHelper::downloadFinished,Qt::QueuedConnection);

    request = new QNetworkRequest();

    request->setUrl(QUrl("http://"+serverIP+"/Images/"+fileName));
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Start upload
    qDebug()<<"Downloading"<<__LINE__<<Q_FUNC_INFO;
    QNetworkReply *reply = r_manager->get(*request );

}

