#include "addmaterialinout.h"
#include "ui_addmaterialinout.h"
#include "customwidgets/gmsearchlineedit.h"
#include "customwidgets/gmledgersearch.h"
#include <QCompleter>
#include <QStandardItem>
#include <QTableView>
#include <QListWidget>

AddMaterialInOut::AddMaterialInOut(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddMaterialInOut)
{
    ui->setupUi(this);
    ui->voucherDateEdit->setDate(QDate::currentDate());
    this->setWindowTitle("Material In/Out Details");

    ui->voucherNoLineEdit->setEnabled(false);
    ui->voucherNoLineEdit->setStyleSheet("border:none;");
    ui->voucherDateEdit->setDate(QDate::currentDate());
    itemsHelper = new SalesInventoryItemDatabaseHelper();
    dbHelper = new materialinoutDatabaseHelper;
    ui->itemTableWidget->setColumnCount(10);

    voucher = new GeneralVoucherDataObject();
//    vHelper = new GeneralVoucherDatabaseHelper;
    QString voucherNo = dbHelper->getNextVoucherNo(LoginValues::voucherPrefix);
    ui->voucherNoLineEdit->setText(voucherNo);

//    int voucherNo = dbHelper->getMaxID() + 1;
//    ui->voucherNoLineEdit->setText(QString::number(voucherNo));
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

    ledgerHelper = new LedgerMasterDatabaseHelper();
    allLedgers = ledgerHelper->getAllLedgers();
    modelLedger = new QStandardItemModel(allLedgers.size(),2,this);

    itemsHelper = new SalesInventoryItemDatabaseHelper();
//    itemsList = itemsHelper->getALLInventoryObjects();
//    itemsListPtr = itemsHelper->getALLInventoryObjectsAsPtr();

    for(inventoryItemDataModel item:itemsList){

        NamesForCompleter.append( item.ItemName + "\nBrand : "+ item.brandName+"\tModel Number : "+item.ItemCode);
        NamesForLineEdit.append(item.ItemName);
    }

    QStandardItemModel* model = new QStandardItemModel();

    // initialize the model
    int rows = itemsList.size();  // assuming this is a QStringList
    model->setRowCount(rows);
    model->setColumnCount(1);

    // load the items
    int row = 0;
    for(inventoryItemDataModel itemObj:itemsList) {
        QStandardItem* item = new QStandardItem(itemObj.ItemName);
        item->setToolTip("Brand : "+itemObj.brandName+"\nModel No:"+itemObj.brandName);
        model->setItem(row, 0, item);
        row++;
    }

    //Additional ledgers
//    ui->additionalTableWidget->setColumnCount(2);
//    setItemTable();
}

AddMaterialInOut::~AddMaterialInOut()
{
    delete ui;
}

void AddMaterialInOut::on_buttonBox_rejected()
{
    this->close();
}

void AddMaterialInOut::on_addItemPushButton_clicked()
{
    addItemRequirementWidget = new addItemRequirement(this);
    addItemRequirementWidget->setWindowFlags(Qt::Window);
    addItemRequirementWidget->show();
}

void AddMaterialInOut::keyPressEvent(QKeyEvent *e)
{
    if ((e->key() == Qt::Key_N)  && (e->modifiers().testFlag(Qt::ControlModifier))){
        on_addItemPushButton_clicked();
    }
}

void AddMaterialInOut::on_buttonBox_accepted()
{
    GeneralVoucherDataObject *obj = new GeneralVoucherDataObject();

    obj->VoucherDate  = ui->voucherDateEdit->date();
//    qDebug()<<"INdex = "<<ui->customerNameComboBox->currentIndex();
//    obj->ledgerObject = allLedgers[ui->customerNameComboBox->currentIndex()];
    obj->InventoryItems = ordersList;
//    obj->ProjectId = this->projectID;
//    obj->VoucherPrefix="A";
    obj->voucherNumber = ui->voucherNoLineEdit->text();
    obj->VoucherDate = ui->voucherDateEdit->date();
//    obj->grandTotal = ui->totalAmountLineEdit->text().toFloat();
    if(vNo == "")
    	dbHelper->insertMaterialInOut(obj);
    else
        dbHelper->updateMaterialInOut(obj,vNo);
    delete obj;
    this->close();
    emit closing();
}

void AddMaterialInOut::setTitle(QString Text)
{
    ui->titleLabel->setText(Text);
}

