#include "requirement.h"
#include "ui_requirement.h"

#include "customwidgets/gmitemsearch.h"
#include "customwidgets/gmledgersearch.h"
//#include <QListWidget>


Requirement::Requirement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Requirement)
{
    ui->setupUi(this);

    ui->addItemPushButton->hide();
    ui->addAdditionalPushButton->hide();

    exportButton = new QToolButton;
    exportButton->setText("Export To Quote");
    exportButton->setIcon(QIcon(":/images/documentExport.ico"));
    exportButton->setIconSize(QSize(32,32));
    exportButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    exportButton->setHidden(true);
    ui->extrasLayout->addWidget(exportButton);

    ui->tabWidget->setCurrentIndex(0);
    this->setWindowTitle("Requirement Details");

    ui->voucherNoLineEdit->setEnabled(false);
    ui->voucherNoLineEdit->setStyleSheet("border:none;");
    ui->voucherDateEdit->setDate(QDate::currentDate());

    ui->totalAmountLineEdit->setEnabled(false);
    ui->totalAmountLineEdit->setStyleSheet("border:none;");
    ui->discountTotal->setEnabled(false);
    ui->subTotal->setEnabled(false);
    ui->discountTotal->setStyleSheet("background-color:#ffffff;color:#000000;");
    ui->subTotal->setStyleSheet("background-color:#ffffff;color:#000000;");
    ui->totalAmountLineEdit->setStyleSheet("background-color:#ffffff;color:#000000;");

    ui->userName->setText(LoginValues::getUserName());
    Username = LoginValues::getUserName();

    itemsHelper = new SalesInventoryItemDatabaseHelper();
    dbHelper = new RequirementsDatabaseHelper;
    ui->itemTableWidget->setColumnCount(10);
//    int voucherNo = dbHelper->getMaxID() + 1;
//    ui->voucherNoLineEdit->setText(QString::number(voucherNo));


    QString voucherNo = dbHelper->getNextVoucherNo(LoginValues::voucherPrefix);
    ui->voucherNoLineEdit->setText(voucherNo);


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

    customerName = new GMLedgerSearch(allLedgersPtr);
//    QPointer<QWidget> SearchWid = new QWidget;
//    QPointer<QVBoxLayout> lay = new QVBoxLayout();
//    lay->setContentsMargins(0,0,0,0);
//    lay->setSpacing(0);
//    lay->addWidget(customerName);
//    SearchWid->setLayout(lay);

//    ui->ledNameLay->addWidget(SearchWid);


    addressBookHelper = new AddressBookDatabaseHelper();
    addresses = addressBookHelper->getAllContactsForSearchAsPtr();


    userHelper = new UserProfileDatabaseHelper();

    contactsWidget = new GMContactSearch(addresses);
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

    QObject::connect(customerName,SIGNAL(SelectedLedger(LedgerMasterDataModel)),
                     this,SLOT(setCustomerName(LedgerMasterDataModel)));
    setItemTable();

}

Requirement::~Requirement()
{
    delete ui;
}


void Requirement::keyPressEvent(QKeyEvent *e)
{
    if ((e->key() == Qt::Key_N)  && (e->modifiers().testFlag(Qt::ControlModifier))){
        on_addItemPushButton_clicked();
    }
}

void Requirement::on_addItemPushButton_clicked()
{
    addItemRequirementWidget = new addItemRequirement(this);
    addItemRequirementWidget->setWindowFlags(Qt::Window);
    addItemRequirementWidget->showMaximized();
}

void Requirement::setCustomerName(LedgerMasterDataModel value)
{
    CustomerName = value;
}

void Requirement::saveVoucher(GeneralVoucherDataObject *obj)
{
    if(vNo == "")
        dbHelper->insertRequirement(obj);
    else
        dbHelper->updateRequirement(obj,vNo);

}

