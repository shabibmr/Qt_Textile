#include "additemwisesalesorder.h"
#include "ui_additemwisesalesorder.h"

#include "sharetools/print/printdotmatrix.h"
#include <QLabel>

AddItemwiseSalesOrder::AddItemwiseSalesOrder(QString voucherItemwiseType,QString vType, VoucherDBAbstract *dbHelper,ItemwiseSalesOrderDatabaseHelper *db1Helper, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddItemwiseSalesOrder){
    ui->setupUi(this);
    qDebug()<<"1st line";

    this->voucherType = vType;
    this->voucherItemwiseType = voucherItemwiseType;
    this->dbHelper = dbHelper;
    this->db1Helper = db1Helper;

    this->setObjectName("editor");
    this->setStyleSheet("QWidget#editor{background-color:" + backgroundColor + "}");
    QPalette pal = palette();

    //    this->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, this->size(),
    //                                          qApp->primaryScreen()->geometry()));
    // set black background
    qDebug()<<"23 line";
    pal.setColor(QPalette::Background,QColor(backgroundColor));
    setAutoFillBackground(true);
    setPalette(pal);

    qDebug()<<"Init DB HELP";
    initDbHelpers();
     qDebug()<<"Init Vouc";
    initVoucher();
     qDebug()<<"Init Glob";
    initGlobalObjects();
     qDebug()<<"Init Wid";
    setWidgets();
     qDebug()<<"Init xtra";
    initExtraActions();
    qDebug()<<"Cons item wise Complets";
    this->setWindowTitle(windowTitle);
}

void AddItemwiseSalesOrder::showShareOptions()
{
    shareOptionsWidget->move(shareButton->pos().x(), shareButton->pos().y() + 90);
    shareOptionsWidget->show();
}

AddItemwiseSalesOrder::~AddItemwiseSalesOrder(){
    delete ui;
}

void AddItemwiseSalesOrder::setWindowTitle(const QString &value)
{
    windowTitle = value;
    windowLabel->setText(windowTitle);

}

void AddItemwiseSalesOrder::initExtraActions()
{
    //    qDebug()<<Q_FUNC_INFO;

    saveButton = new QtMaterialFloatingActionButton(QtMaterialTheme::icon("content", "save"));
    saveButton->setMini(true);
    saveButton->setXOffset(10);
    saveButton->setYOffset(10);
    saveButton->setParent(this);
    saveButton->setToolTip("Save Voucher");

    shareButton = new QtMaterialIconButton(QtMaterialTheme::icon("social", "share"));
    shareButton->setParent(appBar);
    shareButton->setColor(QColor(255,250,0));
    shareButton->setIconSize(QSize(30,30));

    shareOptionsWidget = new ShareTools("Voucher", shareButton);
    shareOptionsWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup);

    appBar->appBarLayout()->addWidget(shareButton);

    QObject::connect(shareButton,&QPushButton::clicked,
                     this,&AddItemwiseSalesOrder::showShareOptions);
    QObject::connect(shareOptionsWidget,&ShareTools::printClicked,
                     this,&AddItemwiseSalesOrder::printVoucher);
    QObject::connect(shareOptionsWidget,&ShareTools::pdfClicked,
                     this,&AddItemwiseSalesOrder::printVoucher);

    QObject::connect(saveButton,&QPushButton::clicked,
                     this,&AddItemwiseSalesOrder::saveVoucher);

}

void AddItemwiseSalesOrder::initDbHelpers()
{
    // Objects init

    ledHelper = new LedgerMasterDatabaseHelper;
    uomHelper = new UomDataBaseHelper;
    salesmanHelper = new UserProfileDatabaseHelper;
    itemHelper = new SalesInventoryItemDatabaseHelper;
//    dbHelper = new SalesOrderDatabaseHelper;
//    db1Helper = new ItemwiseSalesOrderDatabaseHelper;

    priceHelper = new PricelistDatabaseHelper;
    accHelper = new AccountGroupMasterDatabaseHelper;
}

