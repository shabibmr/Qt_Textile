#include "addquotation.h"
#include "ui_addquotation.h"
#include "customwidgets/gmsearchlineedit.h"
#include  "customwidgets/listviewdelegate.h"
#include <QCompleter>
#include <QStandardItem>
#include <QTableView>
#include <QListWidget>
#include <QDialogButtonBox>
#include <QFileDialog>
#include "datamodels/projects/Quotation/quotationstatus.h"

addQuotation::addQuotation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addQuotation)
{
    ui->setupUi(this);

    ui->addItemPushButton->hide();
    ui->addAdditionalPushButton->hide();
    ui->ReviseButton->hide();



    voucher = new GeneralVoucherDataObject();

    ui->itemTableWidget->verticalHeader()->hide();

    reviseButton = new QToolButton;
    reviseButton->setText("Revise");
    reviseButton->setIcon(QIcon(":/images/documentExport.ico"));
    reviseButton->setIconSize(QSize(32,32));
    reviseButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    reviseButton->setHidden(true);
    reviseButton->setAutoRaise(true);
    ui->extrasLayout->addWidget(reviseButton);
    QObject::connect(reviseButton,SIGNAL(clicked(bool)),this,SLOT(reviseButtonClicked(bool)));

    exportButton = new QToolButton;
    exportButton->setText("Export To\n Sales Order");
    exportButton->setIcon(QIcon(":/images/documentExport.ico"));
    exportButton->setIconSize(QSize(32,32));
    exportButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    exportButton->setHidden(true);
    exportButton->setAutoRaise(true);
    ui->extrasLayout->addWidget(exportButton);
    QObject::connect(exportButton,SIGNAL(clicked(bool)),this,SLOT(exportToSalesOrder()));

    managerApprovalButton = new QToolButton;
    clientApprovalButton = new QToolButton;
    AccountsApproval = new QToolButton;

    clientApprovalButton->setText("Client\n Approval");
    clientApprovalButton->setIcon(QIcon(":/images/documentExport.ico"));
    clientApprovalButton->setIconSize(QSize(32,32));
    clientApprovalButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    clientApprovalButton->setHidden(true);
    clientApprovalButton->setAutoRaise(true);
    ui->extrasLayout->addWidget(clientApprovalButton);

    managerApprovalButton ->setText("Completed");
    managerApprovalButton ->setIcon(QIcon(":/icons/check.ico"));
    managerApprovalButton ->setIconSize(QSize(32,32));
    managerApprovalButton ->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    managerApprovalButton ->setHidden(true);
    managerApprovalButton ->setAutoRaise(true);
    ui->extrasLayout->addWidget(managerApprovalButton);

    AccountsApproval ->setText("Accounts\nApproval");
    AccountsApproval ->setIcon(QIcon(":/icons/check.ico"));
    AccountsApproval->setIconSize(QSize(32,32));
    AccountsApproval ->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    AccountsApproval ->setHidden(true);
    AccountsApproval ->setAutoRaise(true);
    ui->extrasLayout->addWidget(AccountsApproval);

    QObject::connect(clientApprovalButton,SIGNAL(clicked(bool)),this,SLOT(clientApprovalButtonClicked()));
    //managerApprovalButtonClicked
    QObject::connect(managerApprovalButton,SIGNAL(clicked(bool)),this,
                     SLOT(managerApprovalButtonClicked(bool)));
    QObject::connect(AccountsApproval,SIGNAL(clicked(bool)),this,
                     SLOT(accountsApprovalButtonClicked(bool)));



    printButton = new QToolButton(this);
    printButton ->setText("Print");
    printButton ->setIcon(QIcon(":/images/printer1.ico"));
    printButton ->setIconSize(QSize(32,32));
    printButton ->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    printButton ->setHidden(true);
    printButton ->setAutoRaise(true);
    ui->extrasLayout->addWidget(printButton);

    ui->revLabel->hide();
    ui->revLineEdit->hide();

    ui->discountAmountLineEdit->setValidator(new QDoubleValidator(0,999999,2,this));
    ui->discountPercentLineEdit->setValidator(new QDoubleValidator(0,999999,2,this));

    ui->discountTotal->setStyleSheet("background-color:#ffffff;color:#000000;");
    ui->subTotal->setStyleSheet("background-color:#ffffff;color:#000000;");
    ui->totalAmountLineEdit->setStyleSheet("background-color:#ffffff;color:#000000;");
    ui->userName->setText(LoginValues::getEmpName());
    Username = LoginValues::getUserName();


    QObject::connect(printButton,SIGNAL(clicked(bool)),this,SLOT(printQuotation()));

    ui->tabWidget->setCurrentIndex(0);
    this->setWindowTitle("Quotation");

    ui->voucherNoLineEdit->setEnabled(false);
    ui->voucherNoLineEdit->setStyleSheet("border:none;");
    ui->voucherDateEdit->setDate(QDate::currentDate());

    ui->totalAmountLineEdit->setStyleSheet("border:none;");

    itemsHelper = new SalesInventoryItemDatabaseHelper();
    dbHelper = new QuotationDatabaseHelper;
    requirementsDatabaseHelper = new RequirementsDatabaseHelper;
    oldQuoteHelper = new OldQuotationDatabaseHelper;

    ui->itemTableWidget->setColumnCount(14);
//    int voucherNo = dbHelper->getMaxID() + 1;
//    ui->voucherNoLineEdit->setText(QString::number(voucherNo));

    QString voucherNo = dbHelper->getNextVoucherNo(LoginValues::voucherPrefix);
    ui->voucherNoLineEdit->setText(voucherNo);

    int i=0;
    QTableWidgetItem* SLNO = new QTableWidgetItem;
    SLNO->setText("#");
    ui->itemTableWidget->setHorizontalHeaderItem(i++,SLNO);

    QTableWidgetItem* ItemName = new QTableWidgetItem;
    ItemName->setText("Item Name");
    ui->itemTableWidget->setHorizontalHeaderItem(i++,ItemName);

    //    ui->itemTableWidget->hideColumn(i);
    QTableWidgetItem* ItemDesc = new QTableWidgetItem;
    ItemDesc->setText("Description");
    ui->itemTableWidget->setHorizontalHeaderItem(i++,ItemDesc);


    QTableWidgetItem* Dimension = new QTableWidgetItem;
    Dimension->setText("Dimension");
    ui->itemTableWidget->setHorizontalHeaderItem(i++,Dimension);


    QTableWidgetItem* uom = new QTableWidgetItem;
    uom->setText("Units");
    ui->itemTableWidget->setHorizontalHeaderItem(i++,uom);

    QTableWidgetItem* ModelNumber = new QTableWidgetItem;
    ModelNumber->setText("Model Number");
    ui->itemTableWidget->setHorizontalHeaderItem(i++,ModelNumber);

    QTableWidgetItem* brandName = new QTableWidgetItem;
    brandName->setText("Brand");
    ui->itemTableWidget->setHorizontalHeaderItem(i++,brandName);

    ui->itemTableWidget->hideColumn(i);

    QTableWidgetItem* serial = new QTableWidgetItem;
    serial->setText("Serial No.");
    ui->itemTableWidget->setHorizontalHeaderItem(i++,serial);

    QTableWidgetItem* quantity = new QTableWidgetItem;
    quantity->setText("Qty");
    ui->itemTableWidget->setHorizontalHeaderItem(i++,quantity);

    QTableWidgetItem* price = new QTableWidgetItem;
    price->setText("Price");
    ui->itemTableWidget->setHorizontalHeaderItem(i++,price);

    QTableWidgetItem* discountPercent = new QTableWidgetItem;
    discountPercent->setText("Disc. %");
    ui->itemTableWidget->setHorizontalHeaderItem(i++,discountPercent);

    QTableWidgetItem* discountAmount = new QTableWidgetItem;
    discountAmount->setText("Disc. Amt");
    ui->itemTableWidget->setHorizontalHeaderItem(i++,discountAmount);

    QTableWidgetItem* subTotal = new QTableWidgetItem;
    subTotal->setText("Total");
    ui->itemTableWidget->setHorizontalHeaderItem(i++,subTotal);

    QTableWidgetItem* deleteItem = new QTableWidgetItem;
    deleteItem->setIcon(QIcon(":/icons/trash.ico"));
    ui->itemTableWidget->setHorizontalHeaderItem(i++,deleteItem);

    qDebug()<<" Columns named = "<<i;

    ui->itemTableWidget->setColumnWidth(0,25);
    //    ui->itemTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    //      ui->itemTableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->itemTableWidget->setColumnWidth(13,25);
    ui->itemTableWidget->setColumnWidth(4,50);

    ui->itemTableWidget->setColumnWidth(2,200);
//    ui->itemTableWidget->setColumnWidth(1,250);
    ui->itemTableWidget->setColumnWidth(8,60);

    ui->itemTableWidget->setColumnWidth(10,60);
    ui->itemTableWidget->setColumnWidth(11,60);


    ui->itemTableWidget->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    //    ui->itemTableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    //    ui->itemTableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);

    //    ui->itemTableWidget->resizeColumnsToContents();

    ui->additionalTableWidget->setColumnCount(2);

    QTableWidgetItem* LedgerName = new QTableWidgetItem;
    LedgerName->setText("Ledger Name");
    ui->additionalTableWidget->setHorizontalHeaderItem(0,LedgerName);

    QTableWidgetItem* ledAmount = new QTableWidgetItem;
    ledAmount->setText("Amount");

    ui->additionalTableWidget->setHorizontalHeaderItem(1,ledAmount);
    ui->additionalTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    itemsHelper = new SalesInventoryItemDatabaseHelper();
    qDebug()<<"getting all items" <<QDateTime::currentDateTime();
    itemsListPtr = itemsHelper->getALLInventoryObjectsAsPtr();
    qDebug()<<"getting all ledgers"<<QDateTime::currentDateTime();
    ledgerHelper = new LedgerMasterDatabaseHelper();
    allLedgersPtr = ledgerHelper->getAllLedgersAsPtr();
    qDebug()<<"Got All ledgers"<<QDateTime::currentDateTime();
    addressBookHelper = new AddressBookDatabaseHelper();

    addresses = addressBookHelper->getAllContactsForSearchAsPtr();
    qDebug()<<"Got All addresses"<<QDateTime::currentDateTime();


    userHelper = new UserProfileDatabaseHelper();

    contactsWidget = new GMContactSearch(addresses);
    qDebug()<<"Contacts Widget Created"<<QDateTime::currentDateTime();

    QPointer<QWidget> SearchWid = new QWidget;
    QPointer<QVBoxLayout> lay = new QVBoxLayout();
    lay->setContentsMargins(0,0,0,0);
    lay->setSpacing(0);
    lay->addWidget(contactsWidget);
    SearchWid->setLayout(lay);


    ui->ledNameLay->addWidget(SearchWid);
    qDebug()<<"Widget Placed"<<QDateTime::currentDateTime();
    QObject::connect(contactsWidget,SIGNAL(SelectedItemID(AddressBookDataModel)),
                     this,SLOT(setCustomerContact(AddressBookDataModel)));


    ui->salesmanCombo->setEditable(true);

    ui->salesmanCombo->addItem("",0);
    salesmanMap = userHelper->getAllUsers();
    QStringList empNames;

    empNames<<"";

    for(int i:salesmanMap.keys()){
        ui->salesmanCombo->addItem(salesmanMap.value(i),i);
        empNames<<salesmanMap.value(i);
    }

    QCompleter *SalesCompleter = new QCompleter(empNames,this);
    SalesCompleter->setFilterMode(Qt::MatchContains);
    SalesCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    SalesCompleter->setCompletionMode(QCompleter::PopupCompletion);

    ui->salesmanCombo->setCompleter(SalesCompleter);
    GMSearchLineEdit* salesmanLine = new GMSearchLineEdit;
    salesmanLine->setPlaceholderText("Search...");
    salesmanLine->setData(empNames);
    ui->salesmanCombo->setLineEdit(salesmanLine);

    if(LoginValues::getModel().MaxDiscountPercent>3){

    }
    else{
        ui->itemTableWidget->hideColumn(10);
        ui->itemTableWidget->hideColumn(11);
    }


    setItemTable();
    setFilesTable();

}

