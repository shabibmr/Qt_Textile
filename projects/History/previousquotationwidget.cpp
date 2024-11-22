#include "previousquotationwidget.h"
#include "ui_previousquotationwidget.h"

PreviousQuotationWidget::PreviousQuotationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreviousQuotationWidget)
{
    ui->setupUi(this);
    ui->addItemPushButton->hide();
    ui->addAdditionalPushButton->hide();
    ui->voucherDateEdit->setReadOnly(true);

//    ui->ReviseButton->hide();

//    reviseButton = new QToolButton;
//    reviseButton->setText("Revise");
//    reviseButton->setIcon(QIcon(":/images/documentExport.ico"));
//    reviseButton->setIconSize(QSize(32,32));
//    reviseButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
//    reviseButton->setHidden(true);
//    reviseButton->setAutoRaise(true);
//    ui->extrasLayout->addWidget(reviseButton);
//    QObject::connect(reviseButton,SIGNAL(clicked(bool)),this,SLOT(reviseButtonClicked(bool)));


//    exportButton = new QToolButton;
//    exportButton->setText("Export To\n Sales Order");
//    exportButton->setIcon(QIcon(":/images/documentExport.ico"));
//    exportButton->setIconSize(QSize(32,32));
//    exportButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
//    exportButton->setHidden(true);
//    exportButton->setAutoRaise(true);
//    ui->extrasLayout->addWidget(exportButton);
//    QObject::connect(exportButton,SIGNAL(clicked(bool)),this,SLOT(exportToSalesOrder()));

    //    managerApprovalButton = new QToolButton;
    //    clientApprovalButton = new QToolButton;

    //    clientApprovalButton->setText("Client\n Approval");
    //    clientApprovalButton->setIcon(QIcon(":/images/documentExport.ico"));
    //    clientApprovalButton->setIconSize(QSize(32,32));
    //    clientApprovalButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    //    clientApprovalButton->setHidden(true);
    //    clientApprovalButton->setAutoRaise(true);
    //    ui->extrasLayout->addWidget(clientApprovalButton);

    //    managerApprovalButton ->setText("Manager\n Approval");
    //    managerApprovalButton ->setIcon(QIcon(":/images/documentExport.ico"));
    //    managerApprovalButton ->setIconSize(QSize(32,32));
    //    managerApprovalButton ->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    //    managerApprovalButton ->setHidden(true);
    //    managerApprovalButton ->setAutoRaise(true);
    //    ui->extrasLayout->addWidget(managerApprovalButton);

    //    QObject::connect(clientApprovalButton,SIGNAL(clicked(bool)),this,SLOT(clientApprovalButtonClicked()));

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

    ui->discountTotal->setEnabled(false);
    ui->subTotal->setEnabled(false);
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

    ui->totalAmountLineEdit->setEnabled(false);
    ui->totalAmountLineEdit->setStyleSheet("border:none;");

    itemsHelper = new SalesInventoryItemDatabaseHelper();
    dbHelper = new OldQuotationDatabaseHelper;

    ui->itemTableWidget->setColumnCount(10);
    int voucherNo = dbHelper->getMaxID() + 1;
    ui->voucherNoLineEdit->setText(QString::number(voucherNo));
    int i=0;

    QTableWidgetItem* SLNO = new QTableWidgetItem;
    SLNO->setText("#");
    ui->itemTableWidget->setHorizontalHeaderItem(i++,SLNO);

    QTableWidgetItem* ItemName = new QTableWidgetItem;
    ItemName->setText("Item Name & Description");
    ui->itemTableWidget->setHorizontalHeaderItem(i++,ItemName);

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

    QTableWidgetItem* subTotal = new QTableWidgetItem;
    subTotal->setText("Total");
    ui->itemTableWidget->setHorizontalHeaderItem(i++,subTotal);

    QTableWidgetItem* deleteItem = new QTableWidgetItem;
    deleteItem->setIcon(QIcon(":/icons/trash.ico"));
    ui->itemTableWidget->setHorizontalHeaderItem(i++,deleteItem);

    ui->itemTableWidget->setColumnWidth(0,25);
    ui->itemTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->itemTableWidget->setColumnWidth(9,25);

    ui->additionalTableWidget->setColumnCount(2);

    QTableWidgetItem* LedgerName = new QTableWidgetItem;
    LedgerName->setText("Ledger Name");
    ui->additionalTableWidget->setHorizontalHeaderItem(0,LedgerName);

    QTableWidgetItem* ledAmount = new QTableWidgetItem;
    ledAmount->setText("Amount");
    ui->additionalTableWidget->setHorizontalHeaderItem(1,ledAmount);
    ui->additionalTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    itemsHelper = new SalesInventoryItemDatabaseHelper();
    itemsListPtr = itemsHelper->getALLInventoryObjectsAsPtr();

    ledgerHelper = new LedgerMasterDatabaseHelper();
    allLedgersPtr = ledgerHelper->getAllLedgersAsPtr();

    addressBookHelper = new AddressBookDatabaseHelper();
    addresses = addressBookHelper->getAllContactsForSearchAsPtr();


    userHelper = new UserProfileDatabaseHelper();

    contactsWidget = new GMContactSearch(addresses);
    contactsWidget->setReadOnly(true);
    ui->salesmanCombo->setEnabled(false);
    ui->salesmanCombo->setStyleSheet("background-color:#ffffff;");
    QPointer<QWidget> SearchWid = new QWidget;
    QPointer<QVBoxLayout> lay = new QVBoxLayout();
    lay->setContentsMargins(0,0,0,0);
    lay->setSpacing(0);
    lay->addWidget(contactsWidget);
    SearchWid->setLayout(lay);

    ui->ledNameLay->addWidget(SearchWid);

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

    setItemTable();

}

