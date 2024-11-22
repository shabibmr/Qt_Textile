#include "addressdialog.h"
#include "ui_addressdialog.h"
#include<QSqlQuery>
#include <QDebug>
#include<QSqlError>
#include<QMessageBox>
#include <QListWidgetItem>
#include "database/Settings/ConfigurationSettingsDatabaseHelper.h"
#include "customwidgets/Delegates/Contacts/contactnamesearchdelegate.h"
#include <QTabWidget>

AddressDialog::AddressDialog(/*GeneralVoucherDataObject *v,*/LedgerMasterDatabaseHelper *ledHelper,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddressDialog)
{
    ui->setupUi(this);
    //    this->voucher = v;
    qDebug()<<Q_FUNC_INFO<<__LINE__<<"1";
    ui->phoneEdit->setFocus();
    //     hideCancel = ConfigurationSettingsDatabaseHelper::getValue(addressAlwaysNeeded,false).toBool();
    //    if(hideCancel){
    //        ui->AddCancel->hide();
    //        this->setWindowFlag(Qt::FramelessWindowHint);
    //    }
    //    db = QSqlDatabase::database("gmdb");
    this->setWindowTitle("Customer Contact Details");

    this->ledHelper = ledHelper;

    dbHelper = new AddressBookDatabaseHelper;
    addr = new AddressBookDataModel;

    locationsList = new QListWidget(this);
    ui->horizontalLayout_6->addWidget(locationsList);
    locationsList->hide();

    QObject::connect(locationsList,&QListWidget::itemClicked,
                     this,&AddressDialog::setAddressFromList);
    QObject::connect(locationsList,&QListWidget::itemDoubleClicked,
                     this,&AddressDialog::setAddressFromList);


    ledger = new GMLedgerLineWidget(ledHelper,ledHelper->getLedgersUnderGroups(QStringList("0x5x19")));

    connect(ledger,&GMLedgerLineWidget::itemSelectedwithItem,[=](LedgerMasterDataModel item){
        setLedger(item);
    });

    ui->horizontalLayout_5->insertWidget(0,new QLabel("Ledger : "));
    ui->horizontalLayout_5->insertWidget(1,ledger);

    ui->NextButton->hide();
    ui->prevButton->hide();



    ui->tabWidget->setStyleSheet("QTabBar::tab { width: 100px; height: 20px; }");

    // qDebug()<<"Call Count = "<< callCount;
    if(callCount < 0 ) {
        ui->NextButton->hide();
    }
    QFont font;
    font.setPointSize(12);

    ui->codeLineEdit->setFont(font);
    //    ui->codeLineEdit->setText(dbHelper->generateNewCustomerCode());
    ui->phoneEdit->setFont(font);
    ui->AddressText->setFont(font);
    ui->NameEdit->setFont(font);
    //    ui->emailLineEdit->setFont(font);
    ui->tabWidget->setCurrentIndex(0);

    ui->fromDate->setDateTime(QDateTime::currentDateTime());
    ui->fromDate->setTime(QTime(0,0,0));
    ui->toDate->setDateTime(QDateTime::currentDateTime());
    ui->toDate->setTime(QTime(23,59,59));


    smodel =  dbHelper->getAddressesForSearchModel();
    //    smodel->setQuery("Select distinct phone_number from address_book",db);

    //    qDebug()<<""

    Phonecompleter = new QCompleter(ui->phoneEdit);
    Phonecompleter->setModel(smodel);
    Phonecompleter->setCompletionMode(QCompleter::PopupCompletion);
    Phonecompleter->setFilterMode(Qt::MatchContains);
    Phonecompleter->setCaseSensitivity(Qt::CaseInsensitive);
    Phonecompleter->setCompletionColumn(2);
    Phonecompleter->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    Phonecompleter->popup()->installEventFilter(this);

    ui->phoneEdit->setCompleter(Phonecompleter);

    smodel2 = dbHelper->getNameAddressesForSearchModel();
    //    smodel2->setQuery("Select name,PHONE  ,address, route, code from customer_details",db);

    //    qDebug()<<""

    smodel2->query().prepare(smodel2->query().lastQuery());

    nameCompleter = new QCompleter(ui->NameEdit);
    nameCompleter->setModel(smodel2);
    nameCompleter->setCompletionMode(QCompleter::PopupCompletion);
    nameCompleter->setFilterMode(Qt::MatchContains);
    nameCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    nameCompleter->setCompletionColumn(1);
    nameCompleter->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    nameCompleter->popup()->installEventFilter(this);
    nameCompleter->setMaxVisibleItems(5);


    nameCompleter->popup()->setItemDelegate(new ContactNameSearchDelegate(this));
    ui->NameEdit->setCompleter(nameCompleter);

    QObject::connect(nameCompleter,SIGNAL(activated(QModelIndex)),
                     this,SLOT(completerActivatedByName(QModelIndex )));

    QObject::connect(Phonecompleter,SIGNAL(activated(QModelIndex)),
                     this,SLOT(completerActivated(QModelIndex )));



    //    setTable();
    //    if(db.open())
    //    {
    //        qDebug() <<"Database Connection Established";
    //    }
    //    else
    //    {
    //        qDebug() <<"Error ="<< db.lastError().text();
    //        QMessageBox box;
    //        box.setText("At login "+db.lastError().text());
    //        box.exec();
    //    }
    ui->phoneEdit->setValidator(new QDoubleValidator(0,999999999999,2,this));
    ui->phoneEdit->setFocus();

    setTabOrder(ui->NameEdit, ui->AddressText);

    qDebug()<<Q_FUNC_INFO<<__LINE__;
    // setupMeasurements();
}

