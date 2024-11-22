#include "notificationledgerslist.h"
#include "ui_notificationledgerslist.h"

NotificationLedgersList::NotificationLedgersList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NotificationLedgersList)
{
    ui->setupUi(this);

    ledHelper = new LedgerMasterDatabaseHelper();
    QStringList ledgerNames = ledHelper->getLedgersListHavingDbName();

    qDebug()<<"ledgers:*************************"<<ledgerNames;
    for(QString led:ledgerNames){
        QCheckBox *checkBox = new QCheckBox;
        checkBox->setText(led);
        checkBox->show();
        ui->verticalLayout->addWidget(checkBox);
        QObject::connect(checkBox, SIGNAL(stateChanged(int)), this,SLOT(addDbNameToList(int)));
    }
}

NotificationLedgersList::~NotificationLedgersList()
{
    delete ui;
}


void NotificationLedgersList::addDbNameToList(int state)
{
    QString ledger = sender()->property("text").toString();
    qDebug()<<"selected"<<ledger;
    qDebug()<<state;

    if(state == 2){
        dbNameList.append(ledHelper->getDbNameByLedgerName(ledger));
    }

}


void NotificationLedgersList::on_SendPushButton_clicked()
{
    qDebug()<<"Selected DBs to send: "<<dbNameList;
    emit sendNotification(dbNameList);
    this->close();
}

void NotificationLedgersList::on_CancelPushButton_clicked()
{
    emit closing();
    this->close();
}