PreviousQuotationWidget::~PreviousQuotationWidget()
{
    delete ui;
}


void PreviousQuotationWidget::keyPressEvent(QKeyEvent *e)
{
    if ((e->key() == Qt::Key_N)  && (e->modifiers().testFlag(Qt::ControlModifier))){
//        on_addItemPushButton_clicked();
    }
}


//void PreviousQuotationWidget::setCustomerName(LedgerMasterDataModel value)
//{
//    CustomerName = value;
//}

void PreviousQuotationWidget::setCustomerContact(AddressBookDataModel x)
{
    CustomerContact =x;
    if(x.ledgerId>0)
        CustomerName = ledgerHelper->getLedgerObjectByID(x.ledgerId);
}




//void PreviousQuotationWidget::saveVoucher(GeneralVoucherDataObject *obj)
//{
//    QuotationDetailsWidget = new QuotationDetails(this);
//    QuotationDetailsWidget ->setWindowFlags(Qt::Window);
//    QuotationDetailsWidget ->showMaximized();
//    QObject::connect(QuotationDetailsWidget,SIGNAL(quoteDetailsSaved(QString,QString,QString,QString,QString,QString,QString,bool,bool,bool)),
//                     this,SLOT(getQuoteDetails(QString,QString,QString,QString,QString,QString,QString,bool,bool,bool)));
//    if(vNo == "")
//        dbHelper->insertQuotation(obj);
//    else
//        dbHelper->updateQuotation(obj,vNo);

//    this->close();
//    emit closing();

//}

//void PreviousQuotationWidget::exportToSalesOrder()
//{
//    // = new GeneralVoucherDataObject();

//    if(!dbHelper->getClientApprovalStatus(vNo)){
//        QMessageBox box;
//        box.setText("Client Approval Pending!!!");
//        box.exec();
//        return;
//    }

//    objSalesOrder.voucherType="Sales Quotation";
//    objSalesOrder.InventoryItems = ordersList;
//    objSalesOrder.ledgersList = ordersLedList;
//    objSalesOrder.ProjectId = this->projectID;
//    objSalesOrder.VoucherPrefix="A";
//    objSalesOrder.voucherNumber = ui->voucherNoLineEdit->text();
//    objSalesOrder.VoucherDate = ui->voucherDateEdit->date();
//    objSalesOrder.grandTotal = ui->totalAmountLineEdit->text().toFloat();
//    objSalesOrder.narration = ui->narrationTextEdit->toPlainText();
//    objSalesOrder.LPO = LPOString;
//    objSalesOrder.prevTransVouchers = vNo;
//    objSalesOrder.SalesmanID = ui->salesmanCombo->itemData(ui->salesmanCombo->currentIndex()).toInt();
//    if(objSalesOrder.SalesmanID ==0){
//        objSalesOrder.SalesmanID =SalesManId;
//    }
//    else{
//        SalesManId=objSalesOrder.SalesmanID;
//    }
//    if(SalesManId==0){
//        QMessageBox box; box.setText("Select Salesman"); box.exec();
//        return;
//    }

//    if(CustomerName.LedgerName.length()>0){
//        openSalesOrder();
//    }
//    else{
//        LedgerMasterDataModel newLedger;
//        newLedger.LedgerName = CustomerContact.CompanyName;
//        AccountGroupMasterDatabaseHelper *accHelpr=new AccountGroupMasterDatabaseHelper();
//        newLedger.LedgerGroupId = accHelpr->getGroupIDByName("Sundry Debtors");
//        newLedger.LedgerGroupName = "Sundry Debtors";
//        newLedger.City = CustomerContact.city;
//        newLedger.Country = CustomerContact.country;
//        newLedger.ContactPersonName = CustomerContact.ContactName;
//        newLedger.ContactPersonNumber = CustomerContact.PhoneNumber;
//        newLedger.ContactPersonEmail = CustomerContact.email;
//        newLedger.PoBox = CustomerContact.POBox;
//        newLedger.narration = CustomerContact.notes;
//        newLedger.Address = CustomerContact.address + CustomerContact.Building+", "+CustomerContact.Street;
//        newLedger.fax = CustomerContact.Fax;

