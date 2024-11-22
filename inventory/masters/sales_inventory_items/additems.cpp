#include "additems.h"
#include "ui_additems.h"
#include <QDate>
#include "customwidgets/gmsearchlineedit.h"
#include <QCompleter>
#include <QLabel>
#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include <QMessageBox>
#include <sharetools/print/printerhelper.h>

#include <QTableView>
#include <QStandardItem>

#include <utils/gmfileuploadhelper.h>
AddItems::AddItems(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddItems)
{
    ui->setupUi(this);

    QString bgcolor = "#e0f7fa";
    this->setObjectName("background");
    this->setStyleSheet("QWidget#background{background-color:" + bgcolor + "}");
    model = new QSqlQueryModel();
    invItem = new inventoryItemDataModel();
    compItem = new CompoundItemDataObject();

    ui->tabWidget->setObjectName("tabw");
    ui->tabWidget->setStyleSheet("QTabWidget#tabw{background-color:" + bgcolor + "}");
    ui->tabWidget->setCurrentIndex(0);
    QString taxNameVal = ConfigurationSettingsDatabaseHelper::getValue(taxName,"Tax").toString();
    ui->rateOfVATLabel->setText("Rate of "+taxNameVal);
    ui->tabWidget->setTabText(1,taxNameVal);

    ui->tabWidget->setTabShape(QTabWidget::Triangular);
    ui->tabWidget->setTabsClosable(true);

    snackbar = new QtMaterialSnackbar(this);
    ui->uomLineEditq->hide();
    ui->uomLabel->hide();
    ui->pluCheck->hide();
    ui->deletePushButton->hide();

    ui->hSNCodeLabel->hide();
    ui->hSNCodeLineEdit->hide();

    ui->purchaseItemCheckBox->setChecked(true);
    ui->salesItemCheckBox->setChecked(true);
    ui->isStockItem->setChecked(true);

    ui->isBundled->setChecked(false);

    //    CategoriesList<<"STORE"<<"BAKERY"<<"RESTAURANT";
    //    QFile file("cat.txt");

    //    if (!file.open(QIODevice::ReadOnly))
    //    {
    //        qDebug() << file.errorString();
    //    }
    //    // QStringList wordList;
    //    while (!file.atEnd()) {
    //        QByteArray line = file.readLine();
    //        QString values = line;
    //        //   wordList.append(line.split('|').at(2));
    //        CategoriesList = values.split("|");

    //    }

    if(CounterSettingsDataModel::TypeOfTrade == "ERP"){
        ui->kotprinterlabel->setText("Select Printers for Gate Pass");
    }
    else if(CounterSettingsDataModel::TypeOfTrade.contains("restaurant",Qt::CaseInsensitive)){
        ui->purchaseItemCheckBox->setChecked(false);
        ui->isStockItem->setChecked(false);
    }

    ui->openingBalanceDateDateEdit->setDate(QDate::currentDate());
    ui->tabWidget->setStyleSheet("background-color:#ffffff");

    itemGroupHelper= new SalesInventoryGroupsDatabaseHelper();

    ui->toTimeTimeEdit->setDateTime(QDateTime::fromString("23:59:59","yyyy-MM-dd hh:mm:ss"));

    dbHelper = new SalesInventoryItemDatabaseHelper ();
    godownHelper = new GodownDatabaseHelper();
    ledgerHelper = new LedgerMasterDatabaseHelper();
    uomHelper = new UomDataBaseHelper();
    propertyHelper = new PropertyDatabaseHelper();
    brandsHelper = new BrandsDatabaseHelper();
    imagesHelper = new InventoryItemImagesDatabaseHelper();
    bomHelper = new BillOfMaterialsIngredientsDataBaseHelper();
    categoryHelper = new CategoryDatabaseHelper;
    bundleHelper = new BundleDatabaseHelper;

    priceListWidget = new PriceListByItem("",dbHelper,this);
    ui->priceLayout->addWidget(priceListWidget);



    ui->dimLabel->setHidden(!ui->customItemCheckBox->isChecked());
    ui->dimensionEdit->setHidden(!ui->customItemCheckBox->isChecked());

    labelWidget = new ItemLabelData();

    ui->verticalLayout_7->addWidget(labelWidget);

    defaultLedgerSearch = new GMLedgerLineWidget(ledgerHelper,ledgerHelper->getLedgerQueryString());

    ui->defLedgerLay->addWidget(defaultLedgerSearch);

    QObject::connect(defaultLedgerSearch,&GMLedgerLineWidget::itemSelectedwithItem,[=](LedgerMasterDataModel ledger){
        invItem->defaultLedgerId = ledger.LedgerID;
    });

    setupItemsTab();

    setupPropertiesTab();
    setupUOMTab();
    setupBOMTab();
    setupImagesTab();
    setupBundleTab();



    ui->barcodeCopiesLineEdit->setValidator(new QDoubleValidator(0.0002,999999,2,this));
    //    ui->rateOfVATLineEdit->setValidator(new QDoubleValidator(0.0002,999999,2,this));
    ui->openingBalanceLineEdit->setValidator(new QDoubleValidator(0.0002,999999,2,this));
    ui->openingBalanceValueLineEdit->setValidator(new QDoubleValidator(0.0002,999999,2,this));
    ui->priceLineEdit->setValidator(new QDoubleValidator(0.0002,999999,2,this));
    ui->nettPrice->setValidator(new QDoubleValidator(0.0002,999999,2,this));
    ui->onlinePriceLine->setValidator(new QDoubleValidator(0.0002,999999,2,this));
    ui->reorderLevelLineEdit->setValidator(new QDoubleValidator(0.0002,999999,2,this));
    ui->warrantyPeriodLineEdit->setValidator(new QDoubleValidator(0.0002,999999,2,this));
    ui->shelfLifeLineEdit->setValidator(new QDoubleValidator(0.0002,999999,2,this));
    ui->stdCostLineEdit->setValidator(new QDoubleValidator(0.0002,999999,2,this));
    ui->purchLine->setValidator(new QDoubleValidator(0.0002,999999,3,this));
    ui->imageWidth->setMaximum(200);
    ui->imageWidth->setMinimum(50);
    ui->imageHeight->setMaximum(200);
    ui->imageHeight->setMinimum(50);
    ui->expDate->setDate(QDate::currentDate());
    ui->mfdDate->setDate(QDate::currentDate());

    //    ui->rateOfVATLineEdit->setText("0.00");

    setupRestaurantTab();
    setupPriceListTab();
    //    setUOMTable();


    ui->baseUnitOfMeasurementComboBox->setEnabled(true);
}


AddItems::~AddItems()
{
    delete ui;
}

void AddItems::setItem(inventoryItemDataModel *value)
{
    invItem = value;
    qDebug()<<"SET ITEM"<<invItem->ItemName<<value->fromExternal<<invItem->fromExternal;

    if(invItem->fromExternal){
        if(  dbHelper->checkItemExists(invItem->ItemID) ){
            invItem->action = 1002;
        }
        else {
            invItem->action = 1001;
        }
    }

}

void AddItems::disableSave()
{
    ui->buttonBox->hide();
}

void AddItems::disableDelete()
{
    ui->deletePushButton->hide();
}