AddressDialog::~AddressDialog()
{
    delete ui;
}

void AddressDialog::setFocustoPhone()
{
    ui->phoneEdit->setFocus();
}

void AddressDialog::setAddressText()
{
    this->blockSignals(true);
    //    qDebug()<<Q_FUNC_INFO<<__LINE__;

    ui->phoneEdit->setText(addr->PhoneNumber);
    //    ui->AddressText->document()->setPlainText(addr->address);
    //    qDebug()<<Q_FUNC_INFO<<__LINE__;
    if(addr->ContactName.length()>0)
        ui->NameEdit->setText(addr->ContactName);
    if(addr->route.length()>0)
        ui->routeLineEdit->setText(addr->route);
    //    qDebug()<<Q_FUNC_INFO<<__LINE__;

    if(addr->code.length()>0)
        ui->codeLineEdit->setText(addr->code);


    ui->activeCheckBox->setChecked(addr->isActive);
    ui->deliveryChargesLineEdit->setText(QString::number(addr->deliveryCharges, 'f', 2));

    ledger->setText(ledHelper->getLedgerNameByID(addr->ledgerId));
    //    qDebug()<<Q_FUNC_INFO<<__LINE__;
    if(addr->email.length()>0)
        ui->emailLineEdit->setText(addr->email);

    // qDebug()<<Q_FUNC_INFO<<__LINE__<<addr->LocationDetails.length()<<addr->LocationDetails.size();
    if(addr->LocationDetails.length()>1){
        locationsList->clear();
        for (int i=0;i<addr->LocationDetails.length() ;i++ ) {
            QListWidgetItem *item = new QListWidgetItem(locationsList);
            item->setText(addr->LocationDetails[i]);
            //            item->setFlags(item->flags() & ~Qt::ItemIsSelectable);Qt::ItemIsEditable);
            locationsList->addItem(item);
        }
        QString str = addr->LocationDetails[addr->LocationDetails.length()-1];
        if(str.length()>1){
            ui->AddressText->setPlainText(addr->LocationDetails[addr->LocationDetails.length()-1]);
        }
        locationsList->show();
    }
    else{
        locationsList->hide();
        if(addr->LocationDetails.length()==1){
            // qDebug()<<Q_FUNC_INFO<<__LINE__<<addr->LocationDetails[0];
            QString str = addr->LocationDetails[0];
            if(str.length()>0){
                ui->AddressText->setPlainText(addr->LocationDetails[0]);
            }
        }
    }
    qDebug()<<"________________________________________________Check Measurements if NULL________________________________________________";
    if(addr->addressId.length()>0){
        ui->showMeasurements->show();
    }
    else{
        ui->showMeasurements->hide();
    }
    if(voucher->measurement != nullptr){
        qDebug()<<"Aint NULL AT MEASUREMENT";
        qDebug()<<"m : "<<voucher->measurement->measurement->measurementName;
        ui->showMeasurements->setText(voucher->measurement->measurement->measurementName + "-" + voucher->measurement->reference);
    }
    this->blockSignals(false);
}

