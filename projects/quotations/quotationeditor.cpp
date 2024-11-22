#include "quotationeditor.h"
#include "ui_quotationeditor.h"


QuotationEditor::QuotationEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QuotationEditor)
{
    ui->setupUi(this);

    this->setWindowTitle(windowTitle);
    this->setObjectName("background");
    this->setStyleSheet("QWidget#background{background-color:#e0f7fa}");


    initDbHelpers();
    initVoucher();
    initGlobalObjects();
    setWidgets();
    initExtraActions();

}

QuotationEditor::~QuotationEditor()
{
    delete ui;
}

void QuotationEditor::initExtraActions()
{
    saveButton = new QtMaterialFloatingActionButton(QtMaterialTheme::icon("content", "save"));
    saveButton->setMini(true);
    saveButton->setXOffset(10);
    saveButton->setYOffset(10);
    saveButton->setParent(this);
    saveButton->setToolTip("Save Voucher");

    shareButton = new QtMaterialFloatingActionButton(QtMaterialTheme::icon("social", "share"));
    shareButton->setMini(true);
    shareButton->setBackgroundColor(QColor(25,150,40));
    shareButton->setCorner(Qt::TopRightCorner);
    shareButton->setParent(this);
    shareButton->setXOffset(45);
    shareButton->setYOffset(5);
    shareButton->hide();


    snackBar = new QtMaterialSnackbar(this);
    drawer = new QtMaterialDrawer(this);
    drawer->setClickOutsideToClose(true);
    drawer->setOverlayMode(true);

    QVBoxLayout *drawerLayout = new QVBoxLayout;
    drawer->setDrawerLayout(drawerLayout);
    printButton = new QtMaterialIconButton(QtMaterialTheme::icon("action", "print"));
    shareButton->setBackgroundColor(QColor(255,250,0));
    shareButton->setForegroundColor(QColor(0,250,0));
    appBar->appBarLayout()->addWidget(printButton);

    QObject::connect(backButton,&QPushButton::clicked,
                           this,&QuotationEditor::closeEditor);
    QObject::connect(saveButton,&QPushButton::clicked,
                           this,&QuotationEditor::saveVoucher);
    QObject::connect(shareButton,&QPushButton::clicked,
                           drawer,&QtMaterialDrawer::openDrawer);
    QObject::connect(printButton,&QPushButton::clicked,
                           this,&QuotationEditor::askForPrint);
}

void QuotationEditor::initDbHelpers()
{
    ledgerHelper = new LedgerMasterDatabaseHelper;
    dbHelper = new ProformaDatabaseHelper;
    uomHelper = new UomDataBaseHelper;
    userHelper = new UserProfileDatabaseHelper;
    itemsHelper = new SalesInventoryItemDatabaseHelper;
}

void QuotationEditor::initVoucher()
{
    voucher = new GeneralVoucherDataObject();
    voucher->VoucherDate = QDate::currentDate();
    voucher->VoucherPrefix = LoginValues::voucherPrefix;
    voucher->voucherType = GMVoucherTypes::getVoucherString(GMVoucherTypes::Proforma);
    voucher->voucherNumber = dbHelper->getNextVoucherNo(voucher->VoucherPrefix);
    voucher->status = QuotationStatus::Started;

}