void AddItems::on_addPropertyPushButton_clicked()
{
    addPropertyToTable();
}

void AddItems::on_buttonBox_accepted()
{
    inventoryItemDataModel item;// = new inventoryItemDataModel();

    item.ItemName = ui->nameLineEdit->text();

    if(item.ItemName.length()<1){
        snackbar->addMessage("Item Name Cannot Be Blank");
        return;
    }

    item.itemNameArabic = ui->arabicNameLineEdit->text();
    item.ItemID =id;
    item.fromExternal = invItem->fromExternal;
    item.action = invItem->action;
    item.GroupID = invGroups.key(ui->groupNameComboBox->currentText());
    item.section = sections.key(ui->sectionComboBox->currentText());
    item.Category = categories.key(ui->categoryComboBox->currentText());
    if(item.GroupID.length()<1)
    {
        snackbar->addMessage("Select Parent Group");
        return;
    }

    qDebug()<<"!!! gp name = "<<item.GroupID;

    item.price = ui->priceLineEdit->text().toFloat();
    item.price_2 =  ui->onlinePriceLine->text().toFloat();
    item.OpeningStock = ui->openingBalanceLineEdit->text().toFloat();
    item.OpeningStockValue = ui->openingBalanceValueLineEdit->text().toFloat();
    item.OpeningStockDate = ui->openingBalanceDateDateEdit->date();
    item.narration=ui->narrationLineEdit->text();
    item.ReorderLevel=ui->reorderLevelLineEdit->text().toFloat();
    item.warrantyDays=ui->warrantyPeriodLineEdit->text().toInt();
    item.shelfLife=ui->shelfLifeLineEdit->text().toFloat();
    item.ItemAlias=ui->aliasLineEdit->text();
    item.ItemCode=ui->codeLineEdit->text();
    if(item.ItemCode.length() > 0 && dbHelper->checkBarCodeExists(item.ItemCode, item.ItemID)){
        snackbar->addMessage("Item Code Exists");
        return;
    }
    item.PartNumber=ui->partNoLineEdit->text();
    item.SerailNumber = ui->SerialLineEdit->text();
    item.isSerailNumbered = ui->isSerialNumbered->isChecked();
    item.isBatchProcessed = ui->isBatchProcessed->isChecked();

    if(item.SerailNumber.length()>0){
        if(item.ItemCode.length()<5){
            item.ItemCode = item.SerailNumber;
//            item.ItemCode = QString::number(00000+item.SerailNumber.toInt());
                        item.ItemCode = QString("%1").arg(item.ItemCode.toInt(), 5, 10, QChar('0'));
        }
    }

    //    if(!ui->customItemCheckBox->isChecked()){
    //        ui->SerialLineEdit->setText("");
    //        ui->codeLineEdit->setText("");

    //    }

    item.brandID = brandsMap.key(ui->brandsComboBox->currentText());
    item.ItemDescription = ui->descriptionEdit->toPlainText();
    item.isCustomItem = ui->customItemCheckBox->isChecked();
    item.Dimension = ui->dimensionEdit->text();
    item.isPurchaseItem = ui->purchaseItemCheckBox->isChecked();
    item.isSalesItem = ui->salesItemCheckBox->isChecked();
    item.hideInPrint = ui->hideInBill->isChecked();
    item.isStockItem = ui->isStockItem->isChecked();
    item.isBundled = ui->isBundled->isChecked();


    item.flags.insert("bundled",QJsonValue( item.isBundled));

//    qDebug()<<"Flags : "<<item.flags;

    item.favo = ui->FavoCheckBox->isChecked();
    item.defaultPurchaseLedgerID = PurchaseledgersList[ui->defaultPurchaseLedgerComboBox->currentIndex()].LedgerID;
    item.defaultSalesLedgerID = SalesledgersList[ui->defaultSalesLedgerComboBox->currentIndex()].LedgerID;
    item.DefaultInputTaxLedgerID = ui->defaultInputTaxLedgerComboBox->currentData().toString();
    item.DefaultOutputTaxLedgerID = ui->defaultOutputTaxLedgerComboBox->currentData().toString();
    item.DefaultPurchaseReturnLedgerID = ui->defaultPurchaseReturnLedgerComboBox->currentData().toString();
    item.DefaultSalesReturnLedgerID = ui->defaultSalesReturnLedgerComboBox->currentData().toString();
    //    item.taxRate=ui->rateOfVATLineEdit->text().toFloat();

    item.taxRate=ui->gstComboBox->currentText().toFloat();
    item.fromTime = ui->fromTimeTimeEdit->time();
    item.toTime = ui->toTimeTimeEdit->time();
    item.hsnCode = ui->hSNCodeLineEdit->text();

//    QJsonObject flags = item.flags;

    //    item.defaultPurchaseLedgerID  = "0x8x8";//PurchaseledgersList[ui->defaultPurchaseLedgerComboBox->currentIndex()].LedgerID;
    //    item.defaultSalesLedgerID     = "0x7x5";//SalesledgersList[ui->defaultSalesLedgerComboBox->currentIndex()].LedgerID;
    //    item.DefaultInputTaxLedgerID  = "0x2x14x7";//ui->defaultInputTaxLedgerComboBox->currentData().toString();
    //    item.DefaultOutputTaxLedgerID = "0x2x14x6";//ui->defaultOutputTaxLedgerComboBox->currentData().toString();
    //    item.DefaultPurchaseReturnLedgerID = "0x8x9";//ui->defaultPurchaseReturnLedgerComboBox->currentData().toString();
    //    item.DefaultSalesReturnLedgerID = "0x7x10";//ui->defaultSalesReturnLedgerComboBox->currentData().toString();
    //    item.vatRate = 0;

//    qDebug()<< "input vat "<<item.DefaultInputTaxLedgerID ;
//    qDebug()<< "output vat "<<item.DefaultOutputTaxLedgerID ;

    item.stdCost = ui->stdCostLineEdit->text().toFloat();

    //Adding printers to be linked with item.
    QStringList KOTPrinterList;
    QString KOTPrinters ="|";
    KOTPrinterList = QPrinterInfo::availablePrinterNames();
    //    qDebug()<<"Printers cnt : "<<ui->KOTPrintersTableWidget->rowCount();
    for(int i=0;i<ui->KOTPrintersTableWidget->rowCount();i++){
        QCheckBox* check = qobject_cast <QCheckBox*> (ui->KOTPrintersTableWidget->cellWidget(i,1));
        //        qDebug()<<"Checked "<<i<<check->isChecked();
        if(check->isChecked()){
            KOTPrinters += ui->KOTPrintersTableWidget->item(i,0)->text() + "|";
        }
    }

    item.KOTPrinter = KOTPrinters;




    //    qDebug()<<"KotPrinters: "<<KOTPrinters;
    //    qDebug()<<"uoms "<<ui->tableWidgetPropertis->rowCount();

    for(int i=0;i<ui->tableWidgetPropertis->rowCount();i++){
        QString index = ui->tableWidgetPropertis->item(i,0)->text();
        //        qDebug()<<index;
        if(index=="")
            continue;
        PropertyDataModel prop;
        prop.ItemProperty_PropertyID = propertyHelper->getPropertyIDByName(index);
        item.baseProperties.append(prop);
    }


    //    int uomIndex = ui->baseUnitOfMeasurementComboBox->currentIndex();
    item.defaultUOMID= ui->baseUnitOfMeasurementComboBox->currentData().toString();
    //    qDebug()<<"UOM ID : "<<item.defaultUOMID;
    compItem->BaseItem = item;

    uomHelper = new UomDataBaseHelper();
    UomConversionDataModel* baseUom = new UomConversionDataModel();
    //    qDebug()<<"Base UOM:"<<ui->baseUnitOfMeasurementComboBox->currentIndex();
    baseUom->BaseUnit = ui->baseUnitOfMeasurementComboBox->currentData().toString();
    baseUom->ToUnit = ui->baseUnitOfMeasurementComboBox->currentData().toString();
    baseUom->conValue = 1;
    baseUom->Narration = "self conversion";
    baseUom->itemID = id;
    baseUom->barCode = item.ItemCode;

    UomConversionDataModel *oldUom = new UomConversionDataModel;
    oldUom->BaseUnit = invItem->uomObject._id;
    oldUom->ToUnit = invItem->uomObject._id;
    oldUom->itemID = id;

    compItem->BaseItem.uomObject = uomHelper->getUomObjectByID(compItem->BaseItem.defaultUOMID);

    bool saveStatus=false;

    if(id =="" || (item.fromExternal == true && item.action == 1001)){
        //        qDebug()<<"Insert";
        if(dbHelper->insertSalesInventoryItem(compItem,imageData)){
            baseUom->itemID = compItem->BaseItem.ItemID;
            uomHelper->insertUOMConversion(baseUom);

            //            savePriceList();
            priceListWidget->setItemID(baseUom->itemID, baseUom->_id);
            priceListWidget->savePrices();
            compItem->Action=1001;
            saveStatus = true;
        }
    }
    else{
        //        qDebug()<<"UPDATE ITEM********* "<<id;
        //        qDebug()<<item.objToJson(item);
        if(dbHelper->updateSalesInventoryItem(compItem,id,imageData)){
            bomHelper->deleteBillOfMaterials(id);
            if(boms.size()>0){
                bomHelper->insertBillOfMaterials(boms);
            }

            uomHelper->deleteBaseUOMConversion(oldUom); // Login to delete base UOM where convValue=1
            uomHelper->insertUOMConversion(baseUom);
            priceListWidget->savePrices();
            //            savePriceList();
            compItem->Action=1002;
            saveStatus = true;
        }

    }

    if(saveStatus && item.fromExternal)
        emit NotificationAccepted();

    if( saveStatus && !item.fromExternal ){
        LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper();
        QStringList ledgerNames = ledHelper->getLedgersListHavingDbName();
        //        qDebug()<<"Ledgers to send"<<ledgerNames.size()<<ledgerNames;
        if(ledgerNames.size()> 0){
            if(ConfigurationSettingsDatabaseHelper::getValue(promptToSendToOtherLedgers,true).toBool()){

                NotificationLedgersList *ledWidget = new NotificationLedgersList();
                ledWidget->setWindowFlags(Qt::WindowStaysOnTopHint);
                ledWidget->show();
                QObject::connect(ledWidget, &NotificationLedgersList::sendNotification, this, &AddItems::sendNotifications);
                QObject::connect(ledWidget, &NotificationLedgersList::closing, this, &AddItems::on_buttonBox_rejected);
            }
            else
                sendNotifications(ledHelper->getDbNamesList());
        }
        else{
            this->close();
            emit savedItem(*compItem);
        }
    }

    else{
        this->close();
        emit savedItem(*compItem);
    }
    emit closing();

}

