#include "addledger.h"
#include "ui_addledger.h"
#include "customwidgets/gmsearchlineedit.h"
#include <QCompleter>

addLedger::addLedger(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addLedger)
{
    ui->setupUi(this);
    this->setWindowTitle("Create New Account");
     QString bgcolor = "#e0f7fa";
    this->setObjectName("background");
    this->setStyleSheet("QWidget#background{background-color:" + bgcolor + "}");

    GMSearchLineEdit *groupSearch = new GMSearchLineEdit;
    GMSearchLineEdit *priceListLineEdit = new GMSearchLineEdit;
    accHelper = new AccountGroupMasterDatabaseHelper();
    ledHlper = new LedgerMasterDatabaseHelper();
    priceHelper = new PricelistDatabaseHelper();
    billHelper = new BillwiseRecordsDatabaseHelper();

    snackBar = new QtMaterialSnackbar(this);

    ledger = new LedgerMasterDataModel();
    accGroups = accHelper->getAllGroupsAsMap();
    priceListNames = priceHelper->getPriceListAsMap();
    groupSearch->setData(accGroups.values());

    priceListLineEdit->setData(priceListNames.values());
    //priceListNames = priceHelper->get;
    ui->groupNameComboBox->addItem("");
    ui->priceListComboBox->addItem("");

    ui->groupNameComboBox->lineEdit()->setPlaceholderText("Select Accounting Group");

//    ui->priceListComboBox->hide();
//    ui->priceListLabel->hide();

    ui->isInndividualLabel->hide();
    ui->isCompany->hide();
    ui->companyLabel->hide();
    ui->companyLineEdit->hide();

    ui->groupNameComboBox->addItems(accGroups.values());
    ui->priceListComboBox->addItems(priceListNames.values());
    ui->groupNameComboBox->setLineEdit(groupSearch);

    QCompleter *completer2 = new QCompleter(accGroups.values(), this);
    completer2->setFilterMode(Qt::MatchContains);
    completer2->setCaseSensitivity(Qt::CaseInsensitive);
    completer2->setCompletionMode(QCompleter::PopupCompletion);
    ui->groupNameComboBox->setCompleter(completer2);
    ui->priceListComboBox->setLineEdit(priceListLineEdit);

    QCompleter *completer3 = new QCompleter(priceListNames.values(), this);
    completer3->setFilterMode(Qt::MatchContains);
    completer3->setCaseSensitivity(Qt::CaseInsensitive);
    completer3->setCompletionMode(QCompleter::PopupCompletion);
    ui->priceListComboBox->setCompleter(completer3);

    ui->crRadioButton->setChecked(true);
    ui->openingBalanceLineEdit->setValidator(new QDoubleValidator(0.0002,999999,2,this));
    ui->defDiscount->setValidator(new MyValidator(0.0,100,2,this));

    ui->dateOfBirthDateEdit->setDate(QDate::currentDate());

    ui->taxWidget->hide();
//    ui->creditWidget->hide();

}

addLedger::~addLedger()
{
    delete ui;
}

void addLedger::setLedger(LedgerMasterDataModel *value)
{
    ledger = value;
    qDebug()<<"Set ledger"<<ledger->LedgerID;
}

void addLedger::setLedgerName(QString name)
{
    ui->ledgerNameLineEdit->setText(name);
}

void addLedger::setContactID(int value)
{
    ContactID = value;
}

void addLedger::disableSave()
{
    ui->buttonBox->hide();
}

