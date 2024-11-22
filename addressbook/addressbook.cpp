#include "addressbook.h"
#include "ui_addressbook.h"

AddressBook::AddressBook(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddressBook)
{
    ui->setupUi(this);
    this->setWindowTitle("Contacts");
    ui->tabWidget->setStyleSheet("background-color:#ffffff");
    ui->tabWidget->setCurrentIndex(0);
    //    empData = new EmployeeAddressDataModel;
    //    addressData = new AddressBookDataModel;
    //    addressData->ContactName ="tet";

    ledgerHelper = new LedgerMasterDatabaseHelper();
    dbHelper = new AddressBookDatabaseHelper();
    allLedgersPtr = ledgerHelper->getAllLedgersAsPtr();



    vendSearch = new GMLedgerSearch(allLedgersPtr);
    QPointer<QWidget> SearchWid = new QWidget;
    QPointer<QVBoxLayout> lay = new QVBoxLayout();
    lay->setContentsMargins(0,0,0,0);
    lay->setSpacing(0);
    lay->addWidget(vendSearch);
    SearchWid->setLayout(lay);

    ui->horizontalLayout_2->addWidget(SearchWid);

    custSearch = new GMLedgerSearch(allLedgersPtr);
    QPointer<QWidget> SearchWid1 = new QWidget;
    QPointer<QVBoxLayout> lay1 = new QVBoxLayout();
    lay1->setContentsMargins(0,0,0,0);
    lay1->setSpacing(0);
    lay1->addWidget(custSearch);
    SearchWid1->setLayout(lay1);

    QObject::connect(custSearch,SIGNAL(SelectedLedger(LedgerMasterDataModel)),this,SLOT(setLedger(LedgerMasterDataModel)));

    ui->horizontalLayout->addWidget(SearchWid1);



}

AddressBook::~AddressBook()
{
    delete ui;
}

void AddressBook::edit(int id)
{

    AddressBookDataModel mod = dbHelper->getContact(id);
    contactId = id;
    ui->customerNamelineEdit->setText(mod.ContactName);
    ui->companyLineEdit->setText(mod.CompanyName);
    ui->locationLineEdit->setText(mod.location);
    ui->customerEmailLineEdit->setText(mod.email);
    ui->customerTelephoneLineEdit->setText(mod.PhoneNumber);
    ledger = ledgerHelper->getLedgerObjectByID(mod.ledgerId);
    custSearch->setText(ledger.LedgerName);
    ui->streetLineEdit->setText(mod.Street);
    ui->cityLineEdit->setText(mod.city);
    ui->countryLineEdit->setText(mod.country);
    ui->pOBoxLineEdit->setText(mod.POBox);
    ui->mobileLineEdit->setText(mod.mobileNumber);
    ui->customerDesignationLineEdit->setText(mod.Designation);
    ui->addressLineLineEdit->setText(mod.address);
    ui->faxLineEdit->setText(mod.Fax);
}

void AddressBook::setLedger(LedgerMasterDataModel led)
{
    ledger = led;
    ui->companyLineEdit->setText(led.LedgerName);
}

void AddressBook::on_customerButtonBox_rejected()
{
    this->close();
}

void AddressBook::on_customerButtonBox_accepted()
{
    AddressBookDataModel add;
    add.ContactName = ui->customerNamelineEdit->text();
    add.CompanyName = ui->companyLineEdit->text();
    add.PhoneNumber = ui->customerTelephoneLineEdit->text();
    add.email       = ui->customerEmailLineEdit->text();
    add.Building    = ui->buildingLineEdit->text();
    add.Street      = ui->streetLineEdit->text();
    add.location    = ui->locationLineEdit->text();
    add.city        = ui->cityLineEdit->text();
    add.country     = ui->countryLineEdit->text();
    add.Designation = ui->customerDesignationLineEdit->text();
    add.ledgerId    = ledger.LedgerID;
    add.Type        = 1;
    add.mobileNumber= ui->mobileLineEdit->text();
    add.address     = ui->addressLineLineEdit->text();
    add.Fax         = ui->faxLineEdit->text();

    if(contactId==0){
        dbHelper->insertContact(add);
    }
    else{
        dbHelper->updateContact(add,contactId);
    }

    this->close();
    emit closing();
}

void AddressBook::on_vendorButtonBox_accepted()
{
    this->close();
    emit closing();
}

void AddressBook::on_employeeButtonBox_accepted()
{

    this->close();
    emit closing();
}