void AddItems::on_buttonBox_rejected()
{
    this->close();
}

void AddItems::on_newImagePushButton_clicked()
{
    //    qDebug()<<QDir::homePath();
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
        GMFileUploadHelper *uploader = new GMFileUploadHelper(fileName,"inv_items/"+invItem->ItemID+".png");
        uploader->multipart();
        connect(uploader,&GMFileUploadHelper::uploadedSignal,this,[=](bool result,QString text){
            if(result == false){
                snackbar->addMessage("Error Uploading : "+text);
                // QMessageBox box; box.setText("Upload Error"); box.exec();
            }
            else{
                snackbar->addMessage("Saved");
            }
        });

        //        connect(uploader,&GMFileUploadHelper::progressSignal,this,[=](qint64 bytesSent, qint64 bytesTotal){
        //            progress->setValue((bytesSent*100)/bytesTotal);
        //        });

    }

    return;

    //Old code
//    QString fileName = QFileDialog::getOpenFileName(this,
//                                                    "Select an image for "+ui->nameLineEdit->text()
//                                                    + " Model :" + ui->codeLineEdit->text(),
//                                                    QDir::homePath(),
//                                                    tr("Image Files (*.png *.jpg *.bmp)"));
//    QImage image(fileName);
//    ui->picture->setPixmap(QPixmap::fromImage(image.scaled(150,150))); // Put image into QLabel object (optional)
//    ui->imageHeight->setValue(150);
//    ui->imageWidth->setValue(150);
//    ui->picture->setStyleSheet("border: 2px solid");
//    ui->imageLabel->setText("Image : "+fileName);

//    imageData.imageFilename=fileName;

//    QFile f(imageData.imageFilename);
//    if(f.open(QIODevice::ReadOnly))
//    {
//        imageData.img = f.readAll();
//        f.close();
//    }

//    imageData.imageHeight=120;
//    imageData.imageWidth=180;

}

