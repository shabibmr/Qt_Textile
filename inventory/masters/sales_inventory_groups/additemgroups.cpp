#include "additemgroups.h"
#include "ui_additemgroups.h"
#include "customwidgets/gmsearchlineedit.h"
#include "QCompleter"

#include "database/Settings/ConfigurationSettingsDatabaseHelper.h"

#include <customwidgets/Material/components/qtmaterialfab.h>

#include <QFileDialog>

#include <utils/gmfileuploadhelper.h>
AddItemGroups::AddItemGroups(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddItemGroups)
{
    ui->setupUi(this);
    this->setWindowTitle("Add Inventory Group");
    qDebug()<<"Add item grp constructor";
    QString bgcolor = "#e0f7fa";
    this->setObjectName("background");
    this->setStyleSheet("QWidget#background{background-color:" + bgcolor + "}");
    dbHelper = new SalesInventoryGroupsDatabaseHelper();
    groupData = dbHelper->getSalesInventoryGroupDataAsMap();
    invGroup = new InventoryItemGroupDataModel();
    GMSearchLineEdit* searchEdit = new GMSearchLineEdit;
    searchEdit->setData(groupData.values());

    snackbar = new QtMaterialSnackbar(this);
    snackbar->setAutoHideDuration(5000);
    //    ui->salesGroupLabel->hide();
    //    ui->checkBox->hide();
    ui->parentGroupComboBox->addItem("");
    ui->parentGroupComboBox->addItems(groupData.values());
    ui->parentGroupComboBox->setLineEdit(searchEdit);

    QCompleter *completer2 = new QCompleter(groupData.values(), this);
    completer2->setFilterMode(Qt::MatchContains);
    completer2->setCaseSensitivity(Qt::CaseInsensitive);
    completer2->setCompletionMode(QCompleter::PopupCompletion);

    ui->parentGroupComboBox->setCompleter(completer2);


    QtMaterialFloatingActionButton *imageSelectButton = new QtMaterialFloatingActionButton(QtMaterialTheme::icon("image","add_a_photo"));
    imageSelectButton->setParent(ui->imageLabel);
    imageSelectButton->setMini(true);
    imageSelectButton->setXOffset(10);
    imageSelectButton->setYOffset(10);


    connect(imageSelectButton,&QPushButton::clicked,this,&AddItemGroups::openImageSelectDialog);

}

AddItemGroups::~AddItemGroups()
{
    delete ui;
}

void AddItemGroups::setInvGroup(InventoryItemGroupDataModel *value)
{
    invGroup = value;

    if(invGroup->fromExternal && invGroup->action == 1002 && !dbHelper->checkItemExists(invGroup->GroupID) ){
        qDebug()<<"group not existing. insert";
        invGroup->action = 1001;
    }
}

void AddItemGroups::disableSave()
{
    ui->buttonBox->hide();
}

