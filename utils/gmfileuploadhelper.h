#ifndef GMFILEUPLOADHELPER_H
#define GMFILEUPLOADHELPER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QFile>
#include <QNetworkRequest>

class GMFileUploadHelper : public QObject
{
    Q_OBJECT
public:
    explicit GMFileUploadHelper(QString m_fileName,QString fileName,QObject *parent = nullptr);

    ~GMFileUploadHelper();

    QString justFileName;


public slots:
//    void on_uploadFile();             // Upload slot
    void uploadFinished(QNetworkReply *reply);  // Upload finish slot
    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);  // Upload progress slot
    void downloadFinished(QNetworkReply *reply);  // Upload finish slot

    void multipart();

    void readFile(QString fileName);


private:

    QNetworkAccessManager *m_manager;
    QNetworkAccessManager *r_manager;

    QString m_fileName;
    QNetworkRequest* request;
    QString serverIP = "localhost";
    QString fileUploadURL="http://localhost/saveimg.php";
    // You must save the file on the heap
    // If you create a file object on the stack, the program will crash.
    QFile *m_file;

signals:

    void progressSignal(qint64 bytesSent, qint64 bytesTotal);

    void uploadedSignal(bool status,QString msg);

    void downLoaded(QByteArray data);

};

#endif // GMFILEUPLOADHELPER_H