void QuotationEditor::initGlobalObjects()
{

    lis<<"#"<<"Particulars"<<"UOM"<<"Quantity"<<"Price"<<"Tax"<<"Tax Amount"<<"Amount";
    DataTableWidget = new InventoryVoucherTableView(voucher);


    SalesmanSearchBox = new GMEmployeeLineMaterial(userHelper,userHelper->getEmployeeQueryForSearch());
    SalesmanSearchBox->setLabel("SalesMan Name");
    SalesmanSearchBox->setParent(this);
    SalesmanSearchBox->setStyleSheet("GMEmployeeSearchLite{background-color:#e0f7fa;}");
    connect(SalesmanSearchBox,&GMEmployeeLineMaterial::itemSelectedwithItem,
            this,[=](UserProfileDataModel emp){
        voucher->SalesmanID = emp._id;
    });

    mainLedgerWidget = new GMLedgerLineMaterial(ledgerHelper,
                                                ledgerHelper->getLedgerQueryString(),
                                                &voucher->ledgerObject
                                                );
    mainLedgerWidget->setLabel("Customer Name");
    mainLedgerWidget->setParent(this);
    mainLedgerWidget->setStyleSheet("GMLedgerLineMaterial{background-color:#e0f7fa;}");


    DataTableWidget->setItemDelegate(new GMInventoryTransactionDelegate(
                              itemsHelper,itemsHelper->getInventoryItemsQueryModel(),
                                         uomHelper));

    model = new GMInventoryItemModel(voucher,1,this);
//    model->setHeaderStrings(lis);
    DataTableWidget->setModel(model);
    DataTableWidget->setColumns();
    connect(mainLedgerWidget,&GMLedgerLineMaterial::itemSelectedwithItem,
            this,[=](LedgerMasterDataModel led){
        voucher->ledgerObject = led;
        if(led.LedgerID.length()<1)
            return;
        mainLedgerWidget->clearFocus();
        //        voucher->priceListId = priceHelper->getDefaultPriceList(led.LedgerID);
        //        voucher->priceListName = priceHelper->getPriceListNameByID(voucher->priceListId);
        //        priceListWidget->setVariable(&voucher->priceListName);
        //        model->setVoucher(voucher);
        DataTableWidget->setEnabled(true);
        DataTableWidget->clearFocus();
        DataTableWidget->setFocusToFirstRow();
    });
//    connect(model,SIGNAL(updateValues()),DataTableWidget,SLOT(setValues()));
//    connect(model,SIGNAL(newLineAdded(int)),DataTableWidget,SLOT(setFocusAfterInsert(int)));

    voucherDateWidget = new GMDateEdit(&voucher->VoucherDate);
    voucherNumberWidget = new GMLineEdit(&voucher->voucherNumber);
    voucherNarrationWidget = new GMPlainTextEdit(&voucher->narration);
    voucherNarrationWidget->setPlaceholderText("Narration");
    voucherNumberWidget->setReadOnly(true);

    listView = new QListView(this);

    ListProxyModel* listModel = new ListProxyModel(voucher);
    listModel->setSourceModel(model);
    listView->setModel(listModel);


//    listView->setModelColumn(1);

    listView->setItemDelegate(new GMInventoryListItemDelegate());


    listView->setObjectName("EditInvList");
    listView->setStyleSheet("QListView::item{"
                        "gridline-color : #ffffff;"
                        "border: 0px solid #fffff8;"
                        "}"
                        "QListView#EditInvList{"
                        "border-bottom : 2px solid black;"
                        "background-image: url(:/images/watermark.png);"
                        "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0.617, y2:0, stop:0 #e0f2f1, stop:1 #80cbc4);"
                        "background-repeat: no-repeat;"
                        "background-position: center;"
                        "selection-color: black;"
                        "}"
                        "QHeaderView::section:horizontal"
                        "{"
                            "background-color: QLinearGradient(x1:0, y1:0, x2:0, y2:1, stop:0 #667db6, stop: 0.5 #0082c8, stop: 0.6 #0082c8, stop:1 #667db6);"
                            "color: white;"
                            "border: 1px solid #ffffff;"
                        "}"
                        );



}

