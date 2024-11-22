#include "stockrequesteditor.h"



StockRequestEditor::StockRequestEditor(VoucherDBAbstract *db, VoucherEditorBase *parent) :
    VoucherEditorBase(parent)
{

    this->dbHelper = db;
//    initDbHelpers();
    initVoucher();
    initGlobalObjects();
    setWidgets();
    initExtraActions();

}

StockRequestEditor::~StockRequestEditor()
{
}

void StockRequestEditor::initExtraActions()
{
    saveButton = new QtMaterialFloatingActionButton(QtMaterialTheme::icon("content", "save"));
    saveButton->setMini(true);
    saveButton->setXOffset(10);
    saveButton->setYOffset(10);
    saveButton->setParent(this);
    saveButton->setToolTip("Save Voucher");

//    shareButton = new QtMaterialFloatingActionButton(QtMaterialTheme::icon("social", "share"));
//    shareButton->setMini(true);
//    shareButton->setBackgroundColor(QColor(25,150,40));
//    shareButton->setCorner(Qt::TopRightCorner);
//    shareButton->setParent(this);
//    shareButton->setXOffset(45);
//    shareButton->setYOffset(5);
//    shareButton->hide();


    snackBar = new QtMaterialSnackbar(this);
    drawer = new QtMaterialDrawer(this);
    drawer->setClickOutsideToClose(true);
    drawer->setOverlayMode(true);


    QVBoxLayout *drawerLayout = new QVBoxLayout;
    drawer->setDrawerLayout(drawerLayout);

    printButton = new QtMaterialIconButton(QtMaterialTheme::icon("action", "print"));
    printButton->setColor(QColor(255,250,0));
    printButton->setIconSize(QSize(30,30));

    contactButton->hide();
    moreButton->hide();
    printButton->hide();



    //    printButton->set
//    shareButton->setBackgroundColor(QColor(255,250,0));
//    shareButton->setForegroundColor(QColor(0,250,0));
    appBar->appBarLayout()->addWidget(printButton);

    QObject::connect(backButton,&QPushButton::clicked,
                           this,&StockRequestEditor::closeEditor);
    QObject::connect(saveButton,&QPushButton::clicked,
                           this,&StockRequestEditor::saveVoucher);


    QObject::connect(printButton,&QPushButton::clicked,
                           this,&StockRequestEditor::askForPrint);
}

void StockRequestEditor::initDbHelpers()
{
    ledgerHelper = new LedgerMasterDatabaseHelper;
    dbHelper = new StockRequestDatabaseHelper;
    uomHelper = new UomDataBaseHelper;
    userHelper = new UserProfileDatabaseHelper;
    itemsHelper = new SalesInventoryItemDatabaseHelper;
}

void StockRequestEditor::initVoucher()
{
    editorStatus= EditorStatus::NewVoucher;

    voucher = new GeneralVoucherDataObject();
    voucher->VoucherDate = QDate::currentDate();
    voucher->VoucherPrefix = LoginValues::voucherPrefix;
    voucher->voucherType = GMVoucherTypes::getVoucherString(GMVoucherTypes::IndentRequest);
//    voucher->voucherType = "Stock Request";
    voucher->voucherNumber = dbHelper->getNextVoucherNo(voucher->VoucherPrefix);
    voucher->status = QuotationStatus::Started;

    qDebug()<<"voucher"<<voucher->voucherType;

    setTitle(GMVoucherTypes::getVoucherTitle(GMVoucherTypes::IndentRequest));
    voucherDateWidget->setVariable(&voucher->VoucherDate);
    voucherNumberWidget->setVariable(&voucher->voucherNumber);

}

void StockRequestEditor::initGlobalObjects()
{

    lis<<"#"<<"Particulars"<<"UOM"<<"Calculated"<<"Requested"<<"Issued";

    DataTableWidget = new StockEntryTableView(voucher);
//    DataTableWidget->setEnabled(false);
    DataTableWidget->setItemDelegate(new StockRequestDelegate(
                              itemsHelper,itemsHelper->getInventoryItemsQueryModel(2),
                                         uomHelper));

    model = new GMStockRequestModel(voucher,lis.size(),1,this);
    model->setHeaderStrings(lis);

    DataTableWidget->setModel(model);
    DataTableWidget->setColumns();

    setTableLayout(DataTableWidget);

//    DataTableWidget->hideColumn(2);

    voucher->fromGodownID="Godown";
    fromGodownWidget = new GMComboBox(&voucher->fromGodownID);
    fromGodownWidget->getGodownList();
    fromGodownWidget->setCurrentIndex(fromGodownWidget->findText("Store"));
    fromGodownWidget->setEditable(false);


    toGodownWidget = new GMComboBox(&voucher->toGodownID);
    toGodownWidget->getGodownList();
    toGodownWidget->removeItem(toGodownWidget->findText("Store"));


    addToExtrasLayout2(new QLabel("Store"));
    addToExtrasLayout2(fromGodownWidget);
    addToExtrasLayout2(new QLabel("Section"));
    addToExtrasLayout2(toGodownWidget);
    extrasLayout2->addStretch(1);

    mainLedgerWidget->hide();

}