//        addLedgerWidget = new addLedger(this);
//        addLedgerWidget->setWindowFlags(Qt::Window);
//        addLedgerWidget->setTitle("Create Ledger");
//        addLedgerWidget->edit(newLedger);
//        addLedgerWidget->setContactID(CustomerContact._id);
//        addLedgerWidget->show();
//        QObject::connect(addLedgerWidget,SIGNAL(closing()),this,SLOT(openSalesOrder()));
//    }
//}

//void PreviousQuotationWidget::openSalesOrder()
//{
//    objSalesOrder.ledgerObject=ledgerHelper->getLedgerObjectByID(addressBookHelper->getLedgerIDFromContactID(CustomerContact._id));
//    AddSalesOrderWidget = new AddSalesOrder(this);
//    AddSalesOrderWidget->setWindowFlags(Qt::Window);

//    AddSalesOrderWidget->importToSalesOrder(objSalesOrder);
//    AddSalesOrderWidget->showMaximized();
//    this->close();
//}




void PreviousQuotationWidget::getQuoteDetails(QString validity,QString subject,QString payment,
                                              QString warranty,QString extra,QString delivery,QString exclusion,
                                              bool printExtra,bool printNow,bool emailNow)
{

    if(printNow==true){
        pHelper = new PrinterHelper();
        GeneralVoucherDataObject obj;

        obj.voucherType="Sales Quotation";
        obj.InventoryItems = ordersList;
        obj.ledgersList = ordersLedList;
        obj.ProjectId = this->projectID;
//        obj.VoucherPrefix="A";
        obj.voucherNumber = ui->voucherNoLineEdit->text();
        obj.VoucherDate = ui->voucherDateEdit->date();
        obj.grandTotal = ui->totalAmountLineEdit->text().toFloat();
        obj.subTotal= ui->totalAmountLineEdit->text().toFloat();
        obj.narration = ui->narrationTextEdit->toPlainText();

        obj.ledgerObject = CustomerName;
        obj.Contact = CustomerContact;
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
                                printExtra,printNow,obj);
    }
}

void PreviousQuotationWidget::printQuotation()
{
    QuotationDetailsWidget = new QuotationDetails(this);
    QuotationDetailsWidget ->setWindowFlags(Qt::Window);
    QuotationDetailsWidget ->showMaximized();
    QObject::connect(QuotationDetailsWidget,SIGNAL(quoteDetailsSaved(QString,QString,QString,QString,QString,QString,QString,bool,bool,bool)),
                     this,SLOT(getQuoteDetails(QString,QString,QString,QString,QString,QString,QString,bool,bool,bool)));

}

void PreviousQuotationWidget::printVoucher(QString validity,QString subject,QString payment,
                                           QString warranty,QString extra,QString delivery,QString exclusion,
                                           bool printExtra,bool printNow,bool emailNow)
{
    pHelper = new PrinterHelper();
    GeneralVoucherDataObject obj;

    obj.voucherType="Quotation";
    obj.InventoryItems = ordersList;
    obj.ledgersList = ordersLedList;
    obj.ProjectId = this->projectID;
//    obj.VoucherPrefix="A";
    obj.voucherNumber = ui->voucherNoLineEdit->text();
    obj.VoucherDate = ui->voucherDateEdit->date();
    obj.grandTotal = ui->totalAmountLineEdit->text().toFloat();
    obj.narration = ui->narrationTextEdit->toPlainText();
    if(obj.AddedBy.length()<1)
        obj.AddedBy = Username;
    obj.SalesmanID = ui->salesmanCombo->itemData(ui->salesmanCombo->currentIndex()).toInt();

    if(contactsWidget->text().length()>0){
        obj.ledgerObject = CustomerName;
    }

    pHelper->printQuotation(validity,subject,payment,warranty,extra,delivery,exclusion,
                            printExtra,printNow,obj);
}

//void PreviousQuotationWidget::reviseQuote(GeneralVoucherDataObject obj)
//{

//}

//void PreviousQuotationWidget::disableSaveButton(bool val)
//{
//    ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(!val);
//    if(LoginValues::model.reviseQuote){
//        reviseButton->show();
//    }
//}





//void PreviousQuotationWidget::on_buttonBox_accepted()
//{
//    GeneralVoucherDataObject *obj = new GeneralVoucherDataObject();