addQuotation::~addQuotation()
{
    delete ui;
}

void addQuotation::on_addItemPushButton_clicked()
{

}

void addQuotation::keyPressEvent(QKeyEvent *e)
{
    if ((e->key() == Qt::Key_N)  && (e->modifiers().testFlag(Qt::ControlModifier))){
        on_addItemPushButton_clicked();
    }
}


void addQuotation::setCustomerName(LedgerMasterDataModel value)
{
    CustomerName = value;
}

void addQuotation::managerApprovalButtonClicked(bool)
{
    QMessageBox::StandardButton repl;   qDebug()<<"Approve Pressed";
    repl = QMessageBox::question(this,"Quotation Approval","No more Changes can be made to this Quotation after this."
                                                           "\nIs the Quotation Verified?",
                                 QMessageBox::Yes|QMessageBox::No);
    if(repl==QMessageBox::Yes)
    {
        dbHelper->setQuotationStatus(QuotationStatus::QuotationPrepared,vNo);
        GlobalStatus = QuotationStatus::QuotationPrepared;
        emit closing();
        this->hide();
    }
}

void addQuotation::accountsApprovalButtonClicked(bool)
{
    QMessageBox::StandardButton repl;   qDebug()<<"Approve Pressed";
    repl = QMessageBox::question(this,"Quotation Approval","Are you sure to send Quote to Draftsman"
                                                           "\nIs the Quotation Ledger Verified?",
                                 QMessageBox::Yes|QMessageBox::No);
    if(repl==QMessageBox::Yes)
    {
        dbHelper->setQuotationStatus(QuotationStatus::SentToDraftsMan,vNo);
        GlobalStatus = QuotationStatus::SentToDraftsMan;
        emit closing();
        this->close();
    }
}

void addQuotation::setCustomerContact(AddressBookDataModel x)
{
    CustomerContact = addressBookHelper->getContact( x._id);
    if(x.ledgerId>0){
        CustomerName = ledgerHelper->getLedgerObjectByID(x.ledgerId);

    }
    ui->cusNamelineEdit->setText(CustomerContact.ContactName);
    ui->cusEmailLine->setText(CustomerContact.email);
    ui->cusPhoneLineEdit->setText(CustomerContact.mobileNumber);
}

void addQuotation::saveVoucher(GeneralVoucherDataObject *voucher)
{
    QuotationDetailsWidget = new QuotationDetails(this->parentWidget());
    QuotationDetailsWidget ->setWindowFlags(Qt::Window);
    QuotationDetailsWidget ->showMaximized();
    QObject::connect(QuotationDetailsWidget,SIGNAL(quoteDetailsSaved(QString,QString,QString,QString,QString,QString,QString,bool,bool,bool,bool)),
                     this,SLOT(getQuoteDetails(QString,QString,QString,QString,QString,QString,QString,bool,bool,bool,bool)));
    if(vNo == ""){
        voucher->status = QuotationStatus::Started;
        dbHelper->insertQuotation(voucher);

    }
    else{
        if(voucher->status == QuotationStatus::QuotationPrepared){
            voucher->status = QuotationStatus::QuotationRevisionStarted;
        }
        dbHelper->updateQuotation(voucher,voucher->voucherNumber);
    }
    this->close();
    emit closing();
}

void addQuotation::exportToSalesOrder()
{
    // = new GeneralVoucherDataObject();

    if(!dbHelper->getClientApprovalStatus(vNo)){
        QMessageBox box;
        box.setText("Client Approval Pending!!!");
        box.exec();
        return;
    }

    objSalesOrder = *voucher;
    objSalesOrder.voucherType="Sales Quotation";
    objSalesOrder.ledgersList = ordersLedList;

//    objSalesOrder.VoucherPrefix="A";
    objSalesOrder.voucherNumber = ui->voucherNoLineEdit->text();
    objSalesOrder.VoucherDate = ui->voucherDateEdit->date();
    objSalesOrder.narration = ui->narrationTextEdit->toPlainText();
    objSalesOrder.LPO = LPOString;
    objSalesOrder.prevTransVouchers = vNo;
    objSalesOrder.SalesmanID = ui->salesmanCombo->itemData(ui->salesmanCombo->currentIndex()).toInt();
    if(objSalesOrder.SalesmanID ==0){
        objSalesOrder.SalesmanID =SalesManId;
    }
    else{
        SalesManId=objSalesOrder.SalesmanID;
    }
    if(SalesManId==0){

        QMessageBox box; box.setText("Select Salesman"); box.exec();
        ui->salesmanCombo->setFocus();
        return;
    }

    if(CustomerName.LedgerName.length()>0){
        openSalesOrder();
    }
    else{
        LedgerMasterDataModel newLedger;
        newLedger.LedgerName = CustomerContact.CompanyName;
        AccountGroupMasterDatabaseHelper *accHelpr=new AccountGroupMasterDatabaseHelper();
        newLedger.LedgerGroupId = accHelpr->getGroupIDByName("Sundry Debtors");
        newLedger.LedgerGroupName = "Sundry Debtors";
        newLedger.City = CustomerContact.city;
        newLedger.Country = CustomerContact.country;
        newLedger.ContactPersonName = CustomerContact.ContactName;
        newLedger.ContactPersonNumber = CustomerContact.PhoneNumber;
        newLedger.ContactPersonEmail = CustomerContact.email;
        newLedger.PoBox = CustomerContact.POBox;
        newLedger.narration = CustomerContact.notes;
        newLedger.Address = CustomerContact.address + CustomerContact.Building+", "+CustomerContact.Street;
        newLedger.fax = CustomerContact.Fax;

        addLedgerWidget = new addLedger(this);
        addLedgerWidget->setWindowFlags(Qt::Window);
        addLedgerWidget->setTitle("Create Ledger");
        addLedgerWidget->setLedger(&newLedger);
        addLedgerWidget->edit();
        addLedgerWidget->setContactID(CustomerContact._id);
        addLedgerWidget->show();
        QObject::connect(addLedgerWidget,SIGNAL(closing()),this,SLOT(openSalesOrder()));
    }
}

