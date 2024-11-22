#include "notificationshistory.h"
#include <QScreen>
#include <QApplication>

NotificationsHistory::NotificationsHistory(QWidget *parent) : QWidget(parent)
{
    this->setWindowTitle("Notifications History");
    this->setWindowFlag(Qt::Popup);

    this->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, this->size(),
                                          qApp->primaryScreen()->geometry()));

    QVBoxLayout *tableLayout = new QVBoxLayout;
    this->setLayout(tableLayout);

    tableView = new ReportsTableView("");
    tableView->lastRow->hide();
//    connect(tableView, &QTableView::doubleClicked, this, &NotificationsList::on_tableView_doubleClicked);
    tableLayout->addWidget(tableView);

    rHelper = new receiveChannelDatabaseHelper();

    setMinimumSize(600,400);
    tableLayout->setContentsMargins(0,0,0,0);

    model = new QSqlQueryModel(this);
    setTableView();

    tableView->horizontalHeader() ->setSectionResizeMode(QHeaderView::Stretch);
    tableView->hideColumn(0);
}

void NotificationsHistory::setTableView()
{

    model = rHelper->getAllNotifications();
    NotificationsDelegate *delegate = new NotificationsDelegate(model, true);
    connect(delegate, &NotificationsDelegate::markRead, this, &NotificationsHistory::markUnRead);

    tableView->setItemDelegate(delegate);
    tableView->setModel(model);

    tableView->setColumnWidth(7,25);


    //    ui->tableLayout->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
 }

void NotificationsHistory::markUnRead(int row)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Notifications","Do you want to mark notification as unread?",
                                  QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::Yes){
        int id = model->record(row).value(0).toInt();
        QString transId = model->record(row).value(7).toString();

        rHelper->updateNotificationStatus(id, transId, QuotationStatus::NotificationUnRead);
        emit refresh();

        setTableView();


    }

    else
        return;

}