void AddItems::setupItemsTab()
{
    invGroups=itemGroupHelper->getSalesInventoryGroupDataAsMap();
    sections = godownHelper->getGodownForSearchAsMap();
    categories = categoryHelper->getAllCategoryAsMap();

    SalesledgersList = ledgerHelper->getAllLedgersUnderGroupByName("Sales Accounts");
    //        qDebug()<<"Sales LEdger : "<<SalesledgersList.size();
    PurchaseledgersList = ledgerHelper->getAllLedgersUnderGroupByName("Purchase Accounts");
    //        qDebug()<<"Purchase LEdger : "<<PurchaseledgersList.size();
    VATledgersList = ledgerHelper->getAllLedgersUnderGroupByName("Duties and Taxes");

    for(LedgerMasterDataModel ledger:PurchaseledgersList){
        ui->defaultPurchaseLedgerComboBox->addItem(ledger.LedgerName,ledger.LedgerID);
        ui->defaultPurchaseReturnLedgerComboBox->addItem(ledger.LedgerName,ledger.LedgerID);
    }
    for(LedgerMasterDataModel ledger:SalesledgersList){
        ui->defaultSalesLedgerComboBox->addItem(ledger.LedgerName,ledger.LedgerID);
        ui->defaultSalesReturnLedgerComboBox->addItem(ledger.LedgerName,ledger.LedgerID);
    }
    for(LedgerMasterDataModel ledger:VATledgersList){
        ui->defaultInputTaxLedgerComboBox->addItem(ledger.LedgerName,ledger.LedgerID);
        ui->defaultOutputTaxLedgerComboBox->addItem(ledger.LedgerName,ledger.LedgerID);
    }

    GMSearchLineEdit *lineEdit = new GMSearchLineEdit;

    lineEdit->setData(invGroups.values());
    ui->groupNameComboBox->addItem("");
    ui->groupNameComboBox->addItems(invGroups.values());
    ui->groupNameComboBox->setLineEdit(lineEdit);

    QCompleter *completer2 = new QCompleter(invGroups.values(), this);
    completer2->setFilterMode(Qt::MatchContains);
    completer2->setCaseSensitivity(Qt::CaseInsensitive);
    completer2->setCompletionMode(QCompleter::PopupCompletion);

    ui->groupNameComboBox->setCompleter(completer2);

    GMSearchLineEdit *godownLineEdit = new GMSearchLineEdit;

    godownLineEdit->setData(sections.values());
    //    godownLineEdit->setText("Store");
    ui->sectionComboBox->addItems(sections.values());
    //    ui->sectionComboBox->setLineEdit(godownLineEdit);

    QCompleter *completer4 = new QCompleter(sections.values(), this);
    completer4->setFilterMode(Qt::MatchContains);
    completer4->setCaseSensitivity(Qt::CaseInsensitive);
    completer4->setCompletionMode(QCompleter::PopupCompletion);

    //    ui->sectionComboBox->setCompleter(completer4);
    ui->sectionComboBox->setCurrentText(godownHelper->getGodownNameById("Godown"));

    //    ui->categoryComboBox->addItem(" ");

    ui->categoryComboBox->addItems(categories.values());
    //    ui->categoryComboBox->addItem("STORE");
    //    ui->categoryComboBox->addItem("BAKERY");
    //    ui->categoryComboBox->addItem("RESTAURANT");

    brandsMap = brandsHelper->getAllBrandsAsMap();
    GMSearchLineEdit *blineEdit = new GMSearchLineEdit;
    blineEdit->setData(brandsMap.values());
    ui->brandsComboBox->addItem("");
    ui->brandsComboBox->addItems(brandsMap.values());
    ui->brandsComboBox->setLineEdit(blineEdit);

    QCompleter *completer3 = new QCompleter(brandsMap.values(), this);
    completer3->setFilterMode(Qt::MatchContains);
    completer3->setCaseSensitivity(Qt::CaseInsensitive);
    completer3->setCompletionMode(QCompleter::PopupCompletion);

    ui->brandsComboBox->setCompleter(completer3);


    QStringList gstRates;
    gstRates<<"0.00"<<"5.00";
    ui->gstComboBox->addItems(gstRates);

    //    if(LoginValues::getCompany().trn.length()>0)
    //        ui->gstComboBox->setCurrentText("5");


}

void AddItems::setupPropertiesTab()
{
    AllPropertyList = propertyHelper->getAllProperties();
    QStringList propNames;
    for(PropertyDataModel prop:AllPropertyList)
        propNames.append(prop.PropertyName);
    ui->propertyComboBox->addItem("");
    ui->propertyComboBox->addItems(propNames);
    GMSearchLineEdit *lineEdit = new GMSearchLineEdit;
    lineEdit->setData(propNames);
    ui->propertyComboBox->setLineEdit(lineEdit);
    QCompleter *completer2 = new QCompleter(propNames, this);
    completer2->setFilterMode(Qt::MatchContains);
    completer2->setCaseSensitivity(Qt::CaseInsensitive);
    completer2->setCompletionMode(QCompleter::PopupCompletion);

    ui->propertyComboBox->setCompleter(completer2);

    ui->tableWidgetPropertis->setColumnCount(2);
    ui->tableWidgetPropertis->setRowCount(0);

    QTableWidgetItem* GroupName = new QTableWidgetItem;
    GroupName->setText("Property Name");
    ui->tableWidgetPropertis->setHorizontalHeaderItem(0,GroupName);

    QTableWidgetItem* parentName = new QTableWidgetItem;
    parentName->setIcon(QIcon(":/icons/trash.ico"));
    ui->tableWidgetPropertis->setHorizontalHeaderItem(1,parentName);
    ui->tableWidgetPropertis->setColumnWidth(1,25);
    ui->tableWidgetPropertis->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    QObject::connect(ui->tableWidgetPropertis,SIGNAL(cellClicked(int,int)),this,SLOT(delPropPressed(int,int)));


}

void AddItems::setupUOMTab()
{
    AllUomList = uomHelper->getAllUOM();
    for(UomDataModel a:AllUomList){
        ui->baseUnitOfMeasurementComboBox->addItem(a.UomName, a._id);
    }

}

void AddItems::setupBOMTab()
{

    ui->itemCostByStdCostLineEdit->setReadOnly(true);

    ui->BOMTableWidget->setColumnCount(6);

    int i=0;
    QTableWidgetItem* PurchaseItem = new QTableWidgetItem;
    PurchaseItem->setText("Ingredient");
    ui->BOMTableWidget->setHorizontalHeaderItem(i++,PurchaseItem);

    QTableWidgetItem* Uom = new QTableWidgetItem;
    Uom->setText("Uom");
    ui->BOMTableWidget->setHorizontalHeaderItem(i++,Uom);


    QTableWidgetItem* Purchqty = new QTableWidgetItem;
    Purchqty->setText("Qty");
    ui->BOMTableWidget->setHorizontalHeaderItem(i++,Purchqty);

    QTableWidgetItem* SalesQty = new QTableWidgetItem;
    SalesQty->setText("Sales Qty");
    ui->BOMTableWidget->setHorizontalHeaderItem(i++,SalesQty);

    QTableWidgetItem*  takeawy = new QTableWidgetItem;
    takeawy->setText("Takeaway");
    ui->BOMTableWidget->setHorizontalHeaderItem(i++,takeawy);

    QTableWidgetItem*  deleta = new QTableWidgetItem;
    deleta->setText("Delete");
    ui->BOMTableWidget->setHorizontalHeaderItem(i++,deleta);

    ui->BOMTableWidget->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);

    i=0;

    QList<QSharedPointer<inventoryItemDataModel>> itemsListPtr = dbHelper->getALLInventoryObjectsAsPtr();

    itemLine = new GMItemSearch(itemsListPtr);

    itemLine->setMinimumWidth(200);

    QToolButton* SearchWid = new QToolButton;
    SearchWid->setAutoRaise(true);
    SearchWid->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Preferred);


    QVBoxLayout *lay = new QVBoxLayout();
    lay->setContentsMargins(0,0,0,0);
    lay->setSpacing(0);

    itemLine->setMinimumHeight(30);
    lay->addWidget(itemLine);

    SearchWid->setLayout(lay);
    SearchWid->setContentsMargins(0,0,0,0);

    ui->bomadditemlay->addWidget(SearchWid);
    ui->salesLinne->setValidator(new QDoubleValidator(0,9999,3,this));
    ui->purchLine->setValidator(new QDoubleValidator(0,9999,3,this));


    QObject::connect(itemLine,SIGNAL(SelectedItemID(inventoryItemDataModel)),
                     this,SLOT(addNewItemBySearch(inventoryItemDataModel)));



}
void AddItems::addNewItemBySearch(inventoryItemDataModel item){

    tempBomItem = item.ItemID;
    tempBomItemObj = dbHelper->getInventoryItemByID(item.ItemID);

    //    ui->uomLineEditq->setText(item.uomObject.UomName);

    //    ui->uomComboBox->setCurrentText(item.UOMName);
    qDebug()<<"Added item "<<item.ItemName<<item.ItemID;

    qDebug()<<"Item id:"<<item.ItemID;
    UomConv=uomHelper->getUomByItem(tempBomItemObj.ItemID);
    for(UomConversionDataModel a:UomConv){
        ui->purchUnitComboBox->addItem(uomHelper->getUOMNameById(a.ToUnit), QVariant::fromValue(a));

        int index = ui->purchUnitComboBox->findText(tempBomItemObj.uomObject.UomName);
        if(index == -1){
            qDebug()<<"UOM EMPTY";
            qDebug()<<tempBomItemObj.uomObject.UomName;
        }
        qDebug()<<"Index from find"<<index;
        ui->purchUnitComboBox->setCurrentIndex(index);
        qDebug()<<"INdex set";
        qDebug()<<"current index"<<ui->purchUnitComboBox->currentIndex();
        UomConversionDataModel uomObj = ui->purchUnitComboBox->currentData().value<UomConversionDataModel>();


        qDebug()<<"UOM Base Unit"<<uomObj._id;

        convalue = uomObj.conValue;
        qDebug()<<"Con value"<<convalue;
        uomId = uomObj.ToUnit;
        qDebug()<<"Set UOM ID"<<uomId;

        QObject::connect(ui->purchUnitComboBox, SIGNAL(currentIndexChanged(int)),
                         this, SLOT(uomIndexChanged(int)));

    }
}

