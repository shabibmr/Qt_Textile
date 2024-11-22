#include "addaccountgroup.h"
#include "ui_addaccountgroup.h"
#include <QCompleter>
#include <QPushButton>

addAccountGroup::addAccountGroup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addAccountGroup)
{
    ui->setupUi(this);
//    ui->parentGroupLineEdit->hide();
//    ui->tcombo->hide();
    this->setWindowTitle("Create New Account Group");

    QString bgcolor = "#e0f7fa";
   this->setObjectName("background");
   this->setStyleSheet("QWidget#background{background-color:" + bgcolor + "}");
    AccountGroupDataModelObject = new AccountGroupDataModel();
    //    ui->parentGroupComboBox->setEditable(true);
    dbHelper = new AccountGroupMasterDatabaseHelper();
    //QString qStr = "SELECT "+AccountGroupMasterDatabaseHelper::Accounts_Group_GroupName+" from Accounts_group";
    groupData = dbHelper ->getAllGroupsAsMap();
    snackBar = new QtMaterialSnackbar(this);
    //    GMSearchLineEdit* searchEdit = new GMSearchLineEdit;
    //    searchEdit->setData(groupData.values());
    //    ui->parentGroupComboBox->addItem("");
    //    ui->parentGroupComboBox->addItems(groupData.values());
    //    ui->parentGroupComboBox->setLineEdit(searchEdit);

    //    ui->checkBox->hide();

    //    QCompleter *completer2 = new QCompleter(groupData.values(), this);
    //    completer2->setFilterMode(Qt::MatchContains);
    //    completer2->setCaseSensitivity(Qt::CaseInsensitive);
    //    completer2->setCompletionMode(QCompleter::PopupCompletion);

    //    ui->parentGroupComboBox->setCompleter(completer2);

    accGroupModel = dbHelper->getAllAccountForSearchModel();
    QCompleter *testCom = new QCompleter(this);

    testCom->setModel(accGroupModel);
    sort = new GMLedgerSearchProxyModel();
    sort->setDynamicSortFilter(true);
    sort->setSourceModel(accGroupModel);


    testCom->setModel(sort);

    testCom->setCompletionMode(QCompleter::PopupCompletion);
    testCom->setFilterMode(Qt::MatchContains);
    testCom->setCaseSensitivity(Qt::CaseInsensitive);
    //    testCom->setCompletionColumn(1);
    testCom->popup()->setItemDelegate(new GMLedgerSearchDelegate());
    testCom->setModelSorting(QCompleter::CaseSensitivelySortedModel);


    testCom->setCompletionPrefix("");
    QObject::connect(testCom,SIGNAL(activated(QModelIndex)),
                     this,SLOT(setGroup(QModelIndex)));


//    ui->tcombo->setModel(accGroupModel);
//    ui->tcombo->setEditable(true);
//    ui->tcombo->setCompleter(testCom);


    GMLedgerSearchLite* groupLineLite = new GMLedgerSearchLite(accGroupModel);
    groupLineLite->setAccountsFlag(true);
    qDebug()<<"123";
    QToolButton* butt = new QToolButton;
    QVBoxLayout* lay = new QVBoxLayout;
    lay->setContentsMargins(0,0,0,0);
    butt->setContentsMargins(0,0,0,0);
    ui->groupLay->setContentsMargins(0,0,0,0);
    lay->addWidget(groupLineLite);
    butt->setLayout(lay);
    butt->setMinimumWidth(250);
    ui->groupLay->addWidget(butt);

    QObject::connect(groupLineLite,SIGNAL(SelectedAccount(AccountGroupDataModel)),this,SLOT(setAccountGroup(AccountGroupDataModel)));


    QStringList types ;
    types<< "Assets" << "Liabilities"<<"Income"<<"Expense";

    ui->grpTypecomboBox->addItems(types);


}
void addAccountGroup::setGroup(QModelIndex index){

    qDebug()<<"Selected row : "<<index.row();

    //    index.model();
    int row = index.row();//sort->mapToSource(index).row();



    QString id = accGroupModel->record(row).value(0).toString();
    QString name = index.model()->index(row,1).data().toString();

    qDebug()<<"Item Name : "<<name;
}

addAccountGroup::~addAccountGroup()
{
    delete ui;
}

void addAccountGroup::disableSave()
{
    ui->buttonBox->hide();
}