void AddressDialog::on_buttonBox_accepted()
{
    // qDebug()<<Q_FUNC_INFO<<__LINE__;

    voucher->Location = ui->AddressText->toPlainText();
    voucher->POCName = ui->NameEdit->text();
    voucher->Contact.address = voucher->Location;
    voucher->POCEmail = ui->emailLineEdit->text();

    voucher->Contact.PhoneNumber = ui->phoneEdit->text();
    voucher->Contact.ContactName = ui->NameEdit->text();
    voucher->Contact.route = ui->routeLineEdit->text();

    // qDebug()<<Q_FUNC_INFO<<__LINE__;

    float deliveryCharges = ui->deliveryChargesLineEdit->text().toFloat();
    QString code = ui->codeLineEdit->text();
    QString currentLocation = ui->AddressText->toPlainText();
    if(deliveryCharges > 0){

    }
    // qDebug()<<Q_FUNC_INFO<<__LINE__;

    QRegExp mailREX("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b");
    mailREX.setCaseSensitivity(Qt::CaseInsensitive);
    mailREX.setPatternSyntax(QRegExp::RegExp);



    voucher->ledgerObject.ContactPersonNumber = voucher->Contact.PhoneNumber;
    voucher->ledgerObject.emailAddress = voucher->Contact.address;
    voucher->ledgerObject.ContactPersonName = voucher->Contact.ContactName;

    if(addr->PhoneNumber != voucher->Contact.PhoneNumber ||
        addr->ContactName != voucher->Contact.ContactName ||
        addr->deliveryCharges != ui->deliveryChargesLineEdit->text().toFloat() ||
        addr->route != voucher->Contact.route ||
        addr->isActive != ui->activeCheckBox->isChecked() ||
        addr->email != ui->emailLineEdit->text() ||
        addr->code != code ||
        !addr->LocationDetails.contains(currentLocation)||
        code.length()==0
        ){
        editMode = true;
    }

    // qDebug()<<Q_FUNC_INFO<<__LINE__;

    if(editMode && ui->phoneEdit->text().length()>0){
        // qDebug()<<"IN EDIT ADDRESS";
        addr->code = ui->codeLineEdit->text();
        addr->address = ui->AddressText->toPlainText();
        addr->email = ui->emailLineEdit->text();

        //         if(mailREX.exactMatch(addr->email)&& addr->email.length()>0){
        //            QMessageBox box; box.setText("Enter a Valid Email"); box.exec();
        //            return;
        //         }

        if(!addr->LocationDetails.contains(addr->address)){
            addr->LocationDetails.append(addr->address);
        }

        // qDebug()<<Q_FUNC_INFO<<__LINE__;
        addr->PhoneNumber =  ui->phoneEdit->text();
        addr->ContactName = ui->NameEdit->text();
        addr->route = ui->routeLineEdit->text();
        addr->isActive = ui->activeCheckBox->isChecked();
        addr->deliveryCharges = ui->deliveryChargesLineEdit->text().toFloat();

        if( addr->code.length()==0 || dbHelper->checkCodeExists(code) ){
            addr->code = dbHelper-> generateNewCustomerCode();
            // qDebug()<<" >>> New Code : "<<addr->code;
        }

        if(addr->PhoneNumber.length()>4 || addr->ContactName.length()>1){
            if(addr->addressId.length() == 0){
                dbHelper->insertContact(*addr);
            }
            else {
                dbHelper->updateContact(*addr, addr->_id);
            }

            oldNumber = addr->PhoneNumber;
            smodel->query().exec();
            smodel2->query().exec();
            //            nameCompleter->setModel(smodel2);
            //            Phonecompleter->setModel(smodel);
        }
    }
    // qDebug()<<Q_FUNC_INFO<<__LINE__;
    editMode = false;
    // qDebug()<<Q_FUNC_INFO<<__LINE__;
    if(voucherType == GMVoucherTypes::SalesOrder){
        // qDebug()<<"emit for draft"<<voucherType;
        emit addrSelectedForDraft();
    }
    else if(voucherType == GMVoucherTypes::SalesVoucher){
        // qDebug()<<"emit for invoice"<<voucherType;
        emit addrSelectedForInvoice();
    }
    else{
        // qDebug()<<"emit otherwise"<<voucherType;
        emit addressSelected();
    }

    smodel->query().exec();
    smodel2->query().exec();


    // qDebug()<<"NEW NAME MODEL : "<<smodel2->rowCount();
    this->close();

}