void AddItems::uomIndexChanged(int index)
{
    qDebug()<<"UOM Index changed"<<index;
    UomConversionDataModel uomObj = ui->purchUnitComboBox->currentData().value<UomConversionDataModel>();

    convalue = uomObj.conValue;
    uomId = uomObj.ToUnit;

    qDebug()<<"#######################Set UOM ID"<<uomId<<convalue;

    qDebug()<<"Qty:"<<tempBomItemObj.quantity;

    ui->purchLine->setText(QString::number(tempBomItemObj.quantity/convalue,'f',3));
}


void AddItems::setBOMTable(){

    qDebug()<<"Set BOM table called";
    int cnt =0;
    qDebug()<<"Setting BO table with cnt : "<<boms.size();
    float totalVal=0;

    ui->BOMTableWidget->setRowCount(boms.size());
    for(int i=0;i<boms.size();i++){
        cnt =0;
        qDebug()<<"Showing item id :"<<boms[i].PurchaseItem;


        inventoryItemDataModel item = dbHelper->getInventoryItemByID(boms[i].PurchaseItem);
        qDebug()<<"Showing item name "<<item.ItemName;

        QTableWidgetItem* itemname = new QTableWidgetItem;
        itemname->setText(item.ItemName);
        itemname->setFlags(itemname->flags() ^ Qt::ItemIsEditable);

        ui->BOMTableWidget->setItem(i,0,itemname);


        //        QTableWidgetItem* uom = new QTableWidgetItem;
        //        uom->setText(item.uomObject.UomName);
        //        ui->BOMTableWidget->setItem(i,1,uom);

        QTableWidgetItem* purchuom = new QTableWidgetItem;
        purchuom->setText(uomHelper->getUOMNameById(boms[i].PurchaseUom));
        ui->BOMTableWidget->setItem(i,1,purchuom);


        QTableWidgetItem* pqty = new QTableWidgetItem;
        qDebug()<<"purch qty:"<<boms[i].PurchaseItemQty<<boms[i].conValue;
        pqty->setText(QString::number(boms[i].PurchaseItemQty/boms[i].conValue,'f',3));
        ui->BOMTableWidget->setItem(i,2,pqty);


        QTableWidgetItem* sqty= new QTableWidgetItem;
        sqty->setText(QString::number(boms[i].SalesItemQty));
        ui->BOMTableWidget->setItem(i,3,sqty);

        QCheckBox* chk  = new QCheckBox;
        chk->setChecked(boms[i].TakeAway);
        ui->BOMTableWidget->setCellWidget(i,4,chk);
        chk->setProperty("row",i);
        QObject::connect(chk,SIGNAL(toggled(bool)),this,SLOT(changeCHK(bool)));


        QToolButton* delButton = new QToolButton;
        delButton->setIcon(QIcon(":/icons/trash.ico"));
        delButton->setProperty("row",i);
        QObject::connect(delButton,SIGNAL(clicked(bool)),this,SLOT(deletebom(bool)));
        ui->BOMTableWidget->setCellWidget(i,5,delButton);

        totalVal += dbHelper->getPrice(boms[i].PurchaseItem) * boms[i].PurchaseItemQty / boms[i].SalesItemQty;

        cnt++;
    }
    ui->itemCostByStdCostLineEdit->setText(QString::number(totalVal,'f',2));
}
void AddItems::changeCHK(bool val){
    int i = sender()->property("row").toInt();
    boms[i].TakeAway = val;

}

void AddItems::deletebom(bool val){
    int i = sender()->property("row").toInt();
    boms.removeAt(i);
    ui->BOMTableWidget->removeRow(i);
}


void AddItems::on_AddBomButton_clicked()
{
    qDebug()<<"Add bom clicked "<<tempBomItem.length();
    if(tempBomItem.length()>1){
        RestaurantIngredientsDataModel line;


        //        UomConversionDataModel uomObj = ui->uomComboBox->currentData().value<UomConversionDataModel>();

        //        float convalue = uomObj.conValue;
        line.SalesItem = id;
        line.PurchaseItem = tempBomItem;
        line.SalesItemQty = ui->salesLinne->text().toFloat();
        line.PurchaseItemQty = ui->purchLine->text().toFloat() * convalue;
        line.PurchaseUom = uomId;
        line.TakeAway = ui->TakeAwaycheckBox->isChecked();
        line.conValue = convalue;

        boms.append(line);
        setBOMTable();
        tempBomItem = "";
        ui->salesLinne->setText("");
        ui->purchLine->setText("");
        itemLine->clear();
        //        ui->uomComboBox->clear();
        ui->uomLineEditq->clear();
        ui->purchUnitComboBox->clear();

        ui->TakeAwaycheckBox->setChecked(false);

    }
}


void AddItems::setBOMItem(inventoryItemDataModel item){

    int currRow = ui->BOMTableWidget->currentRow();

}

void AddItems::setupImagesTab()
{


}

void AddItems::setupBundleTab()
{

    if(ui->isBundled->isChecked() == true){
        //Create Model for BundleTableView
        //Create Delegate

    }

}

void AddItems::setupPriceListTab()
{
    //    priceModel = priceHelper->getAllPricesOfItem(invItem->ItemID);
}

