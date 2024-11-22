#include "additemrequirement.h"
#include "ui_additemrequirement.h"

addItemRequirement::addItemRequirement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addItemRequirement)
{
    ui->setupUi(this);
    this->setWindowTitle("Add Item Details");

    ui->brandLineEdit->setReadOnly(true);
    ui->modelNoLineEdit->setReadOnly(true);
    ui->itemTotalLineEdit->setReadOnly(true);
    ui->itemSubTotalLineEdit->setReadOnly(true);
    ui->subItemsSubTotalLineEdit_2->setReadOnly(true);

    ui->optionalCheckBox->hide();



    ui->label_2->hide();
    ui->label_3->hide();
    ui->label_8->hide();
    ui->rowCount->hide();
    ui->brandLineEdit->hide();
    ui->modelNoLineEdit->hide();
    ui->priceLevelComboBox->hide();
    ui->priceLevelLabel->hide();

    ui->subItemsTotalLineEdit->hide();
    ui->label_5->hide();
    ui->subItemsSubTotalLineEdit_2->hide();

    ui->rowCount->setRange(1,1);

    ui->tabWidget->removeTab(2);
    ui->tabWidget->removeTab(1);
    ui->tabWidget->removeTab(1);

    ui->itemTotalLineEdit->setStyleSheet("background-color:#ffffff;color:black;");
    ui->subItemsSubTotalLineEdit_2->setStyleSheet("background-color:#ffffff;color:black;");
    ui->itemSubTotalLineEdit->setStyleSheet("background-color:#ffffff;color:black;");

    propHelper = new PropertyDatabaseHelper();
    priceListHelper = new PricelistDatabaseHelper();
    invHelper = new InventoryHelper();
    uomHelper = new UomDataBaseHelper();
    UomConvList=uomHelper->getUomByItem(baseItem.ItemID);

    ui->quantityLineEdit->setValidator(new QDoubleValidator(0.0002,999999,2,this));
    ui->priceLineEdit->setValidator(new QDoubleValidator(0.0002,999999,2,this));
    ui->discountLineEdit->setValidator(new QDoubleValidator(0.0002,999999,2,this));
    ui->itemTotalLineEdit->setValidator(new QDoubleValidator(0.0002,999999,2,this));
    ui->lengthLineEdit->setValidator(new QDoubleValidator(0.0002,999999,2,this));
    ui->vatPercent->setValidator(new QDoubleValidator(0.0002,999999,2,this));
    ui->vatAmt->setValidator(new QDoubleValidator(0.0002,999999,2,this));
    ui->dimLineEdit->setEnabled(false);
    ui->lengthLineEdit->setText("1000");
    ui->quantityLineEdit->setText("1");

    ui->lengthLabel->setHidden(true);
    ui->lengthLineEdit->setHidden(true);
    ui->dimlabel->setHidden(true);
    ui->dimLineEdit->setHidden(true);

    ui->subItemsSubTotalLineEdit_2->setEnabled(false);
    ui->dimLineEdit->setEnabled(true);
    ui->subItemsTotalLineEdit->setEnabled(false);


    //    itemsList = itemsHelper->getALLInventoryObjects();

    //    for(inventoryItemDataModel item:itemsList){

    //        NamesForCompleter.append( item.ItemName + "\nBrand : "+ item.brandName+"\tModel Number : "+item.ItemCode);
    //        NamesForLineEdit.append(item.ItemName);
    //    }

//    int i=0;
//    ui->subItemsTable->setColumnCount(10);

//    QTableWidgetItem* SLNO = new QTableWidgetItem;
//    SLNO->setText("#");
//    ui->subItemsTable->setHorizontalHeaderItem(i++,SLNO);

//    QTableWidgetItem* ItemName = new QTableWidgetItem;
//    ItemName->setText("Item Name & Description");
//    ui->subItemsTable->setHorizontalHeaderItem(i++,ItemName);

//    QTableWidgetItem* uom = new QTableWidgetItem;
//    uom->setText("Units");
//    ui->subItemsTable->setHorizontalHeaderItem(i++,uom);

//    QTableWidgetItem* ModelNumber = new QTableWidgetItem;
//    ModelNumber->setText("Model Number");
//    ui->subItemsTable->setHorizontalHeaderItem(i++,ModelNumber);

//    QTableWidgetItem* brandName = new QTableWidgetItem;
//    brandName->setText("Brand");
//    ui->subItemsTable->setHorizontalHeaderItem(i++,brandName);


//    QTableWidgetItem* serial = new QTableWidgetItem;
//    serial->setText("Serial No.");
//    ui->subItemsTable->setHorizontalHeaderItem(i++,serial);

//    QTableWidgetItem* quantity = new QTableWidgetItem;
//    quantity->setText("Qty");
//    ui->subItemsTable->setHorizontalHeaderItem(i++,quantity);

//    QTableWidgetItem* price = new QTableWidgetItem;
//    price->setText("Price");
//    ui->subItemsTable->setHorizontalHeaderItem(i++,price);

//    QTableWidgetItem* subTotal = new QTableWidgetItem;
//    subTotal->setText("Total");
//    ui->subItemsTable->setHorizontalHeaderItem(i++,subTotal);


//    QTableWidgetItem* deleteItem = new QTableWidgetItem;
//    deleteItem->setIcon(QIcon(":/icons/trash.ico"));
//    ui->subItemsTable->setHorizontalHeaderItem(i++,deleteItem);

//    ui->subItemsTable->setColumnWidth(0,25);
//    ui->subItemsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
//    ui->subItemsTable->setColumnWidth(9,25);


//    setSubItemTable();
    ui->tabWidget->setCurrentIndex(0);
    QObject::connect(ui->unitOfMeasurementComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(uomIndexChanged(int)));
}