void AddItemGroups::on_buttonBox_accepted()
{

    invGroup->GroupName = ui->groupNameLineEdit->text();
    invGroup->GroupNameArabic = ui->arabicNameLineEdit->text();
    invGroup->ParentGroupId = groupData.key(ui->parentGroupComboBox->currentText());
    invGroup->ParentGroupName = ui->parentGroupComboBox->currentText();

    qDebug()<<ui->parentGroupComboBox->currentText();
    qDebug()<<"Current parent  ID"<<invGroup->ParentGroupId;


    if(invGroup->ParentGroupId.length() < 1 )
    {
        snackbar->addMessage("Please Select Parent Group");
        return;
    }
    if(ui->checkBox->isChecked()){
        invGroup->GroupType="2";
    }
    else{
        invGroup->GroupType="0";
    }

    bool saveStatus = false;
    qDebug()<<Q_FUNC_INFO<<invGroup->GroupID<<invGroup->fromExternal<<invGroup->action;
    if(invGroup->GroupID == "" || (invGroup->fromExternal == true && invGroup->action == 1001)){
        qDebug()<<"Item Saved"<<invGroup->GroupName;
        if(dbHelper->insertSalesInventoryGroup(invGroup)){
            invGroup->action = 1001;
            saveStatus = true;
        }
    }
    else{
        if(dbHelper->updateSalesInventoryGroup(invGroup,invGroup->GroupID)){
            invGroup->action = 1002;
            saveStatus = true;
        }
    }

    if(saveStatus && invGroup->fromExternal)
        emit NotificationAccepted();

    if( saveStatus && invGroup->fromExternal == false){
        LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper();
        QStringList ledgerNames = ledHelper->getLedgersListHavingDbName();
        qDebug()<<"Ledgers to send"<<ledgerNames.size()<<ledgerNames;
        if(ledgerNames.size()> 0){
            if(ConfigurationSettingsDatabaseHelper::getValue(promptToSendToOtherLedgers,true).toBool()){

                NotificationLedgersList *ledWidget = new NotificationLedgersList();
                ledWidget->setWindowFlags(Qt::WindowStaysOnTopHint);
                ledWidget->show();
                QObject::connect(ledWidget, &NotificationLedgersList::sendNotification, this, &AddItemGroups::sendNotifications);
                QObject::connect(ledWidget, &NotificationLedgersList::closing, this, &AddItemGroups::on_buttonBox_rejected);
            }
            else
                sendNotifications(ledHelper->getDbNamesList());
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

void AddItemGroups::on_buttonBox_rejected()
{
    this->close();
}

void AddItemGroups::sendNotifications(QStringList dbNameList)
{
    for(QString dbName:dbNameList){
        //        qDebug()<<"Insert notification data to send"<<dbName;

        NotificationDataModel *notification = new NotificationDataModel();
        QJsonDocument doc(invGroup->objPtrToJson(invGroup));
        QString strJson(doc.toJson(QJsonDocument::Compact));

        notification->NotificationData = strJson;
        notification->ToDBName=dbName;
        notification->TypeOfData="Inventory Group";
        notification->Action = invGroup->action;
        notification->GeneratedTransactionId= invGroup->GroupID; //QString::number(QDateTime::currentDateTime().toTime_t()) + LoginValues::voucherPrefix;

        sendChannelDatabaseHelper *sendHelper = new sendChannelDatabaseHelper();
        if(sendHelper->insertNotification(notification))
            emit NotificationAccepted();
    }

    this->close();

    emit closing();

}

void AddItemGroups::openImageSelectDialog()
{

    QString fileName = QFileDialog::getOpenFileName(this,
                                            tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));
    if(fileName.isEmpty())
        return;
    ui->imageLabel->setPixmap(QPixmap(fileName).scaled(300,300,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    ui->imageLabel->setScaledContents(true);
    ui->imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->imageLabel->setFixedSize(300,300);
    if(!fileName.isEmpty()){
        qDebug()<<"File is : "<<fileName;
        GMFileUploadHelper *uploader = new GMFileUploadHelper(fileName,"inv_groups/"+invGroup->GroupID+".png");
        uploader->multipart();
        connect(uploader,&GMFileUploadHelper::uploadedSignal,this,[=](bool res,QString text){
            qDebug()<<"Uploaded";
        });

        //        connect(uploader,&GMFileUploadHelper::progressSignal,this,[=](qint64 bytesSent, qint64 bytesTotal){
        //            progress->setValue((bytesSent*100)/bytesTotal);
        //        });

    }
}

void AddItemGroups::edit(InventoryItemGroupDataModel* obj1)
{
    invGroup = obj1;
    ui->groupNameLineEdit->setText(invGroup->GroupName);
    ui->arabicNameLineEdit->setText(invGroup->GroupNameArabic);
    qDebug()<<invGroup->ParentGroupName;
    //    ui->parentGroupComboBox->clear();
    //    ui->parentGroupComboBox->setEditable(true);
    //    groupData = dbHelper->getSalesInventoryParentGroupAsMap(invGroup->GroupID);
    //    ui->parentGroupComboBox->clear();
    //    ui->parentGroupComboBox->addItems(groupData.values());
    ui->parentGroupComboBox->setCurrentText(invGroup->ParentGroupName);
    if(invGroup->GroupType=="2"){
        ui->checkBox->setChecked(true);
    }
    //    id = invGroup->GroupID;
    setImage();
}

void AddItemGroups::setTitle(QString text)
{
    ui->titleLabel->setText(text);
}

void AddItemGroups::setImage()
{
    GMFileUploadHelper *uploader = new GMFileUploadHelper("inv_groups/"+invGroup->GroupID,"xxx.jpg");
    connect(uploader,&GMFileUploadHelper::downLoaded,this,[=](QByteArray data){
        qDebug()<<data.size();
        QPixmap pm;
        pm.loadFromData(data);
        ui->imageLabel->setPixmap(pm.scaled(300,300,Qt::KeepAspectRatio,Qt::SmoothTransformation));
        ui->imageLabel->setScaledContents(true);
        ui->imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        ui->imageLabel->setFixedSize(300,300);
    });

    uploader->readFile("inv_groups/"+invGroup->GroupID+".png");
}