void addQuotation::openSalesOrder()
{
    this->close();
    emit closing();

    objSalesOrder.ledgerObject=ledgerHelper->getLedgerObjectByID(addressBookHelper->getLedgerIDFromContactID(CustomerContact._id));
//    AddSalesOrderWidget = new AddSalesOrder(this);
//    AddSalesOrderWidget->setWindowFlags(Qt::Window);

//    AddSalesOrderWidget->importToSalesOrder(objSalesOrder);
//    AddSalesOrderWidget->showMaximized();
}

void addQuotation::getQuoteDetails(QString validity,QString subject,QString payment,
                                   QString warranty,QString extra,QString delivery,QString exclusion,
                                   bool printExtra,bool printNow,bool emailNow,bool pdf)
{

    if(printNow==true){
        pHelper = new PrinterHelper();
        GeneralVoucherDataObject obj;

        obj.voucherType="Sales Quotation";
        obj.InventoryItems = voucher->InventoryItems;
        obj.ledgersList = ordersLedList;
        obj.ProjectId = this->projectID;
//        obj.VoucherPrefix="A";
        if(vNo.length()>0)
            obj.voucherNumber = vNo;
        else{
            obj.voucherNumber = QString ::number(dbHelper->getMaxID());
        }
        obj.VoucherDate = ui->voucherDateEdit->date();
        obj.grandTotal = ui->totalAmountLineEdit->text().toFloat();
        obj.subTotal= ui->subTotal->text().toFloat();
        obj.narration = ui->narrationTextEdit->toPlainText();
        obj.discount = ui->discountTotal->text().toFloat();
        obj.discountinAmount = ui->discountTotal->text().toFloat();
        obj.ledgerObject = CustomerName;
        obj.Contact = CustomerContact;
        obj.Location = ui->locationLineEdit->text();
        obj.POCName = ui->cusNamelineEdit->text();
        obj.POCEmail = ui->cusEmailLine->text();
        obj.POCPhone = ui->cusPhoneLineEdit->text();

        qDebug()<<"Contact Set to"<<CustomerContact.CompanyName;


        if(obj.AddedBy.length()<1)
            obj.AddedBy = Username;
        obj.SalesmanID = ui->salesmanCombo->itemData(ui->salesmanCombo->currentIndex()).toInt();
        if(obj.SalesmanID==0){
            obj.SalesmanID=SalesManId   ;
        }

        qDebug()<<"index = "<<ui->salesmanCombo->currentIndex();
        qDebug()<<"Sales Man ID = "<<obj.SalesmanID;

        pHelper->printQuotation(validity,subject,payment,warranty,extra,delivery,exclusion,
                                printExtra,printNow,obj,pdf);
    }
}

void addQuotation::printQuotation()
{
    QuotationDetailsWidget = new QuotationDetails(this);
    QuotationDetailsWidget ->setWindowFlags(Qt::Window);
    QuotationDetailsWidget ->showMaximized();
    QObject::connect(QuotationDetailsWidget,SIGNAL(quoteDetailsSaved(QString,QString,QString,QString,QString,QString,QString,bool,bool,bool,bool)),
                     this,SLOT(getQuoteDetails(QString,QString,QString,QString,QString,QString,QString,bool,bool,bool,bool)));

}

void addQuotation::printVoucher(QString validity,QString subject,QString payment,
                                QString warranty,QString extra,QString delivery,QString exclusion,
                                bool printExtra,bool printNow,bool emailNow,bool pdf)
{
    pHelper = new PrinterHelper();
    GeneralVoucherDataObject obj;

    obj.voucherType="Quotation";
    obj.InventoryItems = voucher->InventoryItems;
    obj.ledgersList = ordersLedList;
    obj.ProjectId = this->projectID;
//    obj.VoucherPrefix="A";
    obj.subTotal = ui->subTotal->text().toFloat();
    obj.discount = ui->discountTotal->text().toFloat();
    obj.discountinAmount = ui->discountTotal->text().toFloat();
    obj.voucherNumber = ui->voucherNoLineEdit->text();
    obj.VoucherDate = ui->voucherDateEdit->date();
    obj.grandTotal = ui->totalAmountLineEdit->text().toFloat();
    obj.narration = ui->narrationTextEdit->toPlainText();
    obj.Location = ui->locationLineEdit->text();
    obj.POCName = ui->cusNamelineEdit->text();
    obj.POCEmail = ui->cusEmailLine->text();
    obj.POCPhone = ui->cusPhoneLineEdit->text();

    if(obj.AddedBy.length()<1)
        obj.AddedBy = Username;
    obj.SalesmanID = ui->salesmanCombo->itemData(ui->salesmanCombo->currentIndex()).toInt();

    if(contactsWidget->text().length()>0){
        obj.ledgerObject = CustomerName;
    }

    pHelper->printQuotation(validity,subject,payment,warranty,extra,delivery,exclusion,
                            printExtra,printNow,obj,pdf);
}

void addQuotation::reviseQuote(GeneralVoucherDataObject obj)
{

}

void addQuotation::disableSaveButton(bool val)
{
    ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(!val);
    if(LoginValues::model.reviseQuote){
        reviseButton->show();
    }
}

void addQuotation::on_buttonBox_accepted()
{

    voucher->voucherType="Quotation";
    voucher->VoucherDate  = ui->voucherDateEdit->date();

    voucher->ledgersList = ordersLedList;
    voucher->ProjectId = this->projectID;
//    voucher->VoucherPrefix="A";
    voucher->voucherNumber = ui->voucherNoLineEdit->text();
    voucher->VoucherDate = ui->voucherDateEdit->date();

    voucher->grandTotal = ui->totalAmountLineEdit->text().toFloat();
    voucher->subTotal = ui->subTotal->text().toFloat();
    voucher->discount = ui->discountTotal->text().toFloat();
    voucher->discountinAmount = ui->discountTotal->text().toFloat();
    voucher->narration = ui->narrationTextEdit->toPlainText();
    voucher->AddedBy = LoginValues::getUserName();
    voucher->AddedById = LoginValues::getUserID();qDebug()<<"Rev no : "<<revisionNumber;
    voucher->RevisionNo = revisionNumber;
    voucher->LPO = LPOString;
    voucher->status = GlobalStatus;
    voucher->Location = ui->locationLineEdit->text();
    voucher->POCName = ui->cusNamelineEdit->text();
    voucher->POCEmail = ui->cusEmailLine->text();
    voucher->POCPhone = ui->cusPhoneLineEdit->text();

    voucher->SalesmanID = ui->salesmanCombo->itemData(ui->salesmanCombo->currentIndex()).toInt();
    if(voucher->SalesmanID==0){
        voucher->SalesmanID=SalesManId   ;
    }
    else{
        SalesManId=voucher->SalesmanID;
    }

    if(SalesManId==0){
        QMessageBox box;box.setText("Select Salesman");box.exec();
        return;
    }
    if(contactsWidget->text().length()>0){

        voucher->ledgerObject = CustomerName;
        voucher->Contact = CustomerContact;
    }
    else{
        QMessageBox box;box.setText("Select Customer");box.exec();
        return;
    }


    saveVoucher(voucher);

}

void addQuotation::on_buttonBox_rejected()
{
    this->close();
}

void addQuotation::setTitle(QString Text)
{
    ui->titleLabel->setText(Text);
}