void addLedger::on_buttonBox_accepted()
{
    ledger->LedgerName = ui->ledgerNameLineEdit->text();
    ledger->ContactPersonName = ui->contactPersonNameLineEdit->text();
    ledger->emailAddress = ui->emailLineEdit->text();
    ledger->narration = ui->ledgerNarrationLineEdit->toPlainText();
    ledger->PoBox = ui->postCodeLineEdit->text();
//    ledger->LedgerType = ui->ledgerTypeLineEdit->text();
    ledger->LedgerType = accHelper->getGroupType(ledger->LedgerGroupId);
    ledger->ContactPersonNumber = ui->contactPersonNoLineEdit->text();
    ledger->LedgerGroupName = ui->groupNameComboBox->currentText();
//    qDebug()<<accGroups.key(ledger->LedgerGroupName);
    ledger->LedgerGroupId = accGroups.key(ledger->LedgerGroupName);
    ledger->defaultPriceListID = priceListNames.key(ui->priceListComboBox->currentText());
    ledger->Birth_Date = ui->dateOfBirthDateEdit->date();
    ledger->Address = ui->mailingAddressLineEdit->text();
    ledger->phoneNumber = ui->phoneNoLineEdit->text();
    ledger->Country = ui->countryLineEdit->text();
    ledger->City = ui->cityLineEdit->text();
    ledger->totalTurnover = 0;
    ledger->mobileNumber = ui->mobilNoLineEdit->text();
    ledger->fax = ui->faxLineedit->text();
    ledger->parentCompany = ui->companyLineEdit->text();
    ledger->taxRate = ui->taxRateLineEdit->text().toFloat();
    ledger->DefaultTaxLedger = ledHlper->getLedgerIDByName(ui->defaultTaxLedgerComboBox->currentText());
    ledger->TypeOfSupply = ui->typeOfSupplyComboBox->currentText();
    ledger->TRN = ui->RegistrationLineEdit->text();

    ledger->discountPercent = ui->defDiscount->text().toFloat();
    ledger->isFrequent = ui->isFavourite->isChecked();
    ledger->Credit_Limit = ui->creditLimitLineEdit->text().toFloat();


    if(ledger->LedgerGroupId.length()<1){
        snackBar->addMessage("Please Select Group");
        return;
    }

    if(ui->isCompany->isChecked()){
        ledger->isIndividual=false;
    }
    else{
        ledger->isIndividual=true;
    }
    float opbal = ui->openingBalanceLineEdit->text().toFloat();
    if(ui->crRadioButton->isChecked()){

    }
    else {
        opbal = opbal *-1;
    }
    ledger->openingBalance = opbal;

    ledger->closingBalance = opbal;
    ledger->LedgerID =id;
    if(id == ""|| (ledger->fromExternal == true && ledger->action == 1001)){
        ledHlper->insertLedgerMasterItem(ledger,ContactID);
        ledger->action=1001;
    }
    else{
        ledHlper->updateLedgerMasterItem(ledger,id);
        ledger->action=1002;
    }

//    if(ledger->fromExternal == false){
//        QStringList ledgerNames = ledHlper->getLedgersListHavingDbName();

//        if(ledgerNames.size()> 0){
//        NotificationLedgersList *ledWidget = new NotificationLedgersList();
//        ledWidget->setWindowFlags(Qt::WindowStaysOnTopHint);
//        ledWidget->show();
//        QObject::connect(ledWidget, &NotificationLedgersList::sendNotification, this, &addLedger::sendNotifications);
//        QObject::connect(ledWidget, &NotificationLedgersList::closing, this, &addLedger::on_buttonBox_rejected);
//        }
//        else{
//            this->close();
//            emit newCreated(ledHlper->getLastInsertedLedger());
//        }


//    }

//    else{
        this->close();
        emit closing();
        emit newCreated(ledHlper->getLastInsertedLedger());
//    }
//    emit closing();

}

void addLedger::on_buttonBox_rejected()
{
    this->close();
}

void addLedger::setTitle(QString text)
{
    ui->titleLabel->setText(text);
}