addItemRequirement::~addItemRequirement()
{
    delete ui;
}

void addItemRequirement::setMaxQuantity(float value)
{
    maxQuantity = value + ui->quantityLineEdit->text().toFloat();
    qDebug()<<"maxQuantity = "<<maxQuantity;
    ui->quantityLineEdit->setValidator(new MyValidator(0,maxQuantity,2,this));
}

qint64 addItemRequirement::getRequirementID() const
{
    return RequirementID;
}

void addItemRequirement::setRequirementID(qint64 value )
{
    RequirementID = value;
}

void addItemRequirement::setItemsListPtr(const QList<QSharedPointer<inventoryItemDataModel> > value)
{
    qDebug()<<"STEP1";

}

void addItemRequirement::on_buttonBox_rejected()
{
    this->close();
    emit closing(itemListNumber);
}

void addItemRequirement::on_buttonBox_accepted()
{
    CompoundItemDataObject compItem;
    baseItem.quantity = ui->quantityLineEdit->text().toFloat();
    baseItem.price = ui->priceLineEdit->text().toFloat();
    UomConversionDataModel uomObj = ui->unitOfMeasurementComboBox->currentData().value<UomConversionDataModel>();

    convalue = uomObj.conValue;
    uomId = uomObj.ToUnit;

    qDebug()<<"con value:"<<convalue;
    qDebug()<<" current quantity :"<<baseItem.quantity;
    baseItem.quantity *=convalue;
    qDebug()<<" updated quantity :"<<baseItem.quantity;
    baseItem.price /=convalue;


    baseItem.uomObject.UomName = uomId;
    qDebug()<<"UOM*************************"<<uomId;
    baseItem.uomObject.convRate = convalue;

    baseItem.length = ui->lengthLineEdit->text().toFloat();
    baseItem.discountPercentage = ui->discountLineEdit->text().toFloat();
    baseItem.ItemDescription = ui->descriptionTextEdit->toPlainText();
    baseItem.narration = ui->narrationEdit->toPlainText();
    baseItem.Dimension = ui->dimLineEdit->text();

    int lNumber = ui->rowCount->value()-1;

    baseItem.subTotal = ui->itemTotalLineEdit->text().toFloat() - ui->subItemsSubTotalLineEdit_2->text().toFloat() ;
    float subTotal = ui->itemSubTotalLineEdit->text().toFloat();

    baseItem.subTotal = subTotal;
    //    qDebug()<<"Sub Total"<<subTotal;
    //    qDebug()<<"baseItem.discountPercentage "<<baseItem.discountPercentage ;

    baseItem.discount = subTotal*baseItem.discountPercentage / 100;
    baseItem.discountinAmount = baseItem.discount;

    baseItem.grandTotal = ui->itemTotalLineEdit->text().toFloat();

    if(subOrdersList.size()>0){
        baseItem.IsCompoundItem =true;
    }
    else{
        baseItem.IsCompoundItem =false;
    }
    compItem.BaseItem = baseItem;
    qDebug()<<"ITEM quantity = "<<compItem.BaseItem.quantity;
    compItem.subItems = subOrdersList;
    compItem.subItemsTotal = ui->subItemsSubTotalLineEdit_2->text().toFloat();

    this->close();
    if(lNumber == itemListNumber){
        qDebug()<<"Emit new";
        emit addNewItem(compItem,itemListNumber);
    }
    else{
        qDebug()<<"emit insert";
        emit insertNewItem(compItem,lNumber);
    }


}