void addQuotation::setItemTable()
{
       qDebug()<<"Set Item Table";
    calcTotal();
    qDebug()<<"Calc Complete";
    ui->itemTableWidget->blockSignals(true);
    int r = voucher->InventoryItems.size();
    ui->itemTableWidget->setRowCount(0);
    ui->itemTableWidget->setRowCount(r+1);
    int i=0;qDebug()<<"Calc Complete 638";
    ui->itemTableWidget->setSortingEnabled(false);qDebug()<<"Calc Complete 639";
    for(CompoundItemDataObject obj:voucher->InventoryItems){
              qDebug()<<"####### loop count = "<<i;
        QTableWidgetItem* SLNO = new QTableWidgetItem;
        SLNO->setText(QString::number(i+1));
        SLNO->setFlags(SLNO->flags() ^ Qt::ItemIsEditable);
        ui->itemTableWidget->setItem(i,0,SLNO);

        //        qDebug()<<QDateTime::currentDateTime().toString();

        QPointer<GMItemSearch> itemLine = new GMItemSearch(itemsListPtr);
        itemLine->setText(obj.BaseItem.ItemName);
        itemLine->setProperty("row",i);

        QToolButton* SearchWid = new QToolButton;
        SearchWid->setAutoRaise(true);
        SearchWid->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Preferred);

        SearchWid->setProperty("row",i);

//        QLabel* descLabel = new QLabel;
//        descLabel->setText(obj.BaseItem.ItemDescription);

//        descLabel->setWordWrap(true);
//        descLabel->setContentsMargins(0,0,0,0);
//        descLabel->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Preferred);
//        descLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

        QVBoxLayout *lay = new QVBoxLayout();
        lay->setContentsMargins(0,0,0,0);
        lay->setSpacing(0);
        lay->addWidget(itemLine);
        //        lay->addWidget(descLabel);
        SearchWid->setLayout(lay);
        SearchWid->setContentsMargins(0,0,0,0);

        QObject::connect(SearchWid,SIGNAL(clicked(bool)),itemLine,SLOT(setFocus()));

        ui->itemTableWidget->setCellWidget(i,1,SearchWid);

        QObject::connect(itemLine,SIGNAL(SelectedItemID(inventoryItemDataModel)),
                         this,SLOT(getItemDetails(inventoryItemDataModel)));
        //        qDebug()<<QDateTime::currentDateTime().toString();

        QTableWidgetItem* desc = new QTableWidgetItem;
        desc->setFlags(desc->flags() ^ Qt::ItemIsEditable);
        desc->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        desc->setText(obj.BaseItem.ItemDescription);
        qDebug()<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$";
        qDebug()<<obj.BaseItem.ItemDescription;
        qDebug()<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$";

        ui->itemTableWidget->setItem(i,2,desc);



        QTableWidgetItem* dim = new QTableWidgetItem;
        dim->setFlags(dim->flags() ^ Qt::ItemIsEditable);
        if(obj.BaseItem.Dimension.length()>0)
            dim->setText(QString::number(obj.BaseItem.length)+"x"+obj.BaseItem.Dimension);
        ui->itemTableWidget->setItem(i,3,dim);

        QTableWidgetItem* uom = new QTableWidgetItem;
        uom->setFlags(uom->flags() ^ Qt::ItemIsEditable);

        uom->setText(obj.BaseItem.uomObject.UomName);
        ui->itemTableWidget->setItem(i,4,uom);

        QTableWidgetItem* ModelNumber = new QTableWidgetItem;
        ModelNumber->setFlags(ModelNumber->flags() ^ Qt::ItemIsEditable);
        ModelNumber->setText(obj.BaseItem.ItemCode);
        ui->itemTableWidget->setItem(i,5,ModelNumber);

        QTableWidgetItem* brandName = new QTableWidgetItem;
        brandName->setFlags(brandName->flags() ^ Qt::ItemIsEditable);
        brandName->setText(obj.BaseItem.brandName);
        ui->itemTableWidget->setItem(i,6,brandName);

        QTableWidgetItem* serial = new QTableWidgetItem;
        serial->setFlags(serial->flags() ^ Qt::ItemIsEditable);
        serial->setText(obj.BaseItem.SerailNumber);
        ui->itemTableWidget->setItem(i,7,serial);

        QTableWidgetItem* quantity = new QTableWidgetItem;
        quantity->setFlags(quantity->flags() ^ Qt::ItemIsEditable);
        quantity->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
        quantity->setText(QString::number(obj.BaseItem.quantity,'f',2));
        ui->itemTableWidget->setItem(i,8,quantity);

        QTableWidgetItem* price = new QTableWidgetItem;
        price->setFlags(price->flags() ^ Qt::ItemIsEditable);
        price->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
        if(obj.BaseItem.isCustomItem){
            price->setText(QString::number(obj.BaseItem.subTotal/obj.BaseItem.quantity,'f',2));
            if(obj.BaseItem.subTotal ==0 && obj.BaseItem.price!=0)
            {
                price->setText(QString::number(obj.BaseItem.price,'f',2));
            }
        }
        else
            price->setText(QString::number(obj.BaseItem.price,'f',2));
        ui->itemTableWidget->setItem(i,9,price);

        QLineEdit* discAmount = new QLineEdit;
        QLineEdit* discPercent = new QLineEdit;

        ui->itemTableWidget->setCellWidget(i,10,discPercent);
        ui->itemTableWidget->setCellWidget(i,11,discAmount);


        discAmount->setText(QString::number(obj.BaseItem.discountinAmount,'f',2));
        discPercent->setText(QString::number(obj.BaseItem.discountPercentage,'f',2));

        discAmount->setProperty("row",i);
        discPercent->setProperty("row",i);

        discAmount->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
        discPercent->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        QObject::connect(discAmount,SIGNAL(returnPressed()),this,SLOT(discountAmountEntered( )));
        QObject::connect(discPercent,SIGNAL(returnPressed()),this,SLOT(discountPercentEntered( )));


        QTableWidgetItem* amount = new QTableWidgetItem;
        amount->setFlags(amount->flags() ^ Qt::ItemIsEditable);
        amount->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
        amount->setText(QString::number((obj.BaseItem.subTotal+obj.subItemsTotal),'f',2));
        ui->itemTableWidget->setItem(i,12,amount);

        QToolButton* delButton = new QToolButton;
        delButton->setIcon(QIcon(":/icons/trash.ico"));
        delButton->setProperty("row",i);
        delButton->setAutoRaise(true);
        //deleteItem->setIcon(QIcon(":/icons/trash.ico"));
        ui->itemTableWidget->setCellWidget(i,13,delButton);
        QObject::connect(delButton,SIGNAL(clicked(bool)),this,SLOT(delButtonClicked(bool)));

        //        ui->itemTableWidget->setRowHeight(i,descLabel->height()+itemLine->height());

        ui->itemTableWidget->resizeRowToContents(i);
//        ui->itemTableWidget->resizeColumnToContents(1);

        i++;
    }

    ui->itemTableWidget->setSortingEnabled(true);

    GMItemSearch *itemLine = new GMItemSearch(itemsListPtr);

    itemLine->setProperty("row",i);

    QWidget *SearchWid = new QWidget;
    SearchWid->setProperty("row",i);
    QVBoxLayout *lay = new QVBoxLayout();
    lay->setContentsMargins(0,0,0,0);
    lay->setSpacing(0);
    lay->addWidget(itemLine);
    SearchWid->setLayout(lay);
    ui->itemTableWidget->setCellWidget(i,1,SearchWid);

    QObject::connect(itemLine,SIGNAL(SelectedItemID(inventoryItemDataModel)),
                     this,SLOT(getItemDetails(inventoryItemDataModel)));
    QPointer<QLineEdit> uomEmpty = new QLineEdit;
    uomEmpty->setEnabled(false);
    uomEmpty->setStyleSheet("QLineEdit { border: none }");
    ui->itemTableWidget->setCellWidget(i,2,uomEmpty);
    qDebug()<<"set up uom done";

    ui->itemTableWidget->scrollToBottom();
    //QObject::connect(ItemNameCombo,SIGNAL(activated(int)),this,SLOT(getItemDetails(int)));
    ui->itemTableWidget->blockSignals(false);
    setLedgerTable();
    itemLine->setFocus();

    //   calcLedgerTotal();

}



void addQuotation::getItemDetails(inventoryItemDataModel obj)
{

    //    qDebug()<<openNewWidget;


    if(openNewWidget){
        //        qDebug()<<"inside";
        openNewWidget = false;

        addItemRequirementWidget = new addItemRequirement(this);
        addItemRequirementWidget->setWindowFlags(Qt::Window);

        int currRow = sender()->property("row").toInt();

        if (currRow == -1) {
            if (QWidget* focused = ui->itemTableWidget->focusWidget()) {
               currRow = focused->property("row").toInt();
            }
        }

        addItemRequirementWidget->setItemsListPtr(itemsListPtr);
        addItemRequirementWidget->showOptional();
        addItemRequirementWidget->setListNumber(currRow);
        addItemRequirementWidget->getFirstItemDetail(obj);
        addItemRequirementWidget->showMaximized();
        QObject::connect(addItemRequirementWidget,SIGNAL(addNewItem(CompoundItemDataObject,int)),
                         this,SLOT(appendItem(CompoundItemDataObject,int)));
        QObject::connect(addItemRequirementWidget,SIGNAL(insertNewItem(CompoundItemDataObject,int)),
                         this,SLOT(insertItem(CompoundItemDataObject,int)));

        QObject::connect(addItemRequirementWidget,SIGNAL(closing(int)),
                         this,SLOT(resetFlag(int)));

    }
}