void Requirement::exportToQuotation()
{
    qDebug()<<"Export Clicked Items List size = "<<ordersList.size();
    GeneralVoucherDataObject obj;// = new GeneralVoucherDataObject();

    obj.InventoryItems = ordersList;
    obj.ledgersList = ordersLedList;
    obj.ProjectId = this->projectID;
//    obj.VoucherPrefix="A";
    obj.voucherNumber = ui->voucherNoLineEdit->text();
    obj.VoucherDate = ui->voucherDateEdit->date();
    obj.grandTotal = ui->totalAmountLineEdit->text().toFloat();
    obj.narration = ui->narrationTextEdit->toPlainText();
    obj.Contact=CustomerContact;
    obj.SalesmanID=SalesManId;

    if(customerName->text().length()>0){
        obj.ledgerObject = CustomerName;
    }

    addQuotationWidget = new addQuotation(this);
    addQuotationWidget->setWindowFlags(Qt::Window);

    addQuotationWidget->importToQuotation(obj);
    addQuotationWidget->showMaximized();
    this->close();

}


void Requirement::  on_buttonBox_accepted()
{
    GeneralVoucherDataObject *obj = new GeneralVoucherDataObject();

    obj->VoucherDate  = ui->voucherDateEdit->date();
    obj->InventoryItems = ordersList;
    obj->ledgersList = ordersLedList;
    obj->ProjectId = this->projectID;
//    obj->VoucherPrefix="A";
    obj->voucherNumber = ui->voucherNoLineEdit->text();
    obj->VoucherDate = ui->voucherDateEdit->date();
    obj->grandTotal = ui->totalAmountLineEdit->text().toFloat();
    obj->narration = ui->narrationTextEdit->toPlainText();
    obj->AddedBy = LoginValues::getUserName();
    obj->AddedById = LoginValues::getUserID();
    obj->SalesmanID = ui->salesmanCombo->itemData(ui->salesmanCombo->currentIndex()).toInt();
    if(obj->SalesmanID==0){
        obj->SalesmanID=SalesManId   ;
    }
    else{
        SalesManId=obj->SalesmanID;
    }

    if(SalesManId==0){
        QMessageBox box;box.setText("Select Customer");box.exec();
        return;
    }
    if(contactsWidget->text().length()>0){

        obj->ledgerObject = CustomerName;
        obj->Contact = CustomerContact;
    }
    else{
        QMessageBox box;box.setText("Select Customer");box.exec();
        return;
    }



    saveVoucher(obj);
    this->close();
    emit closing();
}

void Requirement::on_buttonBox_rejected()
{
    this->close();
}

void Requirement::setTitle(QString Text)
{
    ui->titleLabel->setText(Text);
}

void Requirement::setItemTable()
{
    int r = ordersList.size();
    ui->itemTableWidget->setRowCount(0);
    ui->itemTableWidget->setRowCount(r+1);
    int i=0;
    for(CompoundItemDataObject obj:ordersList){

        QTableWidgetItem* SLNO = new QTableWidgetItem;
        SLNO->setText(QString::number(i+1));
        SLNO->setFlags(SLNO->flags() ^ Qt::ItemIsEditable);
        ui->itemTableWidget->setItem(i,0,SLNO);

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

        QTableWidgetItem* deleteItem = new QTableWidgetItem;
        QToolButton* delButton = new QToolButton;
        delButton->setIcon(QIcon(":/icons/trash.ico"));
        delButton->setProperty("row",i);
        delButton->setAutoRaise(true);
        //deleteItem->setIcon(QIcon(":/icons/trash.ico"));
        ui->itemTableWidget->setCellWidget(i,9,delButton);
        QObject::connect(delButton,SIGNAL(clicked(bool)),this,SLOT(delButtonClicked(bool)));
        i++;
    }

    QPointer<GMItemSearch> itemLine = new GMItemSearch(itemsListPtr);
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


    //QObject::connect(ItemNameCombo,SIGNAL(activated(int)),this,SLOT(getItemDetails(int)));
    setLedgerTable();
    itemLine->setFocus();

 //   calcLedgerTotal();
    calcTotal();
}