void addItemRequirement::setTitle(QString Text)
{
    ui->titleLabel->setText(Text);
}

void addItemRequirement::hideSubItemsTab(bool val)
{
    if(val){
        ui->tabWidget->removeTab(2);
    }
}

void addItemRequirement::setItem(CompoundItemDataObject obj)
{
    Item =obj;
    ui->titleLabel->setText(obj.BaseItem.ItemName);
//    ui->unitOfMeasurementComboBox->setCurrentText(obj.BaseItem.UOMName);
    UomConvList=uomHelper->getUomByItem(obj.BaseItem.ItemID);

    for(UomConversionDataModel a:UomConvList){
        ui->unitOfMeasurementComboBox->addItem(uomHelper->getUOMNameById(a.ToUnit), QVariant::fromValue(a));
    }

    UomConversionDataModel uomObj = ui->unitOfMeasurementComboBox->currentData().value<UomConversionDataModel>();

    convalue = uomObj.conValue;
    uomId = uomObj.ToUnit;

}

void addItemRequirement::setProperty(QString ItemId)
{

}

void addItemRequirement::edit(CompoundItemDataObject obj)
{
    qDebug()<<"Edit purchase order";
    baseItem = obj.BaseItem;
    ui->rowCount->hide();
    //    qDebug()<<"ItemEDit"<<obj.BaseItem.ItemName;
    ui->titleLabel->setText(baseItem.ItemName);
//    ui->quantityLineEdit->setText(QString::number(baseItem.quantity,'f',2));
    ui->discountLineEdit ->setText( QString::number(baseItem.discountPercentage,'f',2));
    ui->descriptionTextEdit->setPlainText(baseItem.ItemDescription);
    ui->modelNoLineEdit->setText(baseItem.ItemCode);
    ui->brandLineEdit->setText(baseItem.brandName);
//    ui->unitOfMeasurementComboBox->addItem(baseItem.UOMName);
    UomConvList=uomHelper->getUomByItem(obj.BaseItem.ItemID);
    qDebug()<<"UOM conversion list size for item :"<<UomConvList.size();
    for(UomConversionDataModel a:UomConvList){
        qDebug()<<"To Unit:"<<a.ToUnit;
        ui->unitOfMeasurementComboBox->addItem(uomHelper->getUOMNameById(a.ToUnit), QVariant::fromValue(a));
    }

    int index = ui->unitOfMeasurementComboBox->findText(uomHelper->getUOMNameById(baseItem.uomObject.UomName));
    ui->unitOfMeasurementComboBox->setCurrentIndex(index);

    UomConversionDataModel uomObj = ui->unitOfMeasurementComboBox->currentData().value<UomConversionDataModel>();
    baseItem.ClosingStock = iv.getClosingStockofItemByDate(baseItem.ItemID,QDate::currentDate());
    ui->stockLineEdit->setText(QString::number(baseItem.ClosingStock,'f',2));
    convalue = uomObj.conValue;
    uomId = uomObj.ToUnit;
    qDebug()<<"#######################Set UOM ID"<<uomId;
    ui->quantityLineEdit->setText(QString::number(baseItem.quantity/convalue,'f',2));

    qDebug()<<"SET UOM details in purchase";
    ui->priceLineEdit->setText(QString::number(baseItem.price*convalue,'f',2));
    ui->lengthLineEdit->setText(QString::number(baseItem.length,'f',2));
    ui->dimLineEdit->setText(baseItem.Dimension);
    ui->narrationEdit->setPlainText(baseItem.narration);


    ui->vatPercent->setText(QString::number(baseItem.taxRate));
    ui->vatAmt->setText(QString::number(baseItem.taxAmount));

    bool showDimFlag = (!baseItem.isCustomItem ) && (baseItem.Dimension.length()>0);


    qDebug()<<"Length = "<<baseItem.Dimension.length() << "is "<<baseItem.Dimension;

    ui->lengthLabel->setHidden(!(baseItem.isCustomItem && baseItem.Dimension.length()>0));
    ui->lengthLineEdit->setHidden(!(baseItem.isCustomItem&&baseItem.Dimension.length()>0));
    ui->dimlabel->setHidden(!(baseItem.isCustomItem && baseItem.Dimension.length()>0));
    ui->dimLineEdit->setHidden(!(baseItem.isCustomItem && baseItem.Dimension.length()>0));

    ui->descriptionTextEdit->setReadOnly(!baseItem.isCustomItem);


    subOrdersList = obj.subItems;
    calcTotal();
    if(baseItem.price!=0 && baseItem.subTotal==0){
        ui->optionalCheckBox->setChecked(true);
        on_optionalCheckBox_stateChanged(0);
    }
    ui->quantityLineEdit->setFocus();




    //    qDebug()<<"set edit Completed ";
}