void AddMaterialInOut::setItemTable()
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

        qDebug()<<"Setting combo";
        QComboBox *ItemNameCombo = new QComboBox;
        ItemNameCombo->setEditable(true);
        ItemNameCombo->setProperty("row",i);
        GMSearchLineEdit *searchLine = new GMSearchLineEdit;
        searchLine->setData(NamesForLineEdit);
        ItemNameCombo->addItem("");
        ItemNameCombo->addItems(NamesForLineEdit);

        ItemNameCombo->setLineEdit(searchLine);
        qDebug()<<"Creating completor for"<<obj.BaseItem.ItemName;
        QCompleter *comp = new QCompleter(NamesForCompleter,this);

        comp->setFilterMode(Qt::MatchContains);
        comp->setCaseSensitivity(Qt::CaseInsensitive);
        comp->setCompletionMode(QCompleter::PopupCompletion);
        ItemNameCombo->setCurrentText(obj.BaseItem.ItemName);
        ItemNameCombo->setCompleter(comp);
        ui->itemTableWidget->setCellWidget(i,1,ItemNameCombo);

        QObject::connect(ItemNameCombo,SIGNAL(activated(int)),this,SLOT(getItemDetails(int)));

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
    qDebug()<<"Creating Empty row at "<<i;
    QComboBox *ItemNameCombo = new QComboBox;
    ItemNameCombo->setEditable(true);
    ItemNameCombo->setProperty("row",i);
    GMSearchLineEdit *searchLine = new GMSearchLineEdit;
    searchLine->setData(NamesForLineEdit);
    ItemNameCombo->addItem("");
    ItemNameCombo->addItems(NamesForLineEdit);

    ItemNameCombo->setLineEdit(searchLine);
    qDebug()<<"Creating completor";
    QCompleter *comp = new QCompleter(NamesForCompleter,this);

    comp->setFilterMode(Qt::MatchContains);
    comp->setCaseSensitivity(Qt::CaseInsensitive);
    comp->setCompletionMode(QCompleter::PopupCompletion);

    ItemNameCombo->setCompleter(comp);
    ui->itemTableWidget->setCellWidget(i,1,ItemNameCombo);

    QPointer<QLineEdit> uomEmpty = new QLineEdit;
    uomEmpty->setEnabled(false);
    uomEmpty->setStyleSheet("QLineEdit { border: none }");
    ui->itemTableWidget->setCellWidget(i,2,uomEmpty);
    qDebug()<<"set up uom done";
    ItemNameCombo->setFocus();

    QObject::connect(ItemNameCombo,SIGNAL(activated(int)),this,SLOT(getItemDetails(int)));
    setLedgerTable();
    calcLedgerTotal();
    calcTotal();
}