void AddressDialog::on_phoneEdit_returnPressed()
{
    QString number = ui->phoneEdit->text();
    if(number.length() == 0 )
        return;
    addr = dbHelper->getContactPtrByPhone(number);
    // qDebug()<<Q_FUNC_INFO<<__LINE__<<addr->_id;
    reset();
    if(addr->PhoneNumber.length()==0){
        addr->PhoneNumber = number;
        //        addr->code = dbHelper->generateNewCustomerCode();
        setupMeasurements();
    }

    setAddressText();

}

void AddressDialog::setAddressFromList(QListWidgetItem *item)
{
    // qDebug()<<"Address Changed to "<<item->text();
    addr->address = item->text();
    ui->AddressText->setPlainText(addr->address);
}

void AddressDialog::removeAddressFromList(QListWidgetItem *item)
{
    QString address= item->text();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Remove","Remoe Address : "+ address,
                                  QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::Yes){
        int i = addr->LocationDetails.indexOf(address);
        addr->LocationDetails.removeAt(i);
        setAddressText();
    }
    else{

    }
}



//void AddressDialog::on_NameEdit_returnPressed()
//{
//    //    QString name = ui->NameEdit->text();

//    //    addr = dbHelper->getContactPtrByName(name);
//    //    qDebug()<<Q_FUNC_INFO<<__LINE__<<addr->_id;
//    //    reset();
//    //    if(addr->ContactName.length() == 0)
//    //        addr->ContactName = name;
//    //    setAddressText();
//}

void AddressDialog::keyPressEvent(QKeyEvent *e)
{
    // qDebug() << e->key() <<"  keyy0";
    switch (e->key ()) {
    case Qt::Key_Enter:
        //      on_print_clicked();
        break;



    default:
        break;
        //        AddressDialog::keyPressEvent (e);
    }
}


void AddressDialog::on_phoneEdit_editingFinished()
{
    qDebug()<<"Editing Finished...."<<__LINE__;
    // on_phoneEdit_returnPressed();
}

void AddressDialog::setNumberAndOpen(QString number)
{
    ui->phoneEdit->blockSignals(true);
    ui->phoneEdit->setText(number);
    ui->phoneEdit->blockSignals(false);

    on_phoneEdit_returnPressed();
    this->show();
}

void AddressDialog::setVoucher(GeneralVoucherDataObject *value)
{
    reset();
    voucher = value;
    addr = &voucher->Contact;
    if(addr->PhoneNumber.length()==0){
        ui->phoneEdit->setFocus();
        qDebug()<<Q_FUNC_INFO<<__LINE__;
        return;
    }
    addr = dbHelper->getContactPtrByPhone(voucher->Contact.PhoneNumber);
    addr->address = voucher->Location;

    setAddressText();
    //    ui->routeLineEdit->setText(voucher->Contact.route);

}

void AddressDialog::setAddr()
{

    addr = &voucher->Contact;
    setAddressText();
}

void AddressDialog::disableCancel()
{
    allowEmptyAddress = false;
    ui->AddCancel->hide();
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    qDebug()<<Q_FUNC_INFO<<__LINE__;
}

void AddressDialog::setVoucherType(int value)
{
    voucherType = value;
}

void AddressDialog::on_selectButton_clicked()
{
    QModelIndex index = ui->tableWidget->currentIndex();
    ui->phoneEdit->blockSignals(true);

    ui->phoneEdit->setText(ui->tableWidget->item(index.row(),2)->text());
    ui->phoneEdit->blockSignals(false);

    on_phoneEdit_returnPressed();
    ui->tabWidget->setCurrentIndex(0);
}

