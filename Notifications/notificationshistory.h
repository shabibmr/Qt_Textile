#ifndef NOTIFICATIONSHISTORY_H
#define NOTIFICATIONSHISTORY_H

#include <QWidget>
#include "customwidgets/TableViews/reportstableview.h"
#include "database/Notifications/receivechanneldatabasehelper.h"
#include "customwidgets/Delegates/Notifications/notificationsdelegate.h"
#include <QMessageBox>
#include <QVBoxLayout>

class NotificationsHistory : public QWidget
{
    Q_OBJECT
public:
    explicit NotificationsHistory(QWidget *parent = nullptr);

    QSqlQueryModel *model;
    ReportsTableView *tableView;

    receiveChannelDatabaseHelper *rHelper;

signals:
    void refresh();

public slots:
    void setTableView();
    void markUnRead(int row);
};

#endif // NOTIFICATIONSHISTORY_H