void addAccountGroup::on_buttonBox_accepted()
{


    //    AccountGroupDataModelObject = new AccountGroupDataModel();
    AccountGroupDataModelObject->groupName=ui->groupNameLineEdit->text();
    AccountGroupDataModelObject->groupID=GroupId;
    AccountGroupDataModelObject->groupType=ui->grpTypecomboBox->currentText();
    //        AccountGroupDataModelObject->parentGroupId = groupData.key(ui->parentGroupComboBox->currentText());

    if(  AccountGroupDataModelObject->parentGroupId.length()<1){
        snackBar->addMessage("Please Select Parent  Group");
        return;
    }
    if(ui->checkBox->isChecked())
        AccountGroupDataModelObject->category = "1";
    else
        AccountGroupDataModelObject->category = "0";

    if(AccountGroupDataModelObject->groupID == ""|| (AccountGroupDataModelObject->fromExternal == true && AccountGroupDataModelObject->action == 1001)){
        dbHelper->insertAccountGroup(AccountGroupDataModelObject);
        AccountGroupDataModelObject->action=1001;
    }
    else{
        //        qDebug()<<"updating id "<<AccountGroupDataModelObject->groupID;
        dbHelper->updateAccountGroup(AccountGroupDataModelObject,AccountGroupDataModelObject->groupID);
        AccountGroupDataModelObject->action=1002;
    }

//    if(AccountGroupDataModelObject->fromExternal == false){
//        LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper();
//        QStringList ledgerNames = ledHelper->getLedgersListHavingDbName();
//        qDebug()<<"Ledgers to send"<<ledgerNames.size()<<ledgerNames;
//        if(ledgerNames.size()> 0){
//            NotificationLedgersList *ledWidget = new NotificationLedgersList();
//            ledWidget->setWindowFlags(Qt::WindowStaysOnTopHint);
//            ledWidget->show();
//            QObject::connect(ledWidget, &NotificationLedgersList::sendNotification, this, &addAccountGroup::sendNotifications);
//            QObject::connect(ledWidget, &NotificationLedgersList::closing, this, &addAccountGroup::on_buttonBox_rejected);
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

}

void addAccountGroup::on_buttonBox_rejected()
{
    this->close();
    emit closing();
}

void addAccountGroup::setTitle(QString text)
{
    ui->titleLabel->setText(text);
}

void addAccountGroup::editAccountGroup(AccountGroupDataModel* obj)
{
    AccountGroupDataModelObject = obj;

    ui->groupNameLineEdit->setText(AccountGroupDataModelObject->groupName);
    //    ui->parentGroupComboBox->setCurrentText(obj->parentGroupName);
//    ui->parentGroupLineEdit->setText(AccountGroupDataModelObject->parentGroupName);
    ui->grpTypecomboBox->setCurrentText(AccountGroupDataModelObject->groupType);
    QLineEdit* parentLine = new QLineEdit();
    parentLine->setText(AccountGroupDataModelObject->parentGroupName);
    ui->groupLay->addWidget(parentLine);

    GroupId = AccountGroupDataModelObject->groupID;

    if(AccountGroupDataModelObject->category=="1"){
        ui->checkBox->setChecked(true);
    }

    ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled( !dbHelper->isDefaultRecord(AccountGroupDataModelObject->groupID));

}

//void addAccountGroup::on_parentGroupComboBox_currentIndexChanged(const QString &arg1)
//{
//    QString Type = dbHelper->getGroupType( groupData.key(ui->parentGroupComboBox->currentText()));
//    ui->grpTypecomboBox->setCurrentText(Type);
//    if(Type.length()>0){
//        ui->grpTypecomboBox->setEnabled(false);
//    }
//}

void addAccountGroup::on_grpTypecomboBox_currentTextChanged(const QString &arg1)
{
    if(ui->grpTypecomboBox->currentText()=="Income"||
            ui->grpTypecomboBox->currentText()=="Expense"){
        ui->checkBox->show();
    }
}

void addAccountGroup::setAccountGroup(AccountGroupDataModel obj)
{
    qDebug()<<"Parent selected"<<obj.parentGroupId<<obj.groupID;
    AccountGroupDataModelObject->parentGroupId = obj.groupID;
    AccountGroupDataModelObject->parentGroupName = obj.groupName;

    QString Type = dbHelper->getGroupType(AccountGroupDataModelObject->parentGroupId);
    ui->grpTypecomboBox->setCurrentText(Type);
    if(Type.length()>0){
        ui->grpTypecomboBox->setEnabled(false);
    }
}

void addAccountGroup::on_tcombo_currentTextChanged(const QString &arg1)
{
    if (!arg1.isEmpty()) {
        QRegExp regExp(arg1,Qt::CaseInsensitive);
        sort->setFilterRegExp(regExp);

    }
}

void addAccountGroup::sendNotifications(QStringList dbNameList)
{
    for(QString dbName:dbNameList){
        //        qDebug()<<"Insert notification data to send"<<dbName;

        NotificationDataModel *notification = new NotificationDataModel();
        QJsonDocument doc(AccountGroupDataModelObject->objPtrToJson(AccountGroupDataModelObject));
        QString strJson(doc.toJson(QJsonDocument::Compact));

        notification->NotificationData = strJson;
        notification->ToDBName=dbName;
        notification->TypeOfData="Account Group";
        notification->Action = AccountGroupDataModelObject->action;
        notification->GeneratedTransactionId= AccountGroupDataModelObject->groupID; //QString::number(QDateTime::currentDateTime().toTime_t()) + LoginValues::voucherPrefix;

        sendChannelDatabaseHelper *sendHelper = new sendChannelDatabaseHelper();
        sendHelper->insertNotification(notification);
    }

    this->close();
    emit closing();

}