void addItemRequirement::getFirstItemDetail(inventoryItemDataModel obj)
{
    baseItem = obj;
    //    qDebug()<<"ItemEDit"<<baseItem.ItemName;
    ui->titleLabel->setText(baseItem.ItemName);
    //    qDebug()<<"Is Custom Item "<<obj.isCustomItem;

    ui->lengthLabel->setHidden(!(obj.isCustomItem && baseItem.Dimension.length()>0));
    ui->lengthLineEdit->setHidden(!(obj.isCustomItem && baseItem.Dimension.length()>0));
    ui->dimlabel->setHidden(!(obj.isCustomItem && baseItem.Dimension.length()>0));
    ui->dimLineEdit->setHidden(!(obj.isCustomItem && baseItem.Dimension.length()>0));

    //    ui->quantityLineEdit->setText(QString::number(baseItem.quantity,'f',2));
    ui->discountLineEdit ->setText( QString::number(baseItem.discountPercentage,'f',2));
    ui->descriptionTextEdit->setPlainText(baseItem.ItemDescription);
    ui->modelNoLineEdit->setText(baseItem.ItemCode);
    ui->brandLineEdit->setText(baseItem.brandName);
    ui->narrationEdit->setPlainText(baseItem.narration);
//    ui->unitOfMeasurementComboBox->addItem(baseItem.UOMName);
    baseItem.ClosingStock = iv.getClosingStockofItemByDate(baseItem.ItemID,QDate::currentDate());
    ui->stockLineEdit->setText(QString::number(baseItem.ClosingStock,'f',2));

    qDebug()<<"Default UOM:"<<baseItem.uomObject.UomName;

    UomConvList=uomHelper->getUomByItem(baseItem.ItemID);

    for(UomConversionDataModel a:UomConvList){
        qDebug()<<"Adding :"<<uomHelper->getUOMNameById(a.ToUnit);
        ui->unitOfMeasurementComboBox->addItem(uomHelper->getUOMNameById(a.ToUnit), QVariant::fromValue(a));
    }


    qDebug()<<"Searching for text : "<<uomHelper->getUOMNameById(baseItem.uomObject.UomName);
    int index = ui->unitOfMeasurementComboBox->findText(baseItem.uomObject.UomName);
    if(index == -1){
        qDebug()<<"UOM EMPTY";
        qDebug()<<baseItem.uomObject.UomName;
    }
    qDebug()<<"Index from find"<<index;
    ui->unitOfMeasurementComboBox->setCurrentIndex(index);
    qDebug()<<"INdesx set";
    qDebug()<<"current index"<<ui->unitOfMeasurementComboBox->currentIndex();
    UomConversionDataModel uomObj = ui->unitOfMeasurementComboBox->currentData().value<UomConversionDataModel>();


    qDebug()<<"UOM Base Unit"<<uomObj._id;

    convalue = uomObj.conValue;
    uomId = uomObj.ToUnit;
    qDebug()<<"Set UOM ID"<<uomId;


    ui->priceLineEdit->setText(QString::number(baseItem.price*convalue,'f',2));
    ui->vatPercent->setText(QString::number(baseItem.taxRate,'f',2));
    on_vatPercent_editingFinished();
    ui->descriptionTextEdit->setReadOnly(!obj.isCustomItem);
    ui->quantityLineEdit->setFocus();
    //    qDebug()<<"set edit Completed ";

}

