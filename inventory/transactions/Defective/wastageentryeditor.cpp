#include "wastageentryeditor.h"
//#include "ui_wastageentryeditor.h"



WastageEntryEditor::WastageEntryEditor(VoucherDBAbstract *db, VoucherEditorBase *parent) :
    VoucherEditorBase(parent)
{
//    ui->setupUi(this);

    this->setWindowTitle(windowTitle);
    this->setObjectName("background");
    this->setStyleSheet("QWidget#background{background-color:#e0f7fa}");


    this->dbHelper = db;
//    initDbHelpers();
    initVoucher();

    initGlobalObjects();

//    setWidgets();

    initExtraActions();

}

WastageEntryEditor::~WastageEntryEditor()
{
//    delete ui;
}

void WastageEntryEditor::initExtraActions()
{
    contactButton->hide();
    moreButton->hide();


    QObject::connect(saveButton,&QPushButton::clicked,
                           this,&WastageEntryEditor::saveVoucher);

}


void WastageEntryEditor::initVoucher()
{
    editorStatus = EditorStatus::NewVoucher;

    voucher = new GeneralVoucherDataObject();
    voucher->VoucherDate = QDate::currentDate();
    voucher->VoucherPrefix = LoginValues::voucherPrefix;
    voucher->voucherType = GMVoucherTypes::getVoucherString(GMVoucherTypes::DefectiveVoucher);
    voucher->voucherNumber = dbHelper->getNextVoucherNo(voucher->VoucherPrefix);
    qDebug()<<voucher->voucherNumber;
    voucher->status = QuotationStatus::Started;
    voucher->fromGodownID = godownHelper->getDefaultGodown();

    setTitle(voucher->voucherType);
    voucherDateWidget->setVariable(&voucher->VoucherDate);
    voucherNumberWidget->setVariable(&voucher->voucherNumber);

}

void WastageEntryEditor::initGlobalObjects()
{

    lis<<"#"<<"Particulars"<<"UOM"<<"Quantity"<<"Amount";
    DataTableWidget = new StockEntryTableView(voucher);
    godownWidget->setGodown(&voucher->fromGodownID);
    connect(godownWidget, &GMGodownLineMaterial::itemSelectedwithItem, this,
            [=](QString godownID) {
        qDebug()<<"changed godown"<<godownID<<voucher->fromGodownID;
        voucher->fromGodownID = (godownID);
        for(int i=0;i<voucher->InventoryItems.size();  i++){
            if(voucher->InventoryItems[i].BaseItem.godownList.size() == 0)
                voucher->InventoryItems[i].BaseItem.fromGodownID = voucher->fromGodownID;
            else if (voucher->InventoryItems[i].BaseItem.godownList.size() == 1) {
                voucher->InventoryItems[i].BaseItem.godownList[0].fromGodown = voucher->fromGodownID;
            }

        }
    });

    godownWidget->hide();

    StockEntryDelegate *itemDelegate = new StockEntryDelegate(
                itemsHelper,itemsHelper->getInventoryItemsQueryModel(),
                           uomHelper);
    connect(itemDelegate, &StockEntryDelegate::showItemDetailClicked,
            this, &WastageEntryEditor::showItemDetail);

    DataTableWidget->setItemDelegate(itemDelegate);
    model = new GMWastageEntryModel(voucher,lis.size(),this);
    model->setHeaderStrings(lis);
    DataTableWidget->setModel(model);
    DataTableWidget->setColumns();
    setTableLayout(DataTableWidget);
    ReasonWidget->show();
    mainLedgerWidget->hide();

//    connect(model,SIGNAL(updateValues()),DataTableWidget,SLOT(setValues()));
//    connect(model,SIGNAL(newLineAdded(int)),DataTableWidget,SLOT(setFocusAfterInsert(int)));


}

//void WastageEntryEditor::setWidgets()
//{

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
////    voucherData->addWidget(mainLedgerWidget);
//    voucherData->addStretch(1);

//    voucherData->addWidget(godownWidget);
//    voucherData->addWidget(SalesmanSearchBox);

//    QStringList reasonList;
//    reasonList<<"Used in kitchen"<<"Expiry"<<"Rejected as not per order"<<"Rejected as not per order";
//    reasonList<<"Too much stock"<<"Packing damage"<<"Production Damage"<<"Others";
//    ReasonWidget = new GMMaterialComboBox(&voucher->QuotationDroppedReason, reasonList);
//    ReasonWidget->setLabel("Reason");

//    voucherData->addWidget(ReasonWidget);


////    ui->verticalLayout_2->addWidget(Widget);

//    ui->verticalLayout->addWidget(Widget);

//}

void WastageEntryEditor::resetWidgets()
{
    delete model;


    qDebug()<<"Voucher No : "<<voucher->voucherNumber;
    voucherDateWidget->setVariable(&voucher->VoucherDate);
    voucherNumberWidget->setVariable(&voucher->voucherNumber);
    voucherNarrationWidget->setVariable(&voucher->narration);
    qDebug()<<voucher->QuotationDroppedReason;
    ReasonWidget->setVariable(&voucher->QuotationDroppedReason);
    SalesmanSearchBox->setText(userHelper->getEmployeeNameFromID(voucher->SalesmanID));
//    mainLedgerWidget->setText(voucher->ledgerObject.LedgerName);

    model = new GMWastageEntryModel(voucher,lis.size(),this);
    model->setHeaderStrings(lis);
    DataTableWidget->setVoucher(voucher);
    DataTableWidget->setModel(model);
    DataTableWidget->setColumns();
}

void WastageEntryEditor::saveVoucher()
{
    if(voucher->InventoryItems.size()<1){
        snackBar->addMessage("No Items Found!!!");
        return;
    }


    if(voucher->status == QuotationStatus::Started){
        voucher->status = QuotationStatus::SalesInvoiceRaised;
        dbHelper->insertVoucher(voucher);
    }
    else{
        dbHelper->updateVoucher(voucher,voucher->voucherNumber);
    }

    closeEditor();
}


void WastageEntryEditor::askForPrint()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Print Voucher","Would You Like to Print?",
                                  QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::Yes){
        printVoucher();
    }
}

void WastageEntryEditor::printVoucher()
{
    printHelper = new PrinterHelper();
//    printHelper->PrintInventoryVoucher(voucher);
}

void WastageEntryEditor::importVoucher(GeneralVoucherDataObject *voucher2)
{

}

void WastageEntryEditor::importItemsFromVoucher(GeneralVoucherDataObject *voucher2)
{

}

void WastageEntryEditor::sendEmail()
{

}

void WastageEntryEditor::showVoucher()
{
//    delete  voucher;
//    voucher = new GeneralVoucherDataObject();
//    voucher = obj;

    VoucherEditorBase::showVoucher();
    qDebug()<<"status : "<<voucher->status;
    resetWidgets();
}


void WastageEntryEditor::showItemDetail(int row)
{
    qDebug()<<Q_FUNC_INFO;
    ItemDetail *itemDetailWid = new ItemDetail(voucher, row,1,itemsHelper,
            uomHelper,new PricelistDatabaseHelper,godownHelper, voucher->priceListId);
    itemDetailWid->show();

    connect(itemDetailWid, &ItemDetail::updateItem, this, [=](){
        model->setVoucher(voucher);
    });
}