void QuotationEditor::setWidgets()
{

    QLabel *windowLabel = new QLabel(windowTitle);
    appBar = new QtMaterialAppBar();
    appBar->setBackgroundColor(QColor());

    backButton = new QtMaterialIconButton(QtMaterialTheme::icon("navigation", "arrow_back"));
    backButton->setParent(appBar);
    backButton->setColor(QColor(245,0,87));
    backButton->setIconSize(QSize(30,30));


//    appBar->setParent(this);
    appBar->appBarLayout()->addWidget(backButton);
    appBar->appBarLayout()->addWidget(windowLabel);
    appBar->appBarLayout()->addStretch(1);
    QPalette palette = windowLabel->palette();
    palette.setColor(windowLabel->foregroundRole(), Qt::white);
    windowLabel->setPalette(palette);

    windowLabel->setFont(QFont("Roboto", 18, QFont::Normal));

    QVBoxLayout *layout = new QVBoxLayout;
    QWidget *canvas = new QWidget;

    appBar->setMinimumHeight(50);
    appBar->setMaximumHeight(300);

    layout->setContentsMargins(0, 0, 0, 0);

    layout = new QVBoxLayout;
    canvas->setLayout(layout);
    canvas->setMaximumHeight(60);
    layout->addWidget(appBar);
    layout->addStretch(1);




    QPointer<QWidget> Widget = new QWidget(this);
    QPointer<QVBoxLayout> mainLayout = new QVBoxLayout(Widget);
    Widget->setLayout(mainLayout);

    QPointer<QHBoxLayout> voucherData = new QHBoxLayout;
    QPointer<QHBoxLayout> extrasLayout = new QHBoxLayout;
    QPointer<QVBoxLayout> tablesLayout = new QVBoxLayout;

    QSplitter *splitter = new QSplitter(this);

    splitter->setOrientation(Qt::Vertical);
    splitter->addWidget(DataTableWidget);
    splitter->addWidget(listView);
    splitter->setCollapsible(1,true);

//    voucherData->setContentsMargins(20,0,0,0);
    mainLayout->addWidget(canvas);
    mainLayout->addLayout(voucherData);
    mainLayout->addLayout(extrasLayout);
    mainLayout->addLayout(tablesLayout);


    tablesLayout->addWidget(splitter);

    mainLayout->addWidget(voucherNarrationWidget);


    voucherData->addWidget(new QLabel("Date"));
    voucherData->addWidget(voucherDateWidget);
    voucherData->addWidget(new QLabel("Voucher No"));
    voucherData->addWidget(voucherNumberWidget);
    voucherData->addWidget(mainLedgerWidget);
    voucherData->addStretch(1);
    voucherData->addWidget(SalesmanSearchBox);

//    ui->verticalLayout_2->addWidget(Widget);

    ui->verticalLayout->addWidget(Widget);

}

void QuotationEditor::resetWidgets()
{
    delete model;


    voucherDateWidget->setVariable(&voucher->VoucherDate);
    voucherNumberWidget->setVariable(&voucher->voucherNumber);
    voucherNarrationWidget->setVariable(&voucher->narration);
    mainLedgerWidget->setItem(&voucher->ledgerObject);
    SalesmanSearchBox->setText(userHelper->getEmployeeNameFromID(voucher->SalesmanID));

    model = new GMInventoryItemModel(voucher,1,this);
//    model->setHeaderStrings(lis);
    DataTableWidget->setVoucher(voucher);
    DataTableWidget->setModel(model);
    DataTableWidget->setColumns();

    ListProxyModel* listModel = new ListProxyModel(voucher);
    listModel->setSourceModel(model);
    qDebug()<<"SHABIB "<<listModel->rowCount(QModelIndex());
    listView->setModel(listModel);

}

void QuotationEditor::saveVoucher()
{
    if(voucher->InventoryItems.size()<1){
        snackBar->addMessage("No Items Found!!!");
        return;
    }

    if(voucher->ledgerObject.LedgerID.length()<1){
        snackBar->addMessage("Please Set Ledger");
        return;
    }


    if(voucher->status == QuotationStatus::Started){
        voucher->status = QuotationStatus::SalesInvoiceRaised;
//        dbHelper->insertProforma(voucher);
    }
    else{
//        dbHelper->updateProforma(voucher,voucher->voucherNumber);
    }

    closeEditor();
}

void QuotationEditor::deleteVoucher()
{
    if(true && voucher->status == QuotationStatus::SalesInvoiceRaised)    //Check Authority
    {

//        dbHelper->deleteProforma(voucher);
    }
    else{
        //TODO snackbar
    }
}

void QuotationEditor::askForPrint()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Print Voucher","Would You Like to Print?",
                                  QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::Yes){
        printVoucher();
    }
}

void QuotationEditor::printVoucher()
{
    printHelper = new PrinterHelper();
    printHelper->PrintInventoryVoucher(voucher);
}

void QuotationEditor::importVoucher(GeneralVoucherDataObject *voucher2)
{

}

void QuotationEditor::importItemsFromVoucher(GeneralVoucherDataObject *voucher2)
{

}

void QuotationEditor::sendEmail()
{

}

void QuotationEditor::showVoucher(GeneralVoucherDataObject *obj)
{
    delete  voucher;
    voucher = new GeneralVoucherDataObject();
    voucher = obj;
    TransactionCalculator::VoucherCalculateSales(voucher);
    resetWidgets();
}

void QuotationEditor::closeEditor()
{
    emit closing();
    this->close();
}