void addItemRequirement::appendSubItem(CompoundItemDataObject obj, int row)
{
//    openNewWidget = true;
//    //    qDebug()<<"edited row = "<<row;
//    //    qDebug()<<"row count = "<<ui->subItemsTable->rowCount();
//    if(ui->subItemsTable->rowCount()==row+1){
//        subOrdersList.append(obj.BaseItem);
//        //        qDebug()<<"item appended";
//    }
//    else{
//        subOrdersList[row]=obj.BaseItem;
//        //        qDebug()<<"item Inserted at "<<row;
//    }

//    setSubItemTable();

}

void addItemRequirement::setListNumber(int r)
{

    itemListNumber = r;
    ui->rowCount->setRange(1,r+1);
    ui->rowCount->setValue(r+1);
}

void addItemRequirement::calcTotal()
{
    calcSubItemsTotal();
    float qty = ui->quantityLineEdit->text().toFloat();
    float price = ui->priceLineEdit->text().toFloat();
    float length = ui->lengthLineEdit->text().toFloat();

    float subItemsTotal = ui->subItemsTotalLineEdit->text().toFloat();

    float discountPercent = ui->discountLineEdit->text().toFloat();

    float total = qty * price + subItemsTotal;

    float actualPrice = price + subItemsTotal;
    float actualTotal = qty * actualPrice;
    float vatRate = ui->vatPercent->text().toFloat();

    float subTotal = total;
    ui->itemSubTotalLineEdit->setText(QString::number(subTotal,'f',2));

    total -= total*discountPercent/100 ;
    float vatAmt = vatRate * total /100;

    total += vatAmt;
    ui->vatAmt->setText(QString::number(vatAmt,'f',2));
    ui->subItemsSubTotalLineEdit_2->setText(QString::number(subItemsTotal,'f',2));

    ui->itemTotalLineEdit->setText(QString::number(total,'f',2));
    if(ui->optionalCheckBox->isChecked()){
        ui->itemTotalLineEdit->setText("-");
    }

}

void addItemRequirement::showOptional()
{
    ui->optionalCheckBox->show();
}

void addItemRequirement::calcSubItemsTotal()
{
    float subTotal = 0;
    for(inventoryItemDataModel item:subOrdersList){
        subTotal += item.subTotal;
    }

    ui->subItemsTotalLineEdit->setText(QString::number(subTotal,'f',2));
}