void AddMaterialInOut::getItemDetails(inventoryItemDataModel obj)
{
    qDebug()<<openNewWidget;
    if(openNewWidget ){
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

void AddMaterialInOut::appendItem(CompoundItemDataObject item, int row)
{
    openNewWidget = true;
    qDebug()<<"edited row sub tot= "<<item.subItemsTotal;

    qDebug()<<"row count = "<<ui->itemTableWidget->rowCount();
    if(ui->itemTableWidget->rowCount()==row+1){
        ordersList.append(item);
        qDebug()<<"item appended";
    }
    else{
        ordersList[row]=item;
        qDebug()<<"item Inserted at "<<row;
    }
    setItemTable();
    calcTotal();
}

void AddMaterialInOut::setProjectID(QString projID)
{
    this->projectID = projID;
}

void AddMaterialInOut::delButtonClicked(bool)
{
    int x= sender()->property("row").toInt();

    ui->itemTableWidget->removeRow(x);
    ordersList.removeAt(x);

    setItemTable();
}

void AddMaterialInOut::calcTotal()
{
    float total = 0;

    for(CompoundItemDataObject obj:ordersList){
        total += obj.BaseItem.subTotal +obj.subItemsTotal;
    }
    for(LedgerMasterDataModel obj:ordersLedList){
        total+= obj.amount;
    }
//    ui->totalAmountLineEdit->setText(QString::number(total,'f',2));
}

void AddMaterialInOut::showVoucher(GeneralVoucherDataObject voucher)
{
    ui->voucherNoLineEdit->setText(voucher.voucherNumber);
    ui->voucherDateEdit->setDate(voucher.VoucherDate);
    vNo = voucher.voucherNumber;
    vPrefix =voucher.VoucherPrefix;
qDebug()<<"line no"<<"386";
    GeneralVoucherDataObject obj = dbHelper->getVoucherByVoucherNo(voucher.voucherNumber);

    ordersList = obj.InventoryItems;
    ordersLedList = obj.ledgersList;
    setItemTable();
}

void AddMaterialInOut::getLedgerDetails(int i)
{

    return;
//    addItemAdditionalRequirementWidget = new addItemAdditionalRequirement();
//    addItemAdditionalRequirementWidget->setWindowFlags(Qt::Window);
//    addItemAdditionalRequirementWidget->edit(allLedgers[i]);
//    addItemAdditionalRequirementWidget->show();

}

void AddMaterialInOut::setLedgerTable()
{
    int ledTableSize  = ordersList.size() + ordersLedList.size();
//    ui->additionalTableWidget->setRowCount(0);
//    ui->additionalTableWidget->setRowCount(ledTableSize);


    int rCnt = 0;

    for(CompoundItemDataObject item:ordersList){
        GMSearchLineEdit *sline = new GMSearchLineEdit;
        sline->setData(LedNames);
        sline->setText(ledgerHelper->getLedgerNameByID(item.BaseItem.defaultSalesLedgerID));
        sline->setEnabled(false);
//        ui->additionalTableWidget->setCellWidget(rCnt,0,sline);

        QLineEdit *amountEdit = new QLineEdit;
        amountEdit ->setValidator(new QDoubleValidator(0.0002,999999,2,this));
        amountEdit->setFrame(QFrame::NoFrame);
        amountEdit->setText(QString::number(item.BaseItem.subTotal,'f',2));
        amountEdit->setEnabled(false);
//        ui->additionalTableWidget->setCellWidget(rCnt,0,amountEdit);

        rCnt++;
    }

    for(LedgerMasterDataModel ledger:ordersLedList){

        QComboBox *ledCombo = new QComboBox;
        GMSearchLineEdit *sline = new GMSearchLineEdit;
        sline->setData(LedNames);
        ledCombo->setLineEdit(sline);

//        QCompleter *ledCompleter = new QCompleter(this);
//        ledCompleter->setFilterMode(Qt::MatchContains);
//        ledCompleter->setCaseSensitivity(Qt::CaseInsensitive);
//        ledCompleter->setCompletionMode(QCompleter::PopupCompletion);
//        ledCompleter->setModel(modelLedger);
//        ledCompleter->setPopup(ledCompView);

//        ledCombo->setCompleter(ledCompleter);
//        ledCombo->setCurrentText(ledger.LedgerName);
//        ui->additionalTableWidget->setCellWidget(rCnt,0,ledCombo);

        QLineEdit *amountEdit = new QLineEdit;
        amountEdit ->setValidator(new QDoubleValidator(0.0002,999999,2,this));
        amountEdit->setFrame(QFrame::NoFrame);
        amountEdit->setText(QString::number(ledger.amount,'f',2));
//        ui->additionalTableWidget->setCellWidget(rCnt,1,amountEdit);
        rCnt++;
    }
//    ui->additionalTableWidget->insertRow(ledTableSize);



    QLineEdit *amountEdit = new QLineEdit;
    amountEdit ->setValidator(new QDoubleValidator(0.0002,999999,2,this));
    amountEdit->setFrame(QFrame::NoFrame);

//    ui->additionalTableWidget->setCellWidget(ledTableSize,1,amountEdit);
}

void AddMaterialInOut::deleteLedgerButtonClicked(bool x)
{
    int rCount= sender()->property("row").toInt();
    int invItemsCount = ordersList.size();
//    int xtraLedgersCount = ui->additionalTableWidget->rowCount() - invItemsCount;

    if(rCount>invItemsCount-1){
//        ui->additionalTableWidget->removeRow(rCount);
//        ordersLedList.removeAt(rCount-invItemsCount);
    }

    setItemTable();
}

void AddMaterialInOut::appendLedger(LedgerMasterDataModel item, int row)
{
    openNewWidgetLedger = true;
    qDebug()<<"edited row sub tot= "<<item.amount;

//    qDebug()<<"row count = "<<ui->additionalTableWidget->rowCount();
//    if(ui->additionalTableWidget->rowCount()==row+1){
        ordersLedList.append(item);
        qDebug()<<"item appended";
//    }
//    else{
//        ordersLedList[row]=item;
//        qDebug()<<"item Inserted at "<<row;
//    }
    setItemTable();
    calcTotal();
}

void AddMaterialInOut::calcLedgerTotal()
{

}

void AddMaterialInOut::resetLedgerWidgetFlag()
{

}


void AddMaterialInOut::on_itemTableWidget_doubleClicked(const QModelIndex &index)
{
    qDebug()<<"table double cliked";
    int r= index.row();
    if(openNewWidget && ordersList.size()>r && r>=0){
        qDebug()<<"inside";
        openNewWidget = false;
        addItemRequirementWidget = new addItemRequirement(this);
        addItemRequirementWidget->setWindowFlags(Qt::Window);
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

void AddMaterialInOut::resetFlag(int r)
{
    qDebug()<<"vaue of r :"<<r;
    if(ui->itemTableWidget->rowCount()==r+1){
        try{
            QLineEdit* uom = qobject_cast<QLineEdit*>( ui->itemTableWidget->cellWidget(r,2));
            if(uom->text().length()<1){
//                qDebug()<<"waa";
//                QComboBox* combo= qobject_cast<QComboBox*> (ui->itemTableWidget->cellWidget(r,1));
//                combo->setCurrentText("");
            }
        }
        catch(...){

        }
    }
    openNewWidget = true;
}

//void AddMaterialInOut::on_addAdditionalPushButton_clicked()
//{
//    addItemAdditionalRequirementWidget = new addItemAdditionalRequirement();
//    addItemAdditionalRequirementWidget->setWindowFlags(Qt::Window);
//    addItemAdditionalRequirementWidget->show();
//}

//void AddMaterialInOut::on_additionalTableWidget_doubleClicked(const QModelIndex &index)
//{
//    return;
//    int row = index.row();
//    addItemAdditionalRequirementWidget = new addItemAdditionalRequirement();
//    addItemAdditionalRequirementWidget->setWindowFlags(Qt::Window);
//    addItemAdditionalRequirementWidget->edit(allLedgers[row]);
//    addItemAdditionalRequirementWidget->show();
//}