void AddItemwiseSalesOrder::initVoucher()
{
    voucher = new GeneralVoucherDataObject();
    voucher->status = QuotationStatus::Started;
    voucher->AddedById = LoginValues::userID;
    voucher->AddedBy = LoginValues::empName;
    voucher->VoucherDate = QDate::currentDate();
    voucher->DateCreated = QDate::currentDate();
    voucher->VoucherPrefix = LoginValues::voucherPrefix;
    voucher->voucherType = voucherItemwiseType;
    voucher->voucherNumber = GeneralVoucherDatabaseHelper::getNextVoucherNoByType
            (voucherItemwiseType,voucher->VoucherPrefix,ItemwiseSalesOrderDatabaseHelper::Order_Itemwise_TableName);
    voucher->status = QuotationStatus::Started;
    voucher->priceListName = priceHelper->getPriceListNameByID(priceHelper->getDefaultPriceList());

}

void AddItemwiseSalesOrder::initGlobalObjects()
{

    model = new GMItemwiseOrderModel(voucher,priceHelper);
    DataTableWidget = new ItemwiseEntryTableView(voucher);
    DataTableWidget->setModel(model);
    DataTableWidget->setColumns();
    DataTableWidget->setItemDelegate(new GMItemwiseOrdersDelegate(ledHelper,ledHelper->getAllLedgersModel(),
                                                                  uomHelper,itemHelper,
                                                                  itemHelper->getInventoryItemsQueryModelForSearch(),
                                                                  voucher->priceListId));
    //    qDebug()<<" Model Row cnt : "<<model->rowCount();

    DataTableWidget->hideColumn(7);
    voucherDateWidget = new GMDateEdit(&voucher->VoucherDate);
    voucherDateWidget->setFixedWidth(100);
    voucherDateWidget->setFrame(false);
    voucherDateWidget->setStyleSheet("GMDateEdit{background-color:" + backgroundColor + ";border: none; color: " + textColor + "} ""QDateEdit::down-arrow {image: url(:/icons/icons/navigation/svg/production/ic_arrow_drop_down_circle_24px.svg);height:15px;width:15px; }");
    voucherDateWidget->setFont(QFont("Calibri", 9, QFont::Normal));
    voucherNumberWidget = new GMLineEdit(&voucher->voucherNumber);
    voucherNumberWidget->setStyleSheet("GMLineEdit{background-color:" + backgroundColor + ";border: none; color: " + textColor + "}");
    voucherNumberWidget->setReadOnly(true);
    voucherNumberWidget->setFont(QFont("Calibri", 11, QFont::Normal));
    voucherNumberWidget->setAlignment(Qt::AlignLeft);

    voucherNarrationWidget = new GMPlainTextEdit(&voucher->narration);
    voucherNarrationWidget->setPlaceholderText("Narration");
    SalesmanSearchBox = new GMEmployeeLineMaterial(salesmanHelper,salesmanHelper->getEmployeeQueryForSearch());
    SalesmanSearchBox->setLabel("SalesMan Name");
    SalesmanSearchBox->setParent(this);
    SalesmanSearchBox->setFixedWidth(200);
    SalesmanSearchBox->setStyleSheet("GMEmployeeLineMaterial{background-color:" + backgroundColor + ";}");

    connect(SalesmanSearchBox,&GMEmployeeLineMaterial::itemSelectedwithItem,
            this,[=](UserProfileDataModel emp){
        voucher->SalesmanID = emp._id;
        DataTableWidget->setFocusToFirstRow(voucher->InventoryItems.length());
    });

}