void addItemRequirement::setSubItemTable()
{
    return;
//    //    qDebug()<<"Set Item Table";
//    int r = subOrdersList.size();
//    ui->subItemsTable->setRowCount(0);
//    ui->subItemsTable->setRowCount(r+1);
//    int i=0;
//    for(inventoryItemDataModel obj:subOrdersList){
//        //        qDebug()<<"####### loop count = "<<i;
//        QTableWidgetItem* SLNO = new QTableWidgetItem;
//        SLNO->setText(QString::number(i+1));
//        SLNO->setFlags(SLNO->flags() ^ Qt::ItemIsEditable);
//        ui->subItemsTable->setItem(i,0,SLNO);

//        QPointer<GMItemSearch> itemLine = new GMItemSearch(itemsListPtr);
//        itemLine->setText(obj.ItemName);
//        QWidget *SearchWid = new QWidget;
//        QVBoxLayout *lay = new QVBoxLayout();
//        lay->setContentsMargins(0,0,0,0);
//        lay->setSpacing(0);
//        lay->addWidget(itemLine);
//        SearchWid->setLayout(lay);
//        itemLine->setProperty("row",i);
//        SearchWid->setProperty("row",i);
//        ui->subItemsTable->setCellWidget(i,1,SearchWid);

//        QObject::connect(itemLine,SIGNAL(SelectedItemID(inventoryItemDataModel)),
//                         this,SLOT(getSubItemDetails(inventoryItemDataModel)));

//        //QObject::connect(ItemNameCombo,SIGNAL(activated(int)),this,SLOT(getSubItemDetails(int)));

//        QTableWidgetItem* uom = new QTableWidgetItem;
//        uom->setFlags(uom->flags() ^ Qt::ItemIsEditable);
//        uom->setText(obj.UOMName);
//        ui->subItemsTable->setItem(i,2,uom);

//        QTableWidgetItem* ModelNumber = new QTableWidgetItem;
//        ModelNumber->setFlags(ModelNumber->flags() ^ Qt::ItemIsEditable);
//        ModelNumber->setText(obj.ItemCode);
//        ui->subItemsTable->setItem(i,3,ModelNumber);

//        QTableWidgetItem* brandName = new QTableWidgetItem;
//        brandName->setFlags(brandName->flags() ^ Qt::ItemIsEditable);
//        brandName->setText(obj.brandName);
//        ui->subItemsTable->setItem(i,4,brandName);

//        QTableWidgetItem* serial = new QTableWidgetItem;
//        serial->setFlags(serial->flags() ^ Qt::ItemIsEditable);
//        serial->setText(obj.SerailNumber);
//        ui->subItemsTable->setItem(i,5,serial);

//        QTableWidgetItem* quantity = new QTableWidgetItem;
//        quantity->setFlags(quantity->flags() ^ Qt::ItemIsEditable);
//        quantity->setText(QString::number(obj.quantity,'f',2));
//        ui->subItemsTable->setItem(i,6,quantity);

//        QTableWidgetItem* price = new QTableWidgetItem;
//        price->setFlags(price->flags() ^ Qt::ItemIsEditable);
//        price->setText(QString::number(obj.price,'f',2));
//        ui->subItemsTable->setItem(i,7,price);

//        QTableWidgetItem* amount = new QTableWidgetItem;
//        amount->setFlags(amount->flags() ^ Qt::ItemIsEditable);
//        amount->setText(QString::number(obj.subTotal,'f',2));
//        ui->subItemsTable->setItem(i,8,amount);


//        QToolButton* delButton = new QToolButton;
//        delButton->setIcon(QIcon(":/icons/trash.ico"));
//        delButton->setProperty("row",i);
//        delButton->setAutoRaise(true);
//        //deleteItem->setIcon(QIcon(":/icons/trash.ico"));
//        ui->subItemsTable->setCellWidget(i,9,delButton);
//        QObject::connect(delButton,SIGNAL(clicked(bool)),this,SLOT(delButtonClicked(bool)));
//        i++;
//    }
//    //    qDebug()<<"Creating Empty row at "<<i;

//    QPointer<GMItemSearch> itemLine = new GMItemSearch(itemsListPtr);
//    QWidget *SearchWid = new QWidget;
//    QVBoxLayout *lay = new QVBoxLayout();
//    lay->setContentsMargins(0,0,0,0);
//    lay->setSpacing(0);
//    lay->addWidget(itemLine);
//    SearchWid->setLayout(lay);
//    itemLine->setProperty("row",i);
//    SearchWid->setProperty("row",i);

//    ui->subItemsTable->setCellWidget(i,1,SearchWid);

//    QObject::connect(itemLine,SIGNAL(SelectedItemID(inventoryItemDataModel)),
//                     this,SLOT(getSubItemDetails(inventoryItemDataModel)));


//    QPointer<QLineEdit> uomEmpty = new QLineEdit;
//    uomEmpty->setEnabled(false);
//    uomEmpty->setStyleSheet("QLineEdit { border: none }");
//    ui->subItemsTable->setCellWidget(i,2,uomEmpty);
//    //    qDebug()<<"set up uom done";
//    //ItemNameCombo->setFocus();
//    itemLine->setFocus();
//    // QObject::connect(ItemNameCombo,SIGNAL(activated(int)),this,SLOT(getSubItemDetails(int)));
//    calcTotal();
}

