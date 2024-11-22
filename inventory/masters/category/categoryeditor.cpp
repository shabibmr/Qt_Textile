#include "categoryeditor.h"

#include "Notifications/notificationledgerslist.h"
#include "database/Settings/ConfigurationSettingsDatabaseHelper.h"

CategoryEditor::CategoryEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CategoryEditor)
{
    ui->setupUi(this);
    dbHelper = new BrandsDatabaseHelper();
    brand = new BrandsDataModel;
}

CategoryEditor::~CategoryEditor()
{
    delete ui;
}

void CategoryEditor::setBrand(BrandsDataModel *value)
{
    brand = value;
    if(brand->fromExternal && brand->action == 1002 && !dbHelper->checkItemExists(brand->brandID) ){
        brand->action = 1001;
    }
}

void CategoryEditor::disableSave()
{
    ui->buttonBox->hide();
}

void CategoryEditor::on_buttonBox_accepted()
{

    brand->brandName = ui->nameLineEdit->text();
    brand->brandDesc = ui->descEdit->text();

    bool saveStatus = false;

    qDebug()<<brand->brandID<<brand->fromExternal<<brand->action;

    if(brand->brandID == 0 || (brand->fromExternal == true && brand->action == 1001)){
        if(dbHelper->insertBrand(brand))
        {
            brand->action = 1001;
            saveStatus = true;
        }
    }
    else{
        if(dbHelper->updateBrand(brand,QString::number(brand->brandID)))
        {

            brand->action = 1002;
            saveStatus = true;

        }
    }
    if(saveStatus && brand->fromExternal)
        emit NotificationAccepted();

    if( saveStatus && brand->fromExternal == false){
        LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper();
        QStringList ledgerNames = ledHelper->getLedgersListHavingDbName();
        qDebug()<<"Ledgers to send"<<ledgerNames.size()<<ledgerNames;
        if(ledgerNames.size()> 0){
            if(ConfigurationSettingsDatabaseHelper::getValue(promptToSendToOtherLedgers,true).toBool()){

                NotificationLedgersList *ledWidget = new NotificationLedgersList();
                ledWidget->setWindowFlags(Qt::WindowStaysOnTopHint);
                ledWidget->show();
                QObject::connect(ledWidget, &NotificationLedgersList::sendNotification, this, &CategoryEditor::sendNotifications);
                QObject::connect(ledWidget, &NotificationLedgersList::closing, this, &CategoryEditor::on_buttonBox_rejected);
            }
            else
                sendNotifications(ledgerNames);
        }

        else{
            this->close();
            emit closing();
        }

    }
    else{
        this->close();
        emit closing();
    }
    emit closing();
}

void CategoryEditor::on_buttonBox_rejected()
{
    this->close();

}

void CategoryEditor::edit()
{

    ui->nameLineEdit->setText(brand->brandName);
    ui->descEdit->setText(brand->brandDesc);



}

void CategoryEditor::sendNotifications(QStringList dbNameList)
{
    for(QString dbName:dbNameList){
        //        qDebug()<<"Insert notification data to send"<<dbName;

        NotificationDataModel *notification = new NotificationDataModel();
        QJsonDocument doc(brand->objPtrToJson(brand));
        QString strJson(doc.toJson(QJsonDocument::Compact));

        notification->NotificationData = strJson;
        notification->ToDBName=dbName;
        notification->TypeOfData="Brand";
        notification->Action = brand->action;
        notification->GeneratedTransactionId= QString::number(brand->brandID); //QString::number(QDateTime::currentDateTime().toTime_t()) + LoginValues::voucherPrefix;

        sendChannelDatabaseHelper *sendHelper = new sendChannelDatabaseHelper();
        if(sendHelper->insertNotification(notification))
            emit NotificationAccepted();
    }

    this->close();

    emit closing();

}
