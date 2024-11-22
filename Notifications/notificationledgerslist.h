#ifndef NOTIFICATIONLEDGERSLIST_H
#define NOTIFICATIONLEDGERSLIST_H

#include <QWidget>
#include <QCheckBox>

#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"

namespace Ui {
class NotificationLedgersList;
}

class NotificationLedgersList : public QWidget
{
    Q_OBJECT

public:
    explicit NotificationLedgersList(QWidget *parent = nullptr);
    ~NotificationLedgersList();

    QStringList dbNameList;
    LedgerMasterDatabaseHelper* ledHelper;

private slots:
    void addDbNameToList(int state);

    void on_SendPushButton_clicked();

    void on_CancelPushButton_clicked();

private:
    Ui::NotificationLedgersList *ui;
signals:
    void sendNotification(QStringList);
    void closing();
};

#endif // NOTIFICATIONLEDGERSLIST_H
