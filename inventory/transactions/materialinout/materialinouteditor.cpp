#include "materialinouteditor.h"
#include "ui_materialinouteditor.h"

MaterialInOutEditor::MaterialInOutEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MaterialInOutEditor)
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

MaterialInOutEditor::~MaterialInOutEditor()
{
    delete ui;
}


void MaterialInOutEditor::initExtraActions()
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
                           this,&MaterialInOutEditor::closeEditor);
    QObject::connect(saveButton,&QPushButton::clicked,
                           this,&MaterialInOutEditor::saveVoucher);
    QObject::connect(shareButton,&QPushButton::clicked,
                           drawer,&QtMaterialDrawer::openDrawer);
    QObject::connect(printButton,&QPushButton::clicked,
                           this,&MaterialInOutEditor::askForPrint);
}

void MaterialInOutEditor::initDbHelpers()
{

    dbHelper = new materialinoutDatabaseHelper;
    uomHelper = new UomDataBaseHelper;
    userHelper = new UserProfileDatabaseHelper;
    itemsHelper = new SalesInventoryItemDatabaseHelper;
}

void MaterialInOutEditor::initVoucher()
{
    voucher = new GeneralVoucherDataObject();
    voucher->VoucherDate = QDate::currentDate();
    voucher->VoucherPrefix = LoginValues::voucherPrefix;
    voucher->voucherType = GMVoucherTypes::getVoucherString(GMVoucherTypes::DefectiveVoucher);
    voucher->voucherNumber = dbHelper->getNextVoucherNo(voucher->VoucherPrefix);
    voucher->status = QuotationStatus::Started;

}

void MaterialInOutEditor::initGlobalObjects()
{

    lis<<"#"<<"Particulars"<<"UOM"<<"Consumed"<<"Produced";
    DataTableWidget = new StockEntryTableView(voucher);
    qDebug()<<"Init table";
    SalesmanSearchBox = new GMEmployeeLineMaterial(userHelper,userHelper->getEmployeeQueryForSearch());
    SalesmanSearchBox->setLabel("SalesMan Name");
    SalesmanSearchBox->setParent(this);
    SalesmanSearchBox->setStyleSheet("GMEmployeeSearchLite{background-color:#e0f7fa;}");
    connect(SalesmanSearchBox,&GMEmployeeLineMaterial::itemSelectedwithItem,
            this,[=](UserProfileDataModel emp){
        voucher->SalesmanID = emp._id;
    });



    DataTableWidget->setItemDelegate(new StockJournalDelegate(
                              itemsHelper,itemsHelper->getInventoryItemsQueryModel(),
                                         uomHelper));
    qDebug()<<"Init Model 118";
    model = new GMStockJournalModel(voucher,lis.size(),this);
    qDebug()<<"121";
    model->setHeaderStrings(lis);
    qDebug()<<"model Set";
    DataTableWidget->setModel(model);
    qDebug()<<"Setting cols";
    DataTableWidget->setColumns();
    qDebug()<<"Init Model 125";
//    connect(model,SIGNAL(updateValues()),DataTableWidget,SLOT(setValues()));
//    connect(model,SIGNAL(newLineAdded(int)),DataTableWidget,SLOT(setFocusAfterInsert(int)));

    voucherDateWidget = new GMDateEdit(&voucher->VoucherDate);
    voucherNumberWidget = new GMLineEdit(&voucher->voucherNumber);
    voucherNarrationWidget = new GMPlainTextEdit(&voucher->narration);
    voucherNarrationWidget->setPlaceholderText("Narration");
    voucherNumberWidget->setReadOnly(true);
    qDebug()<<"133";

}

void MaterialInOutEditor::setWidgets()
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

//    voucherData->setContentsMargins(20,0,0,0);
    mainLayout->addWidget(canvas);
    mainLayout->addLayout(voucherData);
    mainLayout->addLayout(extrasLayout);
    mainLayout->addLayout(tablesLayout);

    tablesLayout->addWidget(DataTableWidget);
    mainLayout->addWidget(voucherNarrationWidget);


    voucherData->addWidget(new QLabel("Date"));
    voucherData->addWidget(voucherDateWidget);
    voucherData->addWidget(new QLabel("Voucher No"));
    voucherData->addWidget(voucherNumberWidget);
//    voucherData->addWidget(mainLedgerWidget);
    voucherData->addStretch(1);
    voucherData->addWidget(SalesmanSearchBox);

//    ui->verticalLayout_2->addWidget(Widget);

    ui->verticalLayout->addWidget(Widget);

}

void MaterialInOutEditor::resetWidgets()
{
    delete model;


    voucherDateWidget->setVariable(&voucher->VoucherDate);
    voucherNumberWidget->setVariable(&voucher->voucherNumber);
    voucherNarrationWidget->setVariable(&voucher->narration);
//    mainLedgerWidget->setText(voucher->ledgerObject.LedgerName);
    SalesmanSearchBox->setText(userHelper->getEmployeeNameFromID(voucher->SalesmanID));

    model = new GMStockJournalModel(voucher,lis.size(),this);
    model->setHeaderStrings(lis);
    DataTableWidget->setVoucher(voucher);
    DataTableWidget->setModel(model);
    DataTableWidget->setColumns();
}

void MaterialInOutEditor::saveVoucher()
{
    if(voucher->InventoryItems.size()<1){
        snackBar->addMessage("No Items Found!!!");
        return;
    }


    if(voucher->status == QuotationStatus::Started){
        voucher->status = QuotationStatus::SalesInvoiceRaised;
        dbHelper->insertMaterialInOut(voucher);
    }
    else{
        qDebug()<<"update**********";
        dbHelper->updateMaterialInOut(voucher,voucher->voucherNumber);
    }

    closeEditor();
}

void MaterialInOutEditor::deleteVoucher()
{
    if(true && voucher->status == QuotationStatus::SalesInvoiceRaised)    //Check Authority
    {
        qDebug()<<"delete**********";

        dbHelper->deleteMaterialInOut(voucher);
    }
    else{
        //TODO snackbar
    }
}

void MaterialInOutEditor::askForPrint()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Print Voucher","Would You Like to Print?",
                                  QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::Yes){
        printVoucher();
    }
}

void MaterialInOutEditor::printVoucher()
{
    printHelper = new PrinterHelper();
//    printHelper->PrintInventoryVoucher(voucher);
}

void MaterialInOutEditor::importVoucher(GeneralVoucherDataObject *voucher2)
{

}

void MaterialInOutEditor::importItemsFromVoucher(GeneralVoucherDataObject *voucher2)
{

}

void MaterialInOutEditor::sendEmail()
{

}

void MaterialInOutEditor::showVoucher(GeneralVoucherDataObject *obj)
{
    delete  voucher;
    voucher = new GeneralVoucherDataObject();
    voucher = obj;
    qDebug()<<"Status : "<<voucher->status;
    resetWidgets();
}

void MaterialInOutEditor::closeEditor()
{
    emit closing();
    this->close();
}
