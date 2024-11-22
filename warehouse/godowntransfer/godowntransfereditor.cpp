#include "godowntransfereditor.h"
#include "ui_godowntransfereditor.h"

GodownTransferEditor::GodownTransferEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GodownTransferEditor)
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

GodownTransferEditor::~GodownTransferEditor()
{
    delete ui;
}


void GodownTransferEditor::initExtraActions()
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
                           this,&GodownTransferEditor::closeEditor);
    QObject::connect(saveButton,&QPushButton::clicked,
                           this,&GodownTransferEditor::saveVoucher);
    QObject::connect(shareButton,&QPushButton::clicked,
                           drawer,&QtMaterialDrawer::openDrawer);
    QObject::connect(printButton,&QPushButton::clicked,
                           this,&GodownTransferEditor::askForPrint);
}

void GodownTransferEditor::initDbHelpers()
{

    dbHelper = new GodownTransferDatabaseHelper;
    uomHelper = new UomDataBaseHelper;
    userHelper = new UserProfileDatabaseHelper;
    itemsHelper = new SalesInventoryItemDatabaseHelper;
    godownHelper = new GodownDatabaseHelper;
}

void GodownTransferEditor::initVoucher()
{
    voucher = new GeneralVoucherDataObject();
    voucher->VoucherDate = QDate::currentDate();
    voucher->VoucherPrefix = LoginValues::voucherPrefix;
    voucher->voucherType = GMVoucherTypes::getVoucherString(GMVoucherTypes::GodownTransfer);
    voucher->voucherNumber = dbHelper->getNextVoucherNo(voucher->VoucherPrefix);
    voucher->status = QuotationStatus::Started;

    voucher->fromGodownID = godownHelper->getDefaultGodown();
    voucher->toGodownID = godownHelper->getDefaultGodown();

}

void GodownTransferEditor::initGlobalObjects()
{

    lis<<"#"<<"Particulars"<<"UOM"<<"Qty"<<"Rate";
    DataTableWidget = new StockEntryTableView(voucher);
    qDebug()<<"Init table";
    SalesmanSearchBox = new GMEmployeeSearchWidget(userHelper,userHelper->getEmployeeQueryForSearch());

    FromGodown = new GMGodownLineMaterial(godownHelper,godownHelper->getGodownForSearchModel(),&voucher->fromGodownID);
    ToGodown = new GMGodownLineMaterial(godownHelper,godownHelper->getGodownForSearchModel(),&voucher->toGodownID);

    FromGodown->setLabel("From Godown");
    ToGodown->setLabel("To Godown");

    DataTableWidget->hideColumn(4);

    DataTableWidget->setItemDelegate(new StockJournalDelegate(
                              itemsHelper,itemsHelper->getInventoryItemsQueryModel(),
                                         uomHelper));

    model = new GMGodownTransferModel(voucher,lis.size(),this);
    model->setHeaderStrings(lis);
    DataTableWidget->setModel(model);
    DataTableWidget->setColumns();

//    connect(model,SIGNAL(updateValues()),DataTableWidget,SLOT(setValues()));
//    connect(model,SIGNAL(newLineAdded(int)),DataTableWidget,SLOT(setFocusAfterInsert(int)));

    voucherDateWidget = new GMDateEdit(&voucher->VoucherDate);
    voucherNumberWidget = new GMLineEdit(&voucher->voucherNumber);
    voucherNarrationWidget = new GMPlainTextEdit(&voucher->narration);
    voucherNarrationWidget->setPlaceholderText("Narration");
    voucherNumberWidget->setReadOnly(true);
    qDebug()<<"133";

}

void GodownTransferEditor::setWidgets()
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

    voucherData->addWidget(FromGodown);
    voucherData->addWidget(ToGodown);
    voucherData->addStretch(1);

//    ui->verticalLayout_2->addWidget(Widget);

    ui->verticalLayout->addWidget(Widget);

}

void GodownTransferEditor::resetWidgets()
{
    delete model;


    voucherDateWidget->setVariable(&voucher->VoucherDate);
    voucherNumberWidget->setVariable(&voucher->voucherNumber);
    voucherNarrationWidget->setVariable(&voucher->narration);
//    mainLedgerWidget->setText(voucher->ledgerObject.LedgerName);

    model = new GMGodownTransferModel(voucher,lis.size(),this);
    model->setHeaderStrings(lis);
    DataTableWidget->setVoucher(voucher);
    DataTableWidget->setModel(model);
    DataTableWidget->setColumns();
    FromGodown->setGodown(&voucher->fromGodownID);
    ToGodown->setGodown(&voucher->toGodownID);
}

void GodownTransferEditor::saveVoucher()
{
    if(voucher->InventoryItems.size()<1){
        snackBar->addMessage("No Items Found!!!");
        return;
    }


    if(voucher->status == QuotationStatus::Started){
        voucher->status = QuotationStatus::SalesInvoiceRaised;
        dbHelper->insertGodownTransfer(voucher);
    }
    else{
        dbHelper->updateGodownTransfer(voucher,voucher->voucherNumber);
    }

    closeEditor();
}

void GodownTransferEditor::deleteVoucher()
{
    if(true && voucher->status == QuotationStatus::SalesInvoiceRaised)    //Check Authority
    {

        dbHelper->deleteGodownTransfer(voucher);
    }
    else{
        //TODO snackbar
    }
}

void GodownTransferEditor::askForPrint()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Print Voucher","Would You Like to Print?",
                                  QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::Yes){
        printVoucher();
    }
}

void GodownTransferEditor::printVoucher()
{
    printHelper = new PrinterHelper();
//    printHelper->PrintInventoryVoucher(voucher);
}

void GodownTransferEditor::importVoucher(GeneralVoucherDataObject *voucher2)
{

}

void GodownTransferEditor::importItemsFromVoucher(GeneralVoucherDataObject *voucher2)
{

}

void GodownTransferEditor::sendEmail()
{

}

void GodownTransferEditor::showVoucher(GeneralVoucherDataObject *obj)
{
    delete  voucher;
    voucher = new GeneralVoucherDataObject();
    voucher = obj;
    qDebug()<<"Status : "<<voucher->status;
    resetWidgets();
}

void GodownTransferEditor::closeEditor()
{
    emit closing();
    this->close();
}