//    obj->voucherType="Quotation";
//    obj->VoucherDate  = ui->voucherDateEdit->date();
//    obj->InventoryItems = ordersList;
//    obj->ledgersList = ordersLedList;
//    obj->ProjectId = this->projectID;
//    obj->VoucherPrefix="A";
//    obj->voucherNumber = ui->voucherNoLineEdit->text();
//    obj->VoucherDate = ui->voucherDateEdit->date();
//    obj->grandTotal = ui->totalAmountLineEdit->text().toFloat();
//    obj->subTotal = ui->subTotal->text().toFloat();
//    obj->discount = ui->discountTotal->text().toFloat();
//    obj->narration = ui->narrationTextEdit->toPlainText();
//    obj->AddedBy = LoginValues::getUserName();
//    obj->AddedById = LoginValues::getUserID();qDebug()<<"Rev no : "<<revisionNumber;
//    obj->RevisionNo = QString::number(revisionNumber);
//    obj->LPO = LPOString;
//    obj->SalesmanID = ui->salesmanCombo->itemData(ui->salesmanCombo->currentIndex()).toInt();
//    if(obj->SalesmanID==0){
//        obj->SalesmanID=SalesManId   ;
//    }
//    else{
//        SalesManId=obj->SalesmanID;
//    }

//    if(SalesManId==0){
//        QMessageBox box;box.setText("Select Salesman");box.exec();
//        return;
//    }
//    if(contactsWidget->text().length()>0){

//        obj->ledgerObject = CustomerName;
//        obj->Contact = CustomerContact;
//    }
//    else{
//        QMessageBox box;box.setText("Select Customer");box.exec();
//        return;
//    }
//    QMessageBox bbox;


//    saveVoucher(obj);

//}

//void PreviousQuotationWidget::on_buttonBox_rejected()
//{
//    this->close();
//}

//void PreviousQuotationWidget::setTitle(QString Text)
//{
//    ui->titleLabel->setText(Text);
//}

void PreviousQuotationWidget::setItemTable()
{
    //    qDebug()<<"Set Item Table";
    int r = ordersList.size();
    ui->itemTableWidget->setRowCount(0);
    ui->itemTableWidget->setRowCount(r+1);
    int i=0;
    for(CompoundItemDataObject obj:ordersList){
        //        qDebug()<<"####### loop count = "<<i;
        QTableWidgetItem* SLNO = new QTableWidgetItem;
        SLNO->setText(QString::number(i+1));
        SLNO->setFlags(SLNO->flags() ^ Qt::ItemIsEditable);
        ui->itemTableWidget->setItem(i,0,SLNO);

        //        qDebug()<<QDateTime::currentDateTime().toString();
        QPointer<GMItemSearch> itemLine = new GMItemSearch(itemsListPtr);
        itemLine->setText(obj.BaseItem.ItemName);
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
        //        qDebug()<<QDateTime::currentDateTime().toString();
        QTableWidgetItem* uom = new QTableWidgetItem;
        uom->setFlags(uom->flags() ^ Qt::ItemIsEditable);
        uom->setText(obj.BaseItem.uomObject.UomName);
        ui->itemTableWidget->setItem(i,2,uom);

        QTableWidgetItem* ModelNumber = new QTableWidgetItem;
        ModelNumber->setFlags(ModelNumber->flags() ^ Qt::ItemIsEditable);
        ModelNumber->setText(obj.BaseItem.ItemCode);
        ui->itemTableWidget->setItem(i,3,ModelNumber);

        QTableWidgetItem* brandName = new QTableWidgetItem;
        brandName->setFlags(brandName->flags() ^ Qt::ItemIsEditable);
        brandName->setText(obj.BaseItem.brandName);
        ui->itemTableWidget->setItem(i,4,brandName);

        QTableWidgetItem* serial = new QTableWidgetItem;
        serial->setFlags(serial->flags() ^ Qt::ItemIsEditable);
        serial->setText(obj.BaseItem.SerailNumber);
        ui->itemTableWidget->setItem(i,5,serial);

        QTableWidgetItem* quantity = new QTableWidgetItem;
        quantity->setFlags(quantity->flags() ^ Qt::ItemIsEditable);
        quantity->setText(QString::number(obj.BaseItem.quantity,'f',2));
        ui->itemTableWidget->setItem(i,6,quantity);

        QTableWidgetItem* price = new QTableWidgetItem;
        price->setFlags(price->flags() ^ Qt::ItemIsEditable);
        price->setText(QString::number(obj.BaseItem.price,'f',2));
        ui->itemTableWidget->setItem(i,7,price);

        QTableWidgetItem* amount = new QTableWidgetItem;
        amount->setFlags(amount->flags() ^ Qt::ItemIsEditable);
        amount->setText(QString::number((obj.BaseItem.subTotal+obj.subItemsTotal),'f',2));
        ui->itemTableWidget->setItem(i,8,amount);

        QToolButton* delButton = new QToolButton;
        delButton->setIcon(QIcon(":/icons/trash.ico"));
        delButton->setProperty("row",i);
        delButton->setAutoRaise(true);
        //deleteItem->setIcon(QIcon(":/icons/trash.ico"));
        ui->itemTableWidget->setCellWidget(i,9,delButton);
        QObject::connect(delButton,SIGNAL(clicked(bool)),this,SLOT(delButtonClicked(bool)));
        i++;
    }
    //    qDebug()<<"Creating Empty row at "<<i;
    //    qDebug()<<"start"<<QDateTime::currentDateTime().toString();
//    GMItemSearch *itemLine = new GMItemSearch(itemsListPtr);
//    //    qDebug()<<"End"<<QDateTime::currentDateTime().toString();

//    itemLine->setProperty("row",i);

//    QWidget *SearchWid = new QWidget;
//    SearchWid->setProperty("row",i);
//    QVBoxLayout *lay = new QVBoxLayout();
//    lay->setContentsMargins(0,0,0,0);
//    lay->setSpacing(0);
//    lay->addWidget(itemLine);
//    SearchWid->setLayout(lay);
//    ui->itemTableWidget->setCellWidget(i,1,SearchWid);

//    QObject::connect(itemLine,SIGNAL(SelectedItemID(inventoryItemDataModel)),
//                     this,SLOT(getItemDetails(inventoryItemDataModel)));
//    QPointer<QLineEdit> uomEmpty = new QLineEdit;
//    uomEmpty->setEnabled(false);
//    uomEmpty->setStyleSheet("QLineEdit { border: none }");
//    ui->itemTableWidget->setCellWidget(i,2,uomEmpty);
//    qDebug()<<"set up uom done";

//    //QObject::connect(ItemNameCombo,SIGNAL(activated(int)),this,SLOT(getItemDetails(int)));
//    setLedgerTable();
//    itemLine->setFocus();

//    //   calcLedgerTotal();
    calcTotal();
}