void addItemRequirement::getSubItemDetails(inventoryItemDataModel obj)
{
    //qDebug()<<"open widget r ="<<r;
    //    qDebug()<<openNewWidget;
//    if(openNewWidget)
//    {
//        //        qDebug()<<"inside";
//        openNewWidget = false;

//        addItemRequirement* addItemRequirementWidget = new addItemRequirement(this);
//        addItemRequirementWidget->setWindowFlags(Qt::Window);
//        addItemRequirementWidget->setItemsListPtr(itemsListPtr);
//        int currRow =  ui->subItemsTable->currentRow();

//        if (currRow == -1) {
//            if (QWidget* focused = ui->subItemsTable->focusWidget()) {
//                currRow = focused->property("row").toInt();
//            }
//        }

//        addItemRequirementWidget->setListNumber(currRow);
//        addItemRequirementWidget->getFirstItemDetail(obj);
//        addItemRequirementWidget->hideSubItemsTab();
//        addItemRequirementWidget->showMaximized();
//        QObject::connect(addItemRequirementWidget,SIGNAL(addNewItem(CompoundItemDataObject,int)),
//                         this,SLOT(appendSubItem(CompoundItemDataObject,int)));
//        QObject::connect(addItemRequirementWidget,SIGNAL(closing(int)),
//                         this,SLOT(resetFlag(int)));
//    }
}


void addItemRequirement::appendItem(CompoundItemDataObject item, int row)
{

}

void addItemRequirement::setProjectID(QString projID)
{
    this->projectID=projID;
}

void addItemRequirement::delButtonClicked(bool v)
{
    int x= sender()->property("row").toInt();

    ui->subItemsTable->removeRow(x);
    subOrdersList.removeAt(x);

    setSubItemTable();

}

void addItemRequirement::resetFlag(int r)
{
    //    qDebug()<<"vaue of r :"<<r;
    if(ui->subItemsTable->rowCount()==r+1){
        try{
            QLineEdit* uom = qobject_cast<QLineEdit*>( ui->subItemsTable->cellWidget(r,2));
            if(uom->text().length()<1){
                //                qDebug()<<"waa";
                //                GMItemSearch* combo= qobject_cast<GMItemSearch*> (ui->subItemsTable->cellWidget(r,1));
                //                combo->setText("");
            }
        }
        catch(...){

        }
    }
    openNewWidget = true;
}

void addItemRequirement::DisablePriceEdit(bool value)
{
    priceEdit = !value;
    ui->priceLineEdit->setReadOnly(value);
}

void addItemRequirement::DisableItemUpdate(bool value)
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(value);
}

void addItemRequirement::disableDesriptionEdit(bool)
{
    ui->descriptionTextEdit->setReadOnly(true);
}

void addItemRequirement::on_AddSubItemButton_clicked()
{
    addItemRequirement *addItemRequirementWidget = new addItemRequirement(this);
    addItemRequirementWidget->setWindowFlags(Qt::Window);
    addItemRequirementWidget->hideSubItemsTab();
    addItemRequirementWidget->showMaximized();
    QObject::connect(addItemRequirementWidget,SIGNAL(addNewItem(CompoundItemDataObject)),this,SLOT(appendSubItem(CompoundItemDataObject)));
}

void addItemRequirement::uomIndexChanged(int index)
{
    qDebug()<<"UOM Index changed"<<index;
    UomConversionDataModel uomObj = ui->unitOfMeasurementComboBox->currentData().value<UomConversionDataModel>();

    convalue = uomObj.conValue;
    uomId = uomObj.ToUnit;

    qDebug()<<"#######################Set UOM ID"<<uomId<<convalue;
    qDebug()<<"Qty:"<<baseItem.quantity;

    ui->priceLineEdit->setText(QString::number(baseItem.price*convalue,'f',2));
    ui->quantityLineEdit->setText(QString::number(baseItem.quantity/convalue,'f',2));
}


void addItemRequirement::on_quantityLineEdit_textChanged(const QString &arg1)
{
    baseItem.quantity = arg1.toFloat()*convalue;
    calcTotal();
}

void addItemRequirement::on_priceLineEdit_textChanged(const QString &arg1)
{
    baseItem.price = arg1.toFloat()/convalue;
    calcTotal();
}