void addQuotation::appendItem(CompoundItemDataObject item, int row)
{
    qDebug()<<"Modify or insert a new item at the end"<<row << "list size = "<<voucher->InventoryItems.size();
    openNewWidget = true;
    //    qDebug()<<"edited row sub tot= "<<item.subItemsTotal;

    //    qDebug()<<"row count = "<<ui->itemTableWidget->rowCount();
    if(ui->itemTableWidget->rowCount()==row+1){
        voucher->InventoryItems.append(item);

        //        qDebug()<<"item appended";
    }
    else{
        qDebug()<<"item Changed at "<<row;
        voucher->InventoryItems[row]=item;
                qDebug()<<"item Changed at "<<row;
    }
    setItemTable();

}

void addQuotation::insertItem(CompoundItemDataObject item, int row)
{
    qDebug()<<"Insert a new item at "<<row;
    openNewWidget = true;
    //    qDebug()<<"edited row sub tot= "<<item.subItemsTotal;

    //    qDebug()<<"row count = "<<ui->itemTableWidget->rowCount();
    if(ui->itemTableWidget->rowCount()==row+1){
        voucher->InventoryItems.append(item);

        //        qDebug()<<"item appended";
    }
    else{
        voucher->InventoryItems.insert(row,item);
        //        qDebug()<<"item Inserted at "<<row;
    }
    setItemTable();

}


void addQuotation::setProjectID(QString projID)
{
    this->projectID = projID;
}

void addQuotation::delButtonClicked(bool)
{
    int x= sender()->property("row").toInt();

    voucher->InventoryItems.removeAt(x);
    setItemTable();
}

void addQuotation::calcTotal()
{
    ui->SelectedItemTotalLabel->clear();

    voucher->subTotal = 0;
    voucher->grossTotal     = 0;
    voucher->discountinAmount = 0;
    voucher->discountPercent= 0;

    voucher->taxTotalAmount      = 0;
    voucher->grandTotal     = 0;

    for(int i=0;i<voucher->InventoryItems.size();i++)
    {
        if(voucher->InventoryItems[i].BaseItem.subTotal == 0 &&
                voucher->InventoryItems[i].BaseItem.price>0 &&
                voucher->InventoryItems[i].BaseItem.discountPercentage !=100){
        }
        else{
            float price             = voucher->InventoryItems[i].BaseItem.price;
            float qty               = voucher->InventoryItems[i].BaseItem.quantity;
            float length            = voucher->InventoryItems[i].BaseItem.length/1000;
            float itemDiscountAmt   = voucher->InventoryItems[i].BaseItem.discountinAmount;
            float itemDiscountPrct  = voucher->InventoryItems[i].BaseItem.discountPercentage;

            if(itemDiscountAmt == 0 && itemDiscountPrct >0){
                itemDiscountAmt = (price * qty * length) *itemDiscountPrct/100;
            }

            if(itemDiscountAmt > 0 && itemDiscountPrct == 0){
                itemDiscountPrct = itemDiscountPrct / (price * qty * length)  * 100;
            }


            float subItemPrice = price * qty * length;
            itemDiscountAmt    = itemDiscountPrct /100 * price * qty * length;
            float itemSubTotal = (price*qty*length)-itemDiscountAmt;


            float itemVatAmount    = voucher->InventoryItems[i].BaseItem.taxRate/100 *itemSubTotal;

            voucher->InventoryItems[i].BaseItem.discountPercentage = itemDiscountPrct;
            voucher->InventoryItems[i].BaseItem.discountinAmount = itemDiscountAmt;
            voucher->InventoryItems[i].BaseItem.subTotal = itemSubTotal;
            voucher->InventoryItems[i].BaseItem.taxAmount = itemVatAmount;
            //            voucher->InventoryItems[i].BaseItem.grandTotal = itemSubTotal + itemVatAmount

            voucher->subTotal += subItemPrice;
            voucher->taxTotalAmount+=itemVatAmount;
            voucher->discountinAmount += itemDiscountAmt;
        }
    }

    voucher->grossTotal = voucher->subTotal - voucher->discountinAmount;
    voucher->discountPercent = voucher->discountinAmount / (voucher->subTotal) *100;
    voucher->grandTotal = voucher->grossTotal + voucher->taxTotalAmount;

    if(voucher->discountPercent > LoginValues::getModel().MaxDiscountPercent){
        printButton->hide();
        ui->buttonBox->button(QDialogButtonBox::Save)->hide();
        QMessageBox::information(this,"Discount Exceeded Limits"
                                 ,"You have Entered discount beyond allowed Limit\n"
                                  "Please Get it done by your Manager..",QMessageBox::Ok);
    }
    else{
        printButton->show();
        ui->buttonBox->button(QDialogButtonBox::Save)->show();
    }

    ui->subTotal->setText(QString::number(voucher->subTotal,'f',2));
    ui->discountTotalPercent->setText(QString::number(voucher->discountPercent,'f',2));
    ui->discountTotal->setText(QString::number(voucher->discountinAmount,'f',2));

    ui->vatTotalLineEdit->setText(QString::number(voucher->taxTotalAmount,'f',2));
    ui->totalAmountLineEdit->setText(QString::number(voucher->grandTotal,'f',2));

}

void addQuotation::showVoucher(GeneralVoucherDataObject voucher)
{
    ui->voucherNoLineEdit->setText(voucher.voucherNumber);
    ui->voucherDateEdit->setDate(voucher.VoucherDate);
    ui->narrationTextEdit->setPlainText(voucher.narration);

    ui->salesmanCombo->setCurrentText(salesmanMap.value(voucher.SalesmanID));
    Username =  userHelper->getEmployeeNameFromID(voucher.AddedById);
    userID = voucher.AddedById;
    ui->userName->setText(Username);

    vNo = voucher.voucherNumber;

    vPrefix =voucher.VoucherPrefix;
    SalesManId = voucher.SalesmanID;
    CustomerContact=voucher.Contact;
    LPOString = voucher.LPO;

    if(voucher.RevisionNo>0){

        ui->revLabel->show();
        ui->revLineEdit->show();
        ui->revLineEdit->setText(QString::number(voucher.RevisionNo));
    }

    GeneralVoucherDataObject obj = dbHelper->getVoucherByVoucherNo(voucher.voucherNumber);
    contactsWidget->setText(CustomerContact.CompanyName);

    ui->locationLineEdit->setText(obj.Location);
    ui->cusNamelineEdit->setText(obj.POCName);
    ui->cusPhoneLineEdit->setText(obj.POCPhone);
    ui->cusEmailLine->setText(obj.POCEmail);
    ui->discountAmountLineEdit->setText(QString::number(obj.discountinAmount,'f',2));
    if(CustomerContact.ledgerId.length()>0){
        CustomerName = ledgerHelper->getLedgerObjectByID(CustomerContact.ledgerId);
        //        qDebug()<<"At Show Voucher"<<CustomerName.LedgerName;
    }
    this->voucher->InventoryItems = obj.InventoryItems;
    ordersLedList = obj.ledgersList;
    if(obj.status >= QuotationStatus::SentToDraftsMan && LoginValues::getModel().Sales_Orders_Add){
        exportButton->setHidden(false);
    }
    if(obj.status == QuotationStatus::QuotationPrepared){
        clientApprovalButton->setHidden(false);

    }
    else if (obj.status == QuotationStatus::Started||obj.status==QuotationStatus::QuotationRevisionStarted){
        managerApprovalButton->setHidden(false);
    }
    else if (obj.status == QuotationStatus::ClientApprovedQuoatation && LoginValues::getModel().Finances){
        AccountsApproval->setHidden(false);
    }

    GlobalStatus = obj.status;
    printButton->setHidden(false);qDebug()<<"695";
    setItemTable();

    ui->discountAmountLineEdit->setText(ui->discountTotal->text());
    ui->discountPercentLineEdit->setText(ui->discountTotalPercent->text());

    if(LoginValues::model.Quotes_Update ==false || obj.status>QuotationStatus::Started){
        disableSaveButton(true);
    }
    qDebug()<<"Show Voucher Completed";
}

void addQuotation::importToQuotation(GeneralVoucherDataObject voucher)
{
    contactsWidget->setText(voucher.ledgerObject.LedgerName);
    CustomerName = voucher.ledgerObject;
    ui->narrationTextEdit->setPlainText(voucher.narration);

    ui->salesmanCombo->setCurrentText(salesmanMap.value(voucher.SalesmanID));
    SalesManId = voucher.SalesmanID;
    CustomerContact=voucher.Contact;
    contactsWidget->setText(CustomerContact.CompanyName);

    if(CustomerContact.ledgerId.length()>0){
        CustomerName = ledgerHelper->getLedgerObjectByID(CustomerContact.ledgerId);
        //        qDebug()<<"At Show Voucher"<<CustomerName.LedgerName;
    }

    //    GeneralVoucherDataObject obj = dbHelper->getVoucherByVoucherNo(voucher.voucherNumber);
    this->voucher->InventoryItems = voucher.InventoryItems;
    ordersLedList = voucher.ledgersList;
    setItemTable();
}