void Requirement::getItemDetails(inventoryItemDataModel obj)
{


    if(openNewWidget){
        qDebug()<<"inside";
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
        addItemRequirementWidget->show();
        QObject::connect(addItemRequirementWidget,SIGNAL(addNewItem(CompoundItemDataObject,int)),
                         this,SLOT(appendItem(CompoundItemDataObject,int)));
        QObject::connect(addItemRequirementWidget,SIGNAL(closing(int)),
                         this,SLOT(resetFlag(int)));

    }
}

void Requirement::appendItem(CompoundItemDataObject item, int row)
{
    openNewWidget = true;

    if(ui->itemTableWidget->rowCount()==row+1){
        ordersList.append(item);

    }
    else{
        ordersList[row]=item;

    }
    setItemTable();
    calcTotal();
}

void Requirement::setProjectID(QString projID)
{
    this->projectID = projID;
}

void Requirement::delButtonClicked(bool)
{
    int x= sender()->property("row").toInt();

    ui->itemTableWidget->removeRow(x);
    ordersList.removeAt(x);

    setItemTable();
}

void Requirement::calcTotal()
{
    float total = 0;
    float discount =0;

    for(CompoundItemDataObject obj:ordersList){
        total += obj.BaseItem.subTotal +obj.subItemsTotal;
        discount  += obj.BaseItem.discount;
    }

    for(LedgerMasterDataModel obj:ordersLedList){
        total+= obj.amount;
    }

    ui->subTotal->setText(QString::number(total-discount,'f',2));
    ui->discountTotal->setText(QString::number(discount,'f',2));
    ui->totalAmountLineEdit->setText(QString::number(total,'f',2));

}

void Requirement::showVoucher(GeneralVoucherDataObject voucher)
{
    ui->voucherNoLineEdit->setText(voucher.voucherNumber);
    customerName->setText(voucher.ledgerObject.LedgerName);
    CustomerName=ledgerHelper->getLedgerObjectByID( voucher.ledgerObject.LedgerID);
    ui->narrationTextEdit->setPlainText(voucher.narration);

    ui->voucherDateEdit->setDate(voucher.VoucherDate);
    vNo = voucher.voucherNumber;
    vPrefix =voucher.VoucherPrefix;
    ui->salesmanCombo->setCurrentText(salesmanMap.value(voucher.SalesmanID));
    Username =  userHelper->getEmployeeNameFromID(voucher.AddedById);
    ui->userName->setText(Username);
    vNo = voucher.voucherNumber;
    vPrefix =voucher.VoucherPrefix;
    SalesManId = voucher.SalesmanID;
    CustomerContact=voucher.Contact;

    GeneralVoucherDataObject obj = dbHelper->getVoucherByVoucherNo(voucher.voucherNumber);

    contactsWidget->setText(CustomerContact.CompanyName);
    if(CustomerContact.ledgerId.length()>0){
        CustomerName = ledgerHelper->getLedgerObjectByID(CustomerContact.ledgerId);
//             qDebug()<<"At Show Voucher"<<CustomerName.LedgerName;
    }
    ordersList = obj.InventoryItems;
    ordersLedList = obj.ledgersList;

    exportButton->setHidden(false);
    QObject::connect(exportButton,SIGNAL(clicked(bool)),this,SLOT(exportToQuotation()));
    setItemTable();
}

void Requirement::importToRequirement(GeneralVoucherDataObject voucher)
{
    ui->voucherNoLineEdit->setText(voucher.voucherNumber);
    customerName->setText(voucher.ledgerObject.LedgerName);

    ui->voucherDateEdit->setDate(voucher.VoucherDate);
    GeneralVoucherDataObject obj = dbHelper->getVoucherByVoucherNo(voucher.voucherNumber);
    ordersList = obj.InventoryItems;
    setItemTable();
}