void PreviousQuotationWidget::getItemDetails(inventoryItemDataModel obj)
{

    //    qDebug()<<openNewWidget;
    if(openNewWidget){
        //        qDebug()<<"inside";
        openNewWidget = false;

        addItemRequirementWidget = new addItemRequirement(this);
        addItemRequirementWidget->setWindowFlags(Qt::Window);
        int currRow =  ui->itemTableWidget->currentRow();

        if (currRow == -1) {
            if (QWidget* focused = ui->itemTableWidget->focusWidget()) {
                currRow = focused->property("row").toInt();
            }
        }
        addItemRequirementWidget->setItemsListPtr(itemsListPtr);
        addItemRequirementWidget->setListNumber(currRow);
        addItemRequirementWidget->getFirstItemDetail(obj);
        addItemRequirementWidget->showMaximized();
        QObject::connect(addItemRequirementWidget,SIGNAL(addNewItem(CompoundItemDataObject,int)),
                         this,SLOT(appendItem(CompoundItemDataObject,int)));
        QObject::connect(addItemRequirementWidget,SIGNAL(closing(int)),
                         this,SLOT(resetFlag(int)));

    }
}

void PreviousQuotationWidget::appendItem(CompoundItemDataObject item, int row)
{
    return;
    openNewWidget = true;
    //    qDebug()<<"edited row sub tot= "<<item.subItemsTotal;

    //    qDebug()<<"row count = "<<ui->itemTableWidget->rowCount();
    if(ui->itemTableWidget->rowCount()==row+1){
        ordersList.append(item);
        //        qDebug()<<"item appended";
    }
    else{
        ordersList[row]=item;
        //        qDebug()<<"item Inserted at "<<row;
    }
    setItemTable();
    calcTotal();
}

void PreviousQuotationWidget::setProjectID(QString projID)
{
    this->projectID = projID;
}

//void PreviousQuotationWidget::delButtonClicked(bool)
//{
//    int x= sender()->property("row").toInt();

//    ui->itemTableWidget->removeRow(x);
//    ordersList.removeAt(x);

//    setItemTable();
//}

void PreviousQuotationWidget::calcTotal()
{
    qDebug()<<"At Calc Total";
    float total = 0;
    float discount =0;

    for(CompoundItemDataObject obj:ordersList){
        total += obj.BaseItem.subTotal +obj.subItemsTotal;
        discount  += obj.BaseItem.discount;
    }
    //    qDebug()<<"DIscount ="<<discount;
    for(LedgerMasterDataModel obj:ordersLedList){
        total+= obj.amount;
    }

    ui->subTotal->setText(QString::number(total+discount,'f',2));
    ui->discountTotal->setText(QString::number(discount,'f',2));
    ui->totalAmountLineEdit->setText(QString::number(total,'f',2));
    qDebug()<<"Calc Completed";
}

void PreviousQuotationWidget::showVoucher(GeneralVoucherDataObject voucher)
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

    GeneralVoucherDataObject obj = dbHelper->getVoucherByVoucherNo(voucher.voucherNumber);
    qDebug()<<"Company Name = "<<CustomerContact.CompanyName;
    contactsWidget->setText(CustomerContact.CompanyName);

    if(CustomerContact.ledgerId.length()>0){
        CustomerName = ledgerHelper->getLedgerObjectByID(CustomerContact.ledgerId);
        //        qDebug()<<"At Show Voucher"<<CustomerName.LedgerName;
    }
    ordersList = obj.InventoryItems;
    ordersLedList = obj.ledgersList;