void addLedger::edit()
{

    id = ledger->LedgerID;
    ui->ledgerNameLineEdit->setText(ledger->LedgerName) ;
    ui->contactPersonNameLineEdit->setText(ledger->ContactPersonName) ;
    ui->emailLineEdit->setText(ledger->emailAddress);
    ui->ledgerNarrationLineEdit->setPlainText(ledger->narration);
    ui->postCodeLineEdit->setText(ledger->PoBox  );
    ui->ledgerTypeLineEdit->setText(ledger->LedgerType  );
    ui->contactPersonNoLineEdit->setText(ledger->ContactPersonNumber );
    ui->groupNameComboBox->setCurrentText(ledger->LedgerGroupName);
    //obj.LedgerGroupId  accGroups.key(obj.LedgerGroupName);
    //obj.defaultPriceListID  priceListNames.key(ui->priceListComboBox->currentText());
    ui->priceListComboBox->setCurrentText(priceListNames.value(ledger->defaultPriceListID));
    ui->dateOfBirthDateEdit->setDate(ledger->Birth_Date);
    ui->mailingAddressLineEdit->setText(ledger->Address );
    ui->phoneNoLineEdit->setText(ledger->phoneNumber );
    ui->countryLineEdit->setText(ledger->Country );
    ui->cityLineEdit->setText(ledger->City );
    ui->mobilNoLineEdit->setText(ledger->mobileNumber);
    ui->companyLineEdit->setText(ledger->parentCompany);
    ui->faxLineedit->setText(ledger->fax);
    ui->taxRateLineEdit->setText(QString::number(ledger->taxRate,'f',2));
    ui->typeOfSupplyComboBox->setCurrentText(ledger->TypeOfSupply);
    ui->defaultTaxLedgerComboBox->setCurrentText(ledger->DefaultTaxLedger);
    ui->RegistrationLineEdit->setText(ledger->TRN);
    ui->isFavourite->setChecked(ledger->isFrequent);
    ui->defDiscount->setText(QString::number(ledger->discountPercent,'f',2));
    ui->isCompany->setChecked(!ledger->isIndividual);

    ui->creditLimitLineEdit->setText(QString::number(ledger->Credit_Limit,'f',2));

    qDebug()<<"Opening balance"<<ledger->openingBalance;
    if(ledger->openingBalance<0){
        ui->drRadioButton->setChecked(true);
        ui->openingBalanceLineEdit->setText(QString::number(-1*ledger->openingBalance,'f',2));

    }
    else {
        ui->crRadioButton->setChecked(true);
        ui->openingBalanceLineEdit->setText(QString::number(ledger->openingBalance,'f',2));

    }

    qDebug()<<"p ID : "<<ledger->defaultPriceListID;
    qDebug()<<"Price Name :"<<priceListNames[ledger->defaultPriceListID];
    ui->priceListComboBox->setCurrentText(priceListNames[ledger->defaultPriceListID]);

}

void addLedger::settable()
{

}

bool addLedger::validate()
{

}


void addLedger::on_openingBalanceLineEdit_editingFinished()
{
    qDebug()<<"Opening balance edited";

    ledger->openingBalance = ui->openingBalanceLineEdit->text().toFloat();
    qDebug()<<ledger->openingBalance;
    if(ui->drRadioButton->isChecked())
        ledger->openingBalance = ledger->openingBalance *-1;

    BillwiseRecordDataModel* refObj = new BillwiseRecordDataModel();
    refObj->VoucherNo = "OPENING";
    refObj->MethodOfAdjustment = "ON ACC";
    refObj->Amount = ledger->openingBalance;
    if(ledger->openingBalance>0){
        refObj->CreditAmount = ledger->openingBalance;
        refObj->DebitAmount = 0;
    }
    else {
        refObj->DebitAmount = -1*ledger->openingBalance;
        refObj->CreditAmount = 0;
    }
    refObj->LedgerID = id;
    refObj->VoucherDate = QDate::currentDate();
    refObj->DueDate = QDate::currentDate().addDays(120);
    refObj->VoucherType = "OPENING";
    refObj->VoucherPrefix = "A";
    refObj->RefPrefix = refObj->VoucherPrefix;
    refObj->RefType = refObj->VoucherType;
    refObj->RefVoucherNo = refObj->VoucherNo;

    if(ledger->hasBillwiseMappings == false)
        ledger->mapList.clear();
    ledger->mapList.append(refObj);

}

void addLedger::on_billwiseToolButton_clicked()
{

                AddBillwiseRecord* billwiseWidget = new AddBillwiseRecord() ;
                billwiseWidget->setWindowFlags(Qt::Window);
                billwiseWidget->setAttribute(Qt::WA_DeleteOnClose);
                qDebug()<<ledger->LedgerID;
                billwiseWidget->setLedger(ledger);

//                billwiseWidget->setOnAccountData();
                billwiseWidget->showOpeningBalanceList(   false);
                billwiseWidget->show();

}

void addLedger::sendNotifications(QStringList dbNameList)
{
    for(QString dbName:dbNameList){
//        qDebug()<<"Insert notification data to send"<<dbName;

        NotificationDataModel *notification = new NotificationDataModel();
        QJsonDocument doc(ledger->ObjPtrToJson(ledger));
        QString strJson(doc.toJson(QJsonDocument::Compact));

        notification->NotificationData = strJson;
        notification->ToDBName=dbName;
        notification->TypeOfData="Ledger";
        notification->Action = ledger->action;
        notification->GeneratedTransactionId= ledger->LedgerID; //QString::number(QDateTime::currentDateTime().toTime_t()) + LoginValues::voucherPrefix;

        sendChannelDatabaseHelper *sendHelper = new sendChannelDatabaseHelper();
        sendHelper->insertNotification(notification);
    }

    this->close();

    emit closing();
    emit newCreated(ledHlper->getLastInsertedLedger());

}
