#ifndef QREPLYTIMEOUT_H
#define QREPLYTIMEOUT_H

#include <QObject>
#include <QNetworkReply>
#include <QTimer>
#include <QDebug>

class QReplyTimeout : public QObject {
    Q_OBJECT
public:
    QReplyTimeout(QNetworkReply* reply, const int timeout) : QObject(reply) {
        Q_ASSERT(reply);
        if (reply) {
            QTimer::singleShot(timeout, this, SLOT(timeout()));
        }
    }

private slots:
    void timeout() {
        QNetworkReply* reply = static_cast<QNetworkReply*>(parent());
        if (reply->isRunning()) {
            qDebug()<<"CLOSING TIMEOUT";
            reply->close();
        }
    }
};
#endif // QREPLYTIMEOUT_H