//    exportButton->setHidden(false);
//    clientApprovalButton->setHidden(false);
//    managerApprovalButton->setHidden(false);
    printButton->setHidden(false);
    setItemTable();

//    if(LoginValues::model.Quotes_Update ==false || dbHelper->getClientApprovalStatus(vNo)){
//        disableSaveButton(true);
//    }
    qDebug()<<"Show Voucher Completed";
}

//void PreviousQuotationWidget::importToQuotation(GeneralVoucherDataObject voucher)
//{
////    contactsWidget->setText(voucher.ledgerObject.LedgerName);
////    CustomerName = voucher.ledgerObject;
////    ui->narrationTextEdit->setPlainText(voucher.narration);

////    ui->salesmanCombo->setCurrentText(salesmanMap.value(voucher.SalesmanID));
////    SalesManId = voucher.SalesmanID;
////    CustomerContact=voucher.Contact;
////    contactsWidget->setText(CustomerContact.CompanyName);

////    if(CustomerContact.ledgerId.length()>0){
////        CustomerName = ledgerHelper->getLedgerObjectByID(CustomerContact.ledgerId);
////        //        qDebug()<<"At Show Voucher"<<CustomerName.LedgerName;
////    }

////    //    GeneralVoucherDataObject obj = dbHelper->getVoucherByVoucherNo(voucher.voucherNumber);
////    ordersList = voucher.InventoryItems;
////    ordersLedList = voucher.ledgersList;
////    setItemTable();
//}

//void PreviousQuotationWidget::importMultipleVouchers(QList<GeneralVoucherDataObject> vouchers)
//{
//    for(GeneralVoucherDataObject voucher:vouchers)
//    {
//        contactsWidget->setText(voucher.ledgerObject.LedgerName);
//        CustomerName = voucher.ledgerObject;
//        GeneralVoucherDataObject obj = dbHelper->getVoucherByVoucherNo(voucher.voucherNumber);
//        ordersList.append(obj.InventoryItems);
//        ordersLedList.append(obj.ledgersList);
//    }
//    setItemTable();
//}

void PreviousQuotationWidget::getLedgerDetails(LedgerMasterDataModel led)
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

void PreviousQuotationWidget::setLedgerTable()
{
    int ledTableSize  = ordersList.size() + ordersLedList.size();
    ui->additionalTableWidget->setRowCount(0);
    ui->additionalTableWidget->setRowCount(ledTableSize);


    int rCnt = 0;

    for(CompoundItemDataObject item:ordersList){
        QPointer<GMLedgerSearch> ledSearch = new GMLedgerSearch(allLedgersPtr);
        QPointer<QWidget> SearchWid = new QWidget;
        QPointer<QVBoxLayout> lay = new QVBoxLayout();
        lay->setContentsMargins(0,0,0,0);
        lay->setSpacing(0);
        lay->addWidget(ledSearch);
        SearchWid->setLayout(lay);
        ledSearch->setEnabled(false);
        ledSearch->setText(ledgerHelper->getLedgerNameByID(item.BaseItem.defaultSalesLedgerID));
        qDebug()<<"Default sales item Name = "<<item.BaseItem.ItemName;
        qDebug()<<"Default sales Ledger = "<<item.BaseItem.defaultSalesLedgerID;
        qDebug()<<"Ledger Name = "<<ledgerHelper->getLedgerNameByID(item.BaseItem.defaultSalesLedgerID);
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
        qDebug()<<"454          LEDGER AMOUNT  "<<ledger.amount;
        amountEdit->setText(QString::number(ledger.amount,'f',2));
        ui->additionalTableWidget->setCellWidget(rCnt,1,amountEdit);
        amountEdit->setProperty("row",rCnt);
        QObject::connect(amountEdit,SIGNAL(editingFinished()),this,SLOT(updateLedgerValue()));

        rCnt++;
        qDebug()<<rCnt<< "Done ************************************";
    }

//    ui->additionalTableWidget->insertRow(ledTableSize);

//    QPointer<GMLedgerSearch> ledSearch = new GMLedgerSearch(allLedgersPtr);
//    QPointer<QWidget> SearchWid = new QWidget;
//    QPointer<QVBoxLayout> lay = new QVBoxLayout();
//    lay->setContentsMargins(0,0,0,0);
//    lay->setSpacing(0);
//    lay->addWidget(ledSearch);
//    SearchWid->setLayout(lay);
//    ledSearch->setProperty("row",ledTableSize);
//    SearchWid->setProperty("row",ledTableSize);
//    ui->additionalTableWidget->setCellWidget(ledTableSize,0,SearchWid);

//    //QObject::connect(ledCombo,SIGNAL(activated(int)),this,SLOT(getLedgerDetails(LedgerMasterDataModel led)));
//    QObject::connect(ledSearch,SIGNAL(SelectedLedger(LedgerMasterDataModel)),
//                     this,SLOT(getLedgerDetails(LedgerMasterDataModel)));
//    QLineEdit *amountEdit = new QLineEdit;
//    amountEdit ->setValidator(new QDoubleValidator(0.0002,999999,2,this));
//    amountEdit->setFrame(QFrame::NoFrame);
//    amountEdit->setProperty("row",ledTableSize);
//    ledSearch->setFocus();
//    ledSearch->selectAll();
//    QObject::connect(amountEdit,SIGNAL(editingFinished()),this,SLOT(updateLedgerValue()));
//    ui->additionalTableWidget->setCellWidget(ledTableSize,1,amountEdit);
    calcTotal();
}