void AddressDialog::on_okButton_clicked()
{
    setTable();
}

void AddressDialog::reset()
{
    ui->codeLineEdit->clear();
    ui->phoneEdit->clear();
    ui->NameEdit->clear();
    ui->AddressText->clear();
    ui->routeLineEdit->clear();
    ui->activeCheckBox->setChecked(false);
    ui->deliveryChargesLineEdit->setText("0.00");
    ui->emailLineEdit->clear();
    ui->showMeasurements->setText("Show Measurements");
    ui->showMeasurements->hide();
    locationsList->hide();
}

void AddressDialog::completerActivated(QModelIndex index)
{
    //    addr->_id = nameCompleter->completionModel()->index(index.row(), 0).data().toInt();
    //    addr->ContactName = nameCompleter->completionModel()->index(index.row(), 1).data().toString();
    addr->PhoneNumber = Phonecompleter->completionModel()->index(index.row(), 2).data().toString();
    //    addr->address = nameCompleter->completionModel()->index(index.row(), 3).data().toString();
    //    addr->route = nameCompleter->completionModel()->index(index.row(), 4).data().toString();
    //    addr->code = nameCompleter->completionModel()->index(index.row(), 5).data().toString();
    //    QString route = nameCompleter->completionModel()->index(index.row(), 3).data().toString();
    qDebug()<<"XXXXXXX)))))>>>>>>>>>PHONE : "<<addr->PhoneNumber;
    addr= dbHelper->getContactPtrByPhone(addr->PhoneNumber);
    setAddressText();

    //    ui->routeLineEdit->setText(route);
}

void AddressDialog::completerActivatedByName(QModelIndex index)
{
    //    addr->_id = nameCompleter->completionModel()->index(index.row(), 0).data().toInt();
    //    addr->ContactName = nameCompleter->completionModel()->index(index.row(), 1).data().toString();
    addr->PhoneNumber = nameCompleter->completionModel()->index(index.row(), 2).data().toString();
    //    addr->address = nameCompleter->completionModel()->index(index.row(), 3).data().toString();
    //    addr->route = nameCompleter->completionModel()->index(index.row(), 4).data().toString();
    //    addr->code = nameCompleter->completionModel()->index(index.row(), 5).data().toString();
    //    QString route = nameCompleter->completionModel()->index(index.row(), 3).data().toString();

    addr= dbHelper->getContactPtrByPhone(addr->PhoneNumber);
    setAddressText();

    //    ui->routeLineEdit->setText(route);
}

void AddressDialog::setTable()
{
    ui->tableWidget->setRowCount(0);
    QString dateFrom = ui->fromDate->dateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString dateTo =  ui->toDate->dateTime().toString("yyyy-MM-dd hh:mm:ss");

    QString qStr = "SELECT TimeStamp, Number,";
    qStr += " (SELECT NAME FROM customer_details WHERE phone = LOG.Number limit 1),";
    qStr += " (SELECT ADDRESS FROM customer_details WHERE phone = LOG.Number limit 1), ";
    qStr += " (SELECT route FROM customer_details WHERE phone = LOG.Number limit 1) ";
    qStr += " FROM call_log LOG WHERE TIMESTAMP >='"+dateFrom+"' and ";
    qStr += " TIMESTAMP <='"+dateTo+"' order by _id desc";

    //    qDebug()<<qStr;

    //    QSqlQuery query(db);

    //    if(!query.exec(qStr)){

    //        //        qDebug()<<query.lastError();
    //    }
    //    else{
    //        int i =0,c=0;
    //        //        qDebug()<<" Inside loop"<<query.size();
    //        while(query.next()){
    //            c=0;
    //            ui->tableWidget->insertRow(i);

    //            QTableWidgetItem* h1 = new QTableWidgetItem;
    //            h1->setText(QString::number(i+1));
    //            ui->tableWidget->setItem(i,c++,h1);

    //            QTableWidgetItem* h2 = new QTableWidgetItem;
    //            h2->setText(query.value(0).toDateTime().toString("dd-MM-yyyy hh:mm"));
    //            ui->tableWidget->setItem(i,c++,h2);

    //            QTableWidgetItem* h3 = new QTableWidgetItem;
    //            h3->setText(query.value(1).toString());
    //            ui->tableWidget->setItem(i,c++,h3);

    //            QTableWidgetItem* h4 = new QTableWidgetItem;
    //            h4->setText(query.value(2).toString());
    //            ui->tableWidget->setItem(i,c++,h4);

    //            QTableWidgetItem* h5 = new QTableWidgetItem;
    //            h5->setText(query.value(3).toString());
    //            ui->tableWidget->setItem(i,c++,h5);

    //            QTableWidgetItem* h6 = new QTableWidgetItem;
    //            h6->setText(query.value(4).toString());
    //            ui->tableWidget->setItem(i,c++,h6);

    //            QTableWidgetItem* h7 = new QTableWidgetItem;
    //            h7->setText(query.value(4).toString());
    //            ui->tableWidget->setItem(i,c++,h7);
    //            i++;
    //        }
    //    }
}