void StockRequestEditor::setWidgets()
{

//    QLabel *windowLabel = new QLabel(windowTitle);
//    appBar = new QtMaterialAppBar();
//    appBar->setBackgroundColor(QColor());

//    backButton = new QtMaterialIconButton(QtMaterialTheme::icon("navigation", "arrow_back"));
//    backButton->setParent(appBar);
//    backButton->setColor(QColor(245,0,87));
//    backButton->setIconSize(QSize(30,30));


////    appBar->setParent(this);
//    appBar->appBarLayout()->addWidget(backButton);
//    appBar->appBarLayout()->addWidget(windowLabel);
//    appBar->appBarLayout()->addStretch(1);
//    QPalette palette = windowLabel->palette();
//    palette.setColor(windowLabel->foregroundRole(), Qt::white);
//    windowLabel->setPalette(palette);

//    windowLabel->setFont(QFont("Roboto", 18, QFont::Normal));

//    QVBoxLayout *layout = new QVBoxLayout;
//    QWidget *canvas = new QWidget;

//    appBar->setMinimumHeight(50);
//    appBar->setMaximumHeight(300);

//    layout->setContentsMargins(0, 0, 0, 0);

//    layout = new QVBoxLayout;
//    canvas->setLayout(layout);
//    canvas->setMaximumHeight(60);
//    layout->addWidget(appBar);
//    layout->addStretch(1);




//    QPointer<QWidget> Widget = new QWidget(this);
//    QPointer<QVBoxLayout> mainLayout = new QVBoxLayout(Widget);
//    Widget->setLayout(mainLayout);

//    QPointer<QHBoxLayout> voucherData = new QHBoxLayout;
//    QPointer<QHBoxLayout> extrasLayout = new QHBoxLayout;
//    QPointer<QVBoxLayout> tablesLayout = new QVBoxLayout;

////    voucherData->setContentsMargins(20,0,0,0);
//    mainLayout->addWidget(canvas);
//    mainLayout->addLayout(voucherData);
//    mainLayout->addLayout(extrasLayout);
//    mainLayout->addLayout(tablesLayout);

//    tablesLayout->addWidget(DataTableWidget);
//    mainLayout->addWidget(voucherNarrationWidget);


//    voucherData->addWidget(new QLabel("Date"));
//    voucherData->addWidget(voucherDateWidget);
//    voucherData->addWidget(new QLabel("Voucher No"));
//    voucherData->addWidget(voucherNumberWidget);


//    voucherData->addStretch(1);
//    voucherData->addWidget(SalesmanSearchBox);

////    ui->verticalLayout_2->addWidget(Widget);

//    ui->verticalLayout->addWidget(Widget);

}

void StockRequestEditor::resetWidgets()
{
    delete model;

    voucherDateWidget->setVariable(&voucher->VoucherDate);
    voucherNumberWidget->setVariable(&voucher->voucherNumber);
    voucherNarrationWidget->setVariable(&voucher->narration);

    fromGodownWidget->setValue(&voucher->fromGodownID);
    toGodownWidget->setValue(&voucher->toGodownID);
    SalesmanSearchBox->setText(userHelper->getEmployeeNameFromID(voucher->SalesmanID));

    qDebug()<<"voucher type"<<voucher->voucherType;
    model = new GMStockRequestModel(voucher,lis.size(),1,this);
    model->setHeaderStrings(lis);
    DataTableWidget->setVoucher(voucher);
    DataTableWidget->setModel(model);
    DataTableWidget->setColumns();
}

void StockRequestEditor::saveVoucher()
{

    if(voucher->fromGodownID == ""){
        snackBar->addMessage("From Location not selected!!!");
        return;
    }

    if(voucher->toGodownID == ""){
        snackBar->addMessage("To Location not selected!!!");
        return;

    }

    if(voucher->InventoryItems.size()<1){
        snackBar->addMessage("No Items Found!!!");
        return;
    }

    if(voucher->status == QuotationStatus::Started){
        voucher->status = QuotationStatus::Created;
        dbHelper->insertVoucher(voucher);
    }
    else if(voucher->status != QuotationStatus::Started ){
        qDebug()<<"XXXXXXXXXXXXXXXXXXXXx";
        dbHelper->updateVoucher(voucher,voucher->voucherNumber);
    }

    if(voucher->fromExternal){
        emit NotificationAccepted();
    }

    closeEditor();
}

//void StockRequestEditor::deleteVoucher()
//{
//    if(true && voucher->status == QuotationStatus::SalesInvoiceRaised)    //Check Authority
//    {

//        dbHelper->deleteVoucher(voucher);
//    }
//    else{
//        //TODO snackbar
//        snackBar->addInstantMessage("Cannot Delete");
//    }
//}

void StockRequestEditor::askForPrint()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Print Voucher","Would You Like to Print?",
                                  QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::Yes){
        printVoucher();
    }
}

void StockRequestEditor::printVoucher()
{
    printHelper = new PrinterHelper();
    printHelper->PrintIssueRequestVoucher(voucher);
}

void StockRequestEditor::importVoucher(GeneralVoucherDataObject *voucher2)
{
    setVoucher(voucher2);
    showVoucher();
}

void StockRequestEditor::importItemsFromVoucher(GeneralVoucherDataObject *voucher2)
{
    voucher->InventoryItems.append(voucher2->InventoryItems);
    resetWidgets();
}



void StockRequestEditor::sendEmail()
{

}

void StockRequestEditor::importWorkOrder(GeneralVoucherDataObject *voucher2)
{

    voucher->InventoryItems = voucher2->InventoryItems;
    voucher->fromGodownID = "Godown";
    voucher->toGodownID = voucher2->toGodownID;
    voucher->RequirementVoucherNo = voucher2->RequirementVoucherNo;
    voucher->TransactionId = voucher2->TransactionId;
    qDebug()<<"change to"<<voucher->voucherType;
    resetWidgets();

}

void StockRequestEditor::showVoucher()
{
//    delete  voucher;
//    voucher = new GeneralVoucherDataObject();
//    voucher = obj;
    resetWidgets();
    qDebug()<<"from external"<<voucher->fromExternal;
    qDebug()<<"Num of items"<<voucher->InventoryItems.size();
}

//void StockRequestEditor::closeEditor()
//{
//    emit closing();
//    this->close();
//}