void AddItemwiseSalesOrder::setWidgets()
{

    QStringList salesGroups;
    salesGroups.append(accHelper->getGroupIDByName("Cash-in-Hand"));
    salesGroups.append(accHelper->getGroupIDByName("Sundry Debtors"));
    salesGroups.append(accHelper->getGroupIDByName("Sundry Debtors"));
    salesGroups.append(accHelper->getGroupIDByName("Bank Accounts"));

    for(AccountGroupDataModel acc: accHelper->getAccountsUnderParentGroupRecursive(accHelper->getGroupIDByName("Sundry Creditors"))){
        salesGroups.append(acc.groupID);
    }

    for(AccountGroupDataModel acc: accHelper->getAccountsUnderParentGroupRecursive(accHelper->getGroupIDByName("Sundry Debtors"))){
        salesGroups.append(acc.groupID);
    }

    for(AccountGroupDataModel acc: accHelper->getAccountsUnderParentGroupRecursive(accHelper->getGroupIDByName("Bank Accounts"))){
        salesGroups.append(acc.groupID);
    }

    windowLabel = new QLabel(windowTitle);

    appBar = new QtMaterialAppBar();
    appBar->setBackgroundColor(QColor());

    backButton = new QtMaterialIconButton(QtMaterialTheme::icon("navigation", "arrow_back"));
    backButton->setParent(appBar);
    backButton->setColor(QColor(245,0,87));
    backButton->setIconSize(QSize(30,30));
    appBar->appBarLayout()->addWidget(backButton);
    connect(backButton, &QtMaterialIconButton::clicked, this, [=](){
        this->close();
    });

    QHBoxLayout *vNobox = new QHBoxLayout;
    vNobox->addWidget(new QLabel("Voucher No"));
    vNobox->addWidget(voucherNumberWidget);

    QHBoxLayout *vDatebox = new QHBoxLayout;
    vDatebox->addWidget(new QLabel("Voucher Date"));
    vDatebox->addWidget(voucherDateWidget);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addLayout(vNobox);
    vbox->addLayout(vDatebox);

    //    appBar->appBarLayout()->addLayout(vbox);
    appBar->appBarLayout()->addStretch(1);
    appBar->appBarLayout()->addWidget(windowLabel, 0, Qt::AlignCenter);
    appBar->appBarLayout()->addStretch(1);

    QPalette palette = windowLabel->palette();
    palette.setColor(windowLabel->foregroundRole(), Qt::white);
    windowLabel->setPalette(palette);

    windowLabel->setFont(QFont("Calibri", 18, QFont::Normal));

    QVBoxLayout *layout = new QVBoxLayout;
    QWidget *canvas = new QWidget;

    appBar->setMinimumHeight(50);
    appBar->setMaximumHeight(300);

    layout->setContentsMargins(0, 0, 0, 0);

    layout = new QVBoxLayout;
    canvas->setLayout(layout);
    canvas->setMaximumHeight(60);
    layout->addWidget(appBar);
    //    layout->addStretch(1);

    QPointer<QWidget> Widget = new QWidget(this);
    QPointer<QVBoxLayout> mainLayout = new QVBoxLayout(Widget);
    Widget->setLayout(mainLayout);

    QPointer<QHBoxLayout> voucherData = new QHBoxLayout;
    QPointer<QHBoxLayout> extrasLayout = new QHBoxLayout;
    QPointer<QVBoxLayout> tablesLayout = new QVBoxLayout;
    QPointer<QVBoxLayout> advLayout = new QVBoxLayout;

    //    voucherData->setContentsMargins(20,0,0,0);
    voucherData->addLayout(vbox);
    voucherData->addStretch(1);
    //    voucherData->addWidget(priceListWidget);
    voucherData->addWidget(SalesmanSearchBox);

    mainLayout->addWidget(canvas);
    mainLayout->addLayout(voucherData);
    mainLayout->addLayout(tablesLayout);
    mainLayout->addLayout(extrasLayout);
    mainLayout->addLayout(advLayout);
    mainLayout->setSpacing(7);

    tablesLayout->addWidget(DataTableWidget);
    extrasLayout->addWidget(voucherNarrationWidget,Qt::AlignLeft);

    //    voucherData->addWidget(new QLabel("Date"));
    //    voucherData->addWidget(voucherDateWidget);
    //    voucherData->addWidget(new QLabel("Voucher No"));
    //    voucherData->addWidget(voucherNumberWidget);

    //    qDebug()<<"242";
    ui->verticalLayout->addWidget(Widget);
    //    qDebug()<<"Line 247";

}

void AddItemwiseSalesOrder::resetWidgets()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    delete model;
    voucherDateWidget->setVariable(&voucher->VoucherDate);
    voucherNumberWidget->setVariable(&voucher->voucherNumber);
    voucherNarrationWidget->setVariable(&voucher->narration);
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    SalesmanSearchBox->setItem(salesmanHelper->getEmployeeByID(voucher->SalesmanID));
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    SalesmanSearchBox->clearFocus();
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    model = new GMItemwiseOrderModel(voucher,priceHelper,this);
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    DataTableWidget->setVoucher(voucher);
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    DataTableWidget->setModel(model);
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    DataTableWidget->setColumns();
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    DataTableWidget->setFocus();
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    DataTableWidget->setFocusAfterInsert(voucher->InventoryItems.length());
    qDebug()<<Q_FUNC_INFO<<__LINE__;
}


