#include "adduom.h"
#include "ui_adduom.h"
#include "database/Settings/ConfigurationSettingsDatabaseHelper.h"

addUOM::addUOM(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addUOM)
{
    ui->setupUi(this);

    QString bgcolor = "#e0f7fa";
   this->setObjectName("background");
   this->setStyleSheet("QWidget#background{background-color:" + bgcolor + "}");
    dbHelper = new UomDataBaseHelper();
    uomObj = new UomDataModel();
    this->setWindowTitle("Add New Unit of Measurement");
    ui->decimalPointsLineEdit->setValidator(new QDoubleValidator(0.0002,999999,2,this));
}

addUOM::~addUOM()
{
    //    delete dbHelper;
    delete ui;
}

void addUOM::setUomObj(UomDataModel *value)
{
    uomObj = value;

    if(uomObj->fromExternal && uomObj->action == 1002 && !dbHelper->checkItemExists(uomObj->_id) ){
        uomObj->action = 1001;
    }
}

void addUOM::disableSave()
{
    ui->buttonBox->hide();
}

void addUOM::editUOM(UomDataModel *item)
{
    uomObj = item;
    ui->nameLineEdit->setText(uomObj->UomName);
    ui->decimalPointsLineEdit->setText(QString::number(uomObj->decimalPoints));
    id = uomObj->_id;
    ui->symbolLineEdit->setText(uomObj->UomSymbol);
    ui->narrationLineEdit->setText(uomObj->Narration);


}

void addUOM::setTitle(QString text)
{
    ui->titleLabel->setText(text);
}

void addUOM::on_buttonBox_accepted()
{

    qDebug()<<Q_FUNC_INFO<<__LINE__<<id<<ui->nameLineEdit->text()<<uomObj->UomName;
    //    UomDataModel* item = new UomDataModel();
    uomObj->UomName = ui->nameLineEdit->text();

    uomObj->decimalPoints = ui->decimalPointsLineEdit->text().toInt();

    uomObj->_id = id;

    uomObj->Narration = ui->narrationLineEdit->text();

    uomObj->UomSymbol = ui->symbolLineEdit->text();


    qDebug()<<"Editting id "<<id;

    bool saveStatus = false;

    if(uomObj->_id == ""|| (uomObj->fromExternal == true && uomObj->action == 1001)){

        if(dbHelper->insertUOM(uomObj)){
            uomObj->action = 1001;
            saveStatus = true;
        }
    }
    else{
        if(dbHelper->updateUOM(uomObj,id)){
            uomObj->action = 1002;
            saveStatus = true;
        }
    }
    if(saveStatus && uomObj->fromExternal)
        emit NotificationAccepted();

    if(saveStatus && !uomObj->fromExternal){
        LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper();
        QStringList ledgerNames = ledHelper->getLedgersListHavingDbName();
        qDebug()<<"Ledgers to send"<<ledgerNames.size()<<ledgerNames;
        if(ledgerNames.size()> 0){
            if(ConfigurationSettingsDatabaseHelper::getValue(promptToSendToOtherLedgers,true).toBool()){
                NotificationLedgersList *ledWidget = new NotificationLedgersList();
                ledWidget->setWindowFlags(Qt::WindowStaysOnTopHint);
                ledWidget->show();
                QObject::connect(ledWidget, &NotificationLedgersList::sendNotification, this, &addUOM::sendNotifications);
                QObject::connect(ledWidget, &NotificationLedgersList::closing, this, &addUOM::on_buttonBox_rejected);
            }
            else
                sendNotifications(ledHelper->getDbNamesList());
        }

        else{
            emit closing();
            this->close();
        }

    }

    else{
        emit closing();
        this->close();
    }


}

void addUOM::on_buttonBox_rejected()
{
    emit closing();
    this->close();
}

void addUOM::sendNotifications(QStringList dbNameList)
{
    for(QString dbName:dbNameList){

        NotificationDataModel *notification = new NotificationDataModel();
        QJsonDocument doc(uomObj->ObjPtrToJson(uomObj));
        QString strJson(doc.toJson(QJsonDocument::Compact));

        notification->NotificationData = strJson;
        notification->ToDBName=dbName;
        notification->TypeOfData="UOM";
        notification->Action = uomObj->action;
        notification->GeneratedTransactionId = uomObj->_id;//QString::number(QDateTime::currentDateTime().toTime_t()) + LoginValues::voucherPrefix;

        sendChannelDatabaseHelper *sendHelper = new sendChannelDatabaseHelper();
        sendHelper->insertNotification(notification);
    }

    emit closing();
    this->close();

}