void PreviousQuotationWidget::updateLedgerValue()
{
    int row = sender()->property("row").toInt();
    qDebug()<<"row = "<<row;
    qDebug()<<"Ledger size = "<<ordersLedList.size();
    qDebug()<<"item name = "<<ordersLedList[row-ordersList.size()].LedgerName;
    qDebug()<<"444"<<"Amount entered ro number : "<<row;
    if(row<=ordersLedList.size()){
        qDebug()<<"num ledgers"<<ordersLedList.size();
        ordersLedList[row-ordersList.size()].amount=qobject_cast<QLineEdit*>(ui->additionalTableWidget->cellWidget(row,1))
                ->text().toFloat();
    }
    setLedgerTable();
    return;
}

//void PreviousQuotationWidget::deleteLedgerButtonClicked(bool x)
//{
//    int rCount= sender()->property("row").toInt();

//    int invItemsCount = ordersList.size();
//    int xtraLedgersCount = ui->additionalTableWidget->rowCount() - invItemsCount;

//    if(rCount>invItemsCount-1){
//        ui->additionalTableWidget->removeRow(rCount);
//        ordersLedList.removeAt(rCount-invItemsCount);
//    }

//    setItemTable();
//}

void PreviousQuotationWidget::appendLedger(LedgerMasterDataModel item, int row)
{
    return;
    openNewWidgetLedger = true;
    qDebug()<<"524 row ="<<row;
    QLineEdit* line = qobject_cast<QLineEdit*> (ui->additionalTableWidget->cellWidget(row,1));

    item.amount = line->text().toFloat();
    qDebug()<<"edited row sub tot= "<<item.amount;

    qDebug()<<"524 row count = "<<ui->additionalTableWidget->rowCount();
    if(ordersLedList.size()+ordersList.size() == row ){
        ordersLedList.append(item);
        qDebug()<<"item appended";
    }
    else{
        ordersLedList[row - ordersList.size()]=item;
        qDebug()<<"item Inserted at "<<row;
    }
    //  setItemTable();
    calcTotal();
}

void PreviousQuotationWidget::resetLedgerWidgetFlag()
{

}

//void PreviousQuotationWidget::clientApprovalButtonClicked()
//{
//    approvalDialog = new QDialog(this);
//    approvalDialog->setWindowTitle("Change Approval Status");
//    // approvalDialog->setFixedSize(300,150);

//    QVBoxLayout *vLayout =new QVBoxLayout(approvalDialog);

//    QHBoxLayout *boxlay = new QHBoxLayout(approvalDialog);
//    QHBoxLayout *boxlayLPO = new QHBoxLayout(approvalDialog);
//    QHBoxLayout *boxlayTentDate = new QHBoxLayout(approvalDialog);
//    QHBoxLayout *boxlayokCancel = new QHBoxLayout(approvalDialog);
//    QFormLayout *flay = new QFormLayout(approvalDialog);



//    vLayout->addLayout(flay);
//    vLayout->addLayout(boxlayokCancel);

//    QLabel *label = new QLabel(approvalDialog);
//    label->setText("Client Approved : ");
//    checkbox = new QCheckBox(approvalDialog);
//    qDebug()<<"checkbox is "<<dbHelper->getClientApprovalStatus(vNo);
//    checkbox->setChecked(dbHelper->getClientApprovalStatus(vNo));

//    LPOref = new QLineEdit(approvalDialog);
//    tentDelivery = new QDateEdit(approvalDialog);
//    tentDelivery ->setCalendarPopup(true);
//    QLabel* lpoLabel = new QLabel(approvalDialog);
//    QLabel* tentDate = new QLabel(approvalDialog);

//    lpoLabel->setText("LPO Ref : ");
//    tentDate->setText("Tentative Delivery : ");
//    tentDelivery->setDate(QDate::currentDate());

//    QPushButton *okButton  = new QPushButton(approvalDialog);
//    QPushButton *cancelButton  = new QPushButton(approvalDialog);
//    okButton->setText("OK");
//    cancelButton->setText("Cancel");