void AddressDialog::on_close_clicked()
{
    this->close();
}

void AddressDialog::on_addOk_clicked()
{
    on_buttonBox_accepted();
}

void AddressDialog::on_NextButton_clicked()
{
    if(callCount <= 0){
        ui->NextButton->hide();
        return;
    }
    else{
        ui->NextButton->show();
    }
    ui->prevButton->show();
    callCount--;
    ui->phoneEdit->blockSignals(true);

    ui->phoneEdit->setText(ui->tableWidget->item(callCount,2)->text());
    ui->phoneEdit->blockSignals(false);

    on_phoneEdit_returnPressed();
}

void AddressDialog::on_prevButton_clicked()
{
    if(ui->tableWidget->rowCount()-1==callCount){
        ui->prevButton->hide();
        return;
    }
    else{
        ui->prevButton->show();
    }
    ui->NextButton->show();
    callCount++;
    ui->phoneEdit->blockSignals(true);

    ui->phoneEdit->setText(ui->tableWidget->item(callCount,2)->text());
    ui->phoneEdit->blockSignals(false);

    on_phoneEdit_returnPressed();
}

void AddressDialog::on_AddCancel_clicked()
{
    this->close();
}

void AddressDialog::on_codeLineEdit_returnPressed()
{
    QString code = ui->codeLineEdit->text();
    if(code.length() == 0 )
        return;
    addr = dbHelper->getContactPtrByCode(code);
    qDebug()<<Q_FUNC_INFO<<__LINE__<<addr->_id;
    if(addr->code.length() == 0)
        addr->code = code;
    reset();
    setAddressText();
}

void AddressDialog::setLedger(LedgerMasterDataModel item)
{
    addr->ledgerId = item.LedgerID;
    editMode = true;
}

void AddressDialog::setupMeasurements()
{
    // MeasurementsDatabaseHelper *dbHelper = new MeasurementsDatabaseHelper();
    // ContactMeasurement *cm = new ContactMeasurement();
    // cm->contact = addr;

    // measurePage = new ContactMeasurementPage(cm,dbHelper,this);
    // measurePage->setWindowFlags(Qt::Window);
    // measurePage->showMaximized();

    // ui->tabWidget->addTab(measurePage,"Measurement");

}

void AddressDialog::on_showMeasurements_clicked()
{
    MeasurementsDatabaseHelper *dbHelper = new MeasurementsDatabaseHelper();
    ContactMeasurement *cm = voucher->measurement;

    if(cm == nullptr){
        cm = new ContactMeasurement();
        cm->measurement = nullptr;
        cm->contactID = addr->addressId;
    }

    if(cm->measurement != nullptr){
        qDebug()<<"Its Not Null to begin with";
    }
    else{
        qDebug()<<"Its Null from HERE with addressid : "<<cm->contactID;
    }

    measurePage = new ContactMeasurementPage(cm,dbHelper,this);
    measurePage->setWindowFlags(Qt::Window);
    measurePage->showMaximized();
    connect(measurePage,&ContactMeasurementPage::measurementSelected,this,[=](ContactMeasurement *m){
        voucher->measurement = m;
        ui->showMeasurements->setText(m->measurement->measurementName + "-" + m->reference);
        qDebug()<<"Selected at dialog :"<<m->lables[0].lable.labelName;
    });
}