void addItemRequirement::on_discountLineEdit_textChanged(const QString &arg1)
{
    calcTotal();
}

void addItemRequirement::on_itemTotalLineEdit_returnPressed()
{
    float total = ui->itemTotalLineEdit->text().toFloat();
    float qty = ui->quantityLineEdit->text().toFloat();
    if(qty ==0){
        return;
    }
    float disc = ui->discountLineEdit->text().toFloat();
    float price = total / (qty - (qty - disc /100));
    ui->priceLineEdit->setText(QString::number(price*convalue,'f',2));
}

void addItemRequirement::closeEvent(QCloseEvent *e)
{

    emit closing(itemListNumber);
    qDebug()<<"Closed from window";
    e->accept();
}




void addItemRequirement::on_subItemsTable_doubleClicked(const QModelIndex &index)
{
    //    qDebug()<<"table double cliked";
    int r= index.row();
    if(openNewWidget &&  r>=0)
    {
        //        qDebug()<<"inside";
        openNewWidget = false;

        addItemRequirement* addItemRequirementWidget = new addItemRequirement(this);
        addItemRequirementWidget->setWindowFlags(Qt::Window);
        int currRow =  ui->subItemsTable->currentRow();

        if (currRow == -1) {
            if (QWidget* focused = ui->subItemsTable->focusWidget()) {
                currRow = focused->property("row").toInt();
            }
        }

        addItemRequirementWidget->setListNumber(currRow);
        addItemRequirementWidget->getFirstItemDetail(subOrdersList[r]);
        addItemRequirementWidget->hideSubItemsTab();
        addItemRequirementWidget->show();
        QObject::connect(addItemRequirementWidget,SIGNAL(addNewItem(CompoundItemDataObject,int)),
                         this,SLOT(appendSubItem(CompoundItemDataObject,int)));
        QObject::connect(addItemRequirementWidget,SIGNAL(closing(int)),
                         this,SLOT(resetFlag(int)));
    }
}

void addItemRequirement::showEvent(QShowEvent *event)
{
//    setSubItemTable();
    ui->quantityLineEdit->setFocus();
    ui->quantityLineEdit->selectAll();

}

void addItemRequirement::on_lengthLineEdit_textChanged(const QString &arg1)
{
    if(priceEdit)
        calcTotal();
}


void addItemRequirement::on_optionalCheckBox_stateChanged(int arg1)
{
    qDebug()<<"Is optional checked()"<<ui->optionalCheckBox->isChecked();
    if(ui->optionalCheckBox->isChecked()){
        ui->itemTotalLineEdit->setText("-");
    }
    else{
        calcTotal();
    }
}

void addItemRequirement::on_rowCount_valueChanged(int arg1)
{

}

void addItemRequirement::on_vatPercent_editingFinished()
{
    float perc = ui->vatPercent->text().toFloat();
    qDebug()<<"Vat %="<<perc;
    float discountPercent = ui->discountLineEdit->text().toFloat();
    float subtotal = ui->itemSubTotalLineEdit->text().toFloat();

    float vatAmt = (subtotal - subtotal* discountPercent /100) * perc/100;
    ui->vatAmt->setText(QString::number(vatAmt,'f',2));

    calcTotal();

}

void addItemRequirement::on_quantityLineEdit_returnPressed()
{
    ui->priceLineEdit->setFocus();
}

void addItemRequirement::on_priceLineEdit_returnPressed()
{
    ui->discountLineEdit->setFocus();
}

void addItemRequirement::on_discountLineEdit_returnPressed()
{
    ui->buttonBox->button(QDialogButtonBox::Save)->setFocus();
}

void addItemRequirement::on_vatPercent_returnPressed()
{

//    float perc = ui->vatPercent->text().toFloat();
//    qDebug()<<"Vat %="<<perc;
//    float discountPercent = ui->discountLineEdit->text().toFloat();
//    float subtotal = ui->itemSubTotalLineEdit->text().toFloat();

//    float vatAmt = (subtotal - subtotal* discountPercent /100) * perc/100;
//    ui->vatAmt->setText(QString::number(vatAmt,'f',2));

//    calcTotal();

}