void addQuotation::importMultipleVouchers(QList<GeneralVoucherDataObject> vouchers)
{
    for(GeneralVoucherDataObject voucher:vouchers)
    {
        contactsWidget->setText(voucher.ledgerObject.LedgerName);
        CustomerName = voucher.ledgerObject;
        GeneralVoucherDataObject obj = dbHelper->getVoucherByVoucherNo(voucher.voucherNumber);
        voucher.InventoryItems.append(obj.InventoryItems);
        ordersLedList.append(obj.ledgersList);
    }
    setItemTable();
}

void addQuotation::getLedgerDetails(LedgerMasterDataModel led)
{

    //    qDebug()<<"444"<<"Ledger Selected";
    int row = sender()->property("row").toInt();
    appendLedger(led,row);
    setLedgerTable();
    qobject_cast< QLineEdit*>(ui->additionalTableWidget->cellWidget(row,1))->setFocus();
    qobject_cast< QLineEdit*>(ui->additionalTableWidget->cellWidget(row,1))->selectAll();
    return;
    addItemAdditionalRequirementWidget = new addItemAdditionalRequirement();
    addItemAdditionalRequirementWidget->setWindowFlags(Qt::Window);
    addItemAdditionalRequirementWidget->edit(led);
    addItemAdditionalRequirementWidget->show();

}

void addQuotation::setLedgerTable()
{
    int ledTableSize  = voucher->InventoryItems.size() + ordersLedList.size();
    ui->additionalTableWidget->setRowCount(0);
    ui->additionalTableWidget->setRowCount(ledTableSize);


    int rCnt = 0;

    for(CompoundItemDataObject item:voucher->InventoryItems){
        QPointer<GMLedgerSearch> ledSearch = new GMLedgerSearch(allLedgersPtr);
        QPointer<QWidget> SearchWid = new QWidget;
        QPointer<QVBoxLayout> lay = new QVBoxLayout();
        lay->setContentsMargins(0,0,0,0);
        lay->setSpacing(0);
        lay->addWidget(ledSearch);
        SearchWid->setLayout(lay);
        ledSearch->setEnabled(false);
        ledSearch->setText(ledgerHelper->getLedgerNameByID(item.BaseItem.defaultSalesLedgerID));
//        qDebug()<<"Default sales item Name = "<<item.BaseItem.ItemName;
//        qDebug()<<"Default sales Ledger = "<<item.BaseItem.defaultSalesLedgerID;
//        qDebug()<<"Ledger Name = "<<ledgerHelper->getLedgerNameByID(item.BaseItem.defaultSalesLedgerID);
        ledSearch->setProperty("row",rCnt);
        SearchWid->setProperty("row",rCnt);
        ui->additionalTableWidget->setCellWidget(rCnt,0,SearchWid);


        QLineEdit *amountEdit = new QLineEdit;
        amountEdit ->setValidator(new QDoubleValidator(0.0002,999999,2,this));
        amountEdit->setFrame(QFrame::NoFrame);
        amountEdit->setText(QString::number(item.BaseItem.subTotal,'f',2));
        amountEdit->setEnabled(false);
        ui->additionalTableWidget->setCellWidget(rCnt,1,amountEdit);
        rCnt++;
    }

    for(LedgerMasterDataModel ledger:ordersLedList){

        QPointer<GMLedgerSearch> ledSearch = new GMLedgerSearch(allLedgersPtr);
        QPointer<QWidget> SearchWid = new QWidget;
        QPointer<QVBoxLayout> lay = new QVBoxLayout();
        lay->setContentsMargins(0,0,0,0);
        lay->setSpacing(0);
        lay->addWidget(ledSearch);
        SearchWid->setLayout(lay);
        ledSearch->setText(ledger.LedgerName);
        ledSearch->setProperty("row",rCnt);
        SearchWid->setProperty("row",rCnt);
        ui->additionalTableWidget->setCellWidget(rCnt,0,SearchWid);
        QObject::connect(ledSearch,SIGNAL(SelectedLedger(LedgerMasterDataModel)),
                         this,SLOT(getLedgerDetails(LedgerMasterDataModel)));

        QLineEdit *amountEdit = new QLineEdit;
        amountEdit ->setValidator(new QDoubleValidator(0.0002,999999,2,this));
        amountEdit->setFrame(QFrame::NoFrame);
        qDebug()<<"454 LEDGER AMOUNT  "<<ledger.amount;
        amountEdit->setText(QString::number(ledger.amount,'f',2));
        ui->additionalTableWidget->setCellWidget(rCnt,1,amountEdit);
        amountEdit->setProperty("row",rCnt);
        QObject::connect(amountEdit,SIGNAL(editingFinished()),this,SLOT(updateLedgerValue()));

        rCnt++;
        qDebug()<<rCnt<< "Done ************************************";
    }

    ui->additionalTableWidget->insertRow(ledTableSize);

    QPointer<GMLedgerSearch> ledSearch = new GMLedgerSearch(allLedgersPtr);
    QPointer<QWidget> SearchWid = new QWidget;
    QPointer<QVBoxLayout> lay = new QVBoxLayout();
    lay->setContentsMargins(0,0,0,0);
    lay->setSpacing(0);
    lay->addWidget(ledSearch);
    SearchWid->setLayout(lay);
    ledSearch->setProperty("row",ledTableSize);
    SearchWid->setProperty("row",ledTableSize);
    ui->additionalTableWidget->setCellWidget(ledTableSize,0,SearchWid);

    //QObject::connect(ledCombo,SIGNAL(activated(int)),this,SLOT(getLedgerDetails(LedgerMasterDataModel led)));
    QObject::connect(ledSearch,SIGNAL(SelectedLedger(LedgerMasterDataModel)),
                     this,SLOT(getLedgerDetails(LedgerMasterDataModel)));
    QLineEdit *amountEdit = new QLineEdit;
    amountEdit ->setValidator(new QDoubleValidator(0.0002,999999,2,this));
    amountEdit->setFrame(QFrame::NoFrame);
    amountEdit->setProperty("row",ledTableSize);
    ledSearch->setFocus();
    ledSearch->selectAll();
    QObject::connect(amountEdit,SIGNAL(editingFinished()),this,SLOT(updateLedgerValue()));
    ui->additionalTableWidget->setCellWidget(ledTableSize,1,amountEdit);
    calcTotal();
}

void addQuotation::updateLedgerValue()
{
    int row = sender()->property("row").toInt();
    qDebug()<<"row = "<<row;
    qDebug()<<"Ledger size = "<<ordersLedList.size();
    qDebug()<<"item name = "<<ordersLedList[row-voucher->InventoryItems.size()].LedgerName;
    qDebug()<<"444"<<"Amount entered ro number : "<<row;
    if(row<=ordersLedList.size()){
        qDebug()<<"num ledgers"<<ordersLedList.size();
        ordersLedList[row-voucher->InventoryItems.size()].amount
                =qobject_cast<QLineEdit*>(ui->additionalTableWidget->cellWidget(row,1))
                ->text().toFloat();
    }
    setLedgerTable();
    return;
}

void addQuotation::deleteLedgerButtonClicked(bool x)
{
    int rCount= sender()->property("row").toInt();

    int invItemsCount = voucher->InventoryItems.size();
    int xtraLedgersCount = ui->additionalTableWidget->rowCount() - invItemsCount;

    if(rCount>invItemsCount-1){
        ui->additionalTableWidget->removeRow(rCount);
        ordersLedList.removeAt(rCount-invItemsCount);
    }

    setItemTable();
}

void addQuotation::appendLedger(LedgerMasterDataModel item, int row)
{
    openNewWidgetLedger = true;
    qDebug()<<"524 row ="<<row;
    QLineEdit* line = qobject_cast<QLineEdit*> (ui->additionalTableWidget->cellWidget(row,1));

    item.amount = line->text().toFloat();
    qDebug()<<"edited row sub tot= "<<item.amount;

    qDebug()<<"524 row count = "<<ui->additionalTableWidget->rowCount();
    if(ordersLedList.size()+voucher->InventoryItems.size() == row ){
        ordersLedList.append(item);
        qDebug()<<"item appended";
    }
    else{
        ordersLedList[row - voucher->InventoryItems.size()]=item;
        qDebug()<<"item Inserted at "<<row;
    }
    //  setItemTable();
    calcTotal();
}

void addQuotation::resetLedgerWidgetFlag()
{

}