void AddItemwiseSalesOrder::saveVoucher(){

    if(voucher->InventoryItems.size()<1){
        return;
    }

    if(voucher->status == QuotationStatus::SalesOrderCreated){
        db1Helper->updateOrder(voucher,voucher->voucherNumber);
    }
    else{
        voucher->status = QuotationStatus::SalesOrderCreated;
        db1Helper->insertOrder(voucher);
    }

    //    if(!voucher->SalesmanID){

    //        return;
    //    }

    /*
       *
       * 1. loop thru
       * 2. read each default_sal_ledgerid
       * 3. fetch sales order of led if any ELSE create new s.o. voucher
       * 4. remove all inv[] where itemid = current itemid
       * 5. append all lines of current item to s.o (loop voucher) and remove index;
       * 6. insert or update s.o.
       */

    //    for(int i =0;i<voucher->InventoryItems.size();i++){
    //        for(int j=0;j<voucher->InventoryItems.size();j++){
    //            if(voucher->InventoryItems[i].BaseItem.defaultSalesLedgerID < voucher->InventoryItems[j].BaseItem.defaultSalesLedgerID){
    //                CompoundItemDataObject temp;
    //                temp = voucher->InventoryItems[i];
    //                voucher->InventoryItems[i] = voucher->InventoryItems[j];
    //                voucher->InventoryItems[j] = temp;
    //            }
    //        }
    //    }


    int cnt =1;
    for(int i=0;i<voucher->InventoryItems.size();i++){
        if(voucher->InventoryItems[i].BaseItem.length>0){
            voucher->InventoryItems[i].BaseItem.length = cnt++;
        }
        if(voucher->InventoryItems[i].BaseItem.bomList.size() > 0){
            if(voucher->InventoryItems[i].BaseItem.isSalesItem && voucher->InventoryItems[i].BaseItem.requestQty>0){
                if(voucher->InventoryItems[i].BaseItem.bomList[0].TakeAway){
                    CompoundItemDataObject c;
                    c.BaseItem = itemHelper->getInventoryItemByID(voucher->InventoryItems[i].BaseItem.bomList[0].PurchaseItem);
                    c.BaseItem.quantity = voucher->InventoryItems[i].BaseItem.requestQty;
                    c.BaseItem.Dimension = voucher->voucherNumber;
                    c.BaseItem.fromGodownID = voucher->fromGodownID;
                    c.BaseItem.manufactureDate = voucher->InventoryItems[i].BaseItem.manufactureDate;
                    c.BaseItem.narration = voucher->InventoryItems[i].BaseItem.ItemID;
                    c.BaseItem.defaultSalesLedgerID  = voucher->InventoryItems[i].BaseItem.defaultSalesLedgerID;
                    c.BaseItem.length = -1;
                    voucher->InventoryItems.insert(i+1,c);
                }
            }
        }
    }


    QStringList currVouchers = db1Helper->getVoucherswithOrderID(voucher->voucherNumber);
    qDebug()<<"Current  VOuchers : "<<currVouchers;
    QString currentItemID = voucher->InventoryItems[0].BaseItem.ItemID;
    qDebug()<<"LOOP BEGINS : "<<currentItemID;
    for(int i =0;i<voucher->InventoryItems.size();i++){
        qDebug()<<"At "<<i<<" Size is "<<voucher->InventoryItems.size();
        currentItemID = voucher->InventoryItems[i].BaseItem.ItemID;
        voucher->InventoryItems[i].BaseItem.Dimension = voucher->voucherNumber;
        QString LedId = voucher->InventoryItems[i].BaseItem.defaultSalesLedgerID;
        QString vNo = dbHelper->getVoucherNumberByLedID(LedId,voucher->VoucherDate);
        qDebug()<<"V no : "<<vNo <<" for item "<<i;
        if(vNo != "-1"){
            currVouchers.removeOne(vNo);
            qDebug()<<"Updating Old Voucher"<<ledHelper->getLedgerNameByID(LedId);
            GeneralVoucherDataObject *VoucherObj = dbHelper->getVoucherByVoucherNoPtr(vNo,LoginValues::voucherPrefix);
            for(int j=0;j<VoucherObj->InventoryItems.size();j++){
                if(VoucherObj->InventoryItems[j].BaseItem.Dimension == voucher->voucherNumber){
                    VoucherObj->InventoryItems.removeAt(j);
                    j--;
                }
            }
            VoucherObj->InventoryItems.append(voucher->InventoryItems[i]);
            voucher->InventoryItems.removeAt(i--);
            for(int k=i+1; k<voucher->InventoryItems.size(); k++){
                if( LedId == voucher->InventoryItems[k].BaseItem.defaultSalesLedgerID){
                    VoucherObj->InventoryItems.append(voucher->InventoryItems[k]);
                    voucher->InventoryItems.removeAt(k);
                    k--;
                }
            }
            qDebug()<<"For Led : "<< VoucherObj->ledgerObject.LedgerName;
            for(int i =0;i<VoucherObj->InventoryItems.length();i++){
                qDebug()<<i<<VoucherObj->InventoryItems[i].BaseItem.ItemName<<VoucherObj->InventoryItems[i].BaseItem.quantity<<" For "
                       <<VoucherObj->ledgerObject.LedgerName;
            }
            TransactionCalculator::VoucherCalculateSales(VoucherObj);
            dbHelper->updateVoucher(VoucherObj,VoucherObj->voucherNumber);
            qDebug()<<"Update COmplete"<<"List is left with "<<voucher->InventoryItems.size();
            qDebug()<<" i value is "<<i;
        }
        else{
            // This Means No Order of this led Created Yet!
            qDebug()<<"Creating New Voucher for "<<ledHelper->getLedgerNameByID(LedId);
            GeneralVoucherDataObject *VoucherObj = new GeneralVoucherDataObject;
            VoucherObj->ledgerObject = ledHelper->getLedgerObjectByID(LedId);
            VoucherObj->VoucherDate = voucher->VoucherDate;
            VoucherObj->SalesmanID = voucher->SalesmanID;
            VoucherObj->status = QuotationStatus::SalesOrderCreated;
            VoucherObj->AddedById = voucher->AddedById;
            VoucherObj->voucherType = voucherType;
            VoucherObj->VoucherPrefix = voucher->VoucherPrefix;
            VoucherObj->InventoryItems.append(voucher->InventoryItems[i]);
            voucher->InventoryItems.removeAt(i--);
            for(int x=i+1;x<voucher->InventoryItems.size();x++){
                if(voucher->InventoryItems[x].BaseItem.defaultSalesLedgerID == LedId)
                {
                    VoucherObj->InventoryItems.append(voucher->InventoryItems[x]);
                    voucher->InventoryItems.removeAt(x);
                    x--;
                }
            }
            TransactionCalculator::VoucherCalculateSales(VoucherObj);
            qDebug()<<"Order is new with "<<VoucherObj->InventoryItems.size()<<" items";
            VoucherObj->status = QuotationStatus::SalesOrderCreated;
            dbHelper->insertVoucher(VoucherObj);
        }
    }

    qDebug()<<"Current  VOuchers AFTER CHANGE : "<<currVouchers;

    for(int i =0;i<currVouchers.length();i++){
        GeneralVoucherDataObject *VoucherObj = dbHelper->getVoucherByVoucherNoPtr(currVouchers[i],LoginValues::voucherPrefix);
        for(int j=0;j<VoucherObj->InventoryItems.size();j++){
            if(VoucherObj->InventoryItems[j].BaseItem.Dimension == voucher->voucherNumber){
                VoucherObj->InventoryItems.removeAt(j);
                j--;
            }
        }
        if(VoucherObj->InventoryItems.length()>0){
            TransactionCalculator::VoucherCalculateSales(VoucherObj);
            dbHelper->updateVoucher(VoucherObj,VoucherObj->voucherNumber);
        }
        else{
            dbHelper->deleteVoucher(VoucherObj);
        }
    }
    emit closing();
    this->close();

}

//void AddItemwiseSalesOrder::on_buttonBox_rejected(){
//    this->close();
//}



void AddItemwiseSalesOrder::setVoucher(GeneralVoucherDataObject *obj){

    qDebug()<<Q_FUNC_INFO<<__LINE__;
    delete  voucher;
    voucher = new GeneralVoucherDataObject();
    voucher = obj;
    TransactionCalculator::VoucherCalculateSales(voucher);
    resetWidgets();

    qDebug()<<Q_FUNC_INFO<<__LINE__;



}

//void AddItemwiseSalesOrder::on_printButton_clicked()
//{
//    askForPrint();
//}

void AddItemwiseSalesOrder::askForPrint()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Print Voucher","Would You Like to Print?",
                                  QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::Yes){
        printVoucher();
    }
}

void AddItemwiseSalesOrder::printVoucher()
{
    PrintDotMatrix printer;
    printer.printSalesVoucherForItemwise(voucher);
}