//    flay->addRow(label,checkbox);
//    flay->addRow(lpoLabel,LPOref);
//    flay->addRow(tentDate,tentDelivery);
//    boxlayokCancel->addWidget(okButton);
//    boxlayokCancel->addWidget(cancelButton);
//    //    okButton->setGeometry(QRect(100,100,80,35));
//    //    cancelButton->setGeometry(QRect(200,100,80,35));
//    QObject::connect(cancelButton,SIGNAL(clicked(bool)),approvalDialog,SLOT(close()));
//    QObject::connect(okButton,SIGNAL(clicked(bool)),this,SLOT(changeClientApprovalStatus(bool)));

//    approvalDialog->show();
//}

//void PreviousQuotationWidget::changeClientApprovalStatus(bool)
//{
//    if(LPOref->text().length()<1 && checkbox->isChecked()){
//        QMessageBox box;
//        box.setText("Enter LPO Reference Number");
//        box.exec();
//    }
//    else{
//        approvalDialog->close();
//        dbHelper->setClientApproval(vNo,checkbox->isChecked(),LPOref->text(),tentDelivery->date());
//        LPOString=LPOref->text();

//    }

//}

void PreviousQuotationWidget::on_itemTableWidget_doubleClicked(const QModelIndex &index)
{
    qDebug()<<"table double cliked";
    int r= index.row();
    if(openNewWidget && ordersList.size()>r && r>=0){
        qDebug()<<"inside";
        openNewWidget = false;
        addItemRequirementWidget = new addItemRequirement(this);
        addItemRequirementWidget->setWindowFlags(Qt::Window);
        addItemRequirementWidget->setItemsListPtr(itemsListPtr);
        addItemRequirementWidget->edit(ordersList[r]);
        addItemRequirementWidget->setListNumber(r);
        addItemRequirementWidget->showMaximized();
        QObject::connect(addItemRequirementWidget,SIGNAL(addNewItem(CompoundItemDataObject,int)),
                         this,SLOT(appendItem(CompoundItemDataObject,int)));
        QObject::connect(addItemRequirementWidget,SIGNAL(closing(int)),
                         this,SLOT(resetFlag(int)));

    }
    //QObject::connect(addItemRequirementWidget,SIGNAL(closing())
}

void PreviousQuotationWidget::resetFlag(int r)
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

void PreviousQuotationWidget::on_addAdditionalPushButton_clicked()
{
    addItemAdditionalRequirementWidget = new addItemAdditionalRequirement();
    addItemAdditionalRequirementWidget->setWindowFlags(Qt::Window);
    addItemAdditionalRequirementWidget->show();
}

void PreviousQuotationWidget::on_additionalTableWidget_doubleClicked(const QModelIndex &index)
{
    return;
    int row = index.row();
    addItemAdditionalRequirementWidget = new addItemAdditionalRequirement();
    addItemAdditionalRequirementWidget->setWindowFlags(Qt::Window);
    addItemAdditionalRequirementWidget->edit(allLedgers[row]);
    addItemAdditionalRequirementWidget->show();
}

void PreviousQuotationWidget::on_voucherDateEdit_editingFinished()
{

}

//void PreviousQuotationWidget::on_ReviseButton_clicked()
//{
//    QMessageBox::StandardButton reply;
//    reply = QMessageBox::question(this,"Revise Quotation","Revise and Save Quote?",QMessageBox::Yes|QMessageBox::No);
//    if(reply == QMessageBox::Yes){
//        on_buttonBox_accepted();
//        oldQuoteHelper->insertQuotation(oldQuote);
//    }

//}
//void PreviousQuotationWidget::reviseButtonClicked(bool)
//{
//    ui->revLabel->show();
//    ui->revLineEdit->show();
//    ui->ReviseButton->show();
//    reviseButton->hide();
//    ui->buttonBox->button(QDialogButtonBox::Save)->hide();
//    exportButton->hide();
//    clientApprovalButton->hide();
//    managerApprovalButton->hide();
//    int revNo = dbHelper->getRevisionNumber(ui->voucherNoLineEdit->text()).toInt();
//    revisionNumber = revNo+1;
//    ui->revLineEdit->setText(QString::number(revisionNumber));
//    oldQuote = new GeneralVoucherDataObject;
//    oldQuote->voucherType="Sales Quotation";
//    oldQuote->InventoryItems = ordersList;
//    oldQuote->ledgersList = ordersLedList;
//    oldQuote->ProjectId = this->projectID;
//    oldQuote->VoucherPrefix="A";
//    oldQuote->voucherNumber = ui->voucherNoLineEdit->text()+"."+QString::number(revNo);
//    oldQuote->VoucherDate = ui->voucherDateEdit->date();
//    oldQuote->grandTotal = ui->totalAmountLineEdit->text().toFloat();
//    oldQuote->narration = ui->narrationTextEdit->toPlainText();
//    oldQuote->RevisionNo=QString::number(revNo);
//    oldQuote->LPO = LPOString;
//    oldQuote->prevTransVouchers = vNo;
//    oldQuote->SalesmanID = SalesManId;
//    oldQuote->AddedById = userID;

//}