void addQuotation::clientApprovalButtonClicked()
{
    approvalDialog = new QDialog(this);
    approvalDialog->setWindowTitle("Change Approval Status");
    // approvalDialog->setFixedSize(300,150);

    QVBoxLayout *vLayout =new QVBoxLayout(approvalDialog);

    QHBoxLayout *boxlayokCancel = new QHBoxLayout(approvalDialog);
    QFormLayout *flay = new QFormLayout(approvalDialog);



    vLayout->addLayout(flay);
    vLayout->addLayout(boxlayokCancel);

    QLabel *label = new QLabel(approvalDialog);
    label->setText("Client Approved : ");
    checkbox = new QCheckBox(approvalDialog);
    qDebug()<<"checkbox is "<<dbHelper->getClientApprovalStatus(vNo);
    checkbox->setChecked(dbHelper->getClientApprovalStatus(vNo));
    QWidget* attWid = new QWidget(approvalDialog);

    QHBoxLayout *attLay = new QHBoxLayout(attWid);

    LPOref = new QLineEdit(approvalDialog);
    Attachment = new QLineEdit(approvalDialog);
    Attachment->setReadOnly(true);
    tentDelivery = new QDateEdit(approvalDialog);
    tentDelivery ->setCalendarPopup(true);
    Remarks = new QLineEdit(approvalDialog);
    QLabel* lpoLabel = new QLabel(approvalDialog);
    QLabel* tentDate = new QLabel(approvalDialog);
    QLabel* AttachmentLabel = new QLabel(approvalDialog);
    QLabel* RemarksLabel = new QLabel(approvalDialog);

    QPushButton* browseButton = new QPushButton(approvalDialog);
    attLay->addWidget(Attachment);
    attLay->addWidget(browseButton);
    browseButton->setText("Browse");
    QObject::connect(browseButton,SIGNAL(clicked(bool)),this,SLOT(browseClicked(bool)));
    lpoLabel->setText("LPO Ref : ");
    tentDate->setText("Tentative Delivery : ");
    AttachmentLabel->setText("Attchament : ");
    RemarksLabel->setText("Notes : ");
    tentDelivery->setDate(QDate::currentDate());

    QPushButton *okButton  = new QPushButton(approvalDialog);
    QPushButton *cancelButton  = new QPushButton(approvalDialog);
    okButton->setText("OK");
    cancelButton->setText("Cancel");

    flay->addRow(label,checkbox);
    flay->addRow(lpoLabel,LPOref);
    flay->addRow(tentDate,tentDelivery);
    flay->addRow(AttachmentLabel,attWid);
    flay->addRow(RemarksLabel,Remarks);
    boxlayokCancel->addWidget(okButton);
    boxlayokCancel->addWidget(cancelButton);
    //okButton->setGeometry(QRect(100,100,80,35));
    //cancelButton->setGeometry(QRect(200,100,80,35));
    QObject::connect(cancelButton,SIGNAL(clicked(bool)),approvalDialog,SLOT(close()));
    QObject::connect(okButton,SIGNAL(clicked(bool)),this,SLOT(changeClientApprovalStatus(bool)));

    approvalDialog->show();
}

void addQuotation::changeClientApprovalStatus(bool)
{
    if(LPOref->text().length()<1 && checkbox->isChecked()){
        QMessageBox box;
        box.setText("Enter LPO Reference Number");
        box.exec();
    }

    else if(Remarks->text().length()<10){
        QMessageBox box;
        box.setText("A Minimum of 10 charactes mandated");
        box.exec();
    }
    else{
        approvalDialog->close();
        dbHelper->setClientApproval(vNo,checkbox->isChecked(),LPOref->text(),tentDelivery->date());
        dbHelper->setQuotationStatus(QuotationStatus::ClientApprovedQuoatation,vNo);
        GlobalStatus = QuotationStatus::ClientApprovedQuoatation;
        QFile f(Attachment->text());
        if(f.open(QIODevice::ReadOnly))
        {
            QFileInfo fileInfo(f.fileName());
            QString filename(fileInfo.fileName());

            DocumentsDataModel doc;
            doc.fileObj = f.readAll();
            f.close();
            doc.filename = filename;
            doc.fileType = fileInfo.completeSuffix();
            doc.TagType = DocumentType::LPO;
            doc.fileDescr ="LPO";
            doc.addedByID = LoginValues::getUserID();
            doc.projectID = projectID;
            doc.quoteNo = vNo;

            dbHelper->insertQImage(doc);

            emit closing();
        }
        else{
            QMessageBox box;
            box.setText("Cannot Open File!!!");
            box.exec();
        }

        LPOString=LPOref->text();

    }

}

void addQuotation::on_itemTableWidget_doubleClicked(const QModelIndex &index)
{
    qDebug()<<"table double cliked";
    int r= index.row();
    //13
    QToolButton* tool = qobject_cast<QToolButton*> (ui->itemTableWidget->cellWidget(r,13));
    r = tool->property("row").toInt();
    if(openNewWidget && voucher->InventoryItems.size()>r && r>=0){
        qDebug()<<"inside";
        openNewWidget = false;
        addItemRequirementWidget = new addItemRequirement(this);
        addItemRequirementWidget->setWindowFlags(Qt::Window);
        addItemRequirementWidget->setItemsListPtr(itemsListPtr);
        addItemRequirementWidget->edit(voucher->InventoryItems[r]);
        addItemRequirementWidget->setListNumber(r);
        addItemRequirementWidget->showMaximized();
        addItemRequirementWidget->showOptional();
        QObject::connect(addItemRequirementWidget,SIGNAL(addNewItem(CompoundItemDataObject,int)),
                         this,SLOT(appendItem(CompoundItemDataObject,int)));
        QObject::connect(addItemRequirementWidget,SIGNAL(closing(int)),
                         this,SLOT(resetFlag(int)));

    }
    //QObject::connect(addItemRequirementWidget,SIGNAL(closing())
}

void addQuotation::resetFlag(int r)
{
    qDebug()<<"vaue of r :"<<r;
    if(ui->itemTableWidget->rowCount()==r+1){

        QLineEdit* uom = qobject_cast<QLineEdit*>( ui->itemTableWidget->cellWidget(r,2));
        if(uom->text().length()<1){
            qDebug()<<"waa";
            //            GMItemSearch* combo= qobject_cast<GMItemSearch*> (ui->itemTableWidget->cellWidget(r,1));
            //            qDebug()<<combo->text();
            //            combo->setText("");
        }

    }
    openNewWidget = true;
}

void addQuotation::on_addAdditionalPushButton_clicked()
{
    addItemAdditionalRequirementWidget = new addItemAdditionalRequirement();
    addItemAdditionalRequirementWidget->setWindowFlags(Qt::Window);
    addItemAdditionalRequirementWidget->show();
}

void addQuotation::on_additionalTableWidget_doubleClicked(const QModelIndex &index)
{
    return;
    int row = index.row();
    addItemAdditionalRequirementWidget = new addItemAdditionalRequirement();
    addItemAdditionalRequirementWidget->setWindowFlags(Qt::Window);
    addItemAdditionalRequirementWidget->edit(allLedgers[row]);
    addItemAdditionalRequirementWidget->show();
}

void addQuotation::on_voucherDateEdit_editingFinished()
{

}

void addQuotation::on_ReviseButton_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Revise Quotation","Revise and Save Quote?",QMessageBox::Yes|QMessageBox::No);
    if(reply == QMessageBox::Yes){
        on_buttonBox_accepted();
        oldQuoteHelper->insertQuotation(oldQuote);
    }

}
void addQuotation::reviseButtonClicked(bool)
{
    ui->revLabel->show();
    ui->revLineEdit->show();
    ui->ReviseButton->show();
    reviseButton->hide();
    ui->buttonBox->button(QDialogButtonBox::Save)->hide();
    exportButton->hide();
    clientApprovalButton->hide();
    managerApprovalButton->hide();
    int revNo = dbHelper->getRevisionNumber(ui->voucherNoLineEdit->text()).toInt();
    revisionNumber = revNo+1;
    ui->revLineEdit->setText(QString::number(revisionNumber));
    oldQuote = new GeneralVoucherDataObject;
    oldQuote->voucherType="Sales Quotation";
    oldQuote->InventoryItems = voucher->InventoryItems;
    oldQuote->ledgersList = ordersLedList;
    oldQuote->ProjectId = this->projectID;
//    oldQuote->VoucherPrefix="A";
    oldQuote->voucherNumber = ui->voucherNoLineEdit->text()+"."+QString::number(revNo);
    oldQuote->VoucherDate = ui->voucherDateEdit->date();
    oldQuote->grandTotal = ui->totalAmountLineEdit->text().toFloat();
    oldQuote->narration = ui->narrationTextEdit->toPlainText();
    oldQuote->RevisionNo=revNo;
    oldQuote->LPO = LPOString;
    oldQuote->prevTransVouchers = vNo;
    oldQuote->SalesmanID = SalesManId;
    oldQuote->AddedById = userID;
    oldQuote->Contact= CustomerContact;

}