void AddItems::setupRestaurantTab()
{
    QStringList AvailablePrintersList;
    AvailablePrintersList= QPrinterInfo::availablePrinterNames();

    int i=0;
    ui->KOTPrintersTableWidget->setColumnCount(2);

    QTableWidgetItem* Printer = new QTableWidgetItem;
    Printer->setText("Printer");
    ui->KOTPrintersTableWidget->setHorizontalHeaderItem(i++,Printer);

    QTableWidgetItem* Select = new QTableWidgetItem;
    Select->setText("Select");
    ui->KOTPrintersTableWidget->setHorizontalHeaderItem(i++,Select);

    ui->KOTPrintersTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->KOTPrintersTableWidget->setShowGrid(false);

    int rowCount =0;

    ui->KOTPrintersTableWidget->setRowCount(0);
    ui->KOTPrintersTableWidget->setRowCount(AvailablePrintersList.size());
    for(int i =0;i<AvailablePrintersList.size();i++){
        ui->KOTPrintersTableWidget->setRowHeight(rowCount,40);
        QTableWidgetItem* PrinterName = new QTableWidgetItem;
        QCheckBox* checkBox = new QCheckBox;

        PrinterName->setText(AvailablePrintersList[rowCount]);

        ui->KOTPrintersTableWidget->setItem(i,0,PrinterName);
        ui->KOTPrintersTableWidget->setCellWidget(i,1,checkBox);
        rowCount++;
    }

}

void AddItems::editRestaurantTab(QString KOTPrinters)
{
    QStringList AvailablePrintersList;
    AvailablePrintersList= QPrinterInfo::availablePrinterNames();

    int i=0;
    ui->KOTPrintersTableWidget->setColumnCount(2);

    QTableWidgetItem* Printer = new QTableWidgetItem;
    Printer->setText("Printer");
    ui->KOTPrintersTableWidget->setHorizontalHeaderItem(i++,Printer);

    QTableWidgetItem* Select = new QTableWidgetItem;
    Select->setText("Select");
    ui->KOTPrintersTableWidget->setHorizontalHeaderItem(i++,Select);

    ui->KOTPrintersTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->KOTPrintersTableWidget->setShowGrid(false);

    int rowCount =0;

    ui->KOTPrintersTableWidget->setRowCount(0);
    qDebug()<<"KOT Printers : "<<KOTPrinters;
    ui->KOTPrintersTableWidget->setRowCount(AvailablePrintersList.size());
    for(int i =0;i<AvailablePrintersList.size();i++){
        ui->KOTPrintersTableWidget->setRowHeight(rowCount,40);
        QTableWidgetItem* PrinterName = new QTableWidgetItem;
        QCheckBox* checkBox = new QCheckBox;

        PrinterName->setText(AvailablePrintersList[rowCount]);
        if(KOTPrinters.contains("|"+AvailablePrintersList[rowCount]+"|",Qt::CaseInsensitive)){
            checkBox->setChecked(true);
        }

        ui->KOTPrintersTableWidget->setItem(i,0,PrinterName);
        ui->KOTPrintersTableWidget->setCellWidget(i,1,checkBox);
        rowCount++;
    }

    ui->expDate->setDate(ui->mfdDate->date().addDays(invItem->shelfLife));
}

void AddItems::setUOMTable()
{

    //    UomDataBaseHelper *uomDbHelper = new UomDataBaseHelper();

    qDebug()<<"setUOMTable"<<id;

    model = uomHelper->getUomConversionByItem(id);


    ui->UomConversionTableView->setModel(model);
    ui->UomConversionTableView->setSortingEnabled(true);
    ui->UomConversionTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->UomConversionTableView->hideColumn(0);

    if(model->rowCount()>0){
        ui->baseUnitOfMeasurementComboBox->setEditable(false);
        ui->baseUnitOfMeasurementComboBox->setEnabled(false);
    }
}


void AddItems::edit()
{
    ui->deletePushButton->show();

    ui->titleLabel->setText(invItem->ItemName);
    id = invItem->ItemID;
    qDebug()<<"****************closing stock*****************"<<invItem->ClosingStock;
    ui->nameLineEdit->setText(invItem->ItemName);
    ui->arabicNameLineEdit->setText(invItem->itemNameArabic);
    ui->codeLineEdit->setText(invItem->ItemCode);
    ui->aliasLineEdit->setText(invItem->ItemAlias);
    invItem->GroupName = itemGroupHelper->getGroupNameByID(invItem->GroupID);

    ui->groupNameComboBox->setCurrentText(invItem->GroupName);
    ui->sectionComboBox->setCurrentText(godownHelper->getGodownNameById(invItem->section));
    ui->categoryComboBox->setCurrentText(categoryHelper->getCategoryNameByID(invItem->Category.toInt()));
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    ui->warrantyPeriodLineEdit->setText(QString::number(invItem->warrantyDays));
    ui->shelfLifeLineEdit->setText(QString::number(invItem->shelfLife,'f',2));
    ui->narrationLineEdit->setText(invItem->narration);
    ui->priceLineEdit->setText(QString::number(invItem->price,'f',2));
    ui->nettPrice->setText(QString::number(invItem->price*(1+invItem->taxRate/100),'f',2));
    ui->onlinePriceLine->setText(QString::number(invItem->price_2,'f',2));
    ui->reorderLevelLineEdit->setText(QString::number(invItem->ReorderLevel,'f',2));
    ui->gstComboBox->setCurrentText(QString::number(invItem->taxRate,'f',2));
    qDebug()<<Q_FUNC_INFO<<__LINE__;

    //    qDebug()<<"&& GST RAte"<<invItem->taxRate<<ui->gstComboBox->currentText();


    defaultLedgerSearch->setText(ledgerHelper->getLedgerNameByID(invItem->defaultLedgerId));
    ui->defaultPurchaseLedgerComboBox->setCurrentText(ledgerHelper->getLedgerNameByID(invItem->defaultPurchaseLedgerID));
    ui->defaultSalesLedgerComboBox->setCurrentText(ledgerHelper->getLedgerNameByID(invItem->defaultSalesLedgerID));

    ui->baseUnitOfMeasurementComboBox->setCurrentText(invItem->uomObject.UomName);
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    ui->openingBalanceLineEdit->setText(QString::number(invItem->OpeningStock,'f',2));
    ui->openingBalanceValueLineEdit->setText(QString::number(invItem->OpeningStockValue,'f',2));

    ui->partNoLineEdit->setText(invItem->PartNumber);
    ui->brandsComboBox->setCurrentText(invItem->brandName);
    qDebug()<<Q_FUNC_INFO<<__LINE__<<invItem->ItemDescription;
    ui->descriptionEdit->setPlainText(invItem->ItemDescription);
    ui->customItemCheckBox->setChecked(invItem->isCustomItem);
    ui->dimensionEdit->setText(invItem->Dimension);

    qDebug()<<"PLU : "<<invItem->SerailNumber;
    ui->SerialLineEdit->setText(invItem->SerailNumber);

    ui->defaultInputTaxLedgerComboBox ->setCurrentText(ledgerHelper->getLedgerNameByID(invItem->DefaultInputTaxLedgerID));
    ui->defaultOutputTaxLedgerComboBox->setCurrentText(ledgerHelper->getLedgerNameByID(invItem->DefaultOutputTaxLedgerID));
    ui->defaultSalesReturnLedgerComboBox->setCurrentText(ledgerHelper->getLedgerNameByID(invItem->DefaultSalesReturnLedgerID));
    ui->defaultPurchaseReturnLedgerComboBox->setCurrentText(ledgerHelper->getLedgerNameByID(invItem->DefaultPurchaseReturnLedgerID));

    ui->dimLabel->setHidden(!invItem->isCustomItem);
    ui->dimensionEdit->setHidden(!invItem->isCustomItem);
    //    ui->pluCheck->setChecked(invItem->SerailNumber.length()>0?true:false);
    ui->stdCostLineEdit->setText(QString::number(invItem->stdCost,'f',2));

    ui->AddBomButton->setEnabled(true);
    ui->FavoCheckBox->setChecked(invItem->favo);
    ui->isStockItem->setChecked(invItem->isStockItem);
    ui->isSerialNumbered->setChecked(invItem->isSerailNumbered);
    ui->isBatchProcessed->setChecked(invItem->isBatchProcessed);
    ui->isBundled->setChecked(invItem->isBundled);
    ui->hideInBill->setChecked(invItem->hideInPrint);

    ui->purchaseItemCheckBox->setChecked(invItem->isPurchaseItem);
    ui->salesItemCheckBox->setChecked(invItem->isSalesItem);
    ui->fromTimeTimeEdit->setTime(invItem->fromTime);
    ui->toTimeTimeEdit->setTime(invItem->toTime);

    ui->hSNCodeLineEdit->setText(invItem->hsnCode);


    editProperties(id);
    editUOM(id);
    editBOM(id);
    editImages(id);
    editRestaurantTab(invItem->KOTPrinter);
    editBundle(id);

    priceListWidget->setItemID(invItem->ItemID, invItem->uomObject._id);
    priceListWidget->setTable();

}

