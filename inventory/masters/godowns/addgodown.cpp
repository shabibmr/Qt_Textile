#include "addgodown.h"
#include "ui_addgodown.h"

addGodown::addGodown(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addGodown)
{
    ui->setupUi(this);
    this->setWindowTitle("Create New Godown");

    QString bgcolor = "#e0f7fa";
   this->setObjectName("background");
   this->setStyleSheet("QWidget#background{background-color:" + bgcolor + "}");
    dbHelper =new GodownDatabaseHelper();
    godown = new GodownDataObject();
}

addGodown::~addGodown()
{
    delete ui;
}

void addGodown::setGodown(GodownDataObject *value)
{
    godown = value;
}

void addGodown::disableSave()
{
    ui->buttonBox->hide();
}

void addGodown::edit()
{
    ui->nameLineEdit->setText(godown->GodownName);
    ui->descriptionLineEdit->setText(godown->GodownNarration);
    ui->locationLineEdit->setText(godown->GodownLocation);
    ui->ProfitCentreCheckBox->setChecked(godown->isProfitCentre);
    ui->isDefaultCheckbox->setChecked(godown->isDefault);
    ui->productionCheckBox->setChecked(godown->isProductionCentre);
    id=godown->GodownID;

}

void addGodown::setTitle(QString text)
{
    ui->titleLabel->setText(text);
}

void addGodown::on_buttonBox_accepted()
{
    //    GodownDataObject* obj = new GodownDataObject();

    godown->GodownID = id;
    godown->GodownName = ui->nameLineEdit->text();
    godown->GodownNarration = ui->descriptionLineEdit->text();
    godown->GodownLocation = ui->locationLineEdit->text();
    godown->isProfitCentre = ui->ProfitCentreCheckBox->isChecked();
    godown->isDefault = ui->isDefaultCheckbox->isChecked();
    godown->isProductionCentre = ui->productionCheckBox->isChecked();
    if(id == "" || (godown->fromExternal == true && godown->action == 1001)){
        dbHelper->insertGodown(godown);
        godown->action =1001;
    }
    else{
        dbHelper->updateGodown(godown,id);
        godown->action =1002;
    }

//    if(godown->fromExternal == false){
//        LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper();
//        QStringList ledgerNames = ledHelper->getLedgersListHavingDbName();
//        qDebug()<<"Ledgers to send"<<ledgerNames.size()<<ledgerNames;
//        if(ledgerNames.size()> 0){
//            NotificationLedgersList *ledWidget = new NotificationLedgersList();
//            ledWidget->setWindowTitle("Select Branches");
//            ledWidget->setWindowModality(Qt::ApplicationModal);
//            ledWidget->setWindowFlags(Qt::WindowStaysOnTopHint);
//            ledWidget->show();
//            QObject::connect(ledWidget, &NotificationLedgersList::sendNotification, this, &addGodown::sendNotifications);
//            QObject::connect(ledWidget, &NotificationLedgersList::closing, this, &addGodown::on_buttonBox_rejected);
//        }

//        else{
//            this->close();
//            emit closing();
//        }

//    }

//    else{
        this->close();
        emit closing();
//    }
//    emit closing();
}

void addGodown::on_buttonBox_rejected()
{
    this->close();
}

void addGodown::sendNotifications(QStringList dbNameList)
{
    for(QString dbName:dbNameList){
        //        qDebug()<<"Insert notification data to send"<<dbName;

        NotificationDataModel *notification = new NotificationDataModel();
        QJsonDocument doc(godown->objPtrToJson(godown));
        QString strJson(doc.toJson(QJsonDocument::Compact));

        notification->NotificationData = strJson;
        notification->ToDBName=dbName;
        notification->TypeOfData="Godown";
        notification->Action = godown->action;
        notification->GeneratedTransactionId=QString::number(QDateTime::currentDateTime().toTime_t()) + LoginValues::voucherPrefix;

        sendChannelDatabaseHelper *sendHelper = new sendChannelDatabaseHelper();
        sendHelper->insertNotification(notification);
    }

    this->close();
    emit closing();

}