void Requirement::getLedgerDetails(LedgerMasterDataModel led)
{


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

void Requirement::setLedgerTable()
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

        ledSearch->setProperty("row",rCnt);
        SearchWid->setProperty("row",rCnt);
        ui->additionalTableWidget->setCellWidget(rCnt,0,SearchWid);


        QLineEdit *amountEdit = new QLineEdit;
        amountEdit ->setValidator(new QDoubleValidator(0.0002,999999,2,this));
        amountEdit->setFrame(QFrame::NoFrame);
        amountEdit->setText(QString::number(item.BaseItem.grandTotal,'f',2));
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

        amountEdit->setText(QString::number(ledger.amount,'f',2));
        ui->additionalTableWidget->setCellWidget(rCnt,1,amountEdit);
        amountEdit->setProperty("row",rCnt);
        QObject::connect(amountEdit,SIGNAL(editingFinished()),this,SLOT(updateLedgerValue()));

        rCnt++;

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

void Requirement::updateLedgerValue()
{
    int row = sender()->property("row").toInt();

    if(row<=ordersLedList.size()){
        qDebug()<<"num ledgers"<<ordersLedList.size();
        ordersLedList[row-ordersList.size()].amount=qobject_cast<QLineEdit*>(ui->additionalTableWidget->cellWidget(row,1))
                ->text().toFloat();
    }
    setLedgerTable();
    return;
}

void Requirement::deleteLedgerButtonClicked(bool x)
{
    int rCount= sender()->property("row").toInt();

    int invItemsCount = ordersList.size();
    int xtraLedgersCount = ui->additionalTableWidget->rowCount() - invItemsCount;

    if(rCount>invItemsCount-1){
        ui->additionalTableWidget->removeRow(rCount);
        ordersLedList.removeAt(rCount-invItemsCount);
    }

    setItemTable();
}

void Requirement::appendLedger(LedgerMasterDataModel item, int row)
{
    openNewWidgetLedger = true;

    QLineEdit* line = qobject_cast<QLineEdit*> (ui->additionalTableWidget->cellWidget(row,1));

    item.amount = line->text().toFloat();

    if(ordersLedList.size()+ordersList.size() == row ){
        ordersLedList.append(item);

    }
    else{
        ordersLedList[row - ordersList.size()]=item;

    }
    //  setItemTable();
    calcTotal();
}

void Requirement::resetLedgerWidgetFlag()
{

}

void Requirement::on_itemTableWidget_doubleClicked(const QModelIndex &index)
{

    int r= index.row();
    if(openNewWidget && ordersList.size()>r && r>=0){

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

void Requirement::resetFlag(int r)
{

    if(ui->itemTableWidget->rowCount()==r+1){

        QLineEdit* uom = qobject_cast<QLineEdit*>( ui->itemTableWidget->cellWidget(r,2));
        if(uom->text().length()<1){

            //            GMItemSearch* combo= qobject_cast<GMItemSearch*> (ui->itemTableWidget->cellWidget(r,1));
            //            qDebug()<<combo->text();
            //            combo->setText("");
        }

    }
    openNewWidget = true;
}

void Requirement::on_addAdditionalPushButton_clicked()
{
    addItemAdditionalRequirementWidget = new addItemAdditionalRequirement();
    addItemAdditionalRequirementWidget->setWindowFlags(Qt::Window);
    addItemAdditionalRequirementWidget->show();
}

void Requirement::on_additionalTableWidget_doubleClicked(const QModelIndex &index)
{
    return;
    int row = index.row();
    addItemAdditionalRequirementWidget = new addItemAdditionalRequirement();
    addItemAdditionalRequirementWidget->setWindowFlags(Qt::Window);
    addItemAdditionalRequirementWidget->edit(allLedgers[row]);
    addItemAdditionalRequirementWidget->show();
}

void Requirement::setCustomerContact(AddressBookDataModel x)
{
    CustomerContact =x;
    if(x.ledgerId>0)
        CustomerName = ledgerHelper->getLedgerObjectByID(x.ledgerId);
}