void AddItems::editProperties(QString itemID)
{
    qDebug()<<"edit properties";
    QList<PropertyDataModel> props = dbHelper->getAllPropertiesOfItem(itemID);
    qDebug()<<"edit properties"<<props.size();

    ui->tableWidgetPropertis->setRowCount(0);
    //    ui->tabWidget->setTabEnabled(4,false);
    int r=0;
    for(PropertyDataModel prop:props){
        ui->tableWidgetPropertis->insertRow(r);
        QTableWidgetItem* GroupName = new QTableWidgetItem;
        GroupName->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

        GroupName->setText(prop.PropertyName);

        ui->tableWidgetPropertis->setItem(r,0,GroupName);

        QTableWidgetItem *delButton = new QTableWidgetItem;
        delButton->setIcon(QIcon(":/icons/trash.ico"));

        ui->tableWidgetPropertis->setItem(r,1,delButton);
        r++;
    }
}

void AddItems::editUOM(QString itemID)
{
    setUOMTable();
}

void AddItems::editImages(QString itemID)
{
    GMFileUploadHelper *uploader = new GMFileUploadHelper("inv_items/"+invItem->ItemID,"xxx.jpg");
    connect(uploader,&GMFileUploadHelper::downLoaded,this,[=](QByteArray data){
        qDebug()<<data.size();
        QPixmap pm;
        pm.loadFromData(data);
        ui->imageLabel->setPixmap(pm.scaled(300,300,Qt::KeepAspectRatio,Qt::SmoothTransformation));
        ui->imageLabel->setScaledContents(true);
        ui->imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        ui->imageLabel->setFixedSize(300,300);
    });

    uploader->readFile("inv_items/"+itemID+".png");

    return;
    ItemImageDatamodel imageData1;
    imageData1.itemID=itemID;
    imageData = imagesHelper->getItemImageAsQimage(imageData1);
    int width = imageData.imageWidth;
    int height = imageData.imageHeight;

    ui->imageWidth->setValue(width);
    ui->imageHeight->setValue(height);
    if(imageData.img.size()>0){
        QPixmap pic;
        pic.loadFromData( imageData.img);

        // Show the image into a QLabel object
        ui->picture->setPixmap(pic.scaled(width,height) );
        //ui->imglbl_2->setScaledContents(true);
    }

}


void AddItems::editBOM(QString itemID)
{
    if(invItem->fromExternal)
        boms = invItem->bomList;
    else
        boms =  BillOfMaterialsIngredientsDataBaseHelper::getBillOfMaterials(itemID);
    qDebug()<<"BOm Count = "<<boms.size();
    setBOMTable();

    //    ui->BOMTableWidget->setRowCount(0);
    //    for(){

    //    }
}

void AddItems::editBundle(QString itemID)
{

    QList<inventoryItemDataModel> freeList = bundleHelper-> getBundledItemsMaster(itemID);

    bundleTable = new ItemBundleTableView();
    bundleTable->setParent(this);
    bundleModel = new ItemBundleListModel(freeList);
    bundleTable->setModel(bundleModel);
    bundleTable->setItemDelegate(new ItemBundleListDelegate(dbHelper,dbHelper->getInventoryItemsQueryModelForSearch()));
    bundleTable->setColumnWidth(0,25);
    bundleTable->setColumnWidth(3,25);
    bundleTable->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->verticalLayout_15->addWidget(bundleTable);
    connect(bundleModel,&ItemBundleListModel::newLineAdded,bundleTable,ItemBundleTableView::setFocusAfterInsert);
}

void AddItems::addPropertyToTable()
{

    QString propName = ui->propertyComboBox->currentText();
    if(propName.length()<1)
        return;
    int r= ui->tableWidgetPropertis->rowCount();
    ui->tableWidgetPropertis->insertRow(r);

    QTableWidgetItem* GroupName = new QTableWidgetItem;
    GroupName->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

    GroupName->setText(propName);

    ui->tableWidgetPropertis->setItem(r,0,GroupName);

    QTableWidgetItem *delButton = new QTableWidgetItem;
    delButton->setIcon(QIcon(":/icons/trash.ico"));
    ui->tableWidgetPropertis->setItem(r,1,delButton);
}

void AddItems::delPropPressed(int rc,int c)
{
    //int r = ui->tableWidgetPropertis->currentRow();
    qDebug()<<rc<<c;
    if(c==1){
        qDebug()<<"Row to delete "<<rc;
        ui->tableWidgetPropertis->removeRow(rc);
    }
}

void AddItems::setTitle(QString text)
{
    ui->titleLabel->setText(text);
}

void AddItems::setItemCode(QString text)
{
    ui->codeLineEdit->setText(text);
}

void AddItems::keyPressEvent(QKeyEvent *e)
{
    if ((e->key() == Qt::Key_F)  && (e->modifiers().testFlag(Qt::ControlModifier)))
    {}
    else
    {}
}

void AddItems::on_imageResizeButton_clicked()
{
    imageData.imageWidth = ui->imageWidth->value();
    imageData.imageHeight = ui->imageHeight->value();

    QPixmap pic;
    pic.loadFromData( imageData.img);

    // Show the image into a QLabel object
    ui->picture->setPixmap(pic.scaled(imageData.imageWidth,imageData.imageHeight) );
    //ui->imglbl_2->setScaledContents(true);

}

void AddItems::on_customItemCheckBox_clicked()
{
    //    ui->serialLabel->setHidden(!ui->customItemCheckBox->isChecked());
    //    ui->SerialLineEdit->setHidden(!ui->customItemCheckBox->isChecked());
}

