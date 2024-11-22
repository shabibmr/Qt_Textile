#ifndef GMSYNCHELPER_H
#define GMSYNCHELPER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QTimer>



class GMSyncHelper : public QObject
{
    Q_OBJECT

public:
    GMSyncHelper();
    ~GMSyncHelper();

    void run();


    QStringList URLS;

public slots:
    void callPost();
    void getPOST(QNetworkReply *);
private:
    QTimer *datasync;
    QNetworkAccessManager* mgr;

};

#endif // GMSYNCHELPER_H