void addQuotation::browseClicked(bool)
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Select a File for ",
                                                    QDir::homePath());
    Attachment->setText(fileName);
}

void addQuotation::discountAmountEntered()
{
    int n = sender()->property("row").toInt();

    QLineEdit* valueLine = qobject_cast<QLineEdit*>(ui->itemTableWidget->cellWidget(n,11));

    voucher->InventoryItems[n].BaseItem.discountPercentage = 0;
    voucher->InventoryItems[n].BaseItem.discountinAmount = valueLine->text().toFloat();

    setItemTable();
}

void addQuotation::discountPercentEntered()
{
    int n = sender()->property("row").toInt();

    QLineEdit* percLine = qobject_cast<QLineEdit*>(ui->itemTableWidget->cellWidget(n,10));

    voucher->InventoryItems[n].BaseItem.discountPercentage = percLine->text().toFloat();
    voucher->InventoryItems[n].BaseItem.discountinAmount = 0;

    setItemTable();
}

void addQuotation::downloadClicked(bool v)
{
    int row = sender()->property("row").toInt();
    qDebug()<<docObj[row].id;
    QByteArray ba = dbHelper->getFilewithId(docObj[row].id);
    qDebug()<<"image size "<<ba.size();
    QString path = QFileDialog::getExistingDirectory(this,"Select folder",QDir::currentPath());
    QFile file(path+"/"+docObj[row].filename);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out.writeRawData(ba,ba.size());
    //file.write(ba);
    file.close();
}


void addQuotation::on_discountAmountLineEdit_textChanged(const QString &arg1)
{

}

void addQuotation::on_discountPercentLineEdit_textChanged(const QString &arg1)
{


}

void addQuotation::on_itemTableWidget_clicked(const QModelIndex &index)
{
    //    QModelIndexList l= ui->itemTableWidget->selectionModel()->selectedRows();
    //    float total = 0;
    //    for(int i=0;i<l.count();i++){
    //        int r = l[i].row();
    //        total += ui->itemTableWidget->item(r,10)->text().toFloat();
    //    }
    //    qDebug()<<"Totals =" <<total;
}

void addQuotation::on_discountPercentLineEdit_editingFinished()
{
    ui->discountAmountLineEdit->blockSignals(true);

    float percent = ui->discountPercentLineEdit->text().toFloat();
    float  amount = (voucher->subTotal) * percent /100;
    for(int i=0;i<voucher->InventoryItems.size();i++){
        voucher->InventoryItems[i].BaseItem.discountPercentage =percent;
        voucher->InventoryItems[i].BaseItem.discountinAmount = 0;
    }
    ui->discountAmountLineEdit->setText(QString::number(amount,'f',2));

    setItemTable();

    ui->discountAmountLineEdit->blockSignals(false);
}

void addQuotation::on_discountAmountLineEdit_editingFinished()
{
    ui->discountPercentLineEdit->blockSignals(true);

    float discValue = ui->discountAmountLineEdit->text().toFloat();

    float percent = discValue/(voucher->subTotal ) *100;

    for(int i=0;i<voucher->InventoryItems.size();i++){
        voucher->InventoryItems[i].BaseItem.discountPercentage = percent;
        voucher->InventoryItems[i].BaseItem.discountinAmount = 0;
    }

    ui->discountPercentLineEdit->setText(QString::number(percent,'f',2));

    setItemTable();



    ui->discountPercentLineEdit->blockSignals(false);
}

void addQuotation::on_itemTableWidget_itemSelectionChanged()
{

    QModelIndexList l= ui->itemTableWidget->selectionModel()->selectedRows();
    qDebug()<<"Row = "<<l[0].row()<<voucher->InventoryItems.size();

    float total = 0;
    for(int i=0;i<l.count();i++){
        if(l[i].row()>voucher->InventoryItems.size()-1)
            return;

        if(l[i].row()<=voucher->InventoryItems.size()){
            total += ui->itemTableWidget->item(l[i].row(),12)->text().toFloat();
            qDebug() <<i<<voucher->InventoryItems[l[i].row()].BaseItem.subTotal;
        }
    }

    ui->SelectedItemTotalLabel->setText("Selected Total : "+QString::number(total,'f',2));
}


void addQuotation::setFilesTable()
{
    int i=0;
    ui->filesTable->setColumnCount(6);
    i=0;

    QTableWidgetItem* SLNO1 = new QTableWidgetItem;
    SLNO1->setText("#");
    ui->filesTable->setHorizontalHeaderItem(i++,SLNO1);


    QTableWidgetItem* fileName = new QTableWidgetItem;
    fileName->setText("File");
    ui->filesTable->setHorizontalHeaderItem(i++,fileName);

    QTableWidgetItem* filesDescription = new QTableWidgetItem;
    filesDescription->setText("File Description");
    ui->filesTable->setHorizontalHeaderItem(i++,filesDescription);

    QTableWidgetItem* status = new QTableWidgetItem;
    status->setText("Status");
    ui->filesTable->setHorizontalHeaderItem(i++,status);

    QTableWidgetItem* View = new QTableWidgetItem;
    View->setText("Download");
    ui->filesTable->setHorizontalHeaderItem(i++,View);

    QTableWidgetItem* deleteItem1 = new QTableWidgetItem;
    deleteItem1->setText("Delete");
    ui->filesTable->setHorizontalHeaderItem(i++,deleteItem1);

    ui->filesTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->filesTable->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
    ui->filesTable->horizontalHeader()->setSectionResizeMode(3,QHeaderView::ResizeToContents);
    ui->filesTable->horizontalHeader()->setSectionResizeMode(4,QHeaderView::ResizeToContents);

    ui->filesTable->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);



    i=0;
    ui->filesTable->setRowCount(0);
    //    docObj = dbHelper->getALLFilesWithReqID(invItem.BaseItem.requirementItemId);
    docObj = dbHelper->getALLFilesWithQuoteNo(vNo);
    for(DocumentsDataModel obj:docObj){
        ui->filesTable->insertRow(i);
        QTableWidgetItem* SLNO = new QTableWidgetItem;
        SLNO->setText(QString::number(i+1));
        ui->filesTable->setItem(i,0,SLNO);


        QTableWidgetItem* filename = new QTableWidgetItem;
        filename->setText(obj.filename);
        ui->filesTable->setItem(i,1,filename);

        QTableWidgetItem* filedesc = new QTableWidgetItem;
        filedesc->setText(obj.fileDescr);
        ui->filesTable->setItem(i,2,filedesc);


        QToolButton *upload = new QToolButton(this);
        upload->setIcon(QIcon(":/images/dowload.png"));
        upload->setProperty("row",i);
        upload->setAutoRaise(true);
        ui->filesTable->setCellWidget(i,4,upload);
        QObject::connect(upload,SIGNAL(clicked(bool)),this,SLOT(downloadClicked(bool)));

        QToolButton *del = new QToolButton(this);
        del->setIcon(QIcon(":/icons/trash.ico"));
        del->setProperty("row",i);
        del->setAutoRaise(true);
        ui->filesTable->setCellWidget(i,5,del);
        QObject::connect(del,SIGNAL(clicked(bool)),this,SLOT(delButtonClicked(bool)));

        i++;
    }

}


void addQuotation::on_browseButton_clicked()
{
    QString fileName =
            QFileDialog::getOpenFileName(this,
                                         "Select a File for ",
                                         QDir::homePath(),
                                         tr("Image Files (*.png *.jpg *.bmp *.pdf)"));

    ui->fileName->setText(fileName);

}

void addQuotation::on_UploadButton_clicked()
{
    QFile f(ui->fileName->text());
    if(vNo ==""){
        QMessageBox box;box.setText("Please Save Quote First");box.exec();
        return;
    }
    if(f.open(QIODevice::ReadOnly))
    {
        QFileInfo fileInfo(f.fileName());
        QString filename(fileInfo.fileName());

        DocumentsDataModel doc;
        doc.fileObj = f.readAll();
        f.close();
        doc.filename = filename;
        doc.fileType = fileInfo.completeSuffix();
        doc.fileDescr = ui->fileDescreption->text();
        doc.addedByID = LoginValues::getUserID();
        doc.ReqID = "";
        doc.projectID = projectID;
        doc.quoteNo = vNo;
        QMessageBox::StandardButton reply;
        reply=QMessageBox::question(this,"Upload File","Are You Sure you want to upload File?",QMessageBox::Yes|QMessageBox::No);
        if(reply==QMessageBox::Yes){
            doc.id= dbHelper->insertQImage(doc);
            docObj.append(doc);
            setFilesTable();
        }

    }
    else{
        QMessageBox box;
        box.setText("Cannot Open File!!!");
        box.exec();
    }
    ui->uploadWidget->hide();
}