void AddItems::sendNotifications(QStringList dbNameList)
{
    for(QString dbName:dbNameList){
        //        qDebug()<<"Insert notification data to send"<<dbName;
        compItem->BaseItem.bomList = boms;
        NotificationDataModel *notification = new NotificationDataModel();
        QJsonDocument doc(compItem->ObjPtrToJson(compItem));
        QString strJson(doc.toJson(QJsonDocument::Compact));

        notification->NotificationData = strJson;
        notification->ToDBName=dbName;
        notification->TypeOfData="Inventory Item";
        notification->Action = compItem->Action;
        notification->GeneratedTransactionId = compItem->BaseItem.ItemID;
        //QString::number(QDateTime::currentDateTime().toTime_t()) + LoginValues::voucherPrefix + DatabaseHelper::clientId;

        sendChannelDatabaseHelper *sendHelper = new sendChannelDatabaseHelper();
        sendHelper->insertNotification(notification);
    }

    this->close();

    emit closing();
    emit savedItem(*compItem);
}


void AddItems::on_addUomPushButton_clicked()
{
    addUOMCompoundWidget = new addConversion(this);
    addUOMCompoundWidget ->setWindowFlags(Qt::Window|Qt::WindowStaysOnTopHint);
    addUOMCompoundWidget->setBaseUnit(ui->baseUnitOfMeasurementComboBox->currentData().toInt(), id, invItem->ItemCode);
    addUOMCompoundWidget->show();

    QObject::connect(addUOMCompoundWidget,SIGNAL(closing()),
                     this,SLOT(setUOMTable()));

}

void AddItems::on_UomConversionTableView_doubleClicked(const QModelIndex &index)
{
    qDebug()<<"Conversion item double clicked";
    int r = index.row();
    QString convId = model->record(r).value(0).toString();

    uomConvObj = new UomConversionDataModel();
    uomConvObj = uomHelper->getUOMConversionById(convId);
    //    if(uomConvObj->barCode.length() == 0)
    //        uomConvObj->barCode = invItem->ItemCode + "0" + uomConvObj->ToUnit;

    addUOMCompoundWidget = new addConversion(this);
    addUOMCompoundWidget ->setWindowFlags(Qt::Window|Qt::WindowStaysOnTopHint);
    addUOMCompoundWidget->setData(uomConvObj, ui->codeLineEdit->text());
    //    addUOMCompoundWidget->showDeleteButton(true);
    addUOMCompoundWidget->show();

    QObject::connect(addUOMCompoundWidget,SIGNAL(closing()),
                     this,SLOT(setUOMTable()));
    QObject::connect(addUOMCompoundWidget,SIGNAL(deleteUOMConv(UomConversionDataModel*)),
                     this,SLOT(deleteUOMConvSlot(UomConversionDataModel*)));


}

void AddItems::deleteUOMConvSlot(UomConversionDataModel* uomObj){

    //    uomHelper->deleteUOMConversion(uomObj->itemID);


}

void AddItems::on_codeLineEdit_returnPressed()
{
    if(dbHelper->barcodeExist(ui->codeLineEdit->text()) && ui->codeLineEdit->text().length()>0){
        //        QMessageBox box; box.setText("Barcode Already exist");box.exec();
        snackbar->addMessage("Barcode Already exist");
        ui->codeLineEdit->setText("");
    }
}



void AddItems::on_customItemCheckBox_toggled(bool checked)
{
    return;

}

void AddItems::on_pluCheck_toggled(bool checked)
{
    return;

}

void AddItems::on_printBarcodeButton_clicked()
{

    int n = ui->barcodeCopiesLineEdit->text().toInt();
    QDate Mfd=ui->mfdDate->date();
    QDate Exp=ui->expDate->date();

    if(invItem->ItemCode.length()<1)
        snackbar->addMessage("No Barcode Found");

    PrinterHelper phelper;
    if(ui->barcodeSize->isChecked())
        phelper.printBarcode3Lines(n/2,*invItem,Mfd,Exp,ui->withMFD->isChecked());
    else
    {
        if(ui->withMFD->isChecked()){
            phelper.printBarcode(n/2,*invItem,Mfd,Exp);
        }
        else
            phelper.printBarcodeAgro(n/2,*invItem,Mfd,Exp);
    }
}


void AddItems::on_SerialLineEdit_returnPressed()
{
    if(dbHelper->SerialExist(ui->SerialLineEdit->text()) && ui->SerialLineEdit->text().length()>0){
        snackbar->addMessage("PLU Already Exist");
        //        QMessageBox *box= new QMessageBox(this); box->setText("PLU Already Exist"); box->show();
    }
}

void AddItems::on_openingBalanceValueLineEdit_returnPressed()
{
    float cost = ui->openingBalanceLineEdit->text().toFloat()/ ui->openingBalanceValueLineEdit->text().toFloat();
    ui->stdCostLineEdit->setText(QString::number(cost,'f',2));
}

void AddItems::on_geeneratePLU_clicked()
{
    QString PLU = dbHelper->getMaxCode();
    ui->SerialLineEdit->setText(PLU);
    ui->codeLineEdit->setText(QString::number(PLU.toInt()+0000));
}

void AddItems::on_priceLineEdit_returnPressed()
{
    qDebug()<<"Ret Pressed";
    ui->nettPrice->blockSignals(true);
    auto tax =  ui->gstComboBox->currentText().toFloat();
    float val = ui->priceLineEdit->text().toFloat() *(1 + tax/100);
    ui->nettPrice->setText(QString::number(val,'f',2));
    qDebug()<<"val = "<<val;
    ui->nettPrice->blockSignals(false);
}

void AddItems::on_nettPrice_returnPressed()
{
    ui->priceLineEdit->blockSignals(true);
    auto tax =  ui->gstComboBox->currentText().toFloat();
    float val = ui->nettPrice->text().toFloat() * 100 / (100 + tax);
    ui->priceLineEdit->setText(QString::number(val,'f',2));
    ui->priceLineEdit->blockSignals(false);
}

void AddItems::on_priceLineEdit_editingFinished()
{
    qDebug()<<"Ret Pressed";
    ui->nettPrice->blockSignals(true);
    auto tax =  ui->gstComboBox->currentText().toFloat();
    float val = ui->priceLineEdit->text().toFloat() *(1 + tax/100);
    ui->nettPrice->setText(QString::number(val,'f',2));
    qDebug()<<"val = "<<val;
    ui->nettPrice->blockSignals(false);
}

void AddItems::on_deletePushButton_clicked()
{
//    if(dbHelper->checkItemTransactionExists(id)){
//        QMessageBox box; box.setText("Transactions exist for this item. Item cannot be deleted!");
//        box.setWindowModality(Qt::ApplicationModal);
//        box.exec();
//    }
//    else{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Delete Item","Would You Like to Delete?",
                                  QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::Yes){

        if(dbHelper->deleteSalesInventoryItem(id)){
            bomHelper->deleteBillOfMaterials(id);
            uomHelper->deleteUOMConversion(id);
            PricelistDatabaseHelper *priceHelper = new PricelistDatabaseHelper();
            priceHelper->deletePricesByItem(id);
        }

        emit closing();
        this->close();
    }

}

void AddItems::on_nettPrice_editingFinished()
{
    ui->priceLineEdit->blockSignals(true);
    auto tax =  ui->gstComboBox->currentText().toFloat();
    float val = ui->nettPrice->text().toFloat() * 100 / (100 + tax);
    ui->priceLineEdit->setText(QString::number(val,'f',2));
    ui->priceLineEdit->blockSignals(false);
}

